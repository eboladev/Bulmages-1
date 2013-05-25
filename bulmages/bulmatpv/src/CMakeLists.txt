###########################################
# bulmatpv                                #
###########################################

file(GLOB bulmatpv_SRC *.c *.cpp)
file(GLOB bulmatpv_UIS *.ui)

set(bulmatpvEx main.cxx)

set(bulmatpv_MOCS btbulmatpv.h
	   btaboutview.h
	   btticket.h
	   btsubform.h
	   btinput.h)

set(QT_USE_QTASSISTANT false)
set(QT_USE_QTXML true)
set(QT_USE_QTUITOOLS true)
set(QT_USE_QTSCRIPT true)


#INCLUDE(${QT_USE_FILE})

include_directories(
    ${QT_INCLUDE_DIR}
	${PGSQL_INCLUDE_DIR}
	${QT_QTXML_INCLUDE_DIR}
        ${QT_QTSCRIPT_INCLUDE_DIR}
	${CMAKE_CURRENT_BINARY_DIR}
	${CMAKE_CURRENT_SOURCE_DIR}
	${CMAKE_BINARY_DIR}/bulmalib/src/
	${CMAKE_SOURCE_DIR}/bulmalib/src/
	${CMAKE_BINARY_DIR}/bulmatpv/src/
	${CMAKE_SOURCE_DIR}/bulmatpv/src/)


if(USE_QMDIAREA)
      ADD_DEFINITIONS("-D'AREA_QMDI'" )
endif(USE_QMDIAREA)

# generate rules for building source files from the resources
#

# generate rules for building source files that moc generates
QT5_WRAP_UI(bulmatpv_UIS_H ${bulmatpv_UIS} OPTIONS -tr N_)
QT5_WRAP_CPP(bulmatpv_MOC_SRCS ${bulmatpv_MOCS})

#QT5_ADD_RESOURCES(plugin_RCC_SRCS ${plugin_RCCS})

IF(WIN32)
set(bulmatpv_RCCS ../../bulmages.qrc)
QT5_ADD_RESOURCES(bulmatpv_RCC_SRCS ${bulmatpv_RCCS})
ADD_DEFINITIONS(-DQWT_DLL -DQT_DLL -DQWT_MAKEDLL -DBT_BULMATPV)
ENDIF(WIN32)


#ADD_DEFINITIONS(${QT_DEFINITIONS})
ADD_DEFINITIONS(-DQT_SHARED)
# build sources, moc'd sources, and rcc'd sources
# # generate rules for building source files from the resources
# build sources, moc'd sources, and rcc'd sources


add_library(libbulmatpv SHARED ${bulmatpv_MOC_SRCS} ${bulmatpv_UIS_H}
        ${bulmatpv_SRC} ${bulmatpv_MOCS} ${bulmatpv_RCC_SRCS})

set_target_properties( libbulmatpv PROPERTIES
  SOVERSION "${BULMAGES_VERSION_MAJOR}.${BULMAGES_VERSION_MINOR}"
  VERSION "${BULMAGES_VERSION}"
  OUTPUT_NAME bulmatpv )

target_link_libraries( libbulmatpv bulmalib ${QT_LIBRARIES} ${PGSQL_LIBRARIES})

add_executable(bulmatpv   ${bulmatpvEx} ${bulmatpv_RCC_SRCS})

# i18n
if(WIN32)
ADD_DEFINITIONS("-D_(String)=blI18n(\\\"bulmatpv\\\",String)" )
ADD_DEFINITIONS("-DN_(String,Int)=blI18n(\\\"bulmatpv\\\",String)" )
endif(WIN32)

if(NOT WIN32)
ADD_DEFINITIONS("-D'_(String)=blI18n(\"bulmatpv\",String)'" )
ADD_DEFINITIONS("-D'N_(String,Int)=blI18n(\"bulmatpv\",String)'" )
endif(NOT WIN32)


if(USE_GETTEXT)
   set(bulmatpv_i18n ${bulmatpvEx} ${bulmatpv_MOC_SRCS} ${bulmatpv_UIS_H}
	        ${bulmatpv_SRC} ${bulmatpv_MOCS})
   GETTEXT_CREATE_TEMPLATE(bulmatpv ${CMAKE_CURRENT_BINARY_DIR}
                                ${CMAKE_CURRENT_SOURCE_DIR} bulmatpv_i18n)
   GETTEXT_CREATE_TRANSLATIONS(${CMAKE_CURRENT_BINARY_DIR}/bulmatpv.pot
                                ALL LANGUAGES es_ES ca_ES fr_FR)
endif(USE_GETTEXT)

target_link_libraries( bulmatpv libbulmatpv bulmalib ${QT_LIBRARIES} ${PGSQL_LIBRARIES})



add_dependencies(libbulmatpv bulmalib)
add_dependencies(bulmatpv libbulmatpv )

install_targets(/bin bulmatpv)
install_targets(/lib libbulmatpv)
