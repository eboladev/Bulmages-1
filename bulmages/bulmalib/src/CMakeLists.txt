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
#########################################################################
#Bulmalib                                                               #
#########################################################################

configure_file( ${CMAKE_SOURCE_DIR}/bulmalib/src/blconfig.h.in  ${CMAKE_BINARY_DIR}/bulmalib/src/blconfig.h @ONLY)

#set(QT_USE_QTXML true)
#set(QT_USE_QTSCRIPT true)
#set(QT_USE_QTUITOOLS true)

set(bulmalib_SRCS	blpostgresqlclient.cpp
			blconfiguration.cpp
			blspainbankaccountnumber.cpp
			blfunctions.cpp
			bldblogindialog.cpp
			blcompanydialog.cpp
			bldialogchanges.cpp
			blfixed.cpp
			blerrordialog.cpp
			bldatesearch.cpp
			bltablewidget.cpp
			bldockwidget.cpp
			blimportexport.cpp
			blplugins.cpp
			blperiodicitycombobox.cpp
			bldb.cpp
			bldbsubform.cpp
			blsubform.cpp
			blwindowlistdock.cpp
			blworkspace.cpp
			blapplication.cpp
			blform.cpp
			blmaincompany.cpp
			bltexteditdelegate.cpp
			bldoublespinbox.cpp
			blwidget.cpp
			blformlist.cpp
			blformlistfilter.cpp
			bldatelineedit.cpp
			blsplashscreen.cpp
			blformconfig.cpp
			blcountryview.cpp
			blradiobutton.cpp
			blcombobox.cpp
			blcomboboxdelegate.cpp
			bldatedelegate.cpp
			blgenericcomboboxdelegate.cpp
			blprogressbar.cpp
			blmainwindow.cpp
            		blpicture.cpp
			blbankedit.cpp
			blmonthcalendar.cpp
			blsearchwidget.cpp
			bli18n.cpp
			bltreewidget.cpp
			blmaincompanypointer.cpp
			blargparser.cpp
			bltoolbutton.cpp
			blescprinter.cpp
			blcp850eurotextcodec.cpp
			bluiloader.cpp
			blaction.cpp)

set(bulmalib_MOCS	bldblogindialog.h
			blcompanydialog.h
			blerrordialog.h
			bldatesearch.h
			bltablewidget.h
			blperiodicitycombobox.h
			bldockwidget.h
			blwindowlistdock.h
			blapplication.h
			bltexteditdelegate.h
			bldoublespinbox.h
			blwidget.h
			blmainwindow.h
	            	blpicture.h
			blformlist.h
			blformlistfilter.h
			bldatelineedit.h
			blsplashscreen.h
			blformconfig.h
			blcountryview.h
			blradiobutton.h
			blcombobox.h
			blcomboboxdelegate.h
			blgenericcomboboxdelegate.h
			blmonthcalendar.h
			blsearchwidget.h
			blform.h
			blmaincompany.h
			blsubform.h
			bltreewidget.h
			bltoolbutton.h
			blbankedit.h
			bluiloader.h
			blaction.h)





set (bulmalib_HDRS	blmaincompanypointer.h
			blmaincompany.h
			bldialogchanges.h
			blconfiguration.h
			blspainbankaccountnumber.h
			bldbsubform.h
			bltreewidget.h
			blprogressbar.h
			bldb.h
			blescprinter.h
			blcp850eurotextcodec.h
			blfixed.h
			blfunctions.h
			blimportexport.h
			blmaincompany.h
	            	blpicture.h
			blplugins.h
			blpostgresqlclient.h
			blargparser.h)

			
			
#headers that are not moc'ed
#			blpostgresqlclient.h 
#			blconfiguration.h 
#			blspainbankaccountnumber.h 
#			blfunctions.h 
#			dialogchanges.h 
#			blfixed.h 
#			blimportexport.h 
#			blplugins.h 
#			bldb.h 
#			bldbsubform.h 
#			blmaincompany.h 
#			blmainwindow.h
#			blargparser.h


set(bulmalib_RCCS ../../bulmages.qrc)
set(bulmalib_UIS	bldblogindialogbase.ui
			blcompanydialogbase.ui
			blerrordialogbase.ui
			bldatesearchbase.ui
			blsubformbase.ui
			blformconfigbase.ui
			blcountrybase.ui
			blprogressbarbase.ui
			blsearchwidgetbase.ui
			blformlistfilterbase.ui
			blbankeditbase.ui)


#message(FATAL_ERROR "La ruta: ${Qt5Widgets_INCLUDES} ${QT_INCLUDE_DIR}")

