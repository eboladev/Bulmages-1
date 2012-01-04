/***************************************************************************
 *   Copyright (C) 2003 by Tomeu Borras                                    *
 *   tborras@conetxia.com                                                  *
 *   http://www.iglues.org Asociación Iglues -- Contabilidad Linux         *
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

#ifndef BCCENTROCOSTESELECCIONARVIEW_H
#define BCCENTROCOSTESELECCIONARVIEW_H

#include <QTreeWidgetItemIterator>

#include "blfunctions.h"
#include <ui_bccentrocosteseleccionarbase.h>
#include "blwidget.h"


class BcCompany;


/// Maneja la ventana de selector de centros de coste.
/** Esta clase es instanciada en la clase empresa y a partir de entonces se utiliza siempre
    una &uacute;nica instancia de esta en todo el programa \ref selccostes de este modo
    evitamos tener que andar reconstruyendo la clase e introducirle los datos.
    Tambi&eacute;n se utiliza para establecer el centro de coste por defecto, utlidad
    que permite introducir asientos directamente en un centro de coste sin tener que
    teclearlo en cada apunte. */
class BC_EXPORT BcCentroCosteSeleccionarView : public QDialog, public Ui_BcCentroCosteSeleccionarBase, public BlMainCompanyPointer
{
    Q_OBJECT

private:
    ///Indice de la columna que pone el nombre del centro de coste en la lista del formulario.
    int m_colNomCoste;
    /// Indice de la columna que indica la descripcion del centro de coste en la lista
    /// del formulario.
    int m_colDescCoste;
    /// Indice de la columna que indica el identificador del centro de coste en la lista
    /// del formulario.
    int m_colIdCoste;
    /// Indica la columna del status del centro de coste.
    int m_colStatusCoste;
    /// Indica la columna que tiene el checkbox para el centro de coste.
    int m_colCheck;
    /// Número de dígitos que tienen, por defecto, las cuentas.
    int numDigitos;
    /// Iterador de la lista que se presenta en este formulario.
    QTreeWidgetItemIterator *m_iterador;

public:
    /// Constructor de la clase que recibe el parametro de empresa y que inicializa la
    /// base de datos a utilizar.
    BcCentroCosteSeleccionarView ( BcCompany *emp, QWidget *parent = 0 );
    /// Destructor de la clase.
    ~BcCentroCosteSeleccionarView();
    void cargacostes();
    int firstccoste();
    int nextccoste();
    QString cadcoste();
    QString nomcoste();

public slots:
    void on_mui_todo_clicked();
    void on_mui_nada_clicked();
    void on_mui_invertir_clicked();
};

#endif

