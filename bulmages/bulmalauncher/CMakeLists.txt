PROJECT(BulmaLauncher)

add_subdirectory(src)

# if you want to add some private code
# simple put your add_directory in a
# CMakeLists.txt.private

if(EXISTS "${CMAKE_SOURCE_DIR}/bulmalauncher/CMakeLists.txt.private")
   message(STATUS "Private code inserted in bulmalauncher")
   include("${CMAKE_SOURCE_DIR}/bulmalauncher/CMakeLists.txt.private")
endif(EXISTS "${CMAKE_SOURCE_DIR}/bulmalauncher/CMakeLists.txt.private")
