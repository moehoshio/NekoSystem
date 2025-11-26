// Include header for non-module usage
#if !defined(NEKO_SYSTEM_ENABLE_MODULE) || (NEKO_SYSTEM_ENABLE_MODULE == false)

#include <neko/system/platform.hpp>

// Windows specific includes
#define NOMINMAX
#include <windows.h>
#include <vector>

#else

module neko.system;

// Windows specific includes
#define NOMINMAX
#include <windows.h>
#include <vector>

#endif

// RtlGetVersion function pointer type
typedef NTSTATUS(WINAPI* RtlGetVersionPtr)(PRTL_OSVERSIONINFOW);

namespace neko::system {
    std::string getOsVersion() {
        // Use RtlGetVersion to avoid deprecation warning
        HMODULE ntdll = GetModuleHandleW(L"ntdll.dll");
        if (ntdll) {
            RtlGetVersionPtr RtlGetVersion = (RtlGetVersionPtr)GetProcAddress(ntdll, "RtlGetVersion");
            if (RtlGetVersion) {
                RTL_OSVERSIONINFOW info = {0};
                info.dwOSVersionInfoSize = sizeof(info);
                if (RtlGetVersion(&info) == 0) {
                    std::vector<char> buf(64);
                    snprintf(buf.data(), buf.size(), "%lu.%lu.%lu",
                             info.dwMajorVersion, info.dwMinorVersion, info.dwBuildNumber);
                    return std::string(buf.data());
                }
            }
        }
        return "unknown";
    }
} // namespace neko::system
