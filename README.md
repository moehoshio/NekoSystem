# NekoSystem

A cross-platform C++20 system information library providing memory information, platform detection, and system utilities.

[![License](https://img.shields.io/badge/License-MIT%20OR%20Apache--2.0-blue.svg)](LICENSE)
![Require](https://img.shields.io/badge/%20Require%20-%3E=%20C++%2020-orange.svg)
[![CMake](https://img.shields.io/badge/CMake-3.14+-green.svg)](https://cmake.org/)

## Features

- **Platform-consistent paths**: All paths use forward slashes `/` to avoid backslashes, ensuring cross-platform consistency
- **Memory Information**: Get system memory statistics (total, free memory)
- **Platform Detection**: Detect operating system (Windows, macOS, Linux) and architecture (x64, x86, ARM64, ARM)
- **System Utilities**: Work with temporary directories, working paths, and home directories
- **Cross-platform**: Supports Windows, macOS, and Linux
- **Modern C++**: Built with C++20 features and best practices

## Quick Start

Configure:
[CMake](#cmake) | [Tests](#testing)

Example:
[Memory Info](#memory-information) | [Platform Detection](#platform-detection) | [System Utilities](#system-utilities)

### Prerequisites

- C++20 compatible compiler
- CMake 3.14 or higher

## Integration

### CMake

```cmake
include(FetchContent)
FetchContent_Declare(
    NekoSystem
    GIT_REPOSITORY https://github.com/moehoshio/NekoSystem.git
    GIT_TAG        main
)
FetchContent_MakeAvailable(NekoSystem)

# Add your target and link NekoSystem
add_executable(your_target main.cpp)

target_link_libraries(your_target PRIVATE Neko::System)
```

## Usage Examples

### Memory Information

```cpp
#include <neko/system/memoryinfo.hpp>
#include <iostream>

int main() {
    auto memInfo = neko::system::getSystemMemoryInfo();
    if (memInfo) {
        std::cout << "Memory Info: " << memInfo->toString() << std::endl;
        std::cout << "Total: " << memInfo->totalBytes << " bytes" << std::endl;
        std::cout << "Free: " << memInfo->freeBytes << " bytes" << std::endl;
    }
    return 0;
}
```

### Platform Detection

```cpp
#include <neko/system/platform.hpp>
#include <iostream>

int main() {
    using namespace neko::system;
    
    std::cout << "OS: " << getOsName() << std::endl;
    std::cout << "Architecture: " << getOsArch() << std::endl;
    std::cout << "OS Version: " << getOsVersion() << std::endl;
    
    // Platform-specific checks
    if (isWindows()) {
        std::cout << "Running on Windows" << std::endl;
    } else if (isMacOS()) {
        std::cout << "Running on macOS" << std::endl;
    } else if (isLinux()) {
        std::cout << "Running on Linux" << std::endl;
    }
    
    return 0;
}
```

### System Utilities

```cpp
#include <neko/system/platform.hpp>
#include <iostream>

int main() {
    using namespace neko::system;
    
    // Get system directories
    std::cout << "Temp folder: " << tempFolder() << std::endl;
    std::cout << "Work path: " << workPath() << std::endl;
    
    auto home = getHome();
    if (home) {
        std::cout << "Home directory: " << *home << std::endl;
    }
    
    return 0;
}
```

## API Reference

### Memory Information API

#### `neko::system::MemoryInfo`

```cpp
struct MemoryInfo {
    neko::uint64 totalBytes;  // Total physical memory in bytes
    neko::uint64 freeBytes;   // Free physical memory in bytes
    
    std::string toString() const;  // Human-readable format
};
```

#### `neko::system::getSystemMemoryInfo()`

```cpp
std::optional<MemoryInfo> getSystemMemoryInfo();
```

Returns system memory information or `std::nullopt` on failure.

### Platform Detection API

#### Constants

```cpp
constexpr neko::cstr osName;    // "windows", "osx", "linux", or "unknown"
constexpr neko::cstr osArch;    // "x64", "x86", "arm64", "arm", or "unknown"
```

#### Functions

```cpp
constexpr neko::cstr getOsName();           // Get OS name
constexpr neko::cstr getOsArch();           // Get architecture
std::string getOsVersion();                 // Get OS version string

// Platform detection helpers
constexpr bool isWindows();
constexpr bool isMacOS();
constexpr bool isLinux();

// Architecture detection helpers
constexpr bool isArchX64();
constexpr bool isArchX86();
constexpr bool isArchArm64();
constexpr bool isArchArm();
```

### System Utilities API

```cpp
// Directory management
std::string tempFolder(const std::string &setTempDir = "");
std::string workPath(const std::string &setPath = "");
std::optional<std::string> getHome();
```

## Testing

You can run the tests to verify that everything is working correctly.

If you haven't configured the build yet, please run:

```shell
cmake -B ./build . -DNEKO_BUILD_TESTS=ON -DNEKO_AUTO_FETCH_DEPS=ON
```

Now, you can build the test files (you must build them manually at least once before running the tests!).

```shell
cmake --build ./build --config Debug
```

Then, you can run the tests with the following commands:

Unix Makefile / Ninja generator：

```shell
cmake --build ./build --target test
```

Visual Studio generator：

```shell
cmake --build ./build --config Debug --target RUN_TESTS
```

If everything is set up correctly, you should see output similar to the following:

```shell
  Test project /path/to/NekoSystem/build
        Start  1: MemoryInfoTest.GetSystemMemoryInfoReturn 
  sValidResult
   1/41 Test  #1: MemoryInfoTest.GetSystemMemoryInfoReturn
  sValidResult ...   Passed    0.02 sec

#   ......

        Start 41: NekoFunction_tests
  41/41 Test #41: NekoFunction_tests .....................
  ................   Passed    0.02 sec
  
  100% tests passed, 0 tests failed out of 41

  Total Test time (real) =   0.81 sec
```

### Disable Tests

If you want to disable building and running tests, you can set the following CMake option when configuring your project:

```shell
cmake -B ./build . -DNEKO_BUILD_TESTS=OFF
```

This will skip test targets during the build process.

## License

[License](LICENSE) MIT OR Apache-2.0

## See More

- [NekoLog](https://github.com/moehoshio/nlog): An easy-to-use, modern, lightweight, and efficient C++20 logging library.
- [NekoEvent](https://github.com/moehoshio/NekoEvent): A modern easy to use type-safe and high-performance event handling system for C++.
- [NekoSchema](https://github.com/moehoshio/NekoSchema): A lightweight, header-only C++20 schema library.
- [NekoSystem](https://github.com/moehoshio/NekoSystem): A modern C++20 cross-platform system utility library.
- [NekoFunction](https://github.com/moehoshio/NekoFunction): A comprehensive modern C++ utility library that provides practical functions for common programming tasks.
- [NekoThreadPool](https://github.com/moehoshio/NekoThreadPool): An easy to use and efficient C++ 20 thread pool that supports priorities and submission to specific threads.
