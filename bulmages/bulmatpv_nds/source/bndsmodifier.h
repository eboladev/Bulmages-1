/***************************************************************************
 *                                                                         *
 *   BulmaTPV remote client for Nintendo DS console.                       *
 *   -----------------------------------------------                       *
 *                                                                         *
 *   Copyright (C) 2010 by Fco. Javier M. C.                               *
 *   comercial@todo-redes.com                                              *
 *   http://www.todo-redes.com                                             *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation, either version 3 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program.  If not, see <http://www.gnu.org/licenses/>. *
 *                                                                         *
 ***************************************************************************/

#ifndef BNDSMODIFIER_H
#define BNDSMODIFIER_H

#include <nds.h>
#include <fat.h>
#include <stdlib.h>
#include <string.h>


using namespace std;

/* ----------------------------------------------------------------------- */
/* Tratamiento de archivos BMP                                             */

typedef struct {
	u16 type;			/* Magic identifier            */
	u32 size;                       /* File size in bytes          */
	u16 reserved1, reserved2;
	u32 offset;                     /* Offset to image data, bytes */
} PACKED HEADER;

typedef struct {
	u32 size;			/* Header size in bytes      */
	u32 width,height;		/* Width and height of image */
	u16 planes;			/* Number of colour planes   */
	u16 bits;			/* Bits per pixel            */
	u32 compression;		/* Compression type          */
	u32 imagesize;			/* Image size in bytes       */
	u32 xresolution,yresolution;	/* Pixels per meter          */
	u32 ncolours;			/* Number of colours         */
	u32 importantcolours;		/* Important colours         */
} PACKED INFOHEADER;

typedef struct {
	u8 blue;
	u8 green;
	u8 red;
	u8 reserved;
} PACKED INFOPALETTE;


/* ----------------------------------------------------------------------- */


/// Permite que los articulos tengan modificadores con imagenes.
class BndsModifier
{
  private:
      string m_nombreArchivo;
    
  public:
      BndsModifier();
      ~BndsModifier();
      void screenshotToBmp1BitBase64(const char* filename);
      void show();
      int exec();
      void clearCanvas();
      string getNombreArchivo();
};


#endif

