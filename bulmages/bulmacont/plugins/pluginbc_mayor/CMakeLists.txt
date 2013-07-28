###########################################
# Plugin for bulmacont                    #
###########################################
#pluginbc_mayor

set (plugin pluginbc_mayor)

#preproceso de las plantillas CSS
configure_file( ${CMAKE_CURRENT_SOURCE_DIR}/css/${plugin}_es_ES.css.in ${CMAKE_BINARY_DIR}/bulmacont/plugins/${plugin}/${plugin}_es_ES.css @ONLY)
configure_file( ${CMAKE_CURRENT_SOURCE_DIR}/css/${plugin}_ca_ES.css.in ${CMAKE_BINARY_DIR}/bulmacont/plugins/${plugin}/${plugin}_ca_ES.css @ONLY)
configure_file( ${CMAKE_CURRENT_SOURCE_DIR}/css/${plugin}_en_US.css.in ${CMAKE_BINARY_DIR}/bulmacont/plugins/${plugin}/${plugin}_en_US.css @ONLY)
configure_file( ${CMAKE_CURRENT_SOURCE_DIR}/css/${plugin}_fr_FR.css.in ${CMAKE_BINARY_DIR}/bulmacont/plugins/${plugin}/${plugin}_fr_FR.css @ONLY)


file(GLOB plugin_SRC *.c *.cpp *.cxx)
file(GLOB plugin_MOCS *.hpp *.h)
file(GLOB plugin_UIS *.ui)


# Exclude files with no QObject macro.
file(GLOB plugin_NO_MOCS pdefs_pluginbc_mayor.h pluginbc_mayor.h)
list(REMOVE_ITEM plugin_MOCS ${plugin_NO_MOCS})

include_directories(
    ${QT_INCLUDE_DIR}
	${PGSQL_INCLUDE_DIR}
	${QT_QTXML_INCLUDE_DIR}
        ${QT_QTSCRIPT_INCLUDE_DIR}
	${CMAKE_CURRENT_BINARY_DIR}
	${CMAKE_CURRENT_SOURCE_DIR}
	${CMAKE_BINARY_DIR}/bulmalib/src/
	${CMAKE_SOURCE_DIR}/bulmalib/src/
	${CMAKE_BINARY_DIR}/bulmacont/src/
	${CMAKE_SOURCE_DIR}/bulmacont/src/
	${CMAKE_BINARY_DIR}/bulmacont/plugins/pluginbc_asiento/
	${CMAKE_SOURCE_DIR}/bulmacont/plugins/pluginbc_asiento/)

# generate rules for building source files from the resources
#QT5_WRAP_CPP(plugin_MOC_SRCS ${plugin_MOCS})

# generate rules for building source files that moc generates
QT5_WRAP_UI(plugin_UIS_H ${plugin_UIS} OPTIONS -tr N_)
QT5_WRAP_CPP(plugin_MOC_SRCS ${plugin_MOCS})
#QT5_ADD_RESOURCES(plugin_RCC_SRCS ${plugin_RCCS})


IF(WIN32)
SET(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} -Wl,-enable-auto-import")
ADD_DEFINITIONS(-DQWT_DLL -DQT_DLL -DQWT_MAKEDLL -DPLUGINBC_MAYOR)
ENDIF(WIN32)


#ADD_DEFINITIONS(${QT_DEFINITIONS})
ADD_DEFINITIONS(-DQT_SHARED)
ADD_DEFINITIONS(-DQT_PLUGIN)

if(APPLE)
  set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} -undefined dynamic_lookup")
endif(APPLE)

# i18n
if(WIN32)
ADD_DEFINITIONS("-D_(String)=blI18n(\\\"${plugin}\\\",String)" )
ADD_DEFINITIONS("-DN_(String,Int)=blI18n(\\\"${plugin}\\\",String)" )
endif(WIN32)

if(NOT WIN32)
ADD_DEFINITIONS("-D'_(String)=blI18n(\"${plugin}\",String)'" )
ADD_DEFINITIONS("-D'N_(String,Int)=blI18n(\"${plugin}\",String)'" )
endif(NOT WIN32)


if(USE_GETTEXT)
   set(plugin_i18n ${plugin_SRC} ${plugin_MOC_SRCS} ${plugin_UIS_H}
                        ${plugin_SRCS} ${plugin_MOCS})
   GETTEXT_CREATE_TEMPLATE(${plugin} ${CMAKE_CURRENT_BINARY_DIR}
                                ${CMAKE_CURRENT_SOURCE_DIR} plugin_i18n)
   GETTEXT_CREATE_TRANSLATIONS(${CMAKE_CURRENT_BINARY_DIR}/${plugin}.pot
                                ALL LANGUAGES es_ES ca_ES fr_FR)
