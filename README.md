# NekoSystem

A cross-platform C++20 system information library providing memory information, platform detection, and system utilities.

[![License](https://img.shields.io/badge/License-MIT%20OR%20Apache--2.0-blue.svg)](LICENSE)
![Require](https://img.shields.io/badge/%20Require%20-%3E=%20C++%2020-orange.svg)
[![CMake](https://img.shields.io/badge/CMake-3.14+-green.svg)](https://cmake.org/)

## Features

- **Memory Information**: Get system memory statistics (total, free memory)
- **Platform Detection**: Detect operating system (Windows, macOS, Linux) and architecture (x64, x86, ARM64, ARM)
- **System Utilities**: Work with temporary directories, working paths, and home directories
- **Cross-platform**: Supports Windows, macOS, and Linux
- **Header-only Interface**: Easy integration with minimal setup
- **Modern C++**: Built with C++20 features and best practices

## Quick Start

### Prerequisites

- C++20 compatible compiler
- CMake 3.14 or higher

### Integration

#### Using CMake FetchContent (Recommended)

```cmake
include(FetchContent)
FetchContent_Declare(
    NekoSystem
    GIT_REPOSITORY https://github.com/moehoshio/NekoSystem.git
    GIT_TAG        main
)
FetchContent_MakeAvailable(NekoSystem)

target_link_libraries(your_target PRIVATE Neko::System)
```

#### Manual Installation

```bash
git clone https://github.com/moehoshio/NekoSystem.git
cd NekoSystem
mkdir build && cd build
cmake ..
cmake --build .
cmake --install .
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

## Building and Testing

### Build Options

- `NEKO_AUTO_FETCH_DEPS`: Automatically fetch dependencies (default: ON)
- `NEKO_BUILD_TESTS`: Build unit tests (default: ON)

### Building

```bash
# Clone with dependencies
git clone --recursive https://github.com/moehoshio/NekoSystem.git
cd NekoSystem

# Configure
mkdir build && cd build
cmake .. -DNEKO_BUILD_TESTS=ON

# Build
cmake --build . --config Release

# Run tests
ctest --output-on-failure
```

### Running Tests

The project includes comprehensive unit tests covering:

- Memory information retrieval
- Platform and architecture detection
- System utility functions
- Cross-component integration

```bash
# Run all tests
ctest --output-on-failure

# Run specific test executable
./build/Debug/system_tests  # Windows
./build/system_tests        # Unix-like systems
```

## Dependencies

- **NekoSchema**: Type definitions and schema utilities
- **NekoFunction**: Utility functions (path normalization, etc.)
- **GoogleTest**: Testing framework (test builds only)

Dependencies are automatically fetched via CMake FetchContent when `NEKO_AUTO_FETCH_DEPS` is enabled.

## Supported Platforms

| Platform | Compiler | Status |
|----------|----------|--------|
| Windows 10/11 | MSVC 2019+ | ✅ Fully Supported |
| Windows 10/11 | Clang 10+ | ✅ Fully Supported |
| macOS 10.15+ | Clang 10+ | ✅ Fully Supported |
| macOS 10.15+ | GCC 10+ | ✅ Fully Supported |
| Ubuntu 20.04+ | GCC 10+ | ✅ Fully Supported |
| Ubuntu 20.04+ | Clang 10+ | ✅ Fully Supported |

## Project Structure

```text
NekoSystem/
├── include/
│   └── neko/
│       └── system/
│           ├── memoryinfo.hpp    # Memory information API
│           └── platform.hpp      # Platform detection API
├── src/
│   └── neko/
│       └── system/
│           ├── memoryinfo_windows.cpp
│           ├── memoryinfo_macos.cpp
│           ├── memoryinfo_linux.cpp
│           ├── platform_windows.cpp
│           └── platform_unix.cpp
├── tests/
│   └── system_test.cpp          # Comprehensive unit tests
├── CMakeLists.txt               # Build configuration
└── README.md                    # This file
```

## Contributing

We welcome contributions! Please feel free to submit a Pull Request. For major changes, please open an issue first to discuss what you would like to change.

### Development Setup

1. Fork the repository
2. Clone your fork: `git clone https://github.com/yourusername/NekoSystem.git`
3. Create a feature branch: `git checkout -b feature/amazing-feature`
4. Make your changes and add tests
5. Run tests: `cmake --build build && ctest --test-dir build --output-on-failure`
6. Commit your changes: `git commit -m 'Add some amazing feature'`
7. Push to the branch: `git push origin feature/amazing-feature`
8. Open a Pull Request

### Code Style

- Follow the existing code style
- Use C++20 features appropriately
- Add tests for new functionality
- Update documentation as needed

## License

This project is dual-licensed under either:

- The MIT License (MIT), or
- The Apache License, Version 2.0 (Apache-2.0)

You may use this project under the terms of either license, at your option.

See [LICENSE](LICENSE) for the full license texts.

## Changelog

### Version 1.0.0

- Initial release
- Memory information API
- Platform detection API
- System utilities API
- Comprehensive test suite
- Cross-platform support (Windows, macOS, Linux)

## Acknowledgments

- Built with modern C++20 features
- Uses CMake for cross-platform building
- Integrated with GoogleTest for reliable testing
- Part of the Neko ecosystem of C++ libraries

## See More

- [NekoLog](https://github.com/moehoshio/nlog): An easy-to-use, modern, lightweight, and efficient C++20 logging library.
- [NekoEvent](https://github.com/moehoshio/NekoEvent): A modern easy to use type-safe and high-performance event handling system for C++.
- [NekoSchema](https://github.com/moehoshio/NekoSchema): A lightweight, header-only C++20 schema library.
- [NekoSystem](https://github.com/moehoshio/NekoSystem): A modern C++20 cross-platform system utility library.
- [NekoFunction](https://github.com/moehoshio/NekoFunction): A comprehensive modern C++ utility library that provides practical functions for common programming tasks.
- [NekoThreadPool](https://github.com/moehoshio/NekoThreadPool): An easy to use and efficient C++ 20 thread pool that supports priorities and submission to specific threads.
