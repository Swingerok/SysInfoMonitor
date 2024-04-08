cmake_minimum_required(VERSION 3.23)
project(SysInfoMonitor)

set(CMAKE_CXX_STANDARD 23)

add_executable(SysInfoMonitor
        SysInfo.h main.cpp SysInfo.cpp LinuxInfo.cpp LinuxInfo.h WindowsInfo.cpp WindowsInfo.h Str.cpp Str.h)