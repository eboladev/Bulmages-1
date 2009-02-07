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

#ifndef BUSQUEDA_H
#define BUSQUEDA_H

#include <QLineEdit>
#include <QLabel>

#include "blwidget.h"
#include "ui_busquedabase.h"


/// Permite buscar y seleccionar un profesor.
/** Implementa un Widget que permite buscar y seleccionar un
    profesor de BulmaFact de forma generica. */
class Busqueda : public BlWidget, public Ui_BusquedaBase
{
    Q_OBJECT

public:
    /// QHash de los valores a comprobar
    QMap <QString, QString> m_valores;

private:
    /// Almacena la tabla sobre la que vamos a buscar.
    QString m_tabla;

    /// Almacena el id de la tabla seleccionada.
    QString mdb_id;
    /// Almacena el nombreprofesor del profesor seleccionado.
    /// Impide que se produzca un dead-lock entre pintar y on_mui_text_changed.
    bool m_semaforo;

public:
    Busqueda ( QWidget *parent = 0 );
    ~Busqueda();
    void pinta();
    virtual QString id();
    virtual QString valorCampo();
    virtual QString valorCampo(QString campo);
    virtual QString tableName();
    virtual void setId ( QString val );
    virtual void setValorCampo ( QString val );
    virtual void setValorCampo ( QString campo, QString val );
    virtual void setLabel(QString label);
    virtual void setTableName(QString tableName);

public slots:
    virtual void on_mui_buscar_clicked();
    virtual void on_mui_borrar_clicked();
    virtual void on_m_inputBusqueda_textChanged ( const QString & );
    virtual void on_m_inputBusqueda_editingFinished();

signals:
    void valueChanged ( QString );
};

#endif

