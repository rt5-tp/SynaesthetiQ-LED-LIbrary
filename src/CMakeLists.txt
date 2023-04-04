cmake_minimum_required(VERSION 3.18.4)

project(SynaesthetiQ)

set(SOURCES main.cpp SynaesthetiQ/synaesthetiq.cpp)

add_executable(SynaesthetiQ ${SOURCES})

# include directory for libws2811 library
include_directories(/usr/local/lib)

target_link_libraries(SynaesthetiQ /usr/local/lib/libws2811.a)

set(HEADERS SynaesthetiQ/synaesthetiq.hpp SynaesthetiQ/Colour.hpp)

target_include_directories(SynaesthetiQ PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
