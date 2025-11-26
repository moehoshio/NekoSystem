cmake_minimum_required(VERSION 3.14)

# Set policy for MSVC runtime library before project()
if(POLICY CMP0091)
    cmake_policy(SET CMP0091 NEW)
endif()

# Important: vcpkg toolchain must be loaded via CMAKE_TOOLCHAIN_FILE
# The toolchain sets VCPKG_TARGET_TRIPLET and VCPKG_CRT_LINKAGE

# Set MSVC runtime library based on vcpkg triplet
# This must be set before project() command
if(MSVC OR MSVC_IDE OR CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
    # Check if we have vcpkg triplet information
    if(DEFINED VCPKG_TARGET_TRIPLET)
        message(STATUS "vcpkg test: Using triplet ${VCPKG_TARGET_TRIPLET}")
        
        # Determine CRT linkage from triplet name
        if(VCPKG_TARGET_TRIPLET MATCHES ".*-static$")
            message(STATUS "vcpkg test: Detected static triplet, using static runtime (MT)")
            set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")
        else()
            message(STATUS "vcpkg test: Detected dynamic triplet, using dynamic runtime (MD)")
            set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>DLL")
        endif()
    elseif(DEFINED VCPKG_CRT_LINKAGE)
        # Fallback: use VCPKG_CRT_LINKAGE if set
        if(VCPKG_CRT_LINKAGE STREQUAL "static")
            set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")
        else()
            set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>DLL")
        endif()
    else()
        # Default to dynamic runtime
        message(STATUS "vcpkg test: No triplet info, defaulting to dynamic runtime (MD)")
        set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>DLL")
    endif()
endif()

project(vcpkg_test LANGUAGES CXX)

# Find NekoSystem package installed via vcpkg
find_package(NekoSystem CONFIG REQUIRED)

# Create test executable
add_executable(vcpkg_test test.cpp)

# Link against NekoSystem
target_link_libraries(vcpkg_test PRIVATE Neko::System)

# Set C++20 standard
target_compile_features(vcpkg_test PRIVATE cxx_std_20)

# Explicitly set MSVC runtime library on the target to match NekoSystem
if(MSVC)
    if(DEFINED VCPKG_TARGET_TRIPLET AND VCPKG_TARGET_TRIPLET MATCHES ".*-static$")
        # Static triplet: use static runtime
        set_target_properties(vcpkg_test PROPERTIES
            MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>"
        )
        message(STATUS "vcpkg test: Set target to use static runtime (MT/MTd)")
    else()
        # Dynamic triplet or default: use dynamic runtime
        set_target_properties(vcpkg_test PROPERTIES
            MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>DLL"
        )
        message(STATUS "vcpkg test: Set target to use dynamic runtime (MD/MDd)")
    endif()
endif()
