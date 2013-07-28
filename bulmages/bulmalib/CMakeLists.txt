# Copyright (C) 2008 by Leopold Palomo-Avellaneda                       #
# leo@alaxarxa.net                                                      #
# Modified (C) 2013 by Daniel Ripoll                                    #
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
project(bulmalib)

include(MacrosBlI18n)

include_directories(
      ${QT_INCLUDE_DIR}
      ${QT_QTWIDGETS_INCLUDE_DIR}
      ${QT_QTSCRIPT_INCLUDE_DIR}
      ${QT_QTXML_INCLUDE_DIR})

ADD_DEFINITIONS(-DQT_SHARED -DQT_PLUGIN)
ADD_DEFINITIONS(-DQWT_DLL -DQT_DLL -DQWT_MAKEDLL)
ADD_DEFINITIONS(-DBL_BULMALIB)

add_subdirectory(src)
add_subdirectory(plugins)

set (r_files README.Bulmalib.es)
install(FILES ${r_files} DESTINATION share/bulmages/)
