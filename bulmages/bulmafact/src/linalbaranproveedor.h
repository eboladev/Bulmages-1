//
// C++ Interface: LinAlbaranProveedor
//
// Description: 
//
//
// Author: Tomeu Borras <tborras@conetxia.com>, (C) 2005
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef LINALBARANPROVEEDOR_H
#define LINALBARANPROVEEDOR_H

/**
@author Tomeu Borras
*/
#include "company.h"

class LinAlbaranProveedor {
private:
    QString mdb_numlalbaranp;
    QString mdb_desclalbaranp;
    QString mdb_cantlalbaranp;
    QString mdb_pvplalbaranp;
    QString mdb_descontlalbaranp;
    QString mdb_idarticulo;
    QString mdb_ivalalbaranp;
    /// Aunque no es un elemento de la tabla, si es importante que este incluido pq se pinta en muchas partes.
    QString mdb_codigocompletoarticulo; 
    /// Aunque no es un elemento de la tabla, si es importante que este incluido pq se pinta en muchas partes.
    QString mdb_nomarticulo; 
    QString mdb_idalbaranp;
     
    company *companyact;
public:
    LinAlbaranProveedor(company *);
    LinAlbaranProveedor(company *, QString );
    /// La carga rápida tiene un comportamiento poco restrictivo para aumnetar la eficiencia.
    LinAlbaranProveedor(company *comp, QString numlalbaranp, QString desclalbaranp, QString cantlalbaranp, QString pvplalbaranp, QString descontlalbaranp, QString idarticulo, QString codigocompletoarticulo, QString nomarticulo, QString idalbaranp, QString ivalalbaranp);
    virtual ~LinAlbaranProveedor();
    virtual void pintaLinAlbaranProveedor() {};
    void guardaLinAlbaranProveedor();
    void vaciaLinAlbaranProveedor();


    inline QString numlalbaranp() {return mdb_numlalbaranp;};
    inline QString desclalbaranp() {return mdb_desclalbaranp;};
    inline QString cantlalbaranp() {return mdb_cantlalbaranp;};
    inline QString pvplalbaranp()  {return mdb_pvplalbaranp;};
    inline QString descontlalbaranp() {return mdb_descontlalbaranp;};
    inline QString idarticulo() {return mdb_idarticulo;};
    inline QString codigocompletoarticulo() {return mdb_codigocompletoarticulo;};
    inline QString nomarticulo() {return mdb_nomarticulo;};
    inline QString idalbaranp() {return mdb_idalbaranp;};
    inline QString ivalalbaranp() {return mdb_ivalalbaranp;};

    
    inline void setnumlalbaranp(QString val) {mdb_numlalbaranp=val;};
    inline void setdesclalbaranp(QString val) {mdb_desclalbaranp=val;};
    inline void setcantlalbaranp(QString val) {mdb_cantlalbaranp=val;};
    inline void setpvplalbaranp(QString val)  {mdb_pvplalbaranp=val;};
    inline void setdescontlalbaranp(QString val) {mdb_descontlalbaranp=val;};
    void setcodigocompletoarticulo(QString val);
    inline void setnomarticulo(QString val) {mdb_nomarticulo=val;};  
    inline void setidalbaranp(QString val) {mdb_idalbaranp=val;};
    inline void setivalalbaranp(QString val) {mdb_ivalalbaranp=val;};
    void setidarticulo(QString);
   
    /// Hace el calculo de la base imponible de la línea  
    float calculabase();
    /// Hace el calculo del IVA de la línea.
    float calculaiva();
    void borrar();  
};

#endif
