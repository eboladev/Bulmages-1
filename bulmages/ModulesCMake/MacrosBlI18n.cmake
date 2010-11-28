# MacrosBlI18n.cmake
# Copyright (C) 2009 Pedro Lopez-Cabanillas <plcl@users.sourceforge.net>
#
# This program is free software; you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation; either version 2 of the License, or
# (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program; if not, write to the Free Software
# Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
# MA 02110-1301, USA

# Macros:
#   ADD_LOCAL_BLI18N_HEADER

MACRO(ADD_LOCAL_BLI18N_HEADER _input) 
    if (NOT EXISTS ${CMAKE_CURRENT_BINARY_DIR}/local_blI18n.h)
    FILE( WRITE ${CMAKE_CURRENT_BINARY_DIR}/local_blI18n.h
          "/* Generated file. Please do not edit! */\n"
          "/* created by MacrosBlI18n.cmake */\n"
          "#ifndef LOCAL_BLI18N\n"
          "#define LOCAL_BLI18N\n\n"
          "#include \"bli18n.h\"\n\n"
          "#ifndef _\n"
          " #define _(String) blI18n(\"${_input}\",String)\n"
          "#endif\n"
          "#ifndef N_\n"
          " #define N_(String,Int) blI18n(\"${_input}\",String)\n"
          "#endif\n\n"
          "#endif /*LOCAL_BLI18N*/\n" )
    endif (NOT EXISTS ${CMAKE_CURRENT_BINARY_DIR}/local_blI18n.h)
ENDMACRO(ADD_LOCAL_BLI18N_HEADER)
