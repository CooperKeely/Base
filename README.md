# Base
C Systems layer for linux

**Status:** work in progress. See [Current Status](#current-status) and [Roadmap](#roadmap) sections for updates

## Build
**Requires:** Linux with a Wayland compositor (no X11 backend yet).

**Dependencies** Linux: `clang` or `gcc`, `libwayland-dev`, `libegl-dev`.
Optional: `llvm` (for `llvm-symbolizer`, used by the crash handler's backtraces).

```sh
./build.sh main [release] [gcc] [run] [asan] [verbose] [clean]
```

Arguments can be given in any order. `main` is the required build target.

| Option    | Effect                                              |
|-----------|-----------------------------------------------------|
| `release` | Optimized build (default is debug, `-O0 -g3`)       |
| `gcc`     | Use gcc (default is clang)                          |
| `asan`    | AddressSanitizer (clang only)                       |
| `run`     | Run `build/app` after building                      |
| `verbose` | Print the full compile command                      |
| `clean`   | Delete `build/` first                               |

Examples:
```sh
./build.sh main run             # debug, clang, then run
./build.sh main release gcc     # optimized gcc build
./build.sh main asan run        # debug with AddressSanitizer
```
The binary is written to `build/app`.

## Layout
 - src/base : core functionality arenas, strings, math functions, OS api
 - src/wm : window manager abstraction
 - src/render : rendering interface
 - src/format : different file format parsing
 - src/libs : stb header style portable libraries

## Current Status
**Working:** base layer (Arenas, Scratch Scopes, Strings, Math, Logging), linux OS layer, CSV and OBJ parsing, Wayland Window opening with a OpenGL context 
**Not Yet:** X11 support, rendering backend (currently stubs), input handling, Win32 support
**Known Limitations:** [fixed sized arenas, no expanding arena free lists, no multi threading support]

## Roadmap
 - [X] Wayland support
 - [ ] X11 support
 - [ ] Win32 Window Manager Support
 - [ ] Win32 Kernel Support
 - [ ] OpenGL Rendering Backend
 - [ ] Vulkan Rendering Backend
 - [ ] Immediate mode UI library

## Credits
Source inspired by the [RAD Debugger](https://github.com/EpicGames/raddebugger)
License: MIT
