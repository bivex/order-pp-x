add_executable(test_order test_order.c)
target_link_libraries(test_order PRIVATE order::order)
target_compile_features(test_order PRIVATE c_std_99)
add_test(NAME test_order COMMAND test_order)

add_executable(test_order_cpp test_order_cpp.cpp)
target_link_libraries(test_order_cpp PRIVATE order::order)
target_compile_features(test_order_cpp PRIVATE cxx_std_11)
add_test(NAME test_order_cpp COMMAND test_order_cpp)
