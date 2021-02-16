cmake_minimum_required(VERSION 3.17)
project(Norma)

set(CMAKE_CXX_STANDARD 14)

add_executable(Norma main.cpp Classes/Machine.cpp Classes/Machine.h Classes/Intepreter.cpp Classes/Intepreter.h)