cmake_minimum_required(VERSION 2.8 FATAL_ERROR)

project(imagesubstractgrabcutgui)

#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fopenmp")

FILE(GLOB GHEADERS "*.h*")
FILE(GLOB GSOURCES "*.cpp")

find_package( OpenCV REQUIRED )

set(Boost_USE_STATIC_LIBS ON)
find_package(Boost 1.5 COMPONENTS program_options system filesystem )
include_directories(${Boost_INCLUDE_DIRS})
link_directories(${Boost_LIBRARY_DIRS} )

add_executable( main ${GSOURCES} ${GHEADERS} )
# ${Boost_LIBRARIES}
target_link_libraries( main ${OpenCV_LIBS} ${Boost_LIBRARIES})
