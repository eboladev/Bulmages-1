/***************************************************************************
 *   Copyright (C) 2007 by Asociación Iglues                               *
 *   info@iglues.org                                                       *
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

#ifndef CONTRATOSLIST_H
#define CONTRATOSLIST_H

#include "blimportexport.h"
#include "bfcompany.h"
#include "bfsubform.h"
#include "blformlist.h"
#include "bfbulmafact.h"



/** Subformulario de contratoes.
    Esta clase derivada de BfSubForm presenta todo el listado de contratoes.
*/
class ContratosListSubform : public BfSubForm
{
    Q_OBJECT

public:
    ContratosListSubform ( QWidget *parent = 0, const char *name = 0 );
    ~ContratosListSubform();
};


#include "ui_contratoslistbase.h"


/** Esta clase implementa la pantalla de listado de Clientes.
    Deriva de Ficha para la estandarizacion de Pantallas.
    Tiene dos modos de funcionamiento (Edicion y Seleccion)
*/
class ContratosList : public BlFormList, public Ui_ContratosListBase, public BlImportExport
{
    Q_OBJECT

private:
    /// Almacene el idcontratoe del registro seleccionado.
    QString mdb_idcontrato;
    /// Almacena el nomcontratoe del contratoe seleccionado.
    QString mdb_nomcontrato;
    /// Almacena el cifcontratoe del registro seleccionado.
    QString mdb_refcontrato;

public:
    ContratosList ( BfCompany *, QWidget *parent = 0, Qt::WindowFlags flag = 0, edmode editmode = BL_EDIT_MODE );
    ~ContratosList();
    void presenta();
    QString idcontrato();
    QString nomcontrato();
    QString refcontrato();
    void editar ( int );
    void remove ();
    void imprimir ();
    void presentar ();
    virtual void on_mui_actualizar_clicked();
    virtual void on_mui_crear_clicked();
    virtual void on_m_filtro_textChanged ( const QString &text );
    virtual void on_mui_configurar_toggled ( bool checked );

public slots:
    virtual void on_mui_facturar_clicked();

signals:
    void selected ( QString );
};

#endif

