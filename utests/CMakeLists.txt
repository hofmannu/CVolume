add_executable(UtestConstructor utest_constructor.cpp)
target_link_libraries(UtestConstructor PUBLIC Volume)

add_executable(UtestSetGet utest_setget.cpp)
target_link_libraries(UtestSetGet PUBLIC Volume)

add_executable(UtestPositions utest_pos.cpp)
target_link_libraries(UtestPositions PUBLIC Volume)

add_executable(UtestArrayops utest_arrayops.cpp)
target_link_libraries(UtestArrayops PUBLIC Volume)

add_executable(UtestNormalize utest_normalize.cpp)
target_link_libraries(UtestNormalize PUBLIC Volume)

add_executable(UtestRandom utest_rand.cpp)
target_link_libraries(UtestRandom PUBLIC Volume)

add_executable(UtestCopyConst utest_copyconst.cpp)
target_link_libraries(UtestCopyConst PUBLIC Volume)

add_executable(UtestCrop utest_crop.cpp)
target_link_libraries(UtestCrop PUBLIC Volume)

add_executable(UtestMultoperator utest_multoperator.cpp)
target_link_libraries(UtestMultoperator PUBLIC Volume)

add_executable(UtestAddoperator utest_addoperator.cpp)
target_link_libraries(UtestAddoperator PUBLIC Volume)

add_executable(UtestSubsoperator utest_subsoperator.cpp)
target_link_libraries(UtestSubsoperator PUBLIC Volume)

add_executable(UtestDivoperator utest_divoperator.cpp)
target_link_libraries(UtestDivoperator PUBLIC Volume)

add_executable(UtestBracketmagic utest_bracketmagic.cpp)
target_link_libraries(UtestBracketmagic PUBLIC Volume)
