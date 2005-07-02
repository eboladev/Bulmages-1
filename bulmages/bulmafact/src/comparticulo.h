//
// C++ Interface: CompArticulo
//
// Description: 
//
//
// Author: Tomeu Borras <tborras@conetxia.com>, (C) 2005
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef COMPARTICULO_H
#define COMPARTICULO_H

/**
@author Tomeu Borras
*/
#include "company.h"

class CompArticulo {
private:
    QString mdb_idarticulo;
    QString mdb_idcomponente;
    QString mdb_cantcomparticulo;
    QString mdb_codigocompletocomponente;
    /// Aunque no es un elemento de la tabla, si es importante que este incluido pq se pinta en muchas partes.
    QString mdb_nomcomponente;    
    company *companyact;
public:
    CompArticulo(company *);
    CompArticulo(company *, QString, QString );
    /// La carga rápida tiene un comportamiento poco restrictivo para aumnetar la eficiencia.
    CompArticulo(company *, QString , QString , QString , QString , QString );
    virtual ~CompArticulo();
    virtual void pintaCompArticulo() {};
    void guardaCompArticulo();
    void vaciaCompArticulo();

    inline QString idarticulo() {return mdb_idarticulo;};
    inline QString idcomponente() {return mdb_idcomponente;};
    inline QString cantcomparticulo() {return mdb_cantcomparticulo;};
    inline QString codigocompletocomponente()  {return mdb_codigocompletocomponente;};
    inline QString nomcomponente() {return mdb_nomcomponente;};
    
    inline void setidarticulo(QString val) {mdb_idarticulo=val;};
    inline void setcantcomparticulo(QString val) {mdb_cantcomparticulo=val;};
    void setcodigocompletocomponente(QString);  
    void setidcomponente(QString);    
    void borrar();  
        
};

#endif
