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
QT4_WRAP_UI(bulmages_UIS_H ${bulmages_UIS})
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


