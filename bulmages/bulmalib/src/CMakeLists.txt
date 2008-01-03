#Bulmalib

#bulmalib needs QtAssistant and QtXml
set(QT_USE_QTASSISTANT true)
set(QT_USE_QTXML true)

INCLUDE(${QT_USE_FILE})

set(bulmalib_SRCS postgresiface2.cpp configuracion.cpp numerocuenta.cpp 
           	funcaux.cpp logpass.cpp 
			abreempresaview.cpp 
			dialogchanges.cpp 
			fixed.cpp 
			msgerror.cpp 
			busquedafecha.cpp 
			qtable2.cpp 
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
			qcombobox2.cpp)

set(bulmalib_MOCS 
			logpass.h 
			abreempresaview.h 
			msgerror.h 
			busquedafecha.h 
			qtable2.h 
			busquedaperiodo.h 
			subform3.h 
			listventanas.h 
			qworkspace2.h 
			qapplication2.h 
			ficha.h 
	   		qtexteditdelegate.h 
	           	qdoublespinbox2.h 
			blwidget.h 
			listado.h 
			porcentajecarga.h 
			busquedafecha2.h 
			splashscreen.h 
			fichacfg.h 
			paisview.h 
			busquedaprovincia.h 
			qradiobutton2.h
			qcombobox2.h)

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


set(bulmalib_RCCS ../../bulmages.qrc)
set(bulmalib_UIS logpassbase.ui 
        abreempresabase.ui 
        msgerrorbase.ui 
        busquedafechabase.ui 
        subform3base.ui 
	 	porcentajecargabase.ui 
	 	fichacfgbase.ui 
	 	paisbase.ui)

include_directories(
    ${QT_INCLUDE_DIR}
	${PGSQL_INCLUDE_DIR}
	${QT_QTXML_INCLUDE_DIR}
	${CMAKE_CURRENT_BINARY_DIR}
	${CMAKE_CURRENT_SOURCE_DIR}
	#${CMAKE_CURRENT_BINARY_DIR}/../../src/
	${CMAKE_SOURCE_DIR}/bulmalib/src/
	${CMAKE_CURRENT_BINARY_DIR}/bulmalib/src/
	${CMAKE_SOURCE_DIR}/bulmacont/src/)


# generate rules for building source files from the resources
QT4_WRAP_UI(bulmalib_UIS_H ${bulmalib_UIS})
QT4_WRAP_CPP(bulmalib_MOC_SRCS ${bulmalib_MOCS})
QT4_ADD_RESOURCES(bulmalib_RCC_SRCS ${bulmalib_RCCS})

# generate rules for building source files that moc generates

#ADD_DEFINITIONS(${QT_DEFINITIONS})
ADD_DEFINITIONS(-DQT_SHARED)

# Defines if we want our own debugging code
ADD_DEFINITIONS(-DEPURA_DEBUG)

# build sources, moc'd sources, and rcc'd sources

add_library(bulmalib SHARED ${bulmalib_MOC_SRCS} ${bulmalib_UIS_H}
	${bulmalib_SRCS} ${bulmalib_MOCS} ${bulmalib_RCC_SRCS})

set_target_properties( bulmalib PROPERTIES 
  SOVERSION "${BULMAGES_VERSION_MAJOR}.${BULMAGES_VERSION_MINOR}"
  VERSION "${BULMAGES_VERSION}")

target_link_libraries( bulmalib ${QT_LIBRARIES} ${PGSQL_LIBRARIES} )

install_targets(/lib bulmalib)

# TRANSLATIONS += bulmalib_es.ts 
#                 bulmalib_ca.ts 
#                 bulmalib_en.ts 
#                 bulmalib_fr.ts 
#                 bulmalib_de.ts 
#                 bulmalib_zz.ts 
# 
# 	   
# 	   
	   






