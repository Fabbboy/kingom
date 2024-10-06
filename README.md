### Kingom
Yes I called it Kingom and not Kingdom because I originally misspelled it and I thought why not keep it that way.

### Build
It should built on Linux, Macos and Windows. If not, please open an issue. You have two options to build it:
1. The project provides a `vcpkg.json` file. This includes everything you need to build the project. Just download the vcpkg tool.
2. Download the dependencies manually. This works especially well on Linux and Macos. You can find the dependencies in the `vcpkg.json` file.

#### Other dependencies
- CMake
- A C++17 compiler
- Make/Ninja/MSBuild

After the dependencies are installed, you can build the project with the following commands:
```bash
mkdir build
cd build
cmake ..
cmake --build .
```

#### Note
Building the project in Visual Studio should also work but I didn't test it. I just built the project from Terminal using installed LLVM and Clang + Ninja. 

Look at following tools:
- [vcpkg](https://vcpkg.io/) - Microsoft's C++ package manager
- [CMake](https://cmake.org/) - Cross-platform build system
- [Scoop](https://scoop.sh/) - Windows package manager
- [Chocolatey](https://chocolatey.org/) - Windows package manager