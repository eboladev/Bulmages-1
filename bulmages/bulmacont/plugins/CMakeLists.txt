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

add_subdirectory(pluginregistroiva)
add_subdirectory(pluginadocumental)
add_subdirectory(plugincorrector)
add_subdirectory(plugin)
add_subdirectory(plugincanualesods)
add_subdirectory(plugindebugbc)
add_subdirectory(pluginproyectos)
add_subdirectory(plugincaaslods)
add_subdirectory(pluginbalancetree)

# if you want to add some private code
# simple put your add_directory in a
# CMakeLists.txt.private

if(EXISTS "${CMAKE_SOURCE_DIR}/bulmacont/plugins/CMakeLists.txt.private")
   message(STATUS "Private code inserted in bulmacont plugins")
   include("${CMAKE_SOURCE_DIR}/bulmacont/plugins/CMakeLists.txt.private")
endif(EXISTS "${CMAKE_SOURCE_DIR}/bulmacont/plugins/CMakeLists.txt.private")
