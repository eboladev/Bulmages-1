# Copyright (C) 2008 by Leopold Palomo-Avellaneda                       #
# leo@alaxarxa.net                                                      #
# Modified (C) 2013 by Daniel Ripoll                                                     #
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
# bulmages                                                             #
#########################################################################


file(GLOB bulmages_SRC *.c *.cpp)
#file(GLOB bulmages_MOCS *.hpp *.h)

file(GLOB bulmages_UIS *.ui)

set(bulmages_RCCS bgbulmages.qrc resources.qrc)
set(bulmages_MOCS bgbulmages.h mainwindow.h bgserver.h)

set(bulmagesEx main.cxx)

include_directories(
    ${QT_INCLUDE_DIR}
	${PGSQL_INCLUDE_DIR}
	${QT_QTXML_INCLUDE_DIR}
        ${QT_QTSCRIPT_INCLUDE_DIR}
	${CMAKE_CURRENT_BINARY_DIR}
	${CMAKE_CURRENT_SOURCE_DIR}
	${CMAKE_BINARY_DIR}/bulmalib/src/
	${CMAKE_SOURCE_DIR}/bulmalib/src/
	${CMAKE_BINARY_DIR}/bulmages/src/
	${CMAKE_SOURCE_DIR}/bulmages/src/)



# generate rules for building source files from the resources
#

# generate rules for building source files that moc generates
QT5_WRAP_UI(bulmages_UIS_H ${bulmages_UIS} OPTIONS -tr N_)
QT5_WRAP_CPP(bulmages_MOC_SRCS ${bulmages_MOCS})

QT5_ADD_RESOURCES(bulmages_RCC_SRCS ${bulmages_RCCS})

ADD_DEFINITIONS(-DQT_SHARED)


IF(WIN32)
set(bulmages_RCCS ../../bgbulmages.qrc)
#QT5_ADD_RESOURCES(bulmages_RCC_SRCS ${bulmages_RCCS})
ADD_DEFINITIONS(-DQWT_DLL -DQT_DLL -DQWT_MAKEDLL -DBF_BULMAGES)
ENDIF(WIN32)


add_library(libbulmages SHARED ${bulmages_MOC_SRCS} ${bulmages_UIS_H}
        ${bulmages_SRC} ${bulmages_MOCS} ${bulmages_RCC_SRCS})



set_target_properties( libbulmages PROPERTIES
  SOVERSION "${BULMAGES_VERSION_MAJOR}.${BULMAGES_VERSION_MINOR}"
  VERSION "${BULMAGES_VERSION}"
  OUTPUT_NAME bulmages )

target_link_libraries(libbulmages bulmalib ${QT_LIBRARIES} ${PGSQL_LIBRARIES})


add_executable(bulmages ${bulmagesEx} ${bulmages_RCC_SRCS})



# i18n
if(WIN32)
ADD_DEFINITIONS("-D_(String)=blI18n(\\\"bulmages\\\",String)" )
ADD_DEFINITIONS("-DN_(String,Int)=blI18n(\\\"bulmages\\\",String)" )
endif(WIN32)

if(NOT WIN32)
ADD_DEFINITIONS("-D'_(String)=blI18n(\"bulmages\",String)'" )
ADD_DEFINITIONS("-D'N_(String,Int)=blI18n(\"bulmages\",String)'" )
endif(NOT WIN32)



if(USE_GETTEXT)
   set(bulmages_i18n ${bulmagesEx} ${bulmages_MOCS} ${bulmages_MOC_SRCS}
                ${bulmages_UIS_H} ${bulmages_SRC} ${bulmages_SRCS})
   GETTEXT_CREATE_TEMPLATE(bulmages ${CMAKE_CURRENT_BINARY_DIR}
                                ${CMAKE_CURRENT_SOURCE_DIR} bulmages_i18n)
   GETTEXT_CREATE_TRANSLATIONS(${CMAKE_CURRENT_BINARY_DIR}/bulmages.pot
                                ALL LANGUAGES es_ES ca_ES en_US fr_FR)
endif(USE_GETTEXT)

target_link_libraries(bulmages libbulmages bulmalib ${QT_LIBRARIES} ${PGSQL_LIBRARIES})

add_dependencies(libbulmages bulmalib)
add_dependencies(bulmages libbulmages)

install_targets(/bin bulmages)
install_targets(/lib libbulmages)


