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

#include <QString>
#include <Q3Table>
#include <Q3PtrList>

#include "fixed.h"

#include "listlinpresupuesto.h"
#include "listdescpresupuesto.h"

/** @author Tomeu Borras & Alvaro de Miguel
  * \brief Clase que hace de intermediaria entre la tabla de presupuesto de la base de datos y el programa.
*/
#include "company.h"
#include "linpresupuesto.h"
#include "dbrecord.h"

class presupuesto : public DBRecord {
protected:
    listlinpresupuesto *listalineas;
    ListDescuentoPresupuesto *listadescuentos;
    company *companyact;
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

    listlinpresupuesto* _listalineas() {return listalineas;};
    ListDescuentoPresupuesto * _listadescuentos() {return listadescuentos;};
    company * _company() {return companyact;};

    QString idcliente(){return DBvalue("idcliente");};
    QString idalmacen(){return DBvalue("idalmacen");};
    QString numpresupuesto(){return DBvalue("numpresupuesto");};
    QString fpresupuesto(){return DBvalue("fpresupuesto");};
    QString vencpresupuesto(){return DBvalue("vencpresupuesto");};
    QString contactpresupuesto(){return DBvalue("contactpresupuesto");};
    QString telpresupuesto(){return DBvalue("telpresupuesto");};
    QString comentpresupuesto(){return DBvalue("comentpresupuesto");};
    QString idpresupuesto(){return DBvalue("idpresupuesto");};
    QString idusuari(){return DBvalue("idusuari");};
    QString procesadopresupuesto(){return DBvalue("procesadopresupuesto");};
    QString descpresupuesto(){return DBvalue("descpresupuesto");};
    QString refpresupuesto(){return DBvalue("refpresupuesto");};
    QString idforma_pago(){return DBvalue("idforma_pago");};
    QString idtrabajador(){return DBvalue("idtrabajador");};

    virtual ~presupuesto();
    virtual int chargeBudget(QString );
    virtual void pintaPresupuesto();
    virtual void guardapresupuesto();
    virtual void borraPresupuesto();
    virtual void imprimirPresupuesto();
    virtual void calculaypintatotales();
    virtual QString detalleArticulos();
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

    void setidcliente(QString val) { setDBvalue("idcliente",val);};
    void setidalmacen(QString val) {setDBvalue("idalmacen",val);};
    void setidtrabajador(QString val) {setDBvalue("idtrabajador",val);};
    void setNumPresupuesto(QString val) { setDBvalue("numpresupuesto",val);};
    void setFPresupuesto(QString val) { setDBvalue("fpresupuesto",val);};
    void setVencPresupuesto(QString val) { setDBvalue("vencpresupuesto",val);};
    void setContractPresupuesto(QString val) { setDBvalue("contactpresupuesto",val);};
    void setTelPresupuesto(QString val) { setDBvalue("telpresupuesto",val);};
    void setComentPresupuesto(QString val) { setDBvalue("comentpresupuesto",val);};
    void setidpresupuesto(QString val) { setDBvalue("idpresupuesto",val);listalineas->setidpresupuesto(val);};    
    void setIdUsuari(QString val) {setDBvalue("idusuari",val);};
    void setprocesadopresupuesto(QString val) {setDBvalue("procesadopresupuesto",val);};
    void setdescpresupuesto(QString val) {setDBvalue("descpresupuesto",val);};
    void setrefpresupuesto(QString val) {setDBvalue("refpresupuesto",val);};
    void setidforma_pago(QString val) {setDBvalue("idforma_pago",val);};
    void vaciaPresupuesto();
};
#endif
