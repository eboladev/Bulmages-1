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

#ifndef COMISIONESLIST_H
#define COMISIONESLIST_H

#include <QLineEdit>

#include "bfcompany.h"
#include "blfunctions.h"
#include "bfsubform.h"
#include "blformlist.h"
#include "pdefs_pluginbf_asociacion.h"


/** Subformulario especializado en el trabajo con Cobros.
*/
class PLUGINBF_ASOCIACION_EXPORT ComisionesListSubForm : public BfSubForm
{
    Q_OBJECT

public:
    ComisionesListSubForm ( QWidget *parent = 0 );
    ~ComisionesListSubForm();
};


#include "ui_comisioneslistbase.h"


/** Clase que presenta el listado de Cobros.
    Deriva de la clase BlFormList para estandarizacion de Formularios.
    Controla los eventos y la sincronizacion del listado con el filtrado. */
class PLUGINBF_ASOCIACION_EXPORT ComisionesList : public BlFormList, public Ui_ComisionesListBase
{
    Q_OBJECT

private:
    /// Almacena (En el modo seleccion) el identificador del cobro seleccionado.
    QString mdb_idcomision;

public:
    ComisionesList ( QWidget *parent = 0, Qt::WFlags flag = 0, edmode editmodo = BL_EDIT_MODE );
    ComisionesList ( BfCompany *comp = NULL, QWidget *parent = 0, Qt::WFlags flag = 0, edmode editmodo = BL_EDIT_MODE );
    ~ComisionesList();
    void presentar();
    void setMainCompany ( BfCompany *comp );
    QString idcomision();
    void imprimir();
    const QString generaFiltro();
    void remove();
    void crear();
    void editar ( int );
    void submenu ( const QPoint & );

signals:
    /// Estando en modo seleccion al seleccionar un elemento se emite esta se&ntilde;al.
    void selected ( QString );
};

#endif
