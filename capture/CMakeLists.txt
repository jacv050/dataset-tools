cmake_minimum_required(VERSION 2.8 FATAL_ERROR)

project(pcd_record)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fopenmp")

FILE(GLOB GHEADERS "*.h*")
FILE(GLOB GSOURCES "*.cpp")

FILE(COPY ${CMAKE_CURRENT_SOURCE_DIR}/setup.sh DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

#set(GHEADERS Grabber.h)
#set(GSOURCES Grabber.cpp main.cpp)
 
find_package(PCL 1.7 REQUIRED)

find_package(Boost 1.5 COMPONENTS program_options timer thread chrono regex)
include_directories(${Boost_INCLUDE_DIR})
link_directories(${Boost_LIBRARY_DIR})

include_directories(${PCL_INCLUDE_DIRS})
link_directories(${PCL_LIBRARY_DIRS})
add_definitions(${PCL_DEFINITIONS})

find_package(CUDA REQUIRED)
include_directories(${CUDA_TOOLKIT_INCLUDE})
link_directories(${CUDA_CUDART_LIBRARY})

add_executable (main  ${GHEADERS} ${GSOURCES})
target_link_libraries (main ${PCL_LIBRARIES} ${Boost_LIBRARIES} ${CUDA_CUDART_LIBRARY})
