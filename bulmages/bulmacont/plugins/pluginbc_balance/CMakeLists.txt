###########################################
# Plugin for bulmacont                    #
###########################################
#pluginbc_balance

set (plugin pluginbc_balance)
set (docs README.es)

file(GLOB plugin_SRC *.c *.cpp *.cxx)
file(GLOB plugin_MOCS *.hpp *.h)
file(GLOB plugin_UIS *.ui)


# Exclude files with no QObject macro.
file(GLOB plugin_NO_MOCS pdefs_pluginbc_balance.h)
list(REMOVE_ITEM plugin_MOCS ${plugin_NO_MOCS})


set(QT_USE_QTASSISTANT false)
set(QT_USE_QTXML true)

INCLUDE( ${QT_USE_FILE} )

include_directories(
    ${QT_INCLUDE_DIR}
	${PGSQL_INCLUDE_DIR}
	${QT_QTXML_INCLUDE_DIR}
	${CMAKE_CURRENT_BINARY_DIR}
	${CMAKE_CURRENT_SOURCE_DIR}
	${CMAKE_BINARY_DIR}/bulmalib/src/
	${CMAKE_SOURCE_DIR}/bulmalib/src/
	${CMAKE_BINARY_DIR}/bulmacont/src/
	${CMAKE_SOURCE_DIR}/bulmacont/src/
   ${CMAKE_BINARY_DIR}/bulmacont/plugins/pluginbc_cuenta/
   ${CMAKE_SOURCE_DIR}/bulmacont/plugins/pluginbc_cuenta/)

# generate rules for building source files from the resources
#QT4_WRAP_CPP(plugin_MOC_SRCS ${plugin_MOCS})

# generate rules for building source files that moc generates
QT4_WRAP_UI(plugin_UIS_H ${plugin_UIS} OPTIONS -tr N_)
QT4_WRAP_CPP(plugin_MOC_SRCS ${plugin_MOCS})
#QT4_ADD_RESOURCES(plugin_RCC_SRCS ${plugin_RCCS})


IF(WIN32)
ADD_DEFINITIONS(-DQWT_DLL -DQT_DLL -DQWT_MAKEDLL -DPLUGINBC_BALANCE)
ENDIF(WIN32)


#ADD_DEFINITIONS(${QT_DEFINITIONS})
ADD_DEFINITIONS(-DQT_SHARED)
ADD_DEFINITIONS(-DQT_PLUGIN)

if(APPLE)
  set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} -undefined dynamic_lookup")
endif(APPLE)

# i18n
if(WIN32)
ADD_DEFINITIONS("-D_(String)=BlI18n(\\\"${plugin}\\\",String)" )
ADD_DEFINITIONS("-DN_(String,Int)=BlI18n(\\\"${plugin}\\\",String)" )
endif(WIN32)

if(NOT WIN32)
ADD_DEFINITIONS("-D'_(String)=BlI18n(\"${plugin}\",String)'" )
ADD_DEFINITIONS("-D'N_(String,Int)=BlI18n(\"${plugin}\",String)'" )
endif(NOT WIN32)


if(USE_GETTEXT)
   set(plugin_i18n ${plugin_SRC} ${plugin_MOC_SRCS} ${plugin_UIS_H}
                        ${plugin_SRCS} ${plugin_MOCS})
   GETTEXT_CREATE_TEMPLATE(${plugin} ${CMAKE_CURRENT_BINARY_DIR}
                                ${CMAKE_CURRENT_SOURCE_DIR} plugin_i18n)
   GETTEXT_CREATE_TRANSLATIONS(${CMAKE_CURRENT_BINARY_DIR}/${plugin}.pot
                                ALL LANGUAGES es_ES.UTF-8 ca_ES.UTF-8)
endif(USE_GETTEXT)

# build sources, moc'd sources, and rcc'd sources
add_library(${plugin} SHARED ${plugin_SRC} ${plugin_MOC_SRCS}
	${plugin_UIS_H} ${plugin_SRCS} ${plugin_MOCS})


set_target_properties( ${plugin} PROPERTIES 
  SOVERSION "${BULMAGES_VERSION_MAJOR}.${BULMAGES_VERSION_MINOR}"
  VERSION "${BULMAGES_VERSION}")

if(WIN32)
target_link_libraries(${plugin} libbulmacont bulmalib pluginbc_balancearbol ${QT_LIBRARIES} ${PGSQL_LIBRARIES})
else(WIN32)
target_link_libraries(${plugin} libbulmacont bulmalib ${QT_LIBRARIES} ${PGSQL_LIBRARIES})
endif(WIN32)

add_dependencies(${plugin} bulmacont bulmalib pluginbc_balancearbol)

install_targets(/lib/bulmacont/plugins ${plugin})
foreach(dfile ${docs})
 install (FILES ${dfile}
DESTINATION share/bulmages/bulmacont-plugins/ RENAME ${dfile}.${plugin})
endforeach(dfile in ${docs})

set(plugin_STR ${plugin_SRC} ${plugin_MOC_SRCS}
   ${plugin_UIS_H} ${plugin_SRCS} ${plugin_MOCS})

#GETTEXT_CREATE_TEMPLATE(${plugin} ${CMAKE_CURRENT_SOURCE_DIR} plugin_STR)

file(GLOB report_files openreports/*.ui openreports/*.pys openreports/*.rml)

install (FILES ${report_files} DESTINATION share/bulmages/openreports)
