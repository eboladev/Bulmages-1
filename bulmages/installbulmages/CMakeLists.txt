configure_file( ${CMAKE_SOURCE_DIR}/installbulmages/version.in version @ONLY)

install(DIRECTORY ayuda ainteligentes bgtrml2pdf 
				balances planescontables 
				openreports icons 
				dbmodels formularios css
				DESTINATION share/bulmages
				PATTERN ".svn" EXCLUDE)

install(DIRECTORY man DESTINATION share/
				PATTERN ".svn" EXCLUDE)
			
FILE(GLOB files "rev-*" "*.conf" "*.png" "*.jpg" 
			README COPYING 
			${CMAKE_BINARY_DIR}/version)
			
install(FILES ${files} DESTINATION share/bulmages)

FILE(GLOB scripts installbulmages-client
			installbulmages-server
			deinstallbulmages)
install(FILES ${scripts} DESTINATION share/bulmages
		PERMISSIONS OWNER_EXECUTE OWNER_WRITE OWNER_READ
                      GROUP_EXECUTE GROUP_READ)




