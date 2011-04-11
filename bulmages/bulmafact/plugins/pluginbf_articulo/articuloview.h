/***************************************************************************
 *   Copyright (C) 2004 by Alvaro de Miguel                                *
 *   alvaro_demiguel@gmail.com                                             *
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

#ifndef ARTICULOVIEW_H
#define ARTICULOVIEW_H

#include "ui_articleeditbase.h"
#include "blpostgresqlclient.h"
#include "bfform.h"
#include "pdefs_pluginbf_articulo.h"


class BfCompany;


/** Esta clase controla la ventana de un articulo.
    Deriva de la clase BfForm para el control de la ventana y de la clase Articulo
    para el control con la base de datos.
*/
class PLUGINBF_ARTICULO_EXPORT ArticuloView : public BfForm, public Ui_ArticuloBase
{
    Q_OBJECT

private:
    /// La imagen de un articulo es una excepcion ya que no se guarda en la base de datos
    /// por ese motivo la almacenamos en esta variable que hace que corresponda con un archivo.
    QString m_archivoimagen;
    BlDbRecordSet *m_cursorcombo;

public:
    ArticuloView ( BfCompany *emp, QWidget *parent = 0 );
    ~ArticuloView();

public:
    virtual void pintarPost();
    virtual int cargarPost ( QString );
    virtual int guardarPost();
    virtual int borrarPre();

public slots:
    virtual void on_mui_codigocompletoarticulo_editingFinished();
    virtual void on_mui_cambiarimagen_clicked();
    virtual void on_mui_borrarimagen_clicked();
    virtual void on_mui_idfamilia_valueChanged(QString);
};

#endif

