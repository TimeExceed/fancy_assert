cmake_minimum_required(VERSION 3.0)
project(FANCY_ASSERT VERSION 1.0)

add_compile_options(-pthread)
add_link_options(-pthread)

add_subdirectory(src)
