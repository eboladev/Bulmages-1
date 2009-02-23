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

configure_file( ${CMAKE_SOURCE_DIR}/bulmasetup/config.in config.py @ONLY)

configure_file( ${CMAKE_SOURCE_DIR}/bulmasetup/bulmasetup.in bulmasetup.py @ONLY)


install(FILES ${CMAKE_BINARY_DIR}/bulmasetup.py DESTINATION bin RENAME bulmasetup
               PERMISSIONS OWNER_EXECUTE OWNER_WRITE OWNER_READ
                	    GROUP_EXECUTE GROUP_READ
                            WORLD_READ WORLD_EXECUTE)

install(FILES ${CMAKE_BINARY_DIR}/config.py DESTINATION share/bulmages/bulmasetup)


file(GLOB py_files *.py)
install(FILES ${py_files} 
		DESTINATION share/bulmages/bulmasetup)

