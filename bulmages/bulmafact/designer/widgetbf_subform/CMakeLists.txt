###########################################
# Bulmafact plugin for Qt Designer        #
###########################################
#widgetbf_subform


set (plugin widgetbf_subform)

file(GLOB plugin_SRC *.c *.cpp *.cxx)
file(GLOB plugin_MOCS *.hpp *.h)
file(GLOB plugin_UIS *.ui)

set(QT_USE_QTSCRIPT true)
set(QT_USE_QTASSISTANT true)
set(QT_USE_QTDESIGNER true)
set(QT_USE_QTXML true)

INCLUDE(   ${QT_USE_FILE}   )

include_directories(
	${QT_INCLUDE_DIR}
	${QT_QTXML_INCLUDE_DIR}
	${QT_QTASSISTANT_INCLUDE_DIR}
	${QT_QTDESIGNER_INCLUDE_DIR}
	${QT_QTSCRIPT_INCLUDE_DIR}
	${CMAKE_CURRENT_BINARY_DIR}
	${CMAKE_CURRENT_SOURCE_DIR})

# generate rules for building source files that moc generates
QT4_WRAP_UI(plugin_UIS_H ${plugin_UIS})
QT4_WRAP_CPP(plugin_MOC_SRCS ${plugin_MOCS})


ADD_DEFINITIONS(-DQT_SHARED)


add_library(${plugin} SHARED ${plugin_SRC} ${plugin_MOC_SRCS}
	${plugin_UIS_H} ${plugin_SRCS} ${plugin_MOCS})


set_target_properties( ${plugin} PROPERTIES 
  SOVERSION "${BULMAGES_VERSION_MAJOR}.${BULMAGES_VERSION_MINOR}"
  VERSION "${BULMAGES_VERSION}")

target_link_libraries(${plugin} ${QT_LIBRARIES})
install_targets(/lib/bulmafact/plugins ${plugin})


