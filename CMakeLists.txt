cmake_minimum_required(VERSION 3.14)

# Set policy for MSVC runtime library before project()
if(POLICY CMP0091)
    cmake_policy(SET CMP0091 NEW)
endif()

# Set MSVC runtime library based on vcpkg CRT linkage
# This must be set before project() command
if(MSVC)
    if(DEFINED VCPKG_CRT_LINKAGE)
        if(VCPKG_CRT_LINKAGE STREQUAL "static")
            set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")
        else()
            set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>DLL")
        endif()
    elseif(NOT DEFINED CMAKE_MSVC_RUNTIME_LIBRARY)
        # Default to dynamic runtime when not using vcpkg
        set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>DLL")
    endif()
endif()

project(NekoSystem VERSION 1.0.0 LANGUAGES CXX)

# ================
# === Config ====
# ================

# NekoSystem specific options
option(NEKO_SYSTEM_BUILD_TESTS "Neko System Build tests" ON)
option(NEKO_SYSTEM_AUTO_FETCH_DEPS "Neko System Automatically fetch dependencies" ON)
option(NEKO_SYSTEM_ENABLE_MODULE "Neko System Enable module support" OFF)

find_package(NekoSchema QUIET)
find_package(NekoFunction QUIET)
find_package(GTest QUIET)

# Print configuration summary
message(STATUS "Start configuration Neko System...")
message(STATUS "")
message(STATUS "NekoSystem configuration summary:")
message(STATUS "  - CMake version: ${CMAKE_VERSION}")
message(STATUS "")
message(STATUS "  - Neko System Auto fetch deps: ${NEKO_SYSTEM_AUTO_FETCH_DEPS}")
message(STATUS "  - Neko System Build tests: ${NEKO_SYSTEM_BUILD_TESTS}")
message(STATUS "  - Neko System Enable module: ${NEKO_SYSTEM_ENABLE_MODULE}")
message(STATUS "")
message(STATUS "Dependency summary:")
message(STATUS "  - NekoSchema : ${NekoSchema_FOUND} version : ${NekoSchema_VERSION}")
message(STATUS "  - NekoFunction : ${NekoFunction_FOUND} version : ${NekoFunction_VERSION}")
message(STATUS "  - GTest : ${GTest_FOUND} version : ${GTest_VERSION}")
message(STATUS "")


if(NEKO_SYSTEM_AUTO_FETCH_DEPS)
    include(FetchContent)

    if (NOT NekoSchema_FOUND)
        message(STATUS "NekoSchema not found; Neko System Fetching NekoSchema...")
        FetchContent_Declare(
            NekoSchema
            GIT_REPOSITORY https://github.com/moehoshio/NekoSchema.git
            GIT_TAG        main
        )
        set(NEKO_SCHEMA_ENABLE_MODULE ${NEKO_SYSTEM_ENABLE_MODULE} CACHE BOOL "" FORCE)
        set(NEKO_SCHEMA_BUILD_TESTS OFF CACHE BOOL "" FORCE)
        FetchContent_MakeAvailable(NekoSchema)
    endif()
    

    if (NOT NekoFunction_FOUND)
        message(STATUS "NekoFunction not found; Neko System Fetching NekoFunction...")
        FetchContent_Declare(
            NekoFunction
            GIT_REPOSITORY https://github.com/moehoshio/NekoFunction.git
            GIT_TAG        main
        )
        set(NEKO_FUNCTION_ENABLE_MODULE ${NEKO_SYSTEM_ENABLE_MODULE} CACHE BOOL "" FORCE)
        set(NEKO_FUNCTION_BUILD_TESTS OFF CACHE BOOL "" FORCE)
        FetchContent_MakeAvailable(NekoFunction)
    endif()

    if(NOT GTest_FOUND AND NEKO_SYSTEM_BUILD_TESTS)
        message(STATUS "GTest not found; Neko System Fetching GoogleTest...")
        FetchContent_Declare(
            googletest
            GIT_REPOSITORY https://github.com/google/googletest.git
            GIT_TAG        v1.17.0
        )
        set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
        FetchContent_MakeAvailable(googletest)
    endif()

endif(NEKO_SYSTEM_AUTO_FETCH_DEPS)

# ================
# = Main target =
# ================

set(NEKO_SYSTEM_SRCFILES)

if(WIN32)
    list(APPEND NEKO_SYSTEM_SRCFILES
        ${CMAKE_CURRENT_SOURCE_DIR}/src/neko/system/memoryinfo_windows.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/neko/system/platform_windows.cpp
    )
elseif(APPLE)
    list(APPEND NEKO_SYSTEM_SRCFILES
        ${CMAKE_CURRENT_SOURCE_DIR}/src/neko/system/memoryinfo_macos.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/neko/system/platform_unix.cpp
    )
elseif(UNIX)
    list(APPEND NEKO_SYSTEM_SRCFILES 
        ${CMAKE_CURRENT_SOURCE_DIR}/src/neko/system/memoryinfo_linux.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/neko/system/platform_unix.cpp
    )
else()
    message(FATAL_ERROR "Unsupported platform for NekoSystem")
endif()

add_library(NekoSystem STATIC ${NEKO_SYSTEM_SRCFILES})
add_library(Neko::System ALIAS NekoSystem)

