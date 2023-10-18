cmake_minimum_required(VERSION 3.10)
set(PROJECT_NAME learning_boids)
project(${PROJECT_NAME} VERSION 1.0.0 LANGUAGES CXX)

file(GLOB_RECURSE source_files
	"src/*.cpp"
	"src/*.hpp"
	"img/*.png"
)

set(SOURCES ${source_files})

# Set build type
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)
endif(NOT CMAKE_BUILD_TYPE)

set(CMAKE_CXX_FLAGS "-Wall")
set(CMAKE_CXX_FLAGS_DEBUG "-g")
set(CMAKE_CXX_FLAGS_RELEASE "-O3")

add_executable(${PROJECT_NAME} ${SOURCES})
#include images
file(COPY ${CMAKE_SOURCE_DIR}/img DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

# Include SFML headers
target_include_directories(${PROJECT_NAME} PRIVATE "include/SFML-2.6.0/include")

# Link SFML libraries
target_link_directories(${PROJECT_NAME} PRIVATE "include/SFML-2.6.0/lib")

# Add SFML libraries
set(SFML_LIBS sfml-system sfml-window sfml-graphics)
target_link_libraries(${PROJECT_NAME} ${SFML_LIBS})

set_property(TARGET ${PROJECT_NAME} PROPERTY CXX_STANDARD 20)