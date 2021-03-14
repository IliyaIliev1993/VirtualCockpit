cmake_minimum_required(VERSION 3.1)

# Specify compilers
if ("${CMAKE_C_COMPILER}" STREQUAL "")
    set(CMAKE_C_COMPILER clang-3.9 CACHE INTERNAL "")
    message(STATUS "No C compiler was specified. Setting to ${CMAKE_C_COMPILER}")
endif()
if ("${CMAKE_CXX_COMPILER}" STREQUAL "")
    set(CMAKE_CXX_COMPILER clang++-3.9 CACHE INTERNAL "")
    message(STATUS "No C++ compiler was specified. Setting to ${CMAKE_CXX_COMPILER}")
endif()

# Name of the project
project(VirtualCockpit)

# Specify linker flags - set CMAKE_CURRENT_BINARY_DIR to append the current build directory
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -Wl,-rpath,'$ORIGIN'" )
set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -Wl,-rpath,'$ORIGIN'" )

message(STATUS "CMAKE_CURRENT_SOURCE_DIR: ${CMAKE_CURRENT_SOURCE_DIR}")
message(STATUS "CMAKE_CURRENT_BINARY_DIR: ${CMAKE_CURRENT_BINARY_DIR}")

# Check if the build directory starts with build* prefix
if(NOT ${CMAKE_CURRENT_BINARY_DIR} MATCHES "${CMAKE_CURRENT_SOURCE_DIR}/build*")
    message(FATAL_ERROR "\n CMake needs to be run in a directory with prefix /build*")
endif()

# Output compiler Information and build type
message(STATUS "====== Current compiler is: ${CMAKE_CXX_COMPILER} ======")
message(STATUS "====================================")
if ("${CMAKE_BUILD_TYPE}" STREQUAL "")
    message(STATUS "No selected build type")
    message(STATUS "Setting to Debug")
    set(CMAKE_BUILD_TYPE "Debug")
else()
    message(STATUS "Selected build type is ${CMAKE_BUILD_TYPE}")
endif()
message(STATUS "====================================")

# Get all source files and headers
file(GLOB SOURCES
    VirtualCockpit/*.h
    VirtualCockpit/*.cpp
    VirtualCockpit/Font/*.hpp
    VirtualCockpit/Font/*.cpp
    VirtualCockpit/GLFW/*.hpp .h
    VirtualCockpit/GLFW/*.cpp
    VirtualCockpit/GLFW/glm/*.hpp
    VirtualCockpit/GLFW/glm/*.cpp
    VirtualCockpit/Learning/*.hpp
    VirtualCockpit/Learning/*.cpp
    VirtualCockpit/Renderer/*.hpp
    VirtualCockpit/Renderer/*.cpp
    VirtualCockpit/Texture/*.hpp
    VirtualCockpit/Texture/*.cpp
    VirtualCockpit/Timer_mgr/*.hpp
    VirtualCockpit/Timer_mgr/*.cpp
    main.cpp)

# Create executable PROJECT_NAME
add_executable(${PROJECT_NAME} ${SOURCES})

# Specify C++14 as the standard
set_property(TARGET ${PROJECT_NAME} PROPERTY CXX_STANDARD 14)
set (CMAKE_CXX_STANDARD 14)

# Add include directories
set(VIRTUAL_COCKPIT_ROOT_DIR ${CMAKE_CURRENT_SOURCE_DIR})
set(GL_INCLUDES ${VIRTUAL_COCKPIT_ROOT_DIR}/GLFW)
set(INTERNAL_DIRS
    ${VIRTUAL_COCKPIT_ROOT_DIR}/VirtualCockpit
    ${VIRTUAL_COCKPIT_ROOT_DIR}/VirtualCockpit/Font
    ${VIRTUAL_COCKPIT_ROOT_DIR}/VirtualCockpit/Renderer
    ${VIRTUAL_COCKPIT_ROOT_DIR}/VirtualCockpit/Texture
    ${VIRTUAL_COCKPIT_ROOT_DIR}/VirtualCockpit/TimerMgr)
set(GL_INCLUDE_DIRS
    ${GL_INCLUDES}
    ${GL_INCLUDES}/glm
    ${GL_INCLUDES}/KHR
    ${GL_INCLUDES}/shader
    ${GL_INCLUDES}/stb_image)
include_directories(${INTERNAL_DIRS})
include_directories(${GL_INCLUDE_DIRS})

#find required libraries
find_package(glfw3 REQUIRED)
find_package(OpenGL REQUIRED)
find_package(Freetype REQUIRED)

#add library glad
add_library(Glad ${GL_INCLUDES}/glad.h ${GL_INCLUDES}/glad.c)

#add library project
add_library(Prj ${VIRTUAL_COCKPIT_ROOT_DIR}/Font/Font.hpp
                ${VIRTUAL_COCKPIT_ROOT_DIR}/Font/Font.cpp
                ${VIRTUAL_COCKPIT_ROOT_DIR}/Learning/BoardComputer.hpp
                ${VIRTUAL_COCKPIT_ROOT_DIR}/Learning/BoardComputer.cpp
                ${VIRTUAL_COCKPIT_ROOT_DIR}/Learning/IndicatorKMH.hpp
                ${VIRTUAL_COCKPIT_ROOT_DIR}/Learning/IndicatorKMH.cpp
                ${VIRTUAL_COCKPIT_ROOT_DIR}/Learning/IndicatorRPM.hpp
                ${VIRTUAL_COCKPIT_ROOT_DIR}/Learning/IndicatorRPM.cpp
                ${VIRTUAL_COCKPIT_ROOT_DIR}/Learning/MiddleMenu.hpp
                ${VIRTUAL_COCKPIT_ROOT_DIR}/Learning/MiddleMenu.cpp
                ${VIRTUAL_COCKPIT_ROOT_DIR}/Learning/Easing/Sine.h
                ${VIRTUAL_COCKPIT_ROOT_DIR}/Learning/Easing/Sine.cpp
                ${VIRTUAL_COCKPIT_ROOT_DIR}/Renderer/Renderer.hpp
                ${VIRTUAL_COCKPIT_ROOT_DIR}/Renderer/Renderer.cpp
                ${VIRTUAL_COCKPIT_ROOT_DIR}/Texture/Texture.hpp
                ${VIRTUAL_COCKPIT_ROOT_DIR}/Texture/Texture.cpp
                ${VIRTUAL_COCKPIT_ROOT_DIR}/TimerMgr/TimerMgr.hpp
                ${VIRTUAL_COCKPIT_ROOT_DIR}/TimerMgr/TimerMgr.cpp
                ${VIRTUAL_COCKPIT_ROOT_DIR}/TimerMgr/TimerProcess.hpp
                ${VIRTUAL_COCKPIT_ROOT_DIR}/TimerMgr/TimerProcess.cpp)


#link all libraries to executable
target_link_libraries(${PROJECT_NAME} glfw GL Glad freetype X11 pthread Xrandr Prj
                        Xi ${CMAKE_DL_LIBS} stdc++fs)

