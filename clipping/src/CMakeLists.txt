
cmake_minimum_required(VERSION 3.20)

set(CMAKE_CXX_STANDARD 17)

get_filename_component(PROJECT_NAME ${CMAKE_CURRENT_SOURCE_DIR} NAME)

project(${PROJECT_NAME})

set(LAStools_DIR /home/jinhu/Work/Projects/Jinhu-Wang/3rdParty/LAStools/)
set(shapelib_DIR /home/jinhu/Work/Projects/Jinhu-Wang/3rdParty/shapelib/)
include_directories(${LAStools_DIR}/LASlib/inc/)
include_directories(${LAStools_DIR}/LASzip/src/)
include_directories(${shapelib_DIR}/)

link_directories(${LAStools_DIR}/LASlib/lib/)
link_directories(${shapelib_DIR}/build1/)

add_executable(ClipLas
                main.cpp 
                clip_las.h 
                clip_las.cpp
                vec.h 
                box.h 
                polygon.h 
                types.h)

target_link_libraries(ClipLas
                libLASlib.a
                libshp.a)
