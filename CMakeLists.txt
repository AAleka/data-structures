cmake_minimum_required(VERSION 3.20)
project(easylife)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(SOURCE_FILES
	main.cpp
	"vector.h")

add_executable(easylife ${SOURCE_FILES})
