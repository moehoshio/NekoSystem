#include <neko/system/memoryinfo.hpp>
#include <neko/system/platform.hpp>
#include <iostream>

int main() {
    std::cout << "=== NekoSystem vcpkg Integration Test ===" << std::endl;
    
    // Test Platform detection
    std::cout << "\n[Platform Information]" << std::endl;
    std::cout << "Platform: " << neko::system::platform::name() << std::endl;
    std::cout << "OS Family: " << neko::system::platform::os_family() << std::endl;
    std::cout << "Architecture: " << neko::system::platform::architecture() << std::endl;
    
    // Test Memory information
    std::cout << "\n[Memory Information]" << std::endl;
    auto memory = neko::system::memoryinfo::get();
    std::cout << "Total Memory: " << memory.total_physical << " bytes" << std::endl;
    std::cout << "Available Memory: " << memory.available_physical << " bytes" << std::endl;
    std::cout << "Used Memory: " << (memory.total_physical - memory.available_physical) << " bytes" << std::endl;
    
    std::cout << "\n=== Test Completed Successfully ===" << std::endl;
    return 0;
}
