/***************************************************************************
 *   Copyright (C) 2003 by Tomeu Borras Riera                              *
 *   tborras@conetxia.com                                                  *
 *   http://www.iglues.org Asociación Iglues -- Contabilidad Linux         *
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

#ifndef BCDIARIOIMPRIMIR2TXT_H
#define BCDIARIOIMPRIMIR2TXT_H

#include <QWidget>

#ifndef Q_OS_WIN32
#include <unistd.h>
#include <sys/wait.h>
#endif
/*
#include <errno.h>
#include <sys/types.h>
#include <string>
*/

#include "blwidget.h"
#include "bfcompany.h"
#include "pdefs_pluginbf_diario.h"


/// Imprime el libro diario.
/** */
class PLUGINBC_DIARIO_EXPORT BcDiarioImprimir2TXT : public BlMainCompanyPointer
{
public:
    char *finicial;
    char *ffinal;
    char *fichero;

public:
    BcDiarioImprimir2TXT ( BfCompany * );
    ~BcDiarioImprimir2TXT();
    void inicializa1 ( char *, char * );
    void inicializa2 ( char * );
    void accept();

};

#endif

