add_subdirectory(pluginbf_example)
add_subdirectory(pluginbf_tarifa)
add_subdirectory(pluginbf_comercial)
add_subdirectory(pluginbf_cliente)
add_subdirectory(pluginbf_proveedor)
add_subdirectory(pluginbf_articulo)
add_subdirectory(pluginbf_trabajador)
add_subdirectory(pluginbf_iva)
add_subdirectory(pluginbf_almacenamplia)
add_subdirectory(pluginbf_banco)
add_subdirectory(pluginbf_formasdepago)
add_subdirectory(pluginbf_seriesfactura)
add_subdirectory(pluginbf_almacen)
add_subdirectory(pluginbf_modificadores)
add_subdirectory(pluginbf_carteracobros)
add_subdirectory(pluginbf_carterapagos)

#efacturabf en desarrollo
#add_subdirectory(pluginbf_efactura)

#pluginvehiculosbf en desarrollo
#add_subdirectory(pluginbf_vehiculo)

add_subdirectory(pluginbf_trazabilidad)
add_subdirectory(pluginbf_abrecodigobarras)
add_subdirectory(pluginbf_catalogo)
add_subdirectory(pluginbf_informecliente)
add_subdirectory(pluginbf_inventario)
add_subdirectory(pluginbf_contrato)
add_subdirectory(pluginbf_ticket)

#plugin_tc_articulos en desarrollo
add_subdirectory(pluginbf_tallascolores)


add_subdirectory(pluginbf_promedios)
add_subdirectory(pluginbf_multiimprimir)
add_subdirectory(pluginbf_q19)
add_subdirectory(pluginbf_correo)


add_subdirectory(pluginbf_tipotrabajo)
add_subdirectory(pluginbf_cuadrante)
add_subdirectory(pluginbf_asterisk)
add_subdirectory(pluginbf_bulmatpv)
add_subdirectory(pluginbf_debug)
add_subdirectory(pluginbf_preciocoste)
add_subdirectory(pluginbf_resumenarticulo)
add_subdirectory(pluginbf_alias)
add_subdirectory(pluginbf_datosbase)

add_subdirectory(pluginbf_ivaincluido)
add_subdirectory(pluginbf_printercocina)

add_subdirectory(pluginbf_camarero)

#add_subdirectory(pluginbf_bulmacont)

#add_subdirectory(pluginbf_sincrobulmacont)
add_subdirectory(pluginbf_informecliente2sxc)
add_subdirectory(pluginbf_etiquetado)
add_subdirectory(pluginbf_etiquetas)
add_subdirectory(pluginbf_compraventa)
add_subdirectory(pluginbf_clienteproveedor)
add_subdirectory(pluginbf_abrereferencia)

add_subdirectory(pluginbf_corrector)
add_subdirectory(pluginbf_clientealbaranesfactura)

add_subdirectory(pluginbf_proveedorpago)
add_subdirectory(pluginbf_clientecobro)
add_subdirectory(pluginbf_clientepresupuesto)
add_subdirectory(pluginbf_clientepedido)
add_subdirectory(pluginbf_clientealbaran)
add_subdirectory(pluginbf_clientefactura)
add_subdirectory(pluginbf_clienteticket)
add_subdirectory(pluginbf_clientefacturaivainc)

add_subdirectory(pluginbf_proveedorfactura)
add_subdirectory(pluginbf_proveedoralbaran)
add_subdirectory(pluginbf_proveedorpedido)

add_subdirectory(pluginbf_proveedorcodigos)

#pluginqwebcam depende de opencv y esta en desarrollo
#add_subdirectory(pluginbf_qwebcam)

add_subdirectory(pluginbf_z2z)

# add_subdirectory(pluginbf_printercocina)

# if you want to add some private code
# simple put your add_directory in a
# CMakeLists.txt.private


# ======= PLUGINS DE CONTABILIDAD INTEGRADA
add_subdirectory(pluginbf_factcont)
add_subdirectory(pluginbf_cuenta)
add_subdirectory(pluginbf_asiento)
add_subdirectory(pluginbf_bloqueofechas)
add_subdirectory(pluginbf_diario)
add_subdirectory(pluginbf_mayor)
add_subdirectory(pluginbf_balance)
add_subdirectory(pluginbf_ccaa2ods)
#add_subdirectory(pluginbf_cuentaresumen)


# ======== PLUGINS DE GESTION ALUMNOS
add_subdirectory(pluginbf_profesor)
add_subdirectory(pluginbf_alumno)
add_subdirectory(pluginbf_tutor)
add_subdirectory(pluginbf_actividad)
add_subdirectory(pluginbf_recibo)
add_subdirectory(pluginbf_asociacion)
add_subdirectory(pluginbf_minicontabilidad)
add_subdirectory(pluginbf_inventariosimple)


# ========= PLUGINS DE AUTOFORMS
add_subdirectory(pluginbf_biblioteca)
add_subdirectory(pluginbf_alquileres)


if(EXISTS "${CMAKE_SOURCE_DIR}/bulmafact/plugins/CMakeLists.txt.private")
   message(STATUS "Private code inserted in bulmafact plugins")
   include("${CMAKE_SOURCE_DIR}/bulmafact/plugins/CMakeLists.txt.private")
endif(EXISTS "${CMAKE_SOURCE_DIR}/bulmafact/plugins/CMakeLists.txt.private")
