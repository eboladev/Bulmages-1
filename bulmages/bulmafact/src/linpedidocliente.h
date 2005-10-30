//
// C++ Interface: LinPedidoCliente
//
// Description: 
//
//
// Author: Tomeu Borras <tborras@conetxia.com>, (C) 2005
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef LINPEDIDOCLIENTE_H
#define LINPEDIDOCLIENTE_H

/**
@author Tomeu Borras
*/
#include "company.h"

class LinPedidoCliente {
private:
    QString mdb_numlpedidocliente;
    QString mdb_desclpedidocliente;
    QString mdb_cantlpedidocliente;
    QString mdb_pvplpedidocliente;
    QString mdb_prevlpedidocliente;
    QString mdb_ivalpedidocliente;
    QString mdb_descuentolpedidocliente;
    QString mdb_idpedidocliente;
    QString mdb_idarticulo;
    QString mdb_codigocompletoarticulo;
    QString mdb_nomarticulo;
    QString mdb_puntlpedidocliente;

    company *companyact;
public:
    LinPedidoCliente(company *);
    LinPedidoCliente(company *, QString );
    /// La carga r�ida tiene un comportamiento poco restrictivo para aumnetar la eficiencia.
    LinPedidoCliente(company *comp, QString numlpedidocliente, QString desclpedidocliente, QString cantlpedidocliente, QString pvplpedidocliente, QString prevlpedidocliente, QString ivalpedidocliente, QString descuentolpedidocliente, QString idpedidocliente, QString idarticulo, QString codigocompletoarticulo, QString nomarticulo, QString puntlpedidocliente);
    virtual ~LinPedidoCliente();
    virtual void pintaLinPedidoCliente() {};
    void guardaLinPedidoCliente();
    void vaciaLinPedidoCliente();


    inline QString numlpedidocliente() {return mdb_numlpedidocliente;};
    inline QString desclpedidocliente() {return mdb_desclpedidocliente;};
    inline QString cantlpedidocliente() {return mdb_cantlpedidocliente;};
    inline QString pvplpedidocliente()  {return mdb_pvplpedidocliente;};
    inline QString prevlpedidocliente() {return mdb_prevlpedidocliente;};
    inline QString ivalpedidocliente() {return mdb_ivalpedidocliente;};
    inline QString descuentolpedidocliente() {return mdb_descuentolpedidocliente;};
    inline QString idpedidocliente() {return mdb_idpedidocliente;};
    inline QString idarticulo() {return mdb_idarticulo;};
    inline QString codigocompletoarticulo() {return mdb_codigocompletoarticulo;};
    inline QString nomarticulo() {return mdb_nomarticulo;};
    inline QString puntlpedidocliente() {return mdb_puntlpedidocliente;};

    
    
    inline void setnumlpedidocliente(QString val) {mdb_numlpedidocliente=val;};
    inline void setdesclpedidocliente(QString val) {mdb_desclpedidocliente=val;};
    inline void setcantlpedidocliente(QString val) {mdb_cantlpedidocliente=val;};
    inline void setpvplpedidocliente(QString val)  {mdb_pvplpedidocliente=val;};
    inline void setprevlpedidocliente(QString val) {mdb_prevlpedidocliente=val;};
    inline void setivalpedidocliente(QString val) {mdb_ivalpedidocliente=val;};
    inline void setdescuentolpedidocliente(QString val) {mdb_descuentolpedidocliente=val;};
    inline void setidpedidocliente(QString val) {mdb_idpedidocliente=val;};
    void setidarticulo(QString val);
    void setcodigocompletoarticulo(QString val);
    inline void setnomarticulo(QString val) {mdb_nomarticulo=val;};
    inline void setpuntlpedidocliente(QString val) {mdb_puntlpedidocliente = val;};
    
    
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
