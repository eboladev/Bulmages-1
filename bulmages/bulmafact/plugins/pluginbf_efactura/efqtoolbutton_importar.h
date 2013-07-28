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

// #ifndef EFQTOOLBUTTON_IMPORTAR_H
// #define EFQTOOLBUTTON_IMPORTAR_H

#include <QtWidgets/QWidget>
#include <QtWidgets/QToolButton>
#include <QtCore/QString>
#include <QtXml/QDomDocument>
#include <QtCore/QMap>
#include <QtCore/QString>

#include "bfcompany.h"
#include "blfunctions.h"
#include "blwidget.h"


class EFQToolButtonImportar : public QToolButton, BlMainCompanyPointer
{
    Q_OBJECT

private:
    FacturasProveedorList *m_faclistado;

public:
    EFQToolButtonImportar ( FacturasProveedorList *faclistado, QWidget *parent = 0 );
    ~EFQToolButtonImportar();
    void importa_factura_ubl();
    QString obten_valor_nodo ( QString nombre, QDomDocument *doc );
    QString obten_id_proveedor ( QDomDocument *doc );
    QString obten_descuento_factura ( QDomDocument *doc, QList< QMap<QString, QString> > &lista_descuentos );
    void obten_linea_factura ( QDomDocument *doc, QMap<QString, QString> &mapa_lfactura, int i );

public slots:
    void click();

};
