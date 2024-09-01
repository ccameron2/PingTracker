# PingPlotter
<img src="https://github.com/user-attachments/assets/e5bee8e3-a8ac-4859-a8d6-6fc3a90c4b75" width="300" height="300" />

PingPlotter is a simple, lightweight, cross-platform network utility. It helps you visualize network latency and identify connection issues.

![PingPlotterRunning](https://github.com/user-attachments/assets/e0386d29-076f-43ee-ab3c-d4938873a453)


Download: [Releases](https://github.com/ccameron2/PingPlotter/releases)
## Features
- Visual representation of network latency
- Customisable graph view
- Statistics (min,max,average)
- Different colours!

## Installation
[Pictures here]
## Usage
[Pictures here]
## How it works
Built using:
- [SDL3](https://github.com/libsdl-org/SDL)
- [imgui](https://github.com/ocornut/imgui)
- [implot](https://github.com/epezent/implot)
- [imcplib](https://github.com/markondej/cpp-icmplib)
- [plfnanotimer](https://github.com/mattreecebentley/plf_nanotimer)
- [premake](https://github.com/premake/premake-core)
- [cmake](https://github.com/Kitware/CMake)
- [stb_image](https://github.com/nothings/stb/blob/master/stb_image.h) on Linux

This program creates an SDL window, displays an imgui ui, and displays the ping data in a graph using implot. Pings are sent using imcplib and plfnanotimer is used for a timer.

## Source files
```bash
git clone --recurse-submodules https://github.com/ccameron2/PingPlotter
```
## Building from source
### Windows

- Ensure you have cmake installed (required to build SDL)
- Get the source code using the above command
- Run GenerateProjects.bat
- Open the generated solution and build each project
- Run the program from visual studio
- (There may be an issue with "#include SDL_Internal.h in one of the SDL files". If you get this, comment out the include, im not sure whats going on there)

### Linux
Coming soon
## License

PingPlotter is released under the MIT License

---
