#
#  Copyright (c) 2019 Siddharth Chandrasekaran <siddharth@embedjournal.com>
#
#  SPDX-License-Identifier: Apache-2.0
#

cmake_minimum_required(VERSION 3.0 FATAL_ERROR)
project(utils VERSION 0.1.0 LANGUAGES C)

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

# Public headers
include_directories(${CMAKE_SOURCE_DIR}/include)

# libutils.a
set(LIB_UTILS utils)
file(GLOB LIB_UTILS_SRC ${CMAKE_SOURCE_DIR}/src/*.c)
file(GLOB LIB_UTILS_INC ${CMAKE_SOURCE_DIR}/include/utils/*.h)
add_library(${LIB_UTILS} STATIC ${LIB_UTILS_SRC})
set_target_properties(${LIB_UTILS} PROPERTIES
	PUBLIC_HEADER "${LIB_UTILS_INC}"
)
install(
	TARGETS ${LIB_UTILS}
	DESTINATION ${CMAKE_INSTALL_DIR}/lib
	PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_DIR}/include/utils/
)

# check target
set(TEST_BIN test-utils)
file(GLOB TEST_SRC ${CMAKE_SOURCE_DIR}/tests/*.c)
add_executable(${TEST_BIN} ${TEST_SRC})
target_link_libraries(${TEST_BIN} ${LIB_UTILS})
set_target_properties(${TEST_BIN} PROPERTIES EXCLUDE_FROM_ALL TRUE)
add_custom_target(check_utils
	COMMAND ${CMAKE_BINARY_DIR}/bin/${TEST_BIN}
	COMMAND rm ${CMAKE_BINARY_DIR}/bin/${TEST_BIN}
	DEPENDS ${TEST_BIN}
)
