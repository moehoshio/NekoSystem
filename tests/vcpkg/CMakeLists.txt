cmake_minimum_required(VERSION 3.14)
project(vcpkg_test LANGUAGES CXX)

# Find NekoSystem package installed via vcpkg
find_package(NekoSystem CONFIG REQUIRED)

# Create test executable
add_executable(vcpkg_test test.cpp)

# Link against NekoSystem
target_link_libraries(vcpkg_test PRIVATE Neko::System)

# Set C++20 standard
target_compile_features(vcpkg_test PRIVATE cxx_std_20)
