set(VERSION 1.0.0)

set(TRANSLATIONS pluginsubformods_es.ts 
                pluginsubformods_ca.ts 
                pluginsubformods_en.ts 
                pluginsubformods_fr.ts 
                pluginsubformods_de.ts 
                pluginsubformods_zz.ts) 

INCLUDE(   ${QT_USE_FILE}   )
set(VERSION 1.0.0)
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

# generate rules for building source files from the resources
#QT4_WRAP_CPP(plugin_MOC_SRCS ${plugin_MOCS})

# generate rules for building source files that moc generates

ADD_DEFINITIONS(${QT_DEFINITIONS})
ADD_DEFINITIONS(-DQT_SHARED)
# build sources, moc'd sources, and rcc'd sources
add_library(pluginsubformods SHARED ${plugin_SRC})
target_link_libraries(pluginsubformods bulmalib ${QT_LIBRARIES}  ${QT_QTGUI_LIBRARY} ${PGSQL_LIBRARIES})

