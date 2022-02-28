add_executable(UtestConstructor utest_constructor.cpp)
target_link_libraries(UtestConstructor PUBLIC Volume)
add_test(NAME cvolume_constructor COMMAND UtestConstructor)

add_executable(UtestSetGet utest_setget.cpp)
target_link_libraries(UtestSetGet PUBLIC Volume)
add_test(NAME cvolume_setget COMMAND UtestSetGet)

add_executable(UtestPositions utest_pos.cpp)
target_link_libraries(UtestPositions PUBLIC Volume)
add_test(NAME cvolume_pos COMMAND UtestPositions)

add_executable(UtestArrayops utest_arrayops.cpp)
target_link_libraries(UtestArrayops PUBLIC Volume)
add_test(NAME cvolume_arrayops COMMAND UtestArrayops)

add_executable(UtestNormalize utest_normalize.cpp)
target_link_libraries(UtestNormalize PUBLIC Volume)
add_test(NAME cvolume_normalize COMMAND UtestNormalize)

add_executable(UtestRandom utest_rand.cpp)
target_link_libraries(UtestRandom PUBLIC Volume)
add_test(NAME cvolume_random COMMAND UtestRandom)

add_executable(UtestCopyConst utest_copyconst.cpp)
target_link_libraries(UtestCopyConst PUBLIC Volume)
add_test(NAME cvolume_copyconst COMMAND UtestCopyConst)

add_executable(UtestCrop utest_crop.cpp)
target_link_libraries(UtestCrop PUBLIC Volume)
add_test(NAME cvolume_crop COMMAND UtestCrop)

