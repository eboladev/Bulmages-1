//
// C++ Interface: ListLinPrevCobro
//
// Description:
//
//
// Author: Tomeu Borras <tborras@conetxia.com>, (C) 2005
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef LISTLINPREVCOBRO_H
#define LISTLINPREVCOBRO_H

#include <qobject.h>
//Added by qt3to4:
#include <Q3PtrList>

/** @author Tomeu Borras */

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
        m_companyact=c;
    };
    virtual ~ListLinPrevCobro();
    void guardaListLinPrevCobro();
    void vaciar();
    virtual void pintaListLinPrevCobro() {
        fprintf(stderr,"La funci� pintaListLinPrevCobro aun no ha sido implementada\n");
    };
    int chargeBudgetLines();
    void borrar();
    void nuevalinea(QString, QString, QString, QString, QString, QString, QString, QString, QString, QString, QString, QString, QString, QString);
    linprevcobro *linpos(int);
    void borralinprevcobro(int);

    void setidregistroiva(QString id) {
        mdb_idregistroiva=id;
        linprevcobro *linea;
        uint i = 0;
        for ( linea = m_lista.first(); linea; linea = m_lista.next() ) {
            linea->setidregistroiva(mdb_idregistroiva);
            i++;
        }// end for
    };
};

#endif
