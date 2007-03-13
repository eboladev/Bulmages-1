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
    QString idalbaran();
    QString numalbaran();
    QString fechaalbaran();
    QString comentalbaran();
    QString comentprivalbaran();
    QString idcliente();
    QString idforma_pago();
    QString idalmacen();
    QString refalbaran();
    QString descalbaran();
    QString numfactura();
    QString idtrabajador();
    QString procesadoalbaran();
    QString contactalbaran();
    QString telalbaran();
    virtual void pintar();
    virtual int guardar();
    virtual int borrar();
    virtual int cargar(QString);
    void vaciaAlbaranCliente();
    void setidalbaran(QString val);
    void setNumAlbaran(QString val);
    void setfechaalbaran(QString val);
    void setcomentalbaran(QString val);
    void setcomentprivalbaran(QString val);
    void setidcliente(QString val);
    void setidforma_pago(QString val);
    void setidtrabajador(QString val);
    void setidalmacen(QString val);
    void setNumFactura(QString val);
    void setrefalbaran(QString val);
    void setdescalbaran(QString val);
    void setcontactalbaran(QString val);
    void settelalbaran(QString val);
    void setprocesadoalbaran(QString val);
    virtual void pintaIdAlbaran(QString);
    virtual void pintaNumAlbaran(QString);
    virtual void pintafechaalbaran(QString);
    virtual void pintaComentAlbaran(QString);
    virtual void pintaComentPrivAlbaran(QString);
    virtual void pintaidcliente(QString);
    virtual void pintaidforma_pago(QString);
    virtual void pintaIdFactura(QString);
    virtual void pintaidalmacen(QString);
    virtual void pintaNumFactura(QString);
    virtual void pintadescalbaran(QString);
    virtual void pintarefalbaran(QString);
    virtual void pintaidtrabajador(QString);
    virtual void pintacontactalbaran(QString);
    virtual void pintatelalbaran(QString);
    virtual void pintaprocesadoalbaran(QString);
};

#endif

