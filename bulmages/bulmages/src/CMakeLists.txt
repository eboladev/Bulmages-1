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
# bulmages launcher                       #
###########################################


set(bulmages_UIS importContaplusBase.ui 
            selectorbase.ui 
            configuracion.ui
            nuevaempresadlg.ui 
            nuevafactbase.ui 
            importexportbulmafactbase.ui)

set(bulmages_SRC main.cpp 
            bselector.cpp 
            bconfiguracion.cpp 
            bnuevaempresa.cpp 
            nuevafact.cpp 
            gongimportfiles.cpp 
            importContaplus.cpp 
            importexportbulmafactview.cpp)

set(bulmages_MOCS bselector.h 
            bconfiguracion.h 
            bnuevaempresa.h 
            importContaplus.h 
            importexportbulmafactview.h)


set(QT_USE_QTASSISTANT true)
set(QT_USE_QTXML true)

INCLUDE(   ${QT_USE_FILE}   )

include_directories(
   ${QT_INCLUDE_DIR}
   ${PGSQL_INCLUDE_DIR}
   ${QT_QTXML_INCLUDE_DIR}
   ${QT_QTASSISTANT_INCLUDE_DIR}
   ${CMAKE_CURRENT_BINARY_DIR}
   ${CMAKE_CURRENT_SOURCE_DIR}
   ${CMAKE_BINARY_DIR}/bulmalib/src/
   ${CMAKE_SOURCE_DIR}/bulmalib/src/)

# generate rules for building source files from the resources
#

# generate rules for building source files that moc generates
QT4_WRAP_UI(bulmages_UIS_H ${bulmages_UIS} OPTIONS -tr QObject::trUtf8)
QT4_WRAP_CPP(bulmages_MOC_SRCS ${bulmages_MOCS})

#QT4_ADD_RESOURCES(plugin_RCC_SRCS ${plugin_RCCS})


#ADD_DEFINITIONS(${QT_DEFINITIONS})
#ADD_DEFINITIONS(-DQT_SHARED)

# i18n
ADD_DEFINITIONS("-D'_(String)=BlI18n(\"bulmages\", String)'" )
ADD_DEFINITIONS("-D'N_(String, Int)=BlI18n(\"bulmages\", String)'" )

if(USE_GETTEXT)
   set(bulmages_i18n ${bulmages_MOCS} ${bulmages_MOC_SRCS}
		${bulmages_UIS_H} ${bulmages_SRC} ${bulmages_SRCS} )

   GETTEXT_CREATE_TEMPLATE(bulmages ${CMAKE_CURRENT_BINARY_DIR}
                                ${CMAKE_CURRENT_SOURCE_DIR} bulmages_i18n)
   GETTEXT_CREATE_TRANSLATIONS(${CMAKE_CURRENT_BINARY_DIR}/bulmages.pot
                                ALL LANGUAGES es_ES.UTF-8 ca_ES.UTF-8 en_US.UTF-8 fr_FR.UTF-8)
endif(USE_GETTEXT)


# build sources, moc'd sources, and rcc'd sources
# # generate rules for building source files from the resources
# build sources, moc'd sources, and rcc'd sources


add_executable(bulmages  ${bulmages_MOCS} ${bulmages_MOC_SRCS} ${bulmages_UIS_H}
   ${bulmages_SRC} ${bulmages_SRCS} )

target_link_libraries( bulmages bulmalib ${QT_LIBRARIES} ${PGSQL_LIBRARIES})

add_dependencies(bulmages bulmalib)

install_targets(/bin bulmages)	


