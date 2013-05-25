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

#ifndef RESUMCTAIDGET_H
#define RESUMCTAWIDGET_H

#include <QtWidgets/QDockWidget>
#include <QtCore/QString>
#include <QtCore/QObject>

#include "ui_correctorbase.h"
#include "blpostgresqlclient.h"
#include "bfcompany.h"
#include "blfunctions.h"
#include "blwidget.h"


/** Busca errores y incoherencias en la contabilidad y las reporta al usuario en forma de
    listado.
    Esta clase realiza una serie de consultas determinadas en busca de determinados errores
    contables como puedan ser asientos no cerrados, amortizaciones con plazos expirados,
    cobros no realizados, etc.
    Realiza un informe de los errores encontrados para pasarselos al usuario. Tambien
    proporciona el acceso de forma sencilla a los errores encontrados.
    El corrector solo tiene una instancia en toda la ejecucion del programa, es la clase
    empresa la que se encarga de construirlo y una vez construido permanece siempre en
    ejecucion. */
class ResumCtaWidget : public QWidget, public Ui_correctorbase, public BlMainCompanyPointer
{
    Q_OBJECT

public:
    /// El informe generado se forma en HTML y se presenta mediante este QString.
    QString textBrowser;
    /// El corrector es una ventana del tipo dock, que se puede anexar a las esquinas
    /// del worspace de la aplicacion. Este puntero apunta a la ventana contenedora del
    /// corrector.
    QDockWidget *dock;

public:
    ResumCtaWidget ( BfCompany *emp, QWidget* parent = 0, Qt::WindowFlags fl = 0 );
    ~ResumCtaWidget();
    /// El corrector, al contrario que todos los demas elementos tiene la inicializacion de
    /// la base de datos y de la empresa realizada de forma diferente. Debe usarse la
    /// funcion setCompany  para inicializar el objeto. Esto es debido a que la construccion
    /// del corrector es anterior a la construccion de la clase empresa.
    void agregarError ( QString, QString, QString );
    void setMensaje ( QString mensaje );

public slots:
    virtual void alink ( const QUrl &url );
    virtual void cambia ( bool a );
};

#endif
