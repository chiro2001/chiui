cmake_minimum_required(VERSION 3.0.0)
project(chiui VERSION 0.2)

include(CTest)
enable_testing()

message(STATUS "${PROJECT_NAME} started building on ${CMAKE_SYSTEM_NAME}!")

# Options
option(XML_PARSER_VERBOSE "Enable to be told everything the xml parser does" OFF)

if(XML_PARSER_VERBOSE)
	target_compile_definitions(chiui-test PRIVATE XML_PARSER_VERBOSE)
endif(XML_PARSER_VERBOSE)

set(include_dirs
            "${PROJECT_SOURCE_DIR}/src/xml"
						)
message(STATUS "Include dirs: ${include_dirs}")
include_directories(${include_dirs})

# Src

file(GLOB_RECURSE source_files
        "${PROJECT_SOURCE_DIR}/src/xml/*.h" "${PROJECT_SOURCE_DIR}/src/xml/*.c")
message(STATUS "Source files: ${source_files}")

add_executable(chiui-test ${PROJECT_SOURCE_DIR}/src/tests/chiui-test.c ${source_files})

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
