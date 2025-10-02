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
    FetchContent_MakeAvailable(NekoFunction)

    if(NEKO_BUILD_TESTS)
        find_package(GTest QUIET)

        if(NOT GTest_FOUND)
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
        endif(NOT GTest_FOUND)

    endif(NEKO_BUILD_TESTS)

endif(NEKO_AUTO_FETCH_DEPS)

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


# Testing
if(NEKO_BUILD_TESTS)
    if (NOT NEKO_AUTO_FETCH_DEPS AND NOT GTest_FOUND)
        message(FATAL_ERROR "GTest is required for building tests. Please enable NEKO_AUTO_FETCH_DEPS or install GTest manually.")
    endif()

    enable_testing()
    add_executable(system_tests tests/system_test.cpp)
    target_include_directories(system_tests PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include)
    target_link_libraries(system_tests 
        PRIVATE 
        NekoSystem
        GTest::gtest_main
    )
    target_compile_features(system_tests PRIVATE cxx_std_20)
    
    # Add test to CTest
    include(GoogleTest)
    gtest_discover_tests(system_tests)
endif()
