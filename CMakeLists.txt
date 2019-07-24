cmake_minimum_required(VERSION 2.6)
project(CUtils)

set(CUTILS_VERSION_MAJOR 1)
set(CUTILS_VERSION_MINOR 0)

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

include_directories(${PROJECT_SOURCE_DIR}/src)
include_directories(${PROJECT_SOURCE_DIR}/test)

set(TEST_DIR ${PROJECT_SOURCE_DIR}/test)
set(SRC_DIR  ${PROJECT_SOURCE_DIR}/src)
set(SAMPLE_DIR ${PROJECT_SOURCE_DIR}/sample)

set(TEST_SRC ${TEST_DIR}/test.c)

# circular Buffers
set(MOD_SRC ${SRC_DIR}/circbuf.c
            ${SRC_DIR}/cut_string.c)

set(SAMPLE_CB_SRC ${MOD_SRC} ${SAMPLE_DIR}/sample-circbuf.c)
add_executable(sample_circbuf ${SAMPLE_CB_SRC})

set(TEST_SRC ${TEST_SRC} ${MOD_SRC} ${TEST_DIR}/test-circbuf.c
                                    ${TEST_DIR}/test_cut_string.c)

add_executable(test_cutils ${TEST_SRC})
