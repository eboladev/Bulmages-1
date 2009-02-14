add_subdirectory(pluginbf_example)
add_subdirectory(plugintarifas)
add_subdirectory(pluginbf_comercial)
add_subdirectory(pluginbf_cliente)
add_subdirectory(pluginbf_proveedor)
add_subdirectory(pluginbf_articulo)
add_subdirectory(pluginbf_trabajador)

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
add_subdirectory(plugin_tc_articulos)

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

add_subdirectory(pluginivainc)

add_subdirectory(pluginbulmacont)

add_subdirectory(pluginsyncbulmacont)
add_subdirectory(plugininformeclientessxc)
add_subdirectory(pluginetiquetado)
add_subdirectory(pluginetiquetas)
add_subdirectory(plugincompraventa)
add_subdirectory(pluginclientprov)
add_subdirectory(pluginopenref)

add_subdirectory(plugincorrectorbf)
add_subdirectory(pluginfacturar)

add_subdirectory(pluginpagos)
add_subdirectory(plugincobros)
add_subdirectory(pluginpresupuesto)
add_subdirectory(pluginpedido)
add_subdirectory(pluginalbaran)
add_subdirectory(pluginfactura)

add_subdirectory(pluginfacturaprov)
add_subdirectory(pluginalbaranprov)
add_subdirectory(pluginpedidoprov)

#pluginqwebcam depende de opencv y esta en desarrollo
#add_subdirectory(pluginqwebcam)

add_subdirectory(pluginz2z)

# if you want to add some private code
# simple put your add_directory in a
# CMakeLists.txt.private

# ======== PLUGINS DE GESTION ALUMNOS
add_subdirectory(pluginprofesor)
add_subdirectory(pluginalumno)
add_subdirectory(plugintutor)
add_subdirectory(pluginactividad)


if(EXISTS "${CMAKE_SOURCE_DIR}/bulmafact/plugins/CMakeLists.txt.private")
   message(STATUS "Private code inserted in bulmafact plugins")
   include("${CMAKE_SOURCE_DIR}/bulmafact/plugins/CMakeLists.txt.private")
endif(EXISTS "${CMAKE_SOURCE_DIR}/bulmafact/plugins/CMakeLists.txt.private")
