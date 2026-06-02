# Fex Interpreter

**Status: Active Development**

This interpreter is still under development. Features may change and bugs may be present.

---

An interpreter written in C++ for a custom programming language, **Fex**.

The interpreter supports:

- Compiling source files
- An interactive REPL when no file argument is provided

## Build from Source

### Requirements

- C++ compiler with C++20 support (or newer)
- CMake

### Build Steps

```bash
git clone https://github.com/paul-csc/Fex-Interpreter.git
cd Fex-Interpreter

cmake -S . -B build
cmake --build build
```

The compiled binary will be located in the `build` directory.

## Usage

Run the interpreter with a file:

```bash
fex program.fex
```

Run without arguments to start the REPL:

```bash
fex
```

## Syntax

Fex has a syntax similar to **C**.

<!-- TODO: Add syntax examples and language features -->
