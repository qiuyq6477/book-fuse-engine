cmake_minimum_required(VERSION 3.16)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS ON)
set(CMAKE_CXX_STANDARD 17)

project(fuse2d)

# output directories
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

# add engine and game
add_subdirectory(engine)
add_subdirectory(game)

add_custom_target(copy_assets ALL
    COMMAND ${CMAKE_COMMAND} -E copy_directory
            "${CMAKE_SOURCE_DIR}/resource"
            "${CMAKE_BINARY_DIR}/bin/assets"
    COMMENT "Copying assets to output directory"
)