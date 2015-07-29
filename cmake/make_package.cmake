# Make installation package
include(InstallRequiredSystemLibraries)

set(CPACK_NSIS_INSTALL_ROOT "$PROGRAMFILES64")
set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "Idylla Studio")
set(CPACK_PACKAGE_VENDOR "Biocartis")
set(CPACK_PACKAGE_VERSION_MAJOR ${${PROJECT_NAME}_MAJOR_VERSION})
set(CPACK_PACKAGE_VERSION_MINOR ${${PROJECT_NAME}_MINOR_VERSION})
set(CPACK_PACKAGE_VERSION_PATCH ${${PROJECT_NAME}_PATCH_LEVEL})
set(CPACK_PACKAGE_EXECUTABLES "IdyllaStudio" "Idylla Studio")

if(WIN32 AND NOT UNIX)
    set(CPACK_RESOURCE_FILE_LICENSE "${CMAKE_SOURCE_DIR}\\\\license.txt")
    set(CPACK_PACKAGE_ICON "${CMAKE_SOURCE_DIR}\\\\IdyllaStudio.bmp")
    set(CPACK_NSIS_INSTALLED_ICON_NAME "IdyllaStudio.exe")
    set(CPACK_NSIS_DISPLAY_NAME "${CPACK_PACKAGE_INSTALL_DIRECTORY} Idylla Studio")
    set(CPACK_NSIS_HELP_LINK "http:\\\\\\\\www.biocartis.com")
    set(CPACK_NSIS_URL_INFO_ABOUT "http:\\\\\\\\www.biocartis.com")
    set(CPACK_NSIS_CONTACT "info@biocartis.com")
    set(CPACK_NSIS_MODIFY_PATH ON)
    set(CPACK_CREATE_DESKTOP_LINKS "IdyllaStudio")
endif(WIN32 AND NOT UNIX)

include(CPack)

cpack_add_component(runtime
    DISPLAY_NAME "Main program files"
    DESCRIPTION "Idylla Studio executables and dynamic libraries"
    GROUP "Application"
    INSTALL_TYPES Full
)

cpack_add_component(library
    DISPLAY_NAME "Components library"
    DESCRIPTION "Idylla Studio components library"
    DEPENDS runtime
    GROUP "Application"
    INSTALL_TYPES Full
)

cpack_add_component(plugins
    DISPLAY_NAME "Plug-ins"
    DESCRIPTION "Idylla Studio plug-ins"
    DEPENDS runtime
    GROUP "Application"
    INSTALL_TYPES Full
)

cpack_add_component(help
    DISPLAY_NAME "Help pages"
    DESCRIPTION "Idylla Studio help pages"
    DEPENDS runtime
    GROUP "Application"
    INSTALL_TYPES Full
)

set(CPACK_COMPONENT_GROUP_APPLICATION_DESCRIPTION
    "Idylla Studio application and related components")
