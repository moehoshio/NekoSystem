// Include header for non-module usage
#if !defined(NEKO_SYSTEM_ENABLE_MODULE) || (NEKO_SYSTEM_ENABLE_MODULE == false)

#include <neko/schema/types.hpp>
#include <neko/system/memoryinfo.hpp>

#include <optional>

// unix specific includes
#include <sys/sysinfo.h>

#else

module neko.system;

import neko.schema;

#include <optional>

// unix specific includes
#include <sys/sysinfo.h>

#endif

namespace neko::system {
    std::optional<MemoryInfo> getSystemMemoryInfo() {
        struct ::sysinfo sys_info;
        if (::sysinfo(&sys_info) == 0) {
            neko::uint64 total = sys_info.totalram;
            neko::uint64 free = sys_info.freeram;
#ifdef __linux__
            total *= sys_info.mem_unit;
            free *= sys_info.mem_unit;
#endif
            return MemoryInfo{total, free};
        }
        return std::nullopt;
    }

} // namespace neko::system