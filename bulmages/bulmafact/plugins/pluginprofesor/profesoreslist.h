/***************************************************************************
 *   Copyright (C) 2004 by Tomeu Borras Riera                              *
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

#ifndef COBROSLIST_H
#define COBROSLIST_H

#include <QLineEdit>

#include "company.h"
#include "funcaux.h"
#include "subform2bf.h"
#include "listado.h"


/** Subformulario especializado en el trabajo con Cobros.
*/
class ProfesoresListSubForm : public SubForm2Bf
{
    Q_OBJECT

public:
    ProfesoresListSubForm ( QWidget *parent = 0 );
    ~ProfesoresListSubForm();
};


#include "ui_profesoreslistbase.h"


/** Clase que presenta el listado de Cobros.
    Deriva de la clase Ficha para estandarizacion de Formularios.
    Controla los eventos y la sincronizacion del listado con el filtrado. */
class ProfesoresList : public Listado, public Ui_ProfesoresListBase
{
    Q_OBJECT

private:
    /// Almacena (En el modo seleccion) el identificador del cobro seleccionado.
    QString mdb_idprofesor;

public:
    ProfesoresList ( QWidget *parent = 0, Qt::WFlags flag = 0, edmode editmodo = EditMode );
    ProfesoresList ( Company *comp = NULL, QWidget *parent = 0, Qt::WFlags flag = 0, edmode editmodo = EditMode );
    ~ProfesoresList();
    void presentar();
    void setEmpresaBase ( Company *comp );
    QString idprofesor();
    void imprimir();
    QString generaFiltro();
    void borrar();
    void crear();
    void editar ( int );
    void submenu ( const QPoint & );

signals:
    /// Estando en modo seleccion al seleccionar un elemento se emite esta se&ntilde;al.
    void selected ( QString );
};

#endif

