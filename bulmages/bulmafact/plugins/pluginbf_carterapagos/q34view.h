/***************************************************************************
 *   Copyright (C) 2004 by Tomeu Borras Riera                              *
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

#ifndef Q34VIEW_H
#define Q34VIEW_H

#include <QtWidgets/QLineEdit>
#include <QtWidgets/QLabel>
#include <QtWidgets/QCheckBox>
#include <QtCore/QTextStream>

#include "ui_q34base.h"
#include "blpostgresqlclient.h"
#include "bldatesearch.h"
#include "bldialogchanges.h"
#include "bfform.h"
#include "carterapagoslist.h"
#include "pdefs_pluginbf_carterapagos.h"


class BfCompany;


/** Ventana de ficha de cobro.
    Se encarga de la presentacion de la ficha de cobro y del tratamiento de eventos producidos
    en dicha ventana.
    Deriva de BlForm para metodos comunes a todas las ventanas.
    Deriva de Cobro para el manejo de la Base de datos. */
class PLUGINBF_CARTERAPAGOS_EXPORT Q34View : public BfForm, public Ui_Q34Base
{
    Q_OBJECT

public:
    CarteraPagosList *m_carteraPagosList;

    Q34View ( CarteraPagosList *,  BfCompany *, QWidget * );
    ~Q34View();

    QByteArray cobroQ34 ( QTextStream &, QString );
    QByteArray cabeceraPresentador ( QTextStream &, QString );
    QByteArray cabeceraOrdenante ( QTextStream &, QString );
    QByteArray totalOrdenante ( QTextStream &, QString, QString, QString );
    QByteArray totalGeneral ( QTextStream &, QString , QString , QString  );
    /// Como ya es un signaling slot en la clase ficha no se delcara como slot.
    virtual void on_mui_aceptar_clicked();

public slots:
    virtual void on_mui_buscararchivo_clicked();

};

#endif

