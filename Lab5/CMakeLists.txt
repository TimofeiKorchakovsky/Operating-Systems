cmake_minimum_required(VERSION 3.12)
project(Lab5)

set(CMAKE_CXX_STANDARD 14)

set(SOURCES
    Server.cpp
    Client.cpp
)

add_executable(NamedPipeCommunication ${SOURCES})
