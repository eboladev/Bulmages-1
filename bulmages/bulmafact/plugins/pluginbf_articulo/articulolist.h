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

#ifndef ARTICLESLIST_H
#define ARTICLESLIST_H

#include "bfcompany.h"
#include "blimportexport.h"
#include "bfsubform.h"
#include "blformlist.h"
#include "pdefs_pluginbf_articulo.h"


/** Clase que maneja el subformulario de la lista de art&iacute;culos.
    Deriva de BfSubForm y agrega la configuracion para tratar especificamente
    con la tabla articulo.
*/
class PLUGINBF_ARTICULO_EXPORT ArticuloListSubForm : public BfSubForm
{
    Q_OBJECT

public:
    ArticuloListSubForm ( QWidget *parent = 0, const char *name = 0 );
    ~ArticuloListSubForm();
};


#include "ui_articleslistbase.h"


/** Clase que maneja la ventana de listado de articulos
    Se encarga del control y manejo de la venta que presenta el listado de articulos.
*/
class PLUGINBF_ARTICULO_EXPORT ArticuloList : public BlFormList, public Ui_ArticuloListBase, public BlImportExport
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
    ArticuloList ( BfCompany *, QWidget *parent = 0, Qt::WFlags flag = 0, edmode editmodo = BL_EDIT_MODE );
    virtual ~ArticuloList();
    QString formaQuery();
    QString idarticulo();
    QString nomarticulo();
    QString codigocompletoarticulo();
    void presentar();
    void editar ( int );
    void imprimir();
    void remove();
    void crear();

public slots:
    virtual void on_mui_importar_clicked();
    virtual void on_mui_exportar_clicked();

signals:
    void selected ( QString );
};

#endif

