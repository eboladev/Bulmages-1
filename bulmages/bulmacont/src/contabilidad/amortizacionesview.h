/***************************************************************************
 *   Copyright (C) 2004 by Tomeu Borras Riera                              *
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

#ifndef AMORTIZACIONESVIEW_H
#define AMORTIZACIONESVIEW_H

#include "ui_amortizacionesbase.h"
#include "ficha.h"


class empresa;


/// Clase que muestra la ventana de amortizaciones.
/** */
class amortizacionesview : public Ficha, public Ui_AmortizacionesBase {
    Q_OBJECT

public:
    empresa *m_companyact;
    /// Esta variable contiene el idamortizacion cuando se devuelve un parametro.
    QString idamortizacion;
    /// Esta variable contiene el nomamortizacion cuando se devuelve un parametro.
    QString nomamortizacion;
    int modo;
    amortizacionesview(empresa *, QWidget *);
    ~amortizacionesview();
    void inicializatabla();

public slots:
    virtual void on_listado_cellDoubleClicked(int, int);
    virtual void on_mui_crear_clicked();
    virtual void on_mui_borrar_clicked();
    virtual void on_mui_actualizar_clicked();
};

#endif

