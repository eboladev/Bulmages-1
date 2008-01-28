add_subdirectory(pluginregistroiva)
add_subdirectory(pluginadocumental)
add_subdirectory(plugincorrector)
add_subdirectory(plugin)
add_subdirectory(plugincanualesods)
add_subdirectory(plugindebugbc)
add_subdirectory(pluginproyectos)
add_subdirectory(plugincaaslods)

# if you want to add some private code
# simple put your add_directory in a
# CMakeLists.txt.private

if(EXISTS "${CMAKE_SOURCE_DIR}/bulmacont/plugins/CMakeLists.txt.private")
   message(STATUS "Private code inserted in bulmacont plugins")
   include("${CMAKE_SOURCE_DIR}/bulmacont/plugins/CMakeLists.txt.private")
endif(EXISTS "${CMAKE_SOURCE_DIR}/bulmacont/plugins/CMakeLists.txt.private")
