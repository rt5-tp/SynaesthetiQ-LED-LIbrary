cmake_minimum_required(VERSION 3.18.4)

project(Test)

set(SOURCES TestColour.cpp)

add_executable(test ${SOURCES})

target_link_libraries(test gtest pthread)

