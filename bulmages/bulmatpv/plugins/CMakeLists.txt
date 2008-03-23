add_subdirectory(pluginticketbasico)
add_subdirectory(plugintecladonumerico)
add_subdirectory(plugintotal)
add_subdirectory(pluginartgraficos)

#pluginagrevs en desarrollo
#add_subdirectory(pluginabrevs)

add_subdirectory(pluginadmin)
add_subdirectory(plugincobrar)

# if you want to add some private code
# simple put your add_directory in a
# CMakeLists.txt.private

if(EXISTS "${CMAKE_SOURCE_DIR}/bulmatpv/plugins/CMakeLists.txt.private")
   message(STATUS "Private code inserted in bulmatpv plugins")
   include("${CMAKE_SOURCE_DIR}/bulmatpv/plugins/CMakeLists.txt.private")
endif(EXISTS "${CMAKE_SOURCE_DIR}/bulmatpv/plugins/CMakeLists.txt.private")
