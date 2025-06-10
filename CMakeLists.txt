cmake_minimum_required(VERSION 3.22)
project(FANCY_ASSERT VERSION 2.0)

add_compile_options(
    $<$<COMPILE_LANGUAGE:CXX>:--std=c++20> $<$<COMPILE_LANGUAGE:C>:--std=c17>
    -pthread -fwrapv -Wall)
add_link_options(-fuse-ld=mold)

add_compile_options(-O0 -g -fsanitize=address)
add_link_options(-fsanitize=address)

if(${FMTLIB} STREQUAL "std")
    add_compile_definitions(ENABLE_STD_FORMAT)
elseif(${FMTLIB} STREQUAL "fmtlib")
    find_package(fmt REQUIRED)
    add_compile_definitions(ENABLE_FMTLIB)
endif()

add_subdirectory(src)

if(${FMTLIB} STREQUAL "fmtlib")
    target_link_libraries(fassert
    PRIVATE
        fmt::fmt)
endif()

add_subdirectory(examples)
