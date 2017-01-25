cmake_minimum_required(VERSION 2.8 FATAL_ERROR)

project(imagesubstractgrabcutgui)

#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fopenmp -march=native")
#set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fopenmp -march=native")

FILE(GLOB GHEADERS "*.h*")
FILE(GLOB GSOURCES "*.cpp")

FILE(GLOB GUIHEADERS "gui/*.h*")
FILE(GLOB GUISOURCES "gui/*.cpp")

FILE(GLOB GUIPAGESHEADERS "gui/pages/*.h*")
FILE(GLOB GUIPAGESSOURCES "gui/pages/*.cpp")

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)
find_package(Qt5Core)
find_package(Qt5Widgets)

find_package( OpenCV REQUIRED )

set(Boost_USE_STATIC_LIBS ON)
find_package(Boost 1.5 COMPONENTS program_options system filesystem )
include_directories(${Boost_INCLUDE_DIRS})
link_directories(${Boost_LIBRARY_DIRS} )

add_executable( imagesubstract ${GSOURCES} ${GHEADERS} )
target_link_libraries( imagesubstract ${OpenCV_LIBS} ${Boost_LIBRARIES} )

add_executable( main ${GUISOURCES} ${GUIHEADERS} ${GUIPAGESHEADERS} ${GUIPAGESSOURCES} )
target_link_libraries( main Qt5::Widgets Qt5::Core )
