add_executable(not_trigger
    not_trigger.cpp
)
target_link_libraries(not_trigger fassert)
target_include_directories(not_trigger
    PUBLIC
        ../src
)

add_executable(trigger
    trigger.cpp
)
target_link_libraries(trigger fassert)
target_include_directories(trigger
    PUBLIC
        ../src
)

add_executable(cleanup
    cleanup.cpp
)
target_link_libraries(cleanup fassert)
target_include_directories(cleanup
    PUBLIC
        ../src
)

add_executable(what
    what.cpp
)
target_link_libraries(what fassert)
target_include_directories(what
    PUBLIC
        ../src
)
