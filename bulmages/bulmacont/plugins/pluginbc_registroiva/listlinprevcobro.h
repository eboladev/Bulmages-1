/***************************************************************************
 *   Copyright (C) 2005 by Tomeu Borras Riera                              *
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

#ifdef TOMEU


#ifndef LISTLINPREVCOBRO_H
#define LISTLINPREVCOBRO_H

#include <QtCore/QObject>
#include <QtCore/QList>
#include <QtCore/QMutableListIterator>

#include "bccompany.h"
#include "blpostgresqlclient.h"
#include "linprevcobro.h"
#include "blfixed.h"
#include "blfunctions.h"


class ListLinPrevCobro
{
public:
    BcCompany *m_companyact;
    /// Opciones para filtrado y otros.

protected:
    QString mdb_idregistroiva;
    QString mfilt_idregistroiva;
    QString mfilt_tipoprevcobro;
    QString mfilt_codigocuentaprevcobro;
    QString mfilt_finprevcobro;
    QString mfilt_ffiprevcobro;
    QString mfilt_procesado;

protected:
    QList<linprevcobro*> m_lista;

public:
    BlFixed totalCobro();
    BlFixed totalPago();
    ListLinPrevCobro ( BcCompany *comp );
    ListLinPrevCobro();
    void inicializaVariables();
    void setcompany ( BcCompany *c ) {
        m_companyact = c;
    };
    virtual ~ListLinPrevCobro();
    void guardaListLinPrevCobro();
    void vaciar();
    virtual void pintaListLinPrevCobro() {
	BlDebug::blDebug ( ("END ", Q_FUNC_INFO), 0, _("La funcion 'pintaListLinPrevCobro' aun no ha sido implementada") );
    };
    int chargeBudgetLines();
    void remove();
    void nuevalinea ( QString, QString, QString, QString, QString, QString, QString, QString, QString, QString, QString, QString, QString, QString );
    linprevcobro *linpos ( int );
    void borralinprevcobro ( int );
    void setidregistroiva ( QString id ) {
        mdb_idregistroiva = id;
        linprevcobro *linea;
        QMutableListIterator<linprevcobro*> m_ilista ( m_lista );
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

#endif