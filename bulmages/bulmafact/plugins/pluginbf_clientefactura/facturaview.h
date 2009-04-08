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

#ifndef FACTURAVIEW_H
#define FACTURAVIEW_H

#include <QLineEdit>
#include <QLabel>
#include <QCheckBox>

#include "pdefs_clientefactura.h"
#include "ui_facturabase.h"
#include "bldatesearch.h"
#include "busquedaformapago.h"
#include "busquedaseriefactura.h"
#include "busquedaalmacen.h"
#include "bldialogchanges.h"
#include "bfform.h"
#include "bfcompany.h"



/// Muestra y administra la ventana de una factura a cliente.
/** */
class MY_EXPORT_CLIENTEFACTURA FacturaView : public BfForm, public Ui_FacturaBase
{
    Q_OBJECT

public:
    FacturaView ( BfCompany *, QWidget *parent = 0 );
    ~FacturaView();
    void inicializar();
    void pintatotales ( BlFixed, BlFixed, BlFixed, BlFixed, BlFixed, BlFixed );
    virtual int cargarPost ( QString );
    virtual int guardarPost();
    virtual int borrarPre();

public slots:
    virtual void on_mui_agregaralbaran_clicked();
    virtual void on_m_descuentos_editFinish ( int, int );
    virtual void on_subform2_editFinish ( int, int );
    virtual void on_mui_veralbaranes_clicked();
    virtual void on_mui_idcliente_valueChanged ( QString id );
    virtual void on_mui_idalmacen_valueChanged ( QString id );
};


#endif

