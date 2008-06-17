add_subdirectory(plugin)
add_subdirectory(plugintarifas)
add_subdirectory(comercialbf)

#efacturabf en desarrollo
#add_subdirectory(efacturabf)

#pluginvehiculosbf en desarrollo
#add_subdirectory(pluginvehiculosbf)

add_subdirectory(plugintrazabilidad)
add_subdirectory(pluginbarcodeopen)
add_subdirectory(plugincatalogo)
add_subdirectory(plugininformeclientes)
add_subdirectory(plugininventario)
add_subdirectory(plugincontratos)
add_subdirectory(pluginticket)

#plugin_tc_articulos en desarrollo
#add_subdirectory(plugin_tc_articulos)

add_subdirectory(pluginpromedios)
add_subdirectory(pluginimpresionesmultiples)
add_subdirectory(pluginq19)
add_subdirectory(pluginmail)
add_subdirectory(pluginmailthunderbird)
add_subdirectory(pluginalmacen)
add_subdirectory(plugintipostrabajo)
add_subdirectory(plugincuadrante)
add_subdirectory(pluginasterisk)
add_subdirectory(plugintpv)
add_subdirectory(plugindebugbf)
add_subdirectory(pluginpreciocoste)
add_subdirectory(pluginresarticulos)
add_subdirectory(pluginalias)
add_subdirectory(plugindatosbase)

#pluginivainc en desarrollo
#add_subdirectory(pluginivainc)

add_subdirectory(pluginsyncbulmacont)
add_subdirectory(plugininformeclientessxc)
add_subdirectory(pluginetiquetado)

#pluginqwebcam depende de opencv y esta en desarrollo
#add_subdirectory(pluginqwebcam)

# if you want to add some private code
# simple put your add_directory in a
# CMakeLists.txt.private

if(EXISTS "${CMAKE_SOURCE_DIR}/bulmafact/plugins/CMakeLists.txt.private")
   message(STATUS "Private code inserted in bulmafact plugins")
   include("${CMAKE_SOURCE_DIR}/bulmafact/plugins/CMakeLists.txt.private")
endif(EXISTS "${CMAKE_SOURCE_DIR}/bulmafact/plugins/CMakeLists.txt.private")
