# ########## Project setup ##########
PROJECT(bulmages)
CMAKE_MINIMUM_REQUIRED(VERSION 2.4.5)

SET( BULMAGES_VERSION 0.11.0 )
STRING( REGEX MATCHALL "[0-9]+" BULMAGES_VERSIONS ${BULMAGES_VERSION} )
LIST( GET BULMAGES_VERSIONS 0 BULMAGES_VERSION_MAJOR)
LIST( GET BULMAGES_VERSIONS 1 BULMAGES_VERSION_MINOR)
LIST( GET BULMAGES_VERSIONS 2 BULMAGES_VERSION_PATCH)


# ######### General setup ##########
INCLUDE_DIRECTORIES(${CMAKE_SOURCE_DIR})
find_package(Qt4 REQUIRED) 
INCLUDE (${CMAKE_CURRENT_SOURCE_DIR}/ModulesCMake/FindPostgreSQL.cmake)

if(NOT PGSQL_FOUND)
  message(ERROR "Bulmages needs libpq to compile")
endif(NOT PGSQL_FOUND)

########### bulmages library ##########
message(STATUS "Working on bulmalib")
add_subdirectory(bulmalib)

########### bulmacont ##########
message(STATUS "Working on bulmacont")
add_subdirectory(bulmacont)

########### bulmafact ##########
message(STATUS "Working on bulmafact")
add_subdirectory(bulmafact)

########### bulmages launch ##########
message(STATUS "Working on bulmages")
add_subdirectory(bulmages)

########### bulmatpv ##########
message(STATUS "Working on bulmatpv")
add_subdirectory(bulmatpv)

########### private code ########
if(EXISTS "${CMAKE_SOURCE_DIR}/CMakeLists.txt.private")
   message(STATUS "Private code inserted")
   include("${CMAKE_SOURCE_DIR}/CMakeLists.txt.private")
endif(EXISTS "${CMAKE_SOURCE_DIR}/CMakeLists.txt.private")


if(APPLE)
    message(STATUS "Compiling for OSX")
    set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} -undefined dynamic_lookup")
endif(APPLE)

########### some stuff ##########
message(STATUS "Working on installbulmages")
add_subdirectory(installbulmages)


## We include cpack for others platforms differents than unix
SET(CPACK_PACKAGE_DESCRIPTION_SUMMARY "The Bulmagés Suite")
SET(CPACK_PACKAGE_VENDOR "Iglues")
SET(CPACK_PACKAGE_DESCRIPTION_FILE "${CMAKE_CURRENT_SOURCE_DIR}/README")
SET(CPACK_RESOURCE_FILE_LICENSE "${CMAKE_CURRENT_SOURCE_DIR}/installbulmages/COPYING")
SET(CPACK_PACKAGE_VERSION_MAJOR "1")
SET(CPACK_PACKAGE_VERSION_MINOR "3")
SET(CPACK_PACKAGE_VERSION_PATCH "2")
SET(CPACK_PACKAGE_INSTALL_DIRECTORY "CMake ${CMake_VERSION_MAJOR}.${CMake_VERSION_MINOR}")
IF(WIN32 AND NOT UNIX)
  # There is a bug in NSI that does not handle full unix paths properly. Make
  # sure there is at least one set of four (4) backlasshes.
  SET(CPACK_PACKAGE_ICON "${CMake_SOURCE_DIR}/installbulmages/icons\\\\iconbulmacont.xpm")
  SET(CPACK_NSIS_INSTALLED_ICON_NAME "bin\\\\bulmacont")
  SET(CPACK_NSIS_DISPLAY_NAME "${CPACK_PACKAGE_INSTALL_DIRECTORY} The Bulmagés Suite")
  SET(CPACK_NSIS_HELP_LINK "http:\\\\\\\\developer.berlios.de/projects/bulmages")
  SET(CPACK_NSIS_URL_INFO_ABOUT "http:\\\\\\\\developer.berlios.de/projects/bulmages")
  SET(CPACK_NSIS_CONTACT "bulmages@bulma.net")
  SET(CPACK_NSIS_MODIFY_PATH ON)
ELSE(WIN32 AND NOT UNIX)
  SET(CPACK_STRIP_FILES "bin/bulmacont" "bin/bulmafact" "bin/bulmatpv")
  SET(CPACK_SOURCE_STRIP_FILES "")
ENDIF(WIN32 AND NOT UNIX)
SET(CPACK_PACKAGE_EXECUTABLES "bulmacont" "bulmafact" "bulmatpv")


INCLUDE(CPack)


# 	SET(CPACK_CMAKE_GENERATOR "Unix Makefiles")
# 	SET(CPACK_GENERATOR "PackageMaker;STGZ;TGZ")
# 	SET(CPACK_INSTALL_CMAKE_PROJECTS "/Users/Shared/bulmages/trunk/bulmages/build;local/bulmages;ALL;/")
# 	SET(CPACK_MODULE_PATH "")
# 	SET(CPACK_NSIS_DISPLAY_NAME "bulmages 0.11.0")
# 	SET(CPACK_OUTPUT_CONFIG_FILE "/Users/Shared/bulmages/trunk/bulmages/build/CPackConfig.cmake")
# 	SET(CPACK_PACKAGE_DESCRIPTION_FILE "/opt/local/share/cmake-2.4/Templates/CPack.GenericDescription.txt")
# 	SET(CPACK_PACKAGE_DESCRIPTION_SUMMARY "bulmages built using CMake")
# 	SET(CPACK_PACKAGE_FILE_NAME "bulmages-0.11.0-Darwin")
# 	SET(CPACK_PACKAGE_INSTALL_DIRECTORY "bulmages 0.11.0")
# 	SET(CPACK_PACKAGE_INSTALL_REGISTRY_KEY "bulmages 0.11.0")
# 	SET(CPACK_PACKAGE_NAME "bulmages")
# 	SET(CPACK_PACKAGE_VENDOR "Humanity")
# 	SET(CPACK_PACKAGE_VERSION "0.11.0")
# 	SET(CPACK_PACKAGE_VERSION_MAJOR "0")
# 	SET(CPACK_PACKAGE_VERSION_MINOR "11")
# 	SET(CPACK_PACKAGE_VERSION_PATCH "0")
# 	SET(CPACK_RESOURCE_FILE_LICENSE "/opt/local/share/cmake-2.4/Templates/CPack.GenericLicense.txt")
# 	SET(CPACK_RESOURCE_FILE_README "/opt/local/share/cmake-2.4/Templates/CPack.GenericDescription.txt")
# 	SET(CPACK_RESOURCE_FILE_WELCOME "/opt/local/share/cmake-2.4/Templates/CPack.GenericWelcome.txt")
# 	SET(CPACK_SOURCE_GENERATOR "TGZ;TZ")
# 	SET(CPACK_SOURCE_OUTPUT_CONFIG_FILE "/Users/Shared/bulmages/trunk/bulmages/build/CPackSourceConfig.cmake")
# 	SET(CPACK_SYSTEM_NAME "Darwin")
# 	SET(CPACK_TOPLEVEL_TAG "Darwin")

