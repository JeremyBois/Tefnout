# Tefnout Engine
A Game Engine project with a focus on learning more about architecture and advanced concept of C++ like implementing STL compliant classes (see `Tefnout::Buffer::Ring` as an example).

Continuation of previous work at [SimpelGL](https://github.com/JeremyBois/SimpleGL) but architecture is rewritten from scratch.


## Getting started (Dev)
### Setup dependencies
  - `glm` - [Version 0.9.9.8](https://github.com/g-truc/glm/releases/tag/0.9.9.8)
  - `spdlog` - [Version v1.8.2](https://github.com/gabime/spdlog/releases/tag/v1.8.2)

After getting the repository all sub-modules must be fetched then follow each specific dependency configuration below:
```bash
git submodule update --init --recursive
```

#### glm
**LICENSE - MIT**

Nothing to do.

#### Spdlog
**LICENSE - MIT**

In order to reduce compilation time, pre-compiled header for the `spdlog` dependency must be built first as below:
```bash
# Move to spdlog sub-module
cd Tefnout/vendors/spdlog/
# Create the build directory
mkdir build && cd build
# Prepare library installation
cmake .. -D CMAKE_BUILD_TYPE=Debug -D SPDLOG_BUILD_SHARED=OFF -D SPDLOG_FMT_EXTERNAL=ON -D SPDLOG_BUILD_EXAMPLE=OFF -D SPDLOG_BUILD_TESTS=OFF -D CMAKE_INSTALL_PREFIX=./install -D CMAKE_POSITION_INDEPENDENT_CODE=ON
# Compile and install to build/install
make install
```

### Build project
First all dependencies must be fetched and correctly initialized as described in [Setup dependencies section](#setup-dependencies).

Then follow the instructions below to generate the build files for any build tool using Cmake.

```bash
# 1) Be sure dependencies are correctly setup

# 2) Create a build folder
mkdir build && cd build

# 3) Build project configuration for your editor of choice
cmake .. -G <build_tool>
# On Linux using Ninja as build tool
cmake .. -G "Ninja" -DCMAKE_EXPORT_COMPILE_COMMANDS=ON && compdb -p . list > ../compile_commands.json
# On Linux using Make as build tool
cmake .. -G "Unix Makefiles" -DCMAKE_EXPORT_COMPILE_COMMANDS=ON && compdb -p . list > ../compile_commands.json
```


## General information
### Code structure
  - Header and source files should be in the same folder
  - Folder are split based on themes / purposes
  - Avoid `#pragma once` (not supported on every compilers)
  - `explicit` constructor by default
  - Naming convention
    - only upper case for macro
    - camelCase for variables like `variableName`
    - pascalCase for type (structs, classes, ...) like `TypeName`
    - prefix with `s_` static members like `s_StaticAttribute`
    - prefix with `m_` private data to distinguish it from public data like `m_height`

`clangd` is used to run the clang formatter (see `.clang-format`)


### Sublime-text / Vscode
I use the `clangd LSP server` for code completion and other features.
The server need a `compile_commands.json` file to provide IDE features.

`cmake` can built this file for us with `-DCMAKE_EXPORT_COMPILE_COMMANDS=ON`. Header information also added using `compdb` as described below:
```bash
    # Go into the build directory (create it if needed)
    cd build/

    # Cmake template
    cmake .. -G <build_generator> -DCMAKE_EXPORT_COMPILE_COMMANDS=ON
    # Cmake with Ninja
    cmake .. -G "Ninja" -DCMAKE_EXPORT_COMPILE_COMMANDS=ON
    # Cmake with Make
    cmake .. -G "Unix Makefiles" -DCMAKE_EXPORT_COMPILE_COMMANDS=ON

    # Add header information to compile commands for better completion
    compdb -p . list > ../compile_commands.json
```

### Other
`cmake` can build project configuration for every avaiblable tools. More information in the [Getting Started section](#getting-started).


