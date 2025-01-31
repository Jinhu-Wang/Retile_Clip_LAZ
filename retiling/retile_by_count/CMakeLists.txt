
cmake_minimum_required(VERSION 3.20)

get_filename_component(PROJECT_NAME ${CMAKE_CURRENT_SOURCE_DIR} NAME)
project(${PROJECT_NAME})

add_compile_definitions("DATA_DIR=\"${CMAKE_CURRENT_DOURCE_DIR}/data\"")

add_subdirectory(src)

