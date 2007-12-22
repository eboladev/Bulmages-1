###########################################
# bulmacont					  #
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
           balance1view.h 
           balanceview.h 
           aplinteligentesview.h 
           asientosview.h 
           regivaprintview.h 
           mpatrimonialview.h 
           mpatrimonialesview.h 
           balanceprintview.h 
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
	   balancesubform.h 
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
QT4_WRAP_UI(bulmacont_UIS_H ${bulmacont_UIS})
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
# 
# INCLUDE(   ${QT_USE_FILE}   )
# set(QT_USE_QTASSISTANT true)
# set(QT_USE_QTXML true)
# 
# set(bulmacont_SRCS main.cpp 
#            bulmacont.cpp 
#            sobreview.cpp 
#            empresa.cpp 
#            listcuentasview1.cpp 
#            cuentaview.cpp 
#            diarioview.cpp 
#            qlineedit1.cpp 
#            extractoview1.cpp 
#            balance1view.cpp 
#            qlistviewitem1.cpp 
#            balanceview.cpp 
#            aplinteligentesview.cpp 
#            asientosview.cpp 
#            regivaprintview.cpp 
#            mpatrimonialview.cpp 
#            mpatrimonialesview.cpp 
#            balanceprintview.cpp 
#            canualesprintview.cpp 
#            canualesview.cpp 
#            canalview.cpp 
#            ccosteview.cpp 
#            propiedadesempresa.cpp 
#            extractoprintview.cpp 
#            libromayorprint.cpp 
#            diarioprint.cpp 
#            diarioprintview.cpp 
#            cambiactaview.cpp 
#            duplicarasientoview.cpp 
#            amortizacionview.cpp 
#            listado347.cpp 
#            amortizacionesview.cpp 
#            selectccosteview.cpp 
#            selectcanalview.cpp 
#            modelosps.cpp 
#            modelo300.cpp 
#            importainteligente.cpp 
#            importbalance.cpp 
#            bbloqfecha.cpp 
#            tipoivaview.cpp 
#            fpagoview.cpp 
#            arbol.cpp 
# 	   busquedacuenta.cpp 
# 	   asiento1.cpp 
# 	   asiento1view.cpp 
# 	   listlinasiento1view.cpp 
# 	   subform2bc.cpp 
# 	   diariosubform.cpp 
# 	   extractosubform.cpp 
# 	   asientolistsubform.cpp 
# 	   balancesubform.cpp 
# 	   busquedaccoste.cpp 
# 	   busquedacanal.cpp 
# 	   fichabc.cpp)
# 
# set(bulmacont_MOCS bulmacont.h 
#            sobreview.h 
# #           empresa.h 
#            listcuentasview1.h 
#            cuentaview.h 
#            diarioview.h 
# #           qlineedit1.h 
#            extractoview1.h 
#            balance1view.h 
# #           qlistviewitem1.h 
#            balanceview.h 
#            aplinteligentesview.h 
#            asientosview.h 
#            regivaprintview.h 
#            mpatrimonialview.h 
#            mpatrimonialesview.h 
#            balanceprintview.h 
#            canualesprintview.h 
#            canualesview.h 
#            canalview.h 
#            ccosteview.h 
#            propiedadesempresa.h 
#            extractoprintview.h 
# #           libromayorprint.h 
# #           diarioprint.h 
#            diarioprintview.h 
#            cambiactaview.h 
#            duplicarasientoview.h 
#            amortizacionview.h 
#            listado347.h 
#            amortizacionesview.h 
#            selectccosteview.h 
#            selectcanalview.h 
# #           modelosps.h 
#            modelo300.h 
# #           importainteligente.h 
# #           importbalance.h 
#            bbloqfecha.h 
#            tipoivaview.h 
#            fpagoview.h 
# #           arbol.h 
# 	   busquedacuenta.h 
# 	   asiento1.h 
# 	   asiento1view.h 
# 	   listlinasiento1view.h 
# 	   subform2bc.h 
# 	   diariosubform.h 
# 	   extractosubform.h 
# 	   asientolistsubform.h 
# 	   balancesubform.h 
# 	   busquedaccoste.h 
# 	   busquedacanal.h 
# 	   fichabc.h)
# 
# #set(bulmacont_RCCS ../../bulmages.qrc)
# set(bulmacont_UIS bulmacontbase.ui 
# 	 diariobase.ui 
# 	 extractobase.ui 
# 	 extractoprintbase.ui 
# 	 diarioprintbase.ui 
# 	 asientobase.ui 
# 	 asientolistbase.ui 
# 	 balancebase.ui 
# 	 balancetreebase.ui 
# 	 cuentabase.ui 
# 	 listcuentasbase.ui 
# 	 sobrebase.ui 
# 	 amortizacionesbase.ui 
# 	 amortizacionbase.ui 
# 	 busquedacuentabase.ui 
# 	 aplinteligentesbase.ui 
# 	 canualesbase.ui 
# 	 ccostebase.ui 
# 	 canalbase.ui 
# 	 fpagobase.ui 
# 	 tipoivabase.ui 
# 	 bbloqfechabase.ui 
# 	 duplicarasientobase.ui 
# 	 cambiactabase.ui 
# 	 selectcanalbase.ui 
# 	 selectccostebase.ui 
# 	 propiedadesempresabase.ui 
# 	 canualesprintbase.ui 
# 	 regivaprintbase.ui 
# 	 balanceprintbase.ui 
# 	 mpatrimonialesbase.ui 
# 	 mpatrimonialbase.ui 
# 	 listado347base.ui 
# 	 modelo300base.ui)
# 
# include_directories(
#     ${QT_INCLUDE_DIR}
# 	${PGSQL_INCLUDE_DIR}
# 	${QT_QTXML_INCLUDE_DIR}
# 	${CMAKE_CURRENT_BINARY_DIR}
# 	${CMAKE_CURRENT_SOURCE_DIR}
# 	${QT_QTASSISTANT_INCLUDE_DIR}
# 	${CMAKE_CURRENT_BINARY_DIR}
# 	${CMAKE_CURRENT_BINARY_DIR}/../../bulmalib/src/
# 	${CMAKE_SOURCE_DIR}/bulmalib/src
# 
# #  PGSQL_LIBRARIES   - List of libraries when using PGSQL.
# #  PGSQL_FOUND       -
# )
# 
# # generate rules for building source files from the resources
# QT4_WRAP_UI(bulmacont_UIS_H ${bulmacont_UIS})
# QT4_WRAP_CPP(bulmacont_MOC_SRCS ${bulmacont_MOCS})
# QT4_ADD_RESOURCES(bulmacont_RCC_SRCS ${bulmacont_RCCS})
# 
# # generate rules for building source files that moc generates
# 
# ADD_DEFINITIONS(${QT_DEFINITIONS})
# ADD_DEFINITIONS(-DQT_SHARED)
# # build sources, moc'd sources, and rcc'd sources
# add_executable(bulmacont  ${bulmacont_MOC_SRCS} ${bulmacont_UIS_H}
# 	${bulmacont_SRCS} ${bulmacont_MOCS} ${bulmacont_RCC_SRCS})
# 
# target_link_libraries( bulmacont bulmalib ${QT_LIBRARIES}
# 	${QT_QTGUI_LIBRARY} ${PGSQL_LIBRARIES} 	${QT_QTASSISTANT_LIBRARY}
# 	${QT_QTSQL_LIBRARY})
# 
# 
# 
