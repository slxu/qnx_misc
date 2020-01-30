cmake_minimum_required(VERSION 3.5.1)

project(qnx_misc)

# by default build debug version
if(NOT DEFINED CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Debug)
endif(NOT DEFINED CMAKE_BUILD_TYPE)

add_executable(hello_qnx
    src/hello_qnx.cpp
)
add_executable(net_speed_server
    src/net_speed_server.cpp
)
add_executable(net_speed_client
    src/net_speed_client.cpp
)

target_link_libraries(hello_qnx PRIVATE -pthread)
target_link_libraries(net_speed_server PRIVATE -pthread -lsocket)
target_link_libraries(net_speed_client PRIVATE -pthread -lsocket)
