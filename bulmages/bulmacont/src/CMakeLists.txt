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

file(GLOB bulmacont_SRC *.c *.cpp *.cxx)
#file(GLOB bulmacont_MOCS *.hpp *.h)
file(GLOB bulmacont_UIS *.ui)

set(bulmacont_MOCS bulmacont.h 
#           empresa.h 
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
           sobreview.h 
           listcuentasview1.h 
           cuentaview.h 
           diarioview.h 
           extractoview1.h 
#           balance1view.h 
#           balanceview.h 
           aplinteligentesview.h 
           asientosview.h 
           regivaprintview.h 
           mpatrimonialview.h 
           mpatrimonialesview.h 
#           balanceprintview.h 
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
	   subform2bc.h 
	   diariosubform.h 
	   extractosubform.h 
	   asientolistsubform.h 
#	   balancesubform.h 
	   busquedaccoste.h 
	   busquedacanal.h 
	   fichabc.h)


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
QT4_WRAP_UI(bulmacont_UIS_H ${bulmacont_UIS} OPTIONS -tr QObject::trUtf8)
QT4_WRAP_CPP(bulmacont_MOC_SRCS ${bulmacont_MOCS})

#QT4_ADD_RESOURCES(plugin_RCC_SRCS ${plugin_RCCS})


#ADD_DEFINITIONS(${QT_DEFINITIONS})
#ADD_DEFINITIONS(-DQT_SHARED)
# build sources, moc'd sources, and rcc'd sources
# # generate rules for building source files from the resources
# build sources, moc'd sources, and rcc'd sources
add_executable(bulmacont   ${bulmacont_MOCS} ${bulmacont_MOC_SRCS} ${bulmacont_UIS_H}
	${bulmacont_SRC} ${bulmacont_SRCS} )

target_link_libraries( bulmacont bulmalib ${QT_LIBRARIES} ${PGSQL_LIBRARIES})

add_dependencies(bulmacont bulmalib)

install_targets(/bin bulmacont)	

# 
# 
# set(TRANSLATIONS bulmacont_es.ts 
#                 bulmacont_ca.ts 
#                 bulmacont_en.ts 
#                 bulmacont_fr.ts 
#                 bulmacont_de.ts 
# 		bulmacont_zz.ts)
