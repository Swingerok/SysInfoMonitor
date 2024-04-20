cmake_minimum_required(VERSION 3.23)
project(SysInfoMonitor)

set(CMAKE_CXX_STANDARD 23)

add_executable(SysInfoMonitor
        SysInfo.h main.cpp SysInfo.cpp LinuxInfo.cpp LinuxInfo.h WindowsInfo.cpp WindowsInfo.h Str.cpp Str.h)

find_package(OpenGL REQUIRED)

# Поиск библиотеки libpci
find_library(LIBPCI_LIBRARY REQUIRED)
#target_link_libraries(SysInfoMonitor ${LIBPCI_LIBRARY})
target_link_libraries(SysInfoMonitor -lpci /usr/lib/x86_64-linux-gnu/libOpenGL.so /usr/lib/x86_64-linux-gnu/libGLX.so)
target_link_libraries(SysInfoMonitor ${OPENGL_LIBRARIES})

if(NOT LIBPCI_LIBRARY)
    message(FATAL_ERROR "libpci not found")
else()
    message(STATUS "libpci found at: ${LIBPCI_LIBRARY}")
endif()

# Линковка с libpci