if (USE_GETTEXT)
    include_directories(
      ${PGSQL_INCLUDE_DIR}
      ${QT_INCLUDE_DIR}
      ${GETTEXT_INCLUDE_DIR}
      ${QT_QTXML_INCLUDE_DIR}
      ${QT_QTSCRIPT_INCLUDE_DIR}
      ${Qt5Widgets_INCLUDES}
      ${CMAKE_CURRENT_BINARY_DIR}
      ${CMAKE_CURRENT_SOURCE_DIR}
      ${CMAKE_SOURCE_DIR}/bulmalib/src/
      ${CMAKE_BINARY_DIR}/bulmalib/src/
      ${CMAKE_SOURCE_DIR}/bulmacont/src/)
else (USE_GETTEXT)
    include_directories(
      ${PGSQL_INCLUDE_DIR}
      ${QT_INCLUDE_DIR}
      ${QT_QTWIDGETS_INCLUDE_DIR}
      ${QT_QTXML_INCLUDE_DIR}
      ${QT_QTSCRIPT_INCLUDE_DIR}
      ${CMAKE_CURRENT_BINARY_DIR}
      ${CMAKE_CURRENT_SOURCE_DIR}
      ${CMAKE_SOURCE_DIR}/bulmalib/src/
      ${CMAKE_BINARY_DIR}/bulmalib/src/
      ${CMAKE_SOURCE_DIR}/bulmacont/src/)
endif(USE_GETTEXT)

if(USE_QMDIAREA)
      ADD_DEFINITIONS("-D'AREA_QMDI'" )
endif(USE_QMDIAREA)

# generate rules for building source files from the resources
QT5_WRAP_UI(bulmalib_UIS_H ${bulmalib_UIS} OPTIONS -tr N_)
QT5_WRAP_CPP(bulmalib_MOC_SRCS ${bulmalib_MOCS})
QT5_ADD_RESOURCES(bulmalib_RCC_SRCS ${bulmalib_RCCS})


# generate rules for building source files that moc generates

#ADD_DEFINITIONS(${QT_DEFINITIONS})
#ADD_DEFINITIONS(-DQT_SHARED)

IF(WIN32)
ADD_DEFINITIONS(-DQWT_DLL -DQT_DLL -DQWT_MAKEDLL -DBL_BULMALIB)
ENDIF(WIN32)

IF(USE_GETTEXT)
ADD_DEFINITIONS(-DUSE_GETTEXT)
ENDIF(USE_GETTEXT)


# Defines if we want our own debugging code
#ADD_DEFINITIONS(-DDEPURA_DEBUG)



# build sources, moc'd sources, and rcc'd sources

add_library(bulmalib SHARED ${bulmalib_MOC_SRCS} ${bulmalib_UIS_H}
	${bulmalib_SRCS} ${bulmalib_MOCS} ${bulmalib_RCC_SRCS})

set_target_properties( bulmalib PROPERTIES 
  SOVERSION "${BULMAGES_VERSION_MAJOR}.${BULMAGES_VERSION_MINOR}"
  VERSION "${BULMAGES_VERSION}")

qt5_use_modules(bulmalib UiTools Xml Core Widgets Script WebKit)

IF(USE_GETTEXT)
	target_link_libraries( bulmalib ${QT_LIBRARIES} ${PGSQL_LIBRARIES} ${GETTEXT_LIBRARIES} )
ELSE(USE_GETTEXT)
	target_link_libraries( bulmalib ${QT_LIBRARIES} ${PGSQL_LIBRARIES} )
ENDIF(USE_GETTEXT)



# i18n
if(WIN32)
ADD_DEFINITIONS("-D_(String)=blI18n(\\\"bulmalib\\\",String)" )
ADD_DEFINITIONS("-DN_(String,Int)=blI18n(\\\"bulmalib\\\",String)" )
endif(WIN32)

if(NOT WIN32)
ADD_DEFINITIONS("-D'_(String)=blI18n(\"bulmalib\",String)'" )
ADD_DEFINITIONS("-D'N_(String,Int)=blI18n(\"bulmalib\",String)'" )
endif(NOT WIN32)


if(USE_GETTEXT)
   set(bulmalib_i18n ${bulmalibEx} ${bulmalib_MOCS} ${bulmalib_MOC_SRCS}
                ${bulmalib_UIS_H} ${bulmalib_SRC} ${bulmalib_SRCS})
   GETTEXT_CREATE_TEMPLATE(bulmalib ${CMAKE_CURRENT_BINARY_DIR}
                                ${CMAKE_CURRENT_SOURCE_DIR} bulmalib_i18n)
   GETTEXT_CREATE_TRANSLATIONS(${CMAKE_CURRENT_BINARY_DIR}/bulmalib.pot
                                ALL LANGUAGES es_ES ca_ES en_US fr_FR)
endif(USE_GETTEXT)


install_targets(/lib bulmalib)
install (FILES ${bulmalib_HDRS} DESTINATION include)



if(USE_PCH)
   add_dependencies( bulmalib  bulmalib_pch )
endif(USE_PCH)

ADD_LOCAL_BLI18N_HEADER( bulmalib )
