cmake_minimum_required(VERSION 2.8) # Проверка версии CMake.

project(final)
	
set(MAIN_CPP main.cpp)
set(LIB_HTML_CPP html_parcer.cpp)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

add_library(html_parcer ${LIB_HTML_CPP})
add_executable(final ${MAIN_CPP})

target_link_libraries(final html_parcer ev pthread)
