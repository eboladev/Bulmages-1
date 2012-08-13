PROJECT(bulmafact)

FILE(READ css/bulmafact_es_ES.css VAR_CSS_es_ES)
FILE(APPEND ${CMAKE_BINARY_DIR}/css/bulmafact_es_ES.css "${VAR_CSS_es_ES}" )

FILE(READ css/bulmafact_ca_ES.css VAR_CSS_ca_ES)
FILE(APPEND ${CMAKE_BINARY_DIR}/css/bulmafact_ca_ES.css "${VAR_CSS_ca_ES}" )

FILE(READ css/bulmafact_fr_FR.css VAR_CSS_fr_FR)
FILE(APPEND ${CMAKE_BINARY_DIR}/css/bulmafact_fr_FR.css "${VAR_CSS_fr_FR}" )

FILE(READ css/bulmafact_en_EN.css VAR_CSS_en_EN)
FILE(APPEND ${CMAKE_BINARY_DIR}/css/bulmafact_en_EN.css "${VAR_CSS_en_EN}" )

add_subdirectory(src)
add_subdirectory(plugins)
add_subdirectory(designer)


set(QT_USE_QTUITOOLS true)
set(QT_USE_QTXML true) # uitools depends on xml
set(QT_USE_QTSCRIPT true)
set(QT_USE_QTWEBKIT true)


# if you want to add some private code
# simple put your add_directory in a
# CMakeLists.txt.private


if(EXISTS "${CMAKE_SOURCE_DIR}/bulmafact/CMakeLists.txt.private")
   message(STATUS "Private code inserted in bulmafact")
   include("${CMAKE_SOURCE_DIR}/bulmafact/CMakeLists.txt.private")
endif(EXISTS "${CMAKE_SOURCE_DIR}/bulmafact/CMakeLists.txt.private")

