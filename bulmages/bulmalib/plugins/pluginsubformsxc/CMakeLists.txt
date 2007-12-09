#
set(TRANSLATIONS pluginsubformsxc_es.ts 
                pluginsubformsxc_ca.ts 
                pluginsubformsxc_en.ts 
                pluginsubformsxc_fr.ts 
                pluginsubformsxc_de.ts 
                pluginsubformsxc_zz.ts )

INCLUDE(   ${QT_USE_FILE}   )

set(plugin_SRC pluginsubformsxc.cpp)

include_directories(
    ${QT_INCLUDE_DIR}
	${PGSQL_INCLUDE_DIR}
	${QT_QTXML_INCLUDE_DIR}
	${CMAKE_CURRENT_BINARY_DIR}
	${CMAKE_CURRENT_SOURCE_DIR}
	${CMAKE_CURRENT_BINARY_DIR}/../../src/
	${CMAKE_SOURCE_DIR}/bulmalib/src
#  PGSQL_LIBRARIES   - List of libraries when using PGSQL.
#  PGSQL_FOUND       -
)

# generate rules for building source files from the resources
#QT4_WRAP_CPP(plugin_MOC_SRCS ${plugin_MOCS})

# generate rules for building source files that moc generates

ADD_DEFINITIONS(${QT_DEFINITIONS})
ADD_DEFINITIONS(-DQT_SHARED)
# build sources, moc'd sources, and rcc'd sources

add_library(pluginsubformsxc SHARED ${plugin_SRC})

set_target_properties( pluginsubformsxc PROPERTIES 
  SOVERSION "${BULMAGES_VERSION_MAJOR}.${BULMAGES_VERSION_MINOR}"
  VERSION "${BULMAGES_VERSION}")

target_link_libraries(pluginsubformsxc bulmalib ${QT_LIBRARIES}  ${QT_QTGUI_LIBRARY} ${PGSQL_LIBRARIES})








