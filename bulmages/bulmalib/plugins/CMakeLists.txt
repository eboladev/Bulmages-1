
#add_subdirectory(pluginbl_subform2sxc)
add_subdirectory(pluginbl_subform2ods)
add_subdirectory(pluginbl_clipboard)
add_subdirectory(pluginbl_template)
add_subdirectory(pluginbl_template2ods)
add_subdirectory(pluginbl_template2mail)
add_subdirectory(pluginbl_importcsv)
add_subdirectory(pluginbl_report)
add_subdirectory(pluginbl_report2ods)
add_subdirectory(pluginbl_formlock)
add_subdirectory(pluginbl_subform2pdf)
add_subdirectory(pluginbl_attachdocument)
add_subdirectory(pluginbl_bloqconf)
add_subdirectory(pluginbl_logo)
#add_subdirectory(pluginbl_backup)
add_subdirectory(pluginbl_feeds)
add_subdirectory(pluginbl_autoform)


if(NOT WIN32)
if(NOT APPLE)
# En Qt5 ya no existen las funciones de embebido.
#add_subdirectory(pluginbl_embed)
#exaro esta dando actualmente problemas de compilacion y esta huerfano. Por este motivo esta comentado.
#add_subdirectory(pluginbl_exaro)
endif(NOT APPLE)
endif(NOT WIN32)


# if you want to add some private code
# simple put your add_directory in a
# CMakeLists.txt.private


if(EXISTS "${CMAKE_SOURCE_DIR}/bulmalib/plugins/CMakeLists.txt.private")
   message(STATUS "Private code inserted in bulmalib plugins")
   include("${CMAKE_SOURCE_DIR}/bulmalib/plugins/CMakeLists.txt.private")
endif(EXISTS "${CMAKE_SOURCE_DIR}/bulmalib/plugins/CMakeLists.txt.private")

