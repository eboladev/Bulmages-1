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
	listalineas->setcompany(companyact);
    };
    
    presupuesto(company *);
    virtual ~presupuesto();
    void chargeBudget(QString );
    void pintaPresupuesto();
    void guardapresupuesto();
    void borraPresupuesto();
    
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
    
    virtual void pintatotales(float, float) {};

    void setIdClient(QString val) { mdb_idcliente=val;};
    void setIdAlmacen(QString val) { mdb_idalmacen=val;};
    void setNumPresupuesto(QString val) { mdb_numpresupuesto=val;};
    void setFPresupuesto(QString val) { mdb_fpresupuesto=val;};
    void setVencPresupuesto(QString val) { mdb_vencpresupuesto=val;};
    void setContractPresupuesto(QString val) { mdb_contactpresupuesto=val;};
    void setTelPresupuesto(QString val) { mdb_telpresupuesto=val;};
    void setComentPresupuesto(QString val) { mdb_comentpresupuesto=val;};
    void setNomClient(QString val) { mdb_nomcliente=val;};
    void setCifClient(QString);
    void setCodigoAlmacen(QString);
    void setNomAlmacen(QString val) { mdb_nomalmacen=val;};
    void setIdPresupuesto(QString val) {mdb_idpresupuesto=val;};
    void setIdUsuari(QString val) {mdb_idusuari=val;};

    void vaciaPresupuesto();

    virtual void chargeBudgetDiscounts(QString) {};
    virtual void calculateImports() {};
    virtual void inicialize() {};
    virtual QString calculateValues() {return "";};
};
#endif
