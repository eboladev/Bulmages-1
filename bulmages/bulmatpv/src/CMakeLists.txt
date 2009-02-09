###########################################
# bulmatpv                                #
###########################################

file(GLOB bulmatpv_SRC *.c *.cpp)
#file(GLOB bulmatpv_MOCS *.hpp *.h)
file(GLOB bulmatpv_UIS *.ui)

set(bulmatpvEx main.cxx)

set(bulmatpv_MOCS bulmatpv.h 
	   aboutview.h 
	   ticket.h 
	   subform2bt.h 
	   input.h)

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
#

# generate rules for building source files that moc generates
QT4_WRAP_UI(bulmatpv_UIS_H ${bulmatpv_UIS} OPTIONS -tr QObject::trUtf8)
QT4_WRAP_CPP(bulmatpv_MOC_SRCS ${bulmatpv_MOCS})

#QT4_ADD_RESOURCES(plugin_RCC_SRCS ${plugin_RCCS})


#ADD_DEFINITIONS(${QT_DEFINITIONS})
ADD_DEFINITIONS(-DQT_SHARED)
# build sources, moc'd sources, and rcc'd sources
# # generate rules for building source files from the resources
# build sources, moc'd sources, and rcc'd sources


add_library(libbulmatpv SHARED ${bulmatpv_MOC_SRCS} ${bulmatpv_UIS_H}
        ${bulmatpv_SRC} ${bulmatpv_MOCS})

set_target_properties( libbulmatpv PROPERTIES
  SOVERSION "${BULMAGES_VERSION_MAJOR}.${BULMAGES_VERSION_MINOR}"
  VERSION "${BULMAGES_VERSION}"
  OUTPUT_NAME bulmatpv )

target_link_libraries( libbulmatpv bulmalib ${QT_LIBRARIES} ${PGSQL_LIBRARIES})

add_executable(bulmatpv   ${bulmatpvEx})

# i18n
ADD_DEFINITIONS("-D'_(String)=BlI18n(\"bulmatpv\", String)'" )
ADD_DEFINITIONS("-D'N_(String, Int)=BlI18n(\"bulmatpv\", String)'" )

if(USE_GETTEXT)
   set(plugin_i18n ${plugin_SRC} ${plugin_MOC_SRCS} ${plugin_UIS_H}
                        ${plugin_SRCS} ${plugin_MOCS})
   GETTEXT_CREATE_TEMPLATE(${plugin} ${CMAKE_CURRENT_BINARY_DIR}
                                ${CMAKE_CURRENT_SOURCE_DIR} plugin_i18n)
   GETTEXT_CREATE_TRANSLATIONS(${CMAKE_CURRENT_BINARY_DIR}/${plugin}.pot
                                ALL LANGUAGES es_ES.UTF-8 ca_ES.UTF-8)
endif(USE_GETTEXT)

target_link_libraries( bulmatpv libbulmatpv bulmalib ${QT_LIBRARIES} ${PGSQL_LIBRARIES})



add_dependencies(libbulmatpv bulmalib)
add_dependencies(bulmatpv libbulmatpv )

install_targets(/bin bulmatpv)	
install_targets(/lib libbulmatpv)