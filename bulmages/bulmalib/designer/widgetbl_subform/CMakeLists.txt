###########################################
# Bulmafact plugin for Qt Designer        #
###########################################
#widgetbl_subform


set (plugin widgetbl_subform)

file(GLOB plugin_SRC *.c *.cpp *.cxx ../../src/bli18n.cpp)
file(GLOB plugin_MOCS *.hpp *.h)
file(GLOB plugin_UIS ../../src/blsubformbase.ui)


set(QT_USE_QTSCRIPT true)
set(QT_USE_QTASSISTANT false)
set(QT_USE_QTDESIGNER true)
set(QT_USE_QTXML true)
set(QT_USE_QTSCRIPT true)

#INCLUDE( ${QT_USE_FILE} )

include_directories(
	${PGSQL_INCLUDE_DIR}
	${QT_INCLUDE_DIR}
	${QT_QTXML_INCLUDE_DIR}
	${QT_QTSCRIPT_INCLUDE_DIR}
	${CMAKE_BINARY_DIR}/bulmalib/src/
	${CMAKE_SOURCE_DIR}/bulmalib/src/
	${CMAKE_CURRENT_BINARY_DIR}
	${CMAKE_CURRENT_SOURCE_DIR})

# generate rules for building source files that moc generates
QT5_WRAP_UI(plugin_UIS_H ${plugin_UIS})
QT5_WRAP_CPP(plugin_MOC_SRCS ${plugin_MOCS})


IF(WIN32)
ADD_DEFINITIONS(-DQWT_DLL -DQT_DLL -DQWT_MAKEDLL)
ENDIF(WIN32)


ADD_DEFINITIONS(-DQT_SHARED)


add_library(${plugin} SHARED ${plugin_SRC} ${plugin_MOC_SRCS}
	${plugin_UIS_H} ${plugin_SRCS} ${plugin_MOCS})


set_target_properties( ${plugin} PROPERTIES 
  SOVERSION "${BULMAGES_VERSION_MAJOR}.${BULMAGES_VERSION_MINOR}"
  VERSION "${BULMAGES_VERSION}")

SET_TARGET_PROPERTIES( ${plugin} PROPERTIES DEFINE_SYMBOL QDESIGNER_EXPORT_WIDGETS)


target_link_libraries(${plugin} ${QT_LIBRARIES})
install_targets(/lib/bulmalib/plugins ${plugin})


