# Install script for directory: /home/tborras/exaro/exaro/libs/report

# Set the install prefix
IF(NOT DEFINED CMAKE_INSTALL_PREFIX)
  SET(CMAKE_INSTALL_PREFIX "/usr/local")
ENDIF(NOT DEFINED CMAKE_INSTALL_PREFIX)
STRING(REGEX REPLACE "/$" "" CMAKE_INSTALL_PREFIX "${CMAKE_INSTALL_PREFIX}")

# Set the install configuration name.
IF(NOT DEFINED CMAKE_INSTALL_CONFIG_NAME)
  IF(BUILD_TYPE)
    STRING(REGEX REPLACE "^[^A-Za-z0-9_]+" ""
           CMAKE_INSTALL_CONFIG_NAME "${BUILD_TYPE}")
  ELSE(BUILD_TYPE)
    SET(CMAKE_INSTALL_CONFIG_NAME "RelWithDebInfo")
  ENDIF(BUILD_TYPE)
  MESSAGE(STATUS "Install configuration: \"${CMAKE_INSTALL_CONFIG_NAME}\"")
ENDIF(NOT DEFINED CMAKE_INSTALL_CONFIG_NAME)

# Set the component getting installed.
IF(NOT CMAKE_INSTALL_COMPONENT)
  IF(COMPONENT)
    MESSAGE(STATUS "Install component: \"${COMPONENT}\"")
    SET(CMAKE_INSTALL_COMPONENT "${COMPONENT}")
  ELSE(COMPONENT)
    SET(CMAKE_INSTALL_COMPONENT)
  ENDIF(COMPONENT)
ENDIF(NOT CMAKE_INSTALL_COMPONENT)

# Install shared libraries without execute permission?
IF(NOT DEFINED CMAKE_INSTALL_SO_NO_EXE)
  SET(CMAKE_INSTALL_SO_NO_EXE "1")
ENDIF(NOT DEFINED CMAKE_INSTALL_SO_NO_EXE)

IF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  list(APPEND CPACK_ABSOLUTE_DESTINATION_FILES
   "/usr/local/lib/libReport.so.2.0.0;/usr/local/lib/libReport.so.2.0;/usr/local/lib/libReport.so")
FILE(INSTALL DESTINATION "/usr/local/lib" TYPE SHARED_LIBRARY FILES
    "/home/tborras/exaro/exaro/build/libs/report/libReport.so.2.0.0"
    "/home/tborras/exaro/exaro/build/libs/report/libReport.so.2.0"
    "/home/tborras/exaro/exaro/build/libs/report/libReport.so"
    )
  FOREACH(file
      "$ENV{DESTDIR}/usr/local/lib/libReport.so.2.0.0"
      "$ENV{DESTDIR}/usr/local/lib/libReport.so.2.0"
      "$ENV{DESTDIR}/usr/local/lib/libReport.so"
      )
    IF(EXISTS "${file}" AND
       NOT IS_SYMLINK "${file}")
      IF(CMAKE_INSTALL_DO_STRIP)
        EXECUTE_PROCESS(COMMAND "/usr/bin/strip" "${file}")
      ENDIF(CMAKE_INSTALL_DO_STRIP)
    ENDIF()
  ENDFOREACH()
ENDIF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")

IF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Devel")
  list(APPEND CPACK_ABSOLUTE_DESTINATION_FILES
   "/usr/local/include/bandinterface.h;/usr/local/include/exportinterface.h;/usr/local/include/iteminterface.h;/usr/local/include/pageinterface.h;/usr/local/include/reportinterface.h;/usr/local/include/reportengine.h;/usr/local/include/exaroexports.h;/usr/local/include/globals.h;/usr/local/include/sqlquery.h")
FILE(INSTALL DESTINATION "/usr/local/include" TYPE FILE FILES
    "/home/tborras/exaro/exaro/libs/report/bandinterface.h"
    "/home/tborras/exaro/exaro/libs/report/exportinterface.h"
    "/home/tborras/exaro/exaro/libs/report/iteminterface.h"
    "/home/tborras/exaro/exaro/libs/report/pageinterface.h"
    "/home/tborras/exaro/exaro/libs/report/reportinterface.h"
    "/home/tborras/exaro/exaro/libs/report/reportengine.h"
    "/home/tborras/exaro/exaro/libs/report/exaroexports.h"
    "/home/tborras/exaro/exaro/libs/report/globals.h"
    "/home/tborras/exaro/exaro/libs/report/sqlquery.h"
    )
ENDIF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Devel")

