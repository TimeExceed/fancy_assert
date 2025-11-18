cmake_minimum_required(VERSION 3.22)
project(FANCY_ASSERT VERSION 2.0.1)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_C_STANDARD 17)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS OFF)

# add_link_options(-fuse-ld=mold)

add_compile_options(-O0 -g -fsanitize=address)
add_link_options(-fsanitize=address)

if(${FMTLIB} STREQUAL "std")
    add_compile_definitions(ENABLE_STD_FORMAT)
elseif(${FMTLIB} STREQUAL "fmtlib")
    find_package(fmt REQUIRED)
    add_compile_definitions(ENABLE_FMTLIB)
endif()

if(${FMTLIB} STREQUAL "fmtlib")
    target_link_libraries(fassert
    PRIVATE
        fmt::fmt)
endif()

add_subdirectory(src)
add_subdirectory(examples)
