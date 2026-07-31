vcpkg_from_github(
    OUT_SOURCE_PATH SOURCE_PATH
    REPO hoshimoe/NekoSystem
    REF v1.0.3
    SHA512 0b3fcbff8238f3a19be3d08345d2097757213ab2ccb66c3325a7fa342271a282a43ef1ee51fb0222da411f886a6f7523fc76057685ae8dee330e5522fa7c4e6a
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
