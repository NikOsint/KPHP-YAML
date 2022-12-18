# KPHP YAML

KPHP integrated YAML parser and emitter based on [yaml-cpp](https://github.com/jbeder/yaml-cpp) library

## Implemented functions

The following functions from [PHP YAML](https://www.php.net/manual/ru/book.yaml.php) library are implemented: 

```php
yaml_emit_file(string $filename, mixed $data): bool

yaml_emit(mixed $data): string

yaml_parse_file(string $filename, int $pos = 0): mixed

yaml_parse(string $data, int $pos = 0): mixed
```

## Prerequisites

In order to use this package, [KPHP](https://vkcom.github.io/kphp) compiler is needed.

> *NOTE:* this library uses [yaml-cpp](https://github.com/jbeder/yaml-cpp) library to parse and emit YAML files, but you do not need to install it manually. It is automatically installed with KPHP compiler.

## Installation

To install this library, follow the steps:

1. [Download KPHP source code](https://github.com/VKCOM/kphp)
```shell
git clone https://github.com/VKCOM/kphp
```

2. [Download KPHP YAML library](https://git.miem.hse.ru/1367/kphp-yaml)
```shell
git clone https://git.miem.hse.ru/1367/kphp-yaml
```

3. Replace/add the following files in KPHP folder:
- builtin-functions/_functions.txt
- runtime/yaml.h
- runtime/yaml.cpp
- runtime/runtime.cmake
- tests/cpp/runtime/yaml-test.cpp
- tests/cpp/runtime/runtime-tests.cmake

4. [Compile KPHP from source files](https://vkcom.github.io/kphp/kphp-internals/developing-and-extending-kphp/compiling-kphp-from-sources.html)

## Testing

> Tests are currently under development

Once KPHP is built, you can run tests.

### C++ Tests

These tests check whether there are bugs in KPHP itself, so no conversion from PHP takes place.

To run them, use command:
```shell
ctest [-j <jobs>]
```

### PHP Tests

These tests are written in PHP and are executed twice: with PHP and with KPHP. Then both outputs are compared.

To run them, use command:
```shell
tests/kphp_tester.py [<testfilename.php>]
``` 

## Running

Once KPHP is built, you can compile your projects with it. 

In order to do so through command line you should:

- Specify path to kphp/objs/bin/kphp2cpp binary
- Specify path to your PHP file

For example, if your PHP file is in directory home/your/php/project, and kphp2cpp binary is in directory home/kphp/objs/bin, command might look like: 
```shell
../../../kphp/objs/bin/kphp2cpp solution.php
```

