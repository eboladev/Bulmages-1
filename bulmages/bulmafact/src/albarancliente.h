/***************************************************************************
 *   Copyright (C) 2005 by Tomeu Borras Riera                              *
 *   tborras@conetxia.com                                                  *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/

#ifndef ALBARANCLIENTE_H
#define ALBARANCLIENTE_H

#include <QString>

#include "company.h"
#include "fixed.h"
#include "fichabf.h"


/// Clase que almacena los datos de albaranes a clientes.
/** Se usa en conjuncion con AlbaranClienteView para conformar entre ambas
    la pantalla de edici&oacute;n de albaranes a Cliente.
*/
class AlbaranCliente : public FichaBf {
Q_OBJECT

public:
    AlbaranCliente(company *, QWidget *);
    
    virtual ~AlbaranCliente();
    QString idalbaran() {
        return DBvalue("idalbaran");
    };
    QString numalbaran() {
        return DBvalue("numalbaran");
    };
    QString fechaalbaran() {
        return DBvalue("fechaalbaran");
    };
    QString comentalbaran() {
        return DBvalue("comentalbaran");
    };
    QString comentprivalbaran() {
        return DBvalue("comentprivalbaran");
    };
    QString idcliente() {
        return DBvalue("idcliente");
    };
    QString idforma_pago() {
        return DBvalue("idforma_pago");
    };
    QString idalmacen() {
        return DBvalue("idalmacen");
    };
    QString refalbaran() {
        return DBvalue("refalbaran");
    };
    QString descalbaran() {
        return DBvalue("descalbaran");
    };
    QString numfactura() {
        return DBvalue("numfactura");
    };
    QString idtrabajador() {
        return DBvalue("idtrabajador");
    };
    QString procesadoalbaran() {
        return DBvalue("procesadoalbaran");
    };
    QString contactalbaran() {
        return DBvalue("contactalbaran");
    };
    QString telalbaran() {
        return DBvalue("telalbaran");
    };

    virtual void pintar();
    virtual int guardar();
    virtual int borrar();
    virtual int cargar(QString);
    void vaciaAlbaranCliente();


    void setidalbaran(QString val) {
        setDBvalue("idalbaran", val);
        m_listalineas->setColumnValue("idalbaran", val);
        m_listadescuentos->setColumnValue("idalbaran", val);
    };
    void setNumAlbaran(QString val) {
        setDBvalue("numalbaran", val);
    };
    void setfechaalbaran(QString val) {
        setDBvalue("fechaalbaran", val);
    };
    void setcomentalbaran(QString val) {
        setDBvalue("comentalbaran", val);
    };
    void setcomentprivalbaran(QString val) {
        setDBvalue("comentprivalbaran", val);
    };
    void setidcliente(QString val) {
        setDBvalue("idcliente", val);
    };
    void setidforma_pago(QString val) {
        setDBvalue("idforma_pago", val);
    };
    void setidtrabajador(QString val) {
        setDBvalue("idtrabajador", val);
    };
    void setidalmacen(QString val) {
        setDBvalue("idalmacen", val);
    };
    void setNumFactura(QString val) {
        setDBvalue("numfactura", val);
    };
    void setrefalbaran(QString val) {
        setDBvalue("refalbaran", val);
    };
    void setdescalbaran(QString val) {
        setDBvalue("descalbaran", val);
    };
    void setcontactalbaran(QString val) {
        setDBvalue("contactalbaran", val);
    };
    void settelalbaran(QString val) {
        setDBvalue("telalbaran", val);
    };
    void setprocesadoalbaran(QString val) {
        setDBvalue("procesadoalbaran", val);
    };
    virtual void pintaIdAlbaran(QString) {}
    ;
    virtual void pintaNumAlbaran(QString) {}
    ;
    virtual void pintafechaalbaran(QString) {}
    ;
    virtual void pintaComentAlbaran(QString) {}
    ;
    virtual void pintaComentPrivAlbaran(QString) {}
    ;
    virtual void pintaidcliente(QString) {}
    ;
    virtual void pintaidforma_pago(QString) {}
    ;
    virtual void pintaIdFactura(QString) {}
    ;
    virtual void pintaidalmacen(QString) {}
    ;
    virtual void pintaNumFactura(QString) {}
    ;
    virtual void pintadescalbaran(QString) {}
    ;
    virtual void pintarefalbaran(QString) {}
    ;
    virtual void pintaidtrabajador(QString) {}
    ;
    virtual void pintacontactalbaran(QString) {}
    ;
    virtual void pintatelalbaran(QString) {}
    ;
    virtual void pintaprocesadoalbaran(QString) {}
    ;
};

#endif

