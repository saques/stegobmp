cmake_minimum_required(VERSION 3.10)
project(stegobmp)

set(CMAKE_CXX_STANDARD 14)
add_subdirectory(cxxopts)
add_subdirectory(easyloggingpp)
set(SOURCE_FILES main.cpp easyloggingpp/src/easylogging++.cc BMP.h)
add_executable(stegobmp ${SOURCE_FILES})

find_package(OpenSSL REQUIRED)
include_directories(${OPENSSL_INCLUDE_DIR} ${CXXOPTS_LIBRARIES} easyloggingpp/src)
target_link_libraries(${PROJECT_NAME} ${OPENSSL_LIBRARIES} cxxopts)

enable_testing()
add_subdirectory(googletest)
include_directories(${gtest_SOURCE_DIR}/include ${gtest_SOURCE_DIR})
set(TEST_SOURCE_FILES
	test.cpp)

add_executable(tests_run ${TEST_SOURCE_FILES})
target_include_directories(tests_run PUBLIC . ${OPENSSL_INCLUDE_DIR} ${CXXOPTS_LIBRARIES})
target_link_libraries(tests_run gtest gtest_main ${OPENSSL_LIBRARIES} cxxopts)
add_test(eq tests_run)