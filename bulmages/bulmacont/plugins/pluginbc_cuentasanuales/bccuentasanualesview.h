/***************************************************************************
 *   Copyright (C) 2003 by Tomeu Borras Riera                              *
 *   tborras@conetxia.com                                                  *
 *   http://www.iglues.org Asociación Iglues -- Contabilidad Linux         *
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

#ifndef BALANCESVIEW_H
#define BALANCESVIEW_H

#include <QtWidgets/QWidget>

#include "blfunctions.h"
#include "ui_bccuentasanualesbase.h"
#include "bcform.h"
#include "pdefs_pluginbc_cuentasanuales.h"


class BcCompany;


/// Listado de balances disponibles.
/** Clase que muestra un listado de todos los balances introducidos en el sistema.
    Sirve como punto de entrada a la generaci&oacute;n de los balances permitiendo la
    importaci&oacute;n/exportaci&oacute;n de los mismos, su modificaci&oacute;n y su impresi&oacute;n. */
class PLUGINBC_CUENTASANUALES_EXPORT BcCuentasAnualesView : public BcForm, public Ui_BcCuentasAnualesBase
{
    Q_OBJECT

private:
    /// Esta variable indica si se abre para b&uacute;squeda o para edici&oacute;n.
    /// (edicion = 0; seleccion = 1)
    int m_modo;
    /// Indica el nombre del balance seleccionado.
    QString m_nomBalance;
    /// Indica el identificador del balance seleccionado.
    QString m_idBalance;

public:
    BcCuentasAnualesView ( BcCompany *, QWidget *parent = 0 );
    ~BcCuentasAnualesView();
    void inicializaTabla();
    /// Establece el funcionamiento de la ventana como selector (afecta sobre
    /// el dobleclick en el listado)
    void setModoSelector();
    /// Establece el funcionamiento de la ventana como editor (afecta sobre el
    /// dobleclick en el listado)
    void setModoEditor();
    /// Devuelve el nombre del Balance seleccionado por la clase.
    QString nomBalance();
    /// Devuelve el identificador del Balance que se ha seleccionado.
    QString idBalance();

private slots:
    /// SLOT que responde a la pulsaci&oacute;n del bot&oacute;n de imprimir balance.
    virtual void imprimir();
    /// SLOT que responde a la acci&oacute;n de hacer doble click sobre el listado.
    virtual void on_mui_listado_itemDoubleClicked ( QTableWidgetItem * );
};

#endif

