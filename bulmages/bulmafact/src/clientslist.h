/***************************************************************************
 *   Copyright (C) 2004 by J. M. Estopa Rey                                *
 *   pepma@telefonica.net                                                  *
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

#ifndef clientslist_H
#define clientslist_H

#include "pgimportfiles.h"
#include "company.h"
#include "subform2bf.h"
#include "listado.h"


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
class ClientsList : public Listado, public Ui_ClientsListBase, public pgimportfiles {
    Q_OBJECT

private:
    /// Almacene el idcliente del registro seleccionado.
    QString mdb_idcliente;
    /// Almacena el nomcliente del cliente seleccionado.
    QString mdb_nomcliente;
    /// Almacena el cifcliente del registro seleccionado.
    QString mdb_cifcliente;

public:
    ClientsList(company *, QWidget *parent = 0, Qt::WFlags flag = 0, edmode editmode = EditMode);
    ~ClientsList();
    void presentar();
    void editar(int);
    void imprimir();
    void crear();
    void borrar();
    QString idclient();
    QString nomclient();
    QString cifclient();
    void submenu(const QPoint &);

public slots:
    virtual void on_mui_exportar_clicked();
    virtual void on_mui_importar_clicked();

signals:
    void selected(QString);
};

#endif

