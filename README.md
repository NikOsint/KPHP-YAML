# KPHP YAML

KPHP integrated YAML parser and emitter based on [yaml-cpp](https://github.com/jbeder/yaml-cpp) library

> *NOTE*: In PHP, YAML functions are handled by another library - [LibYAML](https://pyyaml.org/wiki/LibYAML).
> 
> Behaviour of LibYAML and yaml-cpp differs slightly, so YAML documents parsed with PHP and KPHP might differ in style (not in content).

## Implemented functions

The following functions from [PHP YAML](https://www.php.net/manual/ru/book.yaml.php) library are implemented: 

```php
yaml_emit_file(string $filename, mixed $data): bool

yaml_emit(mixed $data): string

yaml_parse_file(string $filename, int $pos = 0): mixed

yaml_parse(string $data, int $pos = 0): mixed
```

### yaml_emit_file()

Generate a YAML representation of the provided $data in the $filename.

Parameters:
- `string $filename` - name of the file to write data to
- `mixed $data` - data to generate a YAML document from; can be any type except a resource

Returns: *bool*
- `true` if the file was written successfully
- `false` if something went wrong and the file was not written

### yaml_emit()

Generate a YAML representation of the provided $data.

Parameters:
- `mixed $data` - data to generate a YAML document from; can be any type except a resource

Returns: *string* - YAML representation of $data

### yaml_parse_file()

Convert a YAML document stream read from a file to a PHP variable.

Parameters:
- `string $filename` - name of the file to read data from
- `int $pos` - number of document to extract from stream; not implemented yet, all the documents are extracted

Returns: *mixed*
- `false` on failure
- otherwise, value encoded in input in appropriate PHP type

### yaml_parse()

Convert a YAML document stream to a PHP variable.

Parameters:
- `string $data` - YAML document to parse
- `int $pos` - number of document to extract from stream; not implemented yet, all the documents are extracted

Returns: *mixed*
- `false` on failure
- otherwise, value encoded in input in appropriate PHP type

## Prerequisites

In order to use this package, [KPHP](https://vkcom.github.io/kphp) compiler is needed.

> *NOTE*: this library uses [yaml-cpp](https://github.com/jbeder/yaml-cpp) library to parse and emit YAML files, but you do not need to install it manually.
> 
> It is automatically installed with KPHP compiler.

## Installation

If the [Merge Request](https://github.com/VKCOM/kphp/pull/707) is closed, you do not need to install this package - it is already built in KPHP.

Otherwise, to install this library, follow the steps:

1. [Download KPHP source code](https://github.com/VKCOM/kphp)
```shell
git clone https://github.com/VKCOM/kphp
```

2. [Download KPHP YAML library](https://git.miem.hse.ru/1367/kphp-yaml)
```shell
git clone https://git.miem.hse.ru/1367/kphp-yaml
```

3. Replace/add the following files in KPHP folder:
- `builtin-functions/_functions.txt`
- `runtime/yaml.h`
- `runtime/yaml.cpp`
- `runtime/runtime.cmake`
- `tests/cpp/runtime/yaml-test.cpp`
- `tests/cpp/runtime/runtime-tests.cmake`
- `tests/phpt/yaml/01_yaml_complex.php`
- `tests/phpt/yaml/02_yaml_config_file.php`
- `tests/phpt/yaml/03_yaml_escaping.php`
- `tests/phpt/yaml/yaml_escaping.yml`
- `tests/python/lib/kphp_run_once.py`

4. [Compile KPHP from source files](https://vkcom.github.io/kphp/kphp-internals/developing-and-extending-kphp/compiling-kphp-from-sources.html)

## Testing

Once KPHP is built, you can run tests.

### C++ Tests

These tests check whether there are bugs in the library.

To run them, use command:
```shell
ctest [-j <jobs>]
```

### PHP Tests

These tests are written in PHP and are executed twice: with PHP and with KPHP. Then both outputs are compared.

To run PHP tests for this particular library, execute the following command in your terminal, assuming you are in kphp folder:

```shell
tests/kphp_tester.py tests/phpt/yaml
```

> *NOTE*: PHP YAML module is not installed with PHP by default.
> 
> Without it, PHP tests will crash.
> 
> To install PHP YAML, run command:
> ```shell
> apt install php-yaml
> ```

To run other PHP tests, please follow the [official guide](https://vkcom.github.io/kphp/kphp-internals/developing-and-extending-kphp/writing-and-running-tests.html#4-testing-compiler-and-runtime-compare-kphp-and-php).

## Running

Once KPHP is built, you can compile your projects with it. 

In order to do so through command line you should:

- Specify path to kphp/objs/bin/kphp2cpp binary
- Specify path to your PHP file

For example, if your PHP file is in directory home/your/php/project, and kphp2cpp binary is in directory home/kphp/objs/bin, command might look like: 
```shell
../../../kphp/objs/bin/kphp2cpp solution.php
```

