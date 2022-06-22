add_library(fassert STATIC
    fassert.cpp
)

target_link_libraries(fassert
PUBLIC
    prettyprint
)

target_include_directories(fassert
PUBLIC
    .
)
