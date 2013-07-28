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

#ifndef CARTERACOBROSLIST_H
#define CARTERACOBROSLIST_H

#include "blimportexport.h"
#include "bfsubform.h"
#include "blformlist.h"
#include "bfbulmafact.h"
#include "bfcompany.h"
#include "pdefs_pluginbf_carteracobros.h"

/** Subformulario de contratoes.
    Esta clase derivada de BfSubForm presenta todo el listado de contratoes.
*/
class PLUGINBF_CARTERACOBROS_EXPORT CarteraCobrosListSubForm : public BfSubForm
{
    Q_OBJECT

public:
    CarteraCobrosListSubForm ( QWidget *parent = 0, const char *name = 0 );
    ~CarteraCobrosListSubForm();
};


#include "ui_carteracobroslistbase.h"


/** Esta clase implementa la pantalla de listado de Clientes.
    Deriva de Ficha para la estandarizacion de Pantallas.
    Tiene dos modos de funcionamiento (Edicion y Seleccion)
*/
/// \TODO: Deberia crearse la clase BlFormList para poner en ella mas funcionalidades comunes a los listados.
class PLUGINBF_CARTERACOBROS_EXPORT CarteraCobrosList : public BlFormList, public Ui_CarteraCobrosListBase, public BlImportExport
{
    Q_OBJECT

private:
    /// Indica si la pantalla esta en modo edicion o en modo seleccion.
    edmode m_modo;
    /// Almacene el idcontratoe del registro seleccionado.
    QString mdb_idvencimientoc;
    /// Almacena el nomcontratoe del contratoe seleccionado.
    QString mdb_nomvencimientoc;
    /// Almacena el cifcontratoe del registro seleccionado.
    QString mdb_refcontrato;


public:
    CarteraCobrosList ( BfCompany *, QWidget *parent = 0, Qt::WindowFlags flag = 0, edmode modo = BL_EDIT_MODE );
    ~CarteraCobrosList();
    void presenta();
    QString idvencimientoc();
    void hideBotonera();
    void showBotonera();
    void hideBusqueda();
    void showBusqueda();
    void editar ( int );
    virtual void crear();
    virtual void on_mui_imprimir_clicked();
    virtual void remove();
    virtual void on_m_filtro_textChanged ( const QString &text );
    virtual void on_mui_configurar_toggled ( bool checked );
    virtual void on_mui_actualizar_clicked();

public slots:
    void on_mui_q34_clicked();
    void on_mui_suma_clicked();
signals:
    void selected ( QString );
};

#endif

