cmake_minimum_required(VERSION 3.20)
project(yamj LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_executable(${PROJECT_NAME} WIN32 src/main.cpp src/yamj.rc)

# Link Windows libraries
target_link_libraries(${PROJECT_NAME} PRIVATE user32 shell32 comctl32)

# CPack installer configuration
include(InstallRequiredSystemLibraries)
set(CPACK_RESOURCE_FILE_LICENSE "${CMAKE_CURRENT_SOURCE_DIR}/LICENSE")
set(CPACK_PACKAGE_VERSION_MAJOR "1")
set(CPACK_PACKAGE_VERSION_MINOR "0")
set(CPACK_PACKAGE_VERSION_PATCH "0")
set(CPACK_GENERATOR "NSIS;ZIP")
set(CPACK_PACKAGE_NAME "YAMJ")
# set(CPACK_PACKAGE_VENDOR "Costantino Pipero")
# set(CPACK_PACKAGE_CONTACT "your@email.com")
set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "Yet Another Mouse Jiggler")
set(CPACK_NSIS_DISPLAY_NAME "YAMJ - Yet Another Mouse Jiggler")
set(CPACK_NSIS_MUI_ICON "")
set(CPACK_NSIS_INSTALLED_ICON_NAME "yamj.exe")
set(CPACK_NSIS_HELP_LINK "https://github.com/piperoc/yamj")
set(CPACK_NSIS_URL_INFO_ABOUT "https://github.com/piperoc/yamj")
# set(CPACK_NSIS_CONTACT "your@email.com")
install(TARGETS ${PROJECT_NAME} DESTINATION .)
include(CPack)
