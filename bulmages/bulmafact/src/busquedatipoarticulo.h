/***************************************************************************
 *   Copyright (C) 2005 by Tomeu Borras Riera                              *
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

#ifndef BUSQUEDATIPOARTICULO_H
#define BUSQUEDATIPOARTICULO_H

#include <QLineEdit>
#include <QLabel>

#include "ui_busquedatipoarticulobase.h"


class company;

/// Permite buscar y seleccionar un tipo de art&iacute;culo.
/** Este Widget permite buscar y seleccionar un
    tipo de art&iacute;culo. */
class BusquedaTipoArticulo : public QWidget, public Ui_BusquedaTipoArticuloBase {
    Q_OBJECT

private:
    /// Puntero a la clase company que se propaga por toda la aplicacion para poder trabajar con BAses de datos y hacer paso de mensajes.
    company *m_companyact;
    /// Almacena el idtipo_articulo del elemento seleccionado.
    QString mdb_idtipo_articulo;
    /// Almacena la descripcion del tipo de articulo seleccionado.
    QString mdb_desctipo_articulo;
    /// Almacena el codigo del tipo de articulo seleccionado.
    QString mdb_codtipo_articulo;

public:
    BusquedaTipoArticulo(QWidget *parent = 0);
    ~BusquedaTipoArticulo();
    void setcompany(company *comp) {
        m_companyact = comp;
    };
    virtual QString codtipo_articulo() {
        return m_codtipo_articulo->text();
    };
    virtual QString idtipo_articulo() {
        return mdb_idtipo_articulo;
    };
    virtual QString desctipo_articulo() {
        return desctipo_articulo();
    };
    virtual void setidtipo_articulo(QString val);
    virtual void setcodtipo_articulo(QString val);

public slots:
    virtual void on_mui_buscar_clicked();
    virtual void on_m_codtipo_articulo_textChanged(const QString &);

signals:
    void valueChanged(QString);
};

#endif

