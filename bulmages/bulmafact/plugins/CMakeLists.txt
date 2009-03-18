add_subdirectory(pluginbf_example)
add_subdirectory(pluginbf_tarifa)
add_subdirectory(pluginbf_comercial)
add_subdirectory(pluginbf_cliente)
add_subdirectory(pluginbf_proveedor)
add_subdirectory(pluginbf_articulo)
add_subdirectory(pluginbf_trabajador)
add_subdirectory(pluginbf_iva)
add_subdirectory(pluginalmacen)
add_subdirectory(pluginbf_banco)
add_subdirectory(pluginbf_formasdepago)
add_subdirectory(pluginbf_seriesfactura)
add_subdirectory(pluginbf_almacen)


#efacturabf en desarrollo
#add_subdirectory(efacturabf)

#pluginvehiculosbf en desarrollo
#add_subdirectory(pluginbf_vehiculo)

add_subdirectory(pluginbf_trazabilidad)
add_subdirectory(pluginbarcodeopen)
add_subdirectory(plugincatalogo)
add_subdirectory(plugininformeclientes)
add_subdirectory(plugininventario)
add_subdirectory(plugincontratos)
add_subdirectory(pluginbf_ticket)

#plugin_tc_articulos en desarrollo
add_subdirectory(pluginbf_tallascolores)

add_subdirectory(pluginpromedios)
add_subdirectory(pluginimpresionesmultiples)
add_subdirectory(pluginq19)
add_subdirectory(pluginmail)
add_subdirectory(pluginmailthunderbird)
add_subdirectory(plugintipostrabajo)
add_subdirectory(plugincuadrante)
add_subdirectory(pluginbf_asterisk)
add_subdirectory(pluginbf_bulmatpv)
add_subdirectory(pluginbf_debug)
add_subdirectory(pluginbf_preciocoste)
add_subdirectory(pluginresarticulos)
add_subdirectory(pluginalias)
add_subdirectory(plugindatosbase)

add_subdirectory(pluginbf_ivaincluido)

add_subdirectory(pluginbf_bulmacont)

add_subdirectory(pluginsyncbulmacont)
add_subdirectory(plugininformeclientessxc)
add_subdirectory(pluginetiquetado)
add_subdirectory(pluginetiquetas)
add_subdirectory(plugincompraventa)
add_subdirectory(pluginbf_clienteproveedor)
add_subdirectory(pluginopenref)

add_subdirectory(pluginbf_corrector)
add_subdirectory(pluginbf_clientealbaranesfactura)

add_subdirectory(pluginbf_proveedorpago)
add_subdirectory(pluginbf_clientecobro)
add_subdirectory(pluginbf_clientepresupuesto)
add_subdirectory(pluginbf_clientepedido)
add_subdirectory(pluginbf_clientealbaran)
add_subdirectory(pluginbf_clientefactura)

add_subdirectory(pluginbf_proveedorfactura)
add_subdirectory(pluginbf_proveedoralbaran)
add_subdirectory(pluginbf_proveedorpedido)

#pluginqwebcam depende de opencv y esta en desarrollo
#add_subdirectory(pluginqwebcam)

add_subdirectory(pluginz2z)

# if you want to add some private code
# simple put your add_directory in a
# CMakeLists.txt.private

# ======== PLUGINS DE GESTION ALUMNOS
add_subdirectory(pluginbf_profesor)
add_subdirectory(pluginbf_alumno)
add_subdirectory(pluginbf_tutor)
add_subdirectory(pluginbf_actividad)
add_subdirectory(pluginbf_recibo)

if(EXISTS "${CMAKE_SOURCE_DIR}/bulmafact/plugins/CMakeLists.txt.private")
   message(STATUS "Private code inserted in bulmafact plugins")
   include("${CMAKE_SOURCE_DIR}/bulmafact/plugins/CMakeLists.txt.private")
endif(EXISTS "${CMAKE_SOURCE_DIR}/bulmafact/plugins/CMakeLists.txt.private")
