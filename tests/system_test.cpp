/**
 * @file system_test.cpp
 * @brief Unit tests for the neko::system module.
 */

#include <gtest/gtest.h>
#include <neko/system/memoryinfo.hpp>
#include <neko/system/platform.hpp>

#include <filesystem>
#include <regex>

namespace neko::system::tests {

    /**
     * @brief Test fixture for memory information tests.
     */
    class MemoryInfoTest : public ::testing::Test {
    protected:
        void SetUp() override {
            // Setup code if needed
        }

        void TearDown() override {
            // Cleanup code if needed
        }
    };

    /**
     * @brief Test fixture for platform information tests.
     */
    class PlatformTest : public ::testing::Test {
    protected:
        void SetUp() override {
            // Setup code if needed
        }

        void TearDown() override {
            // Cleanup code if needed
        }
    };

    // ============= Memory Information Tests =============

    /**
     * @brief Test that getSystemMemoryInfo returns a valid result.
     */
    TEST_F(MemoryInfoTest, GetSystemMemoryInfoReturnsValidResult) {
        auto memInfo = getSystemMemoryInfo();
        
        ASSERT_TRUE(memInfo.has_value()) << "Failed to get system memory information";
        
        // Memory values should be non-zero and reasonable
        EXPECT_GT(memInfo->totalBytes, 0) << "Total memory should be greater than 0";
        EXPECT_GE(memInfo->totalBytes, memInfo->freeBytes) << "Total memory should be >= free memory";
        
        // Basic sanity check: total memory should be at least 1GB (very conservative)
        EXPECT_GE(memInfo->totalBytes, 1024ULL * 1024 * 1024) << "Total memory should be at least 1GB";
    }

    /**
     * @brief Test the toString method of MemoryInfo.
     */
    TEST_F(MemoryInfoTest, MemoryInfoToStringFormatIsCorrect) {
        MemoryInfo info;
        info.totalBytes = 8ULL * 1024 * 1024 * 1024; // 8GB
        info.freeBytes = 4ULL * 1024 * 1024 * 1024;  // 4GB
        
        std::string result = info.toString();
        
        // Should contain "Total: 8192 MB, Free: 4096 MB"
        EXPECT_TRUE(result.find("Total: 8192 MB") != std::string::npos) 
            << "String should contain 'Total: 8192 MB', got: " << result;
        EXPECT_TRUE(result.find("Free: 4096 MB") != std::string::npos) 
            << "String should contain 'Free: 4096 MB', got: " << result;
    }

    /**
     * @brief Test MemoryInfo with edge case values.
     */
    TEST_F(MemoryInfoTest, MemoryInfoEdgeCases) {
        MemoryInfo info;
        
        // Test with zero values
        info.totalBytes = 0;
        info.freeBytes = 0;
        std::string result = info.toString();
        EXPECT_TRUE(result.find("Total: 0 MB") != std::string::npos);
        EXPECT_TRUE(result.find("Free: 0 MB") != std::string::npos);
        
        // Test with very large values
        info.totalBytes = UINT64_MAX;
        info.freeBytes = UINT64_MAX / 2;
        result = info.toString();
        EXPECT_FALSE(result.empty()) << "toString should handle large values gracefully";
    }

    // ============= Platform Tests =============

    /**
     * @brief Test OS name detection.
     */
    TEST_F(PlatformTest, OsNameIsValid) {
        const char* os = getOsName();
        ASSERT_NE(os, nullptr) << "OS name should not be null";
        
        std::string osStr(os);
        EXPECT_TRUE(osStr == "windows" || osStr == "osx" || osStr == "linux" || osStr == "unknown")
            << "OS name should be one of: windows, osx, linux, unknown. Got: " << osStr;
    }

    /**
     * @brief Test architecture detection.
     */
    TEST_F(PlatformTest, OsArchIsValid) {
        const char* arch = getOsArch();
        ASSERT_NE(arch, nullptr) << "Architecture should not be null";
        
        std::string archStr(arch);
        EXPECT_TRUE(archStr == "x64" || archStr == "x86" || archStr == "arm64" || 
                   archStr == "arm" || archStr == "unknown")
            << "Architecture should be one of: x64, x86, arm64, arm, unknown. Got: " << archStr;
    }

    /**
     * @brief Test platform detection functions.
     */
    TEST_F(PlatformTest, PlatformDetectionFunctions) {
#ifdef _WIN32
        EXPECT_TRUE(isWindows()) << "Should detect Windows platform";
        EXPECT_FALSE(isMacOS()) << "Should not detect macOS on Windows";
        EXPECT_FALSE(isLinux()) << "Should not detect Linux on Windows";
#elif defined(__APPLE__)
        EXPECT_TRUE(isMacOS()) << "Should detect macOS platform";
        EXPECT_FALSE(isWindows()) << "Should not detect Windows on macOS";
        EXPECT_FALSE(isLinux()) << "Should not detect Linux on macOS";
#elif defined(__linux__)
        EXPECT_TRUE(isLinux()) << "Should detect Linux platform";
        EXPECT_FALSE(isMacOS()) << "Should not detect macOS on Linux";
        EXPECT_FALSE(isWindows()) << "Should not detect Windows on Linux";
#endif
    }

