cmake_minimum_required(VERSION 4.3)

project(Geyser LANGUAGES CXX C)

set(CMAKE_CXX_STANDARD 26)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_C_STANDARD 23)
set(CMAKE_C_STANDARD_REQUIRED ON)

# Detect operating system
if (WIN32)
    set(GY_HOST_OS "windows")
elseif (CMAKE_SYSTEM_NAME STREQUAL "Linux")
    set(GY_HOST_OS "linux")
elseif (APPLE)
    set(GY_HOST_OS "macos")
elseif (UNIX)
    set(GY_HOST_OS "unix")
endif ()

# Detect architecture
if (CMAKE_SYSTEM_PROCESSOR MATCHES "^(x86_64|AMD64|amd64)$")
    set(GY_HOST_ARCH "x86_64")
elseif (CMAKE_SYSTEM_PROCESSOR MATCHES "^(aarch64|arm64|ARM64)$")
    set(GY_HOST_ARCH "aarch64")
else ()
    message(FATAL_ERROR
            "Unsupported architecture: ${CMAKE_SYSTEM_PROCESSOR}")
endif ()

# Locate a shared platform directory using architecture/OS groups.
set(GY_PLATFORMS_ROOT
        "${CMAKE_SOURCE_DIR}/../platforms"
)

file(GLOB GY_PLATFORM_CANDIDATES
        LIST_DIRECTORIES TRUE
        "${GY_PLATFORMS_ROOT}/*"
)

set(GY_PLATFORM_DIR "")

foreach (candidate IN LISTS GY_PLATFORM_CANDIDATES)
    if (NOT IS_DIRECTORY "${candidate}")
        continue()
    endif ()

    get_filename_component(candidate_name "${candidate}" NAME)

    # Find the group belonging to the detected architecture.
    # Example:
    # x86_64-windows+linux+freebsd+macos_arm64-linux+macos+windows
    #
    # The underscore inside x86_64 is part of the architecture name.
    # An underscore immediately before another architecture starts
    # the next architecture group.

    string(REGEX MATCH
            "(^|_)${GY_HOST_ARCH}-([^_]+)"
            matched_group
            "${candidate_name}"
    )

    if (matched_group STREQUAL "")
        continue()
    endif ()

    set(GY_SUPPORTED_OS_GROUP "${CMAKE_MATCH_2}")

    # Convert the plus-separated OS names into a CMake list.
    string(REPLACE "+"
            ";"
            GY_SUPPORTED_OS_LIST
            "${GY_SUPPORTED_OS_GROUP}"
    )

    # Does this architecture group include the detected OS?
    list(FIND GY_SUPPORTED_OS_LIST
            "${GY_HOST_OS}"
            GY_OS_INDEX
    )

    if (NOT GY_OS_INDEX EQUAL -1)
        if (NOT GY_PLATFORM_DIR STREQUAL "")
            message(FATAL_ERROR
                    "Multiple platform directories match "
                    "${GY_HOST_ARCH}-${GY_HOST_OS}. "
                    "Please remove the ambiguity."
            )
        endif ()

        set(GY_PLATFORM_DIR "${candidate}")
    endif ()
endforeach ()

if (GY_PLATFORM_DIR STREQUAL "")
    message(FATAL_ERROR
            "No shared platform directory supports "
            "${GY_HOST_ARCH}-${GY_HOST_OS}."
    )
endif ()

message(STATUS "Geyser platform directory: ${GY_PLATFORM_DIR}")

# Load Geyser platform manifest
set(GY_MANIFEST "${GY_PLATFORM_DIR}/manifest.json")

if (NOT EXISTS "${GY_MANIFEST}")
    message(FATAL_ERROR
            "Geyser manifest not found: ${GY_MANIFEST}"
    )
endif ()

file(READ "${GY_MANIFEST}" GY_MANIFEST_CONTENT)

string(JSON GY_MANIFEST_PLATFORM
        ERROR_VARIABLE GY_MANIFEST_ERROR
        GET "${GY_MANIFEST_CONTENT}" platform
)

if (GY_MANIFEST_ERROR)
    message(FATAL_ERROR
            "Unable to read platform from manifest: ${GY_MANIFEST_ERROR}"
    )
endif ()

string(JSON GY_MANIFEST_SCHEMA_VERSION
        ERROR_VARIABLE GY_SCHEMA_ERROR
        GET "${GY_MANIFEST_CONTENT}" schema_version
)

