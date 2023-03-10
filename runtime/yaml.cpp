#include <yaml-cpp/yaml.h>

#include "runtime/optional.h"
#include "runtime/streams.h"
#include "runtime/critical_section.h"
#include "runtime/yaml.h"

static void yaml_node_to_mixed(const YAML::Node &node, mixed &data, const string &source) noexcept {
  data.clear(); // sets data to NULL
  if (node.IsScalar()) {
    const string string_data(node.as<std::string>().c_str());
    const bool string_data_has_quotes = (source[node.Mark().pos] == '"' && source[node.Mark().pos + string_data.size() + 1] == '"');
    if (string_data_has_quotes) {
      data = string_data;
    } else if (string_data == string("true") || string_data == string("false")) {
      data = (string_data == string("true"));
    } else if (string_data.is_int()) {
      data = string_data.to_int();
    } else {
      double float_data = 0.0;
      if (string_data.try_to_float(&float_data)) {
        data = float_data;
      } else {
        data = string_data;
      }
    }
  } else if (node.IsSequence()) {
    for (auto it = node.begin(); it != node.end(); ++it) {
      mixed data_piece;
      yaml_node_to_mixed(*it, data_piece, source);
      data.push_back(data_piece);
    }
  } else if (node.IsMap()) {
    for (const auto &it : node) {
      mixed data_piece;
      yaml_node_to_mixed(it.second, data_piece, source);
      data[string(it.first.as<std::string>().c_str())] = data_piece;
    }
  }
}

static string yaml_print_tabs(const uint8_t nesting_level) noexcept {
  return string(2 * nesting_level, ' ');
}

static string yaml_print_key(const mixed& data_key) noexcept {
  if (data_key.is_string()) {
    return data_key.as_string();
  }
  return string(data_key.as_int()); // key can not be an array; bool and float keys are cast to int
}

static void mixed_to_string(const mixed& data, string& string_data, const uint8_t nesting_level = 0) noexcept {
  string buffer;
  if (!data.is_array()) {
    if (data.is_null()) {
      buffer.push_back('~');
    } else if (data.is_string()) {
      const string& string_data_piece = data.as_string();
      if (string_data_piece.size() < 2 || (string_data_piece[0] != '"' && string_data_piece[string_data_piece.size() - 1] != '"')) {
        buffer.push_back('"');
        buffer.append(string_data_piece);
        buffer.push_back('"');
      } else {
        buffer = string_data_piece;
      }
    } else if (data.is_int()) {
      buffer.append(data.as_int());
    } else if (data.is_float()) {
      buffer.append(data.as_double());
    } else if (data.is_bool()) {
      buffer = (data.as_bool()) ? string("true") : string("false");
    }
    string_data.append(buffer);
    string_data.push_back('\n');
    return;
  }
  const array<mixed> &data_array = data.as_array();
  if (data_array.is_pseudo_vector()) {
    for (const auto &it : data_array) {
      const mixed &data_piece = it.get_value();
      buffer = yaml_print_tabs(nesting_level);
      if (data_piece.is_array()) {
        buffer.append("-\n");
      } else {
        buffer.append("- ");
      };
      string_data.append(buffer);
      mixed_to_string(data_piece, string_data, nesting_level + 1);
    }
  } else {
    for (const auto &it : data_array) {
      const mixed &data_piece = it.get_value();
      buffer = yaml_print_tabs(nesting_level);
      buffer.append(yaml_print_key(it.get_key()));
      if (data_piece.is_array()) {
        buffer.append(":\n");
      } else {
        buffer.append(": ");
      };
      string_data.append(buffer);
      mixed_to_string(data_piece, string_data, nesting_level + 1);
    }
  }
}

bool f$yaml_emit_file(const string &filename, const mixed &data) {
  if (filename.empty()) {
    php_warning("Filename cannot be empty");
    return false;
  }
  string emitted_data = f$yaml_emit(data);
  Optional<int64_t> size = f$file_put_contents(filename, emitted_data);
  if (size.is_false()) {
    php_warning("Error while writing to file \"%s\"", filename.c_str());
    return false;
  }
  return true;
}

string f$yaml_emit(const mixed &data) {
  string string_data("---\n");
  mixed_to_string(data, string_data);
  string_data.append("...");
  return string_data;
}

mixed f$yaml_parse_file(const string &filename, int pos) {
  if (filename.empty()) {
    php_warning("Filename cannot be empty");
    return {};
  }
  Optional<string> data = f$file_get_contents(filename);
  if (data.is_false()) {
    php_warning("Error while reading file \"%s\"", filename.c_str());
    return {};
  }
  return f$yaml_parse(data.ref(), pos);
}

mixed f$yaml_parse(const string &data, int pos) {
  if (pos != 0) {
    php_warning("Argument \"pos\" = %d. Values other than 0 are not supported yet. Setting to default (pos = 0)", pos);
  }
  YAML::Node node = YAML::Load(data.c_str());
  mixed parsed_data;
  yaml_node_to_mixed(node, parsed_data, data);
  return parsed_data;
}
