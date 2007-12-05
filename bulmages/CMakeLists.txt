# ########## Project setup ##########
PROJECT(bulmages)
CMAKE_MINIMUM_REQUIRED(VERSION 2.4.5)

# ######### General setup ##########
INCLUDE_DIRECTORIES(${CMAKE_SOURCE_DIR})

# ########## bulmages library ##########
# Sources:
SET(bulmages_library_SRCS
    bulmages.qrc
)

# Headers:
SET(bulmages_library_HDRS
)



# actual target:
ADD_LIBRARY(bulmages ${bulmages_library}_SCRS ${bulmages_library_HDRS})
SET_TARGET_PROPERTIES(bulmages PROPERTIES VERSION 0.0.0)

# add install target:
INSTALL(TARGETS bulmages DESTINATION lib)
