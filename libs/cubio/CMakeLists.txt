if(${Boost_FOUND})
  include_directories("${Boost_INCLUDE_DIRS}")
  link_directories(${BOOST_ROOT}/lib)

  add_library(cubio STATIC
    cubio.h
    src/cubio.cpp
  )

  add_dependencies(cubio build_boost)

  target_link_libraries(cubio ${BOOST_SYSTEM_LIB} ${BOOST_ASIO_LIB} pthread)

  add_subdirectory(sample)
  add_subdirectory(test)

endif()
