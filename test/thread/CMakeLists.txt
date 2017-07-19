if (CMAKE_VERSION VERSION_LESS "3.1")
  set(ENABLED_TEST FALSE)
else()
  #message("Supported features = ${CMAKE_CXX_COMPILE_FEATURES}")
  if(";${CMAKE_CXX_COMPILE_FEATURES};" MATCHES ";cxx_std_11;")
    set(ENABLED_TEST TRUE)
  endif()
endif()


if(ENABLED_TEST)
  include_directories(${CMAKE_BINARY_DIR}/external/include/lzo)
  set(SRC_BASE_DIR ${CMAKE_SOURCE_DIR}/src)

  set( ThreadTestSuite
      test_critical_section_basic.cpp
  )

  create_test_sourcelist( ThreadTests test_thread.cpp ${ThreadTestSuite})

  add_executable(test_thread ${ThreadTests})

  target_link_libraries(test_thread
    cubrid
  )

  add_test(NAME TestCSBasic     COMMAND  test_thread test_critical_section_basic)

endif(ENABLED_TEST)
