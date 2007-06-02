/***************************************************************************
 *   Copyright (C) 2005 by Tomeu Borras Riera                              *
 *   tborras@conetxia.com                                                  *
 *   http://www.iglues.org                                                 *
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

#ifndef FACTURA_H
#define FACTURA_H

#include <QString>

#include "listlinfacturaview.h"
#include "listdescfacturaview.h"
#include "fixed.h"
#include "company.h"
#include "dbrecord.h"
#include "fichabf.h"

/// Administra los datos de una factura a cliente.
/** Intermedia entre la base de datos y la pantalla de presentacion. */
class Factura : public FichaBf {
    Q_OBJECT

public:
    Factura(company *, QWidget *parent);
    virtual ~Factura();
    /// Establece cu&aacute;l es la lista subformulario del presupuesto. Normalmente para
    /// apuntar listlinpresupuestoview.
    company *_company();
    QString codigoserie_factura();
    QString idcliente();
    QString idalmacen();
    QString numfactura();
    QString ffactura();
    QString procesadafactura();
    QString comentfactura();
    QString reffactura();
    QString descfactura();
    QString idfactura();
    QString idforma_pago();
    void pintaFactura();
    void setidcliente(QString val);
    void setidalmacen(QString val);
    void setreffactura(QString val);
    void setNumFactura(QString val);
    void setfechafactura(QString val);
    void setdescfactura(QString val);
    void setcodigoserie_factura(QString val);
    void setcomentfactura(QString val);
    void setidfactura(QString val);
    void setidforma_pago(QString val);
    void setprocesadafactura(QString val);
    void vaciaFactura();
    virtual int cargar(QString);
    virtual int guardar();
    virtual int borrar();
    virtual void pintaidcliente(QString);
    virtual void pintareffactura(QString);
    virtual void pintaNumFactura(QString );
    virtual void pintafechafactura(QString );
    virtual void pintadescfactura(QString);
    virtual void pintacodigoserie_factura(QString);
    virtual void pintaComentFactura(QString);
    virtual void pintaidalmacen(QString );
    virtual void pintaidforma_pago(QString);
    virtual void pintaprocesadafactura(QString);
    virtual void cargaFacturaDescuentas(QString);
    virtual void calculateImports();
    virtual void inicialize();
    virtual QString calculateValues();
};

#endif

