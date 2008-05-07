PROJECT(Bulmafact)

add_subdirectory(src)

add_subdirectory(plugins)

# if you want to add some private code
# simple put your add_directory in a
# CMakeLists.txt.private
 

if(EXISTS "${CMAKE_SOURCE_DIR}/bulmafact/CMakeLists.txt.private")
   message(STATUS "Private code inserted in bulmafact")
   include("${CMAKE_SOURCE_DIR}/bulmafact/CMakeLists.txt.private")
endif(EXISTS "${CMAKE_SOURCE_DIR}/bulmafact/CMakeLists.txt.private")

