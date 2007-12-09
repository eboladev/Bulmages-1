set(TRANSLATIONS pluginsubformods_es.ts 
                pluginsubformods_ca.ts 
                pluginsubformods_en.ts 
                pluginsubformods_fr.ts 
                pluginsubformods_de.ts 
                pluginsubformods_zz.ts) 

INCLUDE(   ${QT_USE_FILE}   )
set(plugin_SRC pluginsubformods.cpp)

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


ADD_DEFINITIONS(${QT_DEFINITIONS})
ADD_DEFINITIONS(-DQT_SHARED)
# build sources, moc'd sources, and rcc'd sources
add_library(pluginsubformods SHARED ${plugin_SRC})

set_target_properties( pluginsubformods PROPERTIES 
  SOVERSION "${BULMAGES_VERSION_MAJOR}.${BULMAGES_VERSION_MINOR}"
  VERSION "${BULMAGES_VERSION}")

target_link_libraries(pluginsubformods bulmalib ${QT_LIBRARIES}  ${QT_QTGUI_LIBRARY} ${PGSQL_LIBRARIES})

