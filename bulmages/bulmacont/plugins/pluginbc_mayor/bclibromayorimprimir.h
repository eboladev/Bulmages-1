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

#ifndef LIBROMAYORPRINT_H
#define LIBROMAYORPRINT_H

#include <QWidget>

#ifndef WIN32
#include <unistd.h>
#include <sys/wait.h>
#endif

#include <errno.h>
#include <sys/types.h>
#include <string>

#include "bccompany.h"
#include "blfunctions.h"
#include "blwidget.h"

///
/** */
class BC_EXPORT BcLibroMayorImprimir : public BlMainCompanyPointer
{
public:
    QString codinicial;
    QString codfinal;
    QString finicial;
    QString ffinal;
    char *fichero;

public:
    BcLibroMayorImprimir ( BcCompany *emp );
    ~BcLibroMayorImprimir();
    void inicializa1 ( QString, QString, QString, QString );
    void inicializa2 ( char * );
    void accept();
};

#endif

