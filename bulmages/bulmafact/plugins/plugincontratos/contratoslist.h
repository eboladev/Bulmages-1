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

#include "pgimportfiles.h"
#include "company.h"
#include "subform2bf.h"
#include "blformlist.h"
#include "bulmafact.h"


class myplugincont : public QObject
{
    Q_OBJECT

public:
    BlPostgreSqlClient *m_conexionbase;
    Bulmafact *m_bulmafact;

public:
    myplugincont();
    ~myplugincont();
    void inicializa ( Bulmafact * );

public slots:
    void elslot();
};


/** Subformulario de contratoes.
    Esta clase derivada de SubForm2Bf presenta todo el listado de contratoes.
*/
class ContratosListSubform : public SubForm2Bf
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
class ContratosList : public BlFormList, public Ui_ContratosListBase, public pgimportfiles
{
    Q_OBJECT

public:
    enum edmode {
        EditMode = 0,
        SelectMode = 1
    };

private:
    /// Indica si la pantalla esta en modo edicion o en modo seleccion.
    edmode m_modo;
    /// Almacene el idcontratoe del registro seleccionado.
    QString mdb_idcontrato;
    /// Almacena el nomcontratoe del contratoe seleccionado.
    QString mdb_nomcontrato;
    /// Almacena el cifcontratoe del registro seleccionado.
    QString mdb_refcontrato;

public:
    ContratosList ( Company *, QWidget *parent = 0, Qt::WFlags flag = 0, edmode editmode = EditMode );
    ~ContratosList();
    void selectMode();
    void editMode();
    void presenta();
    QString idcontrato();
    QString nomcontrato();
    QString refcontrato();
    void hideBotonera();
    void showBotonera();
    void hideBusqueda();
    void showBusqueda();
    void editar ( int );
    virtual void on_mui_actualizar_clicked();
    virtual void on_mui_crear_clicked();
    virtual void on_mui_editar_clicked();
    virtual void on_mui_imprimir_clicked();
    virtual void on_mui_borrar_clicked();
    virtual void on_m_filtro_textChanged ( const QString &text );
    virtual void on_mui_configurar_toggled ( bool checked );

public slots:
    void on_mui_list_itemDoubleClicked ( QTableWidgetItem * );
    virtual void on_mui_facturar_clicked();

signals:
    void selected ( QString );
};

#endif

