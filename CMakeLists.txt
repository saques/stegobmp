cmake_minimum_required(VERSION 3.8)
project(stegobmp)

set(CMAKE_CXX_STANDARD 14)

add_subdirectory(cxxopts)
add_subdirectory(easyloggingpp)
set(SOURCE_FILES main.cpp easyloggingpp/src/easylogging++.cc BMP.h)
add_executable(stegobmp ${SOURCE_FILES})

find_package(OpenSSL REQUIRED)
include_directories(${OPENSSL_INCLUDE_DIR} ${CXXOPTS_LIBRARIES} easyloggingpp/src)
target_link_libraries(${PROJECT_NAME} ${OPENSSL_LIBRARIES} cxxopts)