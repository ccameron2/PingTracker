# PingTracker
<img src="https://github.com/user-attachments/assets/e5bee8e3-a8ac-4859-a8d6-6fc3a90c4b75" width="300" height="300" />

PingTracker is a simple, lightweight, cross-platform network utility written in C++. It helps you visualize network latency and identify connection issues.

![PingTracker](https://github.com/user-attachments/assets/5523eda9-2db5-417a-9480-3a55d0e32535)

## Download: [Releases](https://github.com/ccameron2/PingTracker/releases)

## Features
- Visual representation of network latency
- Customisable graph view
- Statistics (min, max, average)
- Different colours!

## Installation
Download a release from [Releases](https://github.com/ccameron2/PingTracker/releases), extract, and double click the Install.bat file.
Delete the downloaded folder after installation if desired

## How it works

Built using:
- [SDL3](https://github.com/libsdl-org/SDL)
- [imgui](https://github.com/ocornut/imgui)
- [implot](https://github.com/epezent/implot)
- [imcplib](https://github.com/markondej/cpp-icmplib)
- [plfnanotimer](https://github.com/mattreecebentley/plf_nanotimer)
- [premake](https://github.com/premake/premake-core)
- [cmake](https://github.com/Kitware/CMake)

This program creates an SDL window, displays an imgui ui, and displays ping data in a graph using implot. 
Pings are sent to "8.8.8.8" using imcplib and plfnanotimer is used to measure ping times.
PingTracker uses a premake build system, GenerateProjects also calls cmake to build SDL.

## Source files
```bash
git clone --recurse-submodules https://github.com/ccameron2/PingTracker
cd PingTracker/PingTracker/vendor/imgui
git checkout docking
```
## Building from source

### Windows
- Ensure you have [cmake](https://github.com/Kitware/CMake) installed (required to build SDL)
- Get the source code using the above command
- Run GenerateProjects.bat
- Open the generated solution and build the project

### Linux
- Ensure you have [cmake](https://github.com/Kitware/CMake) installed (required to build SDL)
- Get the source code using the above command
- Run GenerateProjects.sh
- Note: PingTracker must run with admin permissions on linux
  
## License

PingTracker is released under the MIT License

---
