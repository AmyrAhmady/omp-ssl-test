cmake_minimum_required(VERSION 3.19)
project(SSLTest LANGUAGES C CXX VERSION 0.0.1)

# Conan third-party libraries
include(${CMAKE_SOURCE_DIR}/cmake-conan/conan.cmake)
include(${CMAKE_SOURCE_DIR}/cmake-conan/conan-omp.cmake)

set(SHARED_OPENSSL FALSE CACHE BOOL "Whether to dynamically link the OpenSSL library")

# OpenSSL
# -------
if(SHARED_OPENSSL)
    message("Using shared OpenSSL")
    conan_omp_add_lib_opt(openssl 3.0.13 "openssl:shared=True")
else()
    message("Using static OpenSSL")
    conan_omp_add_lib(openssl 3.0.13)
endif()

add_subdirectory(sdk)

add_library(${PROJECT_NAME} SHARED main.cpp)

set_target_properties(${PROJECT_NAME} PROPERTIES PREFIX "")

target_link_libraries(${PROJECT_NAME} PRIVATE
    OMP-SDK
	CONAN_PKG::openssl
)

