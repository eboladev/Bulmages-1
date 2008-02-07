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
SET(CPACK_PACKAGE_DESCRIPTION_SUMMARY "The BulmaGés Suite")
SET(CPACK_PACKAGE_VENDOR "Iglues")
SET(CPACK_PACKAGE_DESCRIPTION_FILE "${CMAKE_CURRENT_SOURCE_DIR}/README.txt")
SET(CPACK_RESOURCE_FILE_LICENSE "${CMAKE_CURRENT_SOURCE_DIR}/installbulmages/COPYING.txt")
SET(CPACK_PACKAGE_VERSION_MAJOR "0")
SET(CPACK_PACKAGE_VERSION_MINOR "11")
SET(CPACK_PACKAGE_VERSION_PATCH "1")
SET(CPACK_PACKAGE_INSTALL_DIRECTORY "CMake ${CMake_VERSION_MAJOR}.${CMake_VERSION_MINOR}")
IF(WIN32 AND NOT UNIX)
  # There is a bug in NSI that does not handle full unix paths properly. Make
  # sure there is at least one set of four (4) backlasshes.
  SET(CPACK_PACKAGE_ICON "${CMake_SOURCE_DIR}/installbulmages/icons\\\\iconbulmacont.xpm")
  SET(CPACK_NSIS_INSTALLED_ICON_NAME "bin\\\\bulmacont")
  SET(CPACK_NSIS_DISPLAY_NAME "${CPACK_PACKAGE_INSTALL_DIRECTORY} The BulmaGés Suite")
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
set( BULMAGES_CONFIG_DIR /etc/bulmages CACHE PATH "The location of bulmages configuration dir")

MARK_AS_ADVANCED(
	BULMAGES_VERSION
)
 
