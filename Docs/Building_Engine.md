# Building Orion Engine

## Table of Contents

- [Building Orion Engine](#building-orion-engine)
    - [Table of Contents](#table-of-contents)
    - [Prerequisites](#prerequisites)
        - [General](#general)
        - [Graphics API](#graphics-api)
            - [None](#none)
            - [Vulkan](#vulkan)
    - [Obtaining the source code](#obtaining-the-source-code)
    - [Generating build files](#generating-build-files)
    - [Compiling the project](#compiling-the-project)

## Prerequisites

### General

* CMake >= 3.25
* C++26 compatible compiler:
    * GCC >= 16
    * Clang >= 22
* Git
* Ninja [Optionally]

### Graphics API

Depending on the selected graphics API (via `-DORION_GRAPHICS_API` flag) there are some additional prerequisites.

#### None

* Nothing.

#### Vulkan

* Vulkan SDK >= 1.3.329

## Obtaining the source code

You can obtain the source code by cloning the project via Git:

```sh
git clone https://github.com/SandNoodle/OrionEngine.git
cd OrionEngine
```

## Generating build files

This project uses CMake as its build system - for more available options you can read
this [CMakeLists.txt](/Engine/CMakeLists.txt) file.

Assuming you are in the cloned directory, here's a minimal setup for configuring this project with `Vulkan`
support in `Debug` mode (using `Clang` as a compiler and `ninja` as a build system).

```sh
cmake . -B cmake-clang-debug-vulkan -G Ninja -DCMAKE_BUILD_TYPE=Debug -DCMAKE_C_COMPILER=clang -DCMAKE_CXX_COMPILER=clang++ -DORION_GRAPHICS_API=Vulkan
```

## Compiling the project

To compile the project using `ninja` (given that the project was configured correctly), just invoke these commands:

```sh
cd cmake-clang-debug-vulkan
ninja
```
