cmake_minimum_required(VERSION 3.14)
project(NekoSystem VERSION 1.0 LANGUAGES CXX)

# ================
# === Config ====
# ================

# NekoSystem specific options
option(NEKO_SYSTEM_BUILD_TESTS "Neko System Build tests" ON)
option(NEKO_SYSTEM_AUTO_FETCH_DEPS "Neko System Automatically fetch dependencies" ON)

find_package(GTest CONFIG QUIET)

# Print configuration summary
message(STATUS "Start configuration Neko System...")
message(STATUS "")
message(STATUS "NekoSystem configuration summary:")
message(STATUS "  - CMake version: ${CMAKE_VERSION}")
message(STATUS "")
message(STATUS "  - Neko System Auto fetch deps: ${NEKO_SYSTEM_AUTO_FETCH_DEPS}")
message(STATUS "  - Neko System Build tests: ${NEKO_SYSTEM_BUILD_TESTS}")
message(STATUS "")
message(STATUS "Dependency summary:")
message(STATUS "  - GTest : ${GTest_FOUND} version : ${GTest_VERSION}")
message(STATUS "")


if(NEKO_SYSTEM_AUTO_FETCH_DEPS)
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

    if(NOT GTEST_FOUND AND NEKO_SYSTEM_BUILD_TESTS)
        message(STATUS "GTest not found; Neko System Fetching GoogleTest...")

        FetchContent_Declare(
            googletest
            GIT_REPOSITORY https://github.com/google/googletest.git
            GIT_TAG        v1.17.0
        )
        # For Windows: Prevent overriding the parent project's compiler/linker settings
        set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
        FetchContent_MakeAvailable(googletest)
    endif()

endif(NEKO_SYSTEM_AUTO_FETCH_DEPS)

set(SRCFILES)

if(WIN32)
    list(APPEND SRCFILES
        ${CMAKE_CURRENT_SOURCE_DIR}/src/neko/system/memoryinfo_windows.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/neko/system/platform_windows.cpp
    )
elseif(APPLE)
    list(APPEND SRCFILES 
        ${CMAKE_CURRENT_SOURCE_DIR}/src/neko/system/memoryinfo_macos.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/neko/system/platform_unix.cpp
    )
elseif(UNIX)
    list(APPEND SRCFILES 
        ${CMAKE_CURRENT_SOURCE_DIR}/src/neko/system/memoryinfo_linux.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/neko/system/platform_unix.cpp
    )
endif()


# ================
# = Main target =
# ================

add_library(NekoSystem STATIC ${SRCFILES})
add_library(Neko::System ALIAS NekoSystem)

target_include_directories(NekoSystem PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
)
target_link_libraries(NekoSystem PUBLIC NekoSchema NekoFunction)
target_compile_features(NekoSystem PUBLIC cxx_std_20)


# ================
# ==== Tests =====
# ================

if(NEKO_SYSTEM_BUILD_TESTS)
    enable_testing()
    message(STATUS "NekoSystem tests enabled (NEKO_SYSTEM_BUILD_TESTS=ON)")

    if (NOT GTEST_FOUND AND NOT NEKO_SYSTEM_AUTO_FETCH_DEPS)
        message(STATUS "GTest is required for building tests but was not found.")
        message(FATAL_ERROR "Please enable -DNEKO_SYSTEM_AUTO_FETCH_DEPS=ON or install GTest and make it discoverable by CMake. e.g -DCMAKE_PREFIX_PATH=</path/to/googletest>")
    endif()

    add_executable(NekoSystem_tests tests/system_test.cpp)
    target_include_directories(NekoSystem_tests PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include)
    target_link_libraries(NekoSystem_tests PRIVATE NekoSystem GTest::gtest_main)
    
    # Add test to CTest
    include(GoogleTest)
    gtest_discover_tests(NekoSystem_tests)
    
else()
    message(STATUS "NekoSystem tests disabled (NEKO_SYSTEM_BUILD_TESTS=OFF)")
endif()

message(STATUS "Neko System End of configuration")