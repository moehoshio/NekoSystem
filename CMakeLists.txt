cmake_minimum_required(VERSION 3.14)
project(NekoSystem VERSION 1.0 LANGUAGES CXX)

option(NEKO_AUTO_FETCH_DEPS "Automatically fetch dependencies" ON)
option(NEKO_BUILD_TESTS "Build tests" ON)


if(NEKO_AUTO_FETCH_DEPS)
    include(FetchContent)
    FetchContent_Declare(
        NekoSchema
        GIT_REPOSITORY https://github.com/moehoshio/NekoSchema.git
        GIT_TAG        main
    )
    FetchContent_MakeAvailable(NekoSchema)

    FetchContent_Declare(
        NekoFunction
        GIT_REPOSITORY https://github.com/moehoshio/NekoFunction.git
        GIT_TAG        main
    )
    
    # Get NekoFunction source without building its tests to avoid C++20 compatibility issues
    FetchContent_GetProperties(NekoFunction)
    if(NOT nekofunction_POPULATED)
        FetchContent_Populate(NekoFunction)
        
        # Check if NekoFunction has tests that might cause issues
        set(BUILD_TESTS_BACKUP ${BUILD_TESTS})
        set(BUILD_TESTING_BACKUP ${BUILD_TESTING})
        set(NEKO_BUILD_TESTS_BACKUP ${NEKO_BUILD_TESTS})
        
        set(BUILD_TESTS OFF CACHE BOOL "Disable NekoFunction tests" FORCE)
        set(BUILD_TESTING OFF CACHE BOOL "Disable NekoFunction testing" FORCE)
        set(NEKO_BUILD_TESTS OFF CACHE BOOL "Disable NekoFunction tests" FORCE)
        
        add_subdirectory(${nekofunction_SOURCE_DIR} ${nekofunction_BINARY_DIR})
        
        # Restore settings
        set(BUILD_TESTS ${BUILD_TESTS_BACKUP} CACHE BOOL "" FORCE)
        set(BUILD_TESTING ${BUILD_TESTING_BACKUP} CACHE BOOL "" FORCE)
        set(NEKO_BUILD_TESTS ${NEKO_BUILD_TESTS_BACKUP} CACHE BOOL "" FORCE)
    endif()
endif()

set(SRCFILES)

if(WIN32)
    list(APPEND SRCFILES
        ${CMAKE_SOURCE_DIR}/src/neko/system/memoryinfo_windows.cpp
        ${CMAKE_SOURCE_DIR}/src/neko/system/platform_windows.cpp
    )
elseif(APPLE)
    list(APPEND SRCFILES 
        ${CMAKE_SOURCE_DIR}/src/neko/system/memoryinfo_macos.cpp
        ${CMAKE_SOURCE_DIR}/src/neko/system/platform_unix.cpp
    )
elseif(UNIX)
    list(APPEND SRCFILES 
        ${CMAKE_SOURCE_DIR}/src/neko/system/memoryinfo_linux.cpp
        ${CMAKE_SOURCE_DIR}/src/neko/system/platform_unix.cpp
    )
endif()


# Main library target
add_library(NekoSystem STATIC ${SRCFILES})
add_library(Neko::System ALIAS NekoSystem)

target_include_directories(NekoSystem PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
)
target_link_libraries(NekoSystem PUBLIC NekoSchema NekoFunction)
target_compile_features(NekoSystem PUBLIC cxx_std_20)

# Compiler-specific options to enable proper C++20 feature detection
if(MSVC)
    target_compile_options(NekoSystem PUBLIC /Zc:__cplusplus)
elseif(CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
    # GCC needs explicit standard and feature test macro updates
    target_compile_options(NekoSystem PUBLIC -std=c++20)
elseif(CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
    # Clang needs explicit standard for proper feature detection
    target_compile_options(NekoSystem PUBLIC -std=c++20)
endif()

# Testing
if(NEKO_BUILD_TESTS)
    enable_testing()
    
    # Fetch GoogleTest
    include(FetchContent)
    FetchContent_Declare(
        googletest
        GIT_REPOSITORY https://github.com/google/googletest.git
        GIT_TAG        release-1.12.1
    )
    # For Windows: Prevent overriding the parent project's compiler/linker settings
    set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
    FetchContent_MakeAvailable(googletest)
    
    # System Test executable
    add_executable(system_tests tests/system_test.cpp)
    target_link_libraries(system_tests 
        PRIVATE 
        NekoSystem
        gtest_main
        gtest
    )
    target_compile_features(system_tests PRIVATE cxx_std_20)
    
    # Compiler-specific options for tests to enable proper C++20 feature detection
    if(MSVC)
        target_compile_options(system_tests PRIVATE /Zc:__cplusplus)
    elseif(CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
        target_compile_options(system_tests PRIVATE -std=c++20)
    elseif(CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
        target_compile_options(system_tests PRIVATE -std=c++20)
    endif()
    
    # Add test to CTest
    include(GoogleTest)
    gtest_discover_tests(system_tests)
endif()
