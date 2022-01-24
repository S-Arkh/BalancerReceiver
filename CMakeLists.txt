cmake_minimum_required(VERSION 3.1)

project(Server_test)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++1y -Wall -Wextra -pthread")

add_executable(Balancer_receiver main.cpp)
