
cmake_minimum_required(VERSION 3.20)

set(CMAKE_CXX_STANDARD 17)

get_filename_component(PROJECT_NAME ${CMAKE_CURRENT_SOURCE_DIR} NAME)

project(${PROJECT_NAME})

set(LAStools_DIR /home/jinhu/Work/Projects/Jinhu-Wang/3rdParty/LAStools/)
include_directories(${LAStools_DIR}/LASlib/inc/)
include_directories(${LAStools_DIR}/LASzip/src/)

link_directories(${LAStools_DIR}/LASlib/lib/)

add_executable(RetileByCount
                retile_by_count.cpp 
                retile_las.h 
                retile_las.cpp)

target_link_libraries(RetileByCount 
                libLASlib.a)

