###########################################
# Plugin for bulmafact                    #
###########################################
#pluginbf_factcont

set (plugin pluginbf_factcont)

configure_file( ${CMAKE_CURRENT_SOURCE_DIR}/README.es ${CMAKE_CURRENT_BINARY_DIR}/README.es @ONLY)
install (FILES ${CMAKE_CURRENT_BINARY_DIR}/README.es DESTINATION share/bulmages/bulmafact-plugins/ RENAME README.es.${plugin})

configure_file( ${CMAKE_CURRENT_SOURCE_DIR}/README.ca ${CMAKE_CURRENT_BINARY_DIR}/README.ca @ONLY)
install (FILES ${CMAKE_CURRENT_BINARY_DIR}/README.ca DESTINATION share/bulmages/bulmafact-plugins/ RENAME README.ca.${plugin})

configure_file( ${CMAKE_CURRENT_SOURCE_DIR}/README.fr ${CMAKE_CURRENT_BINARY_DIR}/README.fr @ONLY)
install (FILES ${CMAKE_CURRENT_BINARY_DIR}/README.fr DESTINATION share/bulmages/bulmafact-plugins/ RENAME README.fr.${plugin})

configure_file( ${CMAKE_CURRENT_SOURCE_DIR}/README.en ${CMAKE_CURRENT_BINARY_DIR}/README.en @ONLY)
install (FILES ${CMAKE_CURRENT_BINARY_DIR}/README.en DESTINATION share/bulmages/bulmafact-plugins/ RENAME README.en.${plugin})


install(DIRECTORY dbmodels/ 
      DESTINATION share/bulmages/dbmodels/plugins
      PATTERN ".svn" EXCLUDE)

install(DIRECTORY openreports/ 
      DESTINATION share/bulmages/openreports
      PATTERN ".svn" EXCLUDE)
