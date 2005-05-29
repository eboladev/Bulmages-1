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
    QString mdb_idusuari;

public:
    /// Esta blece cual es la lista subformulario del presupuesto. Normalmente para apuntar listlinpresupuestoview.
    void setlislinpresupuesto ( listlinpresupuesto *a) {
        listalineas =a;
    };
    
    presupuesto(company *);
    virtual ~presupuesto();
    void chargeBudget(QString );
    void pintaPresupuesto();
    void guardapresupuesto();
    
    virtual void pintaIdClient(QString id) {};
    virtual void pintaIdAlmacen(QString id) {};
    virtual void pintaNumPresupuesto(QString id) {};
    virtual void pintaFPresupuesto(QString id) {};
    virtual void pintaVencPresupuesto(QString id) {};
    virtual void pintaContractPresupuesto(QString id) {};
    virtual void pintaTelPresupuesto(QString id) {};
    virtual void pintaComentPresupuesto(QString id) {};
    virtual void pintaNomClient(QString id) {};
    virtual void pintaCifClient(QString id) {};
    virtual void pintaCodigoAlmacen(QString id) {};
    virtual void pintaNomAlmacen(QString id) {};

    void setIdClient(QString val) { mdb_idcliente=val;};
    void setIdAlmacen(QString val) { mdb_idalmacen=val;};
    void setNumPresupuesto(QString val) { mdb_numpresupuesto=val;};
    void setFPresupuesto(QString val) { mdb_fpresupuesto=val;};
    void setVencPresupuesto(QString val) { mdb_vencpresupuesto=val;};
    void setContractPresupuesto(QString val) { mdb_contactpresupuesto=val;};
    void setTelPresupuesto(QString val) { mdb_telpresupuesto=val;};
    void setComentPresupuesto(QString val) { mdb_comentpresupuesto=val;};
    void setNomClient(QString val) { mdb_nomcliente=val;};
    void setCifClient(QString val) {
       mdb_cifcliente=val;
       QString SQLQuery = "SELECT * FROM cliente WHERE cifcliente='"+mdb_cifcliente+"'";
       cursor2 *cur = companyact->cargacursor(SQLQuery);
       if(!cur->eof()) {
       		mdb_idcliente = cur->valor("idcliente");
		mdb_nomcliente = cur->valor("nomcliente");
       } else {
       		mdb_idcliente="";
		mdb_nomcliente="";
       }// end if
       delete cur;
       pintaNomClient(mdb_nomcliente);
    };
    void setCodigoAlmacen(QString val) {
     mdb_codigoalmacen=val;
       QString SQLQuery = "SELECT * FROM almacen WHERE codigoalmacen='"+mdb_codigoalmacen+"'";
       cursor2 *cur = companyact->cargacursor(SQLQuery);
       if(!cur->eof()) {
       		mdb_idalmacen = cur->valor("idalmacen");
		mdb_nomalmacen = cur->valor("nomalmacen");
       } else {
       		mdb_idalmacen="";
		mdb_nomalmacen="";
       }// end if
       delete cur;
       pintaNomAlmacen(mdb_nomalmacen);     
     };
    void setNomAlmacen(QString val) { mdb_nomalmacen=val;};
    void setIdPresupuesto(QString val) {mdb_idpresupuesto=val;};
    void setIdUsuari(QString val) {mdb_idusuari=val;};

    virtual void chargeBudgetDiscounts(QString) {};
    virtual void calculateImports() {};
    virtual void inicialize() {};
    virtual QString calculateValues() {};
};
#endif
