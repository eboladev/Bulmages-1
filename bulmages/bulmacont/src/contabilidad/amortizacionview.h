/***************************************************************************
 *   Copyright (C) 2003 by Tomeu Borras Riera                              *
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

#ifndef AMORTIZACIONDLGAMORTIZACIONVIEW_H
#define AMORTIZACIONDLGAMORTIZACIONVIEW_H

#include "postgresiface2.h"
#include "dialogchanges.h"
#include "ui_amortizacionbase.h"


class empresa;


/// Clase que muestra la ventana de amortizaci&oacute;n.
/** Formulario de introducci&oacute;n y/o visi&oacute;n de amortizaciones. */
class amortizacionview : public QWidget, Ui_AmortizacionBase, dialogChanges {
    Q_OBJECT

public:
    /// Empresa con la que se trabaja.
    empresa *m_companyact;
    /// Esta variable indica cual es el &iacute;ndice de la cuenta de activo de la
    /// amortizaci&oacute;n que estamos viendo. Si no se est&aacute; mostrando una
    /// cuenta este valor vale "".
    QString idctaactivo;
    /// Esta variable indica cual es el &iacute;ndice de la cuenta de amortizacion de la
    /// amortizacion que estamos viendo. Si no se está mostrando una cuenta este valor vale "".
    QString idctaamortizacion;
    /// Esta variable indica cu&aacute;l es la amortizaci&oacute;n que se est&aacute;
    /// ense&ntilde;ando. Si no tiene valor es que no se esta mostrando ninguna.
    QString idamortizacion;
    /// Esta variable indica cu&aacute;l es el asiento plantilla de amortizaci&oacute;n.
    QString idainteligente;
    amortizacionview(empresa *, QWidget *);
    ~amortizacionview();
    void inicializa(QString);
    void trataModificado();

public slots:
    virtual bool close();
    virtual void accept();
    virtual void calculaamortizacion();
    virtual void contextMenuRequested(int, int, const QPoint &);
    virtual void s_saveAmortizacion();
    virtual void s_newAmortizacion();
    virtual void s_deleteAmortizacion();
};

#endif

