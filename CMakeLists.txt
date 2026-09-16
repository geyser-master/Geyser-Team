cmake_minimum_required(VERSION 4.3)

project(Geyser C)

set(CMAKE_C_STANDARD 23)
set(CMAKE_C_STANDARD_REQUIRED ON)
if(WIN32)
    set(GY_HOST_OS "windows")
elseif(APPLE)
    set(GY_HOST_OS "macos")
elseif(UNIX)
    set(GY_HOST_OS "linux")
endif()
if(CMAKE_SYSTEM_PROCESSOR MATCHES "^(x86_64|AMD64)$")
    set(GY_HOST_ARCH "x86_64")
elseif(CMAKE_SYSTEM_PROCESSOR MATCHES "^(aarch64|arm64|ARM64)$")
    set(GY_HOST_ARCH "aarch64")
endif()
set(LLVM_ROOT "${CMAKE_SOURCE_DIR}/targets/${GY_HOST_ARCH}-${GY_HOST_OS}")

add_executable(
        Geyser
        engine/gy.c
        engine/repl.c
)

target_include_directories(Geyser PRIVATE /)
target_include_directories(Geyser PRIVATE ${LLVM_ROOT}/include)
target_link_directories(Geyser PRIVATE ${LLVM_ROOT}/lib)
target_link_libraries(Geyser PRIVATE LLVM-22)

add_custom_command(TARGET Geyser POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
        "${LLVM_ROOT}/bin/libLLVM-22.dll"
        "$<TARGET_FILE_DIR:Geyser>"
)