/***************************************************************************
 *   Copyright (C) 2006 by Tomeu Borras Riera                              *
 *   tborras@conetxia.com                                                  *
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

#ifndef LISTIVA_H
#define LISTIVA_H

#include <QObject>
#include <QList>
#include <QMutableListIterator>

#include "empresa.h"
#include "iva.h"
#include "funcaux.h"
#include "blfixed.h"


class ListIva
{
public:
    Empresa *companyact;
    QString mdb_idregistroiva;
    QList<Iva*> m_lista;

public:
    ListIva ( Empresa *comp );
    ListIva();
    void setcompany ( Empresa *c ) {
        companyact = c;
    };
    virtual ~ListIva();
    void guardaListIva();
    void vaciar();
    virtual void pintaListIva() {
        _depura ( "Esta funcion aun no ha sido implementada\n", 2 );
    };
    int cargaListIva ( QString );
    void borrar();
    Iva *linpos ( int );
    void append ( Iva *iva ) {
        m_lista.append ( iva );
    };
    BlFixed calculaiva();
    BlFixed calculabase();
    void borraIva ( int );
    void setidregistroiva ( QString id ) {
        mdb_idregistroiva = id;
        Iva *linea;
        QMutableListIterator<Iva*> m_ilista ( m_lista );
        /// Vamos delante del primer elemento de la lista.
        m_ilista.toFront();
        /// Comprobamos que el primer elemento y siguientes existan.
        while ( m_ilista.hasNext() ) {
            /// Si existe el elemento nos desplazamos a el moviendo el cursor.
            linea = m_ilista.next();
            linea->setidregistroiva ( mdb_idregistroiva );
        } // end while
    };
};

#endif

