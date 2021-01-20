# Tefranout Engine

## Getting started
### Dev


## Build tool
Cmake is used to generate the build files for any build tool.

## Code structure
  - header and source files should be in the same folder
  - folder are split based on themes / purposes
  - avoid `#pragma once` (not supported on every compilers)
  - only upper case for macro
  - camelCase for variables
    - `variableName`
  - pascalCase for type (structs, classes, ...)
    - `TypeName`


## Dependencies
  - `glm` - [Version 0.9.9.8](https://github.com/g-truc/glm/releases/tag/0.9.9.8)


## Sublime-text
I use the `clangd LSP server` for code completion and other features.
The server need a `compile_commands.json` file to provide IDE features.

`cmake` can built this file for us with by adding `-DCMAKE_EXPORT_COMPILE_COMMANDS=1` as elow:
```bash
    cd build/

    # Generic command
    cmake .. -G <build_generator> -DCMAKE_EXPORT_COMPILE_COMMANDS=1

    # Cmake with Ninja
    cmake .. -G "Ninja" -DCMAKE_EXPORT_COMPILE_COMMANDS=1
    ninja
```
