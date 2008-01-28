configure_file( ${CMAKE_SOURCE_DIR}/installbulmages/version.in version @ONLY)
configure_file( ${CMAKE_SOURCE_DIR}/installbulmages/bulmages.conf.in bulmages.conf @ONLY)
configure_file( ${CMAKE_SOURCE_DIR}/installbulmages/bulmafact.conf.in bulmafact.conf @ONLY)
configure_file( ${CMAKE_SOURCE_DIR}/installbulmages/bulmacont.conf.in bulmacont.conf @ONLY)
configure_file( ${CMAKE_SOURCE_DIR}/installbulmages/bulmatpv.conf.in bulmatpv.conf @ONLY)
configure_file( ${CMAKE_SOURCE_DIR}/installbulmages/installbulmages-db.in installbulmages-db @ONLY)
configure_file( ${CMAKE_SOURCE_DIR}/installbulmages/uninstallbulmages.in uninstallbulmages  @ONLY)

install(DIRECTORY ayuda ainteligentes bgtrml2pdf 
				balances planescontables 
				openreports icons 
				formularios css
				DESTINATION share/bulmages
 				PATTERN ".svn" EXCLUDE)

install(DIRECTORY dbmodels/crear dbmodels/actualizar 
				DESTINATION share/bulmages/dbmodels
 				PATTERN ".svn" EXCLUDE)

install(DIRECTORY man DESTINATION share/
				PATTERN ".svn" EXCLUDE)
			
FILE(GLOB files "rev-*" "*.png" "*.jpg" 
			README COPYING 
			${CMAKE_BINARY_DIR}/version)
install(FILES ${files} DESTINATION share/bulmages)

FILE(GLOB conf_etc ${CMAKE_BINARY_DIR}/bulmacont.conf 
					${CMAKE_BINARY_DIR}/bulmafact.conf
					${CMAKE_BINARY_DIR}/bulmages.conf
					${CMAKE_BINARY_DIR}/bulmatpv.conf)
install(FILES ${conf_etc} DESTINATION /etc/bulmages)


set (scripts installbulmages-client
			installbulmages-server
			deinstallbulmages
			${CMAKE_BINARY_DIR}/installbulmages-db
			${CMAKE_BINARY_DIR}/uninstallbulmages)
install(FILES ${scripts} DESTINATION share/bulmages
		PERMISSIONS OWNER_EXECUTE OWNER_WRITE OWNER_READ
                      GROUP_EXECUTE GROUP_READ WORLD_READ WORLD_EXECUTE)

FILE(scriptsdb dbmodels/actualizabulmacont
					dbmodels/actualizabulmafact
					dbmodels/creabulmatcont
					dbmodels/creabulmafact)
install(FILES ${scriptsdb} DESTINATION share/bulmages/dbmodels
		PERMISSIONS OWNER_EXECUTE OWNER_WRITE OWNER_READ
                      GROUP_EXECUTE GROUP_READ GROUP_READ WORLD_READ WORLD_EXECUTE)



