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

set(bulmacont_MOCS bulmacont.h 
           sobreview.h 
           listcuentasview1.h 
           cuentaview.h 
           diarioview.h 
           extractoview1.h 
           aplinteligentesview.h 
           asientosview.h 
           regivaprintview.h 
           mpatrimonialview.h 
           mpatrimonialesview.h 
           canualesprintview.h 
           canualesview.h 
           canalview.h 
           ccosteview.h 
           propiedadesempresa.h 
           extractoprintview.h 
           diarioprintview.h 
           cambiactaview.h 
           duplicarasientoview.h 
           amortizacionview.h 
           listado347.h 
           amortizacionesview.h 
           selectccosteview.h 
           selectcanalview.h 
           modelo300.h 
           bbloqfecha.h 
           tipoivaview.h 
           fpagoview.h 
		   busquedacuenta.h 
		   asiento1.h 
		   asiento1view.h 
	   	   listlinasiento1view.h 
  	   	   bcsubform.h 
	   	   diariosubform.h 
	   	   extractosubform.h 
	   	   asientolistsubform.h 
		   busquedaccoste.h 
	       busquedacanal.h 
	       bcform.h
	       cuentalistview.h)

# ficheros no MOc's
#           bccompany.h 
#           qlineedit1.h 
#           qlistviewitem1.h 
#           libromayorprint.h 
#           diarioprint.h 
#           libromayorprint.h 
#           diarioprint.h 
#           modelosps.h 
#           importainteligente.h 
#           importbalance.h 
#           arbol.h 
#           balance1view.h 
#           balanceview.h 
#           balanceprintview.h 
#	        balancesubform.h 
	   

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
	${CMAKE_SOURCE_DIR}/bulmalib/src/
	${CMAKE_BINARY_DIR}/bulmacont/src/
	${CMAKE_SOURCE_DIR}/bulmacont/src/)

# generate rules for building source files from the resources
#

# generate rules for building source files that moc generates
QT4_WRAP_UI(bulmacont_UIS_H ${bulmacont_UIS} OPTIONS -tr N_)
QT4_WRAP_CPP(bulmacont_MOC_SRCS ${bulmacont_MOCS})


# i18n
ADD_DEFINITIONS("-D'_(String)=BlI18n(\"bulmacont\", String)'" )
ADD_DEFINITIONS("-D'N_(String, Int)=BlI18n(\"bulmacont\", String)'" )

if(USE_GETTEXT)
   set(bulmacont_i18n ${bulmacontEx} ${bulmacont_MOCS} ${bulmacont_MOC_SRCS}
                ${bulmacont_UIS_H} ${bulmacont_SRC} ${bulmacont_SRCS})
   GETTEXT_CREATE_TEMPLATE(bulmacont ${CMAKE_CURRENT_BINARY_DIR}
                                ${CMAKE_CURRENT_SOURCE_DIR} bulmacont_i18n)
   GETTEXT_CREATE_TRANSLATIONS(${CMAKE_CURRENT_BINARY_DIR}/bulmacont.pot
                                ALL LANGUAGES es_ES.UTF-8 ca_ES.UTF-8
					      en_US.UTF-8 fr_FR.UTF-8)
endif(USE_GETTEXT)




add_library(libbulmacont SHARED ${bulmacont_MOCS} ${bulmacont_MOC_SRCS}
		${bulmacont_UIS_H} ${bulmacont_SRC} ${bulmacont_SRCS} )

set_target_properties( libbulmacont PROPERTIES
  SOVERSION "${BULMAGES_VERSION_MAJOR}.${BULMAGES_VERSION_MINOR}"
  VERSION "${BULMAGES_VERSION}"
  OUTPUT_NAME bulmacont )

target_link_libraries( libbulmacont bulmalib ${QT_LIBRARIES}
				${PGSQL_LIBRARIES})


add_executable(bulmacont ${bulmacontEx})

target_link_libraries( bulmacont libbulmacont bulmalib ${QT_LIBRARIES} ${PGSQL_LIBRARIES})

add_dependencies(libbulmacont bulmalib)
add_dependencies(bulmacont libbulmacont)

install_targets(/bin bulmacont)	
install_targets(/lib/ libbulmacont)                   

set(bulmacont_STR ${bulmacontEx} ${bulmacont_MOCS} ${bulmacont_MOC_SRCS}
                ${bulmacont_UIS_H} ${bulmacont_SRC} ${bulmacont_SRCS})

