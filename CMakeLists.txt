cmake_minimum_required(VERSION 3.0)
# emscriptem
cmake_policy(SET CMP0015 NEW)
project(FinalProject)

option(USE_EXTRAS "Use the extra stuff I've added to this project! (Basically emscriptem)" OFF)

set(CMAKE_CXX_STANDARD 17)

if (USE_EXTRAS)
    message("Using emscriptem linker and compile flags!")
    #add_definitions("-std=c++17")
    set(CMAKE_VERBOSE_MAKEFILE on)
else()
    # used to debug memory related issues
    if ((CMAKE_BUILD_TYPE MATCHES Debug))
        if (UNIX)
            set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fsanitize=address -g")
            add_compile_options(-fsanitize=address)
            add_link_options(-fsanitize=address)
        endif ()
        message("Building for Debug")
    else ()
        message("Building for Release")
    endif ()

    set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
    set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
    set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)

    add_subdirectory(libraries/glfw-3.3.8)

endif()



# My Stuff
file(GLOB_RECURSE CPP_FILES "src/*.cpp")

include_directories(include/)

add_subdirectory(libraries/BLT)
add_subdirectory(libraries/freetype-2.13.0)

add_executable(FinalProject ${CPP_FILES})
target_link_libraries(FinalProject PRIVATE BLT)
target_link_libraries(FinalProject PRIVATE freetype)

file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/assets DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/)

if (USE_EXTRAS)
    #set_target_properties(FinalProject PROPERTIES LINK_FLAGS "-s DEMANGLE_SUPPORT=1 --preload-file ${CMAKE_SOURCE_DIR}/assets --bind")
    #set_target_properties(FinalProject PROPERTIES LINK_FLAGS "-s DEMANGLE_SUPPORT=1 --preload-file 'assets' --bind")
    set_target_properties(FinalProject PROPERTIES LINK_FLAGS "-sMAX_WEBGL_VERSION=2 -s ASSERTIONS=1 -sUSE_GLFW=3 --preload-file 'assets'")
    # these flags will be set by cmake automatically based on build type
    #set_target_properties(FinalProject PROPERTIES COMPILE_FLAGS "-O3")
    #set_target_properties(FinalProject PROPERTIES COMPILE_FLAGS "-g")
else()
    target_link_libraries(FinalProject PRIVATE glfw)
endif()