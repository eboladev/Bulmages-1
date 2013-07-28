/***************************************************************************
 *   Copyright (C) 2003 by Tomeu Borras Riera                              *
 *   tborras@conetxia.com                                                  *
 *   Copyright (C) 2003 by Antoni Mirabete i Teres                         *
 *   amirabet@biada.org                                                    *
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

#ifndef BALANCEPRINTVIEW_H
#define BALANCEPRINTVIEW_H

//#include <QtWidgets/QDialog>

#include "ui_balanceprintbase.h"
#include "blpostgresqlclient.h"
#include "bccompany.h"
#include "blwidget.h"
#include "bcform.h"


/// Formulario introducción de datos para la impresión del balance de sumas y saldos.
/** \todo Hay que cambiar el array de ccostes por un cursor.
    Esta clase permite introducir los datos necesarios para la generaci&oacute;n del balance
    (Fechas y rangos) y que se encarga de generar el balance en los formatos de
    impresi&oacute;n indicados.
*/
class BalancePrintView : public BcForm, public Ui_BalancePrintBase
//, public BlMainCompanyPointer
{
    Q_OBJECT

public:
    BalancePrintView ( BcCompany *emp, QWidget *parent = 0, int flags = 0 );
    ~BalancePrintView();
    void inicializa ( QString, QString, QString, QString, bool );

private:
    void presentar ( const char * tipus );

private slots:
    /// No usar mui_imprimir para no confundirlo con BcForm.
    virtual void on_mui_generaImpresion_clicked();
    virtual void on_mui_canales_clicked();
    virtual void on_mui_centroCostes_clicked();
};

#endif

