//
// C++ Interface: linpresupuesto
//
// Description: 
//
//
// Author: Tomeu Borras <tborras@conetxia.com>, (C) 2005
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef LINPRESUPUESTO_H
#define LINPRESUPUESTO_H

/**
@author Tomeu Borras
*/
#include "company.h"

class linpresupuesto {
public:
    QString mdb_idlpresupuesto;
    QString mdb_desclpresupuesto;
    QString mdb_cantlpresupuesto;
    QString mdb_pvplpresupuesto;
    QString mdb_descuentolpresupuesto;
    QString mdb_idpresupuesto;
    QString mdb_idarticulo;
    /// Aunque no es un elemento de la tabla, si es importante que este incluido pq se pinta en muchas partes.
    QString mdb_codigocompletoarticulo; 
    /// Aunque no es un elemento de la tabla, si es importante que este incluido pq se pinta en muchas partes.
    QString mdb_nomarticulo;    
    company *companyact;
    linpresupuesto(company *);
    linpresupuesto(company *, QString );
    /// La carga rápida tiene un comportamiento poco restrictivo para aumnetar la eficiencia.
    linpresupuesto(company *, QString , QString , QString , QString , QString , QString , QString, QString, QString );
    virtual ~linpresupuesto();
    virtual void pintalinpresupuesto() {};
    void guardalinpresupuesto();
    void vacialinpresupuesto();
    
    inline QString idlpresupuesto() {return mdb_idlpresupuesto;};
    inline QString desclpresupuesto() {return mdb_desclpresupuesto;};
    inline QString cantlpresupuesto() {return mdb_cantlpresupuesto;};
    inline QString pvplpresupuesto()  {return mdb_pvplpresupuesto;};
    inline QString descuentolpresupuesto() {return mdb_descuentolpresupuesto;};
    inline QString idpresupuesto() {return mdb_idpresupuesto;};
    inline QString idarticulo() {return mdb_idarticulo;};
    inline QString codigocompletoarticulo() {return mdb_codigocompletoarticulo;};
    inline QString nomarticulo() {return mdb_nomarticulo;};

    inline void setidlpresupuesto(QString val) {mdb_idlpresupuesto=val;};
    inline void setdesclpresupuesto(QString val) {mdb_desclpresupuesto=val;};
    inline void setcantlpresupuesto(QString val) {mdb_cantlpresupuesto=val;};
    inline void setpvplpresupuesto(QString val)  {mdb_pvplpresupuesto=val;};
    inline void setdescuentolpresupuesto(QString val) {mdb_descuentolpresupuesto=val;};
    inline void setidpresupuesto(QString val) {mdb_idpresupuesto=val;};
    /// EStas funciones no estan como debe, el uso de cada una de ellas debe hacer cambios sobre la base de datos.
    inline void setidarticulo(QString val) {mdb_idarticulo=val;};    
    inline void setcodigocompletoarticulo(QString val) {mdb_codigocompletoarticulo=val;};    
    inline void setnomarticulo(QString val) {mdb_nomarticulo=val;};    
        
};

#endif
