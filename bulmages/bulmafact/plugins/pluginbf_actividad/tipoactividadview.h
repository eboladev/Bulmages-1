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

#ifndef TIPOACTIVIDADVIEW_H
#define TIPOACTIVIDADVIEW_H

#include "blfunctions.h"
#include <ui_tipoactividadbase.h>

#include "bfform.h"
#include "blpostgresqlclient.h"


/// Muestra y administra la ventana con la informaci&oacute;n de un trabajador.
/** */
class TipoActividadView : public BfForm, public Ui_TipoActividadBase
{
    Q_OBJECT

private:
    BlDbRecordSet *m_cursortipoactividades;
    /// Indica cual es el objeto que se esta mostrando.
    QString mdb_idtipoactividad;
    /// Indica el archivo de imagen que se esta mostrando. Y si se ha cambiado la imagen
    /// tambien lo indica.
    QString m_archivoimagen;
    /// Indica en la lista de trabajadores cual es el item seleccionado.
    QListWidgetItem *m_item;

private:
    /// Se encarga de hacer la carga del query inicial y de mostrar la lista bien
    /// y presentar el elemento que se especifique.
    void pintar();
    virtual void imprimir();

public:
    TipoActividadView ( BfCompany * emp, QWidget *parent = 0 );
    ~TipoActividadView();
    bool trataModificado();
    QString idtipoactividad();
    virtual void on_mui_guardar_clicked();
    virtual void on_mui_borrar_clicked();

private slots:
    virtual void on_mui_lista_currentItemChanged ( QListWidgetItem *cur, QListWidgetItem *prev );
    virtual void on_mui_nuevo_clicked();
};

#endif

