cmake_minimum_required(VERSION 3.10)

project(GUI)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)

include_directories(
    ${PROJECT_SOURCE_DIR}               
    ${PROJECT_SOURCE_DIR}/DNA          
    /usr/local/include                 
)

link_directories(
    /usr/local/lib                     
)
# set(CMAKE_PREFIX_PATH "/usr/local")
find_package(yaml-cpp REQUIRED)
# find_package(yaml-cpp REQUIRED PATHS /usr/local/lib/cmake/yaml-cpp)



set(SOURCES
    pose_class.cpp
)

add_executable(GUI ${SOURCES} pose_class.hpp)

target_link_libraries(GUI PRIVATE yaml-cpp)
