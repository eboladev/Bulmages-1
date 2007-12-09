# ########## Project setup ##########
PROJECT(bulmages)
CMAKE_MINIMUM_REQUIRED(VERSION 2.4.5)

SET( BULMAGES_VERSION 0.11.0 )
STRING( REGEX MATCHALL "[0-9]+" BULMAGES_VERSIONS ${BULMAGES_VERSION} )
LIST( GET BULMAGES_VERSIONS 0 BULMAGES_VERSION_MAJOR)
LIST( GET BULMAGES_VERSIONS 1 BULMAGES_VERSION_MINOR)
LIST( GET BULMAGES_VERSIONS 2 BULMAGES_VERSION_PATCH)


# ######### General setup ##########
INCLUDE_DIRECTORIES(${CMAKE_SOURCE_DIR})
find_package(Qt4 REQUIRED) 
INCLUDE (${CMAKE_CURRENT_SOURCE_DIR}/ModulesCMake/FindPostgreSQL.cmake)
# ########## bulmages library ##########


add_subdirectory(bulmalib)
add_subdirectory(bulmacont)
add_subdirectory(bulmafact)
#add_subdirectory(bulmages)
#add_subdirectory(bulmatpv)
add_subdirectory(installbulmages)





# Sources:
#SET(bulmages_library_SRCS
#    bulmages.qrc
#)

# Headers:
#SET(bulmages_library_HDRS
#)



# actual target:
#ADD_LIBRARY(bulmages ${bulmages_library}_SCRS ${bulmages_library_HDRS})
#SET_TARGET_PROPERTIES(bulmages PROPERTIES VERSION 0.0.0)

# add install target:
#INSTALL(TARGETS bulmages DESTINATION lib)
