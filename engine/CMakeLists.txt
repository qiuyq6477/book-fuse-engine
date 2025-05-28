# engine project (shared lib)
project(engine VERSION 0.0.1)

# get all cpp-files
file(GLOB_RECURSE SRC_FILES src/*.cpp CMAKE_CONFIGURE_DEPENDS)
add_library(${PROJECT_NAME} ${SRC_FILES})

# define symbols
target_compile_definitions(engine PUBLIC FUSE_EXPORT)
target_compile_definitions(engine PUBLIC _DEBUG)

# 设置SDL2等依赖的根路径
set(SDL2_ROOT         "${CMAKE_SOURCE_DIR}/dep/SDL2-2.32.6/cmake")
set(SDL2_IMAGE_ROOT   "${CMAKE_SOURCE_DIR}/dep/SDL2_image-2.8.8/cmake")
set(SDL2_MIXER_ROOT   "${CMAKE_SOURCE_DIR}/dep/SDL2_mixer-2.8.1/cmake")
set(SDL2_NET_ROOT     "${CMAKE_SOURCE_DIR}/dep/SDL2_net-2.2.0/cmake")
set(SDL2_TTF_ROOT     "${CMAKE_SOURCE_DIR}/dep/SDL2_ttf-2.24.0/cmake")

# 优先查找本地依赖
list(PREPEND CMAKE_PREFIX_PATH
    "${SDL2_ROOT}"
    "${SDL2_IMAGE_ROOT}"
    "${SDL2_MIXER_ROOT}"
    "${SDL2_NET_ROOT}"
    "${SDL2_TTF_ROOT}"
)
# Add SDL2 CMake modules
# list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake/sdl2)

# Find SDL2
find_package(SDL2 REQUIRED)
find_package(SDL2_image REQUIRED)
find_package(SDL2_net REQUIRED)
find_package(SDL2_mixer REQUIRED)
find_package(SDL2_ttf REQUIRED)

include(FetchContent)

set(YAML_CPP_BUILD_TESTS OFF CACHE INTERNAL "Disable yaml-cpp tests")
set(YAML_CPP_BUILD_TOOLS OFF CACHE INTERNAL "Disable yaml-cpp tools")
set(YAML_CPP_BUILD_CONTRIB OFF CACHE INTERNAL "Disable yaml-cpp contrib")

FetchContent_Declare(
  yaml-cpp
  GIT_REPOSITORY https://github.com/jbeder/yaml-cpp.git
  GIT_TAG 0.8.0
)
FetchContent_MakeAvailable(yaml-cpp)

target_include_directories(${PROJECT_NAME} PUBLIC ${SDL2_INCLUDE_DIR})
target_link_libraries(${PROJECT_NAME}
    SDL2::SDL2main 
    SDL2::SDL2
    SDL2_net::SDL2_net
    SDL2_image::SDL2_image
    SDL2_mixer::SDL2_mixer
    SDL2_ttf::SDL2_ttf
    yaml-cpp
)

# precompile headers
target_precompile_headers(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/src/pch.h)

# add include directory
target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/src)


if(WIN32)
    # 判断架构
    if(CMAKE_SIZEOF_VOID_P EQUAL 8)
        set(ARCH_DIR "x86_64-w64-mingw32")
    else()
        set(ARCH_DIR "i686-w64-mingw32")
    endif()

    # 查找所有依赖库的DLL
    file(GLOB SDL2_DLLS
        "${CMAKE_SOURCE_DIR}/dep/SDL2-2.32.6/${ARCH_DIR}/bin/*.dll"
        "${CMAKE_SOURCE_DIR}/dep/SDL2_image-2.8.8/${ARCH_DIR}/bin/*.dll"
        "${CMAKE_SOURCE_DIR}/dep/SDL2_mixer-2.8.1/${ARCH_DIR}/bin/*.dll"
        "${CMAKE_SOURCE_DIR}/dep/SDL2_net-2.2.0/${ARCH_DIR}/bin/*.dll"
        "${CMAKE_SOURCE_DIR}/dep/SDL2_ttf-2.24.0/${ARCH_DIR}/bin/*.dll"
    )

    # 复制到输出目录
    foreach(dll ${SDL2_DLLS})
        add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy_if_different
                "${dll}"
                "$<TARGET_FILE_DIR:${PROJECT_NAME}>"
        )
    endforeach()
endif()