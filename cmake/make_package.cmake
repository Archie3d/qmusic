# Make installation package
include(InstallRequiredSystemLibraries)

set(CPACK_NSIS_INSTALL_ROOT "$PROGRAMFILES64")
set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "QMusic")
set(CPACK_PACKAGE_VENDOR "Archie3d")
set(CPACK_PACKAGE_VERSION_MAJOR ${${PROJECT_NAME}_MAJOR_VERSION})
set(CPACK_PACKAGE_VERSION_MINOR ${${PROJECT_NAME}_MINOR_VERSION})
set(CPACK_PACKAGE_VERSION_PATCH ${${PROJECT_NAME}_PATCH_LEVEL})
set(CPACK_PACKAGE_EXECUTABLES "QMusic" "QMusic")

if(WIN32 AND NOT UNIX)
    set(CPACK_RESOURCE_FILE_LICENSE "${CMAKE_SOURCE_DIR}\\\\license.txt")
    set(CPACK_PACKAGE_ICON "${CMAKE_SOURCE_DIR}\\\\QMusic.bmp")
    set(CPACK_NSIS_INSTALLED_ICON_NAME "QMusic.exe")
    set(CPACK_NSIS_DISPLAY_NAME "${CPACK_PACKAGE_INSTALL_DIRECTORY} QMusic")
    set(CPACK_NSIS_HELP_LINK "http://github.com/Archie3d/qmusic")
    set(CPACK_NSIS_URL_INFO_ABOUT "http://github.com/Archie3d/qmusic")
    set(CPACK_NSIS_CONTACT "arthur.benilov@gmail.com")
    set(CPACK_NSIS_MODIFY_PATH ON)
    set(CPACK_CREATE_DESKTOP_LINKS "QMusic")
endif(WIN32 AND NOT UNIX)

include(CPack)

cpack_add_component(runtime
    DISPLAY_NAME "Main program files"
    DESCRIPTION "QMusic executables and dynamic libraries"
    GROUP "Application"
    INSTALL_TYPES Full
)

cpack_add_component(plugins
    DISPLAY_NAME "Plug-ins"
    DESCRIPTION "QMusic plug-ins"
    DEPENDS runtime
    GROUP "Application"
    INSTALL_TYPES Full
)

cpack_add_component(patches
    DISPLAY_NAME "Patches"
    DESCRIPTION "QMusic ready to use patches"
    DEPENDS runtime
    GROUP "Application"
    INSTALL_TYPES Full
)

set(CPACK_COMPONENT_GROUP_APPLICATION_DESCRIPTION
    "QMusic application and related components")
