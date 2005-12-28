//
// C++ Interface: ControlStock
//
// Description: 
//
//
// Author: Tomeu Borras <tborras@conetxia.com>, (C) 2005
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef  CONTROLSTOCK_H
#define  CONTROLSTOCK_H

/**
@author Tomeu Borras
*/
#include "company.h"

class ControlStock {
private:
    QString mdb_idinventariooriginal;
    QString mdb_idalmacenoriginal;
    QString mdb_idarticulooriginal;

    QString mdb_idinventario;
    QString mdb_idalmacen;
    QString mdb_idarticulo;
    QString mdb_stockantcontrolstock;
    QString mdb_stocknewcontrolstock;
    QString mdb_punteocontrolstock;
    /// Aunque no es un elemento de la tabla, si es importante que este incluido pq se pinta en muchas partes.
    QString mdb_codigocompletoarticulo; 
    /// Aunque no es un elemento de la tabla, si es importante que este incluido pq se pinta en muchas partes.
    QString mdb_nomarticulo;  
    QString mdb_nomalmacen;
    QString mdb_codigoalmacen;
    company *companyact;
public:
    ControlStock(company *);
    ControlStock(company *, QString, QString, QString );
    /// La carga r�ida tiene un comportamiento poco restrictivo para aumnetar la eficiencia.
    ControlStock(company *comp, QString idinventario, QString idalmacen, QString idarticulo, QString stockantcontrolstock, QString stocknewcontrolstock, QString punteocontrolstock, QString codigocompletoarticulo, QString nomarticulo, QString nomalmacen, QString codigoalmacen);
    virtual ~ControlStock();
    virtual void pintaControlStock() {};
    void guardaControlStock();
    void vaciaControlStock();


    inline QString idinventario() {return mdb_idinventario;};
    inline QString idalmacen() {return mdb_idalmacen;};
    inline QString idarticulo() {return mdb_idarticulo;};
    inline QString stockantcontrolstock()  {return mdb_stockantcontrolstock;};
    inline QString stocknewcontrolstock() {return mdb_stocknewcontrolstock;};
    inline QString punteocontrolstock() {return mdb_punteocontrolstock;};
    inline QString codigocompletoarticulo() {return mdb_codigocompletoarticulo;};
    inline QString nomarticulo() {return mdb_nomarticulo;};
    inline QString nomalmacen() {return mdb_nomalmacen;};
    inline QString codigoalmacen() {return mdb_codigoalmacen;};

    
    inline void setidinventario(QString val) {mdb_idinventario=val;};
    inline void setidalmacen(QString val) {mdb_idalmacen=val;};
    inline void setstocknewcontrolstock(QString val)  {mdb_stocknewcontrolstock=val;};

    void setcodigocompletoarticulo(QString);
    void setcodigoalmacen(QString);

    /// EStas funciones no estan como debe, el uso de cada una de ellas debe hacer cambios sobre la base de datos.
    void setidarticulo(QString);    
    inline void setnomarticulo(QString val) {mdb_nomarticulo=val;};
    void borrar();  
        
};

#endif
