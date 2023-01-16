cmake_minimum_required(VERSION 3.24)
project(Final_Project)

set(CMAKE_CXX_STANDARD 17)

# My Stuff
file(GLOB_RECURSE CPP_FILES "src/*.cpp")

include_directories(include/)

add_subdirectory(libraries/BLT)
# External Libraries
find_package(GLUT)

if(NOT ${GLUT_FOUND})
    message("WARNING -- If you are on linux please install the FreeGLUT lib")
    set(GLUT_INCLUDE_DIRS "${CMAKE_CURRENT_SOURCE_DIR}/include/glut/include")
    set(GLUT_LIBRARIES "${CMAKE_CURRENT_SOURCE_DIR}/include/glut/lib/freeglut.lib")
endif()
message("Using GLUT ${GLUT_INCLUDE_DIRS} and ${GLUT_LIBRARIES}")

find_library(FreeImage_LIBRARIES libfreeimage.so OR libfreeimage.a)
find_file(FreeImage_INCLUDE FreeImage.h)
message(${FreeImage_LIBRARIES})
if(NOT ${FreeImage_LIBRARIES} OR NOT ${FreeImage_INCLUDE})
    message("WARNING -- If you are on linux please install the FreeImage lib")
    #set(FreeImage_LIBRARIES "${CMAKE_CURRENT_SOURCE_DIR}/libraries/freeimage/lib/FreeImage.lib;${CMAKE_CURRENT_SOURCE_DIR}/libraries/freeimage/lib/FreeImaged.dll")
endif()
message("Using FreeImage ${FreeImage_LIBRARIES} and ${FreeImage_INCLUDE}")

include_directories(${GLUT_INCLUDE_DIRS})
include_directories(${FreeImage_INCLUDE})

add_executable(Final_Project ${CPP_FILES})
target_link_libraries(Final_Project PRIVATE BLT)
target_link_libraries(Final_Project PRIVATE ${GLUT_LIBRARIES})
target_link_libraries(Final_Project PRIVATE ${FreeImage_LIBRARIES})