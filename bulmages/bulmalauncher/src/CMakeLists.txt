###########################################
# bulmalauncher                           #
###########################################

file(GLOB bulmalauncher_SRC *.c *.cpp)
#file(GLOB bulmalauncher_MOCS *.hpp *.h)
file(GLOB bulmalauncher_UIS *.ui)

set(bulmalauncherEx main.cxx)

set(bulmalauncher_RCCS blauncher.qrc)

set(bulmalauncher_MOCS bulmalauncher.h)

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
	${CMAKE_BINARY_DIR}/bulmalauncher/src/
	${CMAKE_SOURCE_DIR}/bulmalauncher/src/)

#	${CMAKE_BINARY_DIR}/bulmalib/src/
#	${CMAKE_SOURCE_DIR}/bulmalib/src/


# generate rules for building source files from the resources
#

# generate rules for building source files that moc generates
QT4_WRAP_UI(bulmalauncher_UIS_H ${bulmalauncher_UIS} OPTIONS -tr QObject::trUtf8)
QT4_WRAP_CPP(bulmalauncher_MOC_SRCS ${bulmalauncher_MOCS})

QT4_ADD_RESOURCES(bulmalauncher_RCC_SRCS ${bulmalauncher_RCCS})


#ADD_DEFINITIONS(${QT_DEFINITIONS})
ADD_DEFINITIONS(-DQT_SHARED)
# build sources, moc'd sources, and rcc'd sources
# # generate rules for building source files from the resources
# build sources, moc'd sources, and rcc'd sources


add_library(libbulmalauncher SHARED ${bulmalauncher_MOC_SRCS} ${bulmalauncher_UIS_H}
        ${bulmalauncher_SRC} ${bulmalauncher_MOCS} ${bulmalauncher_RCC_SRCS})

set_target_properties( libbulmalauncher PROPERTIES
  SOVERSION "${BULMAGES_VERSION_MAJOR}.${BULMAGES_VERSION_MINOR}"
  VERSION "${BULMAGES_VERSION}"
  OUTPUT_NAME bulmalauncher )

#target_link_libraries( libbulmalauncher bulmalib ${QT_LIBRARIES} ${PGSQL_LIBRARIES})
target_link_libraries( libbulmalauncher ${QT_LIBRARIES} ${PGSQL_LIBRARIES})

add_executable(bulmalauncher   ${bulmalauncherEx})

#target_link_libraries( bulmalauncher libbulmalauncher bulmalib ${QT_LIBRARIES} ${PGSQL_LIBRARIES})
target_link_libraries( bulmalauncher libbulmalauncher ${QT_LIBRARIES} ${PGSQL_LIBRARIES})



#add_dependencies(libbulmalauncher bulmalib)
add_dependencies(bulmalauncher libbulmalauncher )

install_targets(/bin bulmalauncher)	
install_targets(/lib libbulmalauncher)
