project(test_formatplusplus)
find_package(GTest REQUIRED)

add_executable(test_formatplusplus test.cpp test_main.cpp)
target_link_libraries(test_formatplusplus formatplusplus gtest pthread)
