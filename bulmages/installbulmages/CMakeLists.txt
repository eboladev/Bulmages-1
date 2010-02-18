# Copyright (C) 2008 by Leopold Palomo-Avellaneda                       #
# leo@alaxarxa.net                                                      #
# http://www.iglues.org                                                 #
#                                                                       #
# This program is free software; you can redistribute it and/or modify  #
# it under the terms of the GNU General Public License as published by  #
# the Free Software Foundation; either version 2 of the License, or     #
# (at your option) any later version.                                   #
#                                                                       #
# This program is distributed in the hope that it will be useful,       #
# but WITHOUT ANY WARRANTY; without even the implied warranty of        #
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         #
# GNU General Public License for more details.                          #
#                                                                       #
# You should have received a copy of the GNU General Public License     #
# along with this program; if not, write to the                         #
# Free Software Foundation, Inc.,                                       #
# 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             #
#                                                                       #

configure_file( ${CMAKE_SOURCE_DIR}/installbulmages/version.in version @ONLY)
configure_file( ${CMAKE_SOURCE_DIR}/installbulmages/bulmages.conf.in bulmages.conf @ONLY)
configure_file( ${CMAKE_SOURCE_DIR}/installbulmages/bulmafact.conf.in bulmafact.conf @ONLY)
configure_file( ${CMAKE_SOURCE_DIR}/installbulmages/bulmacont.conf.in bulmacont.conf @ONLY)
configure_file( ${CMAKE_SOURCE_DIR}/installbulmages/bulmatpv.conf.in bulmatpv.conf @ONLY)
configure_file( ${CMAKE_SOURCE_DIR}/installbulmages/examples/scripts/installbulmages-db.in installbulmages-db @ONLY)
configure_file( ${CMAKE_SOURCE_DIR}/installbulmages/examples/scripts/actualizabulmacont.in actualizabulmacont @ONLY)
configure_file( ${CMAKE_SOURCE_DIR}/installbulmages/examples/scripts/actualizabulmafact.in actualizabulmafact @ONLY)
configure_file( ${CMAKE_SOURCE_DIR}/installbulmages/examples/scripts/creabulmacont.in creabulmacont @ONLY)
configure_file( ${CMAKE_SOURCE_DIR}/installbulmages/examples/scripts/creabulmafact.in creabulmafact @ONLY)
configure_file( ${CMAKE_SOURCE_DIR}/installbulmages/bgtrml2pdf/bgtrml2pdf.in bgtrml2pdf.py  @ONLY)

configure_file( ${CMAKE_SOURCE_DIR}/installbulmages/i18n/create_gettext_files.in create_gettext_files @ONLY)
configure_file( ${CMAKE_SOURCE_DIR}/installbulmages/i18n/create_message_catalog.in create_message_catalog @ONLY)
configure_file( ${CMAKE_SOURCE_DIR}/installbulmages/i18n/update_message_catalog_lang.in update_message_catalog_lang @ONLY)



install(DIRECTORY ainteligentes canuales planescontables rev-conf
				openreports icons images dbmodels ayuda
				DESTINATION share/bulmages
 				PATTERN ".svn" EXCLUDE)

install(FILES ${CMAKE_BINARY_DIR}/bgtrml2pdf.py DESTINATION bin RENAME bgtrml2pdf
                PERMISSIONS OWNER_EXECUTE OWNER_WRITE OWNER_READ
                GROUP_EXECUTE GROUP_READ
                WORLD_READ WORLD_EXECUTE)

file(GLOB f_rml bgtrml2pdf/*.py)
install(FILES ${f_rml} 
		DESTINATION share/bulmages/bgtrml2pdf)


FILE(GLOB f_css css/*.css)

foreach(fcss ${f_css})
    FILE(READ ${fcss} VAR_CSS)
    GET_FILENAME_COMPONENT(css_file ${fcss} NAME)
    FILE(APPEND ${CMAKE_BINARY_DIR}/css/${css_file} "${VAR_CSS}" )
endforeach(fcss in ${f_css})


install(DIRECTORY ainteligentes canuales planescontables ${CMAKE_BINARY_DIR}/css
            openreports icons images
            DESTINATION share/bulmages
            PATTERN ".svn" EXCLUDE)

install(DIRECTORY man DESTINATION share/
				PATTERN ".svn" EXCLUDE)
			
FILE(GLOB files COPYING version)
install(FILES ${files} DESTINATION share/bulmages)

FILE(GLOB conf_etc bulmacont.conf bulmafact.conf bulmages.conf bulmatpv.conf)
install(FILES ${conf_etc} DESTINATION ${BULMAGES_CONFIG_DIR})

install(DIRECTORY examples/databases DESTINATION share/bulmages/examples/
            PATTERN ".svn" EXCLUDE)

FILE(GLOB scripts_ex installbulmages-db
                     actualizabulmacont
                     actualizabulmafact
                     creabulmacont
                     creabulmafact)
install(FILES ${scripts_ex} DESTINATION share/bulmages/examples/scripts
	PERMISSIONS OWNER_EXECUTE OWNER_WRITE OWNER_READ
                	    GROUP_EXECUTE GROUP_READ
                            WORLD_READ WORLD_EXECUTE)


