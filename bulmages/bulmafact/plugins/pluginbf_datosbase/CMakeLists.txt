set (docs README.es README.ca README.en README.fr)

install(DIRECTORY dbmodels/ 
		DESTINATION share/bulmages/dbmodels/plugins
		PATTERN ".svn" EXCLUDE)

install(DIRECTORY dbmodels/ 
		DESTINATION share/bulmages/dbmodels/plugins
		PATTERN ".svn" EXCLUDE)

foreach(dfile ${docs})
   install (FILES ${dfile} DESTINATION share/bulmages/bulmafact-plugins/ RENAME ${dfile}.${plugin})
endforeach(dfile in ${docs})

