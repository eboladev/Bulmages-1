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

#ifndef LISTLINPREVCOBRO_H
#define LISTLINPREVCOBRO_H

#include <QObject>
#include <Q3PtrList>

#include "empresa.h"
#include "postgresiface2.h"
#include "linprevcobro.h"
#include "fixed.h"


class ListLinPrevCobro {
public:
    empresa *m_companyact;
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
    Q3PtrList<linprevcobro> m_lista;

public:
    Fixed totalCobro();
    Fixed totalPago();
    ListLinPrevCobro(empresa *comp);
    ListLinPrevCobro();
    void inicializaVariables();
    void setcompany(empresa *c) {
        m_companyact = c;
    };
    virtual ~ListLinPrevCobro();
    void guardaListLinPrevCobro();
    void vaciar();
    virtual void pintaListLinPrevCobro() {
        fprintf(stderr,"La funcion pintaListLinPrevCobro aun no ha sido implementada\n");
    };
    int chargeBudgetLines();
    void borrar();
    void nuevalinea(QString, QString, QString, QString, QString, QString, QString, QString, QString, QString, QString, QString, QString, QString);
    linprevcobro *linpos(int);
    void borralinprevcobro(int);
    void setidregistroiva(QString id) {
        mdb_idregistroiva = id;
        linprevcobro *linea;
        uint i = 0;
        for (linea = m_lista.first(); linea; linea = m_lista.next()) {
            linea->setidregistroiva(mdb_idregistroiva);
            i++;
        } // end for
    };
};

#endif

