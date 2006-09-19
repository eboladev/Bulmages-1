/***************************************************************************
 *   Copyright (C) 2003 by Tomeu Borras Riera                              *
 *   tborras@conetxia.com                                                  *
 *   Copyright (C) 2003 by Antoni Mirabete i Teres                         *
 *   amirabet@biada.org                                                    *
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

#ifndef BALANCEPRINTVIEW_H
#define BALANCEPRINTVIEW_H

#include <errno.h>

#include "ui_balanceprintbase.h"
#include "postgresiface2.h"
#include "empresa.h"


/**
  * @author Tomeu Borras Riera
  * \class BalancePrintView balanceprintview.h
  * \brief Formulario introducción de datos para la impresión del balance de sumas y saldos.
  * \todo Hay que cambiar el array de ccostes por un cursor
  *
  * Clase que permite introducir los datos necesarios para la generación del balance (Fechas
  * y rangos) y que se encarga de generar el balance en los formatos de impresion indicados.
  */
class BalancePrintView : public QDialog, public Ui_BalancePrintBase {
    Q_OBJECT

private:
    /// La base de datos con la que la clase esta trabajando.
    postgresiface2 *conexionbase;
    /// La empresa con la que la clase esta trabajando.
    empresa *empresaactual;

public:
    BalancePrintView(empresa *emp);
    ~BalancePrintView();
    void inicializa1(QString, QString, QString, QString, bool);

private:
    void accept();
    void presentar(char *tipus);

private slots:
    virtual void boton_canales();
    virtual void boton_ccostes();
};

#endif

