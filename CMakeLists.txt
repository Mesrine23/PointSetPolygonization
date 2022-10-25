cmake_minimum_required(VERSION 3.1...3.23)
project(PointSetPolygonization)

find_package(CGAL REQUIRED)

set(CMAKE_BUILD_TYPE Debug)
set(CMAKE_BUILD_TYPE RelWithDebInfo)

# create a target per cppfile
file(
        GLOB cppfiles
        RELATIVE ${CMAKE_CURRENT_SOURCE_DIR}
        ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp)
foreach(cppfile ${cppfiles})
    create_single_source_cgal_program("${cppfile}")
endforeach()