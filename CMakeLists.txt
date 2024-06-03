cmake_minimum_required(VERSION 3.16.1)
project(SysInfoMonitor)

set(CMAKE_CXX_STANDARD 17)

if(CMAKE_SYSTEM_NAME STREQUAL "Linux")
    add_executable(SysInfoMonitor
        SysInfo.h main.cpp SysInfo.cpp LinuxInfo.cpp LinuxInfo.h Str.cpp Str.h SysInfoFactory.h)

    find_package(OpenGL REQUIRED)

    find_library(LIBPCI_LIBRARY REQUIRED)

    target_link_libraries(SysInfoMonitor -lpci)
    target_link_libraries(SysInfoMonitor ${OPENGL_LIBRARIES})

elseif(CMAKE_SYSTEM_NAME STREQUAL "Windows")
    add_executable(SysInfoMonitor
        SysInfo.h main.cpp SysInfo.cpp WindowsInfo.cpp WindowsInfo.h SysInfoFactory.h)
else()
    message("Unsupported platform.")
endif()