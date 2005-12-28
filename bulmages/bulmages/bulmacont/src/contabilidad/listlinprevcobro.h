//
// C++ Interface: listlinprevcobro
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

class listlinprevcobro {
public:
    empresa *empresaactual;
    postgresiface2 *conexionbase;

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
    listlinprevcobro(empresa *comp);
    listlinprevcobro();
    void inicializaVariables();
    void setcompany(empresa *c) {
        fprintf(stderr,"listlinprevcobro setCompany\n");
        empresaactual=c;
        conexionbase = c->bdempresa();
        fprintf(stderr,"listlinprevcobro  fin de setCompany\n");
    };
    virtual ~listlinprevcobro();
    void guardalistlinprevcobro();
    void vaciar();
    virtual void pintalistlinprevcobro() {
        fprintf(stderr,"La funci� pintalistlinprevcobro aun no ha sido implementada\n");
    };
    void chargeBudgetLines();
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
