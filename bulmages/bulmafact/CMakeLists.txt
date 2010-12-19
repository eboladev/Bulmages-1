PROJECT(bulmafact)

add_subdirectory(src)
add_subdirectory(plugins)
add_subdirectory(designer)


# if you want to add some private code
# simple put your add_directory in a
# CMakeLists.txt.private


SET(QT_USE_QTUITOOLS TRUE)
SET(QT_USE_QTXML TRUE) # uitools depends on xml
set(QT_USE_QTSCRIPT true) #by R. Cabezas



if(EXISTS "${CMAKE_SOURCE_DIR}/bulmafact/CMakeLists.txt.private")
   message(STATUS "Private code inserted in bulmafact")
   include("${CMAKE_SOURCE_DIR}/bulmafact/CMakeLists.txt.private")
endif(EXISTS "${CMAKE_SOURCE_DIR}/bulmafact/CMakeLists.txt.private")

