include_directories(src/cci)
add_executable(test_basic test_basic.cpp)
target_link_libraries(test_basic cascci)
