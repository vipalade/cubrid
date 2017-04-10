string(TOLOWER "${CMAKE_BUILD_TYPE}" BOOST_BUILD_TYPE)

if(WITH_BOOST MATCHES "^http")
  set(boost_PREFIX ${CMAKE_BINARY_DIR}/external)
  if(CMAKE_SYSTEM MATCHES "Windows*")
      ExternalProject_Add(
          build_boost
          EXCLUDE_FROM_ALL 1
          PREFIX ${boost_PREFIX}
          URL "${WITH_BOOST}"
          URL_MD5 "7b493c08bc9557bbde7e29091f28b605"
          INSTALL_DIR ${boost_PREFIX}
          UPDATE_COMMAND ""
          CONFIGURE_COMMAND "bootstrap.bat"
          #BUILD_COMMAND bjam --with-system --with-thread toolset=msvc-14.0 link=static variant=release threading=multi --prefix=${boost_PREFIX} --layout=system stage install
          BUILD_COMMAND bjam --with-system --with-thread --with-program-options --with-date_time toolset=msvc-14.0 address-model=64 variant=${BOOST_BUILD_TYPE} link=static threading=multi --prefix=${boost_PREFIX} install
          INSTALL_COMMAND ""
          BUILD_IN_SOURCE 1
          LOG_UPDATE ON
          LOG_CONFIGURE ON
          LOG_BUILD ON
          LOG_INSTALL ON
      )
      
      set(Boost_INCLUDE_DIRS ${boost_PREFIX}/include/boost-1_63)
      
      #set(BOOST_SYSTEM_LIB libboost_system)
      #set(BOOST_THREAD_LIB libboost_thread)
  else()
      ExternalProject_Add(
          build_boost
          EXCLUDE_FROM_ALL 1
          PREFIX ${boost_PREFIX}
          URL "${WITH_BOOST}"
          #URL_MD5 "6f4571e7c5a66ccc3323da6c24be8f05"
          CONFIGURE_COMMAND ./bootstrap.sh --with-libraries=system,thread,program_options --prefix=${boost_PREFIX}
          BUILD_COMMAND ./bjam link=static variant=${BOOST_BUILD_TYPE} threading=multi cxxflags='-fPIC'
          INSTALL_COMMAND ./bjam link=static install
          BUILD_IN_SOURCE 1
          LOG_UPDATE ON
          LOG_CONFIGURE ON
          LOG_BUILD ON
          LOG_INSTALL ON
      )
      set(BOOST_SYSTEM_LIB boost_system)
      set(BOOST_THREAD_LIB boost_thread)
      set(BOOST_PROGRAM_OPTIONS_LIB boost_program_options)
      set(OTHER_LIBS pthread)
      set(Boost_INCLUDE_DIRS ${boost_PREFIX}/include)
          
  endif()
  set(BOOST_ROOT "${boost_PREFIX}")
  set(Boost_FOUND TRUE)
  set(Boost_SYSTEM_LIBRARY boost_system)
else()
  add_custom_target(build_boost)
  
  set(BOOST_ROOT "${WITH_BOOST}")
  set(Boost_INCLUDE_DIRS "${BOOST_ROOT}/include")
  set(Boost_USE_STATIC_LIBS ON)
  set(Boost_USE_MULTITHREADED ON)
  set(Boost_USE_STATIC_RUNTIME ON)

  find_package(Boost
    COMPONENTS
      system
      asio
  )
endif()

