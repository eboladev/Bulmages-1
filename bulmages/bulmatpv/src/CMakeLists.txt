###########################################
# bulmatpv					  #
###########################################

file(GLOB bulmatpv_SRC *.c *.cpp *.cxx)
#file(GLOB bulmatpv_MOCS *.hpp *.h)
file(GLOB bulmatpv_UIS *.ui)

set(bulmatpv_MOCS bulmatpv.h 
	   aboutview.h 
	   empresatpv.h 
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
#ADD_DEFINITIONS(-DQT_SHARED)
# build sources, moc'd sources, and rcc'd sources
# # generate rules for building source files from the resources
# build sources, moc'd sources, and rcc'd sources
add_executable(bulmatpv   ${bulmatpv_MOCS} ${bulmatpv_MOC_SRCS} ${bulmatpv_UIS_H}
	${bulmatpv_SRC} ${bulmatpv_SRCS} )

target_link_libraries( bulmatpv bulmalib ${QT_LIBRARIES} ${PGSQL_LIBRARIES})

add_dependencies(bulmatpv bulmalib)

install_targets(/bin bulmatpv)	
