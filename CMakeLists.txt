cmake_minimum_required(VERSION 3.1)

add_library(msgq message.c client.c)
set_target_properties(msgq PROPERTIES PUBLIC_HEADER "client.h;message.h")

add_executable(broker broker.c)
target_link_libraries(broker msgq)

add_executable(sample sample.c)
target_link_libraries(sample msgq)

install(TARGETS sample broker msgq 
    RUNTIME DESTINATION ${CMAKE_INSTALL_PREFIX}/bin
    ARCHIVE DESTINATION ${CMAKE_INSTALL_PREFIX}/lib
    PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_PREFIX}/include
)

