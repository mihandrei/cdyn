cmake_minimum_required(VERSION 2.8.4)
find_package(PkgConfig REQUIRED)

project(concrete_osc)

set(CMAKE_BUILD_TYPE DEBUG)
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -std=c99")

add_library(coupled STATIC
    dyn.c
    coupled.c)


##################
#pkg_search_module(CAIRO REQUIRED cairo)  #cact nu mere

include_directories("/usr/include/cairo")

add_executable(cairogui cairogui.c)

target_link_libraries(cairogui coupled cairo)

