# Tefnout Engine

Cmake is used to generate the build files for any build tool.


## Setup dependencies
  - `glm` - [Version 0.9.9.8](https://github.com/g-truc/glm/releases/tag/0.9.9.8)
  - `spdlog` - [Version v1.8.2](https://github.com/gabime/spdlog/releases/tag/v1.8.2)

After getting the repository all sub-modules must be fetched then follow each specific dependency configuration below:
```bash
git submodule update --init --recursive
```

### glm
**LICENSE - MIT**

Nothing to do.

### Spdlog
**LICENSE - MIT**

In order to reduce compilation time, pre-compiled `spdlog` is used.
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




## General
## Code structure
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


### Sublime-text
I use the `clangd LSP server` for code completion and other features.
The server need a `compile_commands.json` file to provide IDE features.

`cmake` can built this file for us with by adding `-DCMAKE_EXPORT_COMPILE_COMMANDS=ON` and also add header information using `compdb` as below:
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


