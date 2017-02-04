cmake_minimum_required(VERSION 2.8 FATAL_ERROR)

project(dataset-tools)

#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fopenmp -march=native")
#set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fopenmp -march=native")

FILE(GLOB GHEADERS "substract/*.h*")
FILE(GLOB GSOURCES "substract/*.cpp")

FILE(GLOB GUIHEADERS "gui/*.h*")
FILE(GLOB GUISOURCES "gui/*.cpp")

FILE(GLOB GUIPAGESHEADERS "gui/pages/*.h*")
FILE(GLOB GUIPAGESSOURCES "gui/pages/*.cpp")

FILE(GLOB GCAPTUREHEADERS "capture/*.h*")
FILE(GLOB GCAPTURESOURCES "capture/*.cpp")

FILE(GLOB GVIEWERHEADERS "3dviewer/*.h*")
FILE(GLOB GVIEWERSOURCES "3dviewer/*.cpp")

#Python tools
FILE(COPY ${CMAKE_CURRENT_SOURCE_DIR}/py-pngIndexed/indexPngDataset.py DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

#Find PCL
find_package(PCL 1.7 REQUIRED)
include_directories(${PCL_INCLUDE_DIRS})
link_directories(${PCL_LIBRARY_DIRS})
add_definitions(${PCL_DEFINITIONS})

#Find QT
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)
find_package(Qt5Core)
find_package(Qt5Widgets)

#Find opencv
find_package( OpenCV REQUIRED )

#Find boost
set(Boost_USE_STATIC_LIBS ON)
find_package(Boost 1.5 COMPONENTS program_options system filesystem regex chrono timer)
include_directories(${Boost_INCLUDE_DIRS})
link_directories(${Boost_LIBRARY_DIRS} )

add_executable( imagesubstract ${GSOURCES} ${GHEADERS} )
target_link_libraries( imagesubstract ${OpenCV_LIBS} ${Boost_LIBRARIES} )

add_executable( main ${GUISOURCES} ${GUIHEADERS} ${GUIPAGESHEADERS} ${GUIPAGESSOURCES} )
target_link_libraries( main Qt5::Widgets Qt5::Core )

add_executable(capture ${GCAPTUREHEADERS} ${GCAPTURESOURCES})
target_link_libraries(capture ${PCL_LIBRARIES} ${Boost_LIBRARIES})

add_executable(viewer ${GVIEWERHEADERS} ${GVIEWERSOURCES})
target_link_libraries(viewer ${PCL_LIBRARIES} ${Boost_LIBRARIES})