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

#ifndef BNDSREADFILECONTENT_H
#define BNDSREADFILECONTENT_H

#include <stdio.h>
#include <stdlib.h>
#include <string>

using namespace std;


class BndsReadFileContent
{
private:
    unsigned int m_fileSize;
    unsigned int m_origin;
    FILE* m_tmpFile;
    unsigned int m_buffer;
    string m_salida;
    bool m_eof;

private:
    void calculateFileLenght();

public:
    BndsReadFileContent();
    ~BndsReadFileContent();
    bool openFileForRead(string fileName);
    string readContent();
    unsigned int getFileSize();
    bool isEof();
};

#endif

