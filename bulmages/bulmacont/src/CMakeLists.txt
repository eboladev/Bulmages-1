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

###########################################
# bulmacont                               #
###########################################

file(GLOB bulmacont_SRC *.c *.cpp)
file(GLOB bulmacont_UIS *.ui)


set(bulmacontEx main.cxx)

set(bulmacont_MOCS bcbulmacont.h
           bcaboutview.h
           bcmasapatrimonialview.h
           bcmasapatrimoniallistview.h
           bccanalview.h
           bccentrocosteview.h
           bcconfiguracionview.h
           bccambiacuentaview.h
           bccentrocosteseleccionarview.h
           bccanalseleccionarview.h
           bctipoivaview.h
           bcformapagoview.h
           bcsubform.h
           bcbuscarcentrocoste.h
           bcbuscarcanal.h 
           bcform.h
           bcasientoduplicarview.h)


# Exclude files with no QObject macro.
#file(GLOB bulmacont_NO_MOCS bcmodelo300imprimir2ps.h)
#list(REMOVE_ITEM bulmacont_MOCS ${bulmacont_NO_MOCS}) 


set(QT_USE_QTASSISTANT false)
set(QT_USE_QTXML true)
set(QT_USE_QTSCRIPT true)


INCLUDE(${QT_USE_FILE})

include_directories(
    ${QT_INCLUDE_DIR}
	${PGSQL_INCLUDE_DIR}
	${QT_QTSCRIPT_INCLUDE_DIR}
	${QT_QTXML_INCLUDE_DIR}
        ${QT_QTSCRIPT_INCLUDE_DIR}
	${CMAKE_CURRENT_BINARY_DIR}
	${CMAKE_CURRENT_SOURCE_DIR}
	${CMAKE_BINARY_DIR}/bulmalib/src/
	${CMAKE_SOURCE_DIR}/bulmalib/src/
	${CMAKE_BINARY_DIR}/bulmacont/src/
	${CMAKE_SOURCE_DIR}/bulmacont/src/)

if(USE_QMDIAREA)
      ADD_DEFINITIONS("-D'AREA_QMDI'" )
endif(USE_QMDIAREA)

# generate rules for building source files from the resources
#

# generate rules for building source files that moc generates
QT4_WRAP_UI(bulmacont_UIS_H ${bulmacont_UIS} OPTIONS -tr N_)
QT4_WRAP_CPP(bulmacont_MOC_SRCS ${bulmacont_MOCS})

IF(WIN32)
set(bulmacont_RCCS ../../bulmages.qrc)
QT4_ADD_RESOURCES(bulmacont_RCC_SRCS ${bulmacont_RCCS})
ADD_DEFINITIONS(-DQWT_DLL -DQT_DLL -DQWT_MAKEDLL -DBC_BULMACONT)
ENDIF(WIN32)


# i18n
if(WIN32)
ADD_DEFINITIONS("-D_(String)=blI18n(\\\"bulmacont\\\",String)" )
ADD_DEFINITIONS("-DN_(String,Int)=blI18n(\\\"bulmacont\\\",String)" )
endif(WIN32)

if(NOT WIN32)
ADD_DEFINITIONS("-D'_(String)=blI18n(\"bulmacont\",String)'" )
ADD_DEFINITIONS("-D'N_(String,Int)=blI18n(\"bulmacont\",String)'" )
endif(NOT WIN32)

if(USE_GETTEXT)
   set(bulmacont_i18n ${bulmacontEx} ${bulmacont_MOCS} ${bulmacont_MOC_SRCS}
                ${bulmacont_UIS_H} ${bulmacont_SRC} ${bulmacont_SRCS})
   GETTEXT_CREATE_TEMPLATE(bulmacont ${CMAKE_CURRENT_BINARY_DIR}
                                ${CMAKE_CURRENT_SOURCE_DIR} bulmacont_i18n)
   GETTEXT_CREATE_TRANSLATIONS(${CMAKE_CURRENT_BINARY_DIR}/bulmacont.pot
                                ALL LANGUAGES es_ES ca_ES
					      en_US fr_FR)
endif(USE_GETTEXT)




add_library(libbulmacont SHARED ${bulmacont_MOCS} ${bulmacont_MOC_SRCS}
		${bulmacont_UIS_H} ${bulmacont_SRC} ${bulmacont_SRCS} ${bulmacont_RCC_SRCS})

set_target_properties( libbulmacont PROPERTIES
  SOVERSION "${BULMAGES_VERSION_MAJOR}.${BULMAGES_VERSION_MINOR}"
  VERSION "${BULMAGES_VERSION}"
  OUTPUT_NAME bulmacont )

target_link_libraries( libbulmacont bulmalib ${QT_LIBRARIES}
				${PGSQL_LIBRARIES})


add_executable(bulmacont ${bulmacontEx} ${bulmacont_RCC_SRCS})

target_link_libraries( bulmacont libbulmacont bulmalib ${QT_LIBRARIES} ${PGSQL_LIBRARIES})

add_dependencies(libbulmacont bulmalib)
add_dependencies(bulmacont libbulmacont)

install_targets(/bin bulmacont)
install_targets(/lib libbulmacont)

set(bulmacont_STR ${bulmacontEx} ${bulmacont_MOCS} ${bulmacont_MOC_SRCS}
                ${bulmacont_UIS_H} ${bulmacont_SRC} ${bulmacont_SRCS})

