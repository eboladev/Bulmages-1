# Copyright (C) 2008 by Leopold Palomo-Avellaneda                       #
# leo@alaxarxa.net                                                      #
# Modified (C) 2013 by Daniel Ripoll                                    #
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

# Exclude files with no QObject macro.
file(GLOB bulmafact_NO_MOCS bfcompany.h bfinformereferencia.h)
list(REMOVE_ITEM bulmafact_MOCS ${bulmafact_NO_MOCS})

file(GLOB bulmafact_UIS *.ui)

set(bulmafactEx main.cxx)

include_directories(
    ${QT_INCLUDE_DIR}
	${PGSQL_INCLUDE_DIR}
	${QT_QTXML_INCLUDE_DIR}
        ${QT_QTSCRIPT_INCLUDE_DIR}
	${CMAKE_CURRENT_BINARY_DIR}
	${CMAKE_CURRENT_SOURCE_DIR}
	${CMAKE_BINARY_DIR}/bulmalib/src/
	${CMAKE_SOURCE_DIR}/bulmalib/src/
	${CMAKE_BINARY_DIR}/bulmafact/src/
	${CMAKE_SOURCE_DIR}/bulmafact/src/)


# generate rules for building source files from the resources
#

# generate rules for building source files that moc generates
QT5_WRAP_UI(bulmafact_UIS_H ${bulmafact_UIS} OPTIONS -tr N_)
QT5_WRAP_CPP(bulmafact_MOC_SRCS ${bulmafact_MOCS})

set(bulmafact_RCCS ../../bulmages.qrc)
QT5_ADD_RESOURCES(bulmafact_RCC_SRCS ${bulmafact_RCCS})

IF(WIN32)
ADD_DEFINITIONS(-DQWT_DLL -DQT_DLL -DQWT_MAKEDLL -DBF_BULMAFACT)
ENDIF(WIN32)


add_library(libbulmafact SHARED ${bulmafact_MOCS} ${bulmafact_MOC_SRCS}
		${bulmafact_UIS_H} ${bulmafact_SRC} ${bulmafact_SRCS} ${bulmafact_RCC_SRCS})



set_target_properties( libbulmafact PROPERTIES
  SOVERSION "${BULMAGES_VERSION_MAJOR}.${BULMAGES_VERSION_MINOR}"
  VERSION "${BULMAGES_VERSION}"
  OUTPUT_NAME bulmafact )

qt5_use_modules(libbulmafact UiTools Xml Core Widgets Script)

target_link_libraries(libbulmafact bulmalib ${QT_LIBRARIES} ${PGSQL_LIBRARIES})


add_executable(bulmafact ${bulmafactEx} ${bulmafact_RCC_SRCS})



# i18n
if(WIN32)
ADD_DEFINITIONS("-D_(String)=blI18n(\\\"bulmafact\\\",String)" )
ADD_DEFINITIONS("-DN_(String,Int)=blI18n(\\\"bulmafact\\\",String)" )
endif(WIN32)

if(NOT WIN32)
ADD_DEFINITIONS("-D'_(String)=blI18n(\"bulmafact\",String)'" )
ADD_DEFINITIONS("-D'N_(String,Int)=blI18n(\"bulmafact\",String)'" )
endif(NOT WIN32)



if(USE_GETTEXT)
   set(bulmafact_i18n ${bulmafactEx} ${bulmafact_MOCS} ${bulmafact_MOC_SRCS}
                ${bulmafact_UIS_H} ${bulmafact_SRC} ${bulmafact_SRCS})
   GETTEXT_CREATE_TEMPLATE(bulmafact ${CMAKE_CURRENT_BINARY_DIR}
                                ${CMAKE_CURRENT_SOURCE_DIR} bulmafact_i18n)
   GETTEXT_CREATE_TRANSLATIONS(${CMAKE_CURRENT_BINARY_DIR}/bulmafact.pot
                                ALL LANGUAGES es_ES ca_ES en_US fr_FR)
endif(USE_GETTEXT)

target_link_libraries(bulmafact libbulmafact bulmalib ${QT_LIBRARIES} ${PGSQL_LIBRARIES})

add_dependencies(libbulmafact bulmalib)
add_dependencies(bulmafact libbulmafact)

install_targets(/bin bulmafact)
install_targets(/lib libbulmafact)


