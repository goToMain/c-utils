cmake_minimum_required(VERSION 2.6)
project(CUtils)

set(CUTILS_VERSION_MAJOR 1)
set(CUTILS_VERSION_MINOR 0)

include_directories("${PROJECT_SOURCE_DIR}")
include_directories("${PROJECT_SOURCE_DIR}/test")

set(TEST_DIR "${PROJECT_SOURCE_DIR}/test")
set(SRC_DIR "${PROJECT_SOURCE_DIR}")

set(TEST_SRC "${TEST_DIR}/test.c"
             "${SRC_DIR}/circular-generic-buffer.c"
             "${TEST_DIR}/test-circular-buffer.c")

add_executable(test_cutils ${TEST_SRC})

