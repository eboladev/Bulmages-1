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
#                                                                       #
#########################################################################
# bulmafact                                                             #
#########################################################################


file(GLOB bulmafact_SRC *.c *.cpp)
file(GLOB bulmafact_MOCS *.hpp *.h)
file(GLOB bulmafact_UIS *.ui)

set(bulmafactEx main.cxx)

set(QT_USE_QTASSISTANT true)
set(QT_USE_QTXML true)
set(QT_USE_QTDBUS true)

INCLUDE(${QT_USE_FILE})

include_directories(
    ${QT_INCLUDE_DIR}
	${PGSQL_INCLUDE_DIR}
	${QT_QTXML_INCLUDE_DIR}
	${QT_QTASSISTANT_INCLUDE_DIR}
	${CMAKE_CURRENT_BINARY_DIR}
	${CMAKE_CURRENT_SOURCE_DIR}
	${CMAKE_BINARY_DIR}/bulmalib/src/
	${CMAKE_SOURCE_DIR}/bulmalib/src/
	${CMAKE_BINARY_DIR}/bulmafact/src/
	${CMAKE_SOURCE_DIR}/bulmafact/src/)

# generate rules for building source files from the resources
#

# generate rules for building source files that moc generates
QT4_WRAP_UI(bulmafact_UIS_H ${bulmafact_UIS} OPTIONS -tr i18n)
QT4_WRAP_CPP(bulmafact_MOC_SRCS ${bulmafact_MOCS})



# delete previous list of files to process (i18n)
FILE(REMOVE ${CMAKE_BINARY_DIR}/bulmafact/src/bulmafact_pot_list)

FOREACH(loop ${bulmafact_UIS_H} ${bulmafact_SRC} ${bulmafactEx})
    FILE(APPEND ${CMAKE_BINARY_DIR}/bulmafact/src/bulmafact_pot_list ${loop} "\n")
ENDFOREACH(loop)



add_library(libbulmafact SHARED ${bulmafact_MOCS} ${bulmafact_MOC_SRCS}
		${bulmafact_UIS_H} ${bulmafact_SRC} ${bulmafact_SRCS} )



set_target_properties( libbulmafact PROPERTIES
  SOVERSION "${BULMAGES_VERSION_MAJOR}.${BULMAGES_VERSION_MINOR}"
  VERSION "${BULMAGES_VERSION}"
  OUTPUT_NAME bulmafact )

target_link_libraries( libbulmafact bulmalib ${QT_LIBRARIES}
				${PGSQL_LIBRARIES})


add_executable(bulmafact ${bulmafactEx})

# i18n
GETTEXT_CREATE_TEMPLATE(bulmafact ${CMAKE_BINARY_DIR}/bulmafact/src ${CMAKE_SOURCE_DIR}/bulmafact/src/ ${CMAKE_BINARY_DIR}/bulmafact/src/bulmafact_pot_list)
GETTEXT_CREATE_TRANSLATIONS (${CMAKE_BINARY_DIR}/bulmafact/src/bulmafact.pot ALL LANGUAGES es_ES.UTF-8 en_US.UTF-8 fr_FR.UTF-8)

# i18n commands executed after build.
ADD_CUSTOM_COMMAND(TARGET bulmafact POST_BUILD COMMAND make bulmafact.pot WORKING_DIRECTORY ${CMAKE_BINARY_DIR})
ADD_CUSTOM_COMMAND(TARGET bulmafact POST_BUILD COMMAND make bulmafact_pos WORKING_DIRECTORY ${CMAKE_BINARY_DIR})

target_link_libraries( bulmafact libbulmafact bulmalib ${QT_LIBRARIES} ${PGSQL_LIBRARIES})

add_dependencies(libbulmafact bulmalib)
add_dependencies(bulmafact libbulmafact)

install_targets(/bin bulmafact)	
install_targets(/lib libbulmafact)                   


