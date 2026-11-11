cmake_minimum_required(VERSION 3.20)
project(Sekhmet VERSION 0.1.0 LANGUAGES CXX)

if(NOT EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/portablekit/CMakeLists.txt")
    message(FATAL_ERROR "PortableKit submodule is missing. Run: git submodule update --init --recursive")
endif()

add_subdirectory(portablekit)
add_subdirectory(hle/geburst_crypto)

portablekit_add_game(SekhmetNative
    PROFILE_DIR "${CMAKE_CURRENT_SOURCE_DIR}"
    SOURCES host/sekhmet_profile.cpp)
