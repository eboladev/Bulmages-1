/***************************************************************************
 *   Copyright (C) 2007 by Tomeu Borras Riera                              *
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

#ifndef ARTICLESLISTTPV_H
#define ARTICLESLISTTPV_H

#include "bfcompany.h"
#include "blimportexport.h"
#include "bfsubform.h"
#include "blformlist.h"


/** Clase que maneja el subformulario de la lista de art&iacute;culos.
    Deriva de BtSubForm y agrega la configuracion para tratar especificamente
    con la tabla articulo.
*/
class ArticuloList1SubForm1 : public BfSubForm
{
    Q_OBJECT

public:
    ArticuloList1SubForm1 ( QWidget *parent = 0, const char *name = 0 );
    ~ArticuloList1SubForm1();
};


#include "ui_articulosubformbase.h"


/** Clase que maneja la ventana de listado de articulos
    Se encarga del control y manejo de la venta que presenta el listado de articulos.
*/
class ArticuloList1 : public BlFormList, public Ui_ArticuloSubFormBase
{
    Q_OBJECT

private:
    /// El identificador de articulo para el modo seleccion. Indica el articulo seleccionado.
    QString mdb_idarticulo;
    /// El nombre del articulo seleccionado si estamos en modo seleccion.
    QString mdb_nomarticulo;
    /// El codigo completo del articulo seleccionado si estamos en modo seleccion.
    QString mdb_codigocompletoarticulo;

public:
    ArticuloList1 ( BfCompany *, QWidget *parent = 0, Qt::WindowFlags flag = 0, edmode editmodo = BL_EDIT_MODE );
    virtual ~ArticuloList1();
    QString formaQuery();
    QString idarticulo();
    QString nomarticulo();
    QString codigocompletoarticulo();
    void presentar();
    void submenu ( const QPoint & );
    virtual void on_mui_list_cellDoubleClicked ( int, int );

public slots:
    virtual void on_mui_seleccionar_clicked();

signals:
    void selected ( QString );
};

#endif

