cmake_minimum_required(VERSION 3.1)
set(ROOT "${CMAKE_CURRENT_LIST_DIR}")
set(DEPS "${ROOT}/deps")
project(country-registration)
option(USE_SOLUTION "Use solution" OFF)
include_directories("include/")

# c++ version
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

find_package(OpenGL REQUIRED)

if(USE_SOLUTION)
  add_definitions("-DUSE_SOLUTION")
endif()

if(NOT TARGET glfw)
  set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL " " FORCE)
  set(GLFW_BUILD_TESTS OFF CACHE BOOL " " FORCE)
  set(GLFW_BUILD_DOCS OFF CACHE BOOL " " FORCE)
  set(GLFW_INSTALL OFF CACHE BOOL " " FORCE)
  add_subdirectory(${DEPS}/glfw glfw)
endif()

find_package(GDAL REQUIRED)
if(GDAL_FOUND)
  include_directories(${GDAL_INCLUDE_DIRS})
else()
  message(STATUS "GDAL NOT found")
endif()

add_subdirectory(${DEPS}/glad glad)

FILE(GLOB SRC_FILES src/*.cpp)
FILE(GLOB SHADER_FILES shaders/*)
add_executable(${PROJECT_NAME} ${SRC_FILES} ${SHADER_FILES})

target_include_directories(${PROJECT_NAME} SYSTEM PUBLIC ${OPENGL_INCLUDE_DIR})
target_include_directories(${PROJECT_NAME} SYSTEM PUBLIC ${DEPS}/eigen/)
target_include_directories(${PROJECT_NAME} SYSTEM PUBLIC ${DEPS}/glfw/include)
target_include_directories(${PROJECT_NAME} SYSTEM PUBLIC ${DEPS}/glad/include)
target_link_libraries(${PROJECT_NAME} glfw ${OPENGL_gl_LIBRARY} glad ${GDAL_LIBRARIES})