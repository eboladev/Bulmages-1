/***************************************************************************
 *   Copyright (C) 2004 by J. M. Estopa Rey                                *
 *   pepma@telefonica.net                                                  *
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

#ifndef PRESUPUESTOCONTABLELIST_H
#define PRESUPUESTOCONTABLELIST_H

#include "blimportexport.h"
#include "bfcompany.h"
#include "bfsubform.h"
#include "blformlist.h"
#include "pdefs_pluginbf_minicontabilidad.h"


/** Subformulario de presupuestocontables.
    Esta clase derivada de BfSubForm presenta todo el listado de presupuestocontables.
*/
class PLUGINBF_MINICONTABILIDAD_EXPORT PresupuestoContableListSubform : public BfSubForm
{
    Q_OBJECT

public:
    PresupuestoContableListSubform ( QWidget *parent = 0, const char *name = 0 );
    ~PresupuestoContableListSubform();
};


#include "ui_presupuestocontablelistbase.h"


/** Esta clase implementa la pantalla de listado de presupuestocontables.
*/
class PLUGINBF_MINICONTABILIDAD_EXPORT PresupuestoContableList : public BlFormList, public Ui_PresupuestoContableListBase, public BlImportExport
{
    Q_OBJECT

private:
    /// Almacene el idpresupuestocontable del registro seleccionado.
    QString mdb_idpresupuestocontable;
    /// Almacena el conceptopresupuestocontable del presupuestocontable seleccionado.
    QString mdb_conceptopresupuestocontable;

public:
    PresupuestoContableList ( BfCompany *, QWidget *parent = 0, Qt::WindowFlags flag = 0, edmode editmode = BL_EDIT_MODE );
    virtual ~PresupuestoContableList();
    void presentar();
    void editar ( int );
    void imprimir();
    void crear();
    void remove();
    QString idpresupuestocontable();
    QString conceptopresupuestocontable();
    void submenu ( const QPoint & );

signals:
    void selected ( QString );
};

#endif

