cmake_minimum_required(VERSION 3.12)

# set project name
project(CVolume 	VERSION 1.0
	DESCRIPTION "Small library to handle and process volumetric datasets"
	LANGUAGES CXX)

# prepare for cuda compilation
set(BUILD_SHARED_LIBS OFF)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_subdirectory(src/)
add_subdirectory(lib/)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED true)

# here comes all stugg related to unit testing
add_subdirectory(utests/)

