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

project(bulmalib)

include(MacrosBlI18n)

set(QT_USE_QTASSISTANT false)
set(QT_USE_QTUITOOLS true)
set(QT_USE_QTXML true)
set(QT_USE_QTSCRIPT true)
set(QT_USE_QTNETWORK true)

include(${QT_USE_FILE})

include_directories(
      ${QT_INCLUDE_DIR}
      ${QT_QTUITOOLS_INCLUDE_DIR}
      ${QT_QTSCRIPT_INCLUDE_DIR}
      ${QT_QTXML_INCLUDE_DIR})

ADD_DEFINITIONS(-DQT_SHARED -DQT_PLUGIN)
ADD_DEFINITIONS(-DQWT_DLL -DQT_DLL -DQWT_MAKEDLL)
ADD_DEFINITIONS(-DBL_BULMALIB)
# Defines if we want our own debugging code
#ADD_DEFINITIONS(-DDEPURA_DEBUG)

IF(USE_PCH)
    ADD_PRECOMPILED_HEADER( bulmalib_pch pch/stableheaders.hpp )
ENDIF(USE_PCH)

add_subdirectory(src)
add_subdirectory(plugins)
add_subdirectory(designer)

set (r_files README.Bulmalib.es)
install(FILES ${r_files} DESTINATION share/bulmages/)
#add_subdirectory(po)
