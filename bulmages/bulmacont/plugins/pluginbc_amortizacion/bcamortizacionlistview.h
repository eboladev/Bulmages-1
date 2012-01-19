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

#ifndef BCAMORTIZACIONLISTVIEW_H
#define BCAMORTIZACIONLISTVIEW_H

#include "ui_bcamortizacionlistbase.h"
#include "blformlist.h"
#include "pdefs_pluginbc_amortizacion.h"


class BcCompany;


/** Trata la ventana de Listado de Amortizaciones.
*/
class PLUGINBC_AMORTIZACION_EXPORT BcAmortizacionListView : public BlFormList, public Ui_BcAmortizacionListBase
{
    Q_OBJECT

private:
    /// Esta variable contiene el idamortizacion cuando se devuelve un parametro.
    QString m_idAmortizacion;
    /// Esta variable contiene el nomamortizacion cuando se devuelve un parametro.
    QString m_nomAmortizacion;

public:
    BcAmortizacionListView ( BcCompany *company, QWidget *parent );
    ~BcAmortizacionListView();
    void inicializaTabla();
    void editar ( int row );
    virtual void crear();
    virtual void remove();

signals:
    void selected ( QString );

};

#endif

