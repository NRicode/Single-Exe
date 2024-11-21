cmake_minimum_required(VERSION 3.10)
project(PackerProject)

set(CMAKE_CXX_STANDARD 17)

include_directories(${CMAKE_SOURCE_DIR}/include)
link_directories(${CMAKE_SOURCE_DIR}/lib)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -s -Os")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -s -Os")

file(GLOB CPP_FILES "*.cpp")
file(GLOB C_FILES "*.c")
add_executable(packer WIN32 ${CPP_FILES} ${C_FILES})

option(BUILD_EXAMPLES FALSE)
option(BUILD_SHARED_LIBS FALSE)
add_subdirectory(raylib-5.5)

target_link_libraries(packer raylib)
target_link_libraries(packer ole32 gdi32 winmm -static)