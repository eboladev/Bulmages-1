###########################################
# Plugin for bulmafact                    #
###########################################
#pluginbf_biblioteca

set (plugin pluginbf_biblioteca)

cmake_policy(SET CMP0020 OLD)

ADD_DEFINITIONS(-D${plugin})

configure_file( ${CMAKE_CURRENT_SOURCE_DIR}/css/${plugin}_es_ES.css.in ${CMAKE_BINARY_DIR}/bulmafact/plugins/${plugin}/${plugin}_es_ES.css @ONLY)
configure_file( ${CMAKE_CURRENT_SOURCE_DIR}/css/${plugin}_ca_ES.css.in ${CMAKE_BINARY_DIR}/bulmafact/plugins/${plugin}/${plugin}_ca_ES.css @ONLY)
configure_file( ${CMAKE_CURRENT_SOURCE_DIR}/css/${plugin}_en_US.css.in ${CMAKE_BINARY_DIR}/bulmafact/plugins/${plugin}/${plugin}_en_US.css @ONLY)
configure_file( ${CMAKE_CURRENT_SOURCE_DIR}/css/${plugin}_fr_FR.css.in ${CMAKE_BINARY_DIR}/bulmafact/plugins/${plugin}/${plugin}_fr_FR.css @ONLY)

# Mensajes de depuracion
#MESSAGE( STATUS "CMAKE_BINARY_DIR:         " ${CMAKE_BINARY_DIR} )
#MESSAGE( STATUS "CMAKE_CURRENT_SOURCE_DIR: " ${CMAKE_CURRENT_SOURCE_DIR} )
#MESSAGE( STATUS "CMAKE_SOURCE_DIR:         " ${CMAKE_SOURCE_DIR} )
#MESSAGE( STATUS "FILE:                     " ${CMAKE_CURRENT_SOURCE_DIR}/autoform_spec.spc.es.in )


configure_file( ${CMAKE_CURRENT_SOURCE_DIR}/autoform_spec.spc.es.in ${CMAKE_CURRENT_BINARY_DIR}/autoform_spec.spc.es @ONLY)
install (FILES ${CMAKE_CURRENT_BINARY_DIR}/autoform_spec.spc.es DESTINATION share/bulmages/autoforms/ RENAME autoform__${plugin}_spec.spc.es)

configure_file( ${CMAKE_CURRENT_SOURCE_DIR}/autoform_spec.spc.ca.in ${CMAKE_CURRENT_BINARY_DIR}/autoform_spec.spc.ca @ONLY)
install (FILES ${CMAKE_CURRENT_BINARY_DIR}/autoform_spec.spc.ca DESTINATION share/bulmages/autoforms/ RENAME autoform__${plugin}_spec.spc.ca)

configure_file( ${CMAKE_CURRENT_SOURCE_DIR}/autoform_spec.spc.fr.in ${CMAKE_CURRENT_BINARY_DIR}/autoform_spec.spc.fr @ONLY)
install (FILES ${CMAKE_CURRENT_BINARY_DIR}/autoform_spec.spc.fr DESTINATION share/bulmages/autoforms/ RENAME autoform__${plugin}_spec.spc.fr)

configure_file( ${CMAKE_CURRENT_SOURCE_DIR}/autoform_spec.spc.en.in ${CMAKE_CURRENT_BINARY_DIR}/autoform_spec.spc.en @ONLY)
install (FILES ${CMAKE_CURRENT_BINARY_DIR}/autoform_spec.spc.en DESTINATION share/bulmages/autoforms/ RENAME autoform__${plugin}_spec.spc.en)


configure_file( ${CMAKE_CURRENT_SOURCE_DIR}/README.es ${CMAKE_CURRENT_BINARY_DIR}/README.es @ONLY)
install (FILES ${CMAKE_CURRENT_BINARY_DIR}/README.es DESTINATION share/bulmages/bulmafact-plugins/ RENAME README.es.${plugin})

configure_file( ${CMAKE_CURRENT_SOURCE_DIR}/README.ca ${CMAKE_CURRENT_BINARY_DIR}/README.ca @ONLY)
install (FILES ${CMAKE_CURRENT_BINARY_DIR}/README.ca DESTINATION share/bulmages/bulmafact-plugins/ RENAME README.ca.${plugin})

configure_file( ${CMAKE_CURRENT_SOURCE_DIR}/README.fr ${CMAKE_CURRENT_BINARY_DIR}/README.fr @ONLY)
install (FILES ${CMAKE_CURRENT_BINARY_DIR}/README.fr DESTINATION share/bulmages/bulmafact-plugins/ RENAME README.fr.${plugin})

configure_file( ${CMAKE_CURRENT_SOURCE_DIR}/README.en ${CMAKE_CURRENT_BINARY_DIR}/README.en @ONLY)
install (FILES ${CMAKE_CURRENT_BINARY_DIR}/README.en DESTINATION share/bulmages/bulmafact-plugins/ RENAME README.en.${plugin})


FILE(READ ${CMAKE_BINARY_DIR}/bulmafact/plugins/${plugin}/${plugin}_es_ES.css VAR_CSS_es_ES)
FILE(APPEND ${CMAKE_BINARY_DIR}/css/bulmafact_es_ES.css "${VAR_CSS_es_ES}" )

FILE(READ ${CMAKE_BINARY_DIR}/bulmafact/plugins/${plugin}/${plugin}_ca_ES.css VAR_CSS_ca_ES)
FILE(APPEND ${CMAKE_BINARY_DIR}/css/bulmafact_ca_ES.css "${VAR_CSS_ca_ES}" )

FILE(READ ${CMAKE_BINARY_DIR}/bulmafact/plugins/${plugin}/${plugin}_fr_FR.css VAR_CSS_fr_FR)
FILE(APPEND ${CMAKE_BINARY_DIR}/css/bulmafact_fr_FR.css "${VAR_CSS_fr_FR}" )

FILE(READ ${CMAKE_BINARY_DIR}/bulmafact/plugins/${plugin}/${plugin}_en_US.css VAR_CSS_en_US)
FILE(APPEND ${CMAKE_BINARY_DIR}/css/bulmafact_en_US.css "${VAR_CSS_en_US}" )

install(DIRECTORY images DESTINATION share/bulmages PATTERN ".svn" EXCLUDE)

install(DIRECTORY openreports/ 
      DESTINATION share/bulmages/openreports
      PATTERN ".svn" EXCLUDE)

install(DIRECTORY autoforms/
      DESTINATION share/bulmages/autoforms/${plugin}
      PATTERN ".svn" EXCLUDE)

install(DIRECTORY dbmodels/ 
      DESTINATION share/bulmages/dbmodels/plugins
      PATTERN ".svn" EXCLUDE)


