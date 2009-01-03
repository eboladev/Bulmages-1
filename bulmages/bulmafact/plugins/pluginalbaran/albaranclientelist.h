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

#ifndef CLIENTDELIVNOTESLIST_H
#define CLIENTDELIVNOTESLIST_H

#include "busquedacliente.h"
#include "busquedaarticulo.h"
#include "company.h"
#include "subform2bf.h"
#include "listado.h"


/// Subformulario de albaranes de clientes.
/** Esta clase se encarga de implementar el subformulario de albaranes a clientes.
    Es una clase derivada de SubForm2Bf, donde estan las funciones especificas de la facturacion.
    que a su vez deriva de SubForm3 que tiene las funciones de subformularios.

    La funcionalidad de esta clase es configurar el subformulario para que funcione
    con la tabla de albaranes.

    Reimplementa los metodos de cargar para producir el query adecuado.
*/
class AlbaranClienteListSubform : public SubForm2Bf
{
    Q_OBJECT

public:
    AlbaranClienteListSubform ( QWidget *parent = 0 );
    ~AlbaranClienteListSubform();

public slots:
    virtual void cargar();
    virtual void cargar ( QString query );
};


#include "ui_albaranclientelistbase.h"


/** Albaranes a Clientes.
    Esta clase representa la pantalla de albaranes a clientes.
*/
/// \TODO: Deberia crearse una clase derivada de Ficha que fuese Listado y que
/// incorporase algunas funcionalidades comunes a todos los listados.
class AlbaranClienteList : public Listado, public Ui_AlbaranClienteListBase
{
    Q_OBJECT

private:
    /// En el modo selector esta variable indica el elemento seleccionado.
    QString mdb_idalbaran;

public:
    AlbaranClienteList ( QWidget *parent = 0, Qt::WFlags flag = 0, edmode editmodo = EditMode );
    AlbaranClienteList ( Company *, QWidget *parent = 0, Qt::WFlags flag = 0, edmode editmodo = EditMode );
    void setEmpresaBase ( Company * );
    ~AlbaranClienteList();
    void presentar();
    void imprimir();
    void editar ( int );
    void borrar();
    void crear();
    void setidcliente ( QString val );
    QString idCliDelivNote();
    QString generarFiltro();
    void iniciaForm();

signals:
    /// Estando en modo seleccion al seleccionar un elemento se emite esta se&ntilde;al.
    void selected ( QString );
};

#endif