if (GY_SCHEMA_ERROR)
    message(FATAL_ERROR
            "Unable to read manifest schema version: ${GY_SCHEMA_ERROR}"
    )
endif ()

if (NOT GY_MANIFEST_PLATFORM STREQUAL "${GY_HOST_ARCH}-${GY_HOST_OS}")
    message(FATAL_ERROR
            "Manifest platform mismatch! "
            "Expected '${GY_HOST_ARCH}-${GY_HOST_OS}', "
            "but manifest declares '${GY_MANIFEST_PLATFORM}'."
    )
endif ()

if (NOT GY_MANIFEST_SCHEMA_VERSION EQUAL 1)
    message(FATAL_ERROR
            "Unsupported Geyser manifest schema version: "
            "${GY_MANIFEST_SCHEMA_VERSION}"
    )
endif ()

message(STATUS "Geyser platform: ${GY_MANIFEST_PLATFORM}")
message(STATUS "Geyser manifest schema: ${GY_MANIFEST_SCHEMA_VERSION}")

# Read LLVM version from manifest
string(JSON GY_LLVM_VERSION
        ERROR_VARIABLE GY_LLVM_VERSION_ERROR
        GET "${GY_MANIFEST_CONTENT}" llvm version
)

if (GY_LLVM_VERSION_ERROR)
    message(FATAL_ERROR
            "Unable to read LLVM version from manifest: "
            "${GY_LLVM_VERSION_ERROR}"
    )
endif ()

message(STATUS "Geyser LLVM version: ${GY_LLVM_VERSION}")

# LLVM installation directory
set(LLVM_ROOT
        "${CMAKE_SOURCE_DIR}/../platforms/${GY_HOST_ARCH}-${GY_HOST_OS}")

# Validate LLVM runtime and import library
string(JSON GY_LLVM_RUNTIME
        GET "${GY_MANIFEST_CONTENT}" llvm runtime
)

string(JSON GY_LLVM_IMPORT_LIBRARY
        GET "${GY_MANIFEST_CONTENT}" llvm import_library
)

set(GY_LLVM_RUNTIME_PATH
        "${GY_PLATFORM_DIR}/${GY_LLVM_RUNTIME}"
)

set(GY_LLVM_IMPORT_LIBRARY_PATH
        "${GY_PLATFORM_DIR}/${GY_LLVM_IMPORT_LIBRARY}"
)

if (NOT EXISTS "${GY_LLVM_RUNTIME_PATH}")
    message(FATAL_ERROR
            "LLVM runtime DLL not found: ${GY_LLVM_RUNTIME_PATH}"
    )
endif ()

if (NOT EXISTS "${GY_LLVM_IMPORT_LIBRARY_PATH}")
    message(FATAL_ERROR
            "LLVM import library not found: ${GY_LLVM_IMPORT_LIBRARY_PATH}"
    )
endif ()

message(STATUS "LLVM runtime found: ${GY_LLVM_RUNTIME}")
message(STATUS "LLVM import library found: ${GY_LLVM_IMPORT_LIBRARY}")

# Shared frontend
add_library(geyser_frontend STATIC
        ../engine/frontend/lexer.cpp
        ../engine/frontend/parser.cpp
        ../engine/include/tokens.hpp
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
foreach (target gy gpm gybuild gydb)
    target_include_directories(${target} PRIVATE
            ../engine/include
    )
endforeach ()

# Link shared frontend to tools that use it.
target_link_libraries(gy PRIVATE geyser_frontend)

# LLVM configuration for gy
target_include_directories(gy PRIVATE
        "${LLVM_ROOT}/include"
)

target_link_libraries(gy PRIVATE
        "${GY_LLVM_IMPORT_LIBRARY_PATH}"
)

# Place executables together.
set(GY_BIN_DIR "${CMAKE_SOURCE_DIR}/../engine/bin")

foreach (target gy gpm gybuild gydb)
    set_target_properties(${target} PROPERTIES
            RUNTIME_OUTPUT_DIRECTORY "${GY_BIN_DIR}"
    )

    foreach (dll
            libgcc_s_seh-1.dll
            libstdc++-6.dll
            libwinpthread-1.dll
    )
        add_custom_command(TARGET ${target} POST_BUILD
                COMMAND "${CMAKE_COMMAND}" -E copy_if_different
                "${GY_PLATFORM_DIR}/bin/${dll}"
                "$<TARGET_FILE_DIR:${target}>"
                COMMENT "Packaging ${dll} for ${target}"
                VERBATIM
        )
    endforeach ()
endforeach ()