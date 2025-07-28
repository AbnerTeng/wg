cmake_minimum_required(VERSION 3.12)
project(wg LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if(UNIX AND NOT APPLE)
  set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -static-libstdc++ -static-libgcc")
endif()

include(FetchContent)

FetchContent_Declare(
  yaml-cpp
  GIT_REPOSITORY https://github.com/jbeder/yaml-cpp.git
  GIT_TAG yaml-cpp-0.7.0
)
FetchContent_MakeAvailable(yaml-cpp)

set(
  SOURCES
  src/wg.cpp
  src/cli_handler.cpp
  src/gpu_utils.cpp
)

add_executable(wg ${SOURCES})
target_include_directories(wg PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src)
target_link_libraries(wg PUBLIC yaml-cpp::yaml-cpp)
