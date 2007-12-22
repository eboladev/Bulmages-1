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

if(NOT PGSQL_FOUND)
  message(ERROR "Bulmages needs libpq to compile")
endif(NOT PGSQL_FOUND)

########### bulmages library ##########
message(STATUS "Working on bulmalib")
add_subdirectory(bulmalib)

########### bulmacont ##########
message(STATUS "Working on bulmacont")
add_subdirectory(bulmacont)

########### bulmafact ##########
message(STATUS "Working on bulmafact")
add_subdirectory(bulmafact)

########### bulmages launch ##########
message(STATUS "Working on bulmages")
add_subdirectory(bulmages)

########### bulmatpv ##########
message(STATUS "Working on bulmatpv")
add_subdirectory(bulmatpv)

########### some stuff ##########
message(STATUS "Working on installbulmages")
add_subdirectory(installbulmages)




