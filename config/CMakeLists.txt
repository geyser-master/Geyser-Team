cmake_minimum_required(VERSION 4.3)

project(Geyser CXX)

set(CMAKE_CXX_STANDARD 26)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
if(WIN32)
    set(GY_HOST_OS "windows")
elseif(LINUX)
    set(GY_HOST_OS "linux")
elseif(UNIX)
    set(GY_HOST_OS "unix")
endif()
if(CMAKE_SYSTEM_PROCESSOR MATCHES "^(x86_64|AMD64)$")
    set(GY_HOST_ARCH "x86_64")
elseif(CMAKE_SYSTEM_PROCESSOR MATCHES "^(aarch64|arm64|ARM64)$")
    set(GY_HOST_ARCH "aarch64")
endif()
set(LLVM_ROOT "${CMAKE_SOURCE_DIR}/targets/${GY_HOST_ARCH}-${GY_HOST_OS}")

add_executable(
        Geyser
        engine/src/gy.cpp
        engine/src/gpm.cpp
        engine/src/gybuild.cpp
        engine/src/gydb.cpp
        engine/frontend/lexer.cpp
        engine/frontend/parser.cpp
)

target_include_directories(Geyser PRIVATE engine/include)
target_include_directories(Geyser PRIVATE ${LLVM_ROOT}/include)
target_link_directories(Geyser PRIVATE ${LLVM_ROOT}/lib)
target_link_libraries(Geyser PRIVATE LLVM-22)

add_custom_command(TARGET Geyser POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
        "${LLVM_ROOT}/bin/libLLVM-22.dll"
        "$<TARGET_FILE_DIR:Geyser>"
)