cmake_minimum_required(VERSION 2.8)

project(picarus_takeout)
set( CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_LIST_DIR}/bin)

set( CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_CURRENT_LIST_DIR}/cmake )

include( UseCython FindNumPy)

find_package(NumPy REQUIRED)

include_directories(
  ${PYTHON_INCLUDE_DIRS}
  ${NUMPY_INCLUDE_DIRS}
  )

add_subdirectory(picarus_takeout)
