###########################################
# Plugin for bulmafact					  #
###########################################
#pluginalmacen

set (plugin pluginalmacen)

file(GLOB plugin_SRC *.c *.cpp *.cxx)
file(GLOB plugin_MOCS *.hpp *.h)
file(GLOB plugin_UIS *.ui)


INCLUDE(   ${QT_USE_FILE}   )
set(QT_USE_QTASSISTANT true)
set(QT_USE_QTXML true)

include_directories(
    ${QT_INCLUDE_DIR}
	${PGSQL_INCLUDE_DIR}
	${QT_QTXML_INCLUDE_DIR}
	${QT_QTASSISTANT_INCLUDE_DIR}
	${CMAKE_CURRENT_BINARY_DIR}
	${CMAKE_CURRENT_SOURCE_DIR}
	${CMAKE_BINARY_DIR}/bulmalib/src/
	${CMAKE_SOURCE_DIR}/bulmalib/src/
	${CMAKE_BINARY_DIR}/bulmafact/src/
	${CMAKE_SOURCE_DIR}/bulmafact/src/)

# generate rules for building source files from the resources
#QT4_WRAP_CPP(plugin_MOC_SRCS ${plugin_MOCS})

# generate rules for building source files that moc generates
QT4_WRAP_UI(plugin_UIS_H ${plugin_UIS})
QT4_WRAP_CPP(plugin_MOC_SRCS ${plugin_MOCS})
#QT4_ADD_RESOURCES(plugin_RCC_SRCS ${plugin_RCCS})


ADD_DEFINITIONS(${QT_DEFINITIONS})
ADD_DEFINITIONS(-DQT_SHARED)
# build sources, moc'd sources, and rcc'd sources

add_library(${plugin} SHARED ${plugin_SRC} ${plugin_MOC_SRCS}
	${plugin_UIS_H} ${plugin_SRCS} ${plugin_MOCS})


set_target_properties( ${plugin} PROPERTIES 
  SOVERSION "${BULMAGES_VERSION_MAJOR}.${BULMAGES_VERSION_MINOR}"
  VERSION "${BULMAGES_VERSION}")

target_link_libraries(${plugin} bulmalib ${QT_LIBRARIES}  ${QT_QTGUI_LIBRARY} ${PGSQL_LIBRARIES} ${QT_QTASSISTANT_LIBRARY})
install_targets(/lib/bulmafact/plugins ${plugin})
