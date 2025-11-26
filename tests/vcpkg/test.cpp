#include <neko/system/memoryinfo.hpp>
#include <neko/system/platform.hpp>
#include <iostream>

int main() {
    std::cout << "=== NekoSystem vcpkg Integration Test ===" << std::endl;
    
    // Test Platform detection
    std::cout << "\n[Platform Information]" << std::endl;
    std::cout << "OS Name: " << neko::system::getOsName() << std::endl;
    std::cout << "Architecture: " << neko::system::getOsArch() << std::endl;
    std::cout << "Is Windows: " << (neko::system::isWindows() ? "Yes" : "No") << std::endl;
    std::cout << "Is macOS: " << (neko::system::isMacOS() ? "Yes" : "No") << std::endl;
    std::cout << "Is Linux: " << (neko::system::isLinux() ? "Yes" : "No") << std::endl;
    
    // Test Memory information
    std::cout << "\n[Memory Information]" << std::endl;
    auto memInfo = neko::system::getSystemMemoryInfo();
    if (memInfo.has_value()) {
        std::cout << "Total Memory: " << memInfo->totalBytes << " bytes" << std::endl;
        std::cout << "Free Memory: " << memInfo->freeBytes << " bytes" << std::endl;
        std::cout << "Used Memory: " << (memInfo->totalBytes - memInfo->freeBytes) << " bytes" << std::endl;
        std::cout << "Memory Info: " << memInfo->toString() << std::endl;
    } else {
        std::cout << "Failed to get memory information" << std::endl;
        return 1;
    }
    
    // Test directory functions
    std::cout << "\n[Directory Information]" << std::endl;
    std::cout << "Work Path: " << neko::system::workPath() << std::endl;
    std::cout << "Temp Folder: " << neko::system::tempFolder() << std::endl;
    auto home = neko::system::getHome();
    if (home.has_value()) {
        std::cout << "Home Directory: " << home.value() << std::endl;
    }
    
    std::cout << "\n=== Test Completed Successfully ===" << std::endl;
    return 0;
}
