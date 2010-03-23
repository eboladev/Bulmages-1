# 
# FindPostgresSQL.cmake
# 
# Find the native PostgreSQL includes and library (libpq)
# Defines:
#  PGSQL_INCLUDE_DIR - where to find libpq-fe.h, etc.
#  PGSQL_LIBRARIES   - list of libraries when using PGSQL.
#  PGSQL_BIN_DIR     - where are the pg utils found
#  PGSQL_FOUND       - True if PGSQL found.
# from the pvpgn GPL license http://pvpgn.berlios.de/
# ${PGSQL_LIBRARIES})
#
# Modified and adapted by Leopold Palomo-Avellaneda 
#
# Copyright (C) 2008                                                    #
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

FIND_PROGRAM(CMAKE_PGCONFIG_EXECUTABLE pg_config)
########################################

IF(CMAKE_PGCONFIG_EXECUTABLE)
	# MESSAGE("DEBUG: pg_config executable found")

	# set PGSQL_INCLUDE_DIR
    EXEC_PROGRAM(${CMAKE_PGCONFIG_EXECUTABLE}
    	ARGS "--includedir"
        OUTPUT_VARIABLE PGSQL_INCLUDE_DIR)

      
    # set PGSQL_BIN_DIR
    EXEC_PROGRAM(${CMAKE_PGCONFIG_EXECUTABLE}
        ARGS "--bindir"
        OUTPUT_VARIABLE PGSQL_BIN_DIR)
    
	# set PGSQL_LIBRARIES_DIR
    EXEC_PROGRAM(${CMAKE_PGCONFIG_EXECUTABLE}
        ARGS "--libdir"
        OUTPUT_VARIABLE PGSQL_LIBRARIES_DIR)

	SET(PGSQL_NAMES pq)
	FIND_LIBRARY(PGSQL_LIBRARY
  		NAMES ${PGSQL_NAMES}
  		PATHS ${PGSQL_LIBRARIES_DIR})

ELSE(CMAKE_PGCONFIG_EXECUTABLE)

# 	IF (PGSQL_INCLUDE_DIR)
# 	# Already in cache, be silent
# 	SET(PostgreSQL_FIND_QUIETLY TRUE)
# 	ENDIF (PGSQL_INCLUDE_DIR)
	
	# the registry settings checked are in order:
	# - for pgInstaller 8.2.x postgresql version
	SET(PGSQL_WIN_BASE "[HKEY_LOCAL_MACHINE\\SOFTWARE\\PostgreSQL\\Installations\\postgresql-8.4;Base Directory]")
	IF(PGSQL_WIN_BASE)
		IF(MSVC)
			SET(PGSQL_SEARCH_LIB_PATHS "${PGSQL_WIN_BASE}/lib/ms")
		ELSE(MSVC)
			SET(PGSQL_SEARCH_LIB_PATHS "${PGSQL_WIN_BASE}/lib")
		ENDIF(MSVC)
	ENDIF(PGSQL_WIN_BASE)
	
	FIND_PATH(PGSQL_INCLUDE_DIR libpq-fe.h
	"[HKEY_LOCAL_MACHINE\\SOFTWARE\\PostgreSQL\\Installations\\postgresql-8.4;Base Directory]/include"
	/usr/local/pgsql/include
	/usr/local/include/pgsql
	/usr/local/include
	/usr/include/pgsql
	/usr/include
	/usr/pgsql/include
	/usr/include/postgresql
	/opt/local/include/postgresql82
	)
	
	SET(PGSQL_NAMES pq)
	SET(PGSQL_SEARCH_LIB_PATHS 
	${PGSQL_SEARCH_LIB_PATHS}
	/usr/local/pgsql/lib /usr/local/lib /usr/lib /opt/local/lib/postgresql82
	)
	FIND_LIBRARY(PGSQL_LIBRARY
	NAMES ${PGSQL_NAMES}
	PATHS ${PGSQL_SEARCH_LIB_PATHS}
	)

ENDIF(CMAKE_PGCONFIG_EXECUTABLE)

IF (PGSQL_INCLUDE_DIR AND PGSQL_LIBRARY)
  SET(PGSQL_FOUND TRUE)
  SET( PGSQL_LIBRARIES ${PGSQL_LIBRARY} )
ELSE (PGSQL_INCLUDE_DIR AND PGSQL_LIBRARY)
  SET(PGSQL_FOUND FALSE)
  SET( PGSQL_LIBRARIES )
ENDIF (PGSQL_INCLUDE_DIR AND PGSQL_LIBRARY)

IF (PGSQL_FOUND)
	MESSAGE(STATUS "Found PostgreSQL pg_config: ${CMAKE_PGCONFIG_EXECUTABLE}")
	MESSAGE(STATUS "Found PostgreSQL includedir: ${PGSQL_INCLUDE_DIR}")
	MESSAGE(STATUS "Found PostgreSQL library: ${PGSQL_LIBRARY}")
	MESSAGE(STATUS "Found PostgreSQL binary utils dir: ${PGSQL_BIN_DIR}")
ELSE (PGSQL_FOUND)
	IF (PostgreSQL_FIND_REQUIRED)
		MESSAGE(STATUS "Looked for PostgreSQL libraries named ${PGSQL_NAMES}.")
		MESSAGE(FATAL_ERROR "Could NOT find PostgreSQL library")
	ENDIF (PostgreSQL_FIND_REQUIRED)
ENDIF (PGSQL_FOUND)


MARK_AS_ADVANCED(
  	PGSQL_LIBRARY
	PGSQL_LIBRARIES
  	PGSQL_INCLUDE_DIR
	PGSQL_BIN_DIR
	CMAKE_PGCONFIG_EXECUTABLE
)

