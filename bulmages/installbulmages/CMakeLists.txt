# Copyright (C) 2008 by Leopold Palomo-Avellaneda                       #
# leo@alaxarxa.net                                                      #
# Modified (C) 2013 by Daniel Ripoll                                                     #
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
#########################################################################

configure_file( ${CMAKE_SOURCE_DIR}/installbulmages/version.in ${CMAKE_BINARY_DIR}/version @ONLY)
configure_file( ${CMAKE_SOURCE_DIR}/installbulmages/bulmages.conf.in ${CMAKE_BINARY_DIR}/bulmages.conf @ONLY)
configure_file( ${CMAKE_SOURCE_DIR}/installbulmages/bulmafact.conf.in ${CMAKE_BINARY_DIR}/bulmafact.conf @ONLY)
configure_file( ${CMAKE_SOURCE_DIR}/installbulmages/bulmacont.conf.in ${CMAKE_BINARY_DIR}/bulmacont.conf @ONLY)
configure_file( ${CMAKE_SOURCE_DIR}/installbulmages/bulmatpv.conf.in ${CMAKE_BINARY_DIR}/bulmatpv.conf @ONLY)
configure_file( ${CMAKE_SOURCE_DIR}/installbulmages/examples/scripts/installbulmages-db.in ${CMAKE_BINARY_DIR}/installbulmages-db @ONLY)
configure_file( ${CMAKE_SOURCE_DIR}/installbulmages/examples/scripts/actualizabulmacont.in ${CMAKE_BINARY_DIR}/actualizabulmacont @ONLY)
configure_file( ${CMAKE_SOURCE_DIR}/installbulmages/examples/scripts/actualizabulmafact.in ${CMAKE_BINARY_DIR}/actualizabulmafact @ONLY)
configure_file( ${CMAKE_SOURCE_DIR}/installbulmages/examples/scripts/creabulmacont.in ${CMAKE_BINARY_DIR}/creabulmacont @ONLY)
configure_file( ${CMAKE_SOURCE_DIR}/installbulmages/examples/scripts/creabulmafact.in ${CMAKE_BINARY_DIR}/creabulmafact @ONLY)
configure_file( ${CMAKE_SOURCE_DIR}/installbulmages/bgtrml2pdf/bgtrml2pdf.in ${CMAKE_BINARY_DIR}/bgtrml2pdf.py  @ONLY)

configure_file( ${CMAKE_SOURCE_DIR}/installbulmages/i18n/create_gettext_files.in ${CMAKE_BINARY_DIR}/create_gettext_files @ONLY)
configure_file( ${CMAKE_SOURCE_DIR}/installbulmages/i18n/create_message_catalog.in ${CMAKE_BINARY_DIR}/create_message_catalog @ONLY)
configure_file( ${CMAKE_SOURCE_DIR}/installbulmages/i18n/update_message_catalog_lang.in ${CMAKE_BINARY_DIR}/update_message_catalog_lang @ONLY)

configure_file( ${CMAKE_SOURCE_DIR}/installbulmages/applications/bulmasetup.desktop.in ${CMAKE_BINARY_DIR}/applications/bulmasetup.desktop @ONLY)
configure_file( ${CMAKE_SOURCE_DIR}/installbulmages/applications/bulmages.desktop.in ${CMAKE_BINARY_DIR}/applications/bulmages.desktop @ONLY)
configure_file( ${CMAKE_SOURCE_DIR}/installbulmages/applications/bulmacont.desktop.in ${CMAKE_BINARY_DIR}/applications/bulmacont.desktop @ONLY)
configure_file( ${CMAKE_SOURCE_DIR}/installbulmages/applications/bulmafact.desktop.in ${CMAKE_BINARY_DIR}/applications/bulmafact.desktop @ONLY)
configure_file( ${CMAKE_SOURCE_DIR}/installbulmages/applications/bulmatpv.desktop.in ${CMAKE_BINARY_DIR}/applications/bulmatpv.desktop @ONLY)

configure_file( ${CMAKE_SOURCE_DIR}/installbulmages/menu/bulmasetup.menu.in ${CMAKE_BINARY_DIR}/menu/bulmasetup.menu @ONLY)
configure_file( ${CMAKE_SOURCE_DIR}/installbulmages/menu/bulmages.menu.in ${CMAKE_BINARY_DIR}/menu/bulmages.menu @ONLY)
configure_file( ${CMAKE_SOURCE_DIR}/installbulmages/menu/bulmacont.menu.in ${CMAKE_BINARY_DIR}/menu/bulmacont.menu @ONLY)
configure_file( ${CMAKE_SOURCE_DIR}/installbulmages/menu/bulmafact.menu.in ${CMAKE_BINARY_DIR}/menu/bulmafact.menu @ONLY)
configure_file( ${CMAKE_SOURCE_DIR}/installbulmages/menu/bulmatpv.menu.in ${CMAKE_BINARY_DIR}/menu/bulmatpv.menu @ONLY)


install(DIRECTORY ainteligentes canuales planescontables rev-conf
				openreports icons images dbmodels ayuda confs
				DESTINATION share/bulmages)

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
            DESTINATION share/bulmages)

install(DIRECTORY man pixmaps ${CMAKE_BINARY_DIR}/applications ${CMAKE_BINARY_DIR}/menu doc DESTINATION share/)

FILE(GLOB files ${CMAKE_CURRENT_SOURCE_DIR}/COPYING.txt ${CMAKE_BINARY_DIR}/version)
install(FILES ${files} DESTINATION share/bulmages)

FILE(GLOB conf_etc 
	${CMAKE_BINARY_DIR}/bulmacont.conf 
	${CMAKE_BINARY_DIR}/bulmafact.conf 
	${CMAKE_BINARY_DIR}/bulmages.conf 
	${CMAKE_BINARY_DIR}/bulmatpv.conf)
install(FILES ${conf_etc} DESTINATION ${BULMAGES_CONFIG_DIR})

install(DIRECTORY examples/databases DESTINATION share/bulmages/examples/)

FILE(GLOB scripts_ex ${CMAKE_BINARY_DIR}/installbulmages-db
                     ${CMAKE_BINARY_DIR}/actualizabulmacont
                     ${CMAKE_BINARY_DIR}/actualizabulmafact
                     ${CMAKE_BINARY_DIR}/creabulmacont
                     ${CMAKE_BINARY_DIR}/creabulmafact)

install(FILES ${scripts_ex} DESTINATION share/bulmages/examples/scripts
	PERMISSIONS OWNER_WRITE OWNER_READ
                	    GROUP_READ
                            WORLD_READ )


add_custom_command (
	COMMENT "Creando Menus"
	OUTPUT ${CMAKE_BINARY_DIR}/out.temp
	COMMAND update-menus
)
