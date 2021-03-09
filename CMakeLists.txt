cmake_minimum_required(VERSION 3.17)
project(pkgx)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_VERBOSE_MAKEFILE 1)

# Apparently CLion can't find cmake's modules on osx
if (${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
    set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "/usr/local/share/cmake/Modules/")
endif()

# Zstd
include(addons/FindZstd.cmake)
include_directories(${Zstd_INCLUDE_DIR})

# nholmann_json
find_package(nlohmann_json 3.2.0 REQUIRED)

# Sources
file(GLOB_RECURSE _src_headers RELATIVE ${CMAKE_SOURCE_DIR} "src/*.hxx")
file(GLOB_RECURSE _src_sources RELATIVE ${CMAKE_SOURCE_DIR} "src/*.cxx")

# Linkage
add_executable(pkgx ${_src_headers} ${_src_sources})
target_link_libraries(pkgx ${Zstd_LIBRARY} nlohmann_json::nlohmann_json)