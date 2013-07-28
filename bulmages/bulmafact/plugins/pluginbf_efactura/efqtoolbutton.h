/***************************************************************************
 *   Copyright (C) 2006 by Arturo Martin Llado                             *
 *   amartin@conetxia.com                                                  *
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

// #ifndef EFQTOOLBUTTON_H
// #define EFQTOOLBUTTON_H

#include <QtWidgets/QWidget>
#include <QtWidgets/QToolButton>
#include "bfcompany.h"
#include "blfunctions.h"
#include "blfixed.h"
#include "blwidget.h"


class EFQToolButton : public QToolButton, BlMainCompanyPointer
{
    Q_OBJECT

private:
    FacturaView *m_factura;

public:
    EFQToolButton ( FacturaView *fac, QWidget *parent = 0 );
    ~EFQToolButton();
    void escribe_descuento_factura ( QString &string, BlDbRecordSet *descuentos_factura, BlFixed bimpfactura );
    void escribe_linea_factura ( QString &string, BlDbRecordSet *lfactura, int numerolinea );
    void exporta_factura_ubl();

public slots:
    virtual void click();

};
