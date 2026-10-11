cmake_minimum_required(VERSION 4.3)

project(Geyser LANGUAGES CXX C)

set(CMAKE_CXX_STANDARD 26)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_C_STANDARD 23)
set(CMAKE_C_STANDARD_REQUIRED ON)

# Detect operating system
if(WIN32)
    set(GY_HOST_OS "windows")
elseif(CMAKE_SYSTEM_NAME STREQUAL "Linux")
    set(GY_HOST_OS "linux")
elseif(APPLE)
    set(GY_HOST_OS "macos")
elseif(UNIX)
    set(GY_HOST_OS "unix")
endif()

# Detect architecture
if(CMAKE_SYSTEM_PROCESSOR MATCHES "^(x86_64|AMD64|amd64)$")
    set(GY_HOST_ARCH "x86_64")
elseif(CMAKE_SYSTEM_PROCESSOR MATCHES "^(aarch64|arm64|ARM64)$")
    set(GY_HOST_ARCH "aarch64")
else()
    message(FATAL_ERROR
            "Unsupported architecture: ${CMAKE_SYSTEM_PROCESSOR}")
endif()

# LLVM installation directory
set(LLVM_ROOT
        "${CMAKE_SOURCE_DIR}/../platforms/${GY_HOST_ARCH}-${GY_HOST_OS}")

# Shared frontend
add_library(geyser_frontend STATIC
        ../engine/frontend/lexer.cpp
        ../engine/frontend/parser.cpp
)

target_include_directories(geyser_frontend PRIVATE
        ../engine/include
        "${LLVM_ROOT}/include"
)

# Four independent executables
add_executable(gy
        ../engine/src/gy.cpp
)

add_executable(gpm
        ../engine/src/gpm.cpp
)

add_executable(gybuild
        ../engine/src/gybuild.cpp
)

add_executable(gydb
        ../engine/src/gydb.cpp
)

# gybuild requires gy.exe to compile Geyser programs.
add_dependencies(gybuild gy)

# Shared include paths
foreach(target gy gpm gybuild gydb)
    target_include_directories(${target} PRIVATE
            ../engine/include
    )
endforeach()

# Link shared frontend to tools that use it.
target_link_libraries(gy PRIVATE geyser_frontend)

# LLVM configuration for gy
target_include_directories(gy PRIVATE
        "${LLVM_ROOT}/include"
)

target_link_directories(gy PRIVATE
        "${LLVM_ROOT}/lib"
)

target_link_libraries(gy PRIVATE LLVM-22)

# Place executables together.
set(GY_BIN_DIR "${CMAKE_BINARY_DIR}/../engine/bin")

foreach(target gy gpm gybuild gydb)
    set_target_properties(${target} PROPERTIES
            RUNTIME_OUTPUT_DIRECTORY "${GY_BIN_DIR}"
    )
endforeach()