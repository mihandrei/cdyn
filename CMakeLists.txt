cmake_minimum_required(VERSION 2.8.4)
project(concrete_osc)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE_FILES
    dyn.c
    coupled.c
    main.c)

add_executable(concrete_osc ${SOURCE_FILES})