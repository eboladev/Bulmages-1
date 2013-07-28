/***************************************************************************
 *   Copyright (C) 2005 by Tomeu Borras Riera                              *
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

#ifndef TIPOSARTICULOVIEW_H
#define TIPOSARTICULOVIEW_H

#include "blfunctions.h"
#include "ui_tiposarticulobase.h"
#include "bfform.h"
#include "pdefs_pluginbf_articulo.h"


class BfCompany;


/// Muestra y administra la ventana con la informaci&oacute;n de un tipo de art&iacute;culo.
/** */
class PLUGINBF_ARTICULO_EXPORT TipoArticuloList : public BfForm, public Ui_tiposarticulobase
{
    Q_OBJECT

private:

    /// Indica cual es el registro que se esta modificando. Sirve para saber los
    /// cambios que hay que guardar cuando se ha modificado.
    QString m_idtipo;
    /// Indica si es modo consulta o modo edicion. (altera el comportamiento del
    /// doble click sobre la lista)
    bool m_selectMode;
    /// Este semaforo indica si se esta en el proceso de pintado.
    bool m_semaforoPintar;

public:
    TipoArticuloList ( BfCompany *, QWidget *parent = 0, bool selectMode = false );
    ~TipoArticuloList();
    bool trataModificado();
    void mostrarplantilla();
    void setSelectMode();
    void setEditMode();
    QString codtipo_articulo();
    QString idtipo_articulo();
    QString desctipo_articulo();
    virtual int save();
    virtual int remove();
    virtual void on_mui_borrar_clicked();
    void pintar ( QTreeWidgetItem * );

public slots:
    virtual void pintar();
    virtual void on_m_listTipos_itemDoubleClicked ( QTreeWidgetItem * item, int column );
    virtual void on_mui_crear_clicked();
    virtual void on_m_listTipos_currentItemChanged ( QTreeWidgetItem *current, QTreeWidgetItem *previous );


signals:
    void selected ( QString );
};

#endif

