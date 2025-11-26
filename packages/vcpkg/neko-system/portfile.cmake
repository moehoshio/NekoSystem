vcpkg_from_github(
    OUT_SOURCE_PATH SOURCE_PATH
    REPO moehoshio/NekoSystem
    REF v1.0.0
    SHA512 0ef9134cd1db3eabfb9e09d5e9d4afca7ee1098e9efd6c49b3171ef9d3d1f58a5d88c3420fab6df4e859dfd113b418d97935922d6dd4e7aeb8a0d129fcec3a0b
    HEAD_REF main
)

vcpkg_cmake_configure(
    SOURCE_PATH "${SOURCE_PATH}"
    OPTIONS
        ${FEATURE_OPTIONS}
        -DNEKO_SYSTEM_BUILD_TESTS=OFF
        -DNEKO_SYSTEM_AUTO_FETCH_DEPS=OFF
        -DNEKO_SYSTEM_ENABLE_MODULE=OFF
)

vcpkg_cmake_install()
vcpkg_cmake_config_fixup(CONFIG_PATH lib/cmake/NekoSystem PACKAGE_NAME nekosystem)

vcpkg_install_copyright(FILE_LIST "${SOURCE_PATH}/LICENSE")

file(REMOVE_RECURSE "${CURRENT_PACKAGES_DIR}/debug/include")
file(INSTALL "${CMAKE_CURRENT_LIST_DIR}/usage" DESTINATION "${CURRENT_PACKAGES_DIR}/share/${PORT}")
