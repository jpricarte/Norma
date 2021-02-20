cmake_minimum_required(VERSION 3.17)
project(Norma)

set(CMAKE_CXX_STANDARD 14)

add_executable(Norma src/main.cpp src/Machine/Machine.cpp src/Machine/Machine.h src/Intepreter/Intepreter.cpp src/Intepreter/Intepreter.h src/Enums/Operations.h src/Enums/OperationTypes.h src/AuxiliarClasses/ParsedOperation.cpp src/AuxiliarClasses/ParsedOperation.h src/AuxiliarClasses/Subrotine.cpp src/AuxiliarClasses/Subrotine.h src/Compiler/Compiler.cpp src/Compiler/Compiler.h)