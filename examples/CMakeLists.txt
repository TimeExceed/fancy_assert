add_executable(not_trigger
    not_trigger.cpp
)
target_link_libraries(not_trigger fassert)

add_executable(trigger
    trigger.cpp
)
target_link_libraries(trigger fassert)

add_executable(customized-sink
    customized-sink.cpp
)
target_link_libraries(customized-sink fassert)

add_executable(what
    what.cpp
)
target_link_libraries(what fassert)
