# specify minimum cmake version
cmake_minimum_required(VERSION 3.10)

# specify the C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# set the project name
project(POMMEL VERSION 1.0)

# add the include directory
include_directories(include)

# add more sources
file(GLOB POMMEL_SOURCES "src/*.cpp")

# create the executable
add_executable(POMMEL ${POMMEL_SOURCES})

# add the include directory
target_include_directories(POMMEL PUBLIC
    "${PROJECT_SOURCE_DIR}"
    "${PROJECT_SOURCE_DIR}/include"
)

# add libraries

## adding boost libraries
find_package(Boost COMPONENTS system filesystem program_options REQUIRED)
target_link_libraries(POMMEL
    ${Boost_FILESYSTEM_LIBRARY}
    ${Boost_SYSTEM_LIBRARY}
    ${Boost_PROGRAM_OPTIONS_LIBRARY}
)

## adding HighFive
add_subdirectory(HighFive)
target_link_libraries(POMMEL HighFive)

## adding ramulator
add_subdirectory(ramulator)
target_link_libraries(POMMEL ramulator)
target_include_directories(POMMEL PUBLIC
    ${PROJECT_BINARY_DIR}
    ${PROJECT_SOURCE_DIR}/ramulator
    ${PROJECT_SOURCE_DIR}/ramulator/src
)

## adding ctemplate
target_link_libraries(POMMEL ctemplate)

# setting CFlags
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3 -DRAMULATOR -DHIGHFIVE_EXAMPLES=OFF")


