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

#ifndef BUSQUEDAARTICULO_H
#define BUSQUEDAARTICULO_H

#include <QLineEdit>
#include <QLabel>

#include "blcombobox.h"
#include "postgresiface2.h"

#include "ui_busquedaarticulobase.h"
#include "blwidget.h"


class company;


class BusquedaArticuloDelegate : public BlComboBox
{
    Q_OBJECT

private:
    /// Este cursor almacena el listado de series de factura para poder trabajar con ellas.
    cursor2 *m_cursorcombo;
    /// Texto entrado por el usuario (para uso de los plugins)
    QString m_entrada; 

public:
    BusquedaArticuloDelegate ( QWidget *parent = 0 );
    ~BusquedaArticuloDelegate();
    QString entrada();
    QString unicaEleccion(void) ;
    QString eligeUnico(void) ;

public slots:
    virtual void s_editTextChanged ( const QString & );
    virtual void focusOutEvent ( QFocusEvent * event );

};


/// Permite buscar y seleccionar un art&iacute;culo.
/** Muestra la parte del formulario que permite buscar y seleccionar un
    art&iacute;culo. */
class BusquedaArticulo : public BLWidget, public Ui_BusquedaArticuloBase
{
    Q_OBJECT

private:
    /// El Widget almacena el idarticulo seleccionado en esta variable.
    QString mdb_idarticulo;
    /// BusquedaArticulo almacena el nombre del articulo seleccionado en esta variable.
    QString mdb_nomarticulo;
    /// BusquedaArticulo almacena el codigo completo del articulo seleccionado en esta variable de clase.
    QString mdb_codigocompletoarticulo;
    /// Impide que se produzca un dead-lock entre pintar y on_mui_text_changed.
    bool m_semaforo;
    /// Texto entrado por teclado, para comunicacion con pluginalias.
    QString m_entrada;
public:
    BusquedaArticulo ( QWidget *parent = 0 );
    ~BusquedaArticulo();
    void pinta();
    virtual QString codigocompletoarticulo();
    virtual QString idarticulo();
    virtual QString nomarticulo();
    virtual QString valorCampo();
    virtual void setidarticulo ( QString val );
    virtual void setValorCampo ( QString val );
    virtual void setcodigocompletoarticulo ( QString val );

public slots:
    virtual void on_mui_buscar_clicked();
    virtual void on_m_codigocompletoarticulo_textChanged ( const QString & );
    virtual void on_m_codigocompletoarticulo_editingFinished();

signals:
    void valueChanged ( QString );
};

#endif

