cmake_minimum_required(VERSION 3.14)
project(conan_test LANGUAGES CXX)

# Find NekoSystem package installed via Conan
find_package(NekoSystem CONFIG REQUIRED)

# Create test executable
add_executable(conan_test test.cpp)

# Link against NekoSystem
target_link_libraries(conan_test PRIVATE Neko::System)

# Set C++20 standard
target_compile_features(conan_test PRIVATE cxx_std_20)
