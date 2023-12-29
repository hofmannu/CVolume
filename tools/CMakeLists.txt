# a collection of helper tools to e.g. convert from one format to another from
# the command line

add_executable(Converter converter.cpp)
target_link_libraries(Converter PUBLIC Volume)
target_include_directories(Converter PRIVATE ${CMAKE_SOURCE_DIR}/src)
