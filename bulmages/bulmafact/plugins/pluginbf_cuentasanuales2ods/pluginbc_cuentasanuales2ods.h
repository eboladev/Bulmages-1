/***************************************************************************
 *   Copyright (C) 2008 by Ricardo Díaz de la Calle <richard@galdi.es>     *
 *   Copyright (C) 2008 by Adelaida Clavaguera Mora <laida@alaxarxa.net>   *
 *   Copyright (C) 2008 by Leopold Palomo Avellaneda <leo@alaxarxa.net>    *
 *   Copyright (C) 2006 by Fco. Javier M. C. >fcojavmc@todo-redes.com>     *
 *   Copyright (C) 2005 by Tomeu Borras Riera <tborras@conetxia.com>       *
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

#ifndef PLUGINBC_CUENTASANUALES2ODS_H
#define PLUGINBC_CUENTASANUALES2ODS_H

#include <QtWidgets/QMessageBox>
#include <QtWidgets/QWidget>
#include <QtGui/QIcon>
#include <QtWidgets/QApplication>
#include <QtCore/QObject>
#include <QtCore/QTextStream>

#include "bcbulmacont.h"
#include "blpostgresqlclient.h"
#include "bccompany.h"
#include "blfixed.h"
#include "bcplancontablearbol.h"
#include "pdefs_pluginbc_cuentasanuales2ods.h"


extern "C" PLUGINBC_CUENTASANUALES2ODS_EXPORT int entryPoint ( BcBulmaCont * );



//Cuentas Anuales Abreviadas Asociación Sin Lucro CAAASL
//Cuentas Anuales Plan General Contable hasta 2007 CAPGC07
//Cuentas Anuales PYMES 2008 CAPYMES08
//Cuentas Anuales Plan General Contable 2008 CAPGC08
//Cuentas Anuales Abreviadas Plan General Contable 2008 CAAPGC08
enum CAnuales {CAAASL, CAPGC07, CAPYMES08, CAPGC08, CAAPGC08};


class PluginBc_CuentasAnuales2ODS : public QObject
{
    Q_OBJECT

private:
    BlFixed saldoCuenta ( int cuenta );
    BlFixed saldoCuentaAnt ( int cuenta );
    QString ejercicioActual_fechaBalance;
    QString ejercicioAnterior_fechaBalance;
    BlFixed cuentaPositiva ( BlFixed valor );
    BlFixed cuentaNegativa ( BlFixed valor );
    void mensajeAdvertenciaPGC ( CAnuales tipus );
    QString cuentaAnualAsociancionSinLucro ();
    QString cuentaAnualCAPGC07();
    QString cuentaAnualCAPYMES08();
    QString cuentaAnualCAAPGC08();
    QString cuentaAnualCAPGC08();
    BcPlanContableArbol *arbolEjercicioActual, *arbolEjercicioAnterior; /// un arbol por cada periodo
    bool Arboles();

public:
    BcCompany *empresaact;
    BlPostgreSqlClient *dbConnection;

public:
    PluginBc_CuentasAnuales2ODS();
    ~PluginBc_CuentasAnuales2ODS();
    void inicializa ( BcBulmaCont * );

public slots:
    void balanceSituacionODS ( CAnuales tipus );
    bool formDatosBalance ( CAnuales tipus );
    void balsitCAAASL ();
    void balsitCAPGC07 ();
    void balsitCAPYMES08 ();
    void balsitCAPGC08();
    void balsitCAAPGC08();

};

#endif

