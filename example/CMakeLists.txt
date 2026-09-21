set(EXAMPLES
    average
    binary
    duffs_device
    fibonacci
    streams
)

foreach(EX ${EXAMPLES})
    add_executable(example_${EX} ${EX}.c)
    target_link_libraries(example_${EX} PRIVATE order::order)
    target_include_directories(example_${EX} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
    target_compile_features(example_${EX} PRIVATE c_std_99)
endforeach()
