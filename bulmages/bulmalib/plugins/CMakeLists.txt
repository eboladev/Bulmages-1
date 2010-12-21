
add_subdirectory(pluginbl_subform2sxc)
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

if(NOT WIN32)
if(NOT APPLE)
add_subdirectory(pluginbl_embed)
endif(NOT APPLE)
endif(NOT WIN32)



if(EXISTS "${CMAKE_SOURCE_DIR}/bulmalib/plugins/CMakeLists.txt.private")
   message(STATUS "Private code inserted in bulmalib plugins")
   include("${CMAKE_SOURCE_DIR}/bulmalib/plugins/CMakeLists.txt.private")
endif(EXISTS "${CMAKE_SOURCE_DIR}/bulmalib/plugins/CMakeLists.txt.private")

