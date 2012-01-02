/***************************************************************************
 *   Copyright (C) 2003 by Tomeu Borras Riera                              *
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

#ifndef BCAMORTIZACIONVIEW_H
#define BCAMORTIZACIONVIEW_H

#include "bldb.h"
#include "blpostgresqlclient.h"
#include "bldialogchanges.h"
#include "bcsubform.h"
#include "bcform.h"
#include "pdefs_pluginbc_amortizacion.h"


class BcCompany;


class PLUGINBC_AMORTIZACION_EXPORT BcAmortizacionSubForm : public BcSubForm
{
    Q_OBJECT

public:
    BcAmortizacionSubForm ( QWidget *parent );
    virtual ~BcAmortizacionSubForm();
    virtual void createMenu ( QMenu * );
    virtual void execMenuAction ( QAction * );
};


#include "ui_bcamortizacionbase.h"


/// Clase que muestra la ventana de amortizaci&oacute;n.
/** Formulario de introducci&oacute;n y/o visi&oacute;n de amortizaciones. */
class PLUGINBC_AMORTIZACION_EXPORT BcAmortizacionView : public BcForm, public Ui_BcAmortizacionBase
{
    Q_OBJECT

private:
    /// Esta variable indica cual es el &iacute;ndice de la cuenta de activo de la
    /// amortizaci&oacute;n que estamos viendo. Si no se est&aacute; mostrando una
    /// cuenta este valor vale "".
    QString m_idctaactivo;
    /// Esta variable indica cual es el &iacute;ndice de la cuenta de amortizacion de la
    /// amortizacion que estamos viendo. Si no se está mostrando una cuenta este valor vale "".
    QString m_idctaamortizacion;
    /// Esta variable indica cu&aacute;l es la amortizaci&oacute;n que se est&aacute;
    /// ense&ntilde;ando. Si no tiene valor es que no se esta mostrando ninguna.
    QString m_idamortizacion;

public:
    BcAmortizacionView ( BcCompany *, QWidget * );
    ~BcAmortizacionView();
    virtual int load ( QString idamortizacion );
    virtual int save();
    virtual int remove();

public slots:
    virtual void on_mui_btcalcular_clicked();
};

#endif
