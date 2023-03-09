cmake_minimum_required(VERSION 3.0.0)
project(babel VERSION 0.1.0)
set (CMAKE_CXX_STANDARD 17)

# Conan setup
if(EXISTS ${CMAKE_BINARY_DIR}/conan_paths.cmake)
	include(${CMAKE_BINARY_DIR}/conan_paths.cmake)
else()
	message(FATAL_ERROR "Please run 'conan install ..' before.")
endif()

# Get packages
find_package(portaudio)
find_package(opus)
find_package(asio)
find_package(sqlite3)
find_package(Qt5 COMPONENTS Widgets Core Gui REQUIRED)

# For VSCode compatibility :3
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# For common folder
include_directories("common/include")

# Build programs
add_subdirectory(server)
add_subdirectory(client)