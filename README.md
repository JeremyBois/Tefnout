# Tefnout Engine
A Game Engine project with a focus on learning more about architecture and advanced concept of C++ like implementing STL compliant classes (see `Tefnout::Buffer::Ring` as an example).

Continuation of previous work at [SimpelGL](https://github.com/JeremyBois/SimpleGL) but architecture is rewritten from scratch.

Currently only OpenGL is supported on both Windows and GNU/Linux.

## Getting started (Dev)
### Setup dependencies
  - `glm` - [Version 0.9.9.8](https://github.com/g-truc/glm/releases/tag/0.9.9.8)
  - `spdlog` - [Version v1.8.2](https://github.com/gabime/spdlog/releases/tag/v1.8.2)
  - `OpenGL` - Core profil > 3.3
  - `glfw3` - [Version 3.3.2](https://github.com/glfw/glfw/releases/tag/3.3.2)

After getting the repository all sub-modules must be fetched then follow each specific dependency configuration below:
```bash
git submodule update --init --recursive
```

#### glm
**LICENSE - MIT**

Nothing to do.

#### Spdlog
**LICENSE - MIT**

In order to reduce compilation time, pre-compiled header for the `spdlog` dependency must be built first as below for both DEBUG and RELEASE targets. System library could also be used if already installed.
```bash
# Move to spdlog sub-module
cd Tefnout/vendors/spdlog/
# Create the build directory
mkdir build && cd build

# Debug version
cmake .. -G "Ninja" -D CMAKE_BUILD_TYPE=Debug -D SPDLOG_BUILD_SHARED=OFF -D SPDLOG_FMT_EXTERNAL=ON -D SPDLOG_BUILD_EXAMPLE=OFF -D SPDLOG_BUILD_TESTS=OFF -D CMAKE_INSTALL_PREFIX=./install/Debug -D CMAKE_POSITION_INDEPENDENT_CODE=ON
# Compile and install to build/install/Debug
ninja install

# Release version
cmake .. -G "Ninja" -D CMAKE_BUILD_TYPE=Release -D SPDLOG_BUILD_SHARED=OFF -D SPDLOG_FMT_EXTERNAL=ON -D SPDLOG_BUILD_EXAMPLE=OFF -D SPDLOG_BUILD_TESTS=OFF -D CMAKE_INSTALL_PREFIX=./install/Release -D CMAKE_POSITION_INDEPENDENT_CODE=ON
# Compile and install to build/install/Release
ninja install
```

#### OPENGL
**LICENSE - Apache Version 2.0**

As OpenGL is only a standard we still need to create the function pointers to each function at the driver level. As all card does not support the same ammount of features from the standard available functions are implemented by the manufactor. Of course retrieving them is [OS specific](https://www.khronos.org/opengl/wiki/Load_OpenGL_Functions) and its where **GLAD** shines.

#### GLAD
**LICENSE - WTFPL**

configuration used in this project is defined [here](http://glad.dav1d.de/#profile=core&specification=gl&api=gl%3D4.6&api=gles1%3Dnone&api=gles2%3Dnone&api=glsc2%3Dnone&language=c&loader=on) and as been generated using the following parameters
  - Specification - OpenGL
  - Language - C/C++
  - gl API - 4.6
  - Profile - Core
  - Extensions - None

This dependency is already inside the project, so you have nothing to do expect be sure that OpenGL is installed on your computer. The $4.6$ version is not strictly required and not forced by cmake script.
```bash
# Get current installed OpenGL core profil version on Linux
glxinfo | grep "OpenGL core profile version"
# On my computer --> OpenGL core profile version string: 4.6 (Core Profile) Mesa 20.3.3
```

If you have OpenGL you can now move on to [GLFW](#GLFW).


#### GLFW
**LICENSE - zlib-libpng**

To be able to build Tefnout without using pre-compiled library use the following `-D TEFNOUT_USE_PRECOMPILED_GLFW=OFF`. To use pre-compiled libraries continue reading.

GLFW library could also be built first for both DEBUG and RELEASE targets using following commands. This step is optional but could improve compilation time.
```bash
# Move to spdlog sub-module
cd Tefnout/vendors/glfw/
# Create the build directory
mkdir build && cd build

# Debug version (Dynamic)
cmake .. -G "Ninja" -D CMAKE_BUILD_TYPE=Debug -D GLFW_BUILD_DOCS=OFF -D GLFW_BUILD_TESTS=OFF -D GLFW_BUILD_EXAMPLES=OFF -D BUILD_SHARED_LIBS=ON -D CMAKE_INSTALL_PREFIX=./install/Debug
# Compile and install to build/install/Debug
ninja install

# Release version (Dynamic)
cmake .. -G "Ninja" -D CMAKE_BUILD_TYPE=Release -D GLFW_BUILD_DOCS=OFF -D GLFW_BUILD_TESTS=OFF -D GLFW_BUILD_EXAMPLES=OFF -D BUILD_SHARED_LIBS=ON -D CMAKE_INSTALL_PREFIX=./install/Release
# Compile and install to build/install/Release
ninja install
```


### Build project
First all dependencies must be fetched and correctly initialized as described in [Setup dependencies](#setup-dependencies) section.

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

Cmake can also be used to build targets:
```bash
cd build

# BUILD using 4 cores
cmake --build . -j 4

# CLEAN
cmake --build . --target clean

# REBUILD = CLEAN + BUILD
cmake --build . -j 4 --clean-first
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

To keep the project `compile_commands.json` synchronized with the one generated in the build folder one can use the following one liner:
```bash
cmake .. -G "Unix Makefiles" -DCMAKE_EXPORT_COMPILE_COMMANDS=ON && compdb -p . list > ../compile_commands.json && cp ../compile_commands.json compile_commands.json
```

#### Side note
`Ninja` appears to be more performant for incremental build than `make` where there is no difference in speed when compiling from scratch.

### Other
`cmake` can build project configuration for every avaiblable tools. More information in the [Getting Started](#getting-started-dev) section.


