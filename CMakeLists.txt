cmake_minimum_required(VERSION 3.0.0)
project(babel VERSION 0.1.0)

# Conan setup
if(EXISTS ${CMAKE_BINARY_DIR}/conan_paths.cmake)
	include(${CMAKE_BINARY_DIR}/conan_paths.cmake)
else()
	message(FATAL_ERROR "Please run 'conan install ..' before.")
endif()

# Get packages
find_package(portaudio)
find_package(Opus)
find_package(asio REQUIRED)
find_package(SQLite3 REQUIRED)
find_package(Qt5 COMPONENTS Widgets Core Gui REQUIRED)

# For VSCode compatibility :3
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# For common folder
include_directories("common/include")

# For debug owo
add_compile_options(-g)
add_compile_options(-fsanitize=address)
add_link_options(-fsanitize=address)

# Tidy
find_program(CLANG_TIDY_EXE NAMES "clang-tidy")

if(EXISTS ${CLANG_TIDY_EXE})
	set(CLANG_TIDY
		"${CLANG_TIDY_EXE}"
		"--config-file=${CMAKE_SOURCE_DIR}/.clang-tidy"
		"-header-filter=^.*(?:third-party).*gm")
	set(CMAKE_CXX_CLANG_TIDY "${CLANG_TIDY}")
endif()

set(COMMON_SOURCES
	${CMAKE_SOURCE_DIR}/common/src/Packet.cpp
	${CMAKE_SOURCE_DIR}/common/src/Error.cpp
)

# Build programs
add_subdirectory(server)
add_subdirectory(client)
