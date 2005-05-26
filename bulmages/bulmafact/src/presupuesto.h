//
// C++ Interface: presupuesto
//
// Description:
//
//
// Author: Tomeu Borras <tborras@conetxia.com>, (C) 2005
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef PRESUPUESTO_H
#define PRESUPUESTO_H

#include <qstring.h>
#include <qtable.h>
#include <qptrlist.h>

#include "listlinpresupuesto.h"

/** @author Tomeu Borras & Alvaro de Miguel
  * \brief Clase que hace de intermediaria entre la tabla de presupuesto de la base de datos y el programa.
*/
#include "company.h"
#include "linpresupuesto.h"







class presupuesto {
protected:
    listlinpresupuesto *listalineas;

    company *companyact;
    QString m_idpresupuesto;
    QString m_idclient;
    QString m_idalmacen;
    QString m_initialValues;
    cursor2 *m_cursorcombo;

    QString mdb_idcliente;
    QString mdb_idalmacen;
    QString mdb_numpresupuesto;
    QString mdb_fpresupuesto;
    QString mdb_vencpresupuesto;
    QString mdb_contactpresupuesto;
    QString mdb_telpresupuesto;
    QString mdb_comentpresupuesto;
    QString mdb_nomcliente;
    QString mdb_cifcliente;
    QString mdb_codigoalmacen;
    QString mdb_nomalmacen;
    QString mdb_idpresupuesto;

public:
    // Esta blece cual es la lista subformulario del presupuesto. Normalmente para apuntar listlinpresupuestoview.
    void setlislinpresupuesto ( listlinpresupuesto *a) {
        listalineas =a;
    };
    presupuesto(company *);
    virtual ~presupuesto();
    void chargeBudget(QString );
    void pintaPresupuesto();
    virtual void	pintaIdClient(QString id) {}
    ;
    virtual void	pintaIdAlmacen(QString id) {}
    ;
    virtual void	pintaNumPresupuesto(QString id) {}
    ;
    virtual void	pintaFPresupuesto(QString id) {}
    ;
    virtual void	pintaVencPresupuesto(QString id) {}
    ;
    virtual void	pintaContractPresupuesto(QString id) {}
    ;
    virtual void	pintaTelPresupuesto(QString id) {}
    ;
    virtual void	pintaComentPresupuesto(QString id) {}
    ;
    virtual void	pintaNomClient(QString id) {}
    ;
    virtual void	pintaCifClient(QString id) {}
    ;
    virtual void	pintaCodigoAlmacen(QString id) {}
    ;
    virtual void	pintaNomAlmacen(QString id) {}
    ;



    virtual void chargeBudgetDiscounts(QString) {}
    ;
    virtual void calculateImports() {}
    ;
    virtual     void inicialize() {}
    ;
    virtual     QString calculateValues() {}
    ;
};
#endif
