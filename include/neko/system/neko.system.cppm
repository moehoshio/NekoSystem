// =====================
// === Global Module ===
// =====================

module;

// ====================
// = Standard Library =
// ====================

#include <filesystem>
#include <mutex>
#include <optional>
#include <sstream>
#include <string>

// =====================
// = Module Interface ==
// =====================

export module neko.system;

import neko.schema;
import neko.function;

// Control header files to not import dependencies (dependencies are declared and imported by the cppm)
#define NEKO_SYSTEM_ENABLE_MODULE true

export {
#include "memoryinfo.hpp"
#include "platform.hpp"
}