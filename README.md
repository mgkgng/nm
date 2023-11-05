
# nm 📐

> **“However expressive, symbols can never be the things they stand for.”**
> ― Aldous Huxley, The Doors of Perception / Heaven and Hell

Do you know the `nm` command? It's a standard Unix program that lists symbols from ELF files. It is typically used to debug and inspect binary files to understand their structure and to verify the symbols they contain.

Here, I implemented the core functionalities of this tool for educational purposes. Let me provide you with a basic overview and guide you through the usage of this project.

## Project overview 🔎

### Objectives
- Reimplement the nm command for ELF binaries with support for x86_32, x64, object files and dynamic libraries, including sorting and filtering options (-a, -r, -p, -g, -u)

### Achievements
- Successfully rebuilt the core features of the `nm` command including sorting and filtering options for the output
- Gained a thorough understanding of ELF file format, learning to navigate and interpret the structure of executables, object files and libraries
- Deepened knowledge of the compilation and linking processes
- Implemented error handling for edge cases (i.e., invalid ELF headers, out-of-range offsets, etc.)

## Getting started 🦾
### Prerequisites
- Operating System: Linux, due to its reliance on the `elf.h` header for ELF format parsing

### Build & Run
1. Compile:
```
make
```
2. Execute
```
./ft_nm [options] [path(s)]
```
- Replace `[options]` with any combinations of the supported options : `-a`, `-r`, `-p`, `-g`, `-u` (e.g., `-aru`).
- Provide one or more paths in `[file(s)]` to specify target files.
- Feel free to try sample files located in the tests/ directory!
