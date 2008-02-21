/***************************************************************************
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
#ifndef PLUGINREPORTODS_H
#define PLUGINREPORTODS_H

#ifdef Q_WS_WIN
# define MY_EXPORT __declspec(dllexport)
#else
# define MY_EXPORT
#endif

#include <QStringList>
#include <QWidget>
#include <QIcon>
#include <QApplication>
#include <QObject>
#include <QTextStream>



#include "bulmacont.h"
#include "postgresiface2.h"
#include "empresa.h"
#include "fixed.h"

extern "C" MY_EXPORT void entryPoint ( Bulmacont * );

extern QApplication2 *theApp;

//Cuentas Anuales Abreviadas Asociación Sin Lucro CAAASL
//Cuentas Anuales Plan General Contable hasta 2007 CAPGC07
//Cuentas Anuales PYMES 2008 CAPYMES08
//Cuentas Anuales Plan General Contable 2008 CAPGC08
//Cuentas Anuales Abreviadas Plan General Contable 2008 CAAPGC08

enum CAnuales {CAAASL, CAPGC07, CAPYMES08, CAPGC08, CAAPGC08};


class pluginReportODS : public QObject
{
    Q_OBJECT

private:
    Fixed saldoCuenta ( int cuenta );
    Fixed saldoCuentaAnt ( int cuenta );
    QString ejercicioActual_fechaBalance;
    QString ejercicioAnterior_fechaBalance;
	Fixed cuentaPositiva( Fixed valor );
	Fixed cuentaNevativa( Fixed valor );


	QString cuentaAnualAsociancionSinLucro ();
	QString cuentaAnualCAPGC07();
	QString cuentaAnualCAPYMES08();
	

public:
    Empresa *empresaact;
    postgresiface2 *conexionbase;

public:
    pluginReportODS();
    ~pluginReportODS();
    void inicializa ( Bulmacont * );

public slots:
    void balanceSituacionODS(CAnuales tipus);
	void formDatosBalance(CAnuales tipus);
	void balsitCAAASL ();
	void balsitCAPGC07 ();
	void balsitCAPYMES08 ();
	void balsitCAPGC08();
	void balsitCAAPGC08();
    
};


#endif //PLUGINREPORTODS_H
