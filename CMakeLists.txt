cmake_minimum_required(VERSION 3.13)

project(RayScan-JPS
	VERSION 0.0.1
	LANGUAGES CXX C)

find_package(Boost REQUIRED)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED TRUE)

include(cmake/warthog.cmake)
warthog_module_declare(warthog-core v0.5.0)
warthog_module_declare(warthog-jps v0.5.1)
warthog_module(warthog-core)
warthog_module(warthog-jps)

add_library(mylib)
target_include_directories(mylib PUBLIC include)
include(cmake/headers.cmake)

add_executable(RayScan-jps main.cpp)	

add_subdirectory(src)

target_link_libraries(RayScan-jps PUBLIC mylib)
target_link_libraries(RayScan-jps PUBLIC Boost::headers)
target_link_libraries(mylib PUBLIC warthog::libjps)
# add_subdirectory(include)