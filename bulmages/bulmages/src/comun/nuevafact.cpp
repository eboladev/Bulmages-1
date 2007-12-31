/***************************************************************************
 *   Copyright (C) 2003 by Tomeu Borras Riera                              *
 *   tborras@conetxia.com                                                  *
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

#include "nuevafact.h"


///
/**
\param parent
\param f
**/
nuevafact::nuevafact ( QWidget *parent, Qt::WFlags f )
        : QDialog ( parent, f )
{
    setupUi ( this );

    /// Signals and slots connections.
    QObject::connect ( PushButton1, SIGNAL ( clicked() ), this, SLOT ( accept() ) );
    QObject::connect ( PushButton1_2, SIGNAL ( clicked() ), this, SLOT ( close() ) );
}


///
/**
**/
nuevafact::~nuevafact()
{}


///
/**
**/
void nuevafact::accept()
{
    /// Falta comprobar que tengas permisos para crear nuevas empresas.
    QString nombredb;
    QString nombreEmp;

    nombredb = bdempresa->text().trimmed() + ejercicioempresa->text().trimmed();
    nombreEmp = nombreempresa->text().trimmed();
    QString cadena = confpr->valor ( CONF_PROGDATA );
    cadena += "dbmodels/creabulmafact --texto " + nombredb + " 1 " + nombreEmp + " " + ejercicioempresa->text().trimmed() + " ";

    system ( cadena.toAscii().constData() );
    close();
}

