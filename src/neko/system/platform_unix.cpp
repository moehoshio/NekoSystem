// Include header for non-module usage
#if !defined(NEKO_SYSTEM_ENABLE_MODULE) || (NEKO_SYSTEM_ENABLE_MODULE == false)

#include <neko/system/platform.hpp>

#include <string>

#include <sys/utsname.h>

#else

module neko.system;

#include <string>

#include <sys/utsname.h>

#endif

namespace neko::system {
    std::string getOsVersion() {
        struct utsname uts;
        if (uname(&uts) == 0) {
            return std::string(uts.release);
        }
        return "unknown";
    }
} // namespace neko::system
