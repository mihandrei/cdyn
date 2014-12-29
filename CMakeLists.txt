cmake_minimum_required(VERSION 2.8.4)
find_package(PkgConfig REQUIRED)

project(concrete_osc)

set(CMAKE_BUILD_TYPE DEBUG)
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -std=c99")

add_library(coupled SHARED
    lib/coupled.c)


##################
#pkg_search_module(CAIRO REQUIRED cairo)  #cact nu mere

include_directories("/usr/include/cairo")

add_executable(cairogui cairogui.c)

target_link_libraries(cairogui coupled cairo)

####gl stupidity#####
#pkg_search_module(GLFW REQUIRED glfw3)
#pkg_search_module(GLEW REQUIRED glew)

#find_package(OpenGL REQUIRED)

#include_directories(${GLFW_INCLUDE_DIRS})
#include_directories(${GLEW_INCLUDE_DIRS})

#add_executable(glui meh/ui.c)

#target_link_libraries(glui
#    m
#    coupled
#    ${OPENGL_LIBRARY}
#    ${GLFW_LIBRARIES}
#    ${GLEW_LIBRARIES})
