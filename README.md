### Kingom
Yes I called it Kingom and not Kingdom because I originally misspelled it and I thought why not keep it that way.

### What I did last time
There is a unfinished implementation of the `DefaultMaterial` as well as the `DefaultPipeline` these need to be finished else the project doesn't compile. The idea I had was that we have components which implement PipelineItem so we can have custom items but also the default but this would require us to implement a `next` function on all of the items. We could forward it this comes to mind as I'm writing this lol. This is maybe more intelligent so we can force the user to implement it them self.But we still would need to keep the state if the shader and stuff is already bound. The Idea in general is that you can just add all your components and logic to it and call `render` which just does all the state handling for you but the idea is not refined yet.

Actually I have news:
I implemented what I said at the top but encountered the issue that in order to render a mesh you obv. need the layout but the `BasePipeline` does not have an attribute called `layout` only the `DefaultPipeline` has but we can't just use the `DefaultPipeline` as a parameter because the user might have a custom pipeline so we have essentially 3 ways to tackle this:
- Find a different solution
- Put essential rendering steps in the `BasePipeline` and just require the user to have this
- Make `DefaultPipeline` extremly minimal/abstract and require the user to implement it but at this point we could directly use `BasePipeline` but maybe someone also has its own custom rendering system so yea needs to be dicussed

After about 10 seconds of thinking:
I guess we remove DefaultPipeline and use BasePipeline using protected attributes and then when I'm building my own game I'm going to see if iu need it or not :). If you reading this need something like this open a Issue.

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

### State
- [x] Buffers for VBO, EBO
- [x] Vertex Attribute
- [x] Vertex Array Object
