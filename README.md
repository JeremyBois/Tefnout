# Tefnout Engine
A Game Engine project with a focus on learning more about architecture and advanced concept of C++ like implementing STL compliant classes (see `Tefnout::Buffer::Ring` as an example).

Continuation of previous work at [SimpelGL](https://github.com/JeremyBois/SimpleGL) but the architecture is rewritten from scratch.

Currently only OpenGL is supported on both Windows and GNU/Linux but Vulkan is planned.

**What I am currently working on:**
  - OpenGL 2D rendering pipeline (VertexArray, FrameBuffer, Renderer, Texture, Shader)
  - Abstract 2D renderer (because I want OpenGL and Vulkan support)
  - GUI with [ImGui](https://github.com/ocornut/imgui)
  - ECS based on [Sparse set](https://www.geeksforgeeks.org/sparse-set/)

**What is planned:**
  - Vulkan support
  - Continuous integration testing with Circle CI



## Getting started (Dev)
### Setup dependencies
Engine:
  - `glm` - [Version 0.9.9.8](https://github.com/g-truc/glm/releases/tag/0.9.9.8)
  - `spdlog` - [Version v1.8.2](https://github.com/gabime/spdlog/releases/tag/v1.8.2)
  - `OpenGL` - Core profil > 3.3
  - `glfw3` - [Version 3.3.2](https://github.com/glfw/glfw/releases/tag/3.3.2)

Tests only:
  - `Catch2` - [Version v2.14.3](https://github.com/catchorg/Catch2/releases/tag/v2.13.4)


After getting the repository all sub-modules must be fetched. Then you can follow each specific dependency configuration as described below:
```bash
git submodule update --init --recursive
```

#### glm
**LICENSE - [MIT](https://github.com/g-truc/glm/blob/master/copying.txt)**

Nothing to do.

#### Spdlog
**LICENSE - [MIT](https://github.com/gabime/spdlog/blob/v1.x/LICENSE)**

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
**LICENSE - [Apache Version 2.0](https://github.com/KhronosGroup/OpenGL-Registry/issues/376#)**

As OpenGL is only a standard we still need to create the function pointers to each function at the driver level. However All cards do not support the same ammount of features from the standard. Then available functions must be implemented by the manufactor. Of course retrieving them is [OS specific](https://www.khronos.org/opengl/wiki/Load_OpenGL_Functions) and its where **GLAD** shines.

#### GLAD
**LICENSE - [MIT](https://github.com/Dav1dde/glad/blob/master/LICENSE)**

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


#### GLFW3
**LICENSE - [zlib](https://github.com/glfw/glfw/blob/master/LICENSE.md)**

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

#### Catch2
**LICENSE - [Boost Software License 1.0](https://github.com/catchorg/Catch2/blob/devel/LICENSE.txt)**

Nothing to do.


### Build project
First all dependencies must be fetched and correctly initialized as described in [Setup dependencies](#setup-dependencies) section.

Then follow the instructions below to generate the build files for any build tool using Cmake.

```bash
# 1) Be sure dependencies are correctly setup

# 2) Build project configuration for your editor of choice
cmake -G <build_tool> -B "./build" -S "."
# make
cmake -G "Unix Makefiles" -B "./build" -S "." -D CMAKE_EXPORT_COMPILE_COMMANDS=ON
# Create ninja
cmake -G "Ninja" -B "./build" -S "." -D CMAKE_EXPORT_COMPILE_COMMANDS=ON
```

In order to also build tests add the `-D TEFNOUT_BUILD_TESTING=ON` to the command above or update the Cmake cache in the build directory (`CmakeCache.txt`).

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
  - Avoid `#pragma once` (not supported on every compilers) and does not allow headers aggregation
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
The language server need a `compile_commands.json` file to provide IDE features.

The following step are needed in order to make IDE features works nicely
  - Use Cmake to generate the needed `compile_commands.json`
  - Add header information using [compdb](https://github.com/Sarcasm/compdb)
  - Keep build and project `compile_commands.json` in sync

The one liner below performs all steps in a single command and **must be run from the project root directory**:
  1. Create build directory is needed
  2. Use local `CmakeLists.txt` as configuration for Cmake
  3. Generate makefiles for all targets
  4. Generate the compilation database
  5. Update database with header information
  6. Copy database to project folder to keep both in sync

```bash
# WITHOUT tests
cmake -G "Unix Makefiles" -B "./build" -S "." -D CMAKE_EXPORT_COMPILE_COMMANDS=ON && compdb -p ./build list > compile_commands.json && cp compile_commands.json ./build/compile_commands.json

# WITH tests
cmake -G "Unix Makefiles" -B "./build" -S "." -D CMAKE_EXPORT_COMPILE_COMMANDS=ON -D TEFNOUT_BUILD_TESTING=ON && compdb -p ./build list > compile_commands.json && cp compile_commands.json ./build/compile_commands.json
```

#### Side note
`Ninja` appears to be more performant for incremental build than `make` where there is no difference in speed when compiling from scratch.

### Other
`cmake` can build project configuration for every avaiblable tools. More information in the [Getting Started](#getting-started-dev) section.


