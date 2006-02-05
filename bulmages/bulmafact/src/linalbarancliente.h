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
#include "dbrecord.h"

class LinAlbaranCliente : public DBRecord {
private:

    company *companyact;
	void definetabla();
public:
    LinAlbaranCliente(company *);
    LinAlbaranCliente(company *, QString );
    /// La carga r�ida tiene un comportamiento poco restrictivo para aumnetar la eficiencia.
    LinAlbaranCliente(company *comp, QString numlalbaran, QString desclalbaran, QString cantlalbaran, QString pvplalbaran, QString descontlalbaran, QString idarticulo, QString codigocompletoarticulo, QString nomarticulo, QString idalbaran, QString iva);
    virtual ~LinAlbaranCliente();
    virtual void pintaLinAlbaranCliente() {};
    void guardaLinAlbaranCliente();
    void vaciaLinAlbaranCliente();


    inline QString numlalbaran() {return DBvalue("numlalbaran");};
    inline QString desclalbaran() {return DBvalue("desclalbaran");};
    inline QString cantlalbaran() {return DBvalue("cantlalbaran");};
    inline QString pvplalbaran()  {return DBvalue("pvplalbaran");};
    inline QString descontlalbaran() {return DBvalue("descontlalbaran");};
    inline QString idarticulo() {return DBvalue("idarticulo");};
    inline QString codigocompletoarticulo() {return DBvalue("codigocompletoarticulo");};
    inline QString nomarticulo() {return DBvalue("nomarticulo");};
    inline QString idalbaran() {return DBvalue("idalbaran");};
    inline QString ivalalbaran() {return DBvalue("ivalalbaran");};

    
    inline void setnumlalbaran(QString val) {setDBvalue("numlalbaran",val);};
    inline void setdesclalbaran(QString val) {setDBvalue("desclalbaran",val);};
    inline void setcantlalbaran(QString val) {setDBvalue("cantlalbaran",val);};
    inline void setpvplalbaran(QString val)  {setDBvalue("pvplalbaran",val);};
    inline void setdescontlalbaran(QString val) {setDBvalue("descontlalbaran",val);};
    inline void setivalalbaran(QString val) {setDBvalue("ivalalbaran",val);};
    void setcodigocompletoarticulo(QString val);
    inline void setnomarticulo(QString val) {setDBvalue("nomarticulo",val);};  
    inline void setidalbaran(QString val) {setDBvalue("idalbaran",val);};
    void setidarticulo(QString);
    void borrar();  
};

#endif
