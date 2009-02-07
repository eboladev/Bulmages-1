/***************************************************************************
 *   Copyright (C) 2006 by Tomeu Borras Riera                              *
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

#ifndef BUSQUEDAREFERENCIA_H
#define BUSQUEDAREFERENCIA_H

#include <QLineEdit>
#include <QLabel>

#include "blwidget.h"
#include "funcaux.h"
#include "ui_busquedareferenciabase.h"


/// Permite introducir una referencia.
/** Este Widget permite introducir una referencia. */
class BusquedaReferencia : public BlWidget, public Ui_BusquedaReferenciaBase
{
    Q_OBJECT

public:
    BusquedaReferencia ( QWidget *parent = 0 );
    ~BusquedaReferencia();
    virtual void setText ( QString val );
    virtual void setValorCampo ( QString val );
    virtual QString text();
    virtual QString valorCampo();

public slots:
    virtual void on_mui_informe_clicked();
    virtual void on_mui_referencia_editingFinished();
    virtual void on_mui_rferencia_returnPressed();
    virtual void on_mui_abrirtodo_clicked();

signals:
    void valueChanged ( const QString &);
    void returnPressed();
};

#endif

