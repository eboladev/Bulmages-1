//
// C++ Interface: LinAlbaranCliente
//
// Description: 
//
//
// Author: Tomeu Borras <tborras@conetxia.com>, (C) 2005
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef LINALBARANCLIENTE_H
#define LINALBARANCLIENTE_H

/**
@author Tomeu Borras
*/
#include "company.h"

class LinAlbaranCliente {
private:
    QString mdb_numlalbaran;
    QString mdb_desclalbaran;
    QString mdb_cantlalbaran;
    QString mdb_pvplalbaran;
    QString mdb_descontlalbaran;
    QString mdb_ivalalbaran;
    QString mdb_idarticulo;
    /// Aunque no es un elemento de la tabla, si es importante que este incluido pq se pinta en muchas partes.
    QString mdb_codigocompletoarticulo; 
    /// Aunque no es un elemento de la tabla, si es importante que este incluido pq se pinta en muchas partes.
    QString mdb_nomarticulo; 
    QString mdb_idalbaran;
     
    company *companyact;
public:
    LinAlbaranCliente(company *);
    LinAlbaranCliente(company *, QString );
    /// La carga rápida tiene un comportamiento poco restrictivo para aumnetar la eficiencia.
    LinAlbaranCliente(company *comp, QString numlalbaran, QString desclalbaran, QString cantlalbaran, QString pvplalbaran, QString descontlalbaran, QString idarticulo, QString codigocompletoarticulo, QString nomarticulo, QString idalbaran, QString iva);
    virtual ~LinAlbaranCliente();
    virtual void pintaLinAlbaranCliente() {};
    void guardaLinAlbaranCliente();
    void vaciaLinAlbaranCliente();


    inline QString numlalbaran() {return mdb_numlalbaran;};
    inline QString desclalbaran() {return mdb_desclalbaran;};
    inline QString cantlalbaran() {return mdb_cantlalbaran;};
    inline QString pvplalbaran()  {return mdb_pvplalbaran;};
    inline QString descontlalbaran() {return mdb_descontlalbaran;};
    inline QString idarticulo() {return mdb_idarticulo;};
    inline QString codigocompletoarticulo() {return mdb_codigocompletoarticulo;};
    inline QString nomarticulo() {return mdb_nomarticulo;};
    inline QString idalbaran() {return mdb_idalbaran;};
    inline QString ivalalbaran() {return mdb_ivalalbaran;};

    
    inline void setnumlalbaran(QString val) {mdb_numlalbaran=val;};
    inline void setdesclalbaran(QString val) {mdb_desclalbaran=val;};
    inline void setcantlalbaran(QString val) {mdb_cantlalbaran=val;};
    inline void setpvplalbaran(QString val)  {mdb_pvplalbaran=val;};
    inline void setdescontlalbaran(QString val) {mdb_descontlalbaran=val;};
    inline void setivalalbaran(QString val) {mdb_ivalalbaran=val;};
    void setcodigocompletoarticulo(QString val);
    inline void setnomarticulo(QString val) {mdb_nomarticulo=val;};  
    inline void setidalbaran(QString val) {mdb_idalbaran=val;};
    void setidarticulo(QString);
    void borrar();  
};

#endif
