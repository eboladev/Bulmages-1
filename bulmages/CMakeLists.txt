# Copyright (C) 2008 by Leopold Palomo-Avellaneda                       #
# leo@alaxarxa.net                                                      #
# http://www.iglues.org                                                 #
#                                                                       #
# This program is free software; you can redistribute it and/or modify  #
# it under the terms of the GNU General Public License as published by  #
# the Free Software Foundation; either version 2 of the License, or     #
# (at your option) any later version.                                   #
#                                                                       #
# This program is distributed in the hope that it will be useful,       #
# but WITHOUT ANY WARRANTY; without even the implied warranty of        #
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         #
# GNU General Public License for more details.                          #
#                                                                       #
# You should have received a copy of the GNU General Public License     #
# along with this program; if not, write to the                         #
# Free Software Foundation, Inc.,                                       #
# 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             #
#                                                                       #


# ########## Project setup ##########



PROJECT(bulmages)
CMAKE_MINIMUM_REQUIRED(VERSION 2.4.8)

SET( BULMAGES_VERSION 0.11.0 )
STRING( REGEX MATCHALL "[0-9]+" BULMAGES_VERSIONS ${BULMAGES_VERSION} )
LIST( GET BULMAGES_VERSIONS 0 BULMAGES_VERSION_MAJOR)
LIST( GET BULMAGES_VERSIONS 1 BULMAGES_VERSION_MINOR)
LIST( GET BULMAGES_VERSIONS 2 BULMAGES_VERSION_PATCH)


# ######### General setup ##########
INCLUDE_DIRECTORIES(${CMAKE_SOURCE_DIR})
find_package(Qt4 REQUIRED) 

if(NOT QT4_FOUND)
  message(FATAL_ERROR "Bulmages needs QT4 to compile")
endif(NOT QT4_FOUND)

###############################################3
# and now the version string given by qmake
STRING(REGEX REPLACE "^([0-9]+)\\.[0-9]+\\.[0-9]+.*" "\\1" QT_MAJOR_VERSION "${QTVERSION}")
STRING(REGEX REPLACE "^[0-9]+\\.([0-9])+\\.[0-9]+.*" "\\1" QT_MINOR_VERSION "${QTVERSION}")
STRING(REGEX REPLACE "^[0-9]+\\.[0-9]+\\.([0-9]+).*" "\\1" QT_PATCH_VERSION "${QTVERSION}")

if (${QT_MAJOR_VERSION}.${QT_MINOR_VERSION} LESS 4.3)
   message(FATAL_ERROR " Bulmages needs >= QT4.3 to compile")
endif (${QT_MAJOR_VERSION}.${QT_MINOR_VERSION} LESS 4.3) 


INCLUDE (${CMAKE_CURRENT_SOURCE_DIR}/ModulesCMake/FindPostgreSQL.cmake)
INCLUDE (${CMAKE_CURRENT_SOURCE_DIR}/ModulesCMake/FindGettext.cmake)

if(NOT PGSQL_FOUND)
  message(FATAL_ERROR "Bulmages needs libpq to compile")
endif(NOT PGSQL_FOUND)

set( BULMAGES_CONFIG_DIR /etc/bulmages CACHE PATH "The location of bulmages configuration dir")

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

########### bulmasetup ##########
message(STATUS "Working on bulmasetup")
add_subdirectory(bulmasetup)


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

message(STATUS "Working on documentation")
add_subdirectory(doc)

## We include cpack for others platforms differents than unix
SET(CPACK_PACKAGE_DESCRIPTION_SUMMARY "The BulmaGes Suite")
SET(CPACK_PACKAGE_VENDOR "Iglues")
SET(CPACK_PACKAGE_DESCRIPTION_FILE "${CMAKE_CURRENT_SOURCE_DIR}/README.txt")
SET(CPACK_RESOURCE_FILE_LICENSE "${CMAKE_CURRENT_SOURCE_DIR}/installbulmages/COPYING.txt")
SET(CPACK_PACKAGE_VERSION_MAJOR ${BULMAGES_VERSION_MAJOR})
SET(CPACK_PACKAGE_VERSION_MINOR ${BULMAGES_VERSION_MINOR})
SET(CPACK_PACKAGE_VERSION_PATCH ${BULMAGES_VERSION_PATCH})
SET(CPACK_PACKAGE_INSTALL_DIRECTORY "CMake ${CMAKE_VERSION_MAJOR}.${CMAKE_VERSION_MINOR}")
IF(WIN32 AND NOT UNIX)
  # There is a bug in NSI that does not handle full unix paths properly. Make
  # sure there is at least one set of four (4) backlasshes.
  SET(CPACK_PACKAGE_ICON "${CMAKE_SOURCE_DIR}/installbulmages/icons\\\\iconbulmacont.xpm")
  SET(CPACK_NSIS_INSTALLED_ICON_NAME "bin\\\\bulmacont")
  SET(CPACK_NSIS_DISPLAY_NAME "${CPACK_PACKAGE_INSTALL_DIRECTORY} The BulmaGes Suite")
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

#if(BULMAGES_CONFIG_DIR STREQUAL "")
#   set (BULMAGES_CONFIG_DIR /etc/bulmages)
#endif(BULMAGES_CONFIG_DIR STREQUAL "")

MARK_AS_ADVANCED(
	BULMAGES_VERSION
)

 
