# dispatch_example
Simple example of dispatching for types

# Building
```
$ git clone 'https://github.com/cdacamar/dispatch_example.git'
$ cd dispatch_example
$ mkdir build && cd build
$ cmake .. && make
```

If you would rather build from source without the dependencies of cmake and make:
```
$ cd dispatch_example
$ c++ -o dispatch_example -Wall -Werror -pedantic -std=c++1z -I. main.cpp
```
