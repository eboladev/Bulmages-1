/***************************************************************************
 *   Copyright (C) 2006 by Tomeu Borras Riera                              *
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

#ifndef EMPRESABASE_H
#define EMPRESABASE_H

#include <QWorkspace>
#include <QProgressBar>

#include "blpostgresqlclient.h"
#include "listventanas.h"
#include "blworkspace.h"


class EmpresaBase : public BlPostgreSqlClient
{
protected:
    /// Puntero al dock que lista las ventansa. Puesto que esta clase hace el paso de
    /// mensajes tiene el control de la lista de ventanas.
    ListVentanas *m_listventanas;

public:
    /// Puntero al BlWorkspace de la aplicacion. Ya que esta clase crea todas las ventanas,
    /// es comodo tener este puntero inicializado en esta clase.
    BlWorkspace *m_pWorkspace;
    /// El indicador de progreso. Para que determinados procesos puedan indicar su evolucion
    /// de forma general en la ventana principal.
    QProgressBar *m_progressbar;

public:
    EmpresaBase();
    virtual ~EmpresaBase();
    virtual void init ( QString bd, QString tipo );
    QString searchCompany ( QString );
    void setListVentanas ( ListVentanas *doc );
    ListVentanas *listVentanas();
    BlWorkspace *pWorkspace();
    void setWorkspace ( BlWorkspace *qw );
    void setProgressBar ( QProgressBar *pb );
    int meteWindow ( QString nom, QObject *obj, bool compdup = TRUE );
    int seleccionaWindow ( QString nom, QObject *obj );
    int deSeleccionaWindow();
    void sacaWindow ( QObject *nom );
    void s_indexadorCambiaEstado ( bool );
    void muestraPaises();
};

#endif

