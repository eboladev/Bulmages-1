/***************************************************************************
 *   Copyright (C) 2009 by Tomeu Borras Riera                              *
 *   tborras@conetxia.com                                                  *
 *   http://www.iglues.org                                                 *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/


#ifndef BL_DEFS
#define BL_DEFS

#ifdef WIN32

#ifdef BL_BULMALIB
#define BL_EXPORT __declspec(dllexport)
#else
#define BL_EXPORT __declspec(dllimport)
#endif

#ifdef BF_BULMAFACT
#define BF_EXPORT __declspec(dllexport)
#else
#define BF_EXPORT __declspec(dllimport)
#endif


#else

#define BL_EXPORT
#define BF_EXPORT

#endif


#define BL_EDIT_MODE   0
#define BL_SELECT_MODE 1


typedef unsigned int edmode;
#endif



