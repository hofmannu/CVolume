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
add_subdirectory(utests/)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED true)

add_test(NAME cvolume_constructor COMMAND UtestConstructor)
add_test(NAME cvolume_setget COMMAND UtestSetGet)
add_test(NAME cvolume_pos COMMAND UtestPositions)
add_test(NAME cvolume_arrayops COMMAND UtestArrayops)
add_test(NAME cvolume_normalize COMMAND UtestNormalize)
add_test(NAME cvolume_random COMMAND UtestRandom)

enable_testing()