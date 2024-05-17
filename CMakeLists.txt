cmake_minimum_required(VERSION 3.16.1)
project(SysInfoMonitor)

set(CMAKE_CXX_STANDARD 17)

add_executable(SysInfoMonitor
        SysInfo.h main.cpp SysInfo.cpp LinuxInfo.cpp LinuxInfo.h WindowsInfo.cpp WindowsInfo.h Str.cpp Str.h SysInfoFactory.h)


find_package(OpenGL REQUIRED)

find_library(LIBPCI_LIBRARY REQUIRED)

target_link_libraries(SysInfoMonitor -lpci)
target_link_libraries(SysInfoMonitor ${OPENGL_LIBRARIES})