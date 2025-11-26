// Include header for non-module usage
#if !defined(NEKO_SYSTEM_ENABLE_MODULE) || (NEKO_SYSTEM_ENABLE_MODULE == false)

#include <neko/system/memoryinfo.hpp>

#include <optional>

// Windows specific includes
#define NOMINMAX
#include <windows.h>

#else

module neko.system;

#include <optional>

// Windows specific includes
#define NOMINMAX
#include <windows.h>

#endif

namespace neko::system {

    std::optional<MemoryInfo> getSystemMemoryInfo() {

        MEMORYSTATUSEX statex;
        statex.dwLength = sizeof(statex);
        if (GlobalMemoryStatusEx(&statex)) {
            return MemoryInfo{
                statex.ullTotalPhys,
                statex.ullAvailPhys};
        }
        return std::nullopt;
    }

} // namespace neko::system