/***************************************************************************
 *   Copyright (C) 2003 by Tomeu Borras Riera                              *
 *   tborras@conetxia.com                                                  *
 *   Copyright (C) 2012 by Fco. Javier M. C.                               *
 *   fcojavmc@todo-redes.com                                               *
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

#ifndef CORRECTORWIDGET_H
#define CORRECTORWIDGET_H

#include <QString>
#include <QObject>

#include "blfunctions.h"
#include "bldockwidget.h"
#include "ui_correctorbase.h"
#include "blpostgresqlclient.h"
#include "bccompany.h"


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
class CorrectorWidget : public QWidget, public Ui_correctorbase
{
    Q_OBJECT

public:
    /// Base de datos con la que se esta operando.
    BlPostgreSqlClient *m_dbConnection;
    /// Empresa que ha hecho la instancia del corrector.
    BcCompany *m_company;
    /// El informe generado se forma en HTML y se presenta mediante este QString.
    QString m_textBrowser;
    /// El corrector es una ventana del tipo dock, que se puede anexar a las esquinas
    /// del worspace de la aplicacion. Este puntero apunta a la ventana contenedora del
    /// corrector.
    BlDockWidget *m_dockWidget;
    QAction *m_viewCorrector;

public:
    CorrectorWidget ( QWidget *parent = 0, Qt::WFlags flags = 0 );
    ~CorrectorWidget();
    /// El corrector, al contrario que todos los demas elementos tiene la inicializacion de
    /// la base de datos y de la empresa realizada de forma diferente. Debe usarse la
    /// funcion setCompany  para inicializar el objeto. Esto es debido a que la construccion
    /// del corrector es anterior a la construccion de la clase empresa.
    void setCompany  ( BcCompany *company );
    void agregarError ( QString, QString, QString );

public slots:
    virtual void on_mui_corregir_clicked();
    virtual void on_mui_configurar_clicked();
    virtual void alink ( const QUrl &url );
    virtual void cambia ( bool a );
};

#endif