target_include_directories(NekoSystem PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
)
target_link_libraries(NekoSystem PUBLIC NekoSchema NekoFunction)
target_compile_features(NekoSystem PUBLIC cxx_std_20)

# Set MSVC runtime library on the target
if(MSVC)
    if(CMAKE_MSVC_RUNTIME_LIBRARY)
        set_target_properties(NekoSystem PROPERTIES
            MSVC_RUNTIME_LIBRARY "${CMAKE_MSVC_RUNTIME_LIBRARY}"
        )
        message(STATUS "NekoSystem: Using MSVC runtime: ${CMAKE_MSVC_RUNTIME_LIBRARY}")
    endif()
endif()

# ================
# = C++20 Module =
# ================

if(NEKO_SYSTEM_ENABLE_MODULE)
    message(STATUS "Neko System C++20 module enabled (NEKO_SYSTEM_ENABLE_MODULE=ON)")

    # Check CMake version for module support
    if(CMAKE_VERSION VERSION_LESS 3.28)
        message(WARNING "CMake 3.28+ is recommended for full C++20 module support")
    endif()

    add_library(NekoSystem_module)
    add_library(Neko::System::Module ALIAS NekoSystem_module)
    
    # Only .cppm files go in CXX_MODULES
    target_sources(NekoSystem_module
        PUBLIC
            FILE_SET CXX_MODULES FILES
                ${CMAKE_CURRENT_SOURCE_DIR}/include/neko/system/neko.system.cppm
        PRIVATE
            ${NEKO_SYSTEM_SRCFILES}
    )
    
    target_link_libraries(NekoSystem_module PUBLIC Neko::Schema::Module Neko::Function::Module)
    target_compile_features(NekoSystem_module PUBLIC cxx_std_20)

    target_include_directories(NekoSystem_module PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>
    )
else()
    message(STATUS "Neko System C++20 module disabled (NEKO_SYSTEM_ENABLE_MODULE=OFF)")
endif()

# ================
# ==== Tests =====
# ================

if(NEKO_SYSTEM_BUILD_TESTS)
    enable_testing()
    message(STATUS "NekoSystem tests enabled (NEKO_SYSTEM_BUILD_TESTS=ON)")

    if (NOT GTest_FOUND AND NOT NEKO_SYSTEM_AUTO_FETCH_DEPS)
        message(WARNING "GTest is required for building tests but was not found.")
        message(FATAL_ERROR "Please enable -DNEKO_SYSTEM_AUTO_FETCH_DEPS=ON or install GTest and make it discoverable by CMake. e.g -DCMAKE_PREFIX_PATH=</path/to/googletest>")
    endif()

    add_executable(NekoSystem_tests tests/system_test.cpp)
    target_include_directories(NekoSystem_tests PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include)
    target_link_libraries(NekoSystem_tests PRIVATE Neko::System GTest::gtest GTest::gtest_main)
    
    include(GoogleTest)
    gtest_discover_tests(NekoSystem_tests)

    # Module test (if modules are enabled)
    if(NEKO_SYSTEM_ENABLE_MODULE)
        if (MSVC)
            message(STATUS "NekoSystem module tests enabled (MSVC only)")

            add_executable(system_module_test tests/system_module_test.cpp)
            target_link_libraries(system_module_test PRIVATE Neko::Schema::Module Neko::Function::Module Neko::System::Module GTest::gtest GTest::gtest_main)
            target_compile_features(system_module_test PRIVATE cxx_std_20)
            gtest_discover_tests(system_module_test)
        else()
            message(STATUS "Skipping NekoSystem module tests on non-MSVC toolchains")
        endif()
    endif()
    
else()
    message(STATUS "NekoSystem tests disabled (NEKO_SYSTEM_BUILD_TESTS=OFF)")
endif()

# ================
# == Install =====
# ================

include(GNUInstallDirs)
include(CMakePackageConfigHelpers)

# Install headers
install(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/include/
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
    FILES_MATCHING 
    PATTERN "*.hpp"
    PATTERN "*.cppm"
)

# Install targets
install(TARGETS NekoSystem
    EXPORT NekoSystemTargets
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    INCLUDES DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
    FILE_SET CXX_MODULES DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
)

# Install export targets
install(EXPORT NekoSystemTargets
    FILE NekoSystemTargets.cmake
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/NekoSystem
)

# Create and install Config file
configure_package_config_file(
    ${CMAKE_CURRENT_SOURCE_DIR}/cmake/NekoSystemConfig.cmake.in
    ${CMAKE_CURRENT_BINARY_DIR}/NekoSystemConfig.cmake
    INSTALL_DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/NekoSystem
)

# Create and install Version file
write_basic_package_version_file(
    ${CMAKE_CURRENT_BINARY_DIR}/NekoSystemConfigVersion.cmake
    VERSION ${PROJECT_VERSION}
    COMPATIBILITY SameMajorVersion
    ARCH_INDEPENDENT
)

install(FILES
    ${CMAKE_CURRENT_BINARY_DIR}/NekoSystemConfig.cmake
    ${CMAKE_CURRENT_BINARY_DIR}/NekoSystemConfigVersion.cmake
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/NekoSystem
)

message(STATUS "Neko System End of configuration")