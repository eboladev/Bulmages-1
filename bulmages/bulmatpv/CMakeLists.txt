PROJECT(bulmatpv)

cmake_policy(SET CMP0020 OLD)

add_subdirectory(src)
add_subdirectory(plugins)


# if you want to add some private code
# simple put your add_directory in a
# CMakeLists.txt.private

if(EXISTS "${CMAKE_SOURCE_DIR}/bulmatpv/CMakeLists.txt.private")
   message(STATUS "Private code inserted in bulmatpv")
   include("${CMAKE_SOURCE_DIR}/bulmatpv/CMakeLists.txt.private")
endif(EXISTS "${CMAKE_SOURCE_DIR}/bulmatpv/CMakeLists.txt.private")
