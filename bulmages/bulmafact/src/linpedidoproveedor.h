//
// C++ Interface: LinPedidoProveedor
//
// Description: 
//
//
// Author: Tomeu Borras <tborras@conetxia.com>, (C) 2005
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef LINPEDIDOPROVEEDOR_H
#define LINPEDIDOPROVEEDOR_H

/**
@author Tomeu Borras
*/
#include "company.h"

class LinPedidoProveedor {
private:
    QString mdb_numlpedidoproveedor;
    QString mdb_desclpedidoproveedor;
    QString mdb_cantlpedidoproveedor;
    QString mdb_pvplpedidoproveedor;
    QString mdb_prevlpedidoproveedor;
    QString mdb_ivalpedidoproveedor;
    QString mdb_descuentolpedidoproveedor;
    QString mdb_idpedidoproveedor;
    QString mdb_idarticulo;
    QString mdb_codigocompletoarticulo;
    QString mdb_nomarticulo;
    QString mdb_puntlpedidoproveedor;

    company *companyact;
public:
    LinPedidoProveedor(company *);
    LinPedidoProveedor(company *, QString );
    /// La carga r�ida tiene un comportamiento poco restrictivo para aumnetar la eficiencia.
    LinPedidoProveedor(company *comp, QString numlpedidoproveedor, QString desclpedidoproveedor, QString cantlpedidoproveedor, QString pvplpedidoproveedor, QString prevlpedidoproveedor, QString ivalpedidoproveedor, QString descuentolpedidoproveedor, QString idpedidoproveedor, QString idarticulo, QString codigocompletoarticulo, QString nomarticulo, QString puntlpedidoproveedor);
    virtual ~LinPedidoProveedor();
    virtual void pintaLinPedidoProveedor() {};
    void guardaLinPedidoProveedor();
    void vaciaLinPedidoProveedor();


    inline QString numlpedidoproveedor() {return mdb_numlpedidoproveedor;};
    inline QString desclpedidoproveedor() {return mdb_desclpedidoproveedor;};
    inline QString cantlpedidoproveedor() {return mdb_cantlpedidoproveedor;};
    inline QString pvplpedidoproveedor()  {return mdb_pvplpedidoproveedor;};
    inline QString prevlpedidoproveedor() {return mdb_prevlpedidoproveedor;};
    inline QString ivalpedidoproveedor() {return mdb_ivalpedidoproveedor;};
    inline QString descuentolpedidoproveedor() {return mdb_descuentolpedidoproveedor;};
    inline QString idpedidoproveedor() {return mdb_idpedidoproveedor;};
    inline QString idarticulo() {return mdb_idarticulo;};
    inline QString codigocompletoarticulo() {return mdb_codigocompletoarticulo;};
    inline QString nomarticulo() {return mdb_nomarticulo;};
    inline QString puntlpedidoproveedor() {return mdb_puntlpedidoproveedor;};

    
    
    inline void setnumlpedidoproveedor(QString val) {mdb_numlpedidoproveedor=val;};
    inline void setdesclpedidoproveedor(QString val) {mdb_desclpedidoproveedor=val;};
    inline void setcantlpedidoproveedor(QString val) {mdb_cantlpedidoproveedor=val;};
    inline void setpvplpedidoproveedor(QString val)  {mdb_pvplpedidoproveedor=val;};
    inline void setprevlpedidoproveedor(QString val) {mdb_prevlpedidoproveedor=val;};
    inline void setivalpedidoproveedor(QString val) {mdb_ivalpedidoproveedor=val;};
    inline void setdescuentolpedidoproveedor(QString val) {mdb_descuentolpedidoproveedor=val;};
    inline void setidpedidoproveedor(QString val) {mdb_idpedidoproveedor=val;};
    void setidarticulo(QString val);
    void setcodigocompletoarticulo(QString val);
    inline void setnomarticulo(QString val) {mdb_nomarticulo=val;};
    inline void setpuntlpedidoproveedor(QString val) {mdb_puntlpedidoproveedor = val;};
    
    
/*    
    void setcodigocompletoarticulo(QString);  


    /// EStas funciones no estan como debe, el uso de cada una de ellas debe hacer cambios sobre la base de datos.
    void setidarticulo(QString);    
    inline void setnomarticulo(QString val) {mdb_nomarticulo=val;};
*/    
    /// Hace el calculo de la base imponible de la l�ea  
    float calculabase();
    /// Hace el calculo del IVA de la l�ea.
    float calculaiva();
    void borrar();  
        
};

#endif
