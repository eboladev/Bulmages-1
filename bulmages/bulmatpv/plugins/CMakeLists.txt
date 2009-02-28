add_subdirectory(pluginbt_ticket)
add_subdirectory(pluginbt_ticketivaincluido)
add_subdirectory(pluginbt_tecladonumerico)
add_subdirectory(pluginbt_total)
add_subdirectory(pluginbt_totalivaincluido)
add_subdirectory(pluginbt_articulografico)
add_subdirectory(pluginadmin)
add_subdirectory(pluginbt_cobrar)
add_subdirectory(pluginbt_articulosubform)
add_subdirectory(plugintpvivainc)

add_subdirectory(pluginalias1)

add_subdirectory(pluginaliastpv)
add_subdirectory(plugintcaliastpv)
add_subdirectory(pluginticketdesglose)
add_subdirectory(pluginbt_buscaarticulo)
add_subdirectory(pluginbt_visor)
add_subdirectory(pluginbt_buscacliente)
add_subdirectory(pluginbt_cambio)
add_subdirectory(pluginbt_cambioivaincluido)
add_subdirectory(plugindevolucion)
add_subdirectory(plugindevolucion2)
add_subdirectory(pluginticketsdevueltos)


if(NOT APPLE)
add_subdirectory(pluginbt_accesobotones)
add_subdirectory(plugintccompras)
endif(NOT APPLE)


# if you want to add some private code
# simple put your add_directory in a
# CMakeLists.txt.private

if(EXISTS "${CMAKE_SOURCE_DIR}/bulmatpv/plugins/CMakeLists.txt.private")
   message(STATUS "Private code inserted in bulmatpv plugins")
   include("${CMAKE_SOURCE_DIR}/bulmatpv/plugins/CMakeLists.txt.private")
endif(EXISTS "${CMAKE_SOURCE_DIR}/bulmatpv/plugins/CMakeLists.txt.private")