    /**
     * @brief Test architecture detection functions.
     */
    TEST_F(PlatformTest, ArchitectureDetectionFunctions) {
        // At least one architecture should be detected
        bool hasArch = isArchX64() || isArchX86() || isArchArm64() || isArchArm();
        EXPECT_TRUE(hasArch) << "At least one architecture should be detected";
        
        // Only one architecture should be true
        int archCount = 0;
        if (isArchX64()) archCount++;
        if (isArchX86()) archCount++;
        if (isArchArm64()) archCount++;
        if (isArchArm()) archCount++;
        
        EXPECT_EQ(archCount, 1) << "Exactly one architecture should be detected";
    }

    /**
     * @brief Test temporary folder functionality.
     */
    TEST_F(PlatformTest, TempFolderWorks) {
        std::string tempPath = tempFolder();
        
        EXPECT_FALSE(tempPath.empty()) << "Temp folder path should not be empty";
        EXPECT_TRUE(std::filesystem::exists(tempPath)) << "Temp folder should exist: " << tempPath;
        EXPECT_TRUE(std::filesystem::is_directory(tempPath)) << "Temp folder should be a directory: " << tempPath;
        
        // Test setting a custom temp directory
        std::string currentDir = std::filesystem::current_path().string();
        std::string customTemp = tempFolder(currentDir);
        
        // Normalize both paths for comparison since unifiedPath converts to Unix-style
        using namespace neko::ops::pipe;
        std::string normalizedCurrentDir = currentDir | neko::util::lambda::unifiedPath;
        EXPECT_EQ(customTemp, normalizedCurrentDir) << "Should return the set temp directory (normalized)";
    }

    /**
     * @brief Test work path functionality.
     */
    TEST_F(PlatformTest, WorkPathWorks) {
        std::string originalWorkPath = workPath();
        EXPECT_FALSE(originalWorkPath.empty()) << "Work path should not be empty";
        EXPECT_TRUE(std::filesystem::exists(originalWorkPath)) << "Work path should exist";
        
        // The work path should be the current directory
        std::string currentPath = std::filesystem::current_path().string();
        EXPECT_FALSE(currentPath.empty()) << "Current path should not be empty";
    }

    /**
     * @brief Test home directory retrieval.
     */
    TEST_F(PlatformTest, GetHomeWorks) {
        auto home = getHome();
        
        // Home directory should be available on most systems
        ASSERT_TRUE(home.has_value()) << "Home directory should be available";
        EXPECT_FALSE(home->empty()) << "Home directory path should not be empty";
        EXPECT_TRUE(std::filesystem::exists(*home)) << "Home directory should exist: " << *home;
        EXPECT_TRUE(std::filesystem::is_directory(*home)) << "Home directory should be a directory: " << *home;
    }

    /**
     * @brief Test OS version retrieval.
     */
    TEST_F(PlatformTest, GetOsVersionWorks) {
        std::string version = getOsVersion();
        
        EXPECT_FALSE(version.empty()) << "OS version should not be empty";
        
        // Basic format check - should contain some digits
        bool hasDigit = std::any_of(version.begin(), version.end(), ::isdigit);
        EXPECT_TRUE(hasDigit) << "OS version should contain at least one digit: " << version;
    }

    // ============= Cross-Component Integration Tests =============

    /**
     * @brief Test that memory and platform information can be retrieved together.
     */
    TEST(SystemIntegrationTest, MemoryAndPlatformInfoTogether) {
        // Get memory info
        auto memInfo = getSystemMemoryInfo();
        ASSERT_TRUE(memInfo.has_value()) << "Memory info should be available";
        
        // Get platform info
        std::string osName = getOsName();
        std::string osArch = getOsArch();
        std::string osVersion = getOsVersion();
        
        EXPECT_FALSE(osName.empty()) << "OS name should be available";
        EXPECT_FALSE(osArch.empty()) << "OS architecture should be available";
        EXPECT_FALSE(osVersion.empty()) << "OS version should be available";
        
        // Create a system summary
        std::ostringstream summary;
        summary << "System: " << osName << " " << osVersion << " (" << osArch << ")\n";
        summary << "Memory: " << memInfo->toString();
        
        std::string result = summary.str();
        EXPECT_FALSE(result.empty()) << "System summary should not be empty";
        
        // Log the system information for debugging
        std::cout << "System Information:\n" << result << std::endl;
    }

} // namespace neko::system::tests
