add_subdirectory(pluginticketbasico)
add_subdirectory(plugintecladonumerico)
add_subdirectory(plugintotal)
add_subdirectory(pluginartgraficos)
add_subdirectory(pluginadmin)
add_subdirectory(plugincobrar)
add_subdirectory(pluginartsubform)
add_subdirectory(pluginaliastpv)
add_subdirectory(plugintcaliastpv)
add_subdirectory(pluginticketdesglose)
add_subdirectory(pluginbuscaarticulo)
add_subdirectory(pluginvisor)
add_subdirectory(pluginbuscacliente)
add_subdirectory(plugincambio)
add_subdirectory(plugindevolucion)
add_subdirectory(plugindevolucion2)

if(NOT APPLE)
add_subdirectory(pluginabrevs)
endif(NOT APPLE)


# if you want to add some private code
# simple put your add_directory in a
# CMakeLists.txt.private

if(EXISTS "${CMAKE_SOURCE_DIR}/bulmatpv/plugins/CMakeLists.txt.private")
   message(STATUS "Private code inserted in bulmatpv plugins")
   include("${CMAKE_SOURCE_DIR}/bulmatpv/plugins/CMakeLists.txt.private")
endif(EXISTS "${CMAKE_SOURCE_DIR}/bulmatpv/plugins/CMakeLists.txt.private")
