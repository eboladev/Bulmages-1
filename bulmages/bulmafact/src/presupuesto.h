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

#include "fixed.h"

#include "listlinpresupuesto.h"
#include "listdescpresupuesto.h"

/** @author Tomeu Borras & Alvaro de Miguel
  * \brief Clase que hace de intermediaria entre la tabla de presupuesto de la base de datos y el programa.
*/
#include "company.h"
#include "linpresupuesto.h"

class presupuesto {
protected:
    listlinpresupuesto *listalineas;
    ListDescuentoPresupuesto *listadescuentos;
    company *companyact;
    QString mdb_idcliente;
    QString mdb_idalmacen;
    QString mdb_numpresupuesto;
    QString mdb_fpresupuesto;
    QString mdb_vencpresupuesto;
    QString mdb_contactpresupuesto;
    QString mdb_telpresupuesto;
    QString mdb_comentpresupuesto;
    QString mdb_idpresupuesto;
    QString mdb_idusuari;
    QString mdb_procesadopresupuesto;
    QString mdb_descpresupuesto;
    QString mdb_refpresupuesto;
    QString mdb_idforma_pago;
    QString mdb_idtrabajador;
public:
    /// Esta blece cual es la lista subformulario del presupuesto. Normalmente para apuntar listlinpresupuestoview.
    void setlislinpresupuesto ( listlinpresupuesto *a) {
        listalineas =a;
	listalineas->setcompany(companyact);
    };
    void setlisdescpresupuesto ( ListDescuentoPresupuesto *a) {
        listadescuentos =a;
	listadescuentos->setcompany(companyact);
    };    
    presupuesto(company *);
    virtual ~presupuesto();
    virtual void chargeBudget(QString );
    void pintaPresupuesto();
    void guardapresupuesto();
    void borraPresupuesto();
    void imprimirPresupuesto();
    void calculaypintatotales();
    QString detalleArticulos();
    virtual void pintaidcliente(QString) {};
    virtual void pintaIdAlmacen(QString) {};
    virtual void pintaNumPresupuesto(QString) {};
    virtual void pintaFPresupuesto(QString) {};
    virtual void pintaVencPresupuesto(QString) {};
    virtual void pintaContractPresupuesto(QString) {};
    virtual void pintaTelPresupuesto(QString) {};
    virtual void pintaComentPresupuesto(QString) {};
    virtual void pintaidalmacen(QString) {};
    virtual void pintaCodigoAlmacen(QString) {};
    virtual void pintaNomAlmacen(QString) {};
    virtual void pintaprocesadopresupuesto(QString) {};
    virtual void pintadescpresupuesto(QString) {};
    virtual void pintarefpresupuesto(QString) {};
    virtual void pintaidforma_pago(QString){};
    virtual void pintaidtrabajador(QString){};
    virtual void pintatotales(Fixed, Fixed, Fixed, Fixed) {};

    void setidcliente(QString val) { mdb_idcliente=val;};
    void setidalmacen(QString val) {mdb_idalmacen=val;};
    void setidtrabajador(QString val) {mdb_idtrabajador=val;};
    void setNumPresupuesto(QString val) { mdb_numpresupuesto=val;};
    void setFPresupuesto(QString val) { mdb_fpresupuesto=val;};
    void setVencPresupuesto(QString val) { mdb_vencpresupuesto=val;};
    void setContractPresupuesto(QString val) { mdb_contactpresupuesto=val;};
    void setTelPresupuesto(QString val) { mdb_telpresupuesto=val;};
    void setComentPresupuesto(QString val) { mdb_comentpresupuesto=val;};
    void setidpresupuesto(QString val) { mdb_idpresupuesto=val;listalineas->setidpresupuesto(val);};    
    void setIdUsuari(QString val) {mdb_idusuari=val;};
    void setprocesadopresupuesto(QString val) {mdb_procesadopresupuesto = val;};
    void setdescpresupuesto(QString val) {mdb_descpresupuesto = val;};
    void setrefpresupuesto(QString val) {mdb_refpresupuesto = val;};
    void setidforma_pago(QString val) {mdb_idforma_pago = val;};
    void vaciaPresupuesto();
};
#endif
