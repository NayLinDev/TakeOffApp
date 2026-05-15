# Dependencies

- use `vcpkg` to install `PoDoFo` PDF library. `vcpkg` will install its dependencies too. 
- `CMAKE_TOOLCHAIN_FILE` in `CMakePresets.json` needs to be consistant with `vcpkg` installation.
- the directory `dependencies` includes prebuilt `PDFium` binaries for win64.
- the directory `dependencies` also is a folder for all future third-party libraries
- the directory `data` includes test files for the application.

# How to compile

issue 
`cmake --preset vs2026-x64-debug` or `cmake --preset vs2026-x64-release`

from the source directory to run CMake to generate build files.

issue 
`cmake --build build --config Debug` or `cmake --build build --config Release`

to build.

In release builds, `Qt6Core.dll, Qt6Gui.dll, Qt6Widgets.dll, Qt6Svg.dll, imageformats\*.dll and platforms\qwindows.dll` needs to be in the application binary folder.

In CMakePresets.json details are given.

`vcpkg.json` is used in manifest mode. It is disable by changing its name to `vcpkg_avoid.json`.