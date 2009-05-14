###########################################
# bulmages                                #
###########################################

file(GLOB bulmages_SRC *.c *.cpp)
#file(GLOB bulmages_MOCS *.hpp *.h)
file(GLOB bulmages_UIS *.ui)

set(bulmagesEx main.cxx)

set(bulmages_RCCS bgbulmages.qrc)

set(bulmages_MOCS bgbulmages.h)

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
	${CMAKE_BINARY_DIR}/bulmages/src/
	${CMAKE_SOURCE_DIR}/bulmages/src/)

#	${CMAKE_BINARY_DIR}/bulmalib/src/
#	${CMAKE_SOURCE_DIR}/bulmalib/src/


# generate rules for building source files from the resources
#

# generate rules for building source files that moc generates
QT4_WRAP_UI(bulmages_UIS_H ${bulmages_UIS} OPTIONS -tr QObject::trUtf8)
QT4_WRAP_CPP(bulmages_MOC_SRCS ${bulmages_MOCS})

QT4_ADD_RESOURCES(bulmages_RCC_SRCS ${bulmages_RCCS})


#ADD_DEFINITIONS(${QT_DEFINITIONS})
ADD_DEFINITIONS(-DQT_SHARED)
# build sources, moc'd sources, and rcc'd sources
# # generate rules for building source files from the resources
# build sources, moc'd sources, and rcc'd sources


add_library(libbulmages SHARED ${bulmages_MOC_SRCS} ${bulmages_UIS_H}
        ${bulmages_SRC} ${bulmages_MOCS} ${bulmages_RCC_SRCS})

set_target_properties( libbulmages PROPERTIES
  SOVERSION "${BULMAGES_VERSION_MAJOR}.${BULMAGES_VERSION_MINOR}"
  VERSION "${BULMAGES_VERSION}"
  OUTPUT_NAME bulmages )

#target_link_libraries( libbulmages bulmalib ${QT_LIBRARIES} ${PGSQL_LIBRARIES})
target_link_libraries( libbulmages ${QT_LIBRARIES} ${PGSQL_LIBRARIES})

add_executable(bulmages   ${bulmagesEx})

#target_link_libraries( bulmages libbulmages bulmalib ${QT_LIBRARIES} ${PGSQL_LIBRARIES})
target_link_libraries( bulmages libbulmages ${QT_LIBRARIES} ${PGSQL_LIBRARIES})



#add_dependencies(libbulmages bulmalib)
add_dependencies(bulmages libbulmages )

install_targets(/bin bulmages)	
install_targets(/lib libbulmages)
