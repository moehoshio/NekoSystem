#include <neko/system/platform.hpp>

#include <sys/utsname.h>
#include <string>

namespace neko::system {
    std::string getOsVersion() {
        struct utsname uts;
        if (uname(&uts) == 0) {
            return std::string(uts.release);
        }
        return "unknown";
    }
} // namespace neko::system
