/**
 * @file system_module_test.cpp
 * @brief Minimal unit tests for the neko.system C++20 module.
 * 
 * Note: This is a minimal version due to MSVC C++20 module compiler bugs (C1001).
 * The compiler crashes when calling module-exported functions in test context.
 * Full test coverage is provided by system_test.cpp using traditional headers.
 * 
 * This test file primarily verifies that the module can be imported successfully.
 */

#include <gtest/gtest.h>

import neko.schema;
import neko.function;
import neko.system;

namespace neko::system::tests::module {

    /**
     * @brief Test that the module can be imported successfully.
     */
    TEST(ModuleBasicTest, ModuleImportsSuccessfully) {
        // If we reach here, the module imported successfully
        SUCCEED() << "neko.system module imported successfully";
    }

    /**
     * @brief Test compile-time constants are accessible.
     */
    TEST(ModuleBasicTest, CompileTimeConstantsAccessible) {
        // Test that compile-time constants are accessible
        constexpr neko::cstr os = osName;
        constexpr neko::cstr arch = osArch;
        
        EXPECT_NE(os, nullptr) << "osName should not be null";
        EXPECT_NE(arch, nullptr) << "osArch should not be null";
        
        // Verify expected values
        if constexpr (isWindows()) {
            EXPECT_STREQ(os, "windows");
        } else if constexpr (isMacOS()) {
            EXPECT_STREQ(os, "osx");
        } else if constexpr (isLinux()) {
            EXPECT_STREQ(os, "linux");
        }
    }

    /**
     * @brief Test platform detection constexpr functions.
     */
    TEST(ModuleBasicTest, PlatformDetectionConstexprFunctions) {
#ifdef _WIN32
        EXPECT_TRUE(isWindows());
        EXPECT_FALSE(isMacOS());
        EXPECT_FALSE(isLinux());
#elif defined(__APPLE__)
        EXPECT_TRUE(isMacOS());
        EXPECT_FALSE(isWindows());
        EXPECT_FALSE(isLinux());
#elif defined(__linux__)
        EXPECT_TRUE(isLinux());
        EXPECT_FALSE(isMacOS());
        EXPECT_FALSE(isWindows());
#endif
    }

    /**
     * @brief Test architecture detection constexpr functions.
     */
    TEST(ModuleBasicTest, ArchitectureDetectionConstexprFunctions) {
        bool hasArch = isArchX64() || isArchX86() || isArchArm64() || isArchArm();
        EXPECT_TRUE(hasArch) << "At least one architecture should be detected";
        
        int archCount = 0;
        if (isArchX64()) archCount++;
        if (isArchX86()) archCount++;
        if (isArchArm64()) archCount++;
        if (isArchArm()) archCount++;
        
        EXPECT_EQ(archCount, 1) << "Exactly one architecture should be detected";
    }

    /**
     * @brief Test that types from neko.schema are accessible (transitive import).
     */
    TEST(ModuleBasicTest, TransitiveImportTypes) {
        // This tests that neko.schema types are available through neko.system
        neko::uint64 value = 42;
        EXPECT_EQ(value, 42ULL);
        
        // Verify the type is correct
        bool isSameType = std::is_same_v<decltype(value), uint64_t>;
        EXPECT_TRUE(isSameType);
    }

    // ============= Memory Information Tests =============

    /**
     * @brief Test the toString method of MemoryInfo structure.
     */
    TEST(MemoryInfoModuleTest, MemoryInfoToStringWorks) {
        MemoryInfo info;
        info.totalBytes = 8ULL * 1024 * 1024 * 1024; // 8GB
        info.freeBytes = 4ULL * 1024 * 1024 * 1024;  // 4GB
        
        std::string result = info.toString();
        
        EXPECT_FALSE(result.empty()) << "toString should return non-empty string";
        EXPECT_NE(result.find("Total:"), std::string::npos) << "Should contain 'Total:'";
        EXPECT_NE(result.find("Free:"), std::string::npos) << "Should contain 'Free:'";
    }

    /**
     * @brief Test MemoryInfo structure assignment and member access.
     */
    TEST(MemoryInfoModuleTest, MemoryInfoStructureOperations) {
        MemoryInfo info1;
        info1.totalBytes = 16ULL * 1024 * 1024 * 1024;
        info1.freeBytes = 8ULL * 1024 * 1024 * 1024;
        
        // Test assignment
        MemoryInfo info2 = info1;
        EXPECT_EQ(info2.totalBytes, info1.totalBytes);
        EXPECT_EQ(info2.freeBytes, info1.freeBytes);
        
        // Test member access
        neko::uint64 total = info1.totalBytes;
        neko::uint64 free = info1.freeBytes;
        EXPECT_GT(total, free);
    }

    // ============= Platform Information Tests =============

    /**
     * @brief Test constexpr getOsName function.
     */
    TEST(PlatformModuleTest, GetOsNameConstexpr) {
        constexpr const char* os = getOsName();
        ASSERT_NE(os, nullptr);
        
        // Verify it matches osName constant
        EXPECT_STREQ(os, osName);
    }

    /**
     * @brief Test constexpr getOsArch function.
     */
    TEST(PlatformModuleTest, GetOsArchConstexpr) {
        constexpr const char* arch = getOsArch();
        ASSERT_NE(arch, nullptr);
        
        // Verify it matches osArch constant
        EXPECT_STREQ(arch, osArch);
    }

    /**
     * @brief Test platform and architecture consistency.
     */
    TEST(PlatformModuleTest, PlatformArchitectureConsistency) {
        // Verify getOsName() matches compile-time detection
        std::string osNameStr = getOsName();
        
#ifdef _WIN32
        EXPECT_EQ(osNameStr, "windows");
#elif defined(__APPLE__)
        EXPECT_EQ(osNameStr, "osx");
#elif defined(__linux__)
        EXPECT_EQ(osNameStr, "linux");
#endif

        // Verify getOsArch() returns a valid architecture
        std::string osArchStr = getOsArch();
        bool isValidArch = (osArchStr == "x64" || osArchStr == "x86" || 
                           osArchStr == "arm64" || osArchStr == "arm" || 
                           osArchStr == "unknown");
        EXPECT_TRUE(isValidArch);
    }

    // ============= Note on Runtime Function Tests =============
    //
    // Tests for runtime functions like getSystemMemoryInfo(), getOsVersion(),
    // workPath(), tempFolder(), and getHome() cannot be included due to MSVC
    // module implementation limitations.
    //
    // These functions are defined in module implementation units (.cpp files)
    // but fail to link properly when called from test code, resulting in LNK2019
    // errors. This appears to be a bug in MSVC's C++20 module support.
    //
    // The functions work correctly when used in non-module code (tested in
    // system_test.cpp) and when called from within the same module.
    //
    // Workaround: Full testing is provided via traditional header includes in
    // system_test.cpp until MSVC's module support matures.

} // namespace neko::system::tests::module
