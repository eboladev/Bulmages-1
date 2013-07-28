/***************************************************************************
 *   Copyright (C) 2004 by Tomeu Borras Riera                              *
 *   tborras@conetxia.com                                                  *
 *   http://www.iglues.org                                                 *
 *                                                                         *
 *   Copyright (C) 2009 by Arturo Martin Llado                             *
 *   amartin@conetxia.com                                                  *
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

#ifndef RECIBOSLIST_H
#define RECIBOSLIST_H

#include <QtWidgets/QLineEdit>

#include "bfcompany.h"
#include "blfunctions.h"
#include "bfsubform.h"
#include "blformlist.h"


/** Subformulario especializado en el trabajo con Cobros.
*/
class RecibosListSubForm : public BfSubForm
{
    Q_OBJECT

public:
    RecibosListSubForm ( QWidget *parent = 0 );
    ~RecibosListSubForm();
};


#include "ui_reciboslistbase.h"


/** Clase que presenta el listado de Cobros.
    Deriva de la clase BlFormList para estandarizacion de Formularios.
    Controla los eventos y la sincronizacion del listado con el filtrado. */
class RecibosList : public BlFormList, public Ui_RecibosListBase
{
    Q_OBJECT

private:
    /// Almacena (En el modo seleccion) el identificador del cobro seleccionado.
    QString mdb_idrecibo;

public:
    RecibosList ( QWidget *parent = 0, Qt::WindowFlags flag = 0, edmode editmodo = BL_EDIT_MODE );
    RecibosList ( BfCompany *comp = NULL, QWidget *parent = 0, Qt::WindowFlags flag = 0, edmode editmodo = BL_EDIT_MODE );
    ~RecibosList();
    void presentar();
    void setMainCompany ( BfCompany *comp );
    QString idrecibo();
    void imprimir();
    const QString generaFiltro();
    void remove();
    void crear();
    void editar ( int );
    void submenu ( const QPoint & );
    void iniciaForm();

signals:
    /// Estando en modo seleccion al seleccionar un elemento se emite esta se&ntilde;al.
    void selected ( QString );
};

#endif

