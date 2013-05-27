###########################################
# Plugin for bulmafact                    #
###########################################
#pluginbf_qwebcam

set (plugin pluginbf_qwebcam)

file(GLOB plugin_SRC *.c *.cpp *.cxx)
file(GLOB plugin_MOCS *.hpp *.h)
file(GLOB plugin_UIS *.ui)

# Exclude files with no QObject macro.
file(GLOB plugin_NO_MOCS pdefs_pluginbf_qwebcam.h)
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
	${CMAKE_BINARY_DIR}/bulmafact/src/
	${CMAKE_SOURCE_DIR}/bulmafact/src/
	/usr/include/opencv/)

# generate rules for building source files from the resources
#QT5_WRAP_CPP(plugin_MOC_SRCS ${plugin_MOCS})

# generate rules for building source files that moc generates
QT5_WRAP_UI(plugin_UIS_H ${plugin_UIS} OPTIONS -tr N_)
QT5_WRAP_CPP(plugin_MOC_SRCS ${plugin_MOCS})
#QT5_ADD_RESOURCES(plugin_RCC_SRCS ${plugin_RCCS})


IF(WIN32)
SET(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} -Wl,-enable-auto-import")
ADD_DEFINITIONS(-DQWT_DLL -DQT_DLL -DQWT_MAKEDLL -DPLUGINBF_QWEBCAM)
ENDIF(WIN32)


#ADD_DEFINITIONS(${QT_DEFINITIONS})
ADD_DEFINITIONS(-DQT_SHARED)

# i18n
if(WIN32)
ADD_DEFINITIONS("-D_(String)=blI18n(\\\"${plugin}\\\",String)" )
ADD_DEFINITIONS("-DN_(String,Int)=blI18n(\\\"${plugin}\\\",String)" )
endif(WIN32)

if(NOT WIN32)
ADD_DEFINITIONS("-D'_(String)=blI18n(\"${plugin}\",String)'" )
ADD_DEFINITIONS("-D'N_(String,Int)=blI18n(\"${plugin}\",String)'" )
endif(NOT WIN32)


# build sources, moc'd sources, and rcc'd sources

add_library(${plugin} SHARED ${plugin_SRC} ${plugin_MOC_SRCS}
	${plugin_UIS_H} ${plugin_SRCS} ${plugin_MOCS})


set_target_properties( ${plugin} PROPERTIES 
  SOVERSION "${BULMAGES_VERSION_MAJOR}.${BULMAGES_VERSION_MINOR}"
  VERSION "${BULMAGES_VERSION}")

target_link_libraries(${plugin} libbulmafact bulmalib ${QT_LIBRARIES} ${PGSQL_LIBRARIES} -lm -lcv -lhighgui -lcvaux)
install_targets(/lib/bulmafact/plugins ${plugin})

configure_file( ${CMAKE_CURRENT_SOURCE_DIR}/README.es ${CMAKE_CURRENT_BINARY_DIR}/README.es @ONLY)
install (FILES ${CMAKE_CURRENT_BINARY_DIR}/README.es DESTINATION share/bulmages/bulmafact-plugins/ RENAME README.es.${plugin})

configure_file( ${CMAKE_CURRENT_SOURCE_DIR}/README.ca ${CMAKE_CURRENT_BINARY_DIR}/README.ca @ONLY)
install (FILES ${CMAKE_CURRENT_BINARY_DIR}/README.ca DESTINATION share/bulmages/bulmafact-plugins/ RENAME README.ca.${plugin})

configure_file( ${CMAKE_CURRENT_SOURCE_DIR}/README.fr ${CMAKE_CURRENT_BINARY_DIR}/README.fr @ONLY)

install (FILES ${CMAKE_CURRENT_BINARY_DIR}/README.fr DESTINATION share/bulmages/bulmafact-plugins/ RENAME README.fr.${plugin})



configure_file( ${CMAKE_CURRENT_SOURCE_DIR}/README.en ${CMAKE_CURRENT_BINARY_DIR}/README.en @ONLY)

install (FILES ${CMAKE_CURRENT_BINARY_DIR}/README.en DESTINATION share/bulmages/bulmafact-plugins/ RENAME README.en.${plugin})


add_dependencies(${plugin} bulmafact bulmalib)
