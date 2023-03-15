cmake_minimum_required(VERSION 3.20)

project(SynaesthetiQ)

add_library(../../rpi_ws281x/build/libws2811.a)

add_executable(SynaesthetiQ main.c synaesthetiq.cpp)