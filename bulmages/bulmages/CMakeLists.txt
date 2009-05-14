PROJECT(BgBulmaGes)

add_subdirectory(src)

# if you want to add some private code
# simple put your add_directory in a
# CMakeLists.txt.private

if(EXISTS "${CMAKE_SOURCE_DIR}/bulmages/CMakeLists.txt.private")
   message(STATUS "Private code inserted in bulmages")
   include("${CMAKE_SOURCE_DIR}/bulmages/CMakeLists.txt.private")
endif(EXISTS "${CMAKE_SOURCE_DIR}/bulmages/CMakeLists.txt.private")