endif(USE_GETTEXT)

# build sources, moc'd sources, and rcc'd sources
add_library(${plugin} SHARED ${plugin_SRC} ${plugin_MOC_SRCS}
	${plugin_UIS_H} ${plugin_SRCS} ${plugin_MOCS})


set_target_properties( ${plugin} PROPERTIES 
  SOVERSION "${BULMAGES_VERSION_MAJOR}.${BULMAGES_VERSION_MINOR}"
  VERSION "${BULMAGES_VERSION}")

if(WIN32)
target_link_libraries(${plugin} libbulmacont bulmalib pluginbc_asiento ${QT_LIBRARIES} ${PGSQL_LIBRARIES})
else(WIN32)
target_link_libraries(${plugin} libbulmacont bulmalib ${QT_LIBRARIES} ${PGSQL_LIBRARIES})
endif(WIN32)

add_dependencies(${plugin} bulmacont bulmalib pluginbc_asiento)

install_targets(/lib/bulmacont/plugins ${plugin})

# Instalamos el archivo README para las descripciones de BulmaSetup
configure_file( ${CMAKE_CURRENT_SOURCE_DIR}/README.es ${CMAKE_CURRENT_BINARY_DIR}/README.es @ONLY)
install (FILES ${CMAKE_CURRENT_BINARY_DIR}/README.es DESTINATION share/bulmages/bulmacont-plugins/ RENAME README.es.${plugin})

# Instalamos el archivo README para las descripciones de BulmaSetup
configure_file( ${CMAKE_CURRENT_SOURCE_DIR}/README.ca ${CMAKE_CURRENT_BINARY_DIR}/README.ca @ONLY)
install (FILES ${CMAKE_CURRENT_BINARY_DIR}/README.ca DESTINATION share/bulmages/bulmacont-plugins/ RENAME README.ca.${plugin})

# Instalamos el archivo README para las descripciones de BulmaSetup
configure_file( ${CMAKE_CURRENT_SOURCE_DIR}/README.fr ${CMAKE_CURRENT_BINARY_DIR}/README.fr @ONLY)
install (FILES ${CMAKE_CURRENT_BINARY_DIR}/README.fr DESTINATION share/bulmages/bulmacont-plugins/ RENAME README.fr.${plugin})

# Instalamos el archivo README para las descripciones de BulmaSetup
configure_file( ${CMAKE_CURRENT_SOURCE_DIR}/README.en ${CMAKE_CURRENT_BINARY_DIR}/README.en @ONLY)
install (FILES ${CMAKE_CURRENT_BINARY_DIR}/README.en DESTINATION share/bulmages/bulmacont-plugins/ RENAME README.en.${plugin})

set(plugin_STR ${plugin_SRC} ${plugin_MOC_SRCS}
   ${plugin_UIS_H} ${plugin_SRCS} ${plugin_MOCS})

FILE(READ ${CMAKE_BINARY_DIR}/bulmacont/plugins/${plugin}/${plugin}_es_ES.css VAR_CSS_es_ES)
FILE(APPEND ${CMAKE_BINARY_DIR}/css/bulmacont_es_ES.css "${VAR_CSS_es_ES}" )

FILE(READ ${CMAKE_BINARY_DIR}/bulmacont/plugins/${plugin}/${plugin}_ca_ES.css VAR_CSS_ca_ES)
FILE(APPEND ${CMAKE_BINARY_DIR}/css/bulmacont_ca_ES.css "${VAR_CSS_ca_ES}" )

FILE(READ ${CMAKE_BINARY_DIR}/bulmacont/plugins/${plugin}/${plugin}_fr_FR.css VAR_CSS_fr_FR)
FILE(APPEND ${CMAKE_BINARY_DIR}/css/bulmacont_fr_FR.css "${VAR_CSS_fr_FR}" )

FILE(READ ${CMAKE_BINARY_DIR}/bulmacont/plugins/${plugin}/${plugin}_en_US.css VAR_CSS_en_US)
FILE(APPEND ${CMAKE_BINARY_DIR}/css/bulmacont_en_US.css "${VAR_CSS_en_US}" )

install(DIRECTORY images DESTINATION share/bulmages PATTERN ".svn" EXCLUDE)

