add_executable (async_echo_server async_echo_server.c)

target_link_libraries(async_echo_server cubio)

add_executable (timer_async_echo_server timer_async_echo_server.c)

target_link_libraries(timer_async_echo_server cubio)

add_executable (sync_echo_server sync_echo_server.cpp)

target_link_libraries(sync_echo_server cubio)

add_executable (async_echo_client async_echo_client.cpp)

target_link_libraries(async_echo_client cubio)
target_link_libraries(async_echo_client ${BOOST_SYSTEM_LIB} ${BOOST_PROGRAM_OPTIONS_LIB} pthread)
