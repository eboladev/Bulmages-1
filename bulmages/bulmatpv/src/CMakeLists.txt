###########################################
# bulmatpv                                #
###########################################

file(GLOB bulmatpv_SRC *.c *.cpp)
file(GLOB bulmatpv_UIS *.ui)

find_package(Qt5Core REQUIRED)

set(bulmatpvEx main.cxx)

set(bulmatpv_MOCS btbulmatpv.h
	   btaboutview.h
	   btticket.h
	   btsubform.h
	   btinput.h)

include_directories(
	${QT_INCLUDE_DIR}
	${PGSQL_INCLUDE_DIR}
	${CMAKE_CURRENT_BINARY_DIR}
	${CMAKE_CURRENT_SOURCE_DIR}
	${CMAKE_BINARY_DIR}/bulmalib/src/
	${CMAKE_SOURCE_DIR}/bulmalib/src/
	${CMAKE_BINARY_DIR}/bulmatpv/src/
	${CMAKE_SOURCE_DIR}/bulmatpv/src/)

if(USE_GETTEXT)
   set(bulmatpv_i18n ${bulmatpvEx} ${bulmatpv_MOC_SRCS} ${bulmatpv_UIS_H}
	        ${bulmatpv_SRC} ${bulmatpv_MOCS})
   GETTEXT_CREATE_TEMPLATE(bulmatpv ${CMAKE_CURRENT_BINARY_DIR}
                                ${CMAKE_CURRENT_SOURCE_DIR} bulmatpv_i18n)
   GETTEXT_CREATE_TRANSLATIONS(${CMAKE_CURRENT_BINARY_DIR}/bulmatpv.pot
                                ALL LANGUAGES es_ES ca_ES fr_FR)
endif(USE_GETTEXT)

# i18n
if(WIN32)
ADD_DEFINITIONS("-D_(String)=blI18n(\\\"bulmatpv\\\",String)" )
ADD_DEFINITIONS("-DN_(String,Int)=blI18n(\\\"bulmatpv\\\",String)" )
else(WIN32)
ADD_DEFINITIONS("-D'_(String)=blI18n(\"bulmatpv\",String)'" )
ADD_DEFINITIONS("-D'N_(String,Int)=blI18n(\"bulmatpv\",String)'" )
endif(WIN32)

# generate rules for building source files that moc generates
# build sources, moc'd sources, and rcc'd sources
# # generate rules for building source files from the resources
# build sources, moc'd sources, and rcc'd sources
QT5_WRAP_UI(bulmatpv_UIS_H ${bulmatpv_UIS} OPTIONS -tr N_)
QT5_WRAP_CPP(bulmatpv_MOC_SRCS ${bulmatpv_MOCS})

IF(WIN32)
set(bulmatpv_RCCS ../../bulmages.qrc)
QT5_ADD_RESOURCES(bulmatpv_RCC_SRCS ${bulmatpv_RCCS})
ENDIF(WIN32)

ADD_DEFINITIONS(-DQT_SHARED)



add_library(libbulmatpv SHARED ${bulmatpv_MOC_SRCS} ${bulmatpv_UIS_H}
        ${bulmatpv_SRC} ${bulmatpv_MOCS} ${bulmatpv_RCC_SRCS})

IF(WIN32)
set_property(TARGET libbulmatpv PROPERTY COMPILE_DEFINITIONS BT_BULMATPV QWT_MAKEDLL)
ENDIF(WIN32)

set_target_properties(libbulmatpv PROPERTIES
  SOVERSION "${BULMAGES_VERSION_MAJOR}.${BULMAGES_VERSION_MINOR}"
  VERSION "${BULMAGES_VERSION}"
  OUTPUT_NAME bulmatpv )

target_link_libraries(libbulmatpv bulmalib ${QT_LIBRARIES} ${PGSQL_LIBRARIES})


IF(WIN32)
  add_executable(bulmatpv  ${bulmatpvEx} ${bulmatpv_RCC_SRCS} )
  target_link_libraries(bulmatpv libbulmatpv  bulmalib ${PGSQL_LIBRARIES} )
  qt5_use_modules(bulmatpv UiTools Xml Core Widgets Script WebKit)
ELSE(WIN32)
  add_executable(bulmatpv  ${bulmatpvEx})
  target_link_libraries(bulmatpv libbulmatpv)
ENDIF(WIN32)

qt5_use_modules(libbulmatpv UiTools Xml Core Widgets Script WebKit)

add_dependencies(libbulmatpv bulmalib)
add_dependencies(bulmatpv libbulmatpv )

install_targets(/bin bulmatpv)
install_targets(/lib libbulmatpv)
