cmake_minimum_required(VERSION 3.0.0)
project(gccx-test VERSION 0.2)

include(CTest)
enable_testing()

message(STATUS "${PROJECT_NAME} started building on ${CMAKE_SYSTEM_NAME}!")

set(include_dirs
            "${PROJECT_SOURCE_DIR}/chi-dom"
						)
message(STATUS "Include dirs: ${include_dirs}")
include_directories(${include_dirs})

# Src

file(GLOB_RECURSE source_files
		"${PROJECT_SOURCE_DIR}/chi-dom/*.cpp" "${PROJECT_SOURCE_DIR}/chi-dom/*.h"
		"${PROJECT_SOURCE_DIR}/chi-dom/**/*.cpp" "${PROJECT_SOURCE_DIR}/chi-dom/**/*.h")
message(STATUS "Source files: ${source_files}")

add_executable(gccx-test ${PROJECT_SOURCE_DIR}/test_dist.cpp ${source_files})

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
