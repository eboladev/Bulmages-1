###########################################
# Plugin for bulmafact                    #
###########################################
#pluginbf_alquileres

set (plugin pluginbf_alquileres)
ADD_DEFINITIONS(-D${plugin})

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

configure_file( ${CMAKE_CURRENT_SOURCE_DIR}/ContratoBase_spec.spc.es.in ${CMAKE_CURRENT_BINARY_DIR}/ContratoBase_spec.spc.es @ONLY)
install (FILES ${CMAKE_CURRENT_BINARY_DIR}/ContratoBase_spec.spc.es DESTINATION share/bulmages/autoforms/ RENAME ContratoBase__${plugin}_spec.spc.es)

configure_file( ${CMAKE_CURRENT_SOURCE_DIR}/ContratoBase_spec.spc.ca.in ${CMAKE_CURRENT_BINARY_DIR}/ContratoBase_spec.spc.ca @ONLY)
install (FILES ${CMAKE_CURRENT_BINARY_DIR}/ContratoBase_spec.spc.ca DESTINATION share/bulmages/autoforms/ RENAME ContratoBase__${plugin}_spec.spc.ca)

configure_file( ${CMAKE_CURRENT_SOURCE_DIR}/ContratoBase_spec.spc.fr.in ${CMAKE_CURRENT_BINARY_DIR}/ContratoBase_spec.spc.fr @ONLY)
install (FILES ${CMAKE_CURRENT_BINARY_DIR}/ContratoBase_spec.spc.fr DESTINATION share/bulmages/autoforms/ RENAME ContratoBase__${plugin}_spec.spc.fr)

configure_file( ${CMAKE_CURRENT_SOURCE_DIR}/ContratoBase_spec.spc.en.in ${CMAKE_CURRENT_BINARY_DIR}/ContratoBase_spec.spc.en @ONLY)
install (FILES ${CMAKE_CURRENT_BINARY_DIR}/ContratoBase_spec.spc.en DESTINATION share/bulmages/autoforms/ RENAME ContratoBase__${plugin}_spec.spc.en)


configure_file( ${CMAKE_CURRENT_SOURCE_DIR}/README.es ${CMAKE_CURRENT_BINARY_DIR}/README.es @ONLY)
install (FILES ${CMAKE_CURRENT_BINARY_DIR}/README.es DESTINATION share/bulmages/bulmafact-plugins/ RENAME README.es.${plugin})

configure_file( ${CMAKE_CURRENT_SOURCE_DIR}/README.ca ${CMAKE_CURRENT_BINARY_DIR}/README.ca @ONLY)
install (FILES ${CMAKE_CURRENT_BINARY_DIR}/README.ca DESTINATION share/bulmages/bulmafact-plugins/ RENAME README.ca.${plugin})

configure_file( ${CMAKE_CURRENT_SOURCE_DIR}/README.fr ${CMAKE_CURRENT_BINARY_DIR}/README.fr @ONLY)
install (FILES ${CMAKE_CURRENT_BINARY_DIR}/README.fr DESTINATION share/bulmages/bulmafact-plugins/ RENAME README.fr.${plugin})

configure_file( ${CMAKE_CURRENT_SOURCE_DIR}/README.en ${CMAKE_CURRENT_BINARY_DIR}/README.en @ONLY)
install (FILES ${CMAKE_CURRENT_BINARY_DIR}/README.en DESTINATION share/bulmages/bulmafact-plugins/ RENAME README.en.${plugin})



install(DIRECTORY openreports/ 
      DESTINATION share/bulmages/openreports
      PATTERN ".svn" EXCLUDE)

install(DIRECTORY autoforms/
      DESTINATION share/bulmages/autoforms/${plugin}
      PATTERN ".svn" EXCLUDE)

install(DIRECTORY dbmodels/ 
      DESTINATION share/bulmages/dbmodels/plugins
      PATTERN ".svn" EXCLUDE)


