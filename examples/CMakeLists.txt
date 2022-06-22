add_executable(not_trigger
    not_trigger.cpp
)
target_link_libraries(not_trigger fassert)

add_executable(trigger
    trigger.cpp
)
target_link_libraries(trigger fassert)

add_executable(cleanup
    cleanup.cpp
)
target_link_libraries(cleanup fassert)

add_executable(what
    what.cpp
)
target_link_libraries(what fassert)
