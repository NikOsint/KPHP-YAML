# KPHP YAML

KPHP integrated YAML parser based on [yaml-cpp library](https://github.com/jbeder/yaml-cpp)

## Implemented functions

You can use the following functions from [PHP YAML](https://www.php.net/manual/ru/book.yaml.php) library if you want to compile your project with KPHP: 

```
:_)
```

## Installation

To install this library:

1. [Download KPHP source code](https://github.com/VKCOM/kphp)
```
git clone https://github.com/VKCOM/kphp
```

2. [Download KPHP YAML library](https://git.miem.hse.ru/1367/kphp-yaml)
```
git clone https://git.miem.hse.ru/1367/kphp-yaml
```

3. Replace/add the following files in KPHP folder:
- builtin-functions/_functions.txt
- runtime/yaml.h
- runtime/yaml.cpp

4. [Compile KPHP from source files](https://vkcom.github.io/kphp/kphp-internals/developing-and-extending-kphp/compiling-kphp-from-sources.html)

## Running

Once you've built KPHP with this library from source, you can compile your projects. 

In order to do it through command line you should:

- Specify path to kphp/objs/bin/kphp2cpp binary
- Specify path to your PHP file

For example, if your PHP file is in directory home/your/php/project, and kphp2cpp binary is in directory home/kphp/objs/bin, command to compile with KPHP might look like: 
````
../../../kphp/objs/bin/kphp2cpp solution.php
````

