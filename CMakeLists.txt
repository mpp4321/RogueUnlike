cmake_minimum_required(VERSION 3.0.0)
project(GoreLike VERSION 0.1.0)

include(CTest)
enable_testing()

file(GLOB SOURCES "GoreLike/*.cpp")
add_executable(GoreLike ${SOURCES})

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/")

set_property(TARGET GoreLike PROPERTY CXX_STANDARD 17)

include(CPack)

find_package(SDL2 REQUIRED)
find_package(SDL2_image REQUIRED)
find_package(SDL2_ttf REQUIRED)
find_package(fmt)
include_directories(${SDL2_INCLUDE_DIR} ${SDL2_IMAGE_INCLUDE_DIR} ${SDL2TTF_INCLUDE_DIR})

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/GoreLike)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/GoreLike/includes)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/tmxlite/tmxlite/includes)

# Link

link_directories("GoreLike/libs")

target_link_libraries(GoreLike ${SDL2_LIBRARIES} ${SDL2_IMAGE_LIBRARIES} ${SDL2TTF_LIBRARY})
target_link_libraries(GoreLike
    pthread
    tmxlite
    fmt::fmt
)