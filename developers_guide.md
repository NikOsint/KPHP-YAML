# KPHP YAML Developers Guide

Information on how to further develop KPHP YAML package is given below.

## Write new functions

Before writing new functions, you should decide whether KPHP users will be able to use them or not.

### Usable functions

If you develop a new usable function, you should do the following:

1. Add its name, parameters and return value to the [special file](builtin-functions/_functions.txt) which contains all
the functions which can be used in source PHP code. The format is as follows:

`function yaml_emit_file ($filename ::: string, $data ::: mixed) ::: bool;`

2. Add function declaration to the [header file](runtime/yaml.h) with `f$` prefix in the beginning of its name like:

```cpp
bool f$yaml_emit_file(const string &filename, const mixed &data);
```

3. Write function definition to the [source file](runtime/yaml.cpp) like:

```cpp
bool f$yaml_emit_file(const string &filename, const mixed &data) {
  // code here
}
```

Please do not forget to write describing comments so that other developers can easily understand your code.

### Auxiliary functions

If you develop a new auxiliary function which can only be used in other functions in current file,
you should just write it in the [source file](runtime/yaml.cpp) with `static` modifier like:

```cpp
static string yaml_escape(const string &data) noexcept {
  // code here
}
```

Please do not forget to write `noexcept` modifier if your function can not throw exceptions.

Also naming like `yaml_<function_name>` is preferred.

## Write new C++ tests

[GoogleTest library](https://github.com/google/googletest) is used to implement C++ tests.

To write a new one you should add the following to the [tests file](tests/cpp/runtime/yaml-test.cpp):

```cpp
TEST(yaml_test, <your_test_name>) {
  // code here
}
```

Do not forget to replace `<your_test_name` with your tests name.

GoogleTest documentation can be found [here](http://google.github.io/googletest/).

## Write new PHP tests

PHP tests are written in PHP and are placed in [PHP tests folder](tests/phpt/yaml).

To write a new one you should:

1. Create a new file in the PHP tests folder named like `<serial_number>_yaml_<test_name>`. `<serial_number>` is your
tests number. If you write your first PHP test, it is likely to be `04` as three are already implmented.
2. Write your test in the created file. It might look like:

```php
@ok
<?php

function <test_name>() {
  // code here
}

<test_name>();
```

Please do not forget to replace `<test_name>` with your tests name.

`@ok` on the first line indicates that outputs of the test run with PHP and KPHP should be identical. You can use
other identification marks.

More information on writing PHP tests can be found [here](https://vkcom.github.io/kphp/kphp-internals/developing-and-extending-kphp/writing-and-running-tests.html#4-testing-compiler-and-runtime-compare-kphp-and-php).
