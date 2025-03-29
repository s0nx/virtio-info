cmake_minimum_required(VERSION 3.20)

project(
    virtio-info
    LANGUAGES CXX
)

# fmtlib
find_package(fmt REQUIRED)

include(FetchContent)

# CLI11
FetchContent_Declare(
    cli11
    GIT_REPOSITORY https://github.com/CLIUtils/CLI11.git
    GIT_TAG main
)
FetchContent_MakeAvailable(cli11)

set(CMAKE_CXX_STANDARD 20)

add_executable(virtio-info)

# includes
target_include_directories(virtio-info PRIVATE src)
target_include_directories(virtio-info PRIVATE ${CMAKE_CURRENT_BINARY_DIR})

# src
target_sources(virtio-info PRIVATE
    src/config.cpp
    src/main.cpp
    src/virtio_bus.cpp
    #src/util.cpp
)

target_compile_features(virtio-info PUBLIC cxx_std_20)
target_compile_options(virtio-info PRIVATE -Wall -Wextra -pedantic -O3)

target_link_libraries(virtio-info PRIVATE fmt)
target_link_libraries(virtio-info PRIVATE CLI11::CLI11)

add_custom_command(
    OUTPUT git_verhdr_gen
    COMMAND ${CMAKE_COMMAND} -DSOURCE_DIR=${CMAKE_CURRENT_SOURCE_DIR}
                         -P ${CMAKE_CURRENT_SOURCE_DIR}/gitver.cmake
)

add_custom_target(
    gitverhdr ALL
    DEPENDS git_verhdr_gen
)

add_dependencies(virtio-info gitverhdr)
