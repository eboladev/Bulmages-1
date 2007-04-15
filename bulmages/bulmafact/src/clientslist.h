/***************************************************************************
 *   Copyright (C) 2004 by J. M. Estopa Rey                                *
 *   pepma@telefonica.net                                                  *
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

#ifndef clientslist_H
#define clientslist_H

#include "pgimportfiles.h"
#include "company.h"
#include "subform2bf.h"
#include "ficha.h"


/** Subformulario de clientes.
    Esta clase derivada de SubForm2Bf presenta todo el listado de clientes.
*/
class ClienteListSubform : public SubForm2Bf {
    Q_OBJECT

public:
    ClienteListSubform(QWidget *parent = 0, const char *name = 0);
    ~ClienteListSubform();
};


#include "ui_clientslistbase.h"


/** Esta clase implementa la pantalla de listado de Clientes.
    Deriva de Ficha para la estandarizacion de Pantallas.
    Tiene dos modos de funcionamiento (Edicion y Seleccion)
*/
/// \TODO: Deberia crearse la clase Listado para poner en ella mas funcionalidades comunes a los listados.
class ClientsList : public Ficha, public Ui_ClientsListBase, public pgimportfiles {
    Q_OBJECT

public:
    enum edmode
    {
        EditMode = 0,
        SelectMode = 1
    };

private:
    /// Indica si la pantalla esta en modo edicion o en modo seleccion.
    edmode m_modo;
    /// Almacene el idcliente del registro seleccionado.
    QString mdb_idcliente;
    /// Almacena el nomcliente del cliente seleccionado.
    QString mdb_nomcliente;
    /// Almacena el cifcliente del registro seleccionado.
    QString mdb_cifcliente;
    /// Puntero a la clase company para poder trabajar con la Base de Datos y poder hacer paso de mensajes.
    company *m_companyact;

public:
    ClientsList(company *, QWidget *parent = 0, Qt::WFlags flag = 0, edmode editmode = EditMode);
    ~ClientsList();
    void selectMode();
    void editMode();
    void presenta();
    QString idclient();
    QString nomclient();
    QString cifclient();
    void hideBotonera();
    void showBotonera();
    void hideBusqueda();
    void showBusqueda();
    void editar(int);

public slots:
    virtual void on_m_filtro_textChanged(const QString &text);
    void on_mui_list_itemDoubleClicked(QTableWidgetItem *);
    virtual void on_mui_crear_clicked();
    virtual void on_mui_editar_clicked();
    virtual void on_mui_imprimir_clicked();
    virtual void on_mui_informeclientes_clicked();
    virtual void on_mui_actualizar_clicked();
    virtual void on_mui_exportar_clicked();
    virtual void on_mui_importar_clicked();
    virtual void on_mui_borrar_clicked();
    virtual void on_mui_configurar_toggled(bool checked);

signals:
    void selected(QString);
};

#endif

