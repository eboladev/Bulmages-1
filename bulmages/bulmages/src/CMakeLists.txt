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
# bulmages launcher					  	  #
###########################################


set(bulmages_UIS comun/importContaplusBase.ui 
            comun/selectorbase.ui 
            comun/configuracion.ui
            comun/nuevaempresadlg.ui 
            comun/nuevafactbase.ui 
            comun/importexportbulmafactbase.ui)

set(bulmages_SRC main.cpp 
            comun/bselector.cpp 
            comun/bconfiguracion.cpp 
            comun/bnuevaempresa.cpp 
            comun/nuevafact.cpp 
            comun/gongimportfiles.cpp 
            comun/importContaplus.cpp 
            comun/importexportbulmafactview.cpp)

set(bulmages_MOCS comun/bselector.h 
            comun/bconfiguracion.h 
            comun/bnuevaempresa.h 
           #comun/nuevafact.h 
            comun/gongimportfiles.h 
            comun/importContaplus.h 
            comun/importexportbulmafactview.h)


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
# build sources, moc'd sources, and rcc'd sources
# # generate rules for building source files from the resources
# build sources, moc'd sources, and rcc'd sources


add_executable(bulmages  ${bulmages_MOCS} ${bulmages_MOC_SRCS} ${bulmages_UIS_H}
   ${bulmages_SRC} ${bulmages_SRCS} )

target_link_libraries( bulmages bulmalib ${QT_LIBRARIES} ${PGSQL_LIBRARIES})

add_dependencies(bulmages bulmalib)

install_targets(/bin bulmages)	


