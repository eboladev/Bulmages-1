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


#Bulmalib

configure_file( ${CMAKE_SOURCE_DIR}/bulmalib/src/config.h.in  ${CMAKE_BINARY_DIR}/bulmalib/src/config.h @ONLY)

#bulmalib needs QtAssistant and QtXml
set(QT_USE_QTASSISTANT true)
set(QT_USE_QTXML true)
set(QT_USE_QTUITOOLS true)

INCLUDE(${QT_USE_FILE})

set(bulmalib_SRCS postgresiface2.cpp configuracion.cpp numerocuenta.cpp 
         funcaux.cpp logpass.cpp
         abreempresaview.cpp
         dialogchanges.cpp
         fixed.cpp
         msgerror.cpp
         busquedafecha.cpp
         qtable2.cpp 
	 bdockwidget.cpp
         pgimportfiles.cpp
         plugins.cpp
         busquedaperiodo.cpp
         dbrecord.cpp
         subform.cpp
         subform3.cpp
         listventanas.cpp
         qworkspace2.cpp
         qapplication2.cpp
         ficha.cpp
         empresabase.cpp
         qtexteditdelegate.cpp
         qdoublespinbox2.cpp
         blwidget.cpp
         listado.cpp
         porcentajecarga.cpp
         busquedafecha2.cpp
         splashscreen.cpp
         fichacfg.cpp
         paisview.cpp
         busquedaprovincia.cpp
         qradiobutton2.cpp
         qcombobox2.cpp
         blprogressbar.cpp
	 blmainwindow.cpp
	 blbancoedit.cpp
	 blmonthcalendar.cpp
	 busqueda.cpp
	 bli18n.cpp)

set(bulmalib_MOCS 
         logpass.h 
         abreempresaview.h 
         msgerror.h 
         busquedafecha.h 
         qtable2.h 
         busquedaperiodo.h 
         bdockwidget.h 
         subform3.h 
         listventanas.h 
         qworkspace2.h 
         qapplication2.h 
         ficha.h 
         qtexteditdelegate.h 
         qdoublespinbox2.h 
         blwidget.h 
	 blmainwindow.h
         listado.h 
         porcentajecarga.h 
         busquedafecha2.h 
         splashscreen.h 
         fichacfg.h 
         paisview.h 
         busquedaprovincia.h 
         qradiobutton2.h
         qcombobox2.h
	 blbancoedit.h
			blmonthcalendar.h
	busqueda.h
	bli18n.h)

set (bulmalib_HDRS abreempresaview.h
			busquedafecha.h
			busquedaperiodo.h
			configuracion.h
			dbrecord.h
			dialogchanges.h
			empresabase.h
			ficha.h
			fixed.h
			funcaux.h
			listventanas.h
			logpass.h
			msgerror.h
			numerocuenta.h
			pgimportfiles.h
			plugins.h
			postgresiface2.h
			qapplication2.h
			qtable2.h
			qtexteditdelegate.h
			qworkspace2.h
			subform.h
			subform3.h
			blprogressbar.h
			blbancoedit.h)
#headers that are not moc'ed
#			postgresiface2.h 
#			configuracion.h 
#			numerocuenta.h 
#			funcaux.h 
#			dialogchanges.h 
#			fixed.h 
#			pgimportfiles.h 
#			plugins.h 
#			dbrecord.h 
#			subform.h 
#			empresabase.h 
#			blmainwindow.h

set(bulmalib_RCCS ../../bulmages.qrc)
set(bulmalib_UIS logpassbase.ui 
      abreempresabase.ui 
      msgerrorbase.ui 
      busquedafechabase.ui 
      subform3base.ui 
      porcentajecargabase.ui 
      fichacfgbase.ui 
      paisbase.ui
      blprogressbarbase.ui
      busquedabase.ui
      blbancoeditbase.ui)

include_directories(
      ${QT_INCLUDE_DIR}
      ${PGSQL_INCLUDE_DIR}
      ${QT_QTXML_INCLUDE_DIR}
      ${CMAKE_CURRENT_BINARY_DIR}
      ${CMAKE_CURRENT_SOURCE_DIR}
      ${CMAKE_SOURCE_DIR}/bulmalib/src/
      ${CMAKE_BINARY_DIR}/bulmalib/src/
      ${CMAKE_SOURCE_DIR}/bulmacont/src/)


# generate rules for building source files from the resources
QT4_WRAP_UI(bulmalib_UIS_H ${bulmalib_UIS} OPTIONS -tr N_)
QT4_WRAP_CPP(bulmalib_MOC_SRCS ${bulmalib_MOCS})
QT4_ADD_RESOURCES(bulmalib_RCC_SRCS ${bulmalib_RCCS})


# generate rules for building source files that moc generates

#ADD_DEFINITIONS(${QT_DEFINITIONS})
ADD_DEFINITIONS(-DQT_SHARED)

# Defines if we want our own debugging code
#ADD_DEFINITIONS(-DDEPURA_DEBUG)



# build sources, moc'd sources, and rcc'd sources

add_library(bulmalib SHARED ${bulmalib_MOC_SRCS} ${bulmalib_UIS_H}
	${bulmalib_SRCS} ${bulmalib_MOCS} ${bulmalib_RCC_SRCS})

set_target_properties( bulmalib PROPERTIES 
  SOVERSION "${BULMAGES_VERSION_MAJOR}.${BULMAGES_VERSION_MINOR}"
  VERSION "${BULMAGES_VERSION}")

target_link_libraries( bulmalib ${QT_LIBRARIES} ${PGSQL_LIBRARIES} )


# i18n
ADD_DEFINITIONS("-D'_(String)=BlI18n(\"bulmalib\", String)'" )
ADD_DEFINITIONS("-D'N_(String, Int)=BlI18n(\"bulmalib\", String)'" )

if(USE_GETTEXT)
   set(bulmalib_STR ${bulmalib} ${bulmalib_MOCS} ${bulmalib_MOC_SRCS}
                ${bulmalib_UIS_H} ${bulmalib_SRC} ${bulmalib_SRCS})
   GETTEXT_CREATE_TEMPLATE("bulmalib" ${CMAKE_CURRENT_SOURCE_DIR}/po ${CMAKE_CURRENT_SOURCE_DIR} bulmalib_STR)
   GETTEXT_CREATE_TRANSLATIONS(${CMAKE_CURRENT_SOURCE_DIR}/po/bulmalib.pot ALL LANGUAGES es_ES.UTF-8 ca_ES.UTF-8 en_US.UTF-8 fr_FR.UTF-8)
endif(USE_GETTEXT)


install_targets(/lib bulmalib)
install (FILES ${bulmalib_HDRS} DESTINATION include)


# TRANSLATIONS += bulmalib_es.ts 
#                 bulmalib_ca.ts 
#                 bulmalib_en.ts 
#                 bulmalib_fr.ts 
#                 bulmalib_de.ts 
#                 bulmalib_zz.ts 
# 
