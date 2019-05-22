project(test_formatplusplus)
find_package(GTest REQUIRED)

add_compile_options(-Wall -pedantic)
add_executable(test_formatplusplus test.cpp test_main.cpp)
target_link_libraries(test_formatplusplus formatplusplus gtest pthread)
