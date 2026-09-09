@echo off
echo Generate Visual Studio 2019 x86 project files
cmake -G "Visual Studio 16 2019" -A Win32 -S . -B Build/VS2019/x86