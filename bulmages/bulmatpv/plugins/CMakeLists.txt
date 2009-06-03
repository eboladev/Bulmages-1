add_subdirectory(pluginbt_ticket)
add_subdirectory(pluginbt_ticketivaincluido)
add_subdirectory(pluginbt_tecladonumerico)
add_subdirectory(pluginbt_total)
add_subdirectory(pluginbt_totalivaincluido)
add_subdirectory(pluginbt_articulografico)
add_subdirectory(pluginbt_articulograficodb)
add_subdirectory(pluginbt_controlcaja)
add_subdirectory(pluginbt_cobrar)
add_subdirectory(pluginbt_articulosubform)
add_subdirectory(pluginbt_ivaincluido)
add_subdirectory(pluginbt_alias)
add_subdirectory(pluginbt_aliastallasycolores2)
add_subdirectory(pluginbt_aliastallasycolores)
add_subdirectory(pluginbt_ticketdesglose)
add_subdirectory(pluginbt_buscaarticulo)
add_subdirectory(pluginbt_visor)
add_subdirectory(pluginbt_buscacliente)
add_subdirectory(pluginbt_cambio)
add_subdirectory(pluginbt_cambioivaincluido)
add_subdirectory(pluginbt_devolucion)
add_subdirectory(pluginbt_devolucion2)
add_subdirectory(pluginbt_ticketdevuelto)


if(NOT APPLE)
add_subdirectory(pluginbt_accesobotones)
add_subdirectory(pluginbt_stockcompras)
endif(NOT APPLE)


# if you want to add some private code
# simple put your add_directory in a
# CMakeLists.txt.private

if(EXISTS "${CMAKE_SOURCE_DIR}/bulmatpv/plugins/CMakeLists.txt.private")
   message(STATUS "Private code inserted in bulmatpv plugins")
   include("${CMAKE_SOURCE_DIR}/bulmatpv/plugins/CMakeLists.txt.private")
endif(EXISTS "${CMAKE_SOURCE_DIR}/bulmatpv/plugins/CMakeLists.txt.private")
