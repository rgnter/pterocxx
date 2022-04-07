cmake_minimum_required(VERSION 3.16)
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


conan_cmake_configure(REQUIRES fmt/8.1.1 nlohmann_json/3.9.1 openssl/3.0.2
        GENERATORS cmake_find_package)

conan_cmake_autodetect(settings)
conan_cmake_install(PATH_OR_REFERENCE .
        BUILD missing
        REMOTE conancenter
        SETTINGS ${settings})

set(PTEROCXX_HEADERS include/pterocxx.hpp include/rest.hpp)
set(PTEROCXX_SOURCES src/application.cpp src/rest.cpp include/types.hpp src/types.cpp)
set(PTEROCXX_TEST_SOURCES src/test/main.cpp)

if(MSVC)
    set(CXXFLAGS "${CXXFLAGS} /permissive-")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /permissive-")
    set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} /permissive-")
endif()

# Find libaries
find_package(nlohmann_json)
find_package(OpenSSL)
find_package(fmt)
find_package(Boost)

include_directories(${Boost_INCLUDE_DIRS})
include_directories(${OPENSSL_INCLUDE_DIR})
include_directories(include/)

link_libraries(${OPENSSL_LIBRARIES} nlohmann_json::nlohmann_json fmt::fmt)

add_library(${PROJECT_NAME} ${PTEROCXX_HEADERS} ${PTEROCXX_SOURCES})
add_executable(${PROJECT_NAME}_test ${PTEROCXX_TEST_SOURCES})
target_link_libraries(${PROJECT_NAME}_test ${PROJECT_NAME})
