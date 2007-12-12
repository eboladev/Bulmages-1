###########################################
# bulmafact					  #
###########################################

file(GLOB bulmafact_SRC *.c *.cpp *.cxx)
file(GLOB bulmafact_MOCS *.hpp *.h)
file(GLOB bulmafact_UIS *.ui)

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
	${CMAKE_BINARY_DIR}/bulmafact/src/
	${CMAKE_SOURCE_DIR}/bulmafact/src/)

# generate rules for building source files from the resources
#

# generate rules for building source files that moc generates
QT4_WRAP_UI(bulmafact_UIS_H ${bulmafact_UIS})
QT4_WRAP_CPP(bulmafact_MOC_SRCS ${bulmafact_MOCS})

#QT4_ADD_RESOURCES(plugin_RCC_SRCS ${plugin_RCCS})


#ADD_DEFINITIONS(${QT_DEFINITIONS})
#ADD_DEFINITIONS(-DQT_SHARED)
# build sources, moc'd sources, and rcc'd sources
# # generate rules for building source files from the resources
# build sources, moc'd sources, and rcc'd sources
add_executable(bulmafact  ${bulmafact_MOC_SRCS} ${bulmafact_UIS_H}
	${bulmafact_SRCS} ${bulmafact_MOCS} ${bulmafact_RCC_SRCS} ${bulmafact_SRC})

target_link_libraries( bulmafact bulmalib ${QT_LIBRARIES}
	${QT_QTGUI_LIBRARY} ${PGSQL_LIBRARIES} 	${QT_QTASSISTANT_LIBRARY})


install_targets(/bin bulmafact)
