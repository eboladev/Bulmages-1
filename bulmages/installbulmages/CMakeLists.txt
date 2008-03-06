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
configure_file( ${CMAKE_SOURCE_DIR}/installbulmages/installbulmages-db.in installbulmages-db @ONLY)
configure_file( ${CMAKE_SOURCE_DIR}/installbulmages/uninstallbulmages.in uninstallbulmages  @ONLY)
configure_file( ${CMAKE_SOURCE_DIR}/installbulmages/dbmodels/actualizabulmacont.in actualizabulmacont @ONLY)
configure_file( ${CMAKE_SOURCE_DIR}/installbulmages/dbmodels/actualizabulmafact.in actualizabulmafact @ONLY)
configure_file( ${CMAKE_SOURCE_DIR}/installbulmages/dbmodels/creabulmacont.in creabulmacont @ONLY)
configure_file( ${CMAKE_SOURCE_DIR}/installbulmages/dbmodels/creabulmafact.in creabulmafact @ONLY)

install(DIRECTORY ainteligentes bgtrml2pdf 
				canuales planescontables 
				openreports icons 
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
install(FILES ${conf_etc} DESTINATION ${BULMAGES_CONFIG_DIR})


set (scripts installbulmages-client
			installbulmages-server
			deinstallbulmages
			${CMAKE_BINARY_DIR}/installbulmages-db
			${CMAKE_BINARY_DIR}/uninstallbulmages)
install(FILES ${scripts} DESTINATION share/bulmages
		PERMISSIONS OWNER_EXECUTE OWNER_WRITE OWNER_READ
                  GROUP_EXECUTE GROUP_READ 
                  WORLD_READ WORLD_EXECUTE)

set (scriptsdb  ${CMAKE_BINARY_DIR}/actualizabulmacont 
			    ${CMAKE_BINARY_DIR}/actualizabulmafact 
				${CMAKE_BINARY_DIR}/creabulmacont
				${CMAKE_BINARY_DIR}/creabulmafact)
install(FILES ${scriptsdb} DESTINATION share/bulmages/dbmodels
		PERMISSIONS OWNER_EXECUTE OWNER_WRITE OWNER_READ
                  GROUP_EXECUTE GROUP_READ GROUP_READ 
                  WORLD_READ WORLD_EXECUTE)



