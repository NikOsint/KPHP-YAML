#include <yaml-cpp/yaml.h>

#include "runtime/optional.h"
#include "runtime/streams.h"
#include "runtime/string_functions.h"

#include "runtime/yaml.h"

void yaml_node_to_mixed(const YAML::Node &node, mixed &data) {
  data.clear();
  if (node.IsNull()) {
    php_warning("Yaml node is null");
    return;
  }
  if (node.IsScalar()) {
    data = string(node.as<std::string>().c_str());
    return;
  }
  if (node.IsMap()) {
    for (auto it = node.begin(); it != node.end(); ++it) {
      mixed data_piece;
      yaml_node_to_mixed(*it, data_piece);
      data[string(it->first.as<std::string>().c_str())] = data_piece;
    }
  }
  if (node.IsSequence()) {
    for (auto it = node.begin(); it != node.end(); ++it) {
      mixed data_piece;
      yaml_node_to_mixed(*it, data_piece);
      data.push_back(data_piece);
    }
  }
}

void mixed_to_yaml_node(const mixed &data, YAML::Node &node) {
  if (!data.is_array()) {
    if (data.is_null()) {
      php_warning("Cannot convert (mixed)null into yaml node");
      return;
    }
    if (data.is_string()) {
      node.push_back(std::string(data.as_string().c_str()));
      return;
    }
    if (data.is_int()) {
      node.push_back(data.as_int());
      return;
    }
    if (data.is_float()) {
      node.push_back(data.as_double());
      return;
    }
    if (data.is_bool()) {
      if (!data.as_bool()) node.push_back("false");
      else node.push_back("true");
      return;
    }
  }
  array<mixed> data_array = data.as_array();
  if (data_array.is_pseudo_vector()) {
    for (auto it = data_array.begin(); it != data_array.end(); ++it) {
      mixed data_piece = it.get_value();
      YAML::Node node_piece;
      mixed_to_yaml_node(data_piece, node_piece);
      node.push_back(node_piece);
    }
  } else {
    for (auto it = data_array.begin(); it != data_array.end(); ++it) {
      mixed data_piece = it.get_value();
      YAML::Node node_piece;
      mixed_to_yaml_node(data_piece, node_piece);
      node.force_insert(std::string(it.get_key().as_string().c_str()), node_piece);
      //node[std::string(it.get_key().as_string().c_str())] = node_piece; // alternative way
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
  YAML::Node node;
  mixed_to_yaml_node(data, node);
//  YAML::Emitter out;
//  out << node;
//  if (!out.good()) {
//    php_warning("Error while emitting to yaml");
//    return string();
//  }
//  return string(out.c_str());
  return string(YAML::Dump(node).c_str());
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
  YAML::Node node;
  if (pos > 0) {
    Optional<string> optional_data = f$substr(data, pos);
    if (optional_data.is_false()) {
      php_warning("Error while making substring");
      return {};
    }
    node = YAML::Load(optional_data.ref().c_str());
  } else {
    if (pos < 0) {
      php_warning("Argument \"pos\" cannot be negative, but it is %d. Setting to default (pos = 0)", pos);
    }
    node = YAML::Load(data.c_str());
  }
  mixed parsed_data;
  yaml_node_to_mixed(node, parsed_data);
  return parsed_data;
}
