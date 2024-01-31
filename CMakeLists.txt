cmake_minimum_required(VERSION 3.20)
project(ds)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(SOURCE_FILES
	main.cpp
	"vector.h")

add_executable(ds ${SOURCE_FILES})
