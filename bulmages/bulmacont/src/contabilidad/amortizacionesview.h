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
#include "fichabc.h"


class empresa;


/** Trata la ventana de Listado de Amortizaciones.
*/
class AmortizacionesView : public FichaBc, public Ui_AmortizacionesBase {
    Q_OBJECT
private:

    /// Esta variable contiene el idamortizacion cuando se devuelve un parametro.
    QString mdb_idamortizacion;
    /// Esta variable contiene el nomamortizacion cuando se devuelve un parametro.
    QString mdb_nomamortizacion;

public:
   AmortizacionesView(empresa *, QWidget *);
    ~AmortizacionesView();
    void inicializatabla();
    void editAmortizacion(int);
public slots:
    virtual void on_mui_listado_cellDoubleClicked(int, int);
    virtual void on_mui_crear_clicked();
    virtual void on_mui_borrar_clicked();
    virtual void on_mui_actualizar_clicked();

signals:
    void selected(QString);

};

#endif

