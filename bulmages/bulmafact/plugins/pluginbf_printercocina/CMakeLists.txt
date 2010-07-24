###########################################
# Plugin for bulmafact                    #
###########################################
#pluginbf_printercocina

set (docs README.es)


foreach(dfile ${docs})
   install (FILES ${dfile} DESTINATION share/bulmages/bulmafact-plugins/ RENAME ${dfile}.${plugin})
endforeach(dfile in ${docs})

install(DIRECTORY dbmodels/ 
		DESTINATION share/bulmages/dbmodels/plugins
		PATTERN ".svn" EXCLUDE)

