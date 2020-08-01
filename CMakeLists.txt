#
#  Copyright (c) 2020 Siddharth Chandrasekaran <siddharth@embedjournal.com>
#
#  SPDX-License-Identifier: Apache-2.0
#

cmake_minimum_required(VERSION 3.0 FATAL_ERROR)
project(utils VERSION 0.1.0 LANGUAGES C)

# libutils.a
set(LIB_UTILS utils)
file(GLOB LIB_UTILS_SRC ${CMAKE_CURRENT_SOURCE_DIR}/src/*.c)
file(GLOB LIB_UTILS_INC ${CMAKE_CURRENT_SOURCE_DIR}/include/utils/*.h)
add_library(${LIB_UTILS} STATIC ${LIB_UTILS_SRC})
target_include_directories(${LIB_UTILS} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)
set_target_properties(${LIB_UTILS} PROPERTIES PUBLIC_HEADER "${LIB_UTILS_INC}")

# check target
set(TEST_BIN test-utils)
file(GLOB TEST_SRC ${CMAKE_CURRENT_SOURCE_DIR}/tests/*.c)
add_executable(${TEST_BIN} ${TEST_SRC})
target_link_libraries(${TEST_BIN} ${LIB_UTILS})
target_include_directories(${TEST_BIN} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)
set_target_properties(${TEST_BIN} PROPERTIES EXCLUDE_FROM_ALL TRUE)
add_custom_target(check_utils
	COMMAND ${CMAKE_BINARY_DIR}/${TEST_BIN}
	COMMAND rm ${CMAKE_BINARY_DIR}/${TEST_BIN}
	DEPENDS ${TEST_BIN}
)
