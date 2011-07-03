# (c) 2008 by BogDan Vatra < bogdan@licentia.eu >

cmake_minimum_required(VERSION 2.6)
project(quazip-package)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

#set(CMAKE_VERBOSE_MAKEFILE ON)
#comment or remove the above line before release

set (QUAZIP_VERSION_MAJOR 1)
set (QUAZIP_VERSION_MINOR 0)
set (QUAZIP_VERSION_RELEASE 0)
set (QUAZIP_VERSION "${QUAZIP_VERSION_MAJOR}.${QUAZIP_VERSION_MINOR}.${QUAZIP_VERSION_RELEASE}" )

add_definitions (-DQUAZIP_VERSION=\"${QUAZIP_VERSION}\")

set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake/modules" )

find_package(Qt4 REQUIRED)
find_package(ZLIB REQUIRED)

include( ${QT_USE_FILE} )
include_directories(
	${ZLIB_INCLUDE_DIR}
	${CMAKE_CURRENT_SOURCE_DIR} 
	${QT_INCLUDE_DIR}
	${CMAKE_CURRENT_BINARY_DIR}
)

add_subdirectory(quazip)

CONFIGURE_FILE(
  "${CMAKE_CURRENT_SOURCE_DIR}/cmake_uninstall.cmake.in"
  "${CMAKE_CURRENT_BINARY_DIR}/cmake_uninstall.cmake"
  IMMEDIATE @ONLY)

ADD_CUSTOM_TARGET(uninstall
  "${CMAKE_COMMAND}" -P "${CMAKE_CURRENT_BINARY_DIR}/cmake_uninstall.cmake")

install(FILES cmake/modules/FindQuazip.cmake DESTINATION ${CMAKE_ROOT}/Modules COMPONENT Devel)
