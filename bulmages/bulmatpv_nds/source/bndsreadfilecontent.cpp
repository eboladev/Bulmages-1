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


#include "bndsreadfilecontent.h"


BndsReadFileContent::BndsReadFileContent()
{
    m_fileSize = 0;
    m_origin = 0;
    m_tmpFile = NULL;
    m_buffer = sizeof(char) * 1024; // 1KB buffer
    m_salida = "";
    m_eof = false;
}


BndsReadFileContent::~BndsReadFileContent()
{
}


unsigned int BndsReadFileContent::getFileSize()
{
    return m_fileSize;
}


bool BndsReadFileContent::openFileForRead(string fileName)
{

      if ( !(m_tmpFile = fopen (fileName.c_str(), "rb") )) {
	  /// El archivo no existe.
	  return false;
      } else {
	  calculateFileLenght();
	  fseek(m_tmpFile, 0, SEEK_SET);
	  return true;
      } // end if

}

bool BndsReadFileContent::isEof()
{
    return m_eof;
}


void BndsReadFileContent::calculateFileLenght()
{
      /// Se calcula el tamanyo de los datos del archivo.
      fseek(m_tmpFile, 0, SEEK_END);
      m_fileSize = ftell(m_tmpFile);
      rewind(m_tmpFile);
}


string BndsReadFileContent::readContent()
{
    string salida = "";
    int bufferSize = 0;

    /// Calcula la cantidad de datos a leer del archivo.
    if ((m_fileSize - m_origin) >= m_buffer) {
	bufferSize = m_buffer;
    } else {
	bufferSize = (m_fileSize - m_origin);
	m_eof = true;
    } // end if

    char* data = (char*) malloc (bufferSize + 1);

    /// Lee datos de archivo.
    fread(data, bufferSize, 1, m_tmpFile);

    if (ferror(m_tmpFile)) {
	/// Error de lectura del archivo.
    } // end if

    m_origin += m_buffer;

    salida = data;
    free(data);
    
    return salida.substr(0, bufferSize);
}




