###########################################
# Plugin for bulmatpv                     #
###########################################
# pluginticketsdevueltos

set (plugin pluginticketsdevueltos)
set (docs README.es)

file(GLOB plugin_SRC *.c *.cpp *.cxx)
file(GLOB plugin_MOCS *.hpp *.h)

# Exclude files with no QObject macro.
file(GLOB plugin_NO_MOCS pluginticketsdevueltos.h)
list(REMOVE_ITEM plugin_MOCS ${plugin_NO_MOCS})

file(GLOB plugin_UIS *.ui)

set(QT_USE_QTASSISTANT true)
set(QT_USE_QTXML true)

INCLUDE(   ${QT_USE_FILE}   )

include_directories(
    ${QT_INCLUDE_DIR}
	${PGSQL_INCLUDE_DIR}
	${QT_QTXML_INCLUDE_DIR}
	${QT_QTASSISTANT_INCLUDE_DIR}
	${CMAKE_CURRENT_BINARY_DIR}
	${CMAKE_CURRENT_SOURCE_DIR}
	${CMAKE_BINARY_DIR}/bulmalib/src/
	${CMAKE_SOURCE_DIR}/bulmalib/src/
	${CMAKE_BINARY_DIR}/bulmatpv/src/
	${CMAKE_SOURCE_DIR}/bulmatpv/src/)

# generate rules for building source files from the resources
#QT4_WRAP_CPP(plugin_MOC_SRCS ${plugin_MOCS})

# generate rules for building source files that moc generates
QT4_WRAP_UI(plugin_UIS_H ${plugin_UIS} OPTIONS -tr QObject::trUtf8)
QT4_WRAP_CPP(plugin_MOC_SRCS ${plugin_MOCS})
#QT4_ADD_RESOURCES(plugin_RCC_SRCS ${plugin_RCCS})


#ADD_DEFINITIONS(${QT_DEFINITIONS})
ADD_DEFINITIONS(-DQT_SHARED)

# i18n
ADD_DEFINITIONS("-D'_(String)=BlI18n(\"${plugin}\", String)'" )
ADD_DEFINITIONS("-D'N_(String, Int)=BlI18n(\"${plugin}\", String)'" )

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

target_link_libraries(${plugin} libbulmatpv bulmalib ${QT_LIBRARIES} ${PGSQL_LIBRARIES})

foreach(dfile ${docs})
   install (FILES ${dfile} DESTINATION share/bulmages/bulmatpv-plugins/ RENAME ${dfile}.${plugin})
endforeach(dfile in ${docs})

add_dependencies(${plugin} bulmatpv bulmalib)

install_targets(/lib/bulmatpv/plugins ${plugin})
