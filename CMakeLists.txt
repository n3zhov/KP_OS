cmake_minimum_required(VERSION 3.17)
project(KP_OS)

set(CMAKE_CXX_STANDARD 14)

set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)

set(ZMQ_INCLUDE_DIR /usr/include/zmqpp)

find_library(cZeroMQ NAMES zmq PATHS /usr/lib/x86_64-linux-gnu)
find_library(ZeroMQ NAMES zmqpp PATHS /usr/lib/x86_64-linux-gnu)


add_executable(server_exec server_exec.cpp)
add_executable(client_exec client_exec.cpp)


add_library(
        server STATIC
        src/server.cpp
)


add_library(
        client STATIC
        src/client.cpp
)

set_target_properties(client PROPERTIES
        PRIVATE_HEADER src/client.h
        )

set_target_properties(server PROPERTIES
        PRIVATE_HEADER src/server.h
        )

target_include_directories(server PUBLIC ${ZMQ_INCLUDE_DIR})
target_include_directories(client PUBLIC ${ZMQ_INCLUDE_DIR})

target_include_directories(client_exec PUBLIC src/)
target_include_directories(server_exec PUBLIC src/)

add_dependencies(client_exec client)
add_dependencies(server_exec server)

target_link_libraries(server ${ZeroMQ} ${cZeroMQ} Threads::Threads)
target_link_libraries(client ${ZeroMQ} ${cZeroMQ} Threads::Threads)

target_link_libraries(client_exec client)
target_link_libraries(server_exec server)

