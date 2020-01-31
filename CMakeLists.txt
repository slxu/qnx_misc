cmake_minimum_required(VERSION 3.5.1)

project(qnx_misc)

# by default build debug version
if(NOT DEFINED CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Debug)
endif(NOT DEFINED CMAKE_BUILD_TYPE)

add_executable(hello_qnx
    src/hello_qnx.cpp
)
add_executable(tcp_speed_server
    src/tcp_speed_server.cpp
)
add_executable(tcp_speed_client
    src/tcp_speed_client.cpp
)
add_executable(udp_speed_receiver
    src/udp_speed_receiver.cpp
)
add_executable(udp_speed_sender
    src/udp_speed_sender.cpp
)

target_link_libraries(hello_qnx PRIVATE -pthread)
target_link_libraries(tcp_speed_server PRIVATE -pthread -lsocket)
target_link_libraries(tcp_speed_client PRIVATE -pthread -lsocket)
target_link_libraries(udp_speed_receiver PRIVATE -pthread -lsocket)
target_link_libraries(udp_speed_sender PRIVATE -pthread -lsocket)
