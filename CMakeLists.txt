cmake_minimum_required(VERSION 3.23)
project(Captain-Claw)
set(CMAKE_CXX_STANDARD 17)

# Setup build directory
math(EXPR platform_bits "${CMAKE_SIZEOF_VOID_P} * 8")
set(project_platform_binary_dir ${CMAKE_SOURCE_DIR}/bin/${PROJECT_NAME}/$<CONFIG>-x${platform_bits})
# Set .dll/.exe files directory
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${project_platform_binary_dir})

# Setup cache directory
set(CMAKE_BUILD_FILES_DIRECTORY ${CMAKE_SOURCE_DIR}/intermediate/${PROJECT_NAME}/$<CONFIG>-x${platform_bits})
set(CMAKE_BUILD_DIRECTORY ${CMAKE_SOURCE_DIR}/intermediate/${PROJECT_NAME}/$<CONFIG>-x${platform_bits})
set(CMAKE_CACHEFILE_DIR ${CMAKE_SOURCE_DIR}/intermediate/${PROJECT_NAME}/$<CONFIG>-x${platform_bits})

# Search for .cpp files
file(MAKE_DIRECTORY ${CMAKE_SOURCE_DIR}/${PROJECT_NAME}/src)
file(GLOB_RECURSE SRC_FILES ${CMAKE_SOURCE_DIR}/${PROJECT_NAME}/src/*.cpp)

add_executable(${PROJECT_NAME} ${SRC_FILES} Captain-Claw/src/pickup_system.cpp Captain-Claw/src/pickup_system.h Captain-Claw/src/keymap_menu.cpp Captain-Claw/src/keymap_menu.h)

set(BUILD_SHARED_LIBS ON)

target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_SOURCE_DIR}/${PROJECT_NAME}/src)
target_link_libraries(${PROJECT_NAME} PRIVATE sfml-system sfml-window sfml-graphics sfml-audio sfml-network)

add_subdirectory(vendor)