# eva-vm

A simple virtual machine for the Eva programming language.

## Build instructions

### Conan - Detect profile

```
conan profile detect --force
```

### Conan - Install requirements

```
conan install . --output-folder=build --build=missing
```

### CMake - Configuration

```
cd build
cmake .. -DCMAKE_TOOLCHAIN_FILE=conan_toolchain.cmake -DCMAKE_BUILD_TYPE=Release
```

### CMake - Build

```
cmake --build .
```
