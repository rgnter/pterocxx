cmake_minimum_required(VERSION 3.21)
project(pterocxx)

set(CMAKE_CXX_STANDARD 20)
list(APPEND CMAKE_MODULE_PATH ${CMAKE_BINARY_DIR})
list(APPEND CMAKE_PREFIX_PATH ${CMAKE_BINARY_DIR})

if(NOT EXISTS "${CMAKE_BINARY_DIR}/cmake/conan.cmake")
    message(STATUS "Downloading conan.cmake from https://github.com/conan-io/cmake-conan")
    file(DOWNLOAD "https://raw.githubusercontent.com/conan-io/cmake-conan/master/conan.cmake"
            "${CMAKE_BINARY_DIR}/cmake/conan.cmake")
endif()
include("${CMAKE_BINARY_DIR}/cmake/conan.cmake")


conan_cmake_configure(REQUIRES spdlog/1.9.2 nlohmann_json/3.9.1 openssl/3.0.2 ctre/3.6
        GENERATORS cmake_find_package)

conan_cmake_autodetect(settings)
conan_cmake_install(PATH_OR_REFERENCE .
        BUILD missing
        REMOTE conancenter
        SETTINGS ${settings})

set(PTEROCXX_HEADERS include/pterocxx.hpp include/rest.hpp)
set(PTEROCXX_SOURCES src/main.cpp src/application.cpp src/rest.cpp include/types.hpp src/types.cpp)


# Find libaries
find_package(spdlog)
find_package(nlohmann_json)
find_package(cxxopts)
find_package(OpenSSL)
find_package(ctre)
find_package(Boost)

include_directories(${Boost_INCLUDE_DIRS})
include_directories(${OPENSSL_INCLUDE_DIR})
include_directories(include/)

link_libraries(${OPENSSL_LIBRARIES} nlohmann_json::nlohmann_json ctre::ctre)

add_executable(pterocxx ${PTEROCXX_HEADERS} ${PTEROCXX_SOURCES})
