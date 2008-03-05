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

#include <QMenu>
#include <QAction>
#include <QObject>
#include <QMessageBox>

#include <cstdio>

#include "pluginreportods.h"
#include "empresa.h"
#include "funcaux.h"

#include "datosview.h"


///
/**
**/
pluginReportODS::pluginReportODS()
{
    _depura ( "pluginReportODS::pluginReportODS", 0 );
    _depura ( "END pluginReportODS::pluginReportODS", 0 );
}


///
/**
**/
pluginReportODS::~pluginReportODS()
{
    _depura ( "pluginReportODS::~pluginReportODS", 0 );
    _depura ( "END pluginReportODS::~pluginReportODS", 0 );
}




/*
OK, aqui poden haver passat 3 coses.
1.- Petada de la base de dades perque el SQL es erroni o s'ha perdut la conexio. 
	Aleshores el punter al cursor valdrá NULL.
    El tractament d'aquesta excepció se pot fer de dues formes:

    a) comproves que el cursor es distint de NULL abans de seguir. 
	if (cur != NULL)
    
	b) Fas tractament de excepcions generalitzat.
    	try { 
	        } catch (...) {
    	// Si hi ha hagut el error estaré en aquest punt i puc donar el missatge        
        // de error i esborrar les variables que ja no faci servir.
        // Per exemple d'aquesta forma:
           mensajeInfo("Error con la base de datos");
           if (cur) delete cur;
        }


	2.- El query ha anat bé pero no hi ha resultats.
    	Aquesta no es una excepció. Normalment pot passar i ha de esser contemplat.
        Imagina que per exemple amb un query buid intentem accedir a una dada del 
		registre.
        
		valor = cur->valor("idcuenta");

        Això petaría perque es un accés ilegal a una casella que no existeix.

         Per aixó normalment sempre després de fer un cargacursor solem fer:
         if (! cur->eof()) {
         }

         o si el que volem es recorrer el query ho feim d'aquesta altre forma

         while (!cur->eof()) {
         	 cur->siguienteregistro();
         }

        Nota que pot esser normal obtenir un resultat buid, pero en canvi no sol 
		esser gens normal el tenir un error de execució (només quan ho estas 
		programant).


	3.- El query ha anat bé i hi ha resultats. Aqui no tens problemes, 

*/





///
/**
\param cuenta
\return
**/
Fixed pluginReportODS::saldoCuenta ( int cuenta )
{
    _depura ( "pluginReportODS::saldoCuenta", 0 );
    /// Ejercicio actual.
    QString query;
    cursor2 *cur;
    Fixed resultado;
    query.sprintf ( "SELECT (SUM(apunte.debe) - SUM(apunte.haber)) AS saldo FROM cuenta LEFT JOIN apunte ON apunte.idcuenta = cuenta.idcuenta WHERE cuenta.codigo like '%s%%' AND fecha <= '%s'", QString::number ( cuenta ).toAscii().constData(), ejercicioActual_fechaBalance.toAscii().constData() );

    conexionbase->begin();
    cur = conexionbase->cargacursor ( query );
    
	if (cur != NULL)
	{
		resultado = cur->valor ( "saldo" );
    	conexionbase->commit();
	}
	else
		mensajeInfo(trUtf8("Error con la base de datos"));

    _depura ( "END pluginReportODS::saldoCuenta", 0 );
    return resultado;
}


///
/**
\param cuenta
\return
**/
Fixed pluginReportODS::saldoCuentaAnt ( int cuenta )
{
    _depura ( "pluginReportODS::saldoCuentaAnt", 0 );
    /// Ejercicio anterior.
    QString query;
    cursor2 *cur;
    Fixed resultado;
    query.sprintf ( "SELECT (SUM(apunte.debe) - SUM(apunte.haber)) AS saldo FROM cuenta LEFT JOIN apunte ON apunte.idcuenta = cuenta.idcuenta WHERE cuenta.codigo like '%s%%' AND fecha <= '%s'", QString::number ( cuenta ).toAscii().constData(), ejercicioAnterior_fechaBalance.toAscii().constData() );

    conexionbase->begin();
    cur = conexionbase->cargacursor ( query );
	if (cur != NULL)
	{
		resultado = cur->valor ( "saldo" );
    	conexionbase->commit();
	}
	else
		mensajeInfo(trUtf8("Error con la base de datos"));

    _depura ( "END pluginReportODS::saldoCuentaAnt", 0 );
    return resultado;
}



///
/**
\return
**/
void pluginReportODS::formDatosBalance(CAnuales tipus)
{
    _depura ( "pluginReportODS::formDatosBalance", 0 );
    int resultado;

    datosView *dv = new datosView ( 0 );
    dv->exec();
    resultado = dv->result();

    /// Accepted = 1
    /// Rejected = 0

    if ( resultado != 0 ) {
        /// Asigna valores
        ejercicioActual_fechaBalance = dv->mui_ejercicioActual_fechaBalance->text();
        ejercicioAnterior_fechaBalance = dv->mui_ejercicioAnterior_fechaBalance->text();
    } // end if
    delete dv;
        
        if(ejercicioActual_fechaBalance.isEmpty()){
                mensajeError ( trUtf8("ERROR: Debe introducir una fecha en el balance actual."));
                resultado = 0;
        }
        else{
                if(ejercicioAnterior_fechaBalance.isEmpty()){
                        mensajeError ( trUtf8("ERROR: Debe introducir una fecha en el balance anterior."));
                        resultado = 0;
                }
    } // end if

    /// Se pulso el boton 'cancelar'
    if ( resultado == 0 ) {
        return;
    } // end if

    /// Generamos el balance
    balanceSituacionODS(tipus);
    _depura ( "END pluginReportODS::formDatosBalance", 0 );
}


//Cuentas Anuales Abreviadas Asociación Sin Lucro CAAASL
//Cuentas Anuales Plan General Contable hasta 2007 CAPGC07
//Cuentas Anuales PYMES 2008 CAPYMES08
//Cuentas Anuales Plan General Contable 2008 CAPGC08
//Cuentas Anuales Abreviadas Plan General Contable 2008 CAAPGC08


///
/**
**/
void pluginReportODS::balanceSituacionODS(CAnuales tipus)
{
    _depura ( "pluginReportODS::balanceSituacionODS", 0 );
    /// Se genera el Balance de Situacion en formato ODS (Hoja de calculo OpenOffice.org).

    /// BUG: Se necesita usar .toQString('.') porque sino los decimales no
    /// aparecen bien en OpenOffice. Creo que es un bug del script de conversion de .py a .ods
	QString archivosalida;
	bool error = false;
	switch (tipus)
	{
		case CAAASL:
			archivosalida = cuentaAnualAsociancionSinLucro ();
			break;
		
		case CAPGC07:
			archivosalida = cuentaAnualCAPGC07();
			break;
		
		case CAPYMES08:
			archivosalida = cuentaAnualCAPYMES08();
			break;
		
		case CAAPGC08:
			archivosalida = cuentaAnualCAAPGC08();
			break;

		case CAPGC08:
			archivosalida = cuentaAnualCAPGC08();
			break;
		
		default:
			mensajeError ( trUtf8("ERROR: Funcion no implementada todavia."));
            break;
	}


	QString archivod = confpr->valor ( CONF_DIR_USER ) + "canualesods.py";

    QString cadena = "rm " + confpr->valor ( CONF_DIR_USER ) + "canualesods.ods";
    system ( cadena.toAscii() );
    cadena = "rm " + archivod;
    system ( cadena.toAscii() );

    QFile file ( archivod );
    if ( file.open ( QIODevice::WriteOnly ) ) {
    	QTextStream stream ( &file );
        stream.setCodec ( "UTF-8" );
        stream << archivosalida.toAscii();
        file.close();
    } else 
		{
        	mensajeError ( trUtf8("ERROR: No se ha podido crear el archivo" ));
    } // end if

    cadena = " cd " + confpr->valor ( CONF_DIR_USER ) + "; python " + archivod;
    system ( cadena.toAscii() );
    cadena = "oocalc " + confpr->valor ( CONF_DIR_USER ) + "canualesods.ods &";
    system ( cadena.toAscii() );
    _depura ( "END pluginReportODS::balanceSituacionODS", 0 );
}

//Cuentas Anuales Abreviadas Asociación Sin Lucro CAAASL
//Cuentas Anuales Plan General Contable hasta 2007 CAPGC07
//Cuentas Anuales PYMES 2008 CAPYMES08
//Cuentas Anuales Plan General Contable 2008 CAPGC08
//Cuentas Anuales Abreviadas Plan General Contable 2008 CAAPGC08

///
    /**
    \param bcont
    **/
    void pluginReportODS::inicializa ( Bulmacont *bcont )
{
    _depura ( "pluginReportODS::inicializa", 0 );
    empresaact = bcont->empresaactual();
    conexionbase = empresaact->bdempresa();
    /// Creamos el men&uacute;.
    QMenu *pPluginMenu = new QMenu ( trUtf8("&Cuentas Anuales") );

	QMenu *pgc08 = new QMenu ( trUtf8("&PGC 2008") );
	QMenu *pgc07 = new QMenu ( trUtf8("PGC &Anterior 2008") );

    QAction *accion4 = new QAction ( trUtf8("&Cuentas Anuales PGC"), 0 );
    accion4->setStatusTip ( trUtf8("Cuentas Anuales Plan General Contable 2008") );
    accion4->setWhatsThis ( trUtf8("Cuentas Anuales Plan General Contable 2008") );
    connect ( accion4, SIGNAL ( activated() ), this, SLOT ( balsitCAPGC08() ) );
    
    QAction *accion5 = new QAction ( trUtf8("Cuentas Anuales &Abreviadas PGC"), 0 );
    accion4->setStatusTip ( trUtf8("Cuentas Anuales Abreviadas Plan General Contable 2008") );
    accion4->setWhatsThis ( trUtf8("Cuentas Anuales Abreviadas Plan General Contable 2008") );
    connect ( accion5, SIGNAL ( activated() ), this, SLOT ( balsitCAAPGC08() ) );
	
	QAction *accion3 = new QAction ( trUtf8("Cuentas Anuales &PYMES"), 0 );
    accion3->setStatusTip ( trUtf8("Cuentas Anuales PYMES 2008" ));
    accion3->setWhatsThis ( trUtf8("Cuentas Anuales PYMES 2008" ));
    connect ( accion3, SIGNAL ( activated() ), this, SLOT ( balsitCAPYMES08() ) );

	/**********************************************/

	QAction *accion2 = new QAction ( trUtf8("&Cuentas Anuales PGC"), 0 );
    accion2->setStatusTip (trUtf8( "Cuentas Anuales Plan General Contable hasta 2007" ));
    accion2->setWhatsThis ( trUtf8("Cuentas Anuales Plan General Contable hasta 2007" ));
    connect ( accion2, SIGNAL ( activated() ), this, SLOT ( balsitCAPGC07() ) );

	QAction *accion1 = new QAction ( trUtf8("Cuentas A. Abreviadas &Sin lucro"), 0 );
    accion1->setStatusTip (trUtf8( "Cuentas Anuales Abreviadas Asociación Sin ánimo de lucro" ));
    accion1->setWhatsThis (trUtf8( "Cuentas Anuales Abreviadas Asociación Sin ánimo de lucro" ));
    connect ( accion1, SIGNAL ( activated() ), this, SLOT ( balsitCAAASL() ) );
	

	pPluginMenu->addMenu(pgc08);
	pPluginMenu->addSeparator();
	pPluginMenu->addMenu(pgc07);

	pgc08->addAction ( accion4 ); //PGC
	pgc08->addAction (accion5); 
    pgc08->addAction (accion3); 


	pgc07->addAction ( accion2 );
    pgc07->addAction ( accion1 );


	/// A&ntilde;adimos la nueva opci&oacute;n al men&uacute; principal del programa.
    bcont->menuBar() ->addMenu ( pPluginMenu );
    _depura ( "END pluginReportODS::inicializa", 0 );
}


///
    /**
    \param bcont
    **/
    void entryPoint ( Bulmacont *bcont )
{
    _depura ( "Estoy dentro del plugin\n", 0 );
    pluginReportODS *plug = new pluginReportODS();
    plug->inicializa ( bcont );
    /// S&Oacute;LO A MODO DE EJEMPLO: se modifica el t&iacute;tulo de la ventana principal
    /// del programa para indicar que el plugin se ha cargado.
}


///
/**
\param cuentaPositiva
\return
**/
Fixed pluginReportODS::cuentaPositiva( Fixed valor )
{
    _depura ( "pluginReportODS::cuentaPositiva", 0 );
    /// Comprueba que sea un número positivo
    Fixed resultado;
    if (valor > Fixed("0.00"))
		resultado = valor;
    else
		resultado = Fixed("0.00"); 
    return resultado;
}

///
/**
\param cuentaNegativa
\return
**/
Fixed pluginReportODS::cuentaNegativa( Fixed valor )
{
    _depura ( "pluginReportODS::cuentaNegativa", 0 );
    /// Comprueba que sea un número negativo
    Fixed resultado;
    if (valor < Fixed("0.00"))
		resultado = valor;
    else
		resultado = Fixed("0.00");
    return resultado;
}

// CAnuales CAAASL, CAPGC07, CAPYMES08, CAPGC08, CAAPGC08}

void pluginReportODS::balsitCAAASL ()
{
	CAnuales CA = CAAASL;
	mensajeAdvertenciaPGC(CA);
	formDatosBalance(CA);
}

void pluginReportODS::balsitCAPGC07 ()
{
	CAnuales CA = CAPGC07;
	mensajeAdvertenciaPGC(CA);
	formDatosBalance(CA);
}

void pluginReportODS::balsitCAPYMES08 ()
{
	CAnuales CA = CAPYMES08;
	mensajeAdvertenciaPGC(CA);
	formDatosBalance(CA);
}

void pluginReportODS::balsitCAPGC08()
{
	CAnuales CA = CAPGC08;
	mensajeAdvertenciaPGC(CA);
	formDatosBalance(CA);
}

void pluginReportODS::balsitCAAPGC08()
{
	CAnuales CA = CAAPGC08;
	mensajeAdvertenciaPGC(CA);
	formDatosBalance(CA);
}

void pluginReportODS::mensajeAdvertenciaPGC(CAnuales tipus)
{
//Cuentas Anuales Abreviadas Asociación Sin Lucro CAAASL
//Cuentas Anuales Plan General Contable hasta 2007 CAPGC07
//Cuentas Anuales PYMES 2008 CAPYMES08
//Cuentas Anuales Plan General Contable 2008 CAPGC08
//Cuentas Anuales Abreviadas Plan General Contable 2008 CAAPGC08

	QString pgc;

	switch (tipus)
	{
		case CAAASL:
			pgc = trUtf8("Cuentas Anuales Abreviadas para Asociacion Sin Lucro. ");
			break;
		
		case CAPGC07:
			pgc = trUtf8("Cuentas Anuales del Plan General Contable hasta 2007. ");
			break;
		
		case CAPYMES08:
			pgc = trUtf8("Cuentas Anuales para PYMES 2008. ") ;
			break;
		
		case CAAPGC08:
			pgc = trUtf8("Cuentas Anuales Abreviadas del Plan General Contable 2008. ");
			break;
		
		case CAPGC08:
			pgc = trUtf8("Cuentas Anuales del Plan General Contable 2008. ");
			break;
		
		default:
			mensajeError ( trUtf8("ERROR: Funcion no implementada todavia."));
            break;
	}

	mensajeAviso( trUtf8("Advertencia: ha escojido las ") + pgc + trUtf8("El resultado solo sera correcto si coincide con el plan contable de su empresa."));
}


///
/**
**/
QString pluginReportODS::cuentaAnualAsociancionSinLucro ()
{
	return QString::QString("\
#!/usr/bin/python\n\
# -*- coding: utf8 -*-\n\
\n\
import ooolib\n\
\n\
# Crea el documento\n\
doc = ooolib.Calc(\"Activo\")\n\
\n\
\
doc.set_column_property(1, 'width', '0.5in')\n\
doc.set_column_property(2, 'width', '5in')\n\
doc.set_cell_property('bold', True)\n\
doc.set_cell_value(2, 2, 'string', 'BALANCE DE SITUACIÓN ABREVIADO')\n\
doc.set_cell_value(2, 3, 'string', '" + QDateTime::currentDateTime().toString() + "')\n\
doc.set_cell_value(2, 5, 'string', 'ACTIVO')\n\
doc.set_cell_value(3, 5, 'string', 'Ejercicio N')\n\
doc.set_cell_value(4, 5, 'string', 'Ejercicio N-1')\n\
doc.set_cell_property('bold', False)\n\
\
doc.set_cell_property('bold', True)\n\
doc.set_cell_value(2, 7, 'string', 'A) Fundadores/asociados por desembolsos no exigidos')\n\
doc.set_cell_value(3, 7, 'float', '" + ( saldoCuenta ( 196 ) + saldoCuenta ( 197 ) ).toQString ( '.' ) + "')\n\
doc.set_cell_value(4, 7, 'float', '" + ( saldoCuentaAnt ( 196 ) + saldoCuentaAnt ( 197 ) ).toQString ( '.' ) + "')\n\
doc.set_cell_property('bold', False)\n\
\
doc.set_cell_property('bold', True)\n\
doc.set_cell_value(2, 9, 'string', 'B) Inmovilizado')\n\
doc.set_cell_value(3, 9, 'formula', '=C10+C11+C12+C13+C14')\n\
doc.set_cell_value(4, 9, 'formula', '=D10+D11+D12+D13+D14')\n\
doc.set_cell_property('bold', False)\n\
\
doc.set_cell_value(2, 10, 'string', '   I.   Gastos de establecimiento')\n\
doc.set_cell_value(3, 10, 'float', '" + saldoCuenta ( 20 ).toQString ( '.' ) + "')\n\
doc.set_cell_value(4, 10, 'float', '" + saldoCuentaAnt ( 20 ).toQString ( '.' ) + "')\n\
\
\
doc.set_cell_value(2, 11, 'string', '   II.  Inmovilizaciones inmateriales')\n\
doc.set_cell_value(3, 11, 'float', '" + (saldoCuenta ( 21 ) + ( saldoCuenta ( 281 ) ) + ( saldoCuenta ( 291 ) )).toQString ( '.' ) + "')\n\
doc.set_cell_value(4, 11, 'float', '" + (saldoCuentaAnt ( 21 ) + ( saldoCuentaAnt ( 281 ) ) + ( saldoCuentaAnt ( 291 ) )).toQString ( '.' ) + "')\n\
\
doc.set_cell_value(2, 12, 'string', '   III. Bienes de Patrimonio Histórico')\n\
doc.set_cell_value(3, 12, 'float', '" + (saldoCuenta ( 230 ) + saldoCuenta ( 231 ) + saldoCuenta ( 232 ) + saldoCuenta ( 233 ) + saldoCuenta ( 234 ) + saldoCuenta ( 239 ) + ( saldoCuentaAnt ( 2921 ) )).toQString ( '.' ) + "')\n\
doc.set_cell_value(4, 12, 'float', '" + (saldoCuentaAnt ( 230 ) + saldoCuentaAnt ( 231 ) + saldoCuentaAnt ( 232 ) + saldoCuentaAnt ( 233 ) + saldoCuentaAnt ( 234 ) + saldoCuentaAnt ( 239 ) + ( saldoCuentaAnt ( 2921 ))).toQString ( '.' ) + "')\n\
\
doc.set_cell_value(2, 13, 'string', '   IV.  Otras inmovilizaciones materiales')\n\
doc.set_cell_value(3, 13, 'float', '" +( saldoCuenta ( 22 ) + ( saldoCuenta ( 282 ) ) + ( saldoCuenta ( 2920 ) )).toQString ( '.' ) + "')\n\
doc.set_cell_value(4, 13, 'float', '" + ( saldoCuentaAnt ( 22 ) + ( saldoCuentaAnt ( 282 ) ) + ( saldoCuentaAnt ( 2920 ) )).toQString ( '.' ) + "')\n\
\
doc.set_cell_value(2, 14, 'string', '   V.   Inmovilizaciones financieras')\n\
doc.set_cell_value(3, 14, 'float', '" + ( saldoCuenta ( 240 ) + saldoCuenta ( 241 ) + saldoCuenta ( 242 ) + saldoCuenta ( 243 ) + saldoCuenta ( 244 ) + saldoCuenta ( 245 ) + saldoCuenta ( 246 )  + saldoCuenta ( 247 ) + saldoCuenta ( 250 ) + saldoCuenta ( 251 ) + saldoCuenta ( 252 ) + saldoCuenta ( 253 ) + saldoCuenta ( 254 ) + saldoCuenta ( 256 ) + saldoCuenta ( 257 ) + saldoCuenta ( 258 ) + saldoCuenta ( 26 ) + ( saldoCuenta ( 293 ) ) + ( saldoCuenta ( 294 ) ) + ( saldoCuenta ( 295 ) ) + ( saldoCuenta ( 296 ) ) + ( saldoCuenta ( 297 ) ) + ( saldoCuenta ( 298 ) )).toQString ( '.' ) + "')\n\
doc.set_cell_value(4, 14, 'float', '" + ( saldoCuentaAnt ( 240 ) + saldoCuentaAnt ( 241 ) + saldoCuentaAnt ( 242 ) + saldoCuentaAnt ( 243 ) + saldoCuentaAnt ( 244 ) + saldoCuentaAnt ( 245 ) + saldoCuentaAnt ( 246 )  + saldoCuentaAnt ( 247 ) + saldoCuentaAnt ( 250 ) + saldoCuentaAnt ( 251 ) + saldoCuentaAnt ( 252 ) + saldoCuentaAnt ( 253 ) + saldoCuentaAnt ( 254 ) + saldoCuentaAnt ( 256 ) + saldoCuentaAnt ( 257 ) + saldoCuentaAnt ( 258 ) + saldoCuentaAnt ( 26 ) + ( saldoCuentaAnt ( 293 ) ) + ( saldoCuentaAnt ( 294 ) ) + ( saldoCuentaAnt ( 295 ) ) + ( saldoCuentaAnt ( 296 ) ) + ( saldoCuentaAnt ( 297 ) ) + ( saldoCuentaAnt ( 298 ) )).toQString ( '.' ) + "')\n\
\
doc.set_cell_property('bold', True)\n\
doc.set_cell_value(2, 15, 'string', 'C) Gastos a distribuir en varios ejercicios')\n\
doc.set_cell_value(3, 15, 'float', '" + saldoCuenta ( 27 ).toQString ( '.' ) + "')\n\
doc.set_cell_value(4, 15, 'float', '" + saldoCuentaAnt ( 27 ).toQString ( '.' ) + "')\n\
doc.set_cell_property('bold', False)\n\
\
doc.set_cell_property('bold', True)\n\
doc.set_cell_value(2, 16, 'string', 'D) Activo circulante')\n\
doc.set_cell_value(3, 16, 'formula', '=C17+C18+C19+C20+C21+C22+C23')\n\
doc.set_cell_value(4, 16, 'formula', '=D17+D18+D19+D20+D21+D22+D23')\n\
doc.set_cell_property('bold', False)\n\
\
doc.set_cell_value(2, 17, 'string', '   I.   Fundadores/asociados por desembolsos exigidos')\n\
doc.set_cell_value(3, 17, 'float', '" + saldoCuenta ( 558 ).toQString ( '.' ) + "')\n\
doc.set_cell_value(4, 17, 'float', '" + saldoCuentaAnt ( 558 ).toQString ( '.' ) + "')\n\
\
doc.set_cell_value(2, 18, 'string', '   II.  Existencias')\n\
doc.set_cell_value(3, 18, 'float', '" + ( saldoCuenta ( 30 ) + saldoCuenta ( 31 ) + saldoCuenta ( 32 ) + saldoCuenta ( 33 ) + saldoCuenta ( 34 ) + saldoCuenta ( 35 ) + saldoCuenta ( 36 ) + ( saldoCuenta ( 39 ) ) + saldoCuenta ( 407 )).toQString ( '.' ) + "')\n\
doc.set_cell_value(4, 18, 'float', '"  + (saldoCuentaAnt ( 30 ) + saldoCuentaAnt ( 31 ) + saldoCuentaAnt ( 32 ) + saldoCuentaAnt ( 33 ) + saldoCuentaAnt ( 34 ) + saldoCuentaAnt ( 35 ) + saldoCuentaAnt ( 36 ) + ( saldoCuentaAnt ( 39 ) ) + saldoCuentaAnt ( 407 )).toQString ( '.' ) + "')\n\
\
doc.set_cell_value(2, 19, 'string', '   III. Usuarios y otros deudores de la actividad propia')\n\
doc.set_cell_value(3, 19, 'float', '"  + ( saldoCuenta ( 446 ) + saldoCuenta ( 447 ) + ( saldoCuenta ( 495 ) )).toQString ( '.' ) + "')\n\
doc.set_cell_value(4, 19, 'float', '"  + (saldoCuentaAnt ( 446 ) + saldoCuentaAnt ( 447 ) + ( saldoCuentaAnt ( 495 ) )).toQString ( '.' ) + "')\n\
\
doc.set_cell_value(2, 20, 'string', '   IV.  Otros deudores')\n\
doc.set_cell_value(3, 20, 'float', '" + ( saldoCuenta ( 430 ) + saldoCuenta ( 431 ) + saldoCuenta ( 432 ) + saldoCuenta ( 433 ) + saldoCuenta ( 435 ) + ( saldoCuenta ( 436 ) ) + saldoCuenta ( 440 )  + saldoCuenta ( 441 ) + saldoCuenta ( 445 ) + saldoCuenta ( 449 ) + saldoCuenta ( 460 ) + saldoCuenta ( 464 ) + saldoCuenta ( 470 ) + saldoCuenta ( 471 ) + saldoCuenta ( 472 ) + saldoCuenta ( 474 ) + ( saldoCuenta ( 490 ) ) + ( saldoCuenta ( 493 ) ) + ( saldoCuenta ( 494 ) ) + saldoCuenta ( 544 ) + cuentaPositiva( saldoCuenta ( 551 ) ) + cuentaPositiva( saldoCuenta ( 552 ) )+ cuentaPositiva( saldoCuenta ( 553 ) ) ).toQString ( '.' ) + "')\n\
doc.set_cell_value(4, 20, 'float', '" + ( saldoCuentaAnt ( 430 ) + saldoCuentaAnt ( 431 ) + saldoCuentaAnt ( 432 ) + saldoCuentaAnt ( 433 ) + saldoCuentaAnt ( 435 ) + ( saldoCuentaAnt ( 436 ) ) + saldoCuentaAnt ( 440 )  + saldoCuentaAnt ( 441 ) + saldoCuentaAnt ( 445 ) + saldoCuentaAnt ( 449 ) + saldoCuentaAnt ( 460 ) + saldoCuentaAnt ( 464 ) + saldoCuentaAnt ( 470 ) + saldoCuentaAnt ( 471 ) + saldoCuentaAnt ( 472 ) + saldoCuentaAnt ( 474 ) + ( saldoCuentaAnt ( 490 ) ) + ( saldoCuentaAnt ( 493 ) ) + ( saldoCuentaAnt ( 494 ) ) + saldoCuentaAnt ( 544 ) + cuentaPositiva( saldoCuentaAnt ( 551 ) ) + cuentaPositiva( saldoCuentaAnt ( 552 ) )+ cuentaPositiva( saldoCuentaAnt ( 553 ) )).toQString ( '.' ) + "')\n\
\
doc.set_cell_value(2, 21, 'string', '   IV.  Inversiones financieras temporales')\n\
doc.set_cell_value(3, 21, 'float', '" + ( saldoCuenta ( 53 ) + saldoCuenta ( 540 ) + saldoCuenta ( 541 ) + saldoCuenta ( 542 ) + saldoCuenta ( 543 ) + saldoCuenta ( 545 ) + saldoCuenta ( 546 )  + saldoCuenta ( 547 ) + saldoCuenta ( 548 ) + ( saldoCuenta ( 549 ) ) + saldoCuenta ( 565 ) + saldoCuenta ( 566 ) + ( saldoCuenta ( 59 ) )).toQString ( '.' ) + "')\n\
doc.set_cell_value(4, 21, 'float', '" + ( saldoCuentaAnt ( 53 ) + saldoCuentaAnt ( 540 ) + saldoCuentaAnt ( 541 ) + saldoCuentaAnt ( 542 ) + saldoCuentaAnt ( 543 ) + saldoCuentaAnt ( 545 ) + saldoCuentaAnt ( 546 )  + saldoCuentaAnt ( 547 ) + saldoCuentaAnt ( 548 ) + ( saldoCuentaAnt ( 549 ) ) + saldoCuentaAnt ( 565 ) + saldoCuentaAnt ( 566 ) + ( saldoCuentaAnt ( 59 ) )).toQString ( '.' ) + "')\n\
\
doc.set_cell_value(2, 22, 'string', '   VI.  Tesorería')\n\
doc.set_cell_value(3, 22, 'float', '" + saldoCuenta ( 57 ).toQString ( '.' ) + "')\n\
doc.set_cell_value(4, 22, 'float', '" + saldoCuentaAnt ( 57 ).toQString ( '.' ) + "')\n\
\
doc.set_cell_value(2, 23, 'string', '   VII. Ajustes por periodificación')\n\
doc.set_cell_value(3, 23, 'float', '" + ( saldoCuenta ( 480 ) + saldoCuenta ( 580 ) ).toQString ( '.' ) + "')\n\
doc.set_cell_value(4, 23, 'float', '" + ( saldoCuentaAnt ( 480 ) + saldoCuentaAnt ( 580 ) ).toQString ( '.' ) + "')\n\
\
doc.set_cell_property('bold', True)\n\
doc.set_cell_value(2, 25, 'string', 'TOTAL GENERAL (A+B+C+D)')\n\
doc.set_cell_value(3, 25, 'formula', '=C7+C9+C15+C16')\n\
doc.set_cell_value(4, 25, 'formula', '=D7+D9+D15+D16')\n\
doc.set_cell_property('bold', False)\n\
\
\
\
\
\
\
doc.new_sheet(\"Pasivo\")\n\n\
doc.set_column_property(1, 'width', '0.5in')\n\
doc.set_column_property(2, 'width', '5in')\n\
doc.set_cell_property('bold', True)\n\
doc.set_cell_value(2, 2, 'string', 'BALANCE DE SITUACIÓN ABREVIADO')\n\
doc.set_cell_value(2, 5, 'string', 'PASIVO')\n\
doc.set_cell_value(3, 5, 'string', 'Ejercicio N')\n\
doc.set_cell_value(4, 5, 'string', 'Ejercicio N-1')\n\
doc.set_cell_property('bold', False)\n\
\
doc.set_cell_property('bold', True)\n\
doc.set_cell_value(2, 7, 'string', 'A) Fondos propios')\n\
doc.set_cell_value(3, 7, 'formula', '=C8+C9+C10+C11+C12')\n\
doc.set_cell_value(4, 7, 'formula', '=D8+D9+D10+D11+D12')\n\
doc.set_cell_property('bold', False)\n\
\
doc.set_cell_value(2, 8, 'string', '   I.   Dotación fundacional/Fondo social')\n\
doc.set_cell_value(3, 8, 'float', '" + (- saldoCuenta ( 10 )).toQString ( '.' ) + "')\n\
doc.set_cell_value(4, 8, 'float', '" +(- saldoCuentaAnt ( 10 )).toQString ( '.' ) + "')\n\
\
doc.set_cell_value(2, 9, 'string', '   II.  Reserva de revalorización')\n\
doc.set_cell_value(3, 9, 'float', '" +(- saldoCuenta ( 111 )).toQString ( '.' ) + "')\n\
doc.set_cell_value(4, 9, 'float', '" +(- saldoCuentaAnt ( 111 )).toQString ( '.' ) + "')\n\
\
doc.set_cell_value(2, 10, 'string', '   III. Reservas')\n\
doc.set_cell_value(3, 10, 'float', '"  +( - ( saldoCuenta ( 113 ) + saldoCuenta ( 116 ) + saldoCuenta ( 117 ))).toQString ( '.' ) + "')\n\
doc.set_cell_value(4, 10, 'float', '"+( - ( saldoCuentaAnt ( 113 ) + saldoCuentaAnt ( 116 ) + saldoCuentaAnt ( 117 ))).toQString ( '.' ) + "')\n\
\
doc.set_cell_value(2, 11, 'string', '   IV.  Excedentes de ejercicions anteriores')\n\
doc.set_cell_value(3, 11, 'float', '" +( -( saldoCuenta ( 120 ) + ( saldoCuenta ( 121 )))).toQString ( '.' ) + "')\n\
doc.set_cell_value(4, 11, 'float', '" +( - ( saldoCuentaAnt ( 120 ) + ( saldoCuentaAnt ( 121 )))).toQString ( '.' ) + "')\n\
\
doc.set_cell_value(2, 12, 'string', '   V.   Excedentes del ejercicio (positivo o negativo)')\n\
doc.set_cell_value(3, 12, 'float', '" +( - saldoCuenta ( 129 )).toQString ( '.' ) + "')\n\
doc.set_cell_value(4, 12, 'float', '"  +( - saldoCuentaAnt ( 129 )).toQString ( '.' ) + "')\n\
\
doc.set_cell_property('bold', True)\n\
doc.set_cell_value(2, 14, 'string', 'B) Ingresos a distribuir en varios ejercicios')\n\
doc.set_cell_value(3, 14, 'float', '" +( - saldoCuenta ( 13 )).toQString ( '.' ) + "')\n\
doc.set_cell_value(4, 14, 'float', '" +( - saldoCuentaAnt ( 13 )).toQString ( '.' ) + "')\n\
doc.set_cell_property('bold', False)\n\
\
doc.set_cell_property('bold', True)\n\
doc.set_cell_value(2, 16, 'string', 'C) Provisiones para riesgos y gastos')\n\
doc.set_cell_value(3, 16, 'float', '" +( -  saldoCuenta ( 14 )).toQString ( '.' ) + "')\n\
doc.set_cell_value(4, 16, 'float', '" +( - saldoCuentaAnt ( 14 )).toQString ( '.' ) + "')\n\
doc.set_cell_property('bold', False)\n\
\
doc.set_cell_property('bold', True)\n\
doc.set_cell_value(2, 17, 'string', 'D) Acreedores a largo plazo')\n\
doc.set_cell_value(3, 17, 'float', '"  +( - ( saldoCuenta ( 15 ) + saldoCuenta ( 16 ) + saldoCuenta ( 17 ) + saldoCuenta ( 18 ) + saldoCuenta ( 248 ) + saldoCuenta ( 249 ) + saldoCuenta ( 259 ))).toQString ( '.' ) + "')\n\
doc.set_cell_value(4, 17, 'float', '"  +( - ( saldoCuentaAnt ( 15 ) + saldoCuentaAnt ( 16 ) + saldoCuentaAnt ( 17 ) + saldoCuentaAnt ( 18 ) + saldoCuentaAnt ( 248 ) + saldoCuentaAnt ( 249 ) + saldoCuentaAnt ( 259 ))).toQString ( '.' ) + "')\n\
doc.set_cell_property('bold', False)\n\
\
doc.set_cell_property('bold', True)\n\
doc.set_cell_value(2, 19, 'string', 'E) Acreedores a corto plazo')\n\
doc.set_cell_value(3, 19, 'float', '" +(- ( saldoCuenta ( 400 ) + saldoCuenta ( 401 ) + saldoCuenta ( 402 ) + saldoCuenta ( 403 ) + ( saldoCuenta ( 406 ) ) + saldoCuenta ( 41 ) + saldoCuenta ( 437 ) + saldoCuenta ( 465 ) + saldoCuenta ( 475 ) + saldoCuenta ( 476 ) + saldoCuenta ( 477 ) + saldoCuenta ( 479 ) + saldoCuenta ( 485 ) + saldoCuenta ( 499 ) + saldoCuenta ( 50 ) + saldoCuenta ( 51 ) + saldoCuenta ( 52 ) + cuentaNegativa( saldoCuenta ( 551 )) + cuentaNegativa( saldoCuenta ( 552 )) + cuentaNegativa( saldoCuenta ( 553 )) + saldoCuenta ( 555 ) + saldoCuenta ( 556 ) + saldoCuenta ( 560 ) + saldoCuenta ( 561 ) + saldoCuenta ( 585 ) )).toQString ( '.' ) + "')\n\
doc.set_cell_value(4, 19, 'float', '" + (- ( saldoCuentaAnt ( 400 ) + saldoCuentaAnt ( 401 ) + saldoCuentaAnt ( 402 ) + saldoCuentaAnt ( 403 ) + ( saldoCuentaAnt ( 406 ) ) + saldoCuentaAnt ( 41 ) + saldoCuentaAnt ( 437 ) + saldoCuentaAnt ( 465 ) + saldoCuentaAnt ( 475 ) + saldoCuentaAnt ( 476 ) + saldoCuentaAnt ( 477 ) + saldoCuentaAnt ( 479 ) + saldoCuentaAnt ( 485 ) + saldoCuentaAnt ( 499 ) + saldoCuentaAnt ( 50 ) + saldoCuentaAnt ( 51 ) + saldoCuentaAnt ( 52 ) + cuentaNegativa( saldoCuentaAnt ( 551 )) + cuentaNegativa( saldoCuentaAnt ( 552 )) + cuentaNegativa( saldoCuentaAnt ( 553 )) + saldoCuentaAnt ( 555 ) + saldoCuentaAnt ( 556 ) + saldoCuentaAnt ( 560 ) + saldoCuentaAnt ( 561 ) + saldoCuentaAnt ( 585 )) ).toQString ( '.' ) + "')\n\
doc.set_cell_property('bold', False)\n\
\
doc.set_cell_property('bold', True)\n\
doc.set_cell_value(2, 21, 'string', 'TOTAL GENERAL (A+B+C+D+E)')\n\
doc.set_cell_value(3, 21, 'formula', '=C7+C14+C16+C17+C19')\n\
doc.set_cell_value(4, 21, 'formula', '=D7+D14+D16+D17+D19')\n\
doc.set_cell_property('bold', False)\n\
\
\
\
\
\
\
\
doc.new_sheet(\"Debe\")\n\n\
doc.set_column_property(1, 'width', '0.5in')\n\
doc.set_column_property(2, 'width', '5in')\n\
doc.set_cell_property('bold', True)\n\
doc.set_cell_value(2, 2, 'string', 'CUENTA DE RESULTADOS ABREVIADA')\n\
doc.set_cell_value(2, 5, 'string', 'DEBE')\n\
doc.set_cell_property('bold', False)\n\
\
doc.set_cell_property('bold', True)\n\
doc.set_cell_value(2, 7, 'string', 'A) GASTOS')\n\
doc.set_cell_value(3, 7, 'formula', '=0')\n\
doc.set_cell_value(4, 7, 'formula', '=0')\n\
doc.set_cell_property('bold', False)\n\
\
doc.set_cell_value(2, 8, 'string', '       1. Ayudas monetarias y otros')\n\
doc.set_cell_value(3, 8, 'formula', '=C9+C10+C11')\n\
doc.set_cell_value(4, 8, 'formula', '=D9+D10+D11')\n\
\
doc.set_cell_value(2, 9, 'string', '            a) Ayudas monetarias')\n\
doc.set_cell_value(3, 9, 'float', '" + ( saldoCuenta ( 650 ) + saldoCuenta ( 651 ) + saldoCuenta ( 652 ) ).toQString ( '.' ) + "')\n\
doc.set_cell_value(4, 9, 'float', '" + ( saldoCuentaAnt ( 650 ) + saldoCuentaAnt ( 651 ) + saldoCuentaAnt ( 652 ) ).toQString ( '.' ) + "')\n\
\
doc.set_cell_value(2, 10, 'string', '            b) Gastos por colaboraciones y del órgano de gobierno')\n\
doc.set_cell_value(3, 10, 'float', '" + ( saldoCuenta ( 653 ) + saldoCuenta ( 654 ) ).toQString ( '.' ) + "')\n\
doc.set_cell_value(4, 10, 'float', '" + ( saldoCuentaAnt ( 653 ) + saldoCuentaAnt ( 654 ) ).toQString ( '.' ) + "')\n\
\
doc.set_cell_value(2, 11, 'string', '            c) Reintegro de ayudas y asignaciones')\n\
doc.set_cell_value(3, 11, 'float', '" + ( saldoCuenta ( 728 ) ).toQString ( '.' ) + "')\n\
doc.set_cell_value(4, 11, 'float', '" + ( saldoCuentaAnt ( 728 ) ).toQString ( '.' ) + "')\n\
\
doc.set_cell_value(2, 12, 'string', '       2. Consumos de explotacion')\n\
doc.set_cell_value(3, 12, 'float', '" + ( saldoCuenta ( 60 ) +  saldoCuenta ( 61 ) +  saldoCuenta ( 71 )).toQString ( '.' ) + "')\n\
doc.set_cell_value(4, 12, 'float', '" + ( saldoCuentaAnt ( 60 ) +  saldoCuentaAnt ( 61 ) +  saldoCuentaAnt ( 71 )).toQString ( '.' ) + "')\n\
\
doc.set_cell_value(2, 13, 'string', '       3. Gastos de personal')\n\
doc.set_cell_value(3, 13, 'formula', '=SUM(C14:C15)')\n\
doc.set_cell_value(4, 13, 'formula', '=SUM(D14:D15)')\n\
\
doc.set_cell_value(2, 14, 'string', '            a) Sueldos, salarios y asimilados')\n\
doc.set_cell_value(3, 14, 'float', '" + ( saldoCuenta ( 640 ) + saldoCuenta ( 641 ) ).toQString ( '.' ) + "')\n\
doc.set_cell_value(4, 14, 'float', '" + ( saldoCuentaAnt ( 640 ) + saldoCuentaAnt ( 641 ) ).toQString ( '.' ) + "')\n\
doc.set_cell_value(2, 15, 'string', '            b) Cargas sociales')\n\
doc.set_cell_value(3, 15, 'float', '" + ( saldoCuenta ( 642 ) + saldoCuenta ( 643 ) + saldoCuenta ( 649 ) ).toQString ( '.' ) + "')\n\
doc.set_cell_value(4, 15, 'float', '" + ( saldoCuentaAnt ( 642 ) + saldoCuentaAnt ( 643 ) + saldoCuentaAnt ( 649 ) ).toQString ( '.' ) + "')\n\
\
doc.set_cell_value(2, 16, 'string', '       4. Dotaciones para amortizaciones de inmovilizado')\n\
doc.set_cell_value(3, 16, 'float', '" + saldoCuenta ( 68 ).toQString ( '.' ) + "')\n\
doc.set_cell_value(4, 16, 'float', '" + saldoCuentaAnt ( 68 ).toQString ( '.' ) + "')\n\
\
doc.set_cell_value(2, 17, 'string', '       5. Otros gastos')\n\
doc.set_cell_value(3, 17, 'float', '" + ( saldoCuenta ( 62 ) +  saldoCuenta ( 631 ) +  saldoCuenta ( 634 ) + ( saldoCuenta ( 636 )) + ( saldoCuenta ( 639 )) +  saldoCuenta ( 656 ) +  saldoCuenta ( 659 ) +  saldoCuenta ( 690 )).toQString ( '.' ) + "')\n\
doc.set_cell_value(4, 17, 'float', '" + ( saldoCuentaAnt ( 62 ) +  saldoCuentaAnt ( 631 ) +  saldoCuentaAnt ( 634 ) + ( saldoCuentaAnt ( 636 )) + ( saldoCuentaAnt ( 639 )) +  saldoCuentaAnt ( 656 ) +  saldoCuentaAnt ( 659 ) +  saldoCuentaAnt ( 690 )).toQString ( '.' ) + "')\n\
\
doc.set_cell_value(2, 18, 'string', '       6. Variaciones de las provisiones de la actividad')\n\
doc.set_cell_value(3, 18, 'float', '" + ( saldoCuenta ( 655 ) +  saldoCuenta ( 693 ) +  saldoCuenta ( 694 ) + saldoCuenta ( 695 ) + ( saldoCuenta ( 793 )) +  ( saldoCuenta ( 794 )) + ( saldoCuenta ( 795 ))).toQString ( '.' ) + "')\n\
doc.set_cell_value(4, 18, 'float', '" + ( saldoCuentaAnt ( 655 ) +  saldoCuentaAnt ( 693 ) +  saldoCuentaAnt ( 694 ) + saldoCuentaAnt ( 695 ) + ( saldoCuentaAnt ( 793 )) +  ( saldoCuentaAnt ( 794 )) + ( saldoCuentaAnt ( 795 ))).toQString ( '.' ) + "')\n\
\
doc.set_cell_value(2, 20, 'string', '   I.   RESULTADOS POSITIVOS DE LA EXPLOTACIÓN')\n\
doc.set_cell_value(2, 21, 'string', '        (B1+B2+B3-A1-A2-A3-A4-A5-A6)')\n\
doc.set_cell_value(3, 20, 'formula', '= if ((Haber.C8+Haber.C13+Haber.C14-C8-C12-C13-C16-C17-C18)>0;Haber.C8+Haber.C13+Haber.C14-C8-C12-C13-C16-C17-C18;T(0))')\n\
doc.set_cell_value(4, 20, 'formula', '= if((Haber.D8+Haber.D13+Haber.D14-D8-D12-D13-D16-D17-D18) >0;Haber.D8+Haber.D13+Haber.D14-D8-D12-D13-D16-D17-D18;T(0))')\n\
\
doc.set_cell_value(2, 23, 'string', '       7. Gastos financieros y gastos asimilados')\n\
doc.set_cell_value(3, 23, 'formula', '=SUM(C24:C27)')\n\
doc.set_cell_value(4, 23, 'formula', '=SUM(D24:D27)')\n\
\
doc.set_cell_value(2, 24, 'string', '            a) Por deudas con entidades del grupo')\n\
doc.set_cell_value(3, 24, 'float', '" + ( saldoCuenta ( 6610 ) +  saldoCuenta ( 6615 ) +  saldoCuenta ( 6620 ) + saldoCuenta ( 6630 ) + saldoCuenta ( 6640 ) +  saldoCuenta ( 6650 )).toQString ( '.' ) + "')\n\
doc.set_cell_value(4, 24, 'float', '" + ( saldoCuentaAnt ( 6610 ) +  saldoCuentaAnt ( 6615 ) +  saldoCuentaAnt ( 6620 ) + saldoCuentaAnt ( 6630 ) + saldoCuentaAnt ( 6640 ) +  saldoCuentaAnt ( 6650 )).toQString ( '.' ) + "')\n\
\
doc.set_cell_value(2, 25, 'string', '            d) Por deudas con entidades asociadas')\n\
doc.set_cell_value(3, 25, 'float', '" + ( saldoCuenta ( 6611 ) +  saldoCuenta ( 6616 ) +  saldoCuenta ( 6621 ) + saldoCuenta ( 6631 ) + saldoCuenta ( 6641 ) +  saldoCuenta ( 6651 )).toQString ( '.' ) + "')\n\
doc.set_cell_value(4, 25, 'float', '" + ( saldoCuentaAnt ( 6611 ) +  saldoCuentaAnt ( 6616 ) +  saldoCuentaAnt ( 6621 ) + saldoCuentaAnt ( 6631 ) + saldoCuentaAnt ( 6641 ) +  saldoCuentaAnt ( 6651 )).toQString ( '.' ) + "')\n\
\
doc.set_cell_value(2, 26, 'string', '            c) Por otras deudas')\n\
doc.set_cell_value(3, 26, 'float', '" + ( saldoCuenta ( 6613 ) +  saldoCuenta ( 6618 ) +  saldoCuenta ( 6622 ) + saldoCuenta ( 6623 )  +  saldoCuenta ( 6643 ) +  saldoCuenta ( 6653 ) + saldoCuenta ( 669 ) + saldoCuenta ( 6632 ) +  saldoCuenta ( 6633 )).toQString ( '.' ) + "')\n\
doc.set_cell_value(4, 26, 'float', '" + ( saldoCuentaAnt ( 6613 ) +  saldoCuentaAnt ( 6618 ) +  saldoCuentaAnt ( 6622 ) + saldoCuentaAnt ( 6623 )  +  saldoCuentaAnt ( 6643 ) +  saldoCuentaAnt ( 6653 ) + saldoCuentaAnt ( 669 ) + saldoCuentaAnt ( 6632 ) +  saldoCuentaAnt ( 6633 )).toQString ( '.' ) + "')\n\
\
doc.set_cell_value(2, 27, 'string', '            d) Pérdidas de inversiones financieras')\n\
doc.set_cell_value(3, 27, 'float', '" + ( saldoCuenta ( 666 ) + saldoCuenta ( 667 )).toQString ( '.' ) + "')\n\
doc.set_cell_value(4, 27, 'float', '" + ( saldoCuentaAnt ( 666 ) + saldoCuentaAnt ( 667 )).toQString ( '.' ) + "')\n\
\
doc.set_cell_value(2, 28, 'string', '       8. Variación de las provisiones de inversiones financieras')\n\
doc.set_cell_value(3, 28, 'float', '" + ( saldoCuenta ( 6963 ) + saldoCuenta ( 6965 ) + saldoCuenta ( 6966 ) + saldoCuenta ( 697 ) + saldoCuenta ( 698 ) + saldoCuenta ( 699 ) + ( saldoCuenta ( 7963 )) + ( saldoCuenta ( 7965 )) + ( saldoCuenta ( 7966 )) + ( saldoCuenta ( 797 )) + ( saldoCuenta ( 798 )) + ( saldoCuenta ( 799 ))).toQString ( '.' ) + "')\n\
doc.set_cell_value(4, 28, 'float', '" + ( saldoCuentaAnt ( 6963 ) + saldoCuentaAnt ( 6965 ) + saldoCuentaAnt ( 6966 ) + saldoCuentaAnt ( 697 ) + saldoCuentaAnt ( 698 ) + saldoCuentaAnt ( 699 ) + ( saldoCuentaAnt ( 7963 )) + ( saldoCuentaAnt ( 7965 )) + ( saldoCuentaAnt ( 7966 )) + ( saldoCuentaAnt ( 797 )) + ( saldoCuentaAnt ( 798 )) + ( saldoCuentaAnt ( 799 ))).toQString ( '.' ) + "')\n\
\
doc.set_cell_value(2, 29, 'string', '       9. Diferencias negativas de cambio')\n\
doc.set_cell_value(3, 29, 'float', '" + saldoCuenta ( 668 ).toQString ( '.' ) + "')\n\
doc.set_cell_value(4, 29, 'float', '" + saldoCuentaAnt ( 668 ).toQString ( '.' ) + "')\n\
\
doc.set_cell_value(2, 30, 'string', '   II.  RESULTADOS FINANCIEROS POSITIVOS')\n\
doc.set_cell_value(2, 31, 'string', '        (B4+B5-A7-A8-A9)')\n\
doc.set_cell_value(3, 30, 'formula', '=if((Haber.C19+Haber.C24-C23-C28-C29)>0;Haber.C19+Haber.C24-C23-C28-C29;T(0))')\n\
doc.set_cell_value(4, 30, 'formula', '=if((Haber.D19+Haber.D24-D23-D28-D29)>0;Haber.D19+Haber.D24-D23-D28-D29;T(0))')\n\
\
doc.set_cell_value(2, 33, 'string', '   III. RESULTADOS POSITIVOS DE LAS ACTIVIDADES ORDINARIAS')\n\
doc.set_cell_value(2, 34, 'string', '        (AI+AII-BI-BII)')\n\
doc.set_cell_value(3, 33, 'formula', '=if((C20+C30-Haber.C16-Haber.C26)>0;C20+C30-Haber.C16-Haber.C26;T(0))')\n\
doc.set_cell_value(4, 33, 'formula', '=if((D20+D30-Haber.D16-Haber.D26)>0;D20+D30-Haber.D16-Haber.D26;T(0))')\n\
\
doc.set_cell_value(2, 36, 'string', '      10. Variación de las provisiones de inmobilizado inmaterial, material y cartera de control')\n\
doc.set_cell_value(3, 36, 'float', '" + ( saldoCuenta ( 691 ) + saldoCuenta ( 692 ) + saldoCuenta ( 6960 ) + saldoCuenta ( 6961 ) + ( saldoCuenta ( 791 )) + ( saldoCuenta ( 792 )) + ( saldoCuenta ( 7960 )) + ( saldoCuenta ( 7961 )) ).toQString ( '.' ) + "')\n\
doc.set_cell_value(4, 36, 'float', '" + ( saldoCuentaAnt ( 691 ) + saldoCuentaAnt ( 692 ) + saldoCuentaAnt ( 6960 ) + saldoCuentaAnt ( 6961 ) + ( saldoCuentaAnt ( 791 )) + ( saldoCuentaAnt ( 792 )) + ( saldoCuentaAnt ( 7960 )) + ( saldoCuentaAnt ( 7961 )) ).toQString ( '.' ) + "')\n\
\
doc.set_cell_value(2, 37, 'string', '      11. Pérdidas procedentes del inmovilizado inmaterial, material y cartera de control')\n\
doc.set_cell_value(3, 37, 'float', '" + ( saldoCuenta ( 670 ) + saldoCuenta ( 671 ) + saldoCuenta ( 672 ) + saldoCuenta ( 673 )).toQString ( '.' ) + "')\n\
doc.set_cell_value(4, 37, 'float', '" + ( saldoCuentaAnt ( 670 ) + saldoCuentaAnt ( 671 ) + saldoCuentaAnt ( 672 ) + saldoCuentaAnt ( 673 )).toQString ( '.' ) + "')\n\
\
doc.set_cell_value(2, 38, 'string', '      12. Pérdidas por operaciones con obligaciones propias')\n\
doc.set_cell_value(3, 38, 'float', '" + saldoCuenta ( 674 ).toQString ( '.' ) + "')\n\
doc.set_cell_value(4, 38, 'float', '" + saldoCuentaAnt ( 674 ).toQString ( '.' ) + "')\n\
\
doc.set_cell_value(2, 39, 'string', '      13. Gastos extraordinarios')\n\
doc.set_cell_value(3, 39, 'float', '" + saldoCuenta ( 678 ).toQString ( '.' ) + "')\n\
doc.set_cell_value(4, 39, 'float', '" + saldoCuentaAnt ( 678 ).toQString ( '.' ) + "')\n\
\
doc.set_cell_value(2, 40, 'string', '      14. Gastos y pérdidas de otros ejercicios')\n\
doc.set_cell_value(3, 40, 'float', '" + saldoCuenta ( 679 ).toQString ( '.' ) + "')\n\
doc.set_cell_value(4, 40, 'float', '" + saldoCuentaAnt ( 679 ).toQString ( '.' ) + "')\n\
\
doc.set_cell_value(2, 42, 'string', '   IV.  RESULTADOS EXTRAORDINARIOS POSITIVOS')\n\
doc.set_cell_value(2, 43, 'string', '        (B6+B7+B8+B9+B10-A10-A11-A12-A13-A14)')\n\
doc.set_cell_value(3, 42, 'formula', '=if((Haber.C32+Haber.C33+Haber.C34+Haber.C35+Haber.C36-C37-C38-C39-C40)>0;Haber.C32+Haber.C33+Haber.C34+Haber.C35+Haber.C36-C37-C38-C39-C40;T(0))')\n\
doc.set_cell_value(4, 42, 'formula', '=if((Haber.D32+Haber.D33+Haber.D34+Haber.D35+Haber.D36-D37-D38-D39-D40)>0;Haber.D32+Haber.D33+Haber.D34+Haber.D35+Haber.D36-D37-D38-D39-D40;T(0))')\n\
\
doc.set_cell_value(2, 45, 'string', '   V.   RESULTADOS POSITIVOS ANTES DE IMPUESTOS')\n\
doc.set_cell_value(2, 46, 'string', '        (AIII+AIV-BIII-BIV)')\n\
doc.set_cell_value(3, 45, 'formula', '=if((C33+C42-Haber.C29-Haber.C38)>0;C33+C42-Haber.C29-Haber.C38;T(0))')\n\
doc.set_cell_value(4, 45, 'formula', '=if((D33+D42-Haber.D29-Haber.D38)>0;D33+D42-Haber.D29-Haber.D38;T(0))')\n\
\
doc.set_cell_value(2, 48, 'string', '      15. Impuesto sobre Sociedades')\n\
doc.set_cell_value(3, 48, 'float', '" + ( saldoCuenta ( 630 ) + saldoCuenta ( 633 ) + ( saldoCuenta ( 638 ) ) ).toQString ( '.' ) + "')\n\
doc.set_cell_value(4, 48, 'float', '" + ( saldoCuentaAnt ( 630 ) + saldoCuentaAnt ( 633 ) + ( saldoCuentaAnt ( 638 ) ) ).toQString ( '.' ) + "')\n\
\
doc.set_cell_value(2, 49, 'string', '      16. Otros impuestos')\n\
doc.set_cell_value(3, 49, 'formula', '=0')\n\
doc.set_cell_value(4, 49, 'formula', '=0')\n\
\
doc.set_cell_value(2, 51, 'string', '   VI.  EXCEDENTE POSITIVO DEL EJERCICIO (AHORRO)')\n\
doc.set_cell_value(2, 52, 'string', '        (AV-A15-A16)')\n\
doc.set_cell_value(3, 51, 'formula', '=if((C45-C48-C49)>0;C45-C48-C49;T(0))')\n\
doc.set_cell_value(4, 51, 'formula', '=if((D45-D48-D49)>0;D45-D48-D49;T(0))')\n\
\
\
\
\
\
doc.new_sheet(\"Haber\")\n\n\
doc.set_column_property(1, 'width', '0.5in')\n\
doc.set_column_property(2, 'width', '5in')\n\
doc.set_cell_property('bold', True)\n\
doc.set_cell_value(2, 2, 'string', 'CUENTA DE RESULTADOS ABREVIADA')\n\
doc.set_cell_value(2, 5, 'string', 'HABER')\n\
doc.set_cell_property('bold', False)\n\
\
doc.set_cell_property('bold', True)\n\
doc.set_cell_value(2, 7, 'string', 'B) INGRESOS')\n\
doc.set_cell_property('bold', False)\n\
\
doc.set_cell_value(2, 8, 'string', '       1. Ingresos de la entidad por la actividad propia')\n\
doc.set_cell_value(3, 8, 'formula', '=SUM(C9:C12)')\n\
doc.set_cell_value(4, 8, 'formula', '=SUM(D9:D12)')\n\
\
doc.set_cell_value(2, 9, 'string', '            a) Quotas de usuarios y afiliados')\n\
doc.set_cell_value(3, 9, 'float', '" + (- ( saldoCuenta ( 720 ) + saldoCuenta ( 721 )) ).toQString ( '.' ) + "')\n\
doc.set_cell_value(4, 9, 'float', '" + (- ( saldoCuentaAnt ( 720 ) + saldoCuentaAnt ( 721 )) ).toQString ( '.' ) + "')\n\
\
doc.set_cell_value(2, 10, 'string', '            b) Ingresos de pomociones, patrocinadores y colaboraciones')\n\
doc.set_cell_value(3, 10, 'float', '" + (- ( saldoCuenta ( 722 ) + saldoCuenta ( 723 ))).toQString ( '.' ) + "')\n\
doc.set_cell_value(4, 10, 'float', '" + (- ( saldoCuentaAnt ( 722 ) + saldoCuentaAnt ( 723 ))).toQString ( '.' ) + "')\n\
\
doc.set_cell_value(2, 11, 'string', '            c) Subvenciones, donaciones y legados imputados a resultados del ejercicio')\n\
doc.set_cell_value(3, 11, 'float', '" + (- ( saldoCuenta ( 725 ) + saldoCuenta ( 726 ))).toQString ( '.' ) + "')\n\
doc.set_cell_value(4, 11, 'float', '" + (- ( saldoCuentaAnt ( 725 ) + saldoCuentaAnt ( 726 ))).toQString ( '.' ) + "')\n\
\
doc.set_cell_value(2, 12, 'string', '            d) Reintegros de subvenciones donaciones y legados')\n\
doc.set_cell_value(3, 12, 'float', '" + (- ( saldoCuenta ( 658 ))).toQString ( '.' ) + "')\n\
doc.set_cell_value(4, 12, 'float', '" + (- ( saldoCuentaAnt ( 658 ))).toQString ( '.' ) + "')\n\
\
doc.set_cell_value(2, 13, 'string', '       2. Ventas y otros ingresos ordinarios de la actividad mercantil')\n\
doc.set_cell_value(3, 13, 'float', '" + (- saldoCuenta ( 70 ) ).toQString ( '.' ) + "')\n\
doc.set_cell_value(4, 13, 'float', '" +(- saldoCuentaAnt ( 70 ) ).toQString ( '.' ) + "')\n\
\
doc.set_cell_value(2, 14, 'string', '       3. Otros ingresos')\n\
doc.set_cell_value(3, 14, 'float', '" + (- ( saldoCuenta ( 73 ) + saldoCuenta ( 74 ) + saldoCuenta ( 75 ) + saldoCuenta ( 790 ))).toQString ( '.' ) + "')\n\
doc.set_cell_value(4, 14, 'float', '" + (- ( saldoCuentaAnt ( 73 ) + saldoCuentaAnt ( 74 ) + saldoCuentaAnt ( 75 ) + saldoCuentaAnt ( 790 ))).toQString ( '.' ) + "')\n\
\
doc.set_cell_value(2, 16, 'string', '   I.   RESULTADOS NEGATIVOS DE EXPLOTACIÓN')\n\
doc.set_cell_value(2, 17, 'string', '        (A1+A2+A3+A4+A5+A6-B1-B2-B3)')\n\
doc.set_cell_value(3, 16, 'formula', '=if((Debe.C8+Debe.C12+Debe.C13+Debe.C16+Debe.C17+Debe.C18-C8-C13-C14)>0;Debe.C8+Debe.C12+Debe.C13+Debe.C16+Debe.C17+Debe.C18-C8-C13-C14;T(0))')\n\
doc.set_cell_value(4, 16, 'formula', '=if((Debe.D8+Debe.D12+Debe.D13+Debe.D16+Debe.D17+Debe.D18-D8-D13-D14)>0;Debe.D8+Debe.D12+Debe.D13+Debe.D16+Debe.D17+Debe.D18-D8-D13-D14;T(0))')\n\
\
doc.set_cell_value(2, 19, 'string', '       4. Ingresos financieros')\n\
doc.set_cell_value(3, 19, 'formula', '=SUM(C20:C23)')\n\
doc.set_cell_value(4, 19, 'formula', '=SUM(D20:D23)')\n\
\
doc.set_cell_value(2, 20, 'string', '            a) En entidades del grupo')\n\
doc.set_cell_value(3, 20, 'float', '" + (- ( saldoCuenta ( 7600 ) + saldoCuenta ( 7610 ) + saldoCuenta ( 7620 ) + saldoCuenta ( 7630 ) + saldoCuenta ( 7650 ))).toQString ( '.' ) + "')\n\
doc.set_cell_value(4, 20, 'float', '" + (- ( saldoCuentaAnt ( 7600 ) + saldoCuentaAnt ( 7610 ) + saldoCuentaAnt ( 7620 ) + saldoCuentaAnt ( 7630 ) + saldoCuentaAnt ( 7650 ))).toQString ( '.' ) + "')\n\
\
doc.set_cell_value(2, 21, 'string', '            b) En entidades asociadas')\n\
doc.set_cell_value(3, 21, 'float', '" + (- ( saldoCuenta ( 7601 ) + saldoCuenta ( 7611 ) + saldoCuenta ( 7621 ) + saldoCuenta ( 7631 ) + saldoCuenta ( 7651 ))).toQString ( '.' ) + "')\n\
doc.set_cell_value(4, 21, 'float', '" + (- ( saldoCuentaAnt ( 7601 ) + saldoCuentaAnt ( 7611 ) + saldoCuentaAnt ( 7621 ) + saldoCuentaAnt ( 7631 ) + saldoCuentaAnt ( 7651 ))).toQString ( '.' ) + "')\n\
\
doc.set_cell_value(2, 22, 'string', '            c) Otros')\n\
doc.set_cell_value(3, 22, 'float', '" + (- ( saldoCuenta ( 7603 ) + saldoCuenta ( 7613 ) + saldoCuenta ( 7623 ) + saldoCuenta ( 7633 ) + saldoCuenta ( 7653 ) + saldoCuenta ( 769 ))).toQString ( '.' ) + "')\n\
doc.set_cell_value(4, 22, 'float', '" + (- ( saldoCuentaAnt ( 7603 ) + saldoCuentaAnt ( 7613 ) + saldoCuentaAnt ( 7623 ) + saldoCuentaAnt ( 7633 ) + saldoCuentaAnt ( 7653 ) + saldoCuentaAnt ( 769 ))).toQString ( '.' ) + "')\n\
\
doc.set_cell_value(2, 23, 'string', '            d) Beneficios en inversiones financieras')\n\
doc.set_cell_value(3, 23, 'float', '" + (- saldoCuenta ( 766 )).toQString ( '.' ) + "')\n\
doc.set_cell_value(4, 23, 'float', '" + (- saldoCuentaAnt ( 766 )).toQString ( '.' ) + "')\n\
\
doc.set_cell_value(2, 24, 'string', '       5. Diferencias positivas de cambio')\n\
doc.set_cell_value(3, 24, 'float', '" + (- saldoCuenta ( 768 )).toQString ( '.' ) + "')\n\
doc.set_cell_value(4, 24, 'float', '" + (- saldoCuentaAnt ( 768 )).toQString ( '.' ) + "')\n\
\
doc.set_cell_value(2, 26, 'string', '   II.  RESULTADOS FINANCIEROS NEGATIVOS')\n\
doc.set_cell_value(2, 27, 'string', '        (A7+A8+A9-B4-B5)')\n\
doc.set_cell_value(3, 26, 'formula', '=if((Debe.C23+Debe.C28+Debe.C29-C19-C24)>0;Debe.C23+Debe.C28+Debe.C29-C19-C24;T(0))')\n\
doc.set_cell_value(4, 26, 'formula', '=if((Debe.D23+Debe.D28+Debe.D29-D19-D24)>0;Debe.D23+Debe.D28+Debe.D29-D19-D24;T(0))')\n\
\
doc.set_cell_value(2, 29, 'string', '   III. RESULTADOS NEGATIVOS DE LAS ACTIVIDADES ORDINARIAS')\n\
doc.set_cell_value(2, 30, 'string', '        (BI+BII-AI-AII)')\n\
doc.set_cell_value(3, 29, 'formula', '=if((C16+C26-Debe.C20-Debe.C30)>0;C16+C26-Debe.C20-Debe.C30;T(0))')\n\
doc.set_cell_value(4, 29, 'formula', '=if((D16+D26-Debe.D20-Debe.D30)>0;D16+D26-Debe.D20-Debe.D30;T(0))')\n\
\
doc.set_cell_value(2, 32, 'string', '       6. Beneficios en enajenación de inmovilizado inmaterial, material y cartera de control')\n\
doc.set_cell_value(3, 32, 'float', '" + (- ( saldoCuenta ( 770 ) + saldoCuenta ( 771 ) + saldoCuenta ( 772 ) + saldoCuenta ( 773 )) ).toQString ( '.' ) + "')\n\
doc.set_cell_value(4, 32, 'float', '" + (- ( saldoCuentaAnt ( 770 ) + saldoCuentaAnt ( 771 ) + saldoCuentaAnt ( 772 ) + saldoCuentaAnt ( 773 ) )).toQString ( '.' ) + "')\n\
\
doc.set_cell_value(2, 33, 'string', '       7 Beneficios por operaciones con obligaciones propias')\n\
doc.set_cell_value(3, 33, 'float', '" + (- saldoCuenta ( 774 )).toQString ( '.' ) + "')\n\
doc.set_cell_value(4, 33, 'float', '" + (- saldoCuentaAnt ( 774 )).toQString ( '.' ) + "')\n\
\
doc.set_cell_value(2, 34, 'string', '       8 Subvenciones, donaciones y legados de capital y otros afectos a la actividad mercantil traspasados  al resultado del ejercicio')\n\
doc.set_cell_value(3, 34, 'float', '" + (- saldoCuenta ( 775 )).toQString ( '.' ) + "')\n\
doc.set_cell_value(4, 34, 'float', '" + (- saldoCuentaAnt ( 775 )).toQString ( '.' ) + "')\n\
\
doc.set_cell_value(2, 35, 'string', '       9. Ingresos extraordinarios')\n\
doc.set_cell_value(3, 35, 'float', '" + (- saldoCuenta ( 778 )).toQString ( '.' ) + "')\n\
doc.set_cell_value(4, 35, 'float', '" + (- saldoCuentaAnt ( 778 )).toQString ( '.' ) + "')\n\
\
doc.set_cell_value(2, 36, 'string', '      10. Ingresos y beneficios de otros ejercicios')\n\
doc.set_cell_value(3, 36, 'float', '" + (- saldoCuenta ( 779 )).toQString ( '.' ) + "')\n\
doc.set_cell_value(4, 36, 'float', '" + (- saldoCuentaAnt ( 779 )).toQString ( '.' ) + "')\n\
\
doc.set_cell_value(2, 38, 'string', '   IV. RESULTADOS EXTRAORDINARIOS NEGATIVOS')\n\
doc.set_cell_value(2, 39, 'string', '       (A10+A11+A12+A13+A14-B6-B7-B8-B9-B10)')\n\
doc.set_cell_value(3, 38, 'formula', '=if((Debe.C36+Debe.C37+Debe.C38+Debe.C39+Debe.C40-C32-C33-C34-C35-C36)>0;Debe.C36+Debe.C37+Debe.C38+Debe.C39+Debe.C40-C32-C33-C34-C35-C36;T(0))')\n\
doc.set_cell_value(4, 38, 'formula', '=if((Debe.D36+Debe.D37+Debe.D38+Debe.D39+Debe.D40-D32-D33-D34-D35-D36)>0;Debe.D36+Debe.D37+Debe.D38+Debe.D39+Debe.D40-D32-D33-D34-D35-D36;T(0))')\n\
\
doc.set_cell_value(2, 41, 'string', '   V.  RESULTADOS NEGATIVOS ANTES DE IMPUESTOS')\n\
doc.set_cell_value(2, 42, 'string', '       (BIII+BIV-AIII-AIV)')\n\
doc.set_cell_value(3, 41, 'formula', '=if((C29+C38-Debe.C33-Debe.C42)>0;C29+C38-Debe.C33-Debe.C42;T(0))')\n\
doc.set_cell_value(4, 41, 'formula', '=if((D29+D38-Debe.D33-Debe.D42)>0;D29+D38-Debe.D33-Debe.D42;T(0))')\n\
\
doc.set_cell_value(2, 44, 'string', '   VI. EXCEDENTE NEGATIVO DEL EJERCICIO (DESAHORRO)')\n\
doc.set_cell_value(2, 45, 'string', '       (BV+A15+A16)')\n\
doc.set_cell_value(3, 44, 'formula', '=if((C41+Debe.C48+Debe.C49)>0;C41+Debe.C48+Debe.C49;T(0))')\n\
doc.set_cell_value(4, 44, 'formula', '=if((D41+Debe.D48+Debe.D49)>0;D41+Debe.D48+Debe.D49;T(0))')\n\
\
\
\
\
\
# Guardamos el documento\n\
doc.save(\"canualesods.ods\")\n");
}


///
/**
**/
QString pluginReportODS::cuentaAnualCAPGC07()
{
	return QString::QString("\
#!/usr/bin/python\n\
# -*- coding: utf8 -*-\n\
\n\
import ooolib\n\
\n\
# Crea el documento\n\
doc = ooolib.Calc(\"Activo\")\n\
\n\
doc.set_column_property(1, 'width', '0.5in')\n\
doc.set_column_property(2, 'width', '5in')\n\
doc.set_cell_property('bold', True)\n\
doc.set_cell_value(2, 2, 'string', 'BALANCE DE SITUACIÓN')\n\
doc.set_cell_value(2, 3, 'string', '" + QDateTime::currentDateTime().toString() + "')\n\
doc.set_cell_value(2, 5, 'string', 'ACTIVO')\n\
doc.set_cell_value(3, 5, 'string', 'Ejercicio N')\n\
doc.set_cell_value(4, 5, 'string', 'Ejercicio N-1')\n\
doc.set_cell_property('bold', False)\n\
\
doc.set_cell_property('bold', True)\n\
doc.set_cell_value(2, 7, 'string', 'A) Accionistas (socios) por desembolsos no exigidos')\n\
doc.set_cell_value(3, 7, 'float', '" + ( saldoCuenta ( 190 ) + saldoCuenta ( 191 ) + saldoCuenta ( 192 ) + saldoCuenta ( 193 ) + saldoCuenta ( 194 ) + saldoCuenta ( 195 ) + saldoCuenta ( 196 ) ).toQString ( '.' ) + "')\n\
doc.set_cell_value(4, 7, 'float', '" + ( saldoCuentaAnt ( 190 ) + saldoCuentaAnt ( 191 ) + saldoCuentaAnt ( 192 ) + saldoCuentaAnt ( 193 ) + saldoCuentaAnt ( 194 ) + saldoCuentaAnt ( 195 ) + saldoCuentaAnt ( 196 ) ).toQString ( '.' ) + "')\n\
doc.set_cell_property('bold', False)\n\
\
doc.set_cell_property('bold', True)\n\
doc.set_cell_value(2, 9, 'string', 'B) Inmovilizado')\n\
doc.set_cell_value(3, 9, 'formula', '=C10+C11+C20+C28+C37')\n\
doc.set_cell_value(4, 9, 'formula', '=D10+D11+D20+D28+D37')\n\
doc.set_cell_property('bold', False)\n\
\
doc.set_cell_value(2, 10, 'string', '   I.   Gastos de establecimiento')\n\
doc.set_cell_value(3, 10, 'float', '" + saldoCuenta ( 20 ).toQString ( '.' ) + "')\n\
doc.set_cell_value(4, 10, 'float', '" + saldoCuentaAnt ( 20 ).toQString ( '.' ) + "')\n\
\
doc.set_cell_value(2, 11, 'string', '   II.  Inmovilizaciones inmateriales')\n\
doc.set_cell_value(3, 11, 'formula', '=SUM(C12:C19)')\n\
doc.set_cell_value(4, 11, 'formula', '=SUM(D12:D19)')\n\
\
doc.set_cell_value(2, 12, 'string', '       1. Gastos de investigación y desarrollo')\n\
doc.set_cell_value(3, 12, 'float', '" + saldoCuenta ( 210 ).toQString ( '.' ) + "')\n\
doc.set_cell_value(4, 12, 'float', '" + saldoCuentaAnt ( 210 ).toQString ( '.' ) + "')\n\
\
doc.set_cell_value(2, 13, 'string', '       2. Concesiones, patentes, licencias, marcas y similares')\n\
doc.set_cell_value(3, 13, 'float', '" + ( saldoCuenta ( 211 ) + saldoCuenta ( 212 ) ).toQString ( '.' ) + "')\n\
doc.set_cell_value(4, 13, 'float', '" + ( saldoCuentaAnt ( 211 ) + saldoCuentaAnt ( 212 ) ).toQString ( '.' ) + "')\n\
\
doc.set_cell_value(2, 14, 'string', '       3. Fondo de comercio')\n\
doc.set_cell_value(3, 14, 'float', '" + saldoCuenta ( 213 ).toQString ( '.' ) + "')\n\
doc.set_cell_value(4, 14, 'float', '" + saldoCuentaAnt ( 213 ).toQString ( '.' ) + "')\n\
\
doc.set_cell_value(2, 15, 'string', '       4. Derechos de traspaso')\n\
doc.set_cell_value(3, 15, 'float', '" + saldoCuenta ( 214 ).toQString ( '.' ) + "')\n\
doc.set_cell_value(4, 15, 'float', '" + saldoCuentaAnt ( 214 ).toQString ( '.' ) + "')\n\
\
doc.set_cell_value(2, 16, 'string', '       5. Aplicaciones informáticas')\n\
doc.set_cell_value(3, 16, 'float', '" + saldoCuenta ( 215 ).toQString ( '.' ) + "')\n\
doc.set_cell_value(4, 16, 'float', '" + saldoCuentaAnt ( 215 ).toQString ( '.' ) + "')\n\
\
doc.set_cell_value(2, 17, 'string', '       6. Anticipos')\n\
doc.set_cell_value(3, 17, 'float', '" + saldoCuenta ( 219 ).toQString ( '.' ) + "')\n\
doc.set_cell_value(4, 17, 'float', '" + saldoCuentaAnt ( 219 ).toQString ( '.' ) + "')\n\
\
doc.set_cell_value(2, 18, 'string', '       7. Provisiones')\n\
doc.set_cell_value(3, 18, 'float', '" + ( -saldoCuenta ( 291 ) ).toQString ( '.' ) + "')\n\
doc.set_cell_value(4, 18, 'float', '" + ( -saldoCuentaAnt ( 291 ) ).toQString ( '.' ) + "')\n\
\
doc.set_cell_value(2, 19, 'string', '       8. Amortizaciones')\n\
doc.set_cell_value(3, 19, 'float', '" + ( -saldoCuenta ( 281 ) ).toQString ( '.' ) + "')\n\
doc.set_cell_value(4, 19, 'float', '" + ( -saldoCuentaAnt ( 281 ) ).toQString ( '.' ) + "')\n\
\
doc.set_cell_value(2, 20, 'string', '   III. Inmovilizaciones materiales')\n\
doc.set_cell_value(3, 20, 'formula', '=SUM(C21:C27)')\n\
doc.set_cell_value(4, 20, 'formula', '=SUM(D21:D27)')\n\
\
doc.set_cell_value(2, 21, 'string', '       1. Terrenos y construcciones')\n\
doc.set_cell_value(3, 21, 'float', '" + ( saldoCuenta ( 220 ) + saldoCuenta ( 221 ) ).toQString ( '.' ) + "')\n\
doc.set_cell_value(4, 21, 'float', '" + ( saldoCuentaAnt ( 220 ) + saldoCuentaAnt ( 221 ) ).toQString ( '.' ) + "')\n\
\
doc.set_cell_value(2, 22, 'string', '       2. Instalaciones técnicas y maquinaria')\n\
doc.set_cell_value(3, 22, 'float', '" + ( saldoCuenta ( 222 ) + saldoCuenta ( 223 ) ).toQString ( '.' ) + "')\n\
doc.set_cell_value(4, 22, 'float', '" + ( saldoCuentaAnt ( 222 ) + saldoCuentaAnt ( 223 ) ).toQString ( '.' ) + "')\n\
\
doc.set_cell_value(2, 23, 'string', '       3. Otras instalaciones, utillaje y mobiliario')\n\
doc.set_cell_value(3, 23, 'float', '" + ( saldoCuenta ( 224 ) + saldoCuenta ( 225 ) + saldoCuenta ( 226 ) ).toQString ( '.' ) + "')\n\
doc.set_cell_value(4, 23, 'float', '" + ( saldoCuentaAnt ( 224 ) + saldoCuentaAnt ( 225 ) + saldoCuentaAnt ( 226 ) ).toQString ( '.' ) + "')\n\
\
doc.set_cell_value(2, 24, 'string', '       4. Anticipos e inmovilizaciones materiales en curso')\n\
doc.set_cell_value(3, 24, 'float', '" + saldoCuenta ( 23 ).toQString ( '.' ) + "')\n\
doc.set_cell_value(4, 24, 'float', '" + saldoCuentaAnt ( 23 ).toQString ( '.' ) + "')\n\
\
doc.set_cell_value(2, 25, 'string', '       5. Otro inmobilizado')\n\
doc.set_cell_value(3, 25, 'float', '" + ( saldoCuenta ( 227 ) + saldoCuenta ( 228 ) + saldoCuenta ( 229 ) )  .toQString ( '.' ) + "')\n\
doc.set_cell_value(4, 25, 'float', '" + ( saldoCuentaAnt ( 227 ) + saldoCuentaAnt ( 228 ) + saldoCuentaAnt ( 229 ) )  .toQString ( '.' ) + "')\n\
\
doc.set_cell_value(2, 26, 'string', '       6. Provisiones')\n\
doc.set_cell_value(3, 26, 'float', '" + ( -saldoCuenta ( 292 ) ).toQString ( '.' ) + "')\n\
doc.set_cell_value(4, 26, 'float', '" + ( -saldoCuentaAnt ( 292 ) ).toQString ( '.' ) + "')\n\
\
doc.set_cell_value(2, 27, 'string', '       7. Amortizaciones')\n\
doc.set_cell_value(3, 27, 'float', '" + ( -saldoCuenta ( 282 ) ).toQString ( '.' ) + "')\n\
doc.set_cell_value(4, 27, 'float', '" + ( -saldoCuentaAnt ( 282 ) ).toQString ( '.' ) + "')\n\
\
doc.set_cell_value(2, 28, 'string', '   IV.  Inmovilizaciones financieras')\n\
doc.set_cell_value(3, 28, 'formula', '=SUM(C29:C36)')\n\
doc.set_cell_value(4, 28, 'formula', '=SUM(D29:D36)')\n\
\
doc.set_cell_value(2, 29, 'string', '       1. Participaciones en empresas del grupo')\n\
doc.set_cell_value(3, 29, 'float', '" + saldoCuenta ( 240 ).toQString ( '.' ) + "')\n\
doc.set_cell_value(4, 29, 'float', '" + saldoCuentaAnt ( 240 ).toQString ( '.' ) + "')\n\
\
doc.set_cell_value(2, 30, 'string', '       2. Créditos a empresas del grupo')\n\
doc.set_cell_value(3, 30, 'float', '" + ( saldoCuenta ( 242 ) + saldoCuenta ( 244 ) + saldoCuenta ( 246 ) ).toQString ( '.' ) + "')\n\
doc.set_cell_value(4, 30, 'float', '" + ( saldoCuentaAnt ( 242 ) + saldoCuentaAnt ( 244 ) + saldoCuentaAnt ( 246 ) ).toQString ( '.' ) + "')\n\
\
doc.set_cell_value(2, 31, 'string', '       3. Participaciones en empresas asociadas')\n\
doc.set_cell_value(3, 31, 'float', '" + saldoCuenta ( 241 ).toQString ( '.' ) + "')\n\
doc.set_cell_value(4, 31, 'float', '" + saldoCuentaAnt ( 241 ).toQString ( '.' ) + "')\n\
\
doc.set_cell_value(2, 32, 'string', '       4. Créditos a empresas asociadas')\n\
doc.set_cell_value(3, 32, 'float', '" + ( saldoCuenta ( 243 ) + saldoCuenta ( 245 ) + saldoCuenta ( 247 ) ).toQString ( '.' ) + "')\n\
doc.set_cell_value(4, 32, 'float', '" + ( saldoCuentaAnt ( 243 ) + saldoCuentaAnt ( 245 ) + saldoCuentaAnt ( 247 ) ).toQString ( '.' ) + "')\n\
\
doc.set_cell_value(2, 33, 'string', '       5. Cartera de valores a largo plazo')\n\
doc.set_cell_value(3, 33, 'float', '" + ( saldoCuenta ( 250 ) + saldoCuenta ( 251 ) + saldoCuenta ( 256 ) ).toQString ( '.' ) + "')\n\
doc.set_cell_value(4, 33, 'float', '" + ( saldoCuentaAnt ( 250 ) + saldoCuentaAnt ( 251 ) + saldoCuentaAnt ( 256 ) ).toQString ( '.' ) + "')\n\
\
doc.set_cell_value(2, 34, 'string', '       6. Otros créditos')\n\
doc.set_cell_value(3, 34, 'float', '" + ( saldoCuenta ( 252 ) + saldoCuenta ( 253 ) + saldoCuenta ( 254 ) + saldoCuenta ( 257 ) + saldoCuenta ( 258 ) ).toQString ( '.' ) + "')\n\
doc.set_cell_value(4, 34, 'float', '" + ( saldoCuentaAnt ( 252 ) + saldoCuentaAnt ( 253 ) + saldoCuentaAnt ( 254 ) + saldoCuentaAnt ( 257 ) + saldoCuentaAnt ( 258 ) ).toQString ( '.' ) + "')\n\
\
doc.set_cell_value(2, 35, 'string', '       7. Depósitos y fianzas constituidos a largo plazo')\n\
doc.set_cell_value(3, 35, 'float', '" + ( saldoCuenta ( 260 ) + saldoCuenta ( 265 ) ).toQString ( '.' ) + "')\n\
doc.set_cell_value(4, 35, 'float', '" + ( saldoCuentaAnt ( 260 ) + saldoCuentaAnt ( 265 ) ).toQString ( '.' ) + "')\n\
\
doc.set_cell_value(2, 36, 'string', '       8. Provisiones')\n\
doc.set_cell_value(3, 36, 'float', '" + ( ( -saldoCuenta ( 293 ) ) + ( -saldoCuenta ( 294 ) ) + ( -saldoCuenta ( 295 ) ) + ( -saldoCuenta ( 296 ) ) + ( -saldoCuenta ( 297 ) ) + ( -saldoCuenta ( 298 ) ) ).toQString ( '.' ) + "')\n\
doc.set_cell_value(4, 36, 'float', '" + ( ( -saldoCuentaAnt ( 293 ) ) + ( -saldoCuentaAnt ( 294 ) ) + ( -saldoCuentaAnt ( 295 ) ) + ( -saldoCuentaAnt ( 296 ) ) + ( -saldoCuentaAnt ( 297 ) ) + ( -saldoCuentaAnt ( 298 ) ) ).toQString ( '.' ) + "')\n\
\
doc.set_cell_value(2, 37, 'string', '   V.   Acciones propias')\n\
doc.set_cell_value(3, 37, 'float', '" + saldoCuenta ( 198 ).toQString ( '.' ) + "')\n\
doc.set_cell_value(4, 37, 'float', '" + saldoCuentaAnt ( 198 ).toQString ( '.' ) + "')\n\
\
\
doc.set_cell_property('bold', True)\n\
doc.set_cell_value(2, 39, 'string', 'C) Gastos a distribuir en varios ejercicios')\n\
doc.set_cell_value(3, 39, 'float', '" + saldoCuenta ( 27 ).toQString ( '.' ) + "')\n\
doc.set_cell_value(4, 39, 'float', '" + saldoCuentaAnt ( 27 ).toQString ( '.' ) + "')\n\
doc.set_cell_property('bold', False)\n\
\
\
doc.set_cell_property('bold', True)\n\
doc.set_cell_value(2, 41, 'string', 'D) Activo circulante')\n\
doc.set_cell_value(3, 41, 'formula', '=C42+C43+C51+C59+C68+C69+C70')\n\
doc.set_cell_value(4, 41, 'formula', '=D42+D43+D51+D59+D68+D69+D70')\n\
doc.set_cell_property('bold', False)\n\
\
doc.set_cell_value(2, 42, 'string', '   I.   Accionistas por desembolsos exigidos')\n\
doc.set_cell_value(3, 42, 'float', '" + saldoCuenta ( 558 ).toQString ( '.' ) + "')\n\
doc.set_cell_value(4, 42, 'float', '" + saldoCuentaAnt ( 558 ).toQString ( '.' ) + "')\n\
\
doc.set_cell_value(2, 43, 'string', '   II.  Existencias')\n\
doc.set_cell_value(3, 43, 'formula', '=SUM(C44:C50)')\n\
doc.set_cell_value(4, 43, 'formula', '=SUM(D44:D50)')\n\
\
doc.set_cell_value(2, 44, 'string', '       1. Comerciales')\n\
doc.set_cell_value(3, 44, 'float', '" + saldoCuenta ( 30 ).toQString ( '.' ) + "')\n\
doc.set_cell_value(4, 44, 'float', '" + saldoCuentaAnt ( 30 ).toQString ( '.' ) + "')\n\
\
doc.set_cell_value(2, 45, 'string', '       2. Materias primas y otros aprovisionamientos')\n\
doc.set_cell_value(3, 45, 'float', '" + ( saldoCuenta ( 31 ) + saldoCuenta ( 32 ) ).toQString ( '.' ) + "')\n\
doc.set_cell_value(4, 45, 'float', '" + ( saldoCuentaAnt ( 31 ) + saldoCuentaAnt ( 32 ) ).toQString ( '.' ) + "')\n\
\
doc.set_cell_value(2, 46, 'string', '       3. Productos en curso y semiterminados')\n\
doc.set_cell_value(3, 46, 'float', '" + ( saldoCuenta ( 33 ) + saldoCuenta ( 34 ) ).toQString ( '.' ) + "')\n\
doc.set_cell_value(4, 46, 'float', '" + ( saldoCuentaAnt ( 33 ) + saldoCuentaAnt ( 34 ) ).toQString ( '.' ) + "')\n\
\
doc.set_cell_value(2, 47, 'string', '       4. Productos terminados')\n\
doc.set_cell_value(3, 47, 'float', '" + saldoCuenta ( 35 ).toQString ( '.' ) + "')\n\
doc.set_cell_value(4, 47, 'float', '" + saldoCuentaAnt ( 35 ).toQString ( '.' ) + "')\n\
\
doc.set_cell_value(2, 48, 'string', '       5. Subproductos, residuos y materiales recuperados')\n\
doc.set_cell_value(3, 48, 'float', '" + saldoCuenta ( 36 ).toQString ( '.' ) + "')\n\
doc.set_cell_value(4, 48, 'float', '" + saldoCuentaAnt ( 36 ).toQString ( '.' ) + "')\n\
\
doc.set_cell_value(2, 49, 'string', '       6. Anticipos')\n\
doc.set_cell_value(3, 49, 'float', '" + saldoCuenta ( 407 ).toQString ( '.' ) + "')\n\
doc.set_cell_value(4, 49, 'float', '" + saldoCuentaAnt ( 407 ).toQString ( '.' ) + "')\n\
\
doc.set_cell_value(2, 50, 'string', '       7. Provisiones')\n\
doc.set_cell_value(3, 50, 'float', '" + ( -saldoCuenta ( 39 ) ).toQString ( '.' ) + "')\n\
doc.set_cell_value(4, 50, 'float', '" + ( -saldoCuentaAnt ( 39 ) ).toQString ( '.' ) + "')\n\
\
doc.set_cell_value(2, 51, 'string', '   III. Deudores')\n\
doc.set_cell_value(3, 51, 'formula', '=SUM(C52:C58)')\n\
doc.set_cell_value(4, 51, 'formula', '=SUM(D52:D58)')\n\
\
doc.set_cell_value(2, 52, 'string', '       1. Clientes por ventas y prestaciones de servicios')\n\
doc.set_cell_value(3, 52, 'float', '" + ( saldoCuenta ( 430 ) + saldoCuenta ( 431 ) + saldoCuenta ( 435 ) - saldoCuenta ( 436 ) ).toQString ( '.' ) + "')\n\
doc.set_cell_value(4, 52, 'float', '" + ( saldoCuentaAnt ( 430 ) + saldoCuentaAnt ( 431 ) + saldoCuentaAnt ( 435 ) - saldoCuentaAnt ( 436 ) ).toQString ( '.' ) + "')\n\
\
doc.set_cell_value(2, 53, 'string', '       2. Empresas del grupo, deudores')\n\
doc.set_cell_value(3, 53, 'float', '" + ( saldoCuenta ( 432 ) + saldoCuenta ( 551 ) ).toQString ( '.' ) + "')\n\
doc.set_cell_value(4, 53, 'float', '" + ( saldoCuentaAnt ( 432 ) + saldoCuentaAnt ( 551 ) ).toQString ( '.' ) + "')\n\
\
doc.set_cell_value(2, 54, 'string', '       3. Empresas asociadas, deudores')\n\
doc.set_cell_value(3, 54, 'float', '" + ( saldoCuenta ( 433 ) + saldoCuenta ( 552 ) ).toQString ( '.' ) + "')\n\
doc.set_cell_value(4, 54, 'float', '" + ( saldoCuentaAnt ( 433 ) + saldoCuentaAnt ( 552 ) ).toQString ( '.' ) + "')\n\
\
doc.set_cell_value(2, 55, 'string', '       4. Deudores varios')\n\
doc.set_cell_value(3, 55, 'float', '" + ( saldoCuenta ( 44 ) + saldoCuenta ( 553 ) ).toQString ( '.' ) + "')\n\
doc.set_cell_value(4, 55, 'float', '" + ( saldoCuentaAnt ( 44 ) + saldoCuentaAnt ( 553 ) ).toQString ( '.' ) + "')\n\
\
doc.set_cell_value(2, 56, 'string', '       5. Personal')\n\
doc.set_cell_value(3, 56, 'float', '" + ( saldoCuenta ( 460 ) + saldoCuenta ( 544 ) ).toQString ( '.' ) + "')\n\
doc.set_cell_value(4, 56, 'float', '" + ( saldoCuentaAnt ( 460 ) + saldoCuentaAnt ( 544 ) ).toQString ( '.' ) + "')\n\
\
doc.set_cell_value(2, 57, 'string', '       6. Administraciones Públicas')\n\
doc.set_cell_value(3, 57, 'float', '" + ( saldoCuenta ( 470 ) + saldoCuenta ( 471 ) + saldoCuenta ( 472 ) + saldoCuenta ( 474 ) ).toQString ( '.' ) + "')\n\
doc.set_cell_value(4, 57, 'float', '" + ( saldoCuentaAnt ( 470 ) + saldoCuentaAnt ( 471 ) + saldoCuentaAnt ( 472 ) + saldoCuentaAnt ( 474 ) ).toQString ( '.' ) + "')\n\
\
doc.set_cell_value(2, 58, 'string', '       7. Provisiones')\n\
doc.set_cell_value(3, 58, 'float', '" + ( ( -saldoCuenta ( 490 ) ) + ( -saldoCuenta ( 493 ) ) + ( -saldoCuenta ( 494 ) ) ).toQString ( '.' ) + "')\n\
doc.set_cell_value(4, 58, 'float', '" + ( ( -saldoCuentaAnt ( 490 ) ) + ( -saldoCuentaAnt ( 493 ) ) + ( -saldoCuentaAnt ( 494 ) ) ).toQString ( '.' ) + "')\n\
\
doc.set_cell_value(2, 59, 'string', '   IV.  Inversiones financieras temporales')\n\
doc.set_cell_value(3, 59, 'formula', '=SUM(C60:C67)')\n\
doc.set_cell_value(4, 59, 'formula', '=SUM(D60:D67)')\n\
\
doc.set_cell_value(2, 60, 'string', '       1. Participaciones en empresas del grupo')\n\
doc.set_cell_value(3, 60, 'float', '" + ( saldoCuenta ( 530 ) + ( -saldoCuenta ( 538 ) ) ).toQString ( '.' ) + "')\n\
doc.set_cell_value(4, 60, 'float', '" + ( saldoCuentaAnt ( 530 ) + ( -saldoCuentaAnt ( 538 ) ) ).toQString ( '.' ) + "')\n\
\
doc.set_cell_value(2, 61, 'string', '       2. Créditos a empresas del grupo')\n\
doc.set_cell_value(3, 61, 'float', '" + ( saldoCuenta ( 532 ) + saldoCuenta ( 534 ) + saldoCuenta ( 536 ) ).toQString ( '.' ) + "')\n\
doc.set_cell_value(4, 61, 'float', '" + ( saldoCuentaAnt ( 532 ) + saldoCuentaAnt ( 534 ) + saldoCuentaAnt ( 536 ) ).toQString ( '.' ) + "')\n\
\
doc.set_cell_value(2, 62, 'string', '       3. Participaciones en empresas asociadas')\n\
doc.set_cell_value(3, 62, 'float', '" + ( saldoCuenta ( 531 ) + ( -saldoCuenta ( 539 ) ) ).toQString ( '.' ) + "')\n\
doc.set_cell_value(4, 62, 'float', '" + ( saldoCuentaAnt ( 531 ) + ( -saldoCuentaAnt ( 539 ) ) ).toQString ( '.' ) + "')\n\
\
doc.set_cell_value(2, 63, 'string', '       4. Creéditos a empresas asociadas')\n\
doc.set_cell_value(3, 63, 'float', '" + ( saldoCuenta ( 533 ) + saldoCuenta ( 535 ) + saldoCuenta ( 537 ) ).toQString ( '.' ) + "')\n\
doc.set_cell_value(4, 63, 'float', '" + ( saldoCuentaAnt ( 533 ) + saldoCuentaAnt ( 535 ) + saldoCuentaAnt ( 537 ) ).toQString ( '.' ) + "')\n\
\
doc.set_cell_value(2, 64, 'string', '       5. Cartera de valores a corto plazo')\n\
doc.set_cell_value(3, 64, 'float', '" + ( saldoCuenta ( 540 ) + saldoCuenta ( 541 ) + saldoCuenta ( 546 ) + ( -saldoCuenta ( 549 ) ) ).toQString ( '.' ) + "')\n\
doc.set_cell_value(4, 64, 'float', '" + ( saldoCuentaAnt ( 540 ) + saldoCuentaAnt ( 541 ) + saldoCuentaAnt ( 546 ) + ( -saldoCuentaAnt ( 549 ) ) ).toQString ( '.' ) + "')\n\
\
doc.set_cell_value(2, 65, 'string', '       6. Otros créditos')\n\
doc.set_cell_value(3, 65, 'float', '" + ( saldoCuenta ( 542 ) + saldoCuenta ( 543 ) + saldoCuenta ( 545 ) + saldoCuenta ( 547 ) + saldoCuenta ( 548 ) ).toQString ( '.' ) + "')\n\
doc.set_cell_value(4, 65, 'float', '" + ( saldoCuentaAnt ( 542 ) + saldoCuentaAnt ( 543 ) + saldoCuentaAnt ( 545 ) + saldoCuentaAnt ( 547 ) + saldoCuentaAnt ( 548 ) ).toQString ( '.' ) + "')\n\
\
doc.set_cell_value(2, 66, 'string', '       7. Depósitos y fianzas constituidos a corto plazo')\n\
doc.set_cell_value(3, 66, 'float', '" + ( saldoCuenta ( 565 ) + saldoCuenta ( 566 ) ).toQString ( '.' ) + "')\n\
doc.set_cell_value(4, 66, 'float', '" + ( saldoCuentaAnt ( 565 ) + saldoCuentaAnt ( 566 ) ).toQString ( '.' ) + "')\n\
\
doc.set_cell_value(2, 67, 'string', '       8. Provisiones')\n\
doc.set_cell_value(3, 67, 'float', '" + ( ( -saldoCuenta ( 593 ) ) + ( -saldoCuenta ( 594 ) ) + ( -saldoCuenta ( 595 ) ) + ( -saldoCuenta ( 596 ) ) + ( -saldoCuenta ( 597 ) ) + ( -saldoCuenta ( 598 ) ) ).toQString ( '.' ) + "')\n\
doc.set_cell_value(4, 67, 'float', '" + ( ( -saldoCuentaAnt ( 593 ) ) + ( -saldoCuentaAnt ( 594 ) ) + ( -saldoCuentaAnt ( 595 ) ) + ( -saldoCuentaAnt ( 596 ) ) + ( -saldoCuentaAnt ( 597 ) ) + ( -saldoCuentaAnt ( 598 ) ) ).toQString ( '.' ) + "')\n\
\
doc.set_cell_value(2, 68, 'string', '   V.   Acciones propias a corto plazo')\n\
doc.set_cell_value(3, 68, 'formula', '=0')\n\
doc.set_cell_value(4, 68, 'formula', '=0')\n\
\
doc.set_cell_value(2, 69, 'string', '   VI.  Tesorería')\n\
doc.set_cell_value(3, 69, 'float', '" + saldoCuenta ( 57 ).toQString ( '.' ) + "')\n\
doc.set_cell_value(4, 69, 'float', '" + saldoCuentaAnt ( 57 ).toQString ( '.' ) + "')\n\
\
doc.set_cell_value(2, 70, 'string', '   VII. Ajustes por periodificación')\n\
doc.set_cell_value(3, 70, 'float', '" + ( saldoCuenta ( 480 ) + saldoCuenta ( 580 ) ).toQString ( '.' ) + "')\n\
doc.set_cell_value(4, 70, 'float', '" + ( saldoCuentaAnt ( 480 ) + saldoCuentaAnt ( 580 ) ).toQString ( '.' ) + "')\n\
\
doc.set_cell_property('bold', True)\n\
doc.set_cell_value(2, 72, 'string', 'TOTAL GENERAL (A+B+C+D)')\n\
doc.set_cell_value(3, 72, 'formula', '=C7+C9+C39+C41')\n\
doc.set_cell_value(4, 72, 'formula', '=D7+D9+D39+D41')\n\
doc.set_cell_property('bold', False)\n\
\
\
\
\
\
\
\
doc.new_sheet(\"Pasivo\")\n\n\
doc.set_column_property(1, 'width', '0.5in')\n\
doc.set_column_property(2, 'width', '5in')\n\
doc.set_cell_property('bold', True)\n\
doc.set_cell_value(2, 2, 'string', 'BALANCE DE SITUACIÓN')\n\
doc.set_cell_value(2, 5, 'string', 'PASIVO')\n\
doc.set_cell_value(3, 5, 'string', 'Ejercicio N')\n\
doc.set_cell_value(4, 5, 'string', 'Ejercicio N-1')\n\
doc.set_cell_property('bold', False)\n\
\
doc.set_cell_property('bold', True)\n\
doc.set_cell_value(2, 7, 'string', 'A) Fondos propios')\n\
doc.set_cell_value(3, 7, 'formula', '=C8+C9+C10+C11+C17+C21+C22')\n\
doc.set_cell_value(4, 7, 'formula', '=D8+D9+D10+D11+D17+D21+D22')\n\
doc.set_cell_property('bold', False)\n\
\
doc.set_cell_value(2, 8, 'string', '   I.   Capital suscrito')\n\
doc.set_cell_value(3, 8, 'float', '" + saldoCuenta ( 10 ).toQString ( '.' ) + "')\n\
doc.set_cell_value(4, 8, 'float', '" + saldoCuentaAnt ( 10 ).toQString ( '.' ) + "')\n\
\
doc.set_cell_value(2, 9, 'string', '   II.  Prima de emisión')\n\
doc.set_cell_value(3, 9, 'float', '" + saldoCuenta ( 110 ).toQString ( '.' ) + "')\n\
doc.set_cell_value(4, 9, 'float', '" + saldoCuentaAnt ( 110 ).toQString ( '.' ) + "')\n\
\
doc.set_cell_value(2, 10, 'string', '   III. Reserva de revalorización')\n\
doc.set_cell_value(3, 10, 'float', '" + saldoCuenta ( 111 ).toQString ( '.' ) + "')\n\
doc.set_cell_value(4, 10, 'float', '" + saldoCuentaAnt ( 111 ).toQString ( '.' ) + "')\n\
\
doc.set_cell_value(2, 11, 'string', '   IV.  Reservas')\n\
doc.set_cell_value(3, 11, 'formula', '=SUM(C12:C16)')\n\
doc.set_cell_value(4, 11, 'formula', '=SUM(D12:D16)')\n\
\
doc.set_cell_value(2, 12, 'string', '       1. Reserva legal')\n\
doc.set_cell_value(3, 12, 'float', '" + saldoCuenta ( 112 ).toQString ( '.' ) + "')\n\
doc.set_cell_value(4, 12, 'float', '" + saldoCuentaAnt ( 112 ).toQString ( '.' ) + "')\n\
\
doc.set_cell_value(2, 13, 'string', '       2. Reserva para acciones propias')\n\
doc.set_cell_value(3, 13, 'float', '" + saldoCuenta ( 115 ).toQString ( '.' ) + "')\n\
doc.set_cell_value(4, 13, 'float', '" + saldoCuentaAnt ( 115 ).toQString ( '.' ) + "')\n\
\
doc.set_cell_value(2, 14, 'string', '       3. Reserva para acciones de la sociedad dominante')\n\
doc.set_cell_value(3, 14, 'float', '" + saldoCuenta ( 114 ).toQString ( '.' ) + "')\n\
doc.set_cell_value(4, 14, 'float', '" + saldoCuentaAnt ( 114 ).toQString ( '.' ) + "')\n\
\
doc.set_cell_value(2, 15, 'string', '       4. Reservas estatutarias')\n\
doc.set_cell_value(3, 15, 'float', '" + saldoCuenta ( 116 ).toQString ( '.' ) + "')\n\
doc.set_cell_value(4, 15, 'float', '" + saldoCuentaAnt ( 116 ).toQString ( '.' ) + "')\n\
\
doc.set_cell_value(2, 16, 'string', '       5. Otras reservas')\n\
doc.set_cell_value(3, 16, 'float', '" + ( saldoCuenta ( 113 ) + saldoCuenta ( 117 ) + saldoCuenta ( 118 ) ).toQString ( '.' ) + "')\n\
doc.set_cell_value(4, 16, 'float', '" + ( saldoCuentaAnt ( 113 ) + saldoCuentaAnt ( 117 ) + saldoCuentaAnt ( 118 ) ).toQString ( '.' ) + "')\n\
\
doc.set_cell_value(2, 17, 'string', '   V.   Resultados de ejercicios anteriores')\n\
doc.set_cell_value(3, 17, 'formula', '=SUM(C18:C20)')\n\
doc.set_cell_value(4, 17, 'formula', '=SUM(D18:D20)')\n\
\
doc.set_cell_value(2, 18, 'string', '       1. Remanente')\n\
doc.set_cell_value(3, 18, 'float', '" + saldoCuenta ( 120 ).toQString ( '.' ) + "')\n\
doc.set_cell_value(4, 18, 'float', '" + saldoCuentaAnt ( 120 ).toQString ( '.' ) + "')\n\
\
doc.set_cell_value(2, 19, 'string', '       2. Resultados negativos de ejercicios anteriores')\n\
doc.set_cell_value(3, 19, 'float', '" + ( -saldoCuenta ( 121 ) ).toQString ( '.' ) + "')\n\
doc.set_cell_value(4, 19, 'float', '" + ( -saldoCuentaAnt ( 121 ) ).toQString ( '.' ) + "')\n\
\
doc.set_cell_value(2, 20, 'string', '       3. Aportaciones de socios para compensación de pérdidas')\n\
doc.set_cell_value(3, 20, 'float', '" + saldoCuenta ( 122 ).toQString ( '.' ) + "')\n\
doc.set_cell_value(4, 20, 'float', '" + saldoCuentaAnt ( 122 ).toQString ( '.' ) + "')\n\
\
doc.set_cell_value(2, 21, 'string', '   VI.  Pérdidas y ganancias (beneficio o pérdida)')\n\
doc.set_cell_value(3, 21, 'float', '" + saldoCuenta ( 129 ).toQString ( '.' ) + "')\n\
doc.set_cell_value(4, 21, 'float', '" + saldoCuentaAnt ( 129 ).toQString ( '.' ) + "')\n\
\
doc.set_cell_value(2, 22, 'string', '   VII. Dividendo a cuenta entregado en el ejercicio')\n\
doc.set_cell_value(3, 22, 'float', '" + ( -saldoCuenta ( 557 ) ).toQString ( '.' ) + "')\n\
doc.set_cell_value(4, 22, 'float', '" + ( -saldoCuentaAnt ( 557 ) ).toQString ( '.' ) + "')\n\
\
doc.set_cell_property('bold', True)\n\
doc.set_cell_value(2, 24, 'string', 'B) Ingresos a distribuir en varios ejercicios')\n\
doc.set_cell_value(3, 24, 'formula', '=SUM(C25:C27)')\n\
doc.set_cell_value(4, 24, 'formula', '=SUM(D25:D27)')\n\
doc.set_cell_property('bold', False)\n\
\
doc.set_cell_value(2, 25, 'string', '       1. Subvenciones de capital')\n\
doc.set_cell_value(3, 25, 'float', '" + ( saldoCuenta ( 130 ) + saldoCuenta ( 131 ) ).toQString ( '.' ) + "')\n\
doc.set_cell_value(4, 25, 'float', '" + ( saldoCuentaAnt ( 130 ) + saldoCuentaAnt ( 131 ) ).toQString ( '.' ) + "')\n\
\
doc.set_cell_value(2, 26, 'string', '       2. Diferencias positivas de cambio')\n\
doc.set_cell_value(3, 26, 'float', '" + saldoCuenta ( 136 ).toQString ( '.' ) + "')\n\
doc.set_cell_value(4, 26, 'float', '" + saldoCuentaAnt ( 136 ).toQString ( '.' ) + "')\n\
\
doc.set_cell_value(2, 27, 'string', '       3. Otros ingresos a distribuir en varios ejercicios')\n\
doc.set_cell_value(3, 27, 'float', '" + saldoCuenta ( 135 ).toQString ( '.' ) + "')\n\
doc.set_cell_value(4, 27, 'float', '" + saldoCuentaAnt ( 135 ).toQString ( '.' ) + "')\n\
\
doc.set_cell_property('bold', True)\n\
doc.set_cell_value(2, 29, 'string', 'C) Provisiones para riesgos y gastos')\n\
doc.set_cell_value(3, 29, 'formula', '=SUM(C30:C33)')\n\
doc.set_cell_value(4, 29, 'formula', '=SUM(D30:D33)')\n\
doc.set_cell_property('bold', False)\n\
\
doc.set_cell_value(2, 30, 'string', '       1. Provisiones para pensiones y obligaciones similares')\n\
doc.set_cell_value(3, 30, 'float', '" + saldoCuenta ( 140 ).toQString ( '.' ) + "')\n\
doc.set_cell_value(4, 30, 'float', '" + saldoCuentaAnt ( 140 ).toQString ( '.' ) + "')\n\
\
doc.set_cell_value(2, 31, 'string', '       2. Provisiones para impuestos')\n\
doc.set_cell_value(3, 31, 'float', '" + saldoCuenta ( 141 ).toQString ( '.' ) + "')\n\
doc.set_cell_value(4, 31, 'float', '" + saldoCuentaAnt ( 141 ).toQString ( '.' ) + "')\n\
\
doc.set_cell_value(2, 32, 'string', '       3. Otras provisiones')\n\
doc.set_cell_value(3, 32, 'float', '" + ( saldoCuenta ( 142 ) + saldoCuenta ( 143 ) ).toQString ( '.' ) + "')\n\
doc.set_cell_value(4, 32, 'float', '" + ( saldoCuentaAnt ( 142 ) + saldoCuentaAnt ( 143 ) ).toQString ( '.' ) + "')\n\
\
doc.set_cell_value(2, 33, 'string', '       4. Fondo de reversión')\n\
doc.set_cell_value(3, 33, 'float', '" + saldoCuenta ( 144 ).toQString ( '.' ) + "')\n\
doc.set_cell_value(4, 33, 'float', '" + saldoCuentaAnt ( 144 ).toQString ( '.' ) + "')\n\
\
doc.set_cell_property('bold', True)\n\
doc.set_cell_value(2, 35, 'string', 'D) Acreedores a largo plazo')\n\
doc.set_cell_value(3, 35, 'formula', '=C36+C40+C41+C44+C48')\n\
doc.set_cell_value(4, 35, 'formula', '=D36+D40+D41+D44+D48')\n\
doc.set_cell_property('bold', False)\n\
\
doc.set_cell_value(2, 36, 'string', '   I.   Emisiones de obligaciones y otros valores negociables')\n\
doc.set_cell_value(3, 36, 'formula', '=SUM(C37:C39)')\n\
doc.set_cell_value(4, 36, 'formula', '=SUM(D37:D39)')\n\
\
doc.set_cell_value(2, 37, 'string', '       1. Obligaciones no convertibles')\n\
doc.set_cell_value(3, 37, 'float', '" + saldoCuenta ( 150 ).toQString ( '.' ) + "')\n\
doc.set_cell_value(4, 37, 'float', '" + saldoCuentaAnt ( 150 ).toQString ( '.' ) + "')\n\
\
doc.set_cell_value(2, 38, 'string', '       2. Obligaciones convertibles')\n\
doc.set_cell_value(3, 38, 'float', '" + saldoCuenta ( 151 ).toQString ( '.' ) + "')\n\
doc.set_cell_value(4, 38, 'float', '" + saldoCuentaAnt ( 151 ).toQString ( '.' ) + "')\n\
\
doc.set_cell_value(2, 39, 'string', '       3. Otras deudas representadas en valores negociables')\n\
doc.set_cell_value(3, 39, 'float', '" + saldoCuenta ( 155 ).toQString ( '.' ) + "')\n\
doc.set_cell_value(4, 39, 'float', '" + saldoCuentaAnt ( 155 ).toQString ( '.' ) + "')\n\
\
doc.set_cell_value(2, 40, 'string', '   II.  Deudas con entidades de crédito')\n\
doc.set_cell_value(3, 40, 'float', '" + saldoCuenta ( 170 ).toQString ( '.' ) + "')\n\
doc.set_cell_value(4, 40, 'float', '" + saldoCuentaAnt ( 170 ).toQString ( '.' ) + "')\n\
\
doc.set_cell_value(2, 41, 'string', '   III. Deudas con empresas del grupo y asociadas')\n\
doc.set_cell_value(3, 41, 'formula', '=SUM(C42:C43)')\n\
doc.set_cell_value(4, 41, 'formula', '=SUM(D42:D43)')\n\
\
doc.set_cell_value(2, 42, 'string', '       1. Deudas con empresas del grupo')\n\
doc.set_cell_value(3, 42, 'float', '" + ( saldoCuenta ( 160 ) + saldoCuenta ( 162 ) + saldoCuenta ( 164 ) ).toQString ( '.' ) + "')\n\
doc.set_cell_value(4, 42, 'float', '" + ( saldoCuentaAnt ( 160 ) + saldoCuentaAnt ( 162 ) + saldoCuentaAnt ( 164 ) ).toQString ( '.' ) + "')\n\
\
doc.set_cell_value(2, 43, 'string', '       2. Deudas con empresas asociadas')\n\
doc.set_cell_value(3, 43, 'float', '" + ( saldoCuenta ( 161 ) + saldoCuenta ( 163 ) + saldoCuenta ( 165 ) ).toQString ( '.' ) + "')\n\
doc.set_cell_value(4, 43, 'float', '" + ( saldoCuentaAnt ( 161 ) + saldoCuentaAnt ( 163 ) + saldoCuentaAnt ( 165 ) ).toQString ( '.' ) + "')\n\
\
doc.set_cell_value(2, 44, 'string', '   IV.  Otros acreedores')\n\
doc.set_cell_value(3, 44, 'formula', '=SUM(C45:C47)')\n\
doc.set_cell_value(4, 44, 'formula', '=SUM(D45:D47)')\n\
\
doc.set_cell_value(2, 45, 'string', '       1. Deudas representadas por efectos a pagar')\n\
doc.set_cell_value(3, 45, 'float', '" + saldoCuenta ( 174 ).toQString ( '.' ) + "')\n\
doc.set_cell_value(4, 45, 'float', '" + saldoCuentaAnt ( 174 ).toQString ( '.' ) + "')\n\
\
doc.set_cell_value(2, 46, 'string', '       2. Otras deudas')\n\
doc.set_cell_value(3, 46, 'float', '" + ( saldoCuenta ( 171 ) + saldoCuenta ( 172 ) + saldoCuenta ( 173 ) ).toQString ( '.' ) + "')\n\
doc.set_cell_value(4, 46, 'float', '" + ( saldoCuentaAnt ( 171 ) + saldoCuentaAnt ( 172 ) + saldoCuentaAnt ( 173 ) ).toQString ( '.' ) + "')\n\
\
doc.set_cell_value(2, 47, 'string', '       3. Fianzas y depósitos recibidos a largo plazo')\n\
doc.set_cell_value(3, 47, 'float', '" + ( saldoCuenta ( 180 ) + saldoCuenta ( 185 ) ).toQString ( '.' ) + "')\n\
doc.set_cell_value(4, 47, 'float', '" + ( saldoCuentaAnt ( 180 ) + saldoCuentaAnt ( 185 ) ).toQString ( '.' ) + "')\n\
\
doc.set_cell_value(2, 48, 'string', '   V.   Desembolsos pendientes sobre acciones, no exigidos')\n\
doc.set_cell_value(3, 48, 'formula', '=SUM(C49:C51)')\n\
doc.set_cell_value(4, 48, 'formula', '=SUM(D49:D51)')\n\
\
doc.set_cell_value(2, 49, 'string', '       1. De empresas del grupo')\n\
doc.set_cell_value(3, 49, 'float', '" + saldoCuenta ( 248 ).toQString ( '.' ) + "')\n\
doc.set_cell_value(4, 49, 'float', '" + saldoCuentaAnt ( 248 ).toQString ( '.' ) + "')\n\
\
doc.set_cell_value(2, 50, 'string', '       2. De empresas asociadas')\n\
doc.set_cell_value(3, 50, 'float', '" + saldoCuenta ( 249 ).toQString ( '.' ) + "')\n\
doc.set_cell_value(4, 50, 'float', '" + saldoCuentaAnt ( 249 ).toQString ( '.' ) + "')\n\
\
doc.set_cell_value(2, 51, 'string', '       3. De otras empresas')\n\
doc.set_cell_value(3, 51, 'float', '" + saldoCuenta ( 259 ).toQString ( '.' ) + "')\n\
doc.set_cell_value(4, 51, 'float', '" + saldoCuentaAnt ( 259 ).toQString ( '.' ) + "')\n\
\
doc.set_cell_property('bold', True)\n\
doc.set_cell_value(2, 53, 'string', 'E) Acreedores a corto plazo')\n\
doc.set_cell_value(3, 53, 'formula', '=C54+C59+C62+C65+C69+C75+C76')\n\
doc.set_cell_value(4, 53, 'formula', '=D54+D59+D62+D65+D69+D75+D76')\n\
doc.set_cell_property('bold', False)\n\
\
doc.set_cell_value(2, 54, 'string', '   I.   Emisiones de obligaciones y otros valores negociables')\n\
doc.set_cell_value(3, 54, 'formula', '=SUM(C55:C58)')\n\
doc.set_cell_value(4, 54, 'formula', '=SUM(D55:D58)')\n\
\
doc.set_cell_value(2, 55, 'string', '       1. Obligaciones no convertibles')\n\
doc.set_cell_value(3, 55, 'float', '" + saldoCuenta ( 500 ).toQString ( '.' ) + "')\n\
doc.set_cell_value(4, 55, 'float', '" + saldoCuentaAnt ( 500 ).toQString ( '.' ) + "')\n\
\
doc.set_cell_value(2, 56, 'string', '       2. Obligaciones convertibles')\n\
doc.set_cell_value(3, 56, 'float', '" + saldoCuenta ( 501 ).toQString ( '.' ) + "')\n\
doc.set_cell_value(4, 56, 'float', '" + saldoCuentaAnt ( 501 ).toQString ( '.' ) + "')\n\
\
doc.set_cell_value(2, 57, 'string', '       3. Otras deudas representadas en valores negociables')\n\
doc.set_cell_value(3, 57, 'float', '" + saldoCuenta ( 505 ).toQString ( '.' ) + "')\n\
doc.set_cell_value(4, 57, 'float', '" + saldoCuentaAnt ( 505 ).toQString ( '.' ) + "')\n\
\
doc.set_cell_value(2, 58, 'string', '       4. Intereses de obligaciones y otros valores')\n\
doc.set_cell_value(3, 58, 'float', '" + saldoCuenta ( 506 ).toQString ( '.' ) + "')\n\
doc.set_cell_value(4, 58, 'float', '" + saldoCuentaAnt ( 506 ).toQString ( '.' ) + "')\n\
\
doc.set_cell_value(2, 59, 'string', '   II.  Deudas con entidades de crédito')\n\
doc.set_cell_value(3, 54, 'formula', '=SUM(C60:C61)')\n\
doc.set_cell_value(4, 54, 'formula', '=SUM(D60:D61)')\n\
\
doc.set_cell_value(2, 60, 'string', '       1. Préstamos y otras deudas')\n\
doc.set_cell_value(3, 60, 'float', '" + saldoCuenta ( 520 ).toQString ( '.' ) + "')\n\
doc.set_cell_value(4, 60, 'float', '" + saldoCuentaAnt ( 520 ).toQString ( '.' ) + "')\n\
\
doc.set_cell_value(2, 61, 'string', '       2. Deudas por intereses')\n\
doc.set_cell_value(3, 61, 'float', '" + saldoCuenta ( 526 ).toQString ( '.' ) + "')\n\
doc.set_cell_value(4, 61, 'float', '" + saldoCuentaAnt ( 526 ).toQString ( '.' ) + "')\n\
\
doc.set_cell_value(2, 62, 'string', '   III. Deudas con empresas del grupo y asociadas a corto plazo')\n\
doc.set_cell_value(3, 54, 'formula', '=SUM(C63:C64)')\n\
doc.set_cell_value(4, 54, 'formula', '=SUM(D63:D64)')\n\
\
doc.set_cell_value(2, 63, 'string', '       1. Deudas con empresas del grupo')\n\
doc.set_cell_value(3, 63, 'float', '" + ( saldoCuenta ( 402 ) + saldoCuenta ( 510 ) + saldoCuenta ( 512 ) + saldoCuenta ( 514 ) + saldoCuenta ( 516 ) + saldoCuenta ( 551 ) ).toQString ( '.' ) + "')\n\
doc.set_cell_value(4, 63, 'float', '" + ( saldoCuentaAnt ( 402 ) + saldoCuentaAnt ( 510 ) + saldoCuentaAnt ( 512 ) + saldoCuentaAnt ( 514 ) + saldoCuentaAnt ( 516 ) + saldoCuentaAnt ( 551 ) ).toQString ( '.' ) + "')\n\
\
doc.set_cell_value(2, 64, 'string', '       2. Deudas con empresas asociadas')\n\
doc.set_cell_value(3, 64, 'float', '" + ( saldoCuenta ( 403 ) + saldoCuenta ( 511 ) + saldoCuenta ( 513 ) + saldoCuenta ( 515 ) + saldoCuenta ( 517 ) + saldoCuenta ( 552 ) ).toQString ( '.' ) + "')\n\
doc.set_cell_value(4, 64, 'float', '" + ( saldoCuentaAnt ( 403 ) + saldoCuentaAnt ( 511 ) + saldoCuentaAnt ( 513 ) + saldoCuentaAnt ( 515 ) + saldoCuentaAnt ( 517 ) + saldoCuentaAnt ( 552 ) ).toQString ( '.' ) + "')\n\
\
doc.set_cell_value(2, 65, 'string', '   IV.  Acreedores comerciales')\n\
doc.set_cell_value(3, 54, 'formula', '=SUM(C66:C68)')\n\
doc.set_cell_value(4, 54, 'formula', '=SUM(D66:D68)')\n\
\
doc.set_cell_value(2, 66, 'string', '       1. Anticipos recibidos por pedidos')\n\
doc.set_cell_value(3, 66, 'float', '" + saldoCuenta ( 437 ).toQString ( '.' ) + "')\n\
doc.set_cell_value(4, 66, 'float', '" + saldoCuentaAnt ( 437 ).toQString ( '.' ) + "')\n\
\
doc.set_cell_value(2, 67, 'string', '       2. Deudas por compras o prestaciones de servicios')\n\
doc.set_cell_value(3, 67, 'float', '" + ( saldoCuenta ( 400 ) + ( -saldoCuenta ( 406 ) ) + saldoCuenta ( 410 ) + saldoCuenta ( 419 ) ).toQString ( '.' ) + "')\n\
doc.set_cell_value(4, 67, 'float', '" + ( saldoCuentaAnt ( 400 ) + ( -saldoCuentaAnt ( 406 ) ) + saldoCuentaAnt ( 410 ) + saldoCuentaAnt ( 419 ) ).toQString ( '.' ) + "')\n\
\
doc.set_cell_value(2, 68, 'string', '       3. Deudas representadas por efectos a pagar')\n\
doc.set_cell_value(3, 68, 'float', '" + ( saldoCuenta ( 401 ) + saldoCuenta ( 411 ) ).toQString ( '.' ) + "')\n\
doc.set_cell_value(4, 68, 'float', '" + ( saldoCuentaAnt ( 401 ) + saldoCuentaAnt ( 411 ) ).toQString ( '.' ) + "')\n\
\
doc.set_cell_value(2, 69, 'string', '   V.   Otras deudas no comerciales')\n\
doc.set_cell_value(3, 54, 'formula', '=SUM(C70:C74)')\n\
doc.set_cell_value(4, 54, 'formula', '=SUM(D70:D74)')\n\
\
doc.set_cell_value(2, 70, 'string', '       1. Administraciones Públicas')\n\
doc.set_cell_value(3, 70, 'float', '" + ( saldoCuenta ( 475 ) + saldoCuenta ( 476 ) + saldoCuenta ( 477 ) + saldoCuenta ( 479 ) ).toQString ( '.' ) + "')\n\
doc.set_cell_value(4, 70, 'float', '" + ( saldoCuentaAnt ( 475 ) + saldoCuentaAnt ( 476 ) + saldoCuentaAnt ( 477 ) + saldoCuentaAnt ( 479 ) ).toQString ( '.' ) + "')\n\
\
doc.set_cell_value(2, 71, 'string', '       2. Deudas representadas por efectos a pagar')\n\
doc.set_cell_value(3, 71, 'float', '" + saldoCuenta ( 524 ).toQString ( '.' ) + "')\n\
doc.set_cell_value(4, 71, 'float', '" + saldoCuentaAnt ( 524 ).toQString ( '.' ) + "')\n\
\
doc.set_cell_value(2, 72, 'string', '       3. Otras deudas')\n\
doc.set_cell_value(3, 72, 'float', '" + ( saldoCuenta ( 509 ) + saldoCuenta ( 521 ) + saldoCuenta ( 523 ) + saldoCuenta ( 525 ) + saldoCuenta ( 527 ) + saldoCuenta ( 553 ) + saldoCuenta ( 555 ) + saldoCuenta ( 556 ) ).toQString ( '.' ) + "')\n\
doc.set_cell_value(4, 72, 'float', '" + ( saldoCuentaAnt ( 509 ) + saldoCuentaAnt ( 521 ) + saldoCuentaAnt ( 523 ) + saldoCuentaAnt ( 525 ) + saldoCuentaAnt ( 527 ) + saldoCuentaAnt ( 553 ) + saldoCuentaAnt ( 555 ) + saldoCuentaAnt ( 556 ) ).toQString ( '.' ) + "')\n\
\
doc.set_cell_value(2, 73, 'string', '       4. Remuneraciones pendientes de pago')\n\
doc.set_cell_value(3, 73, 'float', '" + saldoCuenta ( 465 ).toQString ( '.' ) + "')\n\
doc.set_cell_value(4, 73, 'float', '" + saldoCuentaAnt ( 465 ).toQString ( '.' ) + "')\n\
\
doc.set_cell_value(2, 74, 'string', '       5. Fianzas y depósitos recibidos a corto plazo')\n\
doc.set_cell_value(3, 74, 'float', '" + ( saldoCuenta ( 560 ) + saldoCuenta ( 561 ) ).toQString ( '.' ) + "')\n\
doc.set_cell_value(4, 74, 'float', '" + ( saldoCuentaAnt ( 560 ) + saldoCuentaAnt ( 561 ) ).toQString ( '.' ) + "')\n\
\
doc.set_cell_value(2, 75, 'string', '   VI.  Provisiones para operaciones de tráfico')\n\
doc.set_cell_value(3, 75, 'float', '" + saldoCuenta ( 499 ).toQString ( '.' ) + "')\n\
doc.set_cell_value(4, 75, 'float', '" + saldoCuentaAnt ( 499 ).toQString ( '.' ) + "')\n\
\
doc.set_cell_value(2, 76, 'string', '   VII. Ajustes por periodificación')\n\
doc.set_cell_value(3, 76, 'float', '" + ( saldoCuenta ( 485 ) + saldoCuenta ( 585 ) ).toQString ( '.' ) + "')\n\
doc.set_cell_value(4, 76, 'float', '" + ( saldoCuentaAnt ( 485 ) + saldoCuentaAnt ( 585 ) ).toQString ( '.' ) + "')\n\
\
doc.set_cell_property('bold', True)\n\
doc.set_cell_value(2, 78, 'string', 'TOTAL GENERAL (A+B+C+D+E)')\n\
doc.set_cell_value(3, 78, 'formula', '=C7+C24+C29+C35+C53')\n\
doc.set_cell_value(4, 78, 'formula', '=D7+D24+D29+D35+D53')\n\
doc.set_cell_property('bold', False)\n\
\
\
\
\
\
\
doc.new_sheet(\"Debe\")\n\n\
doc.set_column_property(1, 'width', '0.5in')\n\
doc.set_column_property(2, 'width', '5in')\n\
doc.set_cell_property('bold', True)\n\
doc.set_cell_value(2, 2, 'string', 'CUENTA DE PÉRDIDAS Y GANANCIAS')\n\
doc.set_cell_value(2, 5, 'string', 'DEBE')\n\
doc.set_cell_property('bold', False)\n\
\
doc.set_cell_property('bold', True)\n\
doc.set_cell_value(2, 7, 'string', 'A) GASTOS')\n\
doc.set_cell_value(3, 7, 'formula', '=0')\n\
doc.set_cell_value(4, 7, 'formula', '=0')\n\
doc.set_cell_property('bold', False)\n\
\
doc.set_cell_value(2, 8, 'string', '       1. Reducción de existencias de productos terminados y en curso de fabricación')\n\
doc.set_cell_value(3, 8, 'float', '" + saldoCuenta ( 71 ).toQString ( '.' ) + "')\n\
doc.set_cell_value(4, 8, 'float', '" + saldoCuentaAnt ( 71 ).toQString ( '.' ) + "')\n\
\
doc.set_cell_value(2, 9, 'string', '       2. Aprovisionamientos')\n\
doc.set_cell_value(3, 9, 'float', '" + ( saldoCuenta ( 600 ) + ( -saldoCuenta ( 6080 ) ) ).toQString ( '.' ) + "')\n\
doc.set_cell_value(4, 9, 'float', '" + ( saldoCuentaAnt ( 600 ) + ( -saldoCuentaAnt ( 6080 ) ) ).toQString ( '.' ) + "')\n\
\
doc.set_cell_value(2, 10, 'string', '            a) Consumo de mercaderías')\n\
doc.set_cell_value(3, 10, 'float', '" + ( ( -saldoCuenta ( 6090 ) ) + saldoCuenta ( 610 ) ).toQString ( '.' ) + "')\n\
doc.set_cell_value(4, 10, 'float', '" + ( ( -saldoCuentaAnt ( 6090 ) ) + saldoCuentaAnt ( 610 ) ).toQString ( '.' ) + "')\n\
\
doc.set_cell_value(2, 11, 'string', '            b) Consumo de materias primas y otras materias consumibles')\n\
doc.set_cell_value(3, 11, 'float', '" + ( saldoCuenta ( 601 ) + saldoCuenta ( 602 ) + ( -saldoCuenta ( 6081 ) ) + ( -saldoCuenta ( 6082 ) ) + ( -saldoCuenta ( 6091 ) ) + ( -saldoCuenta ( 6092 ) ) + saldoCuenta ( 611 ) + saldoCuenta ( 612 ) ).toQString ( '.' ) + "')\n\
doc.set_cell_value(4, 11, 'float', '" + ( saldoCuentaAnt ( 601 ) + saldoCuentaAnt ( 602 ) + ( -saldoCuentaAnt ( 6081 ) ) + ( -saldoCuentaAnt ( 6082 ) ) + ( -saldoCuentaAnt ( 6091 ) ) + ( -saldoCuentaAnt ( 6092 ) ) + saldoCuentaAnt ( 611 ) + saldoCuentaAnt ( 612 ) ).toQString ( '.' ) + "')\n\
\
doc.set_cell_value(2, 12, 'string', '            c) Otros gastos externos')\n\
doc.set_cell_value(3, 12, 'float', '" + saldoCuenta ( 607 ).toQString ( '.' ) + "')\n\
doc.set_cell_value(4, 12, 'float', '" + saldoCuentaAnt ( 607 ).toQString ( '.' ) + "')\n\
\
doc.set_cell_value(2, 13, 'string', '       3. Gastos de personal')\n\
doc.set_cell_value(3, 13, 'formula', '=SUM(C14:C15)')\n\
doc.set_cell_value(4, 13, 'formula', '=SUM(C14:C15)')\n\
\
doc.set_cell_value(2, 14, 'string', '            a) Sueldos, salarios y asimilados')\n\
doc.set_cell_value(3, 14, 'float', '" + ( saldoCuenta ( 640 ) + saldoCuenta ( 641 ) ).toQString ( '.' ) + "')\n\
doc.set_cell_value(4, 14, 'float', '" + ( saldoCuentaAnt ( 640 ) + saldoCuentaAnt ( 641 ) ).toQString ( '.' ) + "')\n\
\
doc.set_cell_value(2, 15, 'string', '            b) Cargas sociales')\n\
doc.set_cell_value(3, 15, 'float', '" + ( saldoCuenta ( 642 ) + saldoCuenta ( 643 ) + saldoCuenta ( 649 ) ).toQString ( '.' ) + "')\n\
doc.set_cell_value(4, 15, 'float', '" + ( saldoCuentaAnt ( 642 ) + saldoCuentaAnt ( 643 ) + saldoCuentaAnt ( 649 ) ).toQString ( '.' ) + "')\n\
\
doc.set_cell_value(2, 16, 'string', '       4. Dotaciones para amortizaciones de inmovilizado')\n\
doc.set_cell_value(3, 16, 'float', '" + saldoCuenta ( 68 ).toQString ( '.' ) + "')\n\
doc.set_cell_value(4, 16, 'float', '" + saldoCuentaAnt ( 68 ).toQString ( '.' ) + "')\n\
\
doc.set_cell_value(2, 17, 'string', '       5. Variación de las provisiones de tráfico')\n\
doc.set_cell_value(3, 17, 'formula', '=SUM(C18:C20)')\n\
doc.set_cell_value(4, 17, 'formula', '=SUM(C18:C20)')\n\
\
doc.set_cell_value(2, 18, 'string', '            a) Variaciones de las provisiones de existencias')\n\
doc.set_cell_value(3, 18, 'float', '" + ( saldoCuenta ( 693 ) + ( -saldoCuenta ( 793 ) ) ).toQString ( '.' ) + "')\n\
doc.set_cell_value(4, 18, 'float', '" + ( saldoCuentaAnt ( 693 ) + ( -saldoCuentaAnt ( 793 ) ) ).toQString ( '.' ) + "')\n\
\
doc.set_cell_value(2, 19, 'string', '            b) Variaciones de provisiones y pérdidas de créditos incobrables')\n\
doc.set_cell_value(3, 19, 'float', '" + ( saldoCuenta ( 650 ) + saldoCuenta ( 694 ) + ( -saldoCuenta ( 794 ) ) ).toQString ( '.' ) + "')\n\
doc.set_cell_value(4, 19, 'float', '" + ( saldoCuentaAnt ( 650 ) + saldoCuentaAnt ( 694 ) + ( -saldoCuentaAnt ( 794 ) ) ).toQString ( '.' ) + "')\n\
\
doc.set_cell_value(2, 20, 'string', '            c) Variaciones de otras provisiones de tráfico')\n\
doc.set_cell_value(3, 20, 'float', '" + ( saldoCuenta ( 695 ) + ( -saldoCuenta ( 795 ) ) ).toQString ( '.' ) + "')\n\
doc.set_cell_value(4, 20, 'float', '" + ( saldoCuentaAnt ( 695 ) + ( -saldoCuentaAnt ( 795 ) ) ).toQString ( '.' ) + "')\n\
\
doc.set_cell_value(2, 21, 'string', '       6. Otros gastos de explotación')\n\
doc.set_cell_value(3, 21, 'formula', '=SUM(C22:C25)')\n\
doc.set_cell_value(4, 21, 'formula', '=SUM(C22:C25)')\n\
\
doc.set_cell_value(2, 22, 'string', '            a) Servicios exteriores')\n\
doc.set_cell_value(3, 22, 'float', '" + saldoCuenta ( 62 ).toQString ( '.' ) + "')\n\
doc.set_cell_value(4, 22, 'float', '" + saldoCuentaAnt ( 62 ).toQString ( '.' ) + "')\n\
\
doc.set_cell_value(2, 23, 'string', '            b) Tributos')\n\
doc.set_cell_value(3, 23, 'float', '" + ( saldoCuenta ( 631 ) + saldoCuenta ( 634 ) + ( -saldoCuenta ( 636 ) ) + ( -saldoCuenta ( 639 ) ) ).toQString ( '.' ) + "')\n\
doc.set_cell_value(4, 23, 'float', '" + ( saldoCuentaAnt ( 631 ) + saldoCuentaAnt ( 634 ) + ( -saldoCuentaAnt ( 636 ) ) + ( -saldoCuentaAnt ( 639 ) ) ).toQString ( '.' ) + "')\n\
\
doc.set_cell_value(2, 24, 'string', '            c) Otros gastos de gestión corriente')\n\
doc.set_cell_value(3, 24, 'float', '" + ( saldoCuenta ( 651 ) + saldoCuenta ( 659 ) ).toQString ( '.' ) + "')\n\
doc.set_cell_value(4, 24, 'float', '" + ( saldoCuentaAnt ( 651 ) + saldoCuentaAnt ( 659 ) ).toQString ( '.' ) + "')\n\
\
doc.set_cell_value(2, 25, 'string', '            d) Dotación al fondo de reversión')\n\
doc.set_cell_value(3, 25, 'float', '" + saldoCuenta ( 690 ).toQString ( '.' ) + "')\n\
doc.set_cell_value(4, 25, 'float', '" + saldoCuentaAnt ( 690 ).toQString ( '.' ) + "')\n\
\
doc.set_cell_value(2, 27, 'string', '   I.   BENEFICIOS DE EXLOTACIÓN')\n\
doc.set_cell_value(2, 28, 'string', '        (B1+B2+B3+B4-A1-A2-A3-A4-A5-A6)')\n\
doc.set_cell_value(3, 27, 'formula', '=Haber.C8+Haber.C12+Haber.C13+Haber.C14-C8-C9-C13-C16-C17-C21')\n\
doc.set_cell_value(4, 27, 'formula', '=Haber.D8+Haber.D12+Haber.D13+Haber.D14-D8-D9-D13-D16-D17-D21')\n\
\
doc.set_cell_value(2, 30, 'string', '       7. Gastos financieros y gastos asimilados')\n\
doc.set_cell_value(3, 30, 'formula', '=SUM(C31:C34)')\n\
doc.set_cell_value(4, 30, 'formula', '=SUM(D31:D34)')\n\
\
doc.set_cell_value(2, 31, 'string', '            a) Por deudas con empresas del grupo')\n\
doc.set_cell_value(3, 31, 'float', '" + ( saldoCuenta ( 6610 ) + saldoCuenta ( 6615 ) + saldoCuenta ( 6620 ) + saldoCuenta ( 6630 ) + saldoCuenta ( 6640 ) + saldoCuenta ( 6650 ) ).toQString ( '.' ) + "')\n\
doc.set_cell_value(4, 31, 'float', '" + ( saldoCuentaAnt ( 6610 ) + saldoCuentaAnt ( 6615 ) + saldoCuentaAnt ( 6620 ) + saldoCuentaAnt ( 6630 ) + saldoCuentaAnt ( 6640 ) + saldoCuentaAnt ( 6650 ) ).toQString ( '.' ) + "')\n\
\
doc.set_cell_value(2, 32, 'string', '            b) Por deudas con empresas asociadas')\n\
doc.set_cell_value(3, 32, 'float', '" + ( saldoCuenta ( 6611 ) + saldoCuenta ( 6616 ) + saldoCuenta ( 6621 ) + saldoCuenta ( 6631 ) + saldoCuenta ( 6641 ) + saldoCuenta ( 6651 ) ).toQString ( '.' ) + "')\n\
doc.set_cell_value(4, 32, 'float', '" + ( saldoCuentaAnt ( 6611 ) + saldoCuentaAnt ( 6616 ) + saldoCuentaAnt ( 6621 ) + saldoCuentaAnt ( 6631 ) + saldoCuentaAnt ( 6641 ) + saldoCuentaAnt ( 6651 ) ).toQString ( '.' ) + "')\n\
\
doc.set_cell_value(2, 33, 'string', '            c) Por deudas con terceros y gastos asimilados')\n\
doc.set_cell_value(3, 33, 'float', '" + ( saldoCuenta ( 6613 ) + saldoCuenta ( 6618 ) + saldoCuenta ( 6622 ) ).toQString ( '.' ) + "')\n\
doc.set_cell_value(4, 33, 'float', '" + ( saldoCuentaAnt ( 6613 ) + saldoCuentaAnt ( 6618 ) + saldoCuentaAnt ( 6622 ) ).toQString ( '.' ) + "')\n\
\
doc.set_cell_value(2, 34, 'string', '            d) Pérdidas de inversiones financieras')\n\
doc.set_cell_value(3, 34, 'float', '" + ( saldoCuenta ( 6623 ) + saldoCuenta ( 6632 ) + saldoCuenta ( 6633 ) + saldoCuenta ( 6643 ) + saldoCuenta ( 6653 ) + saldoCuenta ( 669 ) + saldoCuenta ( 666 ) + saldoCuenta ( 667 ) ).toQString ( '.' ) + "')\n\
doc.set_cell_value(4, 34, 'float', '" + ( saldoCuentaAnt ( 6623 ) + saldoCuentaAnt ( 6632 ) + saldoCuentaAnt ( 6633 ) + saldoCuentaAnt ( 6643 ) + saldoCuentaAnt ( 6653 ) + saldoCuentaAnt ( 669 ) + saldoCuentaAnt ( 666 ) + saldoCuentaAnt ( 667 ) ).toQString ( '.' ) + "')\n\
\
doc.set_cell_value(2, 35, 'string', '       8. Variación de las provisiones de inversiones financieras')\n\
doc.set_cell_value(3, 35, 'float', '" + ( saldoCuenta ( 6963 ) + saldoCuenta ( 6965 ) + saldoCuenta ( 6966 ) + saldoCuenta ( 697 ) + saldoCuenta ( 698 ) + saldoCuenta ( 699 ) + ( -saldoCuenta ( 7963 ) ) + ( -saldoCuenta ( 7965 ) ) + ( -saldoCuenta ( 7966 ) ) + ( -saldoCuenta ( 797 ) ) + ( -saldoCuenta ( 798 ) ) + ( -saldoCuenta ( 799 ) ) ).toQString ( '.' ) + "')\n\
doc.set_cell_value(4, 35, 'float', '" + ( saldoCuentaAnt ( 6963 ) + saldoCuentaAnt ( 6965 ) + saldoCuentaAnt ( 6966 ) + saldoCuentaAnt ( 697 ) + saldoCuentaAnt ( 698 ) + saldoCuentaAnt ( 699 ) + ( -saldoCuentaAnt ( 7963 ) ) + ( -saldoCuentaAnt ( 7965 ) ) + ( -saldoCuentaAnt ( 7966 ) ) + ( -saldoCuentaAnt ( 797 ) ) + ( -saldoCuentaAnt ( 798 ) ) + ( -saldoCuentaAnt ( 799 ) ) ).toQString ( '.' ) + "')\n\
\
doc.set_cell_value(2, 36, 'string', '       9. Diferencias negativas de cambio')\n\
doc.set_cell_value(3, 36, 'float', '" + saldoCuenta ( 668 ).toQString ( '.' ) + "')\n\
doc.set_cell_value(4, 36, 'float', '" + saldoCuentaAnt ( 668 ).toQString ( '.' ) + "')\n\
\
doc.set_cell_value(2, 38, 'string', '   II.  RESULTADOS FINANCIEROS POSITIVOS')\n\
doc.set_cell_value(2, 39, 'string', '        (B5+B6+B7+B8-A7-A8-A9)')\n\
doc.set_cell_value(3, 38, 'formula', '=Haber.C22+Haber.C26+Haber.C30+Haber.C35-C31-C35-C36')\n\
doc.set_cell_value(4, 38, 'formula', '=Haber.D22+Haber.D26+Haber.D30+Haber.D35-D31-D35-D36')\n\
\
doc.set_cell_value(2, 41, 'string', '   III. BENEFICIOS DE LAS ACTIVIDADES ORDINARIAS')\n\
doc.set_cell_value(2, 42, 'string', '        (AI+AII-BI-BII)')\n\
doc.set_cell_value(3, 41, 'formula', '=C27+C38-Haber.C19-Haber.C37')\n\
doc.set_cell_value(4, 41, 'formula', '=D27+D38-Haber.D19-Haber.D37')\n\
\
doc.set_cell_value(2, 44, 'string', '      10. Variación de las provisiones de inmobilizado inmaterial, material y cartera de control')\n\
doc.set_cell_value(3, 44, 'float', '" + ( saldoCuenta ( 691 ) + saldoCuenta ( 692 ) + saldoCuenta ( 6960 ) + saldoCuenta ( 6961 ) + ( -saldoCuenta ( 791 ) ) + ( -saldoCuenta ( 792 ) ) + ( -saldoCuenta ( 7960 ) ) + ( -saldoCuenta ( 7961 ) ) ).toQString ( '.' ) + "')\n\
doc.set_cell_value(4, 44, 'float', '" + ( saldoCuentaAnt ( 691 ) + saldoCuentaAnt ( 692 ) + saldoCuentaAnt ( 6960 ) + saldoCuentaAnt ( 6961 ) + ( -saldoCuentaAnt ( 791 ) ) + ( -saldoCuentaAnt ( 792 ) ) + ( -saldoCuentaAnt ( 7960 ) ) + ( -saldoCuentaAnt ( 7961 ) ) ).toQString ( '.' ) + "')\n\
\
doc.set_cell_value(2, 45, 'string', '      11. Pérdidas procedentes del inmovilizado')\n\
doc.set_cell_value(3, 45, 'float', '" + ( saldoCuenta ( 670 ) + saldoCuenta ( 671 ) + saldoCuenta ( 672 ) + saldoCuenta ( 673 ) ).toQString ( '.' ) + "')\n\
doc.set_cell_value(4, 45, 'float', '" + ( saldoCuentaAnt ( 670 ) + saldoCuentaAnt ( 671 ) + saldoCuentaAnt ( 672 ) + saldoCuentaAnt ( 673 ) ).toQString ( '.' ) + "')\n\
\
doc.set_cell_value(2, 46, 'string', '      12. Pérdidas por operaciones con acciones y obligaciones propias')\n\
doc.set_cell_value(3, 46, 'float', '" + saldoCuenta ( 674 ).toQString ( '.' ) + "')\n\
doc.set_cell_value(4, 46, 'float', '" + saldoCuentaAnt ( 674 ).toQString ( '.' ) + "')\n\
\
doc.set_cell_value(2, 47, 'string', '      13. Gastos extraordinarios')\n\
doc.set_cell_value(3, 47, 'float', '" + saldoCuenta ( 678 ).toQString ( '.' ) + "')\n\
doc.set_cell_value(4, 47, 'float', '" + saldoCuentaAnt ( 678 ).toQString ( '.' ) + "')\n\
\
doc.set_cell_value(2, 48, 'string', '      14. Gastos y pérdidas de otros ejercicios')\n\
doc.set_cell_value(3, 48, 'float', '" + saldoCuenta ( 679 ).toQString ( '.' ) + "')\n\
doc.set_cell_value(4, 48, 'float', '" + saldoCuentaAnt ( 679 ).toQString ( '.' ) + "')\n\
\
doc.set_cell_value(2, 50, 'string', '   IV.  RESULTADOS EXTRAORDINARIOS POSITIVOS')\n\
doc.set_cell_value(2, 51, 'string', '        (B9+B10+B11+B12+B13-A10-A11-A12-A13-A14)')\n\
doc.set_cell_value(3, 50, 'formula', '=Haber.C43+Haber.C44+Haber.C45+Haber.C46+Haber.C47-C44-C45-C46-C47-C48')\n\
doc.set_cell_value(4, 50, 'formula', '=Haber.D43+Haber.D44+Haber.D45+Haber.D46+Haber.D47-D44-D45-D46-D47-D48')\n\
\
doc.set_cell_value(2, 53, 'string', '   V.   BENEFICIOS ANTES DE IMPUESTOS')\n\
doc.set_cell_value(2, 54, 'string', '        (AIII+AIV-BIII-BIV)')\n\
doc.set_cell_value(3, 53, 'formula', '=C41+C50-Haber.C40-Haber.C49')\n\
doc.set_cell_value(4, 53, 'formula', '=D41+D50-Haber.D40-Haber.D49')\n\
\
doc.set_cell_value(2, 56, 'string', '      15. Impuesto sobre Sociedades')\n\
doc.set_cell_value(3, 56, 'float', '" + ( saldoCuenta ( 630 ) + saldoCuenta ( 633 ) + ( -saldoCuenta ( 638 ) ) ).toQString ( '.' ) + "')\n\
doc.set_cell_value(4, 56, 'float', '" + ( saldoCuentaAnt ( 630 ) + saldoCuentaAnt ( 633 ) + ( -saldoCuentaAnt ( 638 ) ) ).toQString ( '.' ) + "')\n\
\
doc.set_cell_value(2, 57, 'string', '      16. Otros impuestos')\n\
doc.set_cell_value(3, 57, 'formula', '=0')\n\
doc.set_cell_value(4, 57, 'formula', '=0')\n\
\
doc.set_cell_value(2, 59, 'string', '   VI.  RESULTADO DEL EJERCICIO (BENEFICIOS)')\n\
doc.set_cell_value(2, 60, 'string', '        (AV-A15-A16)')\n\
doc.set_cell_value(3, 59, 'formula', '=C53-C56-C57')\n\
doc.set_cell_value(4, 59, 'formula', '=D53-D56-D57')\n\
\
\
\
\
\
doc.new_sheet(\"Haber\")\n\n\
doc.set_column_property(1, 'width', '0.5in')\n\
doc.set_column_property(2, 'width', '5in')\n\
doc.set_cell_property('bold', True)\n\
doc.set_cell_value(2, 2, 'string', 'CUENTA DE PÉRDIDAS Y GANANCIAS')\n\
doc.set_cell_value(2, 5, 'string', 'HABER')\n\
doc.set_cell_property('bold', False)\n\
\
doc.set_cell_property('bold', True)\n\
doc.set_cell_value(2, 7, 'string', 'B) INGRESOS')\n\
doc.set_cell_property('bold', False)\n\
\
doc.set_cell_value(2, 8, 'string', '       1. Importe neto de la cifra de negocio')\n\
doc.set_cell_value(3, 8, 'formula', '=SUM(C9:C11)')\n\
doc.set_cell_value(4, 8, 'formula', '=SUM(D9:D11)')\n\
\
doc.set_cell_value(2, 9, 'string', '            a) Ventas')\n\
doc.set_cell_value(3, 9, 'float', '" + ( saldoCuenta ( 700 ) + saldoCuenta ( 701 ) + saldoCuenta ( 702 ) + saldoCuenta ( 703 ) + saldoCuenta ( 704 ) ).toQString ( '.' ) + "')\n\
doc.set_cell_value(4, 9, 'float', '" + ( saldoCuentaAnt ( 700 ) + saldoCuentaAnt ( 701 ) + saldoCuentaAnt ( 702 ) + saldoCuentaAnt ( 703 ) + saldoCuentaAnt ( 704 ) ).toQString ( '.' ) + "')\n\
\
doc.set_cell_value(2, 10, 'string', '            b) Prestaciones de servicios')\n\
doc.set_cell_value(3, 10, 'float', '" + saldoCuenta ( 705 ).toQString ( '.' ) + "')\n\
doc.set_cell_value(4, 10, 'float', '" + saldoCuentaAnt ( 705 ).toQString ( '.' ) + "')\n\
\
doc.set_cell_value(2, 11, 'string', '            c) Devoluciones y \"rappels\" sobre ventas')\n\
doc.set_cell_value(3, 11, 'float', '" + ( ( -saldoCuenta ( 708 ) ) + ( -saldoCuenta ( 709 ) ) ).toQString ( '.' ) + "')\n\
doc.set_cell_value(4, 11, 'float', '" + ( ( -saldoCuentaAnt ( 708 ) ) + ( -saldoCuentaAnt ( 709 ) ) ).toQString ( '.' ) + "')\n\
\
doc.set_cell_value(2, 12, 'string', '       2. Aumento de las existencias de productos terminados y en curso de fabricación')\n\
doc.set_cell_value(3, 12, 'float', '" + saldoCuenta ( 71 ).toQString ( '.' ) + "')\n\
doc.set_cell_value(4, 12, 'float', '" + saldoCuentaAnt ( 71 ).toQString ( '.' ) + "')\n\
\
doc.set_cell_value(2, 13, 'string', '       3. Trabajos efectuados por la empresa para el inmobilizado')\n\
doc.set_cell_value(3, 13, 'float', '" + saldoCuenta ( 74 ).toQString ( '.' ) + "')\n\
doc.set_cell_value(4, 13, 'float', '" + saldoCuentaAnt ( 74 ).toQString ( '.' ) + "')\n\
\
doc.set_cell_value(2, 14, 'string', '       4. Otros ingresos de explotación')\n\
doc.set_cell_value(3, 14, 'formula', '=SUM(C15:C17)')\n\
doc.set_cell_value(4, 14, 'formula', '=SUM(D15:D17)')\n\
\
doc.set_cell_value(2, 15, 'string', '            a) Ingresos accesorios y otros de gestión corriente')\n\
doc.set_cell_value(3, 15, 'float', '" + saldoCuenta ( 75 ).toQString ( '.' ) + "')\n\
doc.set_cell_value(4, 15, 'float', '" + saldoCuentaAnt ( 75 ).toQString ( '.' ) + "')\n\
\
doc.set_cell_value(2, 16, 'string', '            b) Subvenciones')\n\
doc.set_cell_value(3, 16, 'float', '" + saldoCuenta ( 74 ).toQString ( '.' ) + "')\n\
doc.set_cell_value(4, 16, 'float', '" + saldoCuentaAnt ( 74 ).toQString ( '.' ) + "')\n\
\
doc.set_cell_value(2, 17, 'string', '            c) Exceso de provisiones de riesgos y gastos')\n\
doc.set_cell_value(3, 17, 'float', '" + saldoCuenta ( 790 ).toQString ( '.' ) + "')\n\
doc.set_cell_value(4, 17, 'float', '" + saldoCuentaAnt ( 790 ).toQString ( '.' ) + "')\n\
\
doc.set_cell_value(2, 19, 'string', '   I.   PÉRDIDAS DE EXPLOTACIÓN')\n\
doc.set_cell_value(2, 20, 'string', '        (A1+A2+A3+A4+A5+A6-B1-B2-B3-B4)')\n\
doc.set_cell_value(3, 19, 'formula', '=Debe.C8+Debe.C9+Debe.C13+Debe.C16+Debe.C17+Debe.C21-C8-C12-C13-C14')\n\
doc.set_cell_value(4, 19, 'formula', '=Debe.D8+Debe.D9+Debe.D13+Debe.D16+Debe.D17+Debe.D21-D8-D12-D13-D14')\n\
\
doc.set_cell_value(2, 22, 'string', '       5. Ingresos de participaciones en capital')\n\
doc.set_cell_value(3, 22, 'formula', '=SUM(C23:C25)')\n\
doc.set_cell_value(4, 22, 'formula', '=SUM(D23:D25)')\n\
\
doc.set_cell_value(2, 23, 'string', '            a) En empresas del grupo')\n\
doc.set_cell_value(3, 23, 'float', '" + saldoCuenta ( 7600 ).toQString ( '.' ) + "')\n\
doc.set_cell_value(4, 23, 'float', '" + saldoCuentaAnt ( 7600 ).toQString ( '.' ) + "')\n\
\
doc.set_cell_value(2, 24, 'string', '            b) En empresas asociadas')\n\
doc.set_cell_value(3, 24, 'float', '" + saldoCuenta ( 7601 ).toQString ( '.' ) + "')\n\
doc.set_cell_value(4, 24, 'float', '" + saldoCuentaAnt ( 7601 ).toQString ( '.' ) + "')\n\
\
doc.set_cell_value(2, 25, 'string', '            c) En empresas fuera del grupo')\n\
doc.set_cell_value(3, 25, 'float', '" + saldoCuenta ( 7603 ).toQString ( '.' ) + "')\n\
doc.set_cell_value(4, 25, 'float', '" + saldoCuentaAnt ( 7603 ).toQString ( '.' ) + "')\n\
\
doc.set_cell_value(2, 26, 'string', '       6. Ingresos de otros valores mobiliarios y de créditos del activo inmovilizado')\n\
doc.set_cell_value(3, 26, 'formula', '=SUM(C27:C29)')\n\
doc.set_cell_value(4, 26, 'formula', '=SUM(D27:D29)')\n\
\
doc.set_cell_value(2, 27, 'string', '            a) De empresas del grupo')\n\
doc.set_cell_value(3, 27, 'float', '" + ( saldoCuenta ( 7610 ) + saldoCuenta ( 7620 ) ).toQString ( '.' ) + "')\n\
doc.set_cell_value(4, 27, 'float', '" + ( saldoCuentaAnt ( 7610 ) + saldoCuentaAnt ( 7620 ) ).toQString ( '.' ) + "')\n\
\
doc.set_cell_value(2, 28, 'string', '            b) De empresas asociadas')\n\
doc.set_cell_value(3, 28, 'float', '" + ( saldoCuenta ( 7611 ) + saldoCuenta ( 7621 ) ).toQString ( '.' ) + "')\n\
doc.set_cell_value(4, 28, 'float', '" + ( saldoCuentaAnt ( 7611 ) + saldoCuentaAnt ( 7621 ) ).toQString ( '.' ) + "')\n\
\
doc.set_cell_value(2, 29, 'string', '            c) De empresas fuera del grupo')\n\
doc.set_cell_value(3, 29, 'float', '" + ( saldoCuenta ( 7613 ) + saldoCuenta ( 7623 ) ).toQString ( '.' ) + "')\n\
doc.set_cell_value(4, 29, 'float', '" + ( saldoCuentaAnt ( 7613 ) + saldoCuentaAnt ( 7623 ) ).toQString ( '.' ) + "')\n\
\
doc.set_cell_value(2, 30, 'string', '       7. Otros intereses e ingresos asimilados')\n\
doc.set_cell_value(3, 30, 'formula', '=SUM(C31:C34)')\n\
doc.set_cell_value(4, 30, 'formula', '=SUM(D31:D34)')\n\
\
doc.set_cell_value(2, 31, 'string', '            a) De empresas del grupo')\n\
doc.set_cell_value(3, 31, 'float', '" + ( saldoCuenta ( 7630 ) + saldoCuenta ( 7650 ) ).toQString ( '.' ) + "')\n\
doc.set_cell_value(4, 31, 'float', '" + ( saldoCuentaAnt ( 7630 ) + saldoCuentaAnt ( 7650 ) ).toQString ( '.' ) + "')\n\
\
doc.set_cell_value(2, 32, 'string', '            b) De empresas asociadas')\n\
doc.set_cell_value(3, 32, 'float', '" + ( saldoCuenta ( 7631 ) + saldoCuenta ( 7651 ) ).toQString ( '.' ) + "')\n\
doc.set_cell_value(4, 32, 'float', '" + ( saldoCuentaAnt ( 7631 ) + saldoCuentaAnt ( 7651 ) ).toQString ( '.' ) + "')\n\
\
doc.set_cell_value(2, 33, 'string', '            c) Otros intereses')\n\
doc.set_cell_value(3, 33, 'float', '" + ( saldoCuenta ( 7633 ) + saldoCuenta ( 7653 ) + saldoCuenta ( 769 ) ).toQString ( '.' ) + "')\n\
doc.set_cell_value(4, 33, 'float', '" + ( saldoCuentaAnt ( 7633 ) + saldoCuentaAnt ( 7653 ) + saldoCuentaAnt ( 769 ) ).toQString ( '.' ) + "')\n\
\
doc.set_cell_value(2, 34, 'string', '            d) Beneficios en inversiones financieras temporales')\n\
doc.set_cell_value(3, 34, 'float', '" + saldoCuenta ( 766 ).toQString ( '.' ) + "')\n\
doc.set_cell_value(4, 34, 'float', '" + saldoCuentaAnt ( 766 ).toQString ( '.' ) + "')\n\
\
doc.set_cell_value(2, 35, 'string', '       8. Diferencias positivas de cambio')\n\
doc.set_cell_value(3, 35, 'float', '" + saldoCuenta ( 768 ).toQString ( '.' ) + "')\n\
doc.set_cell_value(4, 35, 'float', '" + saldoCuentaAnt ( 768 ).toQString ( '.' ) + "')\n\
\
doc.set_cell_value(2, 37, 'string', '   II.  RESULTADOS FINANCIEROS NEGATIVOS')\n\
doc.set_cell_value(2, 38, 'string', '        (A7+A8+A9+A10-B5-B6-B7-B8)')\n\
doc.set_cell_value(3, 37, 'formula', '=Debe.C30+Debe.C35+Debe.C36+Debe.C44-C22-C26-C30-C35')\n\
doc.set_cell_value(4, 37, 'formula', '=Debe.D30+Debe.D35+Debe.D36+Debe.D44-D22-D26-D30-D35')\n\
\
doc.set_cell_value(2, 40, 'string', '   III. PÉRDIDAS DE LAS ACTIVIDADES ORDINARIAS')\n\
doc.set_cell_value(2, 41, 'string', '        (BI+BII-AI-AII)')\n\
doc.set_cell_value(3, 40, 'formula', '=C19+C37-Debe.C27-Debe.C38')\n\
doc.set_cell_value(4, 40, 'formula', '=D19+D37-Debe.D27-Debe.D38')\n\
\
doc.set_cell_value(2, 43, 'string', '       9. Beneficios en enajenación de inmovilizado inmaterial, material y cartera de control')\n\
doc.set_cell_value(3, 43, 'float', '" + ( saldoCuenta ( 770 ) + saldoCuenta ( 771 ) + saldoCuenta ( 772 ) + saldoCuenta ( 773 ) ).toQString ( '.' ) + "')\n\
doc.set_cell_value(4, 43, 'float', '" + ( saldoCuentaAnt ( 770 ) + saldoCuentaAnt ( 771 ) + saldoCuentaAnt ( 772 ) + saldoCuentaAnt ( 773 ) ).toQString ( '.' ) + "')\n\
\
doc.set_cell_value(2, 44, 'string', '      10. Beneficios por operaciones con acciones y obligaciones propias')\n\
doc.set_cell_value(3, 44, 'float', '" + saldoCuenta ( 774 ).toQString ( '.' ) + "')\n\
doc.set_cell_value(4, 44, 'float', '" + saldoCuentaAnt ( 774 ).toQString ( '.' ) + "')\n\
\
doc.set_cell_value(2, 45, 'string', '      11. Subvenciones de capital transferidas al resultado del ejercicio')\n\
doc.set_cell_value(3, 45, 'float', '" + saldoCuenta ( 775 ).toQString ( '.' ) + "')\n\
doc.set_cell_value(4, 45, 'float', '" + saldoCuentaAnt ( 775 ).toQString ( '.' ) + "')\n\
\
doc.set_cell_value(2, 46, 'string', '      12. Ingresos extraordinarios')\n\
doc.set_cell_value(3, 46, 'float', '" + saldoCuenta ( 778 ).toQString ( '.' ) + "')\n\
doc.set_cell_value(4, 46, 'float', '" + saldoCuentaAnt ( 778 ).toQString ( '.' ) + "')\n\
\
doc.set_cell_value(2, 47, 'string', '      13. Ingresos y beneficios de otros ejercicios')\n\
doc.set_cell_value(3, 47, 'float', '" + saldoCuenta ( 779 ).toQString ( '.' ) + "')\n\
doc.set_cell_value(4, 47, 'float', '" + saldoCuentaAnt ( 779 ).toQString ( '.' ) + "')\n\
\
doc.set_cell_value(2, 49, 'string', '   IV. RESULTADOS EXTRAORDINARIOS NEGATIVOS')\n\
doc.set_cell_value(2, 50, 'string', '       (A11+A12+A13+A14+A15-B9-B10-B11-B12-B13)')\n\
doc.set_cell_value(3, 49, 'formula', '=Debe.C45+Debe.C46+Debe.C47+Debe.C56-C43-C44-C45-C46-C47')\n\
doc.set_cell_value(4, 49, 'formula', '=Debe.D45+Debe.D46+Debe.D47+Debe.D56-D43-D44-D45-D46-D47')\n\
\
doc.set_cell_value(2, 52, 'string', '   V.  PÉRDIDAS ANTES DE IMPUESTOS')\n\
doc.set_cell_value(2, 53, 'string', '       (BIII+BIV-AIII-AIV)')\n\
doc.set_cell_value(3, 52, 'formula', '=C40+C49-Debe.C41-Debe.C50')\n\
doc.set_cell_value(4, 52, 'formula', '=D40+D49-Debe.D41-Debe.D50')\n\
\
doc.set_cell_value(2, 55, 'string', '   VI. RESULTADO DEL EJERCICIO (PÉRDIDAS)')\n\
doc.set_cell_value(2, 56, 'string', '       (BV+A15+A16)')\n\
doc.set_cell_value(3, 55, 'formula', '=C52+Debe.C56+Debe.C57')\n\
doc.set_cell_value(4, 55, 'formula', '=D52+Debe.D56+Debe.D57')\n\
\
\
# Guardamos el documento\n\
doc.save(\"canualesods.ods\")\n");
}


///
/**
**/

QString pluginReportODS::cuentaAnualCAPYMES08()
{
	return QString::QString("\
#!/usr/bin/python\n\
# -*- coding: utf8 -*-\n\
\n\
import ooolib\n\
\n\
# Crea el documento\n\
\
\
\n\
doc = ooolib.Calc(\""+ trUtf8("ACTIVO")+ "\")\n\n\
doc.set_column_property(1, 'width', '0.5in')\n\
doc.set_column_property(2, 'width', '5in')\n\
\
doc.set_cell_property('bold', True)\n\
doc.set_cell_value(2,2, 'string', '" + trUtf8("BALANCE DE PYMES AL CIERRE DEL EJERCICIO") + "')\n\
doc.set_cell_value(2,4, 'string', 'ACTIVO')\n\
doc.set_cell_value(3,4, 'string', 'NOTAS')\n\
doc.set_cell_value(4,4, 'string', 'Ejercicio N')\n\
doc.set_cell_value(5,4, 'string', 'Ejercicio N-1')\n\
doc.set_cell_property('bold', False)\n\
\
\
\
doc.set_cell_property('bold', True)\n\
doc.set_cell_value(2,6, 'string', 'A) ACTIVO NO CORRIENTE')\n\
doc.set_cell_value(4,6, 'formula', '=D8+D9+D10+D11+D12+D13')\n\
doc.set_cell_value(5,6, 'formula', '=E8+E9+E10+E11+E12+E13')\n\
doc.set_cell_property('bold', False)\n\
\
doc.set_cell_property('bold', True)\n\
doc.set_cell_value(2,8, 'string', 'I. Inmovilizado intangible.')\n\
doc.set_cell_value(4,8, 'float', '" + ( saldoCuenta (20 ) + saldoCuenta (280 ) + saldoCuenta (290)).toQString ( '.' ) + "')\n\
doc.set_cell_value(5,8, 'float', '"+ ( saldoCuentaAnt (20 ) + saldoCuentaAnt (280 ) + saldoCuentaAnt (290)).toQString ( '.' ) + "')\n\
doc.set_cell_property('bold', False)\n\
\
doc.set_cell_property('bold', True)\n\
doc.set_cell_value(2,9, 'string', 'II. Inmovilizado material.')\n\
doc.set_cell_value(4,9, 'float', '" + ( saldoCuenta (21 ) + saldoCuenta (281 ) + saldoCuenta (291 ) + saldoCuenta (23)).toQString ( '.' ) + "')\n\
doc.set_cell_value(5,9, 'float', '"+ ( saldoCuentaAnt (21 ) + saldoCuentaAnt (281 ) + saldoCuentaAnt (291 ) + saldoCuentaAnt (23)).toQString ( '.' ) + "')\n\
doc.set_cell_property('bold', False)\n\
\
doc.set_cell_property('bold', True)\n\
doc.set_cell_value(2,10, 'string', 'III. Inversiones inmobiliarias.')\n\
doc.set_cell_value(4,10, 'float', '" + ( saldoCuenta (22 ) + saldoCuenta (282 ) + saldoCuenta (292)).toQString ( '.' ) + "')\n\
doc.set_cell_value(5,10, 'float', '"+ ( saldoCuentaAnt (22 ) + saldoCuentaAnt (282 ) + saldoCuentaAnt (292)).toQString ( '.' ) + "')\n\
doc.set_cell_property('bold', False)\n\
\
doc.set_cell_property('bold', True)\n\
doc.set_cell_value(2,11, 'string', 'IV. Inversiones en empresas del grupo y asociadas a largo plazo.')\n\
doc.set_cell_value(4,11, 'float', '" + ( saldoCuenta (2403 ) + saldoCuenta (2404 ) + saldoCuenta (2413 ) + saldoCuenta (2414 ) + saldoCuenta (2423 ) + saldoCuenta (2424 ) + saldoCuenta (2493 ) + saldoCuenta (2494 ) + saldoCuenta (2933 ) + saldoCuenta (2934 ) + saldoCuenta (2943 ) + saldoCuenta (2944 ) + saldoCuenta (2953 ) + saldoCuenta (2954)).toQString ( '.' ) + "')\n\
doc.set_cell_value(5,11, 'float', '"+ ( saldoCuentaAnt (2403 ) + saldoCuentaAnt (2404 ) + saldoCuentaAnt (2413 ) + saldoCuentaAnt (2414 ) + saldoCuentaAnt (2423 ) + saldoCuentaAnt (2424 ) + saldoCuentaAnt (2493 ) + saldoCuentaAnt (2494 ) + saldoCuentaAnt (2933 ) + saldoCuentaAnt (2934 ) + saldoCuentaAnt (2943 ) + saldoCuentaAnt (2944 ) + saldoCuentaAnt (2953 ) + saldoCuentaAnt (2954)).toQString ( '.' ) + "')\n\
doc.set_cell_property('bold', False)\n\
\
doc.set_cell_property('bold', True)\n\
doc.set_cell_value(2,12, 'string', 'V. Inversiones financieras a largo plazo.')\n\
doc.set_cell_value(4,12, 'float', '" + ( saldoCuenta (2405 ) + saldoCuenta (2415 ) + saldoCuenta (2425 ) + saldoCuenta (2495 ) + saldoCuenta (250 ) + saldoCuenta (251 ) + saldoCuenta (252 ) + saldoCuenta (253 ) + saldoCuenta (254 ) + saldoCuenta (255 ) + saldoCuenta (258 ) + saldoCuenta (259 ) + saldoCuenta (26 ) + saldoCuenta (2935 ) + saldoCuenta (2945 ) + saldoCuenta (2955 ) + saldoCuenta (296 ) + saldoCuenta (297 ) + saldoCuenta (298)).toQString ( '.' ) + "')\n\
doc.set_cell_value(5,12, 'float', '"+ ( saldoCuentaAnt (2405 ) + saldoCuentaAnt (2415 ) + saldoCuentaAnt (2425 ) + saldoCuentaAnt (2495 ) + saldoCuentaAnt (250 ) + saldoCuentaAnt (251 ) + saldoCuentaAnt (252 ) + saldoCuentaAnt (253 ) + saldoCuentaAnt (254 ) + saldoCuentaAnt (255 ) + saldoCuentaAnt (258 ) + saldoCuentaAnt (259 ) + saldoCuentaAnt (26 ) + saldoCuentaAnt (2935 ) + saldoCuentaAnt (2945 ) + saldoCuentaAnt (2955 ) + saldoCuentaAnt (296 ) + saldoCuentaAnt (297 ) + saldoCuentaAnt (298)).toQString ( '.' ) + "')\n\
doc.set_cell_property('bold', False)\n\
\
doc.set_cell_property('bold', True)\n\
doc.set_cell_value(2,13, 'string', 'VI. Activos por Impuesto diferido.')\n\
doc.set_cell_value(4,13, 'float', '" + ( saldoCuenta (474)).toQString ( '.' ) + "')\n\
doc.set_cell_value(5,13, 'float', '"+ ( saldoCuentaAnt (474)).toQString ( '.' ) + "')\n\
doc.set_cell_property('bold', False)\n\
\
\
\
\
doc.set_cell_property('bold', True)\n\
doc.set_cell_value(2,15, 'string', 'B) ACTIVO CORRIENTE')\n\
doc.set_cell_value(4,15, 'formula', '=D17+D18+D22+D23+D24+D25')\n\
doc.set_cell_value(5,15, 'formula', '=E17+E18+E22+E23+E24+E25')\n\
doc.set_cell_property('bold', False)\n\
\
\
\
doc.set_cell_property('bold', True)\n\
doc.set_cell_value(2,17, 'string', 'I. Existencias.')\n\
doc.set_cell_value(4,17, 'float', '" + ( saldoCuenta (30 ) + saldoCuenta (31 ) + saldoCuenta (32 ) + saldoCuenta (33 ) + saldoCuenta (34 ) + saldoCuenta (35 ) + saldoCuenta (36 ) + saldoCuenta (39 ) + saldoCuenta (407)).toQString ( '.' ) + "')\n\
doc.set_cell_value(5,17, 'float', '"+ ( saldoCuentaAnt (30 ) + saldoCuentaAnt (31 ) + saldoCuentaAnt (32 ) + saldoCuentaAnt (33 ) + saldoCuentaAnt (34 ) + saldoCuentaAnt (35 ) + saldoCuentaAnt (36 ) + saldoCuentaAnt (39 ) + saldoCuentaAnt (407)).toQString ( '.' ) + "')\n\
doc.set_cell_property('bold', False)\n\
\
doc.set_cell_property('bold', True)\n\
doc.set_cell_value(2,18, 'string', 'II. Deudores comerciales y otras cuentas a cobrar.')\n\
doc.set_cell_value(4,18, 'formula', '=D19+D20+D21')\n\
doc.set_cell_value(5,18, 'formula', '=E19+E20+E21')\n\
doc.set_cell_property('bold', False)\n\
\
doc.set_cell_value(2,19, 'string', '1. Clientes por ventas y Prestaciones de servicios.')\n\
doc.set_cell_value(4,19, 'float', '" + ( saldoCuenta (430 ) + saldoCuenta (431 ) + saldoCuenta (432 ) + saldoCuenta (433 ) + saldoCuenta (434 ) + saldoCuenta (435 ) + saldoCuenta (436 ) + saldoCuenta (437 ) + saldoCuenta (490 ) + saldoCuenta (493)).toQString ( '.' ) + "')\n\
doc.set_cell_value(5,19, 'float', '"+ ( saldoCuentaAnt (430 ) + saldoCuentaAnt (431 ) + saldoCuentaAnt (432 ) + saldoCuentaAnt (433 ) + saldoCuentaAnt (434 ) + saldoCuentaAnt (435 ) + saldoCuentaAnt (436 ) + saldoCuentaAnt (437 ) + saldoCuentaAnt (490 ) + saldoCuentaAnt (493)).toQString ( '.' ) + "')\n\
\
doc.set_cell_value(2,20, 'string', '2. Accionistas (socios) por desembolsos exigidos.')\n\
doc.set_cell_value(4,20, 'float', '" + ( saldoCuenta (5580)).toQString ( '.' ) + "')\n\
doc.set_cell_value(5,20, 'float', '"+ ( saldoCuentaAnt (5580)).toQString ( '.' ) + "')\n\
\
doc.set_cell_value(2,21, 'string', '3. Otros deudores.')\n\
doc.set_cell_value(4,21, 'float', '" + ( saldoCuenta (44 ) + saldoCuenta (460 ) + saldoCuenta (470 ) + saldoCuenta (471 ) + saldoCuenta (472 ) + saldoCuenta (544)).toQString ( '.' ) + "')\n\
doc.set_cell_value(5,21, 'float', '"+ ( saldoCuentaAnt (44 ) + saldoCuentaAnt (460 ) + saldoCuentaAnt (470 ) + saldoCuentaAnt (471 ) + saldoCuentaAnt (472 ) + saldoCuentaAnt (544)).toQString ( '.' ) + "')\n\
\
doc.set_cell_property('bold', True)\n\
doc.set_cell_value(2,22, 'string', 'III. Inversiones en empresas del grupo y asociadas a corto plazo.')\n\
doc.set_cell_value(4,22, 'float', '" + ( saldoCuenta (5303 ) + saldoCuenta (5304 ) + saldoCuenta (5313 ) + saldoCuenta (5314 ) + saldoCuenta (5323 ) + saldoCuenta (5324 ) + saldoCuenta (5333 ) + saldoCuenta (5334 ) + saldoCuenta (5343 ) + saldoCuenta (5344 ) + saldoCuenta (5353 ) + saldoCuenta (5354 ) + saldoCuenta (5393 ) + saldoCuenta (5394 ) + saldoCuenta (5523 ) + saldoCuenta (5524 ) + saldoCuenta (5933 ) + saldoCuenta (5934 ) + saldoCuenta (5943 ) + saldoCuenta (5944 ) + saldoCuenta (5953 ) + saldoCuenta (5954)).toQString ( '.' ) + "')\n\
doc.set_cell_value(5,22, 'float', '"+ ( saldoCuentaAnt (5303 ) + saldoCuentaAnt (5304 ) + saldoCuentaAnt (5313 ) + saldoCuentaAnt (5314 ) + saldoCuentaAnt (5323 ) + saldoCuentaAnt (5324 ) + saldoCuentaAnt (5333 ) + saldoCuentaAnt (5334 ) + saldoCuentaAnt (5343 ) + saldoCuentaAnt (5344 ) + saldoCuentaAnt (5353 ) + saldoCuentaAnt (5354 ) + saldoCuentaAnt (5393 ) + saldoCuentaAnt (5394 ) + saldoCuentaAnt (5523 ) + saldoCuentaAnt (5524 ) + saldoCuentaAnt (5933 ) + saldoCuentaAnt (5934 ) + saldoCuentaAnt (5943 ) + saldoCuentaAnt (5944 ) + saldoCuentaAnt (5953 ) + saldoCuentaAnt (5954)).toQString ( '.' ) + "')\n\
doc.set_cell_property('bold', False)\n\
\
doc.set_cell_property('bold', True)\n\
doc.set_cell_value(2,23, 'string', 'IV. Inversiones financieras a corto plazo.')\n\
doc.set_cell_value(4,23, 'float', '" + ( saldoCuenta (5305 ) + saldoCuenta (5315 ) + saldoCuenta (5325 ) + saldoCuenta (5335 ) + saldoCuenta (5345 ) + saldoCuenta (5355 ) + saldoCuenta (5395 ) + saldoCuenta (540 ) + saldoCuenta (541 ) + saldoCuenta (542 ) + saldoCuenta (543 ) + saldoCuenta (545 ) + saldoCuenta (546 ) + saldoCuenta (547 ) + saldoCuenta (548 ) + saldoCuenta (549 ) + saldoCuenta (551 ) + saldoCuenta (5525 ) + saldoCuenta (5590 ) + saldoCuenta (565 ) + saldoCuenta (566 ) + saldoCuenta (5935 ) + saldoCuenta (5945 ) + saldoCuenta (5955 ) + saldoCuenta (596 ) + saldoCuenta (597 ) + saldoCuenta (598)).toQString ( '.' ) + "')\n\
doc.set_cell_value(5,23, 'float', '"+ ( saldoCuentaAnt (5305 ) + saldoCuentaAnt (5315 ) + saldoCuentaAnt (5325 ) + saldoCuentaAnt (5335 ) + saldoCuentaAnt (5345 ) + saldoCuentaAnt (5355 ) + saldoCuentaAnt (5395 ) + saldoCuentaAnt (540 ) + saldoCuentaAnt (541 ) + saldoCuentaAnt (542 ) + saldoCuentaAnt (543 ) + saldoCuentaAnt (545 ) + saldoCuentaAnt (546 ) + saldoCuentaAnt (547 ) + saldoCuentaAnt (548 ) + saldoCuentaAnt (549 ) + saldoCuentaAnt (551 ) + saldoCuentaAnt (5525 ) + saldoCuentaAnt (5590 ) + saldoCuentaAnt (565 ) + saldoCuentaAnt (566 ) + saldoCuentaAnt (5935 ) + saldoCuentaAnt (5945 ) + saldoCuentaAnt (5955 ) + saldoCuentaAnt (596 ) + saldoCuentaAnt (597 ) + saldoCuentaAnt (598)).toQString ( '.' ) + "')\n\
doc.set_cell_property('bold', False)\n\
\
doc.set_cell_property('bold', True)\n\
doc.set_cell_value(2,24, 'string', 'V. Periodificaciones a corto plazo.')\n\
doc.set_cell_value(4,24, 'float', '" + ( saldoCuenta (480 ) + saldoCuenta (567)).toQString ( '.' ) + "')\n\
doc.set_cell_value(5,24, 'float', '"+ ( saldoCuentaAnt (480 ) + saldoCuentaAnt (567)).toQString ( '.' ) + "')\n\
doc.set_cell_property('bold', False)\n\
\
doc.set_cell_property('bold', True)\n\
doc.set_cell_value(2,25, 'string', 'VI. Efectivo y otros activos líquidos equivalentes.')\n\
doc.set_cell_value(4,25, 'float', '" + ( saldoCuenta (57)).toQString ( '.' ) + "')\n\
doc.set_cell_value(5,25, 'float', '"+ ( saldoCuentaAnt (57)).toQString ( '.' ) + "')\n\
doc.set_cell_property('bold', False)\n\
\
\
doc.set_cell_property('bold', True)\n\
doc.set_cell_value(2,27, 'string', 'TOTAL ACTIVO (A+B)')\n\
doc.set_cell_value(4,27, 'formula', '=D6+D15')\n\
doc.set_cell_value(5,27, 'formula', '=E6+E15')\n\
doc.set_cell_property('bold', False)\n\
\
\
\
\
\
\
\
doc.new_sheet(\"PATRIMONIO NETO Y PASIVO\")\n\n\
doc.set_column_property(1, 'width', '0.5in')\n\
doc.set_column_property(2, 'width', '5in')\n\
\
doc.set_cell_property('bold', True)\n\
doc.set_cell_value(2,2, 'string', 'BALANCE DE PYMES AL CIERRE DEL EJERCICIO')\n\
doc.set_cell_value(2,4, 'string', 'PATRIMONIO NETO Y PASIVO')\n\
doc.set_cell_value(3,4, 'string', 'NOTAS')\n\
doc.set_cell_value(4,4, 'string', 'Ejercicio N')\n\
doc.set_cell_value(5,4, 'string', 'Ejercicio N-1')\n\
doc.set_cell_property('bold', False)\n\
\
\
\
doc.set_cell_property('bold', True)\n\
doc.set_cell_value(2,6, 'string', 'A) PATRIMONIO NETO')\n\
doc.set_cell_value(4,6, 'formula', '=D8+D19')\n\
doc.set_cell_value(5,6, 'formula', '=E8+E19')\n\
doc.set_cell_property('bold', False)\n\
\
\
doc.set_cell_property('bold', True)\n\
doc.set_cell_value(2,8, 'string', 'A-1) Fondos propios.')\n\
doc.set_cell_value(4,8, 'formula', '=D9+D12+D13+D14+D15+D16+D17+D18')\n\
doc.set_cell_value(5,8, 'formula', '=E9+E12+E13+E14+E15+E16+E17+E18')\n\
doc.set_cell_property('bold', False)\n\
\
doc.set_cell_property('bold', True)\n\
doc.set_cell_value(2,9, 'string', 'I. Capital.')\n\
doc.set_cell_value(4,9, 'formula', '=D10+D11')\n\
doc.set_cell_value(5,9, 'formula', '=E10+E11')\n\
doc.set_cell_property('bold', False)\n\
\
doc.set_cell_value(2,10, 'string', '1. Capital escriturado.')\n\
doc.set_cell_value(4,10, 'float', '" + ( saldoCuenta (100 ) + saldoCuenta (101 ) + saldoCuenta (102)).toQString ( '.' ) + "')\n\
doc.set_cell_value(5,10, 'float', '"+ ( saldoCuentaAnt (100 ) + saldoCuentaAnt (101 ) + saldoCuentaAnt (102)).toQString ( '.' ) + "')\n\
\
doc.set_cell_value(2,11, 'string', '2. (Capital no exigido).')\n\
doc.set_cell_value(4,11, 'float', '" + ( saldoCuenta (1030 ) + saldoCuenta (1040)).toQString ( '.' ) + "')\n\
doc.set_cell_value(5,11, 'float', '"+ ( saldoCuentaAnt (1030 ) + saldoCuentaAnt (1040)).toQString ( '.' ) + "')\n\
\
doc.set_cell_property('bold', True)\n\
doc.set_cell_value(2,12, 'string', 'II. Prima de emisión.')\n\
doc.set_cell_value(4,12, 'float', '" + ( saldoCuenta (110)).toQString ( '.' ) + "')\n\
doc.set_cell_value(5,12, 'float', '"+ ( saldoCuentaAnt (110)).toQString ( '.' ) + "')\n\
doc.set_cell_property('bold', False)\n\
\
doc.set_cell_property('bold', True)\n\
doc.set_cell_value(2,13, 'string', 'III. Reservas.')\n\
doc.set_cell_value(4,13, 'float', '" + ( saldoCuenta (112 ) + saldoCuenta (113 ) + saldoCuenta (114 ) + saldoCuenta (119)).toQString ( '.' ) + "')\n\
doc.set_cell_value(5,13, 'float', '"+ ( saldoCuentaAnt (112 ) + saldoCuentaAnt (113 ) + saldoCuentaAnt (114 ) + saldoCuentaAnt (119)).toQString ( '.' ) + "')\n\
doc.set_cell_property('bold', False)\n\
\
doc.set_cell_property('bold', True)\n\
doc.set_cell_value(2,14, 'string', 'IV. (Acciones y participaciones en patrimonio propias).')\n\
doc.set_cell_value(4,14, 'float', '" + ( saldoCuenta (108 ) + saldoCuenta (109)).toQString ( '.' ) + "')\n\
doc.set_cell_value(5,14, 'float', '"+ ( saldoCuentaAnt (108 ) + saldoCuentaAnt (109)).toQString ( '.' ) + "')\n\
doc.set_cell_property('bold', False)\n\
\
doc.set_cell_property('bold', True)\n\
doc.set_cell_value(2,15, 'string', 'V. Resultados de ejercicios anteriores.')\n\
doc.set_cell_value(4,15, 'float', '" + ( saldoCuenta (120 ) + saldoCuenta (121)).toQString ( '.' ) + "')\n\
doc.set_cell_value(5,15, 'float', '"+ ( saldoCuentaAnt (120 ) + saldoCuentaAnt (121)).toQString ( '.' ) + "')\n\
doc.set_cell_property('bold', False)\n\
\
doc.set_cell_property('bold', True)\n\
doc.set_cell_value(2,16, 'string', 'VI. Otras aportaciones de socios.')\n\
doc.set_cell_value(4,16, 'float', '" + ( saldoCuenta (118)).toQString ( '.' ) + "')\n\
doc.set_cell_value(5,16, 'float', '"+ ( saldoCuentaAnt (118)).toQString ( '.' ) + "')\n\
doc.set_cell_property('bold', False)\n\
\
doc.set_cell_property('bold', True)\n\
doc.set_cell_value(2,17, 'string', 'VII. Resultado del ejercicio.')\n\
doc.set_cell_value(4,17, 'float', '" + ( saldoCuenta (129)).toQString ( '.' ) + "')\n\
doc.set_cell_value(5,17, 'float', '"+ ( saldoCuentaAnt (129)).toQString ( '.' ) + "')\n\
doc.set_cell_property('bold', False)\n\
\
doc.set_cell_property('bold', True)\n\
doc.set_cell_value(2,18, 'string', 'VIII. (Dividendo a cuenta).')\n\
doc.set_cell_value(4,18, 'float', '" + ( saldoCuenta (557)).toQString ( '.' ) + "')\n\
doc.set_cell_value(5,18, 'float', '"+ ( saldoCuentaAnt (557)).toQString ( '.' ) + "')\n\
doc.set_cell_property('bold', False)\n\
\
doc.set_cell_property('bold', True)\n\
doc.set_cell_value(2,19, 'string', 'A-2) Subvenciones, donaciones y legados recibidos.')\n\
doc.set_cell_value(4,19, 'float', '" + ( saldoCuenta (130 ) + saldoCuenta (131 ) + saldoCuenta (132)).toQString ( '.' ) + "')\n\
doc.set_cell_value(5,19, 'float', '"+ ( saldoCuentaAnt (130 ) + saldoCuentaAnt (131 ) + saldoCuentaAnt (132)).toQString ( '.' ) + "')\n\
doc.set_cell_property('bold', False)\n\
\
\
doc.set_cell_property('bold', True)\n\
doc.set_cell_value(2,21, 'string', 'B) PASIVO NO CORRIENTE')\n\
doc.set_cell_value(4,21, 'formula', '=D23+D24+D28+D29+D30')\n\
doc.set_cell_value(5,21, 'formula', '=E23+E24+E28+E29+E30')\n\
doc.set_cell_property('bold', False)\n\
\
\
doc.set_cell_property('bold', True)\n\
doc.set_cell_value(2,23, 'string', 'I. Provisiones a largo plazo.')\n\
doc.set_cell_value(4,23, 'float', '" + ( saldoCuenta (14)).toQString ( '.' ) + "')\n\
doc.set_cell_value(5,23, 'float', '"+ ( saldoCuentaAnt (14)).toQString ( '.' ) + "')\n\
doc.set_cell_property('bold', False)\n\
\
doc.set_cell_property('bold', True)\n\
doc.set_cell_value(2,24, 'string', 'II. Deudas a largo plazo.')\n\
doc.set_cell_value(4,24, 'formula', '=D25+D26+D27')\n\
doc.set_cell_value(5,24, 'formula', '=E25+E26+E27')\n\
doc.set_cell_property('bold', False)\n\
\
doc.set_cell_value(2,25, 'string', '1. Deudas con entidades de crédito.')\n\
doc.set_cell_value(4,25, 'float', '" + ( saldoCuenta (1605 ) + saldoCuenta (170)).toQString ( '.' ) + "')\n\
doc.set_cell_value(5,25, 'float', '"+ ( saldoCuentaAnt (1605 ) + saldoCuentaAnt (170)).toQString ( '.' ) + "')\n\
\
doc.set_cell_value(2,26, 'string', '2. Acreedores por arrendamiento financiero')\n\
doc.set_cell_value(4,26, 'float', '" + ( saldoCuenta (1625 ) + saldoCuenta (174)).toQString ( '.' ) + "')\n\
doc.set_cell_value(5,26, 'float', '"+ ( saldoCuentaAnt (1625 ) + saldoCuentaAnt (174)).toQString ( '.' ) + "')\n\
\
doc.set_cell_value(2,27, 'string', '3. Otras deudas a largo plazo.')\n\
doc.set_cell_value(4,27, 'float', '" + ( saldoCuenta (1615 ) + saldoCuenta (1635 ) + saldoCuenta (171 ) + saldoCuenta (172 ) + saldoCuenta (173 ) + saldoCuenta (175 ) + saldoCuenta (176 ) + saldoCuenta (177 ) + saldoCuenta (179 ) + saldoCuenta (180 ) + saldoCuenta (185)).toQString ( '.' ) + "')\n\
doc.set_cell_value(5,27, 'float', '"+ ( saldoCuentaAnt (1615 ) + saldoCuentaAnt (1635 ) + saldoCuentaAnt (171 ) + saldoCuentaAnt (172 ) + saldoCuentaAnt (173 ) + saldoCuentaAnt (175 ) + saldoCuentaAnt (176 ) + saldoCuentaAnt (177 ) + saldoCuentaAnt (179 ) + saldoCuentaAnt (180 ) + saldoCuentaAnt (185)).toQString ( '.' ) + "')\n\
\
doc.set_cell_property('bold', True)\n\
doc.set_cell_value(2,28, 'string', 'III. Deudas con empresas del grupo y asociadas a largo plazo.')\n\
doc.set_cell_value(4,28, 'float', '" + ( saldoCuenta (1603 ) + saldoCuenta (1604 ) + saldoCuenta (1613 ) + saldoCuenta (1614 ) + saldoCuenta (1623 ) + saldoCuenta (1624 ) + saldoCuenta (1633 ) + saldoCuenta (1634)).toQString ( '.' ) + "')\n\
doc.set_cell_value(5,28, 'float', '"+ ( saldoCuentaAnt (1603 ) + saldoCuentaAnt (1604 ) + saldoCuentaAnt (1613 ) + saldoCuentaAnt (1614 ) + saldoCuentaAnt (1623 ) + saldoCuentaAnt (1624 ) + saldoCuentaAnt (1633 ) + saldoCuentaAnt (1634)).toQString ( '.' ) + "')\n\
doc.set_cell_property('bold', False)\n\
\
doc.set_cell_property('bold', True)\n\
doc.set_cell_value(2,29, 'string', 'IV. Pasivos por impuesto diferido')\n\
doc.set_cell_value(4,29, 'float', '" + ( saldoCuenta (479)).toQString ( '.' ) + "')\n\
doc.set_cell_value(5,29, 'float', '"+ ( saldoCuentaAnt (479)).toQString ( '.' ) + "')\n\
doc.set_cell_property('bold', False)\n\
\
doc.set_cell_property('bold', True)\n\
doc.set_cell_value(2,30, 'string', 'V. Periodificaciones a largo plazo')\n\
doc.set_cell_value(4,30, 'float', '" + ( saldoCuenta (181)).toQString ( '.' ) + "')\n\
doc.set_cell_value(5,30, 'float', '"+ ( saldoCuentaAnt (181)).toQString ( '.' ) + "')\n\
doc.set_cell_property('bold', False)\n\
\
doc.set_cell_property('bold', True)\n\
doc.set_cell_value(2,32, 'string', 'C) PASIVO CORRIENTE')\n\
doc.set_cell_value(4,32, 'formula', '=D34+D35+D39+D40+D43')\n\
doc.set_cell_value(5,32, 'formula', '=E34+E35+E39+E40+E43')\n\
doc.set_cell_property('bold', False)\n\
\
doc.set_cell_property('bold', True)\n\
doc.set_cell_value(2,34, 'string', 'I. Provisiones a corto plazo.')\n\
doc.set_cell_value(4,34, 'float', '" + ( saldoCuenta (499 ) + saldoCuenta (529)).toQString ( '.' ) + "')\n\
doc.set_cell_value(5,34, 'float', '"+ ( saldoCuentaAnt (499 ) + saldoCuentaAnt (529)).toQString ( '.' ) + "')\n\
doc.set_cell_property('bold', False)\n\
\
doc.set_cell_property('bold', True)\n\
doc.set_cell_value(2,35, 'string', 'II. Deudas a corto plazo.')\n\
doc.set_cell_value(4,35, 'formula', '=D36+D37+D38')\n\
doc.set_cell_value(5,35, 'formula', '=E36+E37+E38')\n\
doc.set_cell_property('bold', False)\n\
\
doc.set_cell_value(2,36, 'string', '1. Deudas con entidades de crédito.')\n\
doc.set_cell_value(4,36, 'float', '" + ( saldoCuenta (5105) + saldoCuenta(520) + saldoCuenta(527) ).toQString ( '.' ) + "')\n\
doc.set_cell_value(5,36, 'float', '"+ ( saldoCuentaAnt (5105) + saldoCuentaAnt(520) + saldoCuentaAnt(527)).toQString ( '.' ) + "')\n\
\
doc.set_cell_value(2,37, 'string', '2. Acreedores por arrendamiento financiero')\n\
doc.set_cell_value(4,37, 'float', '" + ( saldoCuenta (5125 ) + saldoCuenta (524)).toQString ( '.' ) + "')\n\
doc.set_cell_value(5,37, 'float', '"+ ( saldoCuentaAnt (5125 ) + saldoCuentaAnt (524)).toQString ( '.' ) + "')\n\
\
doc.set_cell_value(2,38, 'string', '3. Otras deudas a corto plazo.')\n\
doc.set_cell_value(4,38, 'float', '" + ( saldoCuenta (1034 ) + saldoCuenta (1044 ) + saldoCuenta (190 ) + saldoCuenta (192 ) + saldoCuenta (194 ) + saldoCuenta (500 ) + saldoCuenta (505 ) + saldoCuenta (506 ) + saldoCuenta (509 ) + saldoCuenta (5115 ) + saldoCuenta (5135 ) + saldoCuenta (51 ) + saldoCuenta (45 ) + saldoCuenta (521 ) + saldoCuenta (522 ) + saldoCuenta (523 ) + saldoCuenta (525 ) + saldoCuenta (526 ) + saldoCuenta (528 ) + saldoCuenta (551 ) + saldoCuenta (5525 ) + saldoCuenta (555 ) + saldoCuenta (5565 ) + saldoCuenta (5566 ) + saldoCuenta (5595 ) + saldoCuenta (560 ) + saldoCuenta (561)).toQString ( '.' ) + "')\n\
doc.set_cell_value(5,38, 'float', '"+ ( saldoCuentaAnt (1034 ) + saldoCuentaAnt (1044 ) + saldoCuentaAnt (190 ) + saldoCuentaAnt (192 ) + saldoCuentaAnt (194 ) + saldoCuentaAnt (500 ) + saldoCuentaAnt (505 ) + saldoCuentaAnt (506 ) + saldoCuentaAnt (509 ) + saldoCuentaAnt (5115 ) + saldoCuentaAnt (5135 ) + saldoCuentaAnt (51 ) + saldoCuentaAnt (45 ) + saldoCuentaAnt (521 ) + saldoCuentaAnt (522 ) + saldoCuentaAnt (523 ) + saldoCuentaAnt (525 ) + saldoCuentaAnt (526 ) + saldoCuentaAnt (528 ) + saldoCuentaAnt (551 ) + saldoCuentaAnt (5525 ) + saldoCuentaAnt (555 ) + saldoCuentaAnt (5565 ) + saldoCuentaAnt (5566 ) + saldoCuentaAnt (5595 ) + saldoCuentaAnt (560 ) + saldoCuentaAnt (561)).toQString ( '.' ) + "')\n\
\
doc.set_cell_property('bold', True)\n\
doc.set_cell_value(2,39, 'string', 'III. Deudas con empresas del grupo y asociadas a corto plazo.')\n\
doc.set_cell_value(4,39, 'float', '" + ( saldoCuenta (5103 ) + saldoCuenta (5104 ) + saldoCuenta (5113 ) + saldoCuenta (5114 ) + saldoCuenta (5123 ) + saldoCuenta (5124 ) + saldoCuenta (5133 ) + saldoCuenta (5134 ) + saldoCuenta (5143 ) + saldoCuenta (5144 ) + saldoCuenta (5523 ) + saldoCuenta (5524 ) + saldoCuenta (5563 ) + saldoCuenta (5564)).toQString ( '.' ) + "')\n\
doc.set_cell_value(5,39, 'float', '"+ ( saldoCuentaAnt (5103 ) + saldoCuentaAnt (5104 ) + saldoCuentaAnt (5113 ) + saldoCuentaAnt (5114 ) + saldoCuentaAnt (5123 ) + saldoCuentaAnt (5124 ) + saldoCuentaAnt (5133 ) + saldoCuentaAnt (5134 ) + saldoCuentaAnt (5143 ) + saldoCuentaAnt (5144 ) + saldoCuentaAnt (5523 ) + saldoCuentaAnt (5524 ) + saldoCuentaAnt (5563 ) + saldoCuentaAnt (5564)).toQString ( '.' ) + "')\n\
doc.set_cell_property('bold', False)\n\
\
doc.set_cell_property('bold', True)\n\
doc.set_cell_value(2,40, 'string', 'IV. Acreedores comerciales y otras cuentas a pagar.')\n\
doc.set_cell_value(4,40, 'formula', '=D41+D42')\n\
doc.set_cell_value(5,40, 'formula', '=E41+E42')\n\
doc.set_cell_property('bold', False)\n\
\
doc.set_cell_value(2,41, 'string', '1. Proveedores.')\n\
doc.set_cell_value(4,41, 'float', '" + ( saldoCuenta (400 ) + saldoCuenta (401 ) + saldoCuenta (403 ) + saldoCuenta (404 ) + saldoCuenta (405 ) + saldoCuenta (406)).toQString ( '.' ) + "')\n\
doc.set_cell_value(5,41, 'float', '"+ ( saldoCuentaAnt (400 ) + saldoCuentaAnt (401 ) + saldoCuentaAnt (403 ) + saldoCuentaAnt (404 ) + saldoCuentaAnt (405 ) + saldoCuentaAnt (406)).toQString ( '.' ) + "')\n\
\
doc.set_cell_value(2,42, 'string', '2. Otros acreedores.')\n\
doc.set_cell_value(4,42, 'float', '" + ( saldoCuenta (41 ) + saldoCuenta (438 ) + saldoCuenta (465 ) + saldoCuenta (475 ) + saldoCuenta (476 ) + saldoCuenta (477)).toQString ( '.' ) + "')\n\
doc.set_cell_value(5,42, 'float', '"+ ( saldoCuentaAnt (41 ) + saldoCuentaAnt (438 ) + saldoCuentaAnt (465 ) + saldoCuentaAnt (475 ) + saldoCuentaAnt (476 ) + saldoCuentaAnt (477)).toQString ( '.' ) + "')\n\
\
doc.set_cell_property('bold', True)\n\
doc.set_cell_value(2,43, 'string', 'V. Periodificaciones a corto plazo.')\n\
doc.set_cell_value(4,43, 'float', '" + ( saldoCuenta (485 ) + saldoCuenta (568)).toQString ( '.' ) + "')\n\
doc.set_cell_value(5,43, 'float', '"+ ( saldoCuentaAnt (485 ) + saldoCuentaAnt (568)).toQString ( '.' ) + "')\n\
doc.set_cell_property('bold', False)\n\
\
\
doc.set_cell_property('bold', True)\n\
doc.set_cell_value(2,45, 'string', 'TOTAL PATRIMONIO NETO Y PASIVO (A + B + C)')\n\
doc.set_cell_value(4,45, 'formula', '=D6+D21+D32')\n\
doc.set_cell_value(5,45, 'formula', '=E6+E21+E32')\n\
doc.set_cell_property('bold', False)\n\
\
\
\
\
\
\
\
doc.new_sheet(\"(Debe) Haber\")\n\n\
doc.set_column_property(1, 'width', '0.5in')\n\
doc.set_column_property(2, 'width', '5in')\n\
\
doc.set_cell_property('bold', True)\n\
doc.set_cell_value(2,2, 'string', 'BALANCE DE PYMES AL CIERRE DEL EJERCICIO')\n\
doc.set_cell_value(2,4, 'string', '(Debe) Haber')\n\
doc.set_cell_value(3,4, 'string', 'NOTAS')\n\
doc.set_cell_value(4,4, 'string', 'Ejercicio N')\n\
doc.set_cell_value(5,4, 'string', 'Ejercicio N-1')\n\
doc.set_cell_property('bold', False)\n\
\
\
\
doc.set_cell_property('bold', True)\n\
doc.set_cell_value(2,6, 'string', '1. Importe neto de la cifra de negocios.')\n\
doc.set_cell_value(4,6, 'float', '" + ( saldoCuenta (700 ) + saldoCuenta (701 ) + saldoCuenta (702 ) + saldoCuenta (703 ) + saldoCuenta (704 ) + saldoCuenta (705 ) + saldoCuenta (706 ) + saldoCuenta (708 ) + saldoCuenta (709)).toQString ( '.' ) + "')\n\
doc.set_cell_value(5,6, 'float', '"+ ( saldoCuentaAnt (700 ) + saldoCuentaAnt (701 ) + saldoCuentaAnt (702 ) + saldoCuentaAnt (703 ) + saldoCuentaAnt (704 ) + saldoCuentaAnt (705 ) + saldoCuentaAnt (706 ) + saldoCuentaAnt (708 ) + saldoCuentaAnt (709)).toQString ( '.' ) + "')\n\
doc.set_cell_property('bold', False)\n\
\
doc.set_cell_property('bold', True)\n\
doc.set_cell_value(2,7, 'string', '2. Variación de existencias de productos terminados y en curso de fabricación.')\n\
doc.set_cell_value(4,7, 'float', '" + ( saldoCuenta (6930 ) + saldoCuenta (71 ) + saldoCuenta (7930)).toQString ( '.' ) + "')\n\
doc.set_cell_value(5,7, 'float', '"+ ( saldoCuentaAnt (6930 ) + saldoCuentaAnt (71 ) + saldoCuentaAnt (7930)).toQString ( '.' ) + "')\n\
doc.set_cell_property('bold', False)\n\
\
doc.set_cell_property('bold', True)\n\
doc.set_cell_value(2,8, 'string', '3. Trabajos realizados por la empresa para su activo.')\n\
doc.set_cell_value(4,8, 'float', '" + ( saldoCuenta (73)).toQString ( '.' ) + "')\n\
doc.set_cell_value(5,8, 'float', '"+ ( saldoCuentaAnt (73)).toQString ( '.' ) + "')\n\
doc.set_cell_property('bold', False)\n\
\
doc.set_cell_property('bold', True)\n\
doc.set_cell_value(2,9, 'string', '4. Aprovisionamientos.')\n\
doc.set_cell_value(4,9, 'float', '" + ( saldoCuenta (600 ) + saldoCuenta (601 ) + saldoCuenta (602 ) + saldoCuenta (606 ) + saldoCuenta (607 ) + saldoCuenta (608 ) + saldoCuenta (609 ) + saldoCuenta (61 ) + saldoCuenta (6931 ) + saldoCuenta (6932 ) + saldoCuenta (6933 ) + saldoCuenta (7931 ) + saldoCuenta (7932 ) + saldoCuenta (7933)).toQString ( '.' ) + "')\n\
doc.set_cell_value(5,9, 'float', '"+ ( saldoCuentaAnt (600 ) + saldoCuentaAnt (601 ) + saldoCuentaAnt (602 ) + saldoCuentaAnt (606 ) + saldoCuentaAnt (607 ) + saldoCuentaAnt (608 ) + saldoCuentaAnt (609 ) + saldoCuentaAnt (61 ) + saldoCuentaAnt (6931 ) + saldoCuentaAnt (6932 ) + saldoCuentaAnt (6933 ) + saldoCuentaAnt (7931 ) + saldoCuentaAnt (7932 ) + saldoCuentaAnt (7933)).toQString ( '.' ) + "')\n\
doc.set_cell_property('bold', False)\n\
\
doc.set_cell_property('bold', True)\n\
doc.set_cell_value(2,10, 'string', '5. Otros ingresos de explotación.')\n\
doc.set_cell_value(4,10, 'float', '" + ( saldoCuenta (740 ) + saldoCuenta (747 ) + saldoCuenta (75)).toQString ( '.' ) + "')\n\
doc.set_cell_value(5,10, 'float', '"+ ( saldoCuentaAnt (740 ) + saldoCuentaAnt (747 ) + saldoCuentaAnt (75)).toQString ( '.' ) + "')\n\
doc.set_cell_property('bold', False)\n\
\
doc.set_cell_property('bold', True)\n\
doc.set_cell_value(2,11, 'string', '6. Gastos de personal.')\n\
doc.set_cell_value(4,11, 'float', '" + ( saldoCuenta (64)).toQString ( '.' ) + "')\n\
doc.set_cell_value(5,11, 'float', '"+ ( saldoCuentaAnt (64)).toQString ( '.' ) + "')\n\
doc.set_cell_property('bold', False)\n\
\
doc.set_cell_property('bold', True)\n\
doc.set_cell_value(2,12, 'string', '7. Otros gastos de explotación.')\n\
doc.set_cell_value(4,12, 'float', '" + ( saldoCuenta (62 ) + saldoCuenta (631 ) + saldoCuenta (634 ) + saldoCuenta (636 ) + saldoCuenta (639 ) + saldoCuenta (65 ) + saldoCuenta (694 ) + saldoCuenta (695 ) + saldoCuenta (794 ) + saldoCuenta (7954)).toQString ( '.' ) + "')\n\
doc.set_cell_value(5,12, 'float', '"+ ( saldoCuentaAnt (62 ) + saldoCuentaAnt (631 ) + saldoCuentaAnt (634 ) + saldoCuentaAnt (636 ) + saldoCuentaAnt (639 ) + saldoCuentaAnt (65 ) + saldoCuentaAnt (694 ) + saldoCuentaAnt (695 ) + saldoCuentaAnt (794 ) + saldoCuentaAnt (7954)).toQString ( '.' ) + "')\n\
doc.set_cell_property('bold', False)\n\
\
doc.set_cell_property('bold', True)\n\
doc.set_cell_value(2,13, 'string', '8. Amortización del inmovilizado.')\n\
doc.set_cell_value(4,13, 'float', '" + ( saldoCuenta (68)).toQString ( '.' ) + "')\n\
doc.set_cell_value(5,13, 'float', '"+ ( saldoCuentaAnt (68)).toQString ( '.' ) + "')\n\
doc.set_cell_property('bold', False)\n\
\
doc.set_cell_property('bold', True)\n\
doc.set_cell_value(2,14, 'string', '9. Imputación de subvenciones de inmovilizado no financiero y otras.')\n\
doc.set_cell_value(4,14, 'float', '" + ( saldoCuenta (746)).toQString ( '.' ) + "')\n\
doc.set_cell_value(5,14, 'float', '"+ ( saldoCuentaAnt (746)).toQString ( '.' ) + "')\n\
doc.set_cell_property('bold', False)\n\
\
doc.set_cell_property('bold', True)\n\
doc.set_cell_value(2,15, 'string', '10. Excesos de provisiones.')\n\
doc.set_cell_value(4,15, 'float', '" + ( saldoCuenta (7951 ) + saldoCuenta (7952 ) + saldoCuenta (7955)).toQString ( '.' ) + "')\n\
doc.set_cell_value(5,15, 'float', '"+ ( saldoCuentaAnt (7951 ) + saldoCuentaAnt (7952 ) + saldoCuentaAnt (7955)).toQString ( '.' ) + "')\n\
doc.set_cell_property('bold', False)\n\
\
doc.set_cell_property('bold', True)\n\
doc.set_cell_value(2,16, 'string', '11. Deterioro y resultado por enajenaciones del inmovilizado.')\n\
doc.set_cell_value(4,16, 'float', '" + ( saldoCuenta (670 ) + saldoCuenta (671 ) + saldoCuenta (672 ) + saldoCuenta (690 ) + saldoCuenta (691 ) + saldoCuenta (692 ) + saldoCuenta (770 ) + saldoCuenta (771 ) + saldoCuenta (772 ) + saldoCuenta (790 ) + saldoCuenta (791 ) + saldoCuenta (792)).toQString ( '.' ) + "')\n\
doc.set_cell_value(5,16, 'float', '"+ ( saldoCuentaAnt (670 ) + saldoCuentaAnt (671 ) + saldoCuentaAnt (672 ) + saldoCuentaAnt (690 ) + saldoCuentaAnt (691 ) + saldoCuentaAnt (692 ) + saldoCuentaAnt (770 ) + saldoCuentaAnt (771 ) + saldoCuentaAnt (772 ) + saldoCuentaAnt (790 ) + saldoCuentaAnt (791 ) + saldoCuentaAnt (792)).toQString ( '.' ) + "')\n\
doc.set_cell_property('bold', False)\n\
\
\
doc.set_cell_property('bold', True)\n\
doc.set_cell_value(2,18, 'string', 'A) RESULTADO DE EXPLOTACIÓN')\n\
doc.set_cell_value(4,18, 'formula', '=D6+D7+D8+D9+D10+D11+D12+D13+D14+D15+D16')\n\
doc.set_cell_value(5,18, 'formula', '=E6+E7+E8+E9+E10+E11+E12+E13+E14+E15+E16')\n\
doc.set_cell_property('bold', False)\n\
\
doc.set_cell_property('bold', True)\n\
doc.set_cell_value(2,19, 'string', '(1+2+3+4+5+6+7+8+9+10+11)')\n\
\
\
doc.set_cell_property('bold', True)\n\
doc.set_cell_value(2,21, 'string', '12. Ingresos financieros.')\n\
doc.set_cell_value(4,21, 'float', '" + ( saldoCuenta (760 ) + saldoCuenta (761 ) + saldoCuenta (762 ) + saldoCuenta (769)).toQString ( '.' ) + "')\n\
doc.set_cell_value(5,21, 'float', '"+ ( saldoCuentaAnt (760 ) + saldoCuentaAnt (761 ) + saldoCuentaAnt (762 ) + saldoCuentaAnt (769)).toQString ( '.' ) + "')\n\
doc.set_cell_property('bold', False)\n\
\
doc.set_cell_property('bold', True)\n\
doc.set_cell_value(2,22, 'string', '13. Gastos financieros.')\n\
doc.set_cell_value(4,22, 'float', '" + ( saldoCuenta (660 ) + saldoCuenta (661 ) + saldoCuenta (662 ) + saldoCuenta (664 ) + saldoCuenta (665 ) + saldoCuenta (669)).toQString ( '.' ) + "')\n\
doc.set_cell_value(5,22, 'float', '"+ ( saldoCuentaAnt (660 ) + saldoCuentaAnt (661 ) + saldoCuentaAnt (662 ) + saldoCuentaAnt (664 ) + saldoCuentaAnt (665 ) + saldoCuentaAnt (669)).toQString ( '.' ) + "')\n\
doc.set_cell_property('bold', False)\n\
\
doc.set_cell_property('bold', True)\n\
doc.set_cell_value(2,23, 'string', '14. Variación de valor razonable en instrumentos financieros.')\n\
doc.set_cell_value(4,23, 'float', '" + ( saldoCuenta (663 ) + saldoCuenta (763)).toQString ( '.' ) + "')\n\
doc.set_cell_value(5,23, 'float', '"+ ( saldoCuentaAnt (663 ) + saldoCuentaAnt (763)).toQString ( '.' ) + "')\n\
doc.set_cell_property('bold', False)\n\
\
doc.set_cell_property('bold', True)\n\
doc.set_cell_value(2,24, 'string', '15. Diferencias de cambio.')\n\
doc.set_cell_value(4,24, 'float', '" + ( saldoCuenta (668 ) + saldoCuenta (768)).toQString ( '.' ) + "')\n\
doc.set_cell_value(5,24, 'float', '"+ ( saldoCuentaAnt (668 ) + saldoCuentaAnt (768)).toQString ( '.' ) + "')\n\
doc.set_cell_property('bold', False)\n\
\
doc.set_cell_property('bold', True)\n\
doc.set_cell_value(2,25, 'string', '16. Deterioro y resultado por enajenaciones de instrumentos financieros.')\n\
doc.set_cell_value(4,25, 'float', '" + ( saldoCuenta (666 ) + saldoCuenta (667 ) + saldoCuenta (673 ) + saldoCuenta (675 ) + saldoCuenta (696 ) + saldoCuenta (697 ) + saldoCuenta (698 ) + saldoCuenta (699 ) + saldoCuenta (766 ) + saldoCuenta (773 ) + saldoCuenta (775 ) + saldoCuenta (796 ) + saldoCuenta (797 ) + saldoCuenta (798 ) + saldoCuenta (799)).toQString ( '.' ) + "')\n\
doc.set_cell_value(5,25, 'float', '"+ ( saldoCuentaAnt (666 ) + saldoCuentaAnt (667 ) + saldoCuentaAnt (673 ) + saldoCuentaAnt (675 ) + saldoCuentaAnt (696 ) + saldoCuentaAnt (697 ) + saldoCuentaAnt (698 ) + saldoCuentaAnt (699 ) + saldoCuentaAnt (766 ) + saldoCuentaAnt (773 ) + saldoCuentaAnt (775 ) + saldoCuentaAnt (796 ) + saldoCuentaAnt (797 ) + saldoCuentaAnt (798 ) + saldoCuentaAnt (799)).toQString ( '.' ) + "')\n\
doc.set_cell_property('bold', False)\n\
\
\
doc.set_cell_property('bold', True)\n\
doc.set_cell_value(2,27, 'string', 'B) RESULTADO FINANCIERO (12+13+14+15+16 )')\n\
doc.set_cell_value(4,27, 'formula', '=D21+D22+D23+D24+D25')\n\
doc.set_cell_value(5,27, 'formula', '=E21+E22+E23+E24+E25')\n\
doc.set_cell_property('bold', False)\n\
\
\
doc.set_cell_property('bold', True)\n\
doc.set_cell_value(2,29, 'string', 'C) RESULTADO ANTES DE IMPUESTOS (A+B)')\n\
doc.set_cell_value(4,29, 'formula', '=D18+D27')\n\
doc.set_cell_value(5,29, 'formula', '=E18+E27')\n\
doc.set_cell_property('bold', False)\n\
\
\
doc.set_cell_property('bold', True)\n\
doc.set_cell_value(2,31, 'string', '17. Impuestos sobre beneficios.')\n\
doc.set_cell_value(4,31, 'float', '" + ( saldoCuenta (6300 ) + saldoCuenta (6301 ) + saldoCuenta (633 ) + saldoCuenta (638)).toQString ( '.' ) + "')\n\
doc.set_cell_value(5,31, 'float', '"+ ( saldoCuentaAnt (6300 ) + saldoCuentaAnt (6301 ) + saldoCuentaAnt (633 ) + saldoCuentaAnt (638)).toQString ( '.' ) + "')\n\
doc.set_cell_property('bold', False)\n\
\
\
doc.set_cell_property('bold', True)\n\
doc.set_cell_value(2,33, 'string', 'D) RESULTADO DEL EJERCICIO (C + 17)')\n\
doc.set_cell_value(4,33, 'formula', '=D29+D31')\n\
doc.set_cell_value(5,33, 'formula', '=E29+E31')\n\
doc.set_cell_property('bold', False)\n\
\
\
\
\
\
\
\
\
\
\
# Guardamos el documento\n\
doc.save(\"canualesods.ods\")\n");
}


///
/**
**/

QString pluginReportODS::cuentaAnualCAAPGC08()
{
	return QString::QString("\
#!/usr/bin/python\n\
# -*- coding: utf8 -*-\n\
\n\
import ooolib\n\
\n\
# Crea el documento\n\
\
\
\
\n\
doc = ooolib.Calc(\"ACTIVO\")\n\n\
doc.set_column_property(1, 'width', '0.5in')\n\
doc.set_column_property(2, 'width', '5in')\n\
\
doc.set_cell_property('bold', True)\n\
doc.set_cell_value(2,2, 'string', 'BALANCE ABREVIADO AL CIERRE DEL EJERCICIO')\n\
doc.set_cell_value(2,4, 'string', 'ACTIVO')\n\
doc.set_cell_value(3,4, 'string', 'NOTAS')\n\
doc.set_cell_value(4,4, 'string', 'Ejercicio N')\n\
doc.set_cell_value(5,4, 'string', 'Ejercicio N-1')\n\
doc.set_cell_property('bold', False)\n\
\
\
\
doc.set_cell_property('bold', True)\n\
doc.set_cell_value(2,6, 'string', 'A) ACTIVO NO CORRIENTE')\n\
doc.set_cell_value(4,6, 'formula', '=D8+D9+D10+D11+D12+D13')\n\
doc.set_cell_value(5,6, 'formula', '=E8+E9+E10+E11+E12+E13')\n\
doc.set_cell_property('bold', False)\n\
\
\
\
doc.set_cell_property('bold', True)\n\
doc.set_cell_value(2,8, 'string', 'I. Inmovilizado intangible.')\n\
doc.set_cell_value(4,8, 'float', '" + ( saldoCuenta (20 ) + saldoCuenta (280 ) + saldoCuenta (290)).toQString ( '.' ) + "')\n\
doc.set_cell_value(5,8, 'float', '"+ ( saldoCuentaAnt (20 ) + saldoCuentaAnt (280 ) + saldoCuentaAnt (290)).toQString ( '.' ) + "')\n\
doc.set_cell_property('bold', False)\n\
\
doc.set_cell_property('bold', True)\n\
doc.set_cell_value(2,9, 'string', 'II. Inmovilizado material.')\n\
doc.set_cell_value(4,9, 'float', '" + ( saldoCuenta (21 ) + saldoCuenta (281 ) + saldoCuenta (291 ) + saldoCuenta (23)).toQString ( '.' ) + "')\n\
doc.set_cell_value(5,9, 'float', '"+ ( saldoCuentaAnt (21 ) + saldoCuentaAnt (281 ) + saldoCuentaAnt (291 ) + saldoCuentaAnt (23)).toQString ( '.' ) + "')\n\
doc.set_cell_property('bold', False)\n\
\
doc.set_cell_property('bold', True)\n\
doc.set_cell_value(2,10, 'string', 'III. Inversiones inmobiliarias.')\n\
doc.set_cell_value(4,10, 'float', '" + ( saldoCuenta (22 ) + saldoCuenta (282 ) + saldoCuenta (292)).toQString ( '.' ) + "')\n\
doc.set_cell_value(5,10, 'float', '"+ ( saldoCuentaAnt (22 ) + saldoCuentaAnt (282 ) + saldoCuentaAnt (292)).toQString ( '.' ) + "')\n\
doc.set_cell_property('bold', False)\n\
\
doc.set_cell_property('bold', True)\n\
doc.set_cell_value(2,11, 'string', 'IV. Inversiones en empresas del grupo y asociadas a largo plazo.')\n\
doc.set_cell_value(4,11, 'float', '" + ( saldoCuenta (2403 ) + saldoCuenta (2404 ) + saldoCuenta (2413 ) + saldoCuenta (2414 ) + saldoCuenta (2423 ) + saldoCuenta (2424 ) + saldoCuenta (2493 ) + saldoCuenta (2494 ) + saldoCuenta (293 ) + saldoCuenta (2943 ) + saldoCuenta (2944 ) + saldoCuenta (2953 ) + saldoCuenta (2954)).toQString ( '.' ) + "')\n\
doc.set_cell_value(5,11, 'float', '"+ ( saldoCuentaAnt (2403 ) + saldoCuentaAnt (2404 ) + saldoCuentaAnt (2413 ) + saldoCuentaAnt (2414 ) + saldoCuentaAnt (2423 ) + saldoCuentaAnt (2424 ) + saldoCuentaAnt (2493 ) + saldoCuentaAnt (2494 ) + saldoCuentaAnt (293 ) + saldoCuentaAnt (2943 ) + saldoCuentaAnt (2944 ) + saldoCuentaAnt (2953 ) + saldoCuentaAnt (2954)).toQString ( '.' ) + "')\n\
doc.set_cell_property('bold', False)\n\
\
doc.set_cell_property('bold', True)\n\
doc.set_cell_value(2,12, 'string', 'V. Inversiones financieras a largo plazo.')\n\
doc.set_cell_value(4,12, 'float', '" + ( saldoCuenta (2405 ) + saldoCuenta (2415 ) + saldoCuenta (2425 ) + saldoCuenta (2495 ) + saldoCuenta (250 ) + saldoCuenta (251 ) + saldoCuenta (252 ) + saldoCuenta (253 ) + saldoCuenta (254 ) + saldoCuenta (255 ) + saldoCuenta (257 ) + saldoCuenta (258 ) + saldoCuenta (259 ) + saldoCuenta (26 ) + saldoCuenta (2945 ) + saldoCuenta (2955 ) + saldoCuenta (297 ) + saldoCuenta (298)).toQString ( '.' ) + "')\n\
doc.set_cell_value(5,12, 'float', '"+ ( saldoCuentaAnt (2405 ) + saldoCuentaAnt (2415 ) + saldoCuentaAnt (2425 ) + saldoCuentaAnt (2495 ) + saldoCuentaAnt (250 ) + saldoCuentaAnt (251 ) + saldoCuentaAnt (252 ) + saldoCuentaAnt (253 ) + saldoCuentaAnt (254 ) + saldoCuentaAnt (255 ) + saldoCuentaAnt (257 ) + saldoCuentaAnt (258 ) + saldoCuentaAnt (259 ) + saldoCuentaAnt (26 ) + saldoCuentaAnt (2945 ) + saldoCuentaAnt (2955 ) + saldoCuentaAnt (297 ) + saldoCuentaAnt (298)).toQString ( '.' ) + "')\n\
doc.set_cell_property('bold', False)\n\
\
doc.set_cell_property('bold', True)\n\
doc.set_cell_value(2,13, 'string', 'VI. Activos por Impuesto diferido.')\n\
doc.set_cell_value(4,13, 'float', '" + ( saldoCuenta (474)).toQString ( '.' ) + "')\n\
doc.set_cell_value(5,13, 'float', '"+ ( saldoCuentaAnt (474)).toQString ( '.' ) + "')\n\
doc.set_cell_property('bold', False)\n\
\
\
\
\
doc.set_cell_property('bold', True)\n\
doc.set_cell_value(2,15, 'string', 'B) ACTIVO CORRIENTE')\n\
doc.set_cell_value(4,15, 'formula', '=D17+D18+D22+D23+D24+D25+D26')\n\
doc.set_cell_value(5,15, 'formula', '=E17+E18+E22+E23+E24+E25+E26')\n\
doc.set_cell_property('bold', False)\n\
\
\
\
doc.set_cell_property('bold', True)\n\
doc.set_cell_value(2,17, 'string', 'I. Activos no corrientes mantenidos para la venta.')\n\
doc.set_cell_value(4,17, 'float', '" + ( saldoCuenta (580 ) + saldoCuenta (581 ) + saldoCuenta (582 ) + saldoCuenta (583 ) + saldoCuenta (584 ) + saldoCuenta (599)).toQString ( '.' ) + "')\n\
doc.set_cell_value(5,17, 'float', '"+ ( saldoCuentaAnt (580 ) + saldoCuentaAnt (581 ) + saldoCuentaAnt (582 ) + saldoCuentaAnt (583 ) + saldoCuentaAnt (584 ) + saldoCuentaAnt (599)).toQString ( '.' ) + "')\n\
doc.set_cell_property('bold', False)\n\
\
doc.set_cell_property('bold', True)\n\
doc.set_cell_value(2,18, 'string', 'II. Existencias.')\n\
doc.set_cell_value(4,18, 'float', '" + (saldoCuenta(30 ) + saldoCuenta (31 ) + saldoCuenta (32 ) + saldoCuenta (33 ) + saldoCuenta (34 ) + saldoCuenta (35 ) + saldoCuenta (36 ) + saldoCuenta (39 ) + saldoCuenta (407)).toQString('.') + "')\n\
doc.set_cell_value(5,18, 'float', '" + (saldoCuentaAnt(30 ) + saldoCuentaAnt (31 ) + saldoCuentaAnt (32 ) + saldoCuentaAnt (33 ) + saldoCuentaAnt (34 ) + saldoCuentaAnt (35 ) + saldoCuentaAnt (36 ) + saldoCuentaAnt (39 ) + saldoCuentaAnt (407)).toQString('.') + "')\n\
doc.set_cell_property('bold', False)\n\
\
doc.set_cell_property('bold', True)\n\
doc.set_cell_value(2,19, 'string', 'III. Deudores comerciales y otras cuentas a cobrar.')\n\
doc.set_cell_value(4,19, 'formula', '=D20+D21+D22')\n\
doc.set_cell_value(5,19, 'formula', '=E20+E21+E22')\n\
doc.set_cell_property('bold', False)\n\
\
doc.set_cell_value(2,20, 'string', '1. Clientes por ventas y prestaciones de servicios.')\n\
doc.set_cell_value(4,20, 'float', '" + ( saldoCuenta (430 ) + saldoCuenta (431 ) + saldoCuenta (432 ) + saldoCuenta (433 ) + saldoCuenta (434 ) + saldoCuenta (435 ) + saldoCuenta (436 ) + saldoCuenta (437 ) + saldoCuenta (490 ) + saldoCuenta (493)).toQString ( '.' ) + "')\n\
doc.set_cell_value(5,20, 'float', '"+ ( saldoCuentaAnt (430 ) + saldoCuentaAnt (431 ) + saldoCuentaAnt (432 ) + saldoCuentaAnt (433 ) + saldoCuentaAnt (434 ) + saldoCuentaAnt (435 ) + saldoCuentaAnt (436 ) + saldoCuentaAnt (437 ) + saldoCuentaAnt (490 ) + saldoCuentaAnt (493)).toQString ( '.' ) + "')\n\
\
doc.set_cell_value(2,21, 'string', '2. Accionistas (socios) por desembolsos exigidos.')\n\
doc.set_cell_value(4,21, 'float', '" + ( saldoCuenta (5580)).toQString ( '.' ) + "')\n\
doc.set_cell_value(5,21, 'float', '"+ ( saldoCuentaAnt (5580)).toQString ( '.' ) + "')\n\
\
doc.set_cell_value(2,22, 'string', '3. Otros deudores.')\n\
doc.set_cell_value(4,22, 'float', '" + ( saldoCuenta (44 ) + saldoCuenta (460 ) + saldoCuenta (470 ) + saldoCuenta (471 ) + saldoCuenta (472 ) + saldoCuenta (5531 ) + saldoCuenta (5533 ) + saldoCuenta (544)).toQString ( '.' ) + "')\n\
doc.set_cell_value(5,22, 'float', '"+ ( saldoCuentaAnt (44 ) + saldoCuentaAnt (460 ) + saldoCuentaAnt (470 ) + saldoCuentaAnt (471 ) + saldoCuentaAnt (472 ) + saldoCuentaAnt (5531 ) + saldoCuentaAnt (5533 ) + saldoCuentaAnt (544)).toQString ( '.' ) + "')\n\
\
doc.set_cell_property('bold', True)\n\
doc.set_cell_value(2,23, 'string', 'IV. Inversiones en empresas del grupo y asociadas a corto plazo.')\n\
doc.set_cell_value(4,23, 'float', '" + ( saldoCuenta (5303 ) + saldoCuenta (5304 ) + saldoCuenta (5313 ) + saldoCuenta (5314 ) + saldoCuenta (5323 ) + saldoCuenta (5324 ) + saldoCuenta (5333 ) + saldoCuenta (5334 ) + saldoCuenta (5343 ) + saldoCuenta (5344 ) + saldoCuenta (5353 ) + saldoCuenta (5354 ) + saldoCuenta (5393 ) + saldoCuenta (5394 ) + saldoCuenta (5523 ) + saldoCuenta (5524 ) + saldoCuenta (593 ) + saldoCuenta (5943 ) + saldoCuenta (5944 ) + saldoCuenta (5953 ) + saldoCuenta (5954)).toQString ( '.' ) + "')\n\
doc.set_cell_value(5,23, 'float', '"+ ( saldoCuentaAnt (5303 ) + saldoCuentaAnt (5304 ) + saldoCuentaAnt (5313 ) + saldoCuentaAnt (5314 ) + saldoCuentaAnt (5323 ) + saldoCuentaAnt (5324 ) + saldoCuentaAnt (5333 ) + saldoCuentaAnt (5334 ) + saldoCuentaAnt (5343 ) + saldoCuentaAnt (5344 ) + saldoCuentaAnt (5353 ) + saldoCuentaAnt (5354 ) + saldoCuentaAnt (5393 ) + saldoCuentaAnt (5394 ) + saldoCuentaAnt (5523 ) + saldoCuentaAnt (5524 ) + saldoCuentaAnt (593 ) + saldoCuentaAnt (5943 ) + saldoCuentaAnt (5944 ) + saldoCuentaAnt (5953 ) + saldoCuentaAnt (5954)).toQString ( '.' ) + "')\n\
doc.set_cell_property('bold', False)\n\
\
doc.set_cell_property('bold', True)\n\
doc.set_cell_value(2,24, 'string', 'V. Inversiones financieras a corto plazo.')\n\
doc.set_cell_value(4,24, 'float', '" + ( saldoCuenta (5305 ) + saldoCuenta (5315 ) + saldoCuenta (5325 ) + saldoCuenta (5335 ) + saldoCuenta (5345 ) + saldoCuenta (5355 ) + saldoCuenta (5395 ) + saldoCuenta (540 ) + saldoCuenta (541 ) + saldoCuenta (542 ) + saldoCuenta (543 ) + saldoCuenta (545 ) + saldoCuenta (546 ) + saldoCuenta (547 ) + saldoCuenta (548 ) + saldoCuenta (549 ) + saldoCuenta (551 ) + saldoCuenta (5525 ) + saldoCuenta (5590 ) + saldoCuenta (5593 ) + saldoCuenta (565 ) + saldoCuenta (566 ) + saldoCuenta (5945 ) + saldoCuenta (5955 ) + saldoCuenta (597 ) + saldoCuenta (598)).toQString ( '.' ) + "')\n\
doc.set_cell_value(5,24, 'float', '"+ ( saldoCuentaAnt (5305 ) + saldoCuentaAnt (5315 ) + saldoCuentaAnt (5325 ) + saldoCuentaAnt (5335 ) + saldoCuentaAnt (5345 ) + saldoCuentaAnt (5355 ) + saldoCuentaAnt (5395 ) + saldoCuentaAnt (540 ) + saldoCuentaAnt (541 ) + saldoCuentaAnt (542 ) + saldoCuentaAnt (543 ) + saldoCuentaAnt (545 ) + saldoCuentaAnt (546 ) + saldoCuentaAnt (547 ) + saldoCuentaAnt (548 ) + saldoCuentaAnt (549 ) + saldoCuentaAnt (551 ) + saldoCuentaAnt (5525 ) + saldoCuentaAnt (5590 ) + saldoCuentaAnt (5593 ) + saldoCuentaAnt (565 ) + saldoCuentaAnt (566 ) + saldoCuentaAnt (5945 ) + saldoCuentaAnt (5955 ) + saldoCuentaAnt (597 ) + saldoCuentaAnt (598)).toQString ( '.' ) + "')\n\
doc.set_cell_property('bold', False)\n\
\
doc.set_cell_property('bold', True)\n\
doc.set_cell_value(2,25, 'string', 'VI. Periodificaciones a corto plazo.')\n\
doc.set_cell_value(4,25, 'float', '" + ( saldoCuenta (480 ) + saldoCuenta (567)).toQString ( '.' ) + "')\n\
doc.set_cell_value(5,25, 'float', '"+ ( saldoCuentaAnt (480 ) + saldoCuentaAnt (567)).toQString ( '.' ) + "')\n\
doc.set_cell_property('bold', False)\n\
\
doc.set_cell_property('bold', True)\n\
doc.set_cell_value(2,26, 'string', 'VII. Efectivo y otros activos líquidos equivalentes.')\n\
doc.set_cell_value(4,26, 'float', '" + ( saldoCuenta (57)).toQString ( '.' ) + "')\n\
doc.set_cell_value(5,26, 'float', '"+ ( saldoCuentaAnt (57)).toQString ( '.' ) + "')\n\
doc.set_cell_property('bold', False)\n\
\
\
\
doc.set_cell_property('bold', True)\n\
doc.set_cell_value(2,28, 'string', 'TOTAL ACTIVO  (A + B)')\n\
doc.set_cell_value(4,28, 'formula', '=+D6+D15')\n\
doc.set_cell_value(5,28, 'formula', '=+E6+E15')\n\
doc.set_cell_property('bold', False)\n\
\
\
\
\
doc.new_sheet(\"PATRIMONIO NETO Y PASIVO\")\n\n\
doc.set_column_property(1, 'width', '0.5in')\n\
doc.set_column_property(2, 'width', '5in')\n\
\
doc.set_cell_property('bold', True)\n\
doc.set_cell_value(2,2, 'string', 'BALANCE ABREVIADO AL CIERRE DEL EJERCICIO')\n\
doc.set_cell_value(2,4, 'string', 'PATRIMONIO NETO Y PASIVO')\n\
doc.set_cell_value(3,4, 'string', 'NOTAS')\n\
doc.set_cell_value(4,4, 'string', 'Ejercicio N')\n\
doc.set_cell_value(5,4, 'string', 'Ejercicio N-1')\n\
doc.set_cell_property('bold', False)\n\
\
\
\
doc.set_cell_property('bold', True)\n\
doc.set_cell_value(2,6, 'string', 'A) PATRIMONIO NETO')\n\
doc.set_cell_value(4,6, 'formula', '=D8+D20+D21')\n\
doc.set_cell_value(5,6, 'formula', '=E8+E20+E21')\n\
doc.set_cell_property('bold', False)\n\
\
\
doc.set_cell_property('bold', True)\n\
doc.set_cell_value(2,8, 'string', 'A-1) Fondos propios.')\n\
doc.set_cell_value(4,8, 'formula', '=D9+D12+D13+D14+D15+D16+D17+D18+D19')\n\
doc.set_cell_value(5,8, 'formula', '=E9+E12+E13+E14+E15+E16+E17+E18+E19')\n\
doc.set_cell_property('bold', False)\n\
\
doc.set_cell_property('bold', True)\n\
doc.set_cell_value(2,9, 'string', 'I. Capital.')\n\
doc.set_cell_value(4,9, 'formula', '=D10+D11')\n\
doc.set_cell_value(5,9, 'formula', '=E10+E11')\n\
doc.set_cell_property('bold', False)\n\
\
doc.set_cell_value(2,10, 'string', '1. Capital escriturado.')\n\
doc.set_cell_value(4,10, 'float', '" + ( saldoCuenta (100 ) + saldoCuenta (101 ) + saldoCuenta (102)).toQString ( '.' ) + "')\n\
doc.set_cell_value(5,10, 'float', '"+ ( saldoCuentaAnt (100 ) + saldoCuentaAnt (101 ) + saldoCuentaAnt (102)).toQString ( '.' ) + "')\n\
\
doc.set_cell_value(2,11, 'string', '2. (Capital no exigido).')\n\
doc.set_cell_value(4,11, 'float', '" + ( saldoCuenta (1030 ) + saldoCuenta ( 1040)).toQString ( '.' ) + "')\n\
doc.set_cell_value(5,11, 'float', '"+ ( saldoCuentaAnt (1030 ) + saldoCuentaAnt ( 1040)).toQString ( '.' ) + "')\n\
\
doc.set_cell_property('bold', True)\n\
doc.set_cell_value(2,12, 'string', 'II. Prima de emisión.')\n\
doc.set_cell_value(4,12, 'float', '" + ( saldoCuenta (110)).toQString ( '.' ) + "')\n\
doc.set_cell_value(5,12, 'float', '"+ ( saldoCuentaAnt (110)).toQString ( '.' ) + "')\n\
doc.set_cell_property('bold', False)\n\
\
doc.set_cell_property('bold', True)\n\
doc.set_cell_value(2,13, 'string', 'III. Reservas.')\n\
doc.set_cell_value(4,13, 'float', '" + ( saldoCuenta (112 ) + saldoCuenta (113 ) + saldoCuenta (114 ) + saldoCuenta (115 ) + saldoCuenta (119)).toQString ( '.' ) + "')\n\
doc.set_cell_value(5,13, 'float', '"+ ( saldoCuentaAnt (112 ) + saldoCuentaAnt (113 ) + saldoCuentaAnt (114 ) + saldoCuentaAnt (115 ) + saldoCuentaAnt (119)).toQString ( '.' ) + "')\n\
doc.set_cell_property('bold', False)\n\
\
doc.set_cell_property('bold', True)\n\
doc.set_cell_value(2,14, 'string', 'IV. (Acciones y participaciones en patrimonio propias).')\n\
doc.set_cell_value(4,14, 'float', '" + ( saldoCuenta (108 ) + saldoCuenta (109)).toQString ( '.' ) + "')\n\
doc.set_cell_value(5,14, 'float', '"+ ( saldoCuentaAnt (108 ) + saldoCuentaAnt (109)).toQString ( '.' ) + "')\n\
doc.set_cell_property('bold', False)\n\
\
doc.set_cell_property('bold', True)\n\
doc.set_cell_value(2,15, 'string', 'V. Resultados de ejercicios anteriores.')\n\
doc.set_cell_value(4,15, 'float', '" + ( saldoCuenta (120 ) + saldoCuenta (121)).toQString ( '.' ) + "')\n\
doc.set_cell_value(5,15, 'float', '"+ ( saldoCuentaAnt (120 ) + saldoCuentaAnt (121)).toQString ( '.' ) + "')\n\
doc.set_cell_property('bold', False)\n\
\
doc.set_cell_property('bold', True)\n\
doc.set_cell_value(2,16, 'string', 'VI. Otras aportaciones de socios.')\n\
doc.set_cell_value(4,16, 'float', '" + ( saldoCuenta (118)).toQString ( '.' ) + "')\n\
doc.set_cell_value(5,16, 'float', '"+ ( saldoCuentaAnt (118)).toQString ( '.' ) + "')\n\
doc.set_cell_property('bold', False)\n\
\
doc.set_cell_property('bold', True)\n\
doc.set_cell_value(2,17, 'string', 'VII. Resultado del ejercicio.')\n\
doc.set_cell_value(4,17, 'float', '" + ( saldoCuenta (129)).toQString ( '.' ) + "')\n\
doc.set_cell_value(5,17, 'float', '"+ ( saldoCuentaAnt (129)).toQString ( '.' ) + "')\n\
doc.set_cell_property('bold', False)\n\
\
doc.set_cell_property('bold', True)\n\
doc.set_cell_value(2,18, 'string', 'VIII. (Dividendo a cuenta).')\n\
doc.set_cell_value(4,18, 'float', '" + ( saldoCuenta (557)).toQString ( '.' ) + "')\n\
doc.set_cell_value(5,18, 'float', '"+ ( saldoCuentaAnt (557)).toQString ( '.' ) + "')\n\
doc.set_cell_property('bold', False)\n\
\
doc.set_cell_property('bold', True)\n\
doc.set_cell_value(2,19, 'string', 'IX. Otros instrumentos de patrimonio neto.')\n\
doc.set_cell_value(4,19, 'float', '" + ( saldoCuenta (111)).toQString ( '.' ) + "')\n\
doc.set_cell_value(5,19, 'float', '"+ ( saldoCuentaAnt (111)).toQString ( '.' ) + "')\n\
doc.set_cell_property('bold', False)\n\
\
doc.set_cell_property('bold', True)\n\
doc.set_cell_value(2,20, 'string', 'A-2) Ajustes por cambios de valor.')\n\
doc.set_cell_value(4,20, 'float', '" + ( saldoCuenta (133 ) + saldoCuenta (1340 ) + saldoCuenta (137)).toQString ( '.' ) + "')\n\
doc.set_cell_value(5,20, 'float', '"+ ( saldoCuentaAnt (133 ) + saldoCuentaAnt (1340 ) + saldoCuentaAnt (137)).toQString ( '.' ) + "')\n\
doc.set_cell_property('bold', False)\n\
\
doc.set_cell_property('bold', True)\n\
doc.set_cell_value(2,21, 'string', 'A-3) Subvenciones, donaciones y legados recibidos.')\n\
doc.set_cell_value(4,21, 'float', '" + ( saldoCuenta (130 ) + saldoCuenta (131 ) + saldoCuenta (132)).toQString ( '.' ) + "')\n\
doc.set_cell_value(5,21, 'float', '"+ ( saldoCuentaAnt (130 ) + saldoCuentaAnt (131 ) + saldoCuentaAnt (132)).toQString ( '.' ) + "')\n\
doc.set_cell_property('bold', False)\n\
\
doc.set_cell_property('bold', True)\n\
doc.set_cell_value(2,23, 'string', 'B) PASIVO NO CORRIENTE')\n\
doc.set_cell_value(4,23, 'formula', '=D25+D26+D30+D31+D32')\n\
doc.set_cell_value(5,23, 'formula', '=E25+E26+E30+E31+E32')\n\
doc.set_cell_property('bold', False)\n\
\
\
doc.set_cell_property('bold', True)\n\
doc.set_cell_value(2,25, 'string', 'I. Provisiones a largo plazo.')\n\
doc.set_cell_value(4,25, 'float', '" + ( saldoCuenta (14)).toQString ( '.' ) + "')\n\
doc.set_cell_value(5,25, 'float', '"+ ( saldoCuentaAnt (14)).toQString ( '.' ) + "')\n\
doc.set_cell_property('bold', False)\n\
\
doc.set_cell_property('bold', True)\n\
doc.set_cell_value(2,26, 'string', 'II. Deudas a largo plazo.')\n\
doc.set_cell_value(4,26, 'formula', '=D27+D28+D29')\n\
doc.set_cell_value(5,26, 'formula', '=E27+E28+E29')\n\
doc.set_cell_property('bold', False)\n\
\
doc.set_cell_value(2,27, 'string', '1. Deudas con entidades de crédito.')\n\
doc.set_cell_value(4,27, 'float', '" + ( saldoCuenta (1605 ) + saldoCuenta ( 170)).toQString ( '.' ) + "')\n\
doc.set_cell_value(5,27, 'float', '"+ ( saldoCuentaAnt (1605 ) + saldoCuentaAnt ( 170)).toQString ( '.' ) + "')\n\
\
doc.set_cell_value(2,28, 'string', '2. Acreedores por arrendamiento financiero.')\n\
doc.set_cell_value(4,28, 'float', '" + ( saldoCuenta (1625 ) + saldoCuenta (174)).toQString ( '.' ) + "')\n\
doc.set_cell_value(5,28, 'float', '"+ ( saldoCuentaAnt (1625 ) + saldoCuentaAnt (174)).toQString ( '.' ) + "')\n\
\
doc.set_cell_value(2,29, 'string', '3. Otras deudas a largo plazo.')\n\
doc.set_cell_value(4,29, 'float', '" + ( saldoCuenta (1615 ) + saldoCuenta (1635 ) + saldoCuenta (171 ) + saldoCuenta (172 ) + saldoCuenta (173 ) + saldoCuenta (175 ) + saldoCuenta (176 ) + saldoCuenta (177 ) + saldoCuenta (178 ) + saldoCuenta (179 ) + saldoCuenta (180 ) + saldoCuenta (185 ) + saldoCuenta (189)).toQString ( '.' ) + "')\n\
doc.set_cell_value(5,29, 'float', '"+ ( saldoCuentaAnt (1615 ) + saldoCuentaAnt (1635 ) + saldoCuentaAnt (171 ) + saldoCuentaAnt (172 ) + saldoCuentaAnt (173 ) + saldoCuentaAnt (175 ) + saldoCuentaAnt (176 ) + saldoCuentaAnt (177 ) + saldoCuentaAnt (178 ) + saldoCuentaAnt (179 ) + saldoCuentaAnt (180 ) + saldoCuentaAnt (185 ) + saldoCuentaAnt (189)).toQString ( '.' ) + "')\n\
\
doc.set_cell_property('bold', True)\n\
doc.set_cell_value(2,30, 'string', 'III. Deudas con empresas del grupo y asociadas a largo plazo.')\n\
doc.set_cell_value(4,30, 'float', '" + ( saldoCuenta (1603 ) + saldoCuenta (1604 ) + saldoCuenta (1613 ) + saldoCuenta (1614 ) + saldoCuenta (1623 ) + saldoCuenta (1624 ) + saldoCuenta (1633 ) + saldoCuenta (1634)).toQString ( '.' ) + "')\n\
doc.set_cell_value(5,30, 'float', '"+ ( saldoCuentaAnt (1603 ) + saldoCuentaAnt (1604 ) + saldoCuentaAnt (1613 ) + saldoCuentaAnt (1614 ) + saldoCuentaAnt (1623 ) + saldoCuentaAnt (1624 ) + saldoCuentaAnt (1633 ) + saldoCuentaAnt (1634)).toQString ( '.' ) + "')\n\
doc.set_cell_property('bold', False)\n\
\
doc.set_cell_property('bold', True)\n\
doc.set_cell_value(2,31, 'string', 'IV. Pasivos por impuesto diferido.')\n\
doc.set_cell_value(4,31, 'float', '" + ( saldoCuenta (479)).toQString ( '.' ) + "')\n\
doc.set_cell_value(5,31, 'float', '"+ ( saldoCuentaAnt (479)).toQString ( '.' ) + "')\n\
doc.set_cell_property('bold', False)\n\
\
doc.set_cell_property('bold', True)\n\
doc.set_cell_value(2,32, 'string', 'V. Periodificaciones a largo plazo.')\n\
doc.set_cell_value(4,32, 'float', '" + ( saldoCuenta (181)).toQString ( '.' ) + "')\n\
doc.set_cell_value(5,32, 'float', '"+ ( saldoCuentaAnt (181)).toQString ( '.' ) + "')\n\
doc.set_cell_property('bold', False)\n\
\
doc.set_cell_property('bold', True)\n\
doc.set_cell_value(2,34, 'string', 'C) PASIVO CORRIENTE')\n\
doc.set_cell_value(4,34, 'formula', '=D36+D37+D41+D42+D45+D46')\n\
doc.set_cell_value(5,34, 'formula', '=E36+E37+E41+E42+E45+E46')\n\
doc.set_cell_property('bold', False)\n\
\
doc.set_cell_property('bold', True)\n\
doc.set_cell_value(2,36, 'string', 'I. Pasivos vinculados con activos no corrientes mantenidos para la venta.')\n\
doc.set_cell_value(4,36, 'float', '" + ( saldoCuenta (585 ) + saldoCuenta (586 ) + saldoCuenta (587 ) + saldoCuenta (588 ) + saldoCuenta (589)).toQString ( '.' ) + "')\n\
doc.set_cell_value(5,36, 'float', '"+ ( saldoCuentaAnt (585 ) + saldoCuentaAnt (586 ) + saldoCuentaAnt (587 ) + saldoCuentaAnt (588 ) + saldoCuentaAnt (589)).toQString ( '.' ) + "')\n\
doc.set_cell_property('bold', False)\n\
\
doc.set_cell_property('bold', True)\n\
doc.set_cell_value(2,37, 'string', 'II. Provisiones a corto plazo.')\n\
doc.set_cell_value(4,37, 'float', '" + ( saldoCuenta (499 ) + saldoCuenta ( 529)).toQString ( '.' ) + "')\n\
doc.set_cell_value(5,37, 'float', '"+ ( saldoCuentaAnt (499 ) + saldoCuentaAnt ( 529)).toQString ( '.' ) + "')\n\
doc.set_cell_property('bold', False)\n\
\
doc.set_cell_property('bold', True)\n\
doc.set_cell_value(2,38, 'string', 'III. Deudas a corto plazo.')\n\
doc.set_cell_value(4,38, 'formula', '=D39+D40+D41')\n\
doc.set_cell_value(5,38, 'formula', '=E39+E40+E41')\n\
doc.set_cell_property('bold', False)\n\
\
doc.set_cell_value(2,39, 'string', '1. Deudas con entidades de crédito.')\n\
doc.set_cell_value(4,39, 'float', '" + ( saldoCuenta (5105 ) + saldoCuenta (520 ) + saldoCuenta (527)).toQString ( '.' ) + "')\n\
doc.set_cell_value(5,39, 'float', '"+ ( saldoCuentaAnt (5105 ) + saldoCuentaAnt (520 ) + saldoCuentaAnt (527)).toQString ( '.' ) + "')\n\
\
doc.set_cell_value(2,40, 'string', '2. Acreedores por arrendamiento financiero.')\n\
doc.set_cell_value(4,40, 'float', '" + ( saldoCuenta (5125 ) + saldoCuenta (524)).toQString ( '.' ) + "')\n\
doc.set_cell_value(5,40, 'float', '"+ ( saldoCuentaAnt (5125 ) + saldoCuentaAnt (524)).toQString ( '.' ) + "')\n\
\
doc.set_cell_value(2,41, 'string', '3. Otras deudas a corto plazo.')\n\
doc.set_cell_value(4,41, 'float', '" + ( saldoCuenta (1034 ) + saldoCuenta (1044 ) + saldoCuenta (190 ) + saldoCuenta (192 ) + saldoCuenta (194 ) + saldoCuenta (500 ) + saldoCuenta (501 ) + saldoCuenta (505 ) + saldoCuenta (506 ) + saldoCuenta (509 ) + saldoCuenta (5115 ) + saldoCuenta (5135 ) + saldoCuenta (5145 ) + saldoCuenta (521 ) + saldoCuenta (522 ) + saldoCuenta (523 ) + saldoCuenta (525 ) + saldoCuenta (526 ) + saldoCuenta (528 ) + saldoCuenta (551 ) + saldoCuenta (5525 ) + saldoCuenta (5530 ) + saldoCuenta (5532 ) + saldoCuenta (555 ) + saldoCuenta (5565 ) + saldoCuenta (5566 ) + saldoCuenta (5595 ) + saldoCuenta (5598 ) + saldoCuenta (560 ) + saldoCuenta (561 ) + saldoCuenta (569)).toQString ( '.' ) + "')\n\
doc.set_cell_value(5,41, 'float', '"+ ( saldoCuentaAnt (1034 ) + saldoCuentaAnt (1044 ) + saldoCuentaAnt (190 ) + saldoCuentaAnt (192 ) + saldoCuentaAnt (194 ) + saldoCuentaAnt (500 ) + saldoCuentaAnt (501 ) + saldoCuentaAnt (505 ) + saldoCuentaAnt (506 ) + saldoCuentaAnt (509 ) + saldoCuentaAnt (5115 ) + saldoCuentaAnt (5135 ) + saldoCuentaAnt (5145 ) + saldoCuentaAnt (521 ) + saldoCuentaAnt (522 ) + saldoCuentaAnt (523 ) + saldoCuentaAnt (525 ) + saldoCuentaAnt (526 ) + saldoCuentaAnt (528 ) + saldoCuentaAnt (551 ) + saldoCuentaAnt (5525 ) + saldoCuentaAnt (5530 ) + saldoCuentaAnt (5532 ) + saldoCuentaAnt (555 ) + saldoCuentaAnt (5565 ) + saldoCuentaAnt (5566 ) + saldoCuentaAnt (5595 ) + saldoCuentaAnt (5598 ) + saldoCuentaAnt (560 ) + saldoCuentaAnt (561 ) + saldoCuentaAnt (569)).toQString ( '.' ) + "')\n\
\
doc.set_cell_property('bold', True)\n\
doc.set_cell_value(2,42, 'string', 'IV. Deudas con empresas del grupo y asociadas a corto plazo.')\n\
doc.set_cell_value(4,42, 'float', '" + ( saldoCuenta (5103 ) + saldoCuenta (5104 ) + saldoCuenta (5113 ) + saldoCuenta (5114 ) + saldoCuenta (5123 ) + saldoCuenta (5124 ) + saldoCuenta (5133 ) + saldoCuenta (5134 ) + saldoCuenta (5143 ) + saldoCuenta (5144 ) + saldoCuenta (5523 ) + saldoCuenta (5524 ) + saldoCuenta (5563 ) + saldoCuenta (5564)).toQString ( '.' ) + "')\n\
doc.set_cell_value(5,42, 'float', '"+ ( saldoCuentaAnt (5103 ) + saldoCuentaAnt (5104 ) + saldoCuentaAnt (5113 ) + saldoCuentaAnt (5114 ) + saldoCuentaAnt (5123 ) + saldoCuentaAnt (5124 ) + saldoCuentaAnt (5133 ) + saldoCuentaAnt (5134 ) + saldoCuentaAnt (5143 ) + saldoCuentaAnt (5144 ) + saldoCuentaAnt (5523 ) + saldoCuentaAnt (5524 ) + saldoCuentaAnt (5563 ) + saldoCuentaAnt (5564)).toQString ( '.' ) + "')\n\
doc.set_cell_property('bold', False)\n\
\
doc.set_cell_property('bold', True)\n\
doc.set_cell_value(2,43, 'string', 'V. Acreedores comerciales y otras cuentas a pagar.')\n\
doc.set_cell_value(4,43, 'formula', '=D44+D45')\n\
doc.set_cell_value(5,43, 'formula', '=E44+E45')\n\
doc.set_cell_property('bold', False)\n\
\
doc.set_cell_value(2,44, 'string', '1. Proveedores.')\n\
doc.set_cell_value(4,44, 'float', '" + ( saldoCuenta (400 ) + saldoCuenta (401 ) + saldoCuenta (403 ) + saldoCuenta (404 ) + saldoCuenta (405 ) + saldoCuenta (406)).toQString ( '.' ) + "')\n\
doc.set_cell_value(5,44, 'float', '"+ ( saldoCuentaAnt (400 ) + saldoCuentaAnt (401 ) + saldoCuentaAnt (403 ) + saldoCuentaAnt (404 ) + saldoCuentaAnt (405 ) + saldoCuentaAnt (406)).toQString ( '.' ) + "')\n\
\
doc.set_cell_value(2,45, 'string', '2. Otros acreedores.')\n\
doc.set_cell_value(4,45, 'float', '" + ( saldoCuenta (41 ) + saldoCuenta (438 ) + saldoCuenta (465 ) + saldoCuenta (466 ) + saldoCuenta (475 ) + saldoCuenta (476 ) + saldoCuenta (477)).toQString ( '.' ) + "')\n\
doc.set_cell_value(5,45, 'float', '"+ ( saldoCuentaAnt (41 ) + saldoCuentaAnt (438 ) + saldoCuentaAnt (465 ) + saldoCuentaAnt (466 ) + saldoCuentaAnt (475 ) + saldoCuentaAnt (476 ) + saldoCuentaAnt (477)).toQString ( '.' ) + "')\n\
\
doc.set_cell_property('bold', True)\n\
doc.set_cell_value(2,46, 'string', 'VI. Periodificaciones a corto plazo.')\n\
doc.set_cell_value(4,46, 'float', '" + ( saldoCuenta (485 ) + saldoCuenta ( 568)).toQString ( '.' ) + "')\n\
doc.set_cell_value(5,46, 'float', '"+ ( saldoCuentaAnt (485 ) + saldoCuentaAnt ( 568)).toQString ( '.' ) + "')\n\
doc.set_cell_property('bold', False)\n\
\
\
doc.set_cell_property('bold', True)\n\
doc.set_cell_value(2,48, 'string', 'TOTAL PATRIMONIO NETO Y PASIVO (A + B + C)')\n\
doc.set_cell_value(4,48, 'formula', '=D6+D23+D34')\n\
doc.set_cell_value(5,48, 'formula', '=E6+E23+E34')\n\
doc.set_cell_property('bold', False)\n\
\
\
\
\
\
\
\
doc.new_sheet(\"PERDIDAS_Y_GANANCIAS\")\n\n\
doc.set_column_property(1, 'width', '0.5in')\n\
doc.set_column_property(2, 'width', '5in')\n\
\
doc.set_cell_property('bold', True)\n\
doc.set_cell_value(2,2, 'string', 'CUENTA DE PÉRDIDAS Y GANANCIAS ABREVIADA CORRESPONDIENTE AL EJERCICIO TERMINADO')\n\
doc.set_cell_value(2,4, 'string', 'PERDIDAS Y GANANCIAS')\n\
doc.set_cell_value(3,4, 'string', 'NOTAS')\n\
doc.set_cell_value(4,4, 'string', 'Ejercicio N')\n\
doc.set_cell_value(5,4, 'string', 'Ejercicio N-1')\n\
doc.set_cell_property('bold', False)\n\
\
\
\
doc.set_cell_property('bold', True)\n\
doc.set_cell_value(2,6, 'string', '1. Importe neto de la cifra de negocios.')\n\
doc.set_cell_value(4,6, 'float', '" + ( saldoCuenta (700 ) + saldoCuenta (701 ) + saldoCuenta (702 ) + saldoCuenta (703 ) + saldoCuenta (704 ) + saldoCuenta ( 705 ) + saldoCuenta (706 ) + saldoCuenta (708 ) + saldoCuenta (709)).toQString ( '.' ) + "')\n\
doc.set_cell_value(5,6, 'float', '"+ ( saldoCuentaAnt (700 ) + saldoCuentaAnt (701 ) + saldoCuentaAnt (702 ) + saldoCuentaAnt (703 ) + saldoCuentaAnt (704 ) + saldoCuentaAnt ( 705 ) + saldoCuentaAnt (706 ) + saldoCuentaAnt (708 ) + saldoCuentaAnt (709)).toQString ( '.' ) + "')\n\
doc.set_cell_property('bold', False)\n\
\
doc.set_cell_property('bold', True)\n\
doc.set_cell_value(2,7, 'string', '2. Variación de existencias de productos terminados y en curso de fabricación.')\n\
doc.set_cell_value(4,7, 'float', '" + ( saldoCuenta (6930 ) + saldoCuenta ( 71 ) + saldoCuenta (7930)).toQString ( '.' ) + "')\n\
doc.set_cell_value(5,7, 'float', '"+ ( saldoCuentaAnt (6930 ) + saldoCuentaAnt ( 71 ) + saldoCuentaAnt (7930)).toQString ( '.' ) + "')\n\
doc.set_cell_property('bold', False)\n\
\
doc.set_cell_property('bold', True)\n\
doc.set_cell_value(2,8, 'string', '3. Trabajos realizados por la empresa para su activo.')\n\
doc.set_cell_value(4,8, 'float', '" + ( saldoCuenta (73)).toQString ( '.' ) + "')\n\
doc.set_cell_value(5,8, 'float', '"+ ( saldoCuentaAnt (73)).toQString ( '.' ) + "')\n\
doc.set_cell_property('bold', False)\n\
\
doc.set_cell_property('bold', True)\n\
doc.set_cell_value(2,9, 'string', '4. Aprovisionamientos.')\n\
doc.set_cell_value(4,9, 'float', '" + ( saldoCuenta (600 ) + saldoCuenta (601 ) + saldoCuenta (602 ) + saldoCuenta (606 ) + saldoCuenta (607 ) + saldoCuenta (608 ) + saldoCuenta (609 ) + saldoCuenta (61 ) + saldoCuenta (6931 ) + saldoCuenta (6932 ) + saldoCuenta (6933 ) + saldoCuenta (7931 ) + saldoCuenta (7932 ) + saldoCuenta (7933)).toQString ( '.' ) + "')\n\
doc.set_cell_value(5,9, 'float', '"+ ( saldoCuentaAnt (600 ) + saldoCuentaAnt (601 ) + saldoCuentaAnt (602 ) + saldoCuentaAnt (606 ) + saldoCuentaAnt (607 ) + saldoCuentaAnt (608 ) + saldoCuentaAnt (609 ) + saldoCuentaAnt (61 ) + saldoCuentaAnt (6931 ) + saldoCuentaAnt (6932 ) + saldoCuentaAnt (6933 ) + saldoCuentaAnt (7931 ) + saldoCuentaAnt (7932 ) + saldoCuentaAnt (7933)).toQString ( '.' ) + "')\n\
doc.set_cell_property('bold', False)\n\
\
doc.set_cell_property('bold', True)\n\
doc.set_cell_value(2,10, 'string', '5. Otros ingresos de explotación.')\n\
doc.set_cell_value(4,10, 'float', '" + ( saldoCuenta (740 ) + saldoCuenta (747 ) + saldoCuenta (75)).toQString ( '.' ) + "')\n\
doc.set_cell_value(5,10, 'float', '"+ ( saldoCuentaAnt (740 ) + saldoCuentaAnt (747 ) + saldoCuentaAnt (75)).toQString ( '.' ) + "')\n\
doc.set_cell_property('bold', False)\n\
\
doc.set_cell_property('bold', True)\n\
doc.set_cell_value(2,11, 'string', '6. Gastos de personal.')\n\
doc.set_cell_value(4,11, 'float', '" + ( saldoCuenta (64 ) + saldoCuenta (7950 ) + saldoCuenta (7957)).toQString ( '.' ) + "')\n\
doc.set_cell_value(5,11, 'float', '"+ ( saldoCuentaAnt (64 ) + saldoCuentaAnt (7950 ) + saldoCuentaAnt (7957)).toQString ( '.' ) + "')\n\
doc.set_cell_property('bold', False)\n\
\
doc.set_cell_property('bold', True)\n\
doc.set_cell_value(2,12, 'string', '7. Otros gastos de explotación.')\n\
doc.set_cell_value(4,12, 'float', '" + ( saldoCuenta (62 ) + saldoCuenta (631 ) + saldoCuenta (634 ) + saldoCuenta (636 ) + saldoCuenta (639 ) + saldoCuenta (65 ) + saldoCuenta (694 ) + saldoCuenta (695 ) + saldoCuenta (794 ) + saldoCuenta ( 7954)).toQString ( '.' ) + "')\n\
doc.set_cell_value(5,12, 'float', '"+ ( saldoCuentaAnt (62 ) + saldoCuentaAnt (631 ) + saldoCuentaAnt (634 ) + saldoCuentaAnt (636 ) + saldoCuentaAnt (639 ) + saldoCuentaAnt (65 ) + saldoCuentaAnt (694 ) + saldoCuentaAnt (695 ) + saldoCuentaAnt (794 ) + saldoCuentaAnt ( 7954)).toQString ( '.' ) + "')\n\
doc.set_cell_property('bold', False)\n\
\
doc.set_cell_property('bold', True)\n\
doc.set_cell_value(2,13, 'string', '8. Amortización del inmovilizado.')\n\
doc.set_cell_value(4,13, 'float', '" + ( saldoCuenta (68)).toQString ( '.' ) + "')\n\
doc.set_cell_value(5,13, 'float', '"+ ( saldoCuentaAnt (68)).toQString ( '.' ) + "')\n\
doc.set_cell_property('bold', False)\n\
\
doc.set_cell_property('bold', True)\n\
doc.set_cell_value(2,14, 'string', '9. Imputación de subvenciones de inmovilizado no financiero y otras.')\n\
doc.set_cell_value(4,14, 'float', '" + ( saldoCuenta (746)).toQString ( '.' ) + "')\n\
doc.set_cell_value(5,14, 'float', '"+ ( saldoCuentaAnt (746)).toQString ( '.' ) + "')\n\
doc.set_cell_property('bold', False)\n\
\
doc.set_cell_property('bold', True)\n\
doc.set_cell_value(2,15, 'string', '10. Excesos de provisiones.')\n\
doc.set_cell_value(4,15, 'float', '" + ( saldoCuenta (7951 ) + saldoCuenta (7952 ) + saldoCuenta (7955 ) + saldoCuenta (7956)).toQString ( '.' ) + "')\n\
doc.set_cell_value(5,15, 'float', '"+ ( saldoCuentaAnt (7951 ) + saldoCuentaAnt (7952 ) + saldoCuentaAnt (7955 ) + saldoCuentaAnt (7956)).toQString ( '.' ) + "')\n\
doc.set_cell_property('bold', False)\n\
\
doc.set_cell_property('bold', True)\n\
doc.set_cell_value(2,16, 'string', '11. Deterioro y resultado por enajenaciones del inmovilizado.')\n\
doc.set_cell_value(4,16, 'float', '" + ( saldoCuenta (670 ) + saldoCuenta (671 ) + saldoCuenta (672 ) + saldoCuenta (690 ) + saldoCuenta (691 ) + saldoCuenta (692 ) + saldoCuenta (770 ) + saldoCuenta (771 ) + saldoCuenta (772 ) + saldoCuenta (790 ) + saldoCuenta (791 ) + saldoCuenta (792)).toQString ( '.' ) + "')\n\
doc.set_cell_value(5,16, 'float', '"+ ( saldoCuentaAnt (670 ) + saldoCuentaAnt (671 ) + saldoCuentaAnt (672 ) + saldoCuentaAnt (690 ) + saldoCuentaAnt (691 ) + saldoCuentaAnt (692 ) + saldoCuentaAnt (770 ) + saldoCuentaAnt (771 ) + saldoCuentaAnt (772 ) + saldoCuentaAnt (790 ) + saldoCuentaAnt (791 ) + saldoCuentaAnt (792)).toQString ( '.' ) + "')\n\
doc.set_cell_property('bold', False)\n\
\
\
doc.set_cell_property('bold', True)\n\
doc.set_cell_value(2,18, 'string', 'A) RESULTADO DE EXPLOTACIÓN')\n\
doc.set_cell_value(4,18, 'formula', '=D6+D7+D8+D9+D10+D11+D12+D13+D14+D15+D16')\n\
doc.set_cell_value(5,18, 'formula', '=E6+E7+E8+E9+E10+E11+E12+E13+E14+E15+E16')\n\
doc.set_cell_property('bold', False)\n\
\
doc.set_cell_property('bold', True)\n\
doc.set_cell_value(2,19, 'string', '(1+2+3+4+5+6+7+8+9+10+11)')\n\
\
\
doc.set_cell_property('bold', True)\n\
doc.set_cell_value(2,21, 'string', '12. Ingresos financieros.')\n\
doc.set_cell_value(4,21, 'float', '" + ( saldoCuenta (760 ) + saldoCuenta (761 ) + saldoCuenta (762 ) + saldoCuenta (767 ) + saldoCuenta (769)).toQString ( '.' ) + "')\n\
doc.set_cell_value(5,21, 'float', '"+ ( saldoCuentaAnt (760 ) + saldoCuentaAnt (761 ) + saldoCuentaAnt (762 ) + saldoCuentaAnt (767 ) + saldoCuentaAnt (769)).toQString ( '.' ) + "')\n\
doc.set_cell_property('bold', False)\n\
\
doc.set_cell_property('bold', True)\n\
doc.set_cell_value(2,22, 'string', '13. Gastos financieros.')\n\
doc.set_cell_value(4,22, 'float', '" + ( saldoCuenta (660 ) + saldoCuenta (661 ) + saldoCuenta (662 ) + saldoCuenta (664 ) + saldoCuenta (665 ) + saldoCuenta (669)).toQString ( '.' ) + "')\n\
doc.set_cell_value(5,22, 'float', '"+ ( saldoCuentaAnt (660 ) + saldoCuentaAnt (661 ) + saldoCuentaAnt (662 ) + saldoCuentaAnt (664 ) + saldoCuentaAnt (665 ) + saldoCuentaAnt (669)).toQString ( '.' ) + "')\n\
doc.set_cell_property('bold', False)\n\
\
doc.set_cell_property('bold', True)\n\
doc.set_cell_value(2,23, 'string', '14. Variación de valor razonable en instrumentos financieros.')\n\
doc.set_cell_value(4,23, 'float', '" + ( saldoCuenta (663 ) + saldoCuenta (763)).toQString ( '.' ) + "')\n\
doc.set_cell_value(5,23, 'float', '"+ ( saldoCuentaAnt (663 ) + saldoCuentaAnt (763)).toQString ( '.' ) + "')\n\
doc.set_cell_property('bold', False)\n\
\
doc.set_cell_property('bold', True)\n\
doc.set_cell_value(2,24, 'string', '15. Diferencias de cambio.')\n\
doc.set_cell_value(4,24, 'float', '" + ( saldoCuenta (668 ) + saldoCuenta (768)).toQString ( '.' ) + "')\n\
doc.set_cell_value(5,24, 'float', '"+ ( saldoCuentaAnt (668 ) + saldoCuentaAnt (768)).toQString ( '.' ) + "')\n\
doc.set_cell_property('bold', False)\n\
\
doc.set_cell_property('bold', True)\n\
doc.set_cell_value(2,25, 'string', '16. Deterioro y resultado por enajenaciones de instrumentos financieros.')\n\
doc.set_cell_value(4,25, 'float', '" + ( saldoCuenta (666 ) + saldoCuenta (667 ) + saldoCuenta (673 ) + saldoCuenta (675 ) + saldoCuenta (696 ) + saldoCuenta (697 ) + saldoCuenta (698 ) + saldoCuenta (699 ) + saldoCuenta (766 ) + saldoCuenta (773 ) + saldoCuenta (775 ) + saldoCuenta (796 ) + saldoCuenta (797 ) + saldoCuenta (798 ) + saldoCuenta (799)).toQString ( '.' ) + "')\n\
doc.set_cell_value(5,25, 'float', '"+ ( saldoCuentaAnt (666 ) + saldoCuentaAnt (667 ) + saldoCuentaAnt (673 ) + saldoCuentaAnt (675 ) + saldoCuentaAnt (696 ) + saldoCuentaAnt (697 ) + saldoCuentaAnt (698 ) + saldoCuentaAnt (699 ) + saldoCuentaAnt (766 ) + saldoCuentaAnt (773 ) + saldoCuentaAnt (775 ) + saldoCuentaAnt (796 ) + saldoCuentaAnt (797 ) + saldoCuentaAnt (798 ) + saldoCuentaAnt (799)).toQString ( '.' ) + "')\n\
doc.set_cell_property('bold', False)\n\
\
\
doc.set_cell_property('bold', True)\n\
doc.set_cell_value(2,27, 'string', 'B) RESULTADO FINANCIERO (12+13+14+15+16 )')\n\
doc.set_cell_value(4,27, 'formula', '=D21+D22+D23+D24+D25')\n\
doc.set_cell_value(5,27, 'formula', '=E21+E22+E23+E24+E25')\n\
doc.set_cell_property('bold', False)\n\
\
\
doc.set_cell_property('bold', True)\n\
doc.set_cell_value(2,29, 'string', 'C) RESULTADO ANTES DE IMPUESTOS (A+B)')\n\
doc.set_cell_value(4,29, 'formula', '=D18+D27')\n\
doc.set_cell_value(5,29, 'formula', '=E18+E27')\n\
doc.set_cell_property('bold', False)\n\
\
\
doc.set_cell_property('bold', True)\n\
doc.set_cell_value(2,31, 'string', '17. Impuestos sobre beneficios.')\n\
doc.set_cell_value(4,31, 'float', '" + ( saldoCuenta (6300 ) + saldoCuenta (6301 ) + saldoCuenta (633 ) + saldoCuenta (638)).toQString ( '.' ) + "')\n\
doc.set_cell_value(5,31, 'float', '"+ ( saldoCuentaAnt (6300 ) + saldoCuentaAnt (6301 ) + saldoCuentaAnt (633 ) + saldoCuentaAnt (638)).toQString ( '.' ) + "')\n\
doc.set_cell_property('bold', False)\n\
\
\
doc.set_cell_property('bold', True)\n\
doc.set_cell_value(2,33, 'string', 'D) RESULTADO DEL EJERCICIO (C + 17)')\n\
doc.set_cell_value(4,33, 'formula', '=D29+D31')\n\
doc.set_cell_value(5,33, 'formula', '=E29+E31')\n\
doc.set_cell_property('bold', False)\n\
\
\
\
\
\
doc.new_sheet(\"INGRESOS Y GASTOS\")\n\n\
doc.set_column_property(1, 'width', '0.5in')\n\
doc.set_column_property(2, 'width', '5in')\n\
\
doc.set_cell_property('bold', True)\n\
doc.set_cell_value(2,2, 'string', 'ESTADO ABREVIADO DE INGRESOS Y GASTOS RECONOCIDOS CORRESPONDIENTE AL EJERCICIO TERMINADO')\n\
doc.set_cell_value(2,4, 'string', 'INGRESOS Y GASTOS')\n\
doc.set_cell_value(3,4, 'string', 'NOTAS')\n\
doc.set_cell_value(4,4, 'string', 'Ejercicio N')\n\
doc.set_cell_value(5,4, 'string', 'Ejercicio N-1')\n\
doc.set_cell_property('bold', False)\n\
\
\
\
doc.set_cell_property('bold', True)\n\
doc.set_cell_value(2,6, 'string', 'A) Resultado de la cuenta de perdidas y ganancias')\n\
doc.set_cell_value(4,6, 'formula', '=" + trUtf8("PERDIDAS_Y_GANANCIAS")+".D33')\n\
doc.set_cell_value(5,6, 'formula', '=PERDIDAS_Y_GANANCIAS.E33')\n\
doc.set_cell_property('bold', False)\n\
\
doc.set_cell_property('bold', True)\n\
doc.set_cell_value(2,7, 'string', 'Ingresos y gastos imputados directamente al patrimonio neto')\n\
doc.set_cell_property('bold', False)\n\
\
\
doc.set_cell_property('bold', True)\n\
doc.set_cell_value(2,9, 'string', '" + trUtf8("I. Por valoración de instrumentos financieros.") + "')\n\
doc.set_cell_value(4,9, 'float', '" + ( saldoCuenta (800 ) + saldoCuenta (89 ) + saldoCuenta (900 ) + saldoCuenta ( 991 ) + saldoCuenta (992)).toQString ( '.' ) + "')\n\
doc.set_cell_value(5,9, 'float', '"+ ( saldoCuentaAnt (800 ) + saldoCuentaAnt (89 ) + saldoCuentaAnt (900 ) + saldoCuentaAnt ( 991 ) + saldoCuentaAnt (992)).toQString ( '.' ) + "')\n\
doc.set_cell_property('bold', False)\n\
\
doc.set_cell_property('bold', True)\n\
doc.set_cell_value(2,10, 'string', 'II. Por coberturas de flujos de efectivo.')\n\
doc.set_cell_value(4,10, 'float', '" + ( saldoCuenta (810 ) + saldoCuenta (910)).toQString ( '.' ) + "')\n\
doc.set_cell_value(5,10, 'float', '"+ ( saldoCuentaAnt (810 ) + saldoCuentaAnt (910)).toQString ( '.' ) + "')\n\
doc.set_cell_property('bold', False)\n\
\
doc.set_cell_property('bold', True)\n\
doc.set_cell_value(2,11, 'string', 'III. Subvenciones, donaciones y legados recibidos.')\n\
doc.set_cell_value(4,11, 'float', '" + ( saldoCuenta (94)).toQString ( '.' ) + "')\n\
doc.set_cell_value(5,11, 'float', '"+ ( saldoCuentaAnt (94)).toQString ( '.' ) + "')\n\
doc.set_cell_property('bold', False)\n\
\
doc.set_cell_property('bold', True)\n\
doc.set_cell_value(2,12, 'string', 'IV. Por ganancias y pérdidas actuariales y otros ajustes')\n\
doc.set_cell_value(4,12, 'float', '" + ( saldoCuenta (85 ) + saldoCuenta (95)).toQString ( '.' ) + "')\n\
doc.set_cell_value(5,12, 'float', '"+ ( saldoCuentaAnt (85 ) + saldoCuentaAnt (95)).toQString ( '.' ) + "')\n\
doc.set_cell_property('bold', False)\n\
\
doc.set_cell_property('bold', True)\n\
doc.set_cell_value(2,13, 'string', 'V. Efecto impositivo.')\n\
doc.set_cell_value(4,13, 'float', '" + ( saldoCuenta (8300 ) + saldoCuenta (8301 ) + saldoCuenta (833 ) + saldoCuenta (834 ) + saldoCuenta (835 ) + saldoCuenta (838)).toQString ( '.' ) + "')\n\
doc.set_cell_value(5,13, 'float', '"+ ( saldoCuentaAnt (8300 ) + saldoCuentaAnt (8301 ) + saldoCuentaAnt (833 ) + saldoCuentaAnt (834 ) + saldoCuentaAnt (835 ) + saldoCuentaAnt (838)).toQString ( '.' ) + "')\n\
doc.set_cell_property('bold', False)\n\
\
\
doc.set_cell_property('bold', True)\n\
doc.set_cell_value(2,15, 'string', 'B) Total Ingresos y gastos imputados directamente al patrimonio neto ')\n\
doc.set_cell_value(4,15, 'formula', '=D9+D10+D11+D12+D13')\n\
doc.set_cell_value(5,15, 'formula', '=E9+E10+E11+E12+E13')\n\
doc.set_cell_property('bold', False)\n\
\
doc.set_cell_property('bold', True)\n\
doc.set_cell_value(2,16, 'string', '(I+II+III+IV+V)')\n\
doc.set_cell_property('bold', False)\n\
\
doc.set_cell_property('bold', True)\n\
doc.set_cell_value(2,17, 'string', 'Transferencias a la cuenta de pérdidas y ganancias')\n\
doc.set_cell_property('bold', False)\n\
\
\
doc.set_cell_property('bold', True)\n\
doc.set_cell_value(2,19, 'string', 'VI. Por valoración de instrumentos financieros.')\n\
doc.set_cell_value(4,19, 'float', '" + ( saldoCuenta (802 ) + saldoCuenta (902 ) + saldoCuenta (993 ) + saldoCuenta (994)).toQString ( '.' ) + "')\n\
doc.set_cell_value(5,19, 'float', '"+ ( saldoCuentaAnt (802 ) + saldoCuentaAnt (902 ) + saldoCuentaAnt (993 ) + saldoCuentaAnt (994)).toQString ( '.' ) + "')\n\
doc.set_cell_property('bold', False)\n\
\
doc.set_cell_property('bold', True)\n\
doc.set_cell_value(2,20, 'string', 'VII. Por coberturas de flujos de efectivo.')\n\
doc.set_cell_value(4,20, 'float', '" + ( saldoCuenta (812 ) + saldoCuenta (912)).toQString ( '.' ) + "')\n\
doc.set_cell_value(5,20, 'float', '"+ ( saldoCuentaAnt (812 ) + saldoCuentaAnt (912)).toQString ( '.' ) + "')\n\
doc.set_cell_property('bold', False)\n\
\
doc.set_cell_property('bold', True)\n\
doc.set_cell_value(2,21, 'string', 'VIII. Subvenciones, donaciones y legados recibidos.')\n\
doc.set_cell_value(4,21, 'float', '" + ( saldoCuenta (84)).toQString ( '.' ) + "')\n\
doc.set_cell_value(5,21, 'float', '"+ ( saldoCuentaAnt (84)).toQString ( '.' ) + "')\n\
doc.set_cell_property('bold', False)\n\
\
doc.set_cell_property('bold', True)\n\
doc.set_cell_value(2,22, 'string', 'IX. Efecto impositivo.')\n\
doc.set_cell_value(4,22, 'float', '" + ( saldoCuenta (8301 ) + saldoCuenta (836 ) + saldoCuenta (837)).toQString ( '.' ) + "')\n\
doc.set_cell_value(5,22, 'float', '"+ ( saldoCuentaAnt (8301 ) + saldoCuentaAnt (836 ) + saldoCuentaAnt (837)).toQString ( '.' ) + "')\n\
doc.set_cell_property('bold', False)\n\
\
\
doc.set_cell_property('bold', True)\n\
doc.set_cell_value(2,24, 'string', 'C) Total transferencias a la cuenta de pérdidas y ganancias (VI+VII+VIII+IX)')\n\
doc.set_cell_value(4,24, 'formula', '=D19+D20+D21+D22')\n\
doc.set_cell_value(5,24, 'formula', '=E19+E20+E21+E22')\n\
doc.set_cell_property('bold', False)\n\
\
\
\
doc.set_cell_property('bold', True)\n\
doc.set_cell_value(2,26, 'string', 'TOTAL DE INGRESOS Y GASTOS RECONOCIDOS (A+ B +C)')\n\
doc.set_cell_value(4,26, 'formula', '=D6+D15+D24')\n\
doc.set_cell_value(5,26, 'formula', '=E6+E15+E24')\n\
doc.set_cell_property('bold', False)\n\
\
\
\
\
\
\
# Guardamos el documento\n\
doc.save(\"canualesods.ods\")\n");
}



///
/**
**/

QString pluginReportODS::cuentaAnualCAPGC08()
{
	return QString::QString("\
#!/usr/bin/python\n\
# -*- coding: utf8 -*-\n\
\n\
import ooolib\n\
\n\
# Crea el documento\n\
\
\
\
\n\
doc = ooolib.Calc(\"" + trUtf8("ACTIVO")+"\")\n\n\
doc.set_column_property(1, 'width', '0.5in')\n\
doc.set_column_property(2, 'width', '5in')\n\
\
doc.set_cell_property('bold', True)\n\
doc.set_cell_value(2,2, 'string', ' " + trUtf8("BALANCE AL CIERRE DEL EJERCICIO") + "')\n\
doc.set_cell_value(2,4, 'string', ' " + trUtf8("ACTIVO") + "')\n\
doc.set_cell_value(3,4, 'string', ' " + trUtf8("NOTAS") + "')\n\
doc.set_cell_value(4,4, 'string', ' " + trUtf8("Ejercicio N") + "')\n\
doc.set_cell_value(5,4, 'string', ' " + trUtf8("Ejercicio N-1") + "')\n\
doc.set_cell_property('bold', False)\n\
\
\
\
doc.set_cell_property('bold', True)\n\
doc.set_cell_value(2,6, 'string', ' " + trUtf8("A) ACTIVO NO CORRIENTE") + "')\n\
doc.set_cell_value(4,6, 'formula', '=D8+D15+D19+D22+D28+D34')\n\
doc.set_cell_value(5,6, 'formula', '=E8+E15+E19+E22+E28+E34')\n\
doc.set_cell_property('bold', False)\n\
\
\
\
doc.set_cell_property('bold', True)\n\
doc.set_cell_value(2,8, 'string', ' " + trUtf8("I. Inmovilizado intangible.") + "')\n\
doc.set_cell_value(4,8, 'formula', '=D9+D10+D11+D12+D13+D14')\n\
doc.set_cell_value(5,8, 'formula', '=E9+E10+E11+E12+E13+E14')\n\
doc.set_cell_property('bold', False)\n\
\
doc.set_cell_value(2,9, 'string', ' " + trUtf8("1. Desarrollo.") + "')\n\
doc.set_cell_value(4,9, 'float', '" + ( saldoCuenta (201 ) + saldoCuenta (2801 ) + saldoCuenta (2901)).toQString ( '.' ) + "')\n\
doc.set_cell_value(5,9, 'float', '"+ ( saldoCuentaAnt (201 ) + saldoCuentaAnt (2801 ) + saldoCuentaAnt (2901)).toQString ( '.' ) + "')\n\
\
doc.set_cell_value(2,10, 'string', ' " + trUtf8("2. Concesiones.") + "')\n\
doc.set_cell_value(4,10, 'float', '" + ( saldoCuenta (202 ) + saldoCuenta (2802 ) + saldoCuenta (2902)).toQString ( '.' ) + "')\n\
doc.set_cell_value(5,10, 'float', '"+ ( saldoCuentaAnt (202 ) + saldoCuentaAnt (2802 ) + saldoCuentaAnt (2902)).toQString ( '.' ) + "')\n\
\
doc.set_cell_value(2,11, 'string', ' " + trUtf8("3. Patentes, licencias, marcas y similares.") + "')\n\
doc.set_cell_value(4,11, 'float', '" + ( saldoCuenta (203 ) + saldoCuenta (2803 ) + saldoCuenta (2903)).toQString ( '.' ) + "')\n\
doc.set_cell_value(5,11, 'float', '"+ ( saldoCuentaAnt (203 ) + saldoCuentaAnt (2803 ) + saldoCuentaAnt (2903)).toQString ( '.' ) + "')\n\
\
doc.set_cell_value(2,12, 'string', ' " + trUtf8("4. Fondo de comercio.") + "')\n\
doc.set_cell_value(4,12, 'float', '" + ( saldoCuenta (204)).toQString ( '.' ) + "')\n\
doc.set_cell_value(5,12, 'float', '"+ ( saldoCuentaAnt (204)).toQString ( '.' ) + "')\n\
\
doc.set_cell_value(2,13, 'string', ' " + trUtf8("5. Aplicaciones informáticas.") + "')\n\
doc.set_cell_value(4,13, 'float', '" + ( saldoCuenta (206 ) + saldoCuenta (2806 ) + saldoCuenta (2906)).toQString ( '.' ) + "')\n\
doc.set_cell_value(5,13, 'float', '"+ ( saldoCuentaAnt (206 ) + saldoCuentaAnt (2806 ) + saldoCuentaAnt (2906)).toQString ( '.' ) + "')\n\
\
doc.set_cell_value(2,14, 'string', ' " + trUtf8("6. Otro inmovilizado intangible.") + "')\n\
doc.set_cell_value(4,14, 'float', '" + ( saldoCuenta (205 ) + saldoCuenta (209 ) + saldoCuenta (2805 ) + saldoCuenta (2905)).toQString ( '.' ) + "')\n\
doc.set_cell_value(5,14, 'float', '"+ ( saldoCuentaAnt (205 ) + saldoCuentaAnt (209 ) + saldoCuentaAnt (2805 ) + saldoCuentaAnt (2905)).toQString ( '.' ) + "')\n\
\
doc.set_cell_property('bold', True)\n\
doc.set_cell_value(2,15, 'string', ' " + trUtf8("II. Inmovilizado material.") + "')\n\
doc.set_cell_value(4,15, 'formula', '=D16+D17+D18')\n\
doc.set_cell_value(5,15, 'formula', '=E16+E17+E18')\n\
doc.set_cell_property('bold', False)\n\
\
doc.set_cell_value(2,16, 'string', ' " + trUtf8("1. Terrenos y construcciones.") + "')\n\
doc.set_cell_value(4,16, 'float', '" + ( saldoCuenta (210 ) + saldoCuenta (211 ) + saldoCuenta (2811 ) + saldoCuenta (2910 ) + saldoCuenta (2911)).toQString ( '.' ) + "')\n\
doc.set_cell_value(5,16, 'float', '"+ ( saldoCuentaAnt (210 ) + saldoCuentaAnt (211 ) + saldoCuentaAnt (2811 ) + saldoCuentaAnt (2910 ) + saldoCuentaAnt (2911)).toQString ( '.' ) + "')\n\
\
doc.set_cell_value(2,17, 'string', ' " + trUtf8("2. Instalaciones técnicas, y otro inmovilizado material.") + "')\n\
doc.set_cell_value(4,17, 'float', '" + ( saldoCuenta (212 ) + saldoCuenta (213 ) + saldoCuenta (214 ) + saldoCuenta (215 ) + saldoCuenta (216 ) + saldoCuenta (217 ) + saldoCuenta (218 ) + saldoCuenta (219 ) + saldoCuenta (2812 ) + saldoCuenta (2813 ) + saldoCuenta (2814 ) + saldoCuenta (2815 ) + saldoCuenta (2816 ) + saldoCuenta (2817 ) + saldoCuenta (2818 ) + saldoCuenta (2819 ) + saldoCuenta (2912 ) + saldoCuenta (2913 ) + saldoCuenta (2914 ) + saldoCuenta (2915 ) + saldoCuenta (2916 ) + saldoCuenta (2917 ) + saldoCuenta (2918 ) + saldoCuenta (2919)).toQString ( '.' ) + "')\n\
doc.set_cell_value(5,17, 'float', '"+ ( saldoCuentaAnt (212 ) + saldoCuentaAnt (213 ) + saldoCuentaAnt (214 ) + saldoCuentaAnt (215 ) + saldoCuentaAnt (216 ) + saldoCuentaAnt (217 ) + saldoCuentaAnt (218 ) + saldoCuentaAnt (219 ) + saldoCuentaAnt (2812 ) + saldoCuentaAnt (2813 ) + saldoCuentaAnt (2814 ) + saldoCuentaAnt (2815 ) + saldoCuentaAnt (2816 ) + saldoCuentaAnt (2817 ) + saldoCuentaAnt (2818 ) + saldoCuentaAnt (2819 ) + saldoCuentaAnt (2912 ) + saldoCuentaAnt (2913 ) + saldoCuentaAnt (2914 ) + saldoCuentaAnt (2915 ) + saldoCuentaAnt (2916 ) + saldoCuentaAnt (2917 ) + saldoCuentaAnt (2918 ) + saldoCuentaAnt (2919)).toQString ( '.' ) + "')\n\
\
doc.set_cell_value(2,18, 'string', ' " + trUtf8("3. Inmovilizado en curso y anticipos.") + "')\n\
doc.set_cell_value(4,18, 'float', '" + ( saldoCuenta (23)).toQString ( '.' ) + "')\n\
doc.set_cell_value(5,18, 'float', '"+ ( saldoCuentaAnt (23)).toQString ( '.' ) + "')\n\
\
doc.set_cell_property('bold', True)\n\
doc.set_cell_value(2,19, 'string', ' " + trUtf8("III. Inversiones inmobiliarias.") + "')\n\
doc.set_cell_value(4,19, 'formula', '=D20+D21')\n\
doc.set_cell_value(5,19, 'formula', '=E20+E21')\n\
doc.set_cell_property('bold', False)\n\
\
doc.set_cell_value(2,20, 'string', ' " + trUtf8("1. Terrenos.") + "')\n\
doc.set_cell_value(4,20, 'float', '" + ( saldoCuenta (220 ) + saldoCuenta (2920)).toQString ( '.' ) + "')\n\
doc.set_cell_value(5,20, 'float', '"+ ( saldoCuentaAnt (220 ) + saldoCuentaAnt (2920)).toQString ( '.' ) + "')\n\
\
doc.set_cell_value(2,21, 'string', ' " + trUtf8("2. Construcciones.") + "')\n\
doc.set_cell_value(4,21, 'float', '" + ( saldoCuenta (221 ) + saldoCuenta (282 ) + saldoCuenta (2921)).toQString ( '.' ) + "')\n\
doc.set_cell_value(5,21, 'float', '"+ ( saldoCuentaAnt (221 ) + saldoCuentaAnt (282 ) + saldoCuentaAnt (2921)).toQString ( '.' ) + "')\n\
\
doc.set_cell_property('bold', True)\n\
doc.set_cell_value(2,22, 'string', ' " + trUtf8("IV. Inversiones en empresas del grupo y asociadas a largo plazo.") + "')\n\
doc.set_cell_value(4,22, 'formula', '=D23+D24+D25+D+D')\n\
doc.set_cell_value(5,22, 'formula', '=E23+E24+E25+E+E')\n\
doc.set_cell_property('bold', False)\n\
\
doc.set_cell_value(2,23, 'string', ' " + trUtf8("1. Instrumentos de patrimonio.") + "')\n\
doc.set_cell_value(4,23, 'float', '" + ( saldoCuenta (2403 ) + saldoCuenta (2404 ) + saldoCuenta (2493 ) + saldoCuenta (2494 ) + saldoCuenta (293)).toQString ( '.' ) + "')\n\
doc.set_cell_value(5,23, 'float', '"+ ( saldoCuentaAnt (2403 ) + saldoCuentaAnt (2404 ) + saldoCuentaAnt (2493 ) + saldoCuentaAnt (2494 ) + saldoCuentaAnt (293)).toQString ( '.' ) + "')\n\
\
doc.set_cell_value(2,24, 'string', ' " + trUtf8("2. Créditos a empresas.") + "')\n\
doc.set_cell_value(4,24, 'float', '" + ( saldoCuenta (2423 ) + saldoCuenta (2424 ) + saldoCuenta (2953 ) + saldoCuenta (2954)).toQString ( '.' ) + "')\n\
doc.set_cell_value(5,24, 'float', '"+ ( saldoCuentaAnt (2423 ) + saldoCuentaAnt (2424 ) + saldoCuentaAnt (2953 ) + saldoCuentaAnt (2954)).toQString ( '.' ) + "')\n\
\
doc.set_cell_value(2,25, 'string', ' " + trUtf8("3. Valores representativos de deuda.") + "')\n\
doc.set_cell_value(4,25, 'float', '" + ( saldoCuenta (2413 ) + saldoCuenta (2414 ) + saldoCuenta (2943 ) + saldoCuenta (2944)).toQString ( '.' ) + "')\n\
doc.set_cell_value(5,25, 'float', '"+ ( saldoCuentaAnt (2413 ) + saldoCuentaAnt (2414 ) + saldoCuentaAnt (2943 ) + saldoCuentaAnt (2944)).toQString ( '.' ) + "')\n\
\
doc.set_cell_value(2,26, 'string', ' " + trUtf8("4. Derivados.") + "')\n\
\
doc.set_cell_value(2,27, 'string', ' " + trUtf8("5. Otros activos financieros.") + "')\n\
\
\
doc.set_cell_property('bold', True)\n\
doc.set_cell_value(2,28, 'string', ' " + trUtf8("V. Inversiones financieras a largo plazo.") + "')\n\
doc.set_cell_value(4,28, 'formula', '=D29+D30+D31+D32+D33')\n\
doc.set_cell_value(5,28, 'formula', '=E29+E30+E31+E32+E33')\n\
doc.set_cell_property('bold', False)\n\
\
doc.set_cell_value(2,29, 'string', ' " + trUtf8("1. Instrumentos de patrimonio.") + "')\n\
doc.set_cell_value(4,29, 'float', '" + ( saldoCuenta (2405 ) + saldoCuenta (2495 ) + saldoCuenta (250 ) + saldoCuenta (259)).toQString ( '.' ) + "')\n\
doc.set_cell_value(5,29, 'float', '"+ ( saldoCuentaAnt (2405 ) + saldoCuentaAnt (2495 ) + saldoCuentaAnt (250 ) + saldoCuentaAnt (259)).toQString ( '.' ) + "')\n\
\
doc.set_cell_value(2,30, 'string', ' " + trUtf8("2. Créditos a terceros") + "')\n\
doc.set_cell_value(4,30, 'float', '" + ( saldoCuenta (2425 ) + saldoCuenta (252 ) + saldoCuenta (253 ) + saldoCuenta (254 ) + saldoCuenta (2955 ) + saldoCuenta (298)).toQString ( '.' ) + "')\n\
doc.set_cell_value(5,30, 'float', '"+ ( saldoCuentaAnt (2425 ) + saldoCuentaAnt (252 ) + saldoCuentaAnt (253 ) + saldoCuentaAnt (254 ) + saldoCuentaAnt (2955 ) + saldoCuentaAnt (298)).toQString ( '.' ) + "')\n\
\
doc.set_cell_value(2,31, 'string', ' " + trUtf8("3. Valores representativos de deuda") + "')\n\
doc.set_cell_value(4,31, 'float', '" + ( saldoCuenta (2415 ) + saldoCuenta (251 ) + saldoCuenta (2945 ) + saldoCuenta (297)).toQString ( '.' ) + "')\n\
doc.set_cell_value(5,31, 'float', '"+ ( saldoCuentaAnt (2415 ) + saldoCuentaAnt (251 ) + saldoCuentaAnt (2945 ) + saldoCuentaAnt (297)).toQString ( '.' ) + "')\n\
\
doc.set_cell_value(2,32, 'string', ' " + trUtf8("4. Derivados.") + "')\n\
doc.set_cell_value(4,32, 'float', '" + ( saldoCuenta (255)).toQString ( '.' ) + "')\n\
doc.set_cell_value(5,32, 'float', '"+ ( saldoCuentaAnt (255)).toQString ( '.' ) + "')\n\
\
doc.set_cell_value(2,33, 'string', ' " + trUtf8("5. Otros activos financieros.") + "')\n\
doc.set_cell_value(4,33, 'float', '" + ( saldoCuenta (258 ) + saldoCuenta (26)).toQString ( '.' ) + "')\n\
doc.set_cell_value(5,33, 'float', '"+ ( saldoCuentaAnt (258 ) + saldoCuentaAnt (26)).toQString ( '.' ) + "')\n\
\
doc.set_cell_property('bold', True)\n\
doc.set_cell_value(2,34, 'string', ' " + trUtf8("VI. Activos por Impuesto diferido.") + "')\n\
doc.set_cell_value(4,34, 'float', '" + ( saldoCuenta (474)).toQString ( '.' ) + "')\n\
doc.set_cell_value(5,34, 'float', '"+ ( saldoCuentaAnt (474)).toQString ( '.' ) + "')\n\
doc.set_cell_property('bold', False)\n\
\
\
\
doc.set_cell_property('bold', True)\n\
doc.set_cell_value(2,36, 'string', ' " + trUtf8("B) ACTIVO CORRIENTE") + "')\n\
doc.set_cell_value(4,36, 'formula', '=D37+D38+D45+D53+D59+D65+D66')\n\
doc.set_cell_value(5,36, 'formula', '=E37+E38+E45+E53+E59+E65+E66')\n\
doc.set_cell_property('bold', False)\n\
\
\
\
doc.set_cell_property('bold', True)\n\
doc.set_cell_value(2,37, 'string', ' " + trUtf8("I. Activos no corrientes mantenidos para la venta.") + "')\n\
doc.set_cell_value(4,37, 'float', '" + ( saldoCuenta (580 ) + saldoCuenta (581 ) + saldoCuenta (582 ) + saldoCuenta (583 ) + saldoCuenta (584 ) + saldoCuenta (599)).toQString ( '.' ) + "')\n\
doc.set_cell_value(5,37, 'float', '"+ ( saldoCuentaAnt (580 ) + saldoCuentaAnt (581 ) + saldoCuentaAnt (582 ) + saldoCuentaAnt (583 ) + saldoCuentaAnt (584 ) + saldoCuentaAnt (599)).toQString ( '.' ) + "')\n\
doc.set_cell_property('bold', False)\n\
\
doc.set_cell_property('bold', True)\n\
doc.set_cell_value(2,38, 'string', ' " + trUtf8("II. Existencias.") + "')\n\
doc.set_cell_value(4,38, 'formula', '=D39+D40+D41+D42+D43+D44')\n\
doc.set_cell_value(5,38, 'formula', '=E39+E40+E41+E42+E43+E44')\n\
doc.set_cell_property('bold', False)\n\
\
doc.set_cell_value(2,39, 'string', ' " + trUtf8("1. Comerciales.") + "')\n\
doc.set_cell_value(4,39, 'float', '" + ( saldoCuenta (30 ) + saldoCuenta (390)).toQString ( '.' ) + "')\n\
doc.set_cell_value(5,39, 'float', '"+ ( saldoCuentaAnt (30 ) + saldoCuentaAnt (390)).toQString ( '.' ) + "')\n\
\
doc.set_cell_value(2,40, 'string', ' " + trUtf8("2. Materias primas y otros aprovisionamientos.") + "')\n\
doc.set_cell_value(4,40, 'float', '" + ( saldoCuenta (31 ) + saldoCuenta (32 ) + saldoCuenta (391 ) + saldoCuenta (392)).toQString ( '.' ) + "')\n\
doc.set_cell_value(5,40, 'float', '"+ ( saldoCuentaAnt (31 ) + saldoCuentaAnt (32 ) + saldoCuentaAnt (391 ) + saldoCuentaAnt (392)).toQString ( '.' ) + "')\n\
\
doc.set_cell_value(2,41, 'string', ' " + trUtf8("3. Productos en curso.") + "')\n\
doc.set_cell_value(4,41, 'float', '" + ( saldoCuenta (33 ) + saldoCuenta (34 ) + saldoCuenta (393 ) + saldoCuenta (394)).toQString ( '.' ) + "')\n\
doc.set_cell_value(5,41, 'float', '"+ ( saldoCuentaAnt (33 ) + saldoCuentaAnt (34 ) + saldoCuentaAnt (393 ) + saldoCuentaAnt (394)).toQString ( '.' ) + "')\n\
\
doc.set_cell_value(2,42, 'string', ' " + trUtf8("4. Productos terminados.") + "')\n\
doc.set_cell_value(4,42, 'float', '" + ( saldoCuenta (35 ) + saldoCuenta (395)).toQString ( '.' ) + "')\n\
doc.set_cell_value(5,42, 'float', '"+ ( saldoCuentaAnt (35 ) + saldoCuentaAnt (395)).toQString ( '.' ) + "')\n\
\
doc.set_cell_value(2,43, 'string', ' " + trUtf8("5. Subproductos, residuos y materiales recuperados.") + "')\n\
doc.set_cell_value(4,43, 'float', '" + ( saldoCuenta (36 ) + saldoCuenta (396)).toQString ( '.' ) + "')\n\
doc.set_cell_value(5,43, 'float', '"+ ( saldoCuentaAnt (36 ) + saldoCuentaAnt (396)).toQString ( '.' ) + "')\n\
\
doc.set_cell_value(2,44, 'string', ' " + trUtf8("6. Anticipos a proveedores") + "')\n\
doc.set_cell_value(4,44, 'float', '" + ( saldoCuenta (407)).toQString ( '.' ) + "')\n\
doc.set_cell_value(5,44, 'float', '"+ ( saldoCuentaAnt (407)).toQString ( '.' ) + "')\n\
\
doc.set_cell_property('bold', True)\n\
doc.set_cell_value(2,45, 'string', ' " + trUtf8("III. Deudores comerciales y otras cuentas a cobrar.") + "')\n\
doc.set_cell_value(4,45, 'formula', '=D46+D47+D48+D49+D50+D51+D52')\n\
doc.set_cell_value(5,45, 'formula', '=E46+E47+E48+E49+E50+E51+E52')\n\
doc.set_cell_property('bold', False)\n\
\
doc.set_cell_value(2,46, 'string', ' " + trUtf8("1. Clientes por ventas y prestaciones de servicios.") + "')\n\
doc.set_cell_value(4,46, 'float', '" + ( saldoCuenta (430 ) + saldoCuenta (431 ) + saldoCuenta (432 ) + saldoCuenta (435 ) + saldoCuenta (436 ) + saldoCuenta (437 ) + saldoCuenta (490 ) + saldoCuenta (4935)).toQString ( '.' ) + "')\n\
doc.set_cell_value(5,46, 'float', '"+ ( saldoCuentaAnt (430 ) + saldoCuentaAnt (431 ) + saldoCuentaAnt (432 ) + saldoCuentaAnt (435 ) + saldoCuentaAnt (436 ) + saldoCuentaAnt (437 ) + saldoCuentaAnt (490 ) + saldoCuentaAnt (4935)).toQString ( '.' ) + "')\n\
\
doc.set_cell_value(2,47, 'string', ' " + trUtf8("2. Clientes, empresas del grupo y asociadas.") + "')\n\
doc.set_cell_value(4,47, 'float', '" + ( saldoCuenta (433 ) + saldoCuenta (434 ) + saldoCuenta (4933 ) + saldoCuenta (4934)).toQString ( '.' ) + "')\n\
doc.set_cell_value(5,47, 'float', '"+ ( saldoCuentaAnt (433 ) + saldoCuentaAnt (434 ) + saldoCuentaAnt (4933 ) + saldoCuentaAnt (4934)).toQString ( '.' ) + "')\n\
\
doc.set_cell_value(2,48, 'string', ' " + trUtf8("3. Deudores varios.") + "')\n\
doc.set_cell_value(4,48, 'float', '" + ( saldoCuenta (44 ) + saldoCuenta (5531 ) + saldoCuenta (5533)).toQString ( '.' ) + "')\n\
doc.set_cell_value(5,48, 'float', '"+ ( saldoCuentaAnt (44 ) + saldoCuentaAnt (5531 ) + saldoCuentaAnt (5533)).toQString ( '.' ) + "')\n\
\
doc.set_cell_value(2,49, 'string', ' " + trUtf8("4. Personal.") + "')\n\
doc.set_cell_value(4,49, 'float', '" + ( saldoCuenta (460 ) + saldoCuenta (544)).toQString ( '.' ) + "')\n\
doc.set_cell_value(5,49, 'float', '"+ ( saldoCuentaAnt (460 ) + saldoCuentaAnt (544)).toQString ( '.' ) + "')\n\
\
doc.set_cell_value(2,50, 'string', ' " + trUtf8("5. Activos por impuesto corriente.") + "')\n\
doc.set_cell_value(4,50, 'float', '" + ( saldoCuenta (4709)).toQString ( '.' ) + "')\n\
doc.set_cell_value(5,50, 'float', '"+ ( saldoCuentaAnt (4709)).toQString ( '.' ) + "')\n\
\
doc.set_cell_value(2,51, 'string', ' " + trUtf8("6. Otros créditos con las Administraciones Públicas.") + "')\n\
doc.set_cell_value(4,51, 'float', '" + ( saldoCuenta (4700 ) + saldoCuenta (4708 ) + saldoCuenta (471 ) + saldoCuenta (472)).toQString ( '.' ) + "')\n\
doc.set_cell_value(5,51, 'float', '"+ ( saldoCuentaAnt (4700 ) + saldoCuentaAnt (4708 ) + saldoCuentaAnt (471 ) + saldoCuentaAnt (472)).toQString ( '.' ) + "')\n\
\
doc.set_cell_value(2,52, 'string', ' " + trUtf8("7. Accionistas (socios) por desembolsos exigidos") + "')\n\
doc.set_cell_value(4,52, 'float', '" + ( saldoCuenta (5580)).toQString ( '.' ) + "')\n\
doc.set_cell_value(5,52, 'float', '"+ ( saldoCuentaAnt (5580)).toQString ( '.' ) + "')\n\
\
doc.set_cell_property('bold', True)\n\
doc.set_cell_value(2,53, 'string', ' " + trUtf8("IV. Inversiones en empresas del grupo y asociadas a corto plazo.") + "')\n\
doc.set_cell_value(4,53, 'formula', '=D54+D55+D56+D+D58')\n\
doc.set_cell_value(5,53, 'formula', '=E54+E55+E56+E+E58')\n\
doc.set_cell_property('bold', False)\n\
\
doc.set_cell_value(2,54, 'string', ' " + trUtf8("1. Instrumentos de patrimonio.") + "')\n\
doc.set_cell_value(4,54, 'float', '" + ( saldoCuenta (5303 ) + saldoCuenta (5304 ) + saldoCuenta (5393 ) + saldoCuenta (5394 ) + saldoCuenta (593)).toQString ( '.' ) + "')\n\
doc.set_cell_value(5,54, 'float', '"+ ( saldoCuentaAnt (5303 ) + saldoCuentaAnt (5304 ) + saldoCuentaAnt (5393 ) + saldoCuentaAnt (5394 ) + saldoCuentaAnt (593)).toQString ( '.' ) + "')\n\
\
doc.set_cell_value(2,55, 'string', ' " + trUtf8("2. Créditos a empresas.") + "')\n\
doc.set_cell_value(4,55, 'float', '" + ( saldoCuenta (5323 ) + saldoCuenta (5324 ) + saldoCuenta (5343 ) + saldoCuenta (5344 ) + saldoCuenta (5953 ) + saldoCuenta (5954)).toQString ( '.' ) + "')\n\
doc.set_cell_value(5,55, 'float', '"+ ( saldoCuentaAnt (5323 ) + saldoCuentaAnt (5324 ) + saldoCuentaAnt (5343 ) + saldoCuentaAnt (5344 ) + saldoCuentaAnt (5953 ) + saldoCuentaAnt (5954)).toQString ( '.' ) + "')\n\
\
doc.set_cell_value(2,56, 'string', ' " + trUtf8("3. Valores representativos de deuda.") + "')\n\
doc.set_cell_value(4,56, 'float', '" + ( saldoCuenta (5313 ) + saldoCuenta (5314 ) + saldoCuenta (5333 ) + saldoCuenta (5334 ) + saldoCuenta (5943 ) + saldoCuenta (5944)).toQString ( '.' ) + "')\n\
doc.set_cell_value(5,56, 'float', '"+ ( saldoCuentaAnt (5313 ) + saldoCuentaAnt (5314 ) + saldoCuentaAnt (5333 ) + saldoCuentaAnt (5334 ) + saldoCuentaAnt (5943 ) + saldoCuentaAnt (5944)).toQString ( '.' ) + "')\n\
\
doc.set_cell_value(2,57, 'string', ' " + trUtf8("4. Derivados.") + "')\n\
\
doc.set_cell_value(2,58, 'string', ' " + trUtf8("5. Otros activos financieros.") + "')\n\
doc.set_cell_value(4,58, 'float', '" + ( saldoCuenta (5353 ) + saldoCuenta (5354 ) + saldoCuenta (5523 ) + saldoCuenta (5524)).toQString ( '.' ) + "')\n\
doc.set_cell_value(5,58, 'float', '"+ ( saldoCuentaAnt (5353 ) + saldoCuentaAnt (5354 ) + saldoCuentaAnt (5523 ) + saldoCuentaAnt (5524)).toQString ( '.' ) + "')\n\
\
doc.set_cell_property('bold', True)\n\
doc.set_cell_value(2,59, 'string', ' " + trUtf8("V. Inversiones financieras a corto plazo.") + "')\n\
doc.set_cell_value(4,59, 'formula', '=D60+D61+D62+D63+D64')\n\
doc.set_cell_value(5,59, 'formula', '=E60+E61+E62+E63+E64')\n\
doc.set_cell_property('bold', False)\n\
\
doc.set_cell_value(2,60, 'string', ' " + trUtf8("1. Instrumentos de patrimonio.") + "')\n\
doc.set_cell_value(4,60, 'float', '" + ( saldoCuenta (5305 ) + saldoCuenta (540 ) + saldoCuenta (5395 ) + saldoCuenta (549)).toQString ( '.' ) + "')\n\
doc.set_cell_value(5,60, 'float', '"+ ( saldoCuentaAnt (5305 ) + saldoCuentaAnt (540 ) + saldoCuentaAnt (5395 ) + saldoCuentaAnt (549)).toQString ( '.' ) + "')\n\
\
doc.set_cell_value(2,61, 'string', ' " + trUtf8("2. Créditos a empresas") + "')\n\
doc.set_cell_value(4,61, 'float', '" + ( saldoCuenta (5325 ) + saldoCuenta (5345 ) + saldoCuenta (542 ) + saldoCuenta (543 ) + saldoCuenta (547 ) + saldoCuenta (5955 ) + saldoCuenta (598)).toQString ( '.' ) + "')\n\
doc.set_cell_value(5,61, 'float', '"+ ( saldoCuentaAnt (5325 ) + saldoCuentaAnt (5345 ) + saldoCuentaAnt (542 ) + saldoCuentaAnt (543 ) + saldoCuentaAnt (547 ) + saldoCuentaAnt (5955 ) + saldoCuentaAnt (598)).toQString ( '.' ) + "')\n\
\
doc.set_cell_value(2,62, 'string', ' " + trUtf8("3. Valores representativos de deuda.") + "')\n\
doc.set_cell_value(4,62, 'float', '" + ( saldoCuenta (5315 ) + saldoCuenta (5335 ) + saldoCuenta (541 ) + saldoCuenta (546 ) + saldoCuenta (5945 ) + saldoCuenta (597)).toQString ( '.' ) + "')\n\
doc.set_cell_value(5,62, 'float', '"+ ( saldoCuentaAnt (5315 ) + saldoCuentaAnt (5335 ) + saldoCuentaAnt (541 ) + saldoCuentaAnt (546 ) + saldoCuentaAnt (5945 ) + saldoCuentaAnt (597)).toQString ( '.' ) + "')\n\
\
doc.set_cell_value(2,63, 'string', ' " + trUtf8("4. Derivados.") + "')\n\
doc.set_cell_value(4,63, 'float', '" + ( saldoCuenta (5590 ) + saldoCuenta (5593)).toQString ( '.' ) + "')\n\
doc.set_cell_value(5,63, 'float', '"+ ( saldoCuentaAnt (5590 ) + saldoCuentaAnt (5593)).toQString ( '.' ) + "')\n\
\
doc.set_cell_value(2,64, 'string', ' " + trUtf8("5. Otros activos financieros.") + "')\n\
doc.set_cell_value(4,64, 'float', '" + ( saldoCuenta (5355 ) + saldoCuenta (545 ) + saldoCuenta (548 ) + saldoCuenta (551 ) + saldoCuenta (5525 ) + saldoCuenta (565 ) + saldoCuenta (566)).toQString ( '.' ) + "')\n\
doc.set_cell_value(5,64, 'float', '"+ ( saldoCuentaAnt (5355 ) + saldoCuentaAnt (545 ) + saldoCuentaAnt (548 ) + saldoCuentaAnt (551 ) + saldoCuentaAnt (5525 ) + saldoCuentaAnt (565 ) + saldoCuentaAnt (566)).toQString ( '.' ) + "')\n\
\
doc.set_cell_property('bold', True)\n\
doc.set_cell_value(2,65, 'string', ' " + trUtf8("VI. Periodificaciones a corto plazo.") + "')\n\
doc.set_cell_value(4,65, 'float', '" + ( saldoCuenta (480 ) + saldoCuenta (567)).toQString ( '.' ) + "')\n\
doc.set_cell_value(5,65, 'float', '"+ ( saldoCuentaAnt (480 ) + saldoCuentaAnt (567)).toQString ( '.' ) + "')\n\
doc.set_cell_property('bold', False)\n\
\
doc.set_cell_property('bold', True)\n\
doc.set_cell_value(2,66, 'string', ' " + trUtf8("VII. Efectivo y otros activos líquidos equivalentes.") + "')\n\
doc.set_cell_value(4,66, 'formula', '=D67+D68')\n\
doc.set_cell_value(5,66, 'formula', '=E67+E68')\n\
doc.set_cell_property('bold', False)\n\
\
doc.set_cell_value(2,67, 'string', ' " + trUtf8("1. Tesorería.") + "')\n\
doc.set_cell_value(4,67, 'float', '" + ( saldoCuenta (570 ) + saldoCuenta (571 ) + saldoCuenta (572 ) + saldoCuenta (573 ) + saldoCuenta (574 ) + saldoCuenta (575)).toQString ( '.' ) + "')\n\
doc.set_cell_value(5,67, 'float', '"+ ( saldoCuentaAnt (570 ) + saldoCuentaAnt (571 ) + saldoCuentaAnt (572 ) + saldoCuentaAnt (573 ) + saldoCuentaAnt (574 ) + saldoCuentaAnt (575)).toQString ( '.' ) + "')\n\
\
doc.set_cell_value(2,68, 'string', ' " + trUtf8("2. Otros activos líquidos equivalentes.") + "')\n\
doc.set_cell_value(4,68, 'float', '" + ( saldoCuenta (576)).toQString ( '.' ) + "')\n\
doc.set_cell_value(5,68, 'float', '"+ ( saldoCuentaAnt (576)).toQString ( '.' ) + "')\n\
\
\
\
doc.set_cell_property('bold', True)\n\
doc.set_cell_value(2,70, 'string', ' " + trUtf8("TOTAL ACTIVO  (A + B)") + "')\n\
doc.set_cell_value(4,70, 'formula', '=+D6+D36')\n\
doc.set_cell_value(5,70, 'formula', '=+E6+E36')\n\
doc.set_cell_property('bold', False)\n\
\
\
\
\
doc.new_sheet(\" " + trUtf8("PATRIMONIO NETO Y PASIVO")+"\")\n\n\
doc.set_column_property(1, 'width', '0.5in')\n\
doc.set_column_property(2, 'width', '5in')\n\
\
doc.set_cell_property('bold', True)\n\
doc.set_cell_value(2,2, 'string', ' " + trUtf8("BALANCE AL CIERRE DEL EJERCICIO") + "')\n\
doc.set_cell_value(2,4, 'string', ' " + trUtf8("PATRIMONIO NETO Y PASIVO") + "')\n\
\
doc.set_cell_value(3,4, 'string', ' " + trUtf8("NOTAS") + "')\n\
doc.set_cell_value(4,4, 'string', ' " + trUtf8("Ejercicio N") + "')\n\
doc.set_cell_value(5,4, 'string', ' " + trUtf8("Ejercicio N-1") + "')\n\
doc.set_cell_property('bold', False)\n\
\
\
\
doc.set_cell_property('bold', True)\n\
doc.set_cell_value(2,6, 'string', ' " + trUtf8("A) PATRIMONIO NETO") + "')\n\
doc.set_cell_value(4,6, 'formula', '=D8+D24+D28')\n\
doc.set_cell_value(5,6, 'formula', '=E8+E24+E28')\n\
doc.set_cell_property('bold', False)\n\
\
\
\
doc.set_cell_property('bold', True)\n\
doc.set_cell_value(2,8, 'string', ' " + trUtf8("A-1) Fondos propios.") + "')\n\
doc.set_cell_value(4,8, 'formula', '=D9+D12+D13+D16+D17+D20+D21+D22+D23')\n\
doc.set_cell_value(5,8, 'formula', '=E9+E12+E13+E16+E17+E20+E21+E22+E23')\n\
doc.set_cell_property('bold', False)\n\
\
doc.set_cell_property('bold', True)\n\
doc.set_cell_value(2,9, 'string', ' " + trUtf8("I. Capital.") + "')\n\
doc.set_cell_value(4,9, 'formula', '=D10+D11')\n\
doc.set_cell_value(5,9, 'formula', '=E10+E11')\n\
doc.set_cell_property('bold', False)\n\
\
doc.set_cell_value(2,10, 'string', ' " + trUtf8("1. Capital escriturado.") + "')\n\
doc.set_cell_value(4,10, 'float', '" + ( saldoCuenta (100 ) + saldoCuenta (101 ) + saldoCuenta (102)).toQString ( '.' ) + "')\n\
doc.set_cell_value(5,10, 'float', '"+ ( saldoCuentaAnt (100 ) + saldoCuentaAnt (101 ) + saldoCuentaAnt (102)).toQString ( '.' ) + "')\n\
\
doc.set_cell_value(2,11, 'string', ' " + trUtf8("2. (Capital no exigido).") + "')\n\
doc.set_cell_value(4,11, 'float', '" + ( saldoCuenta (1030 ) + saldoCuenta (1040)).toQString ( '.' ) + "')\n\
doc.set_cell_value(5,11, 'float', '"+ ( saldoCuentaAnt (1030 ) + saldoCuentaAnt (1040)).toQString ( '.' ) + "')\n\
\
doc.set_cell_property('bold', True)\n\
doc.set_cell_value(2,12, 'string', ' " + trUtf8("II. Prima de emisión.") + "')\n\
doc.set_cell_value(4,12, 'float', '" + ( saldoCuenta (110)).toQString ( '.' ) + "')\n\
doc.set_cell_value(5,12, 'float', '"+ ( saldoCuentaAnt (110)).toQString ( '.' ) + "')\n\
doc.set_cell_property('bold', False)\n\
\
doc.set_cell_property('bold', True)\n\
doc.set_cell_value(2,13, 'string', ' " + trUtf8("III. Reservas.") + "')\n\
doc.set_cell_value(4,13, 'formula', '=D14+D15')\n\
doc.set_cell_value(5,13, 'formula', '=E14+E15')\n\
doc.set_cell_property('bold', False)\n\
\
doc.set_cell_value(2,14, 'string', ' " + trUtf8("1. Legal y estatutarias.") + "')\n\
doc.set_cell_value(4,14, 'float', '" + ( saldoCuenta (112 ) + saldoCuenta (1141)).toQString ( '.' ) + "')\n\
doc.set_cell_value(5,14, 'float', '"+ ( saldoCuentaAnt (112 ) + saldoCuentaAnt (1141)).toQString ( '.' ) + "')\n\
\
doc.set_cell_value(2,15, 'string', ' " + trUtf8("2. Otras reservas.") + "')\n\
doc.set_cell_value(4,15, 'float', '" + ( saldoCuenta (113 ) + saldoCuenta (1140 ) + saldoCuenta (1142 ) + saldoCuenta (1143 ) + saldoCuenta (1144 ) + saldoCuenta (115 ) + saldoCuenta (119)).toQString ( '.' ) + "')\n\
doc.set_cell_value(5,15, 'float', '"+ ( saldoCuentaAnt (113 ) + saldoCuentaAnt (1140 ) + saldoCuentaAnt (1142 ) + saldoCuentaAnt (1143 ) + saldoCuentaAnt (1144 ) + saldoCuentaAnt (115 ) + saldoCuentaAnt (119)).toQString ( '.' ) + "')\n\
\
doc.set_cell_property('bold', True)\n\
doc.set_cell_value(2,16, 'string', ' " + trUtf8("IV. (Acciones y participaciones en patrimonio propias).") + "')\n\
doc.set_cell_value(4,16, 'float', '" + ( saldoCuenta (108 ) + saldoCuenta (109)).toQString ( '.' ) + "')\n\
doc.set_cell_value(5,16, 'float', '"+ ( saldoCuentaAnt (108 ) + saldoCuentaAnt (109)).toQString ( '.' ) + "')\n\
doc.set_cell_property('bold', False)\n\
\
doc.set_cell_property('bold', True)\n\
doc.set_cell_value(2,17, 'string', ' " + trUtf8("V. Resultados de ejercicios anteriores.") + "')\n\
doc.set_cell_value(4,17, 'formula', '=D18+D19')\n\
doc.set_cell_value(5,17, 'formula', '=E18+E19')\n\
doc.set_cell_property('bold', False)\n\
\
doc.set_cell_value(2,18, 'string', ' " + trUtf8("1. Remanente.") + "')\n\
doc.set_cell_value(4,18, 'float', '" + ( saldoCuenta (120)).toQString ( '.' ) + "')\n\
doc.set_cell_value(5,18, 'float', '"+ ( saldoCuentaAnt (120)).toQString ( '.' ) + "')\n\
\
doc.set_cell_value(2,19, 'string', ' " + trUtf8("2. (Resultados negativos de ejercicios anteriores).") + "')\n\
doc.set_cell_value(4,19, 'float', '" + ( saldoCuenta (121)).toQString ( '.' ) + "')\n\
doc.set_cell_value(5,19, 'float', '"+ ( saldoCuentaAnt (121)).toQString ( '.' ) + "')\n\
\
doc.set_cell_property('bold', True)\n\
doc.set_cell_value(2,20, 'string', ' " + trUtf8("VI. Otras aportaciones de socios.") + "')\n\
doc.set_cell_value(4,20, 'float', '" + ( saldoCuenta (118)).toQString ( '.' ) + "')\n\
doc.set_cell_value(5,20, 'float', '"+ ( saldoCuentaAnt (118)).toQString ( '.' ) + "')\n\
doc.set_cell_property('bold', False)\n\
\
doc.set_cell_property('bold', True)\n\
doc.set_cell_value(2,21, 'string', ' " + trUtf8("VII. Resultado del ejercicio.") + "')\n\
doc.set_cell_value(4,21, 'float', '" + ( saldoCuenta (129)).toQString ( '.' ) + "')\n\
doc.set_cell_value(5,21, 'float', '"+ ( saldoCuentaAnt (129)).toQString ( '.' ) + "')\n\
doc.set_cell_property('bold', False)\n\
\
doc.set_cell_property('bold', True)\n\
doc.set_cell_value(2,22, 'string', ' " + trUtf8("VIII. (Dividendo a cuenta).") + "')\n\
doc.set_cell_value(4,22, 'float', '" + ( saldoCuenta (557)).toQString ( '.' ) + "')\n\
doc.set_cell_value(5,22, 'float', '"+ ( saldoCuentaAnt (557)).toQString ( '.' ) + "')\n\
doc.set_cell_property('bold', False)\n\
\
doc.set_cell_property('bold', True)\n\
doc.set_cell_value(2,23, 'string', ' " + trUtf8("IX. Otros instrumentos de patrimonio neto.") + "')\n\
doc.set_cell_value(4,23, 'float', '" + ( saldoCuenta (111)).toQString ( '.' ) + "')\n\
doc.set_cell_value(5,23, 'float', '"+ ( saldoCuentaAnt (111)).toQString ( '.' ) + "')\n\
doc.set_cell_property('bold', False)\n\
\
doc.set_cell_property('bold', True)\n\
doc.set_cell_value(2,24, 'string', ' " + trUtf8("A-2) Ajustes por cambios de valor.") + "')\n\
doc.set_cell_value(4,24, 'formula', '=D25+D26+D27')\n\
doc.set_cell_value(5,24, 'formula', '=E25+E26+E27')\n\
doc.set_cell_property('bold', False)\n\
\
doc.set_cell_value(2,25, 'string', ' " + trUtf8("I. Activos financieros disponibles para la venta.") + "')\n\
doc.set_cell_value(4,25, 'float', '" + ( saldoCuenta (133)).toQString ( '.' ) + "')\n\
doc.set_cell_value(5,25, 'float', '"+ ( saldoCuentaAnt (133)).toQString ( '.' ) + "')\n\
\
doc.set_cell_value(2,26, 'string', ' " + trUtf8("II. Operaciones de cobertura.") + "')\n\
doc.set_cell_value(4,26, 'float', '" + ( saldoCuenta (1340)).toQString ( '.' ) + "')\n\
doc.set_cell_value(5,26, 'float', '"+ ( saldoCuentaAnt (1340)).toQString ( '.' ) + "')\n\
\
doc.set_cell_value(2,27, 'string', ' " + trUtf8("III. Otros.") + "')\n\
doc.set_cell_value(4,27, 'float', '" + ( saldoCuenta (137)).toQString ( '.' ) + "')\n\
doc.set_cell_value(5,27, 'float', '"+ ( saldoCuentaAnt (137)).toQString ( '.' ) + "')\n\
\
doc.set_cell_property('bold', True)\n\
doc.set_cell_value(2,28, 'string', ' " + trUtf8("A-3) Subvenciones, donaciones y legados recibidos.") + "')\n\
doc.set_cell_value(4,28, 'float', '" + ( saldoCuenta (130 ) + saldoCuenta (131 ) + saldoCuenta (132)).toQString ( '.' ) + "')\n\
doc.set_cell_value(5,28, 'float', '"+ ( saldoCuentaAnt (130 ) + saldoCuentaAnt (131 ) + saldoCuentaAnt (132)).toQString ( '.' ) + "')\n\
doc.set_cell_property('bold', False)\n\
\
\
\
doc.set_cell_property('bold', True)\n\
doc.set_cell_value(2,30, 'string', ' " + trUtf8("B) PASIVO NO CORRIENTE") + "')\n\
doc.set_cell_value(4,30, 'formula', '=D32+D37+D43+D44+D45')\n\
doc.set_cell_value(5,30, 'formula', '=E32+E37+E43+E44+E45')\n\
doc.set_cell_property('bold', False)\n\
\
\
\
doc.set_cell_property('bold', True)\n\
doc.set_cell_value(2,32, 'string', ' " + trUtf8("I. Provisiones a largo plazo.") + "')\n\
doc.set_cell_value(4,32, 'formula', '=D33+D34+D35+D36')\n\
doc.set_cell_value(5,32, 'formula', '=E33+E34+E35+E36')\n\
doc.set_cell_property('bold', False)\n\
\
doc.set_cell_value(2,33, 'string', ' " + trUtf8("1. Obligaciones por prestaciones a largo plazo al personal.") + "')\n\
doc.set_cell_value(4,33, 'float', '" + ( saldoCuenta (140)).toQString ( '.' ) + "')\n\
doc.set_cell_value(5,33, 'float', '"+ ( saldoCuentaAnt (140)).toQString ( '.' ) + "')\n\
\
doc.set_cell_value(2,34, 'string', ' " + trUtf8("2. Actuaciones medioambientales.") + "')\n\
doc.set_cell_value(4,34, 'float', '" + ( saldoCuenta (145)).toQString ( '.' ) + "')\n\
doc.set_cell_value(5,34, 'float', '"+ ( saldoCuentaAnt (145)).toQString ( '.' ) + "')\n\
\
doc.set_cell_value(2,35, 'string', ' " + trUtf8("3. Provisiones por reestructuración.") + "')\n\
doc.set_cell_value(4,35, 'float', '" + ( saldoCuenta (146)).toQString ( '.' ) + "')\n\
doc.set_cell_value(5,35, 'float', '"+ ( saldoCuentaAnt (146)).toQString ( '.' ) + "')\n\
\
doc.set_cell_value(2,36, 'string', ' " + trUtf8("4. Otras provisiones.") + "')\n\
doc.set_cell_value(4,36, 'float', '" + ( saldoCuenta (141 ) + saldoCuenta (142 ) + saldoCuenta (143 ) + saldoCuenta (147)).toQString ( '.' ) + "')\n\
doc.set_cell_value(5,36, 'float', '"+ ( saldoCuentaAnt (141 ) + saldoCuentaAnt (142 ) + saldoCuentaAnt (143 ) + saldoCuentaAnt (147)).toQString ( '.' ) + "')\n\
\
doc.set_cell_property('bold', True)\n\
doc.set_cell_value(2,37, 'string', ' " + trUtf8("II. Deudas a largo plazo.") + "')\n\
doc.set_cell_value(4,37, 'formula', '=D38+D39+D40+D41+D42')\n\
doc.set_cell_value(5,37, 'formula', '=E38+E39+E40+E41+E42')\n\
doc.set_cell_property('bold', False)\n\
\
doc.set_cell_value(2,38, 'string', ' " + trUtf8("1. Obligaciones y otros valores negociables.") + "')\n\
doc.set_cell_value(4,38, 'float', '" + ( saldoCuenta (177 ) + saldoCuenta (178 ) + saldoCuenta (179)).toQString ( '.' ) + "')\n\
doc.set_cell_value(5,38, 'float', '"+ ( saldoCuentaAnt (177 ) + saldoCuentaAnt (178 ) + saldoCuentaAnt (179)).toQString ( '.' ) + "')\n\
\
doc.set_cell_value(2,39, 'string', ' " + trUtf8("2. Deudas con entidades de crédito.") + "')\n\
doc.set_cell_value(4,39, 'float', '" + ( saldoCuenta (1605 ) + saldoCuenta (170)).toQString ( '.' ) + "')\n\
doc.set_cell_value(5,39, 'float', '"+ ( saldoCuentaAnt (1605 ) + saldoCuentaAnt (170)).toQString ( '.' ) + "')\n\
\
doc.set_cell_value(2,40, 'string', ' " + trUtf8("3. Acreedores por arrendamiento financiero.") + "')\n\
doc.set_cell_value(4,40, 'float', '" + ( saldoCuenta (1625 ) + saldoCuenta (174)).toQString ( '.' ) + "')\n\
doc.set_cell_value(5,40, 'float', '"+ ( saldoCuentaAnt (1625 ) + saldoCuentaAnt (174)).toQString ( '.' ) + "')\n\
\
doc.set_cell_value(2,41, 'string', ' " + trUtf8("4. Derivados.") + "')\n\
doc.set_cell_value(4,41, 'float', '" + ( saldoCuenta (176)).toQString ( '.' ) + "')\n\
doc.set_cell_value(5,41, 'float', '"+ ( saldoCuentaAnt (176)).toQString ( '.' ) + "')\n\
\
doc.set_cell_value(2,42, 'string', ' " + trUtf8("5. Otros pasivos financieros.") + "')\n\
doc.set_cell_value(4,42, 'float', '" + ( saldoCuenta (1615 ) + saldoCuenta (1635 ) + saldoCuenta (171 ) + saldoCuenta (172 ) + saldoCuenta (173 ) + saldoCuenta (175 ) + saldoCuenta (180 ) + saldoCuenta (185 ) + saldoCuenta (189)).toQString ( '.' ) + "')\n\
doc.set_cell_value(5,42, 'float', '"+ ( saldoCuentaAnt (1615 ) + saldoCuentaAnt (1635 ) + saldoCuentaAnt (171 ) + saldoCuentaAnt (172 ) + saldoCuentaAnt (173 ) + saldoCuentaAnt (175 ) + saldoCuentaAnt (180 ) + saldoCuentaAnt (185 ) + saldoCuentaAnt (189)).toQString ( '.' ) + "')\n\
\
doc.set_cell_property('bold', True)\n\
doc.set_cell_value(2,43, 'string', ' " + trUtf8("III. Deudas con empresas del grupo y asociadas a largo plazo.") + "')\n\
doc.set_cell_value(4,43, 'float', '" + ( saldoCuenta (1603 ) + saldoCuenta (1604 ) + saldoCuenta (1613 ) + saldoCuenta (1614 ) + saldoCuenta (1623 ) + saldoCuenta (1624 ) + saldoCuenta (1633 ) + saldoCuenta (1634)).toQString ( '.' ) + "')\n\
doc.set_cell_value(5,43, 'float', '"+ ( saldoCuentaAnt (1603 ) + saldoCuentaAnt (1604 ) + saldoCuentaAnt (1613 ) + saldoCuentaAnt (1614 ) + saldoCuentaAnt (1623 ) + saldoCuentaAnt (1624 ) + saldoCuentaAnt (1633 ) + saldoCuentaAnt (1634)).toQString ( '.' ) + "')\n\
doc.set_cell_property('bold', False)\n\
\
doc.set_cell_property('bold', True)\n\
doc.set_cell_value(2,44, 'string', ' " + trUtf8("IV. Pasivos por impuesto diferido.") + "')\n\
doc.set_cell_value(4,44, 'float', '" + ( saldoCuenta (479)).toQString ( '.' ) + "')\n\
doc.set_cell_value(5,44, 'float', '"+ ( saldoCuentaAnt (479)).toQString ( '.' ) + "')\n\
doc.set_cell_property('bold', False)\n\
\
doc.set_cell_property('bold', True)\n\
doc.set_cell_value(2,45, 'string', ' " + trUtf8("V. Periodificaciones a largo plazo.") + "')\n\
doc.set_cell_value(4,45, 'float', '" + ( saldoCuenta (181)).toQString ( '.' ) + "')\n\
doc.set_cell_value(5,45, 'float', '"+ ( saldoCuentaAnt (181)).toQString ( '.' ) + "')\n\
doc.set_cell_property('bold', False)\n\
\
\
\
doc.set_cell_property('bold', True)\n\
doc.set_cell_value(2,47, 'string', ' " + trUtf8("C) PASIVO CORRIENTE") + "')\n\
doc.set_cell_value(4,47, 'formula', '=D49+D50+D51+D57+D58+D66')\n\
doc.set_cell_value(5,47, 'formula', '=E49+E50+E51+E57+E58+E66')\n\
doc.set_cell_property('bold', False)\n\
\
\
\
doc.set_cell_property('bold', True)\n\
doc.set_cell_value(2,49, 'string', ' " + trUtf8("I. Pasivos vinculados con activos no corrientes mantenidos para la venta.") + "')\n\
doc.set_cell_value(4,49, 'float', '" + ( saldoCuenta (585 ) + saldoCuenta (586 ) + saldoCuenta (587 ) + saldoCuenta (588 ) + saldoCuenta (589)).toQString ( '.' ) + "')\n\
doc.set_cell_value(5,49, 'float', '"+ ( saldoCuentaAnt (585 ) + saldoCuentaAnt (586 ) + saldoCuentaAnt (587 ) + saldoCuentaAnt (588 ) + saldoCuentaAnt (589)).toQString ( '.' ) + "')\n\
doc.set_cell_property('bold', False)\n\
\
doc.set_cell_property('bold', True)\n\
doc.set_cell_value(2,50, 'string', ' " + trUtf8("II. Provisiones a corto plazo.") + "')\n\
doc.set_cell_value(4,50, 'float', '" + ( saldoCuenta (499 ) + saldoCuenta (529)).toQString ( '.' ) + "')\n\
doc.set_cell_value(5,50, 'float', '"+ ( saldoCuentaAnt (499 ) + saldoCuentaAnt (529)).toQString ( '.' ) + "')\n\
doc.set_cell_property('bold', False)\n\
\
\
doc.set_cell_property('bold', True)\n\
doc.set_cell_value(2,51, 'string', ' " + trUtf8("III. Deudas a corto plazo.") + "')\n\
doc.set_cell_value(4,51, 'formula', '=D52+D53+D54+D55+D56')\n\
doc.set_cell_value(5,51, 'formula', '=E52+E53+E54+E55+E56')\n\
doc.set_cell_property('bold', False)\n\
\
doc.set_cell_value(2,52, 'string', ' " + trUtf8("1. Obligaciones y otros valores negociables.") + "')\n\
doc.set_cell_value(4,52, 'float', '" + ( saldoCuenta (500 ) + saldoCuenta (501 ) + saldoCuenta (505 ) + saldoCuenta (506)).toQString ( '.' ) + "')\n\
doc.set_cell_value(5,52, 'float', '"+ ( saldoCuentaAnt (500 ) + saldoCuentaAnt (501 ) + saldoCuentaAnt (505 ) + saldoCuentaAnt (506)).toQString ( '.' ) + "')\n\
\
doc.set_cell_value(2,53, 'string', ' " + trUtf8("2. Deudas con entidades de crédito.") + "')\n\
doc.set_cell_value(4,53, 'float', '" + ( saldoCuenta (5105 ) + saldoCuenta (520 ) + saldoCuenta (527)).toQString ( '.' ) + "')\n\
doc.set_cell_value(5,53, 'float', '"+ ( saldoCuentaAnt (5105 ) + saldoCuentaAnt (520 ) + saldoCuentaAnt (527)).toQString ( '.' ) + "')\n\
\
doc.set_cell_value(2,54, 'string', ' " + trUtf8("3. Acreedores por arrendamiento financiero.") + "')\n\
doc.set_cell_value(4,54, 'float', '" + ( saldoCuenta (5125 ) + saldoCuenta (524)).toQString ( '.' ) + "')\n\
doc.set_cell_value(5,54, 'float', '"+ ( saldoCuentaAnt (5125 ) + saldoCuentaAnt (524)).toQString ( '.' ) + "')\n\
\
doc.set_cell_value(2,55, 'string', ' " + trUtf8("4. Derivados.") + "')\n\
doc.set_cell_value(4,55, 'float', '" + ( saldoCuenta (5595 ) + saldoCuenta (5598)).toQString ( '.' ) + "')\n\
doc.set_cell_value(5,55, 'float', '"+ ( saldoCuentaAnt (5595 ) + saldoCuentaAnt (5598)).toQString ( '.' ) + "')\n\
\
doc.set_cell_value(2,56, 'string', ' " + trUtf8("5. Otros pasivos financieros.") + "')\n\
doc.set_cell_value(4,56, 'float', '" + ( saldoCuenta (1034 ) + saldoCuenta (1044 ) + saldoCuenta (190 ) + saldoCuenta (192 ) + saldoCuenta (194 ) + saldoCuenta (509 ) + saldoCuenta (5115 ) + saldoCuenta (5135 ) + saldoCuenta (5145 ) + saldoCuenta (521 ) + saldoCuenta (522 ) + saldoCuenta (523 ) + saldoCuenta (525 ) + saldoCuenta (526 ) + saldoCuenta (528 ) + saldoCuenta (551 ) + saldoCuenta (5525 ) + saldoCuenta (5530 ) + saldoCuenta (5532 ) + saldoCuenta (555 ) + saldoCuenta (5565 ) + saldoCuenta (5566 ) + saldoCuenta (560 ) + saldoCuenta (561 ) + saldoCuenta (569)).toQString ( '.' ) + "')\n\
doc.set_cell_value(5,56, 'float', '"+ ( saldoCuentaAnt (1034 ) + saldoCuentaAnt (1044 ) + saldoCuentaAnt (190 ) + saldoCuentaAnt (192 ) + saldoCuentaAnt (194 ) + saldoCuentaAnt (509 ) + saldoCuentaAnt (5115 ) + saldoCuentaAnt (5135 ) + saldoCuentaAnt (5145 ) + saldoCuentaAnt (521 ) + saldoCuentaAnt (522 ) + saldoCuentaAnt (523 ) + saldoCuentaAnt (525 ) + saldoCuentaAnt (526 ) + saldoCuentaAnt (528 ) + saldoCuentaAnt (551 ) + saldoCuentaAnt (5525 ) + saldoCuentaAnt (5530 ) + saldoCuentaAnt (5532 ) + saldoCuentaAnt (555 ) + saldoCuentaAnt (5565 ) + saldoCuentaAnt (5566 ) + saldoCuentaAnt (560 ) + saldoCuentaAnt (561 ) + saldoCuentaAnt (569)).toQString ( '.' ) + "')\n\
\
doc.set_cell_property('bold', True)\n\
doc.set_cell_value(2,57, 'string', ' " + trUtf8("IV. Deudas con empresas del grupo y asociadas a corto plazo.") + "')\n\
doc.set_cell_value(4,57, 'float', '" + ( saldoCuenta (5103 ) + saldoCuenta (5104 ) + saldoCuenta (5113 ) + saldoCuenta (5114 ) + saldoCuenta (5123 ) + saldoCuenta (5124 ) + saldoCuenta (5133 ) + saldoCuenta (5134 ) + saldoCuenta (5143 ) + saldoCuenta (5144 ) + saldoCuenta (5523 ) + saldoCuenta (5524 ) + saldoCuenta (5563 ) + saldoCuenta (5564)).toQString ( '.' ) + "')\n\
doc.set_cell_value(5,57, 'float', '"+ ( saldoCuentaAnt (5103 ) + saldoCuentaAnt (5104 ) + saldoCuentaAnt (5113 ) + saldoCuentaAnt (5114 ) + saldoCuentaAnt (5123 ) + saldoCuentaAnt (5124 ) + saldoCuentaAnt (5133 ) + saldoCuentaAnt (5134 ) + saldoCuentaAnt (5143 ) + saldoCuentaAnt (5144 ) + saldoCuentaAnt (5523 ) + saldoCuentaAnt (5524 ) + saldoCuentaAnt (5563 ) + saldoCuentaAnt (5564)).toQString ( '.' ) + "')\n\
doc.set_cell_property('bold', False)\n\
\
doc.set_cell_property('bold', True)\n\
doc.set_cell_value(2,58, 'string', ' " + trUtf8("V. Acreedores comerciales y otras cuentas a pagar.") + "')\n\
doc.set_cell_value(4,58, 'formula', '=D59+D60+D61+D62+D63+D64+D65')\n\
doc.set_cell_value(5,58, 'formula', '=E59+E60+E61+E62+E63+E64+E65')\n\
doc.set_cell_property('bold', False)\n\
\
doc.set_cell_value(2,59, 'string', ' " + trUtf8("1. Proveedores") + "')\n\
doc.set_cell_value(4,59, 'float', '" + ( saldoCuenta (400 ) + saldoCuenta (401 ) + saldoCuenta (405 ) + saldoCuenta (406)).toQString ( '.' ) + "')\n\
doc.set_cell_value(5,59, 'float', '"+ ( saldoCuentaAnt (400 ) + saldoCuentaAnt (401 ) + saldoCuentaAnt (405 ) + saldoCuentaAnt (406)).toQString ( '.' ) + "')\n\
\
doc.set_cell_value(2,60, 'string', ' " + trUtf8("2. Proveedores, empresas del grupo y asociadas.") + "')\n\
doc.set_cell_value(4,60, 'float', '" + ( saldoCuenta (403 ) + saldoCuenta (404)).toQString ( '.' ) + "')\n\
doc.set_cell_value(5,60, 'float', '"+ ( saldoCuentaAnt (403 ) + saldoCuentaAnt (404)).toQString ( '.' ) + "')\n\
\
doc.set_cell_value(2,61, 'string', ' " + trUtf8("3. Acreedores varios.") + "')\n\
doc.set_cell_value(4,61, 'float', '" + ( saldoCuenta (41)).toQString ( '.' ) + "')\n\
doc.set_cell_value(5,61, 'float', '"+ ( saldoCuentaAnt (41)).toQString ( '.' ) + "')\n\
\
doc.set_cell_value(2,62, 'string', ' " + trUtf8("4. Personal (remuneraciones pendientes de pago).") + "')\n\
doc.set_cell_value(4,62, 'float', '" + ( saldoCuenta (465 ) + saldoCuenta (466)).toQString ( '.' ) + "')\n\
doc.set_cell_value(5,62, 'float', '"+ ( saldoCuentaAnt (465 ) + saldoCuentaAnt (466)).toQString ( '.' ) + "')\n\
\
doc.set_cell_value(2,63, 'string', ' " + trUtf8("5. Pasivos por impuesto corriente.") + "')\n\
doc.set_cell_value(4,63, 'float', '" + ( saldoCuenta (4752)).toQString ( '.' ) + "')\n\
doc.set_cell_value(5,63, 'float', '"+ ( saldoCuentaAnt (4752)).toQString ( '.' ) + "')\n\
\
doc.set_cell_value(2,64, 'string', ' " + trUtf8("6. Otras deudas con las Administraciones Públicas.") + "')\n\
doc.set_cell_value(4,64, 'float', '" + ( saldoCuenta (4750 ) + saldoCuenta (4751 ) + saldoCuenta (4758 ) + saldoCuenta (476 ) + saldoCuenta (477)).toQString ( '.' ) + "')\n\
doc.set_cell_value(5,64, 'float', '"+ ( saldoCuentaAnt (4750 ) + saldoCuentaAnt (4751 ) + saldoCuentaAnt (4758 ) + saldoCuentaAnt (476 ) + saldoCuentaAnt (477)).toQString ( '.' ) + "')\n\
\
doc.set_cell_value(2,65, 'string', ' " + trUtf8("7. Anticipos de clientes.") + "')\n\
doc.set_cell_value(4,65, 'float', '" + ( saldoCuenta (438)).toQString ( '.' ) + "')\n\
doc.set_cell_value(5,65, 'float', '"+ ( saldoCuentaAnt (438)).toQString ( '.' ) + "')\n\
\
doc.set_cell_property('bold', True)\n\
doc.set_cell_value(2,66, 'string', ' " + trUtf8("VI. Periodificaciones a corto plazo.") + "')\n\
doc.set_cell_value(4,66, 'float', '" + ( saldoCuenta (485 ) + saldoCuenta (568)).toQString ( '.' ) + "')\n\
doc.set_cell_value(5,66, 'float', '"+ ( saldoCuentaAnt (485 ) + saldoCuentaAnt (568)).toQString ( '.' ) + "')\n\
doc.set_cell_property('bold', False)\n\
\
\
\
\
doc.set_cell_property('bold', True)\n\
doc.set_cell_value(2,68, 'string', ' " + trUtf8("TOTAL PATRIMONIO NETO Y PASIVO (A + B + C)") + "')\n\
doc.set_cell_value(4,68, 'formula', '=D6+D30+D47')\n\
doc.set_cell_value(5,68, 'formula', '=E6+E30+E47')\n\
doc.set_cell_property('bold', False)\n\
\
\
\
\
doc.new_sheet(\" " + trUtf8("PERDIDAS_Y_GANANCIAS")+"\")\n\n\
doc.set_column_property(1, 'width', '0.5in')\n\
doc.set_column_property(2, 'width', '5in')\n\
\
doc.set_cell_property('bold', True)\n\
doc.set_cell_value(2,2, 'string', ' " + trUtf8("CUENTA DE PÉRDIDAS Y GANANCIAS CORRESPONDIENTE AL EJERCICIO TERMINADO") + "')\n\
doc.set_cell_value(2,4, 'string', ' " + trUtf8("PERDIDAS_Y_GANANCIAS") + "')\n\
\
doc.set_cell_value(3,4, 'string', ' " + trUtf8("NOTAS") + "')\n\
doc.set_cell_value(4,4, 'string', ' " + trUtf8("Ejercicio N") + "')\n\
doc.set_cell_value(5,4, 'string', ' " + trUtf8("Ejercicio N-1") + "')\n\
doc.set_cell_property('bold', False)\n\
\
\
\
doc.set_cell_property('bold', True)\n\
doc.set_cell_value(2,6, 'string', ' " + trUtf8("A) OPERACIONES CONTINUADAS") + "')\n\
doc.set_cell_property('bold', False)\n\
\
\
\
doc.set_cell_property('bold', True)\n\
doc.set_cell_value(2,8, 'string', ' " + trUtf8("1. Importe neto de la cifra de negocios.") + "')\n\
doc.set_cell_value(4,8, 'formula', '=D9+D10')\n\
doc.set_cell_value(5,8, 'formula', '=E9+E10')\n\
doc.set_cell_property('bold', False)\n\
\
doc.set_cell_value(2,9, 'string', ' " + trUtf8("a) Ventas.") + "')\n\
doc.set_cell_value(4,9, 'float', '" + ( saldoCuenta (700 ) + saldoCuenta (701 ) + saldoCuenta (702 ) + saldoCuenta (703 ) + saldoCuenta (704 ) + saldoCuenta (706 ) + saldoCuenta (708 ) + saldoCuenta (709)).toQString ( '.' ) + "')\n\
doc.set_cell_value(5,9, 'float', '"+ ( saldoCuentaAnt (700 ) + saldoCuentaAnt (701 ) + saldoCuentaAnt (702 ) + saldoCuentaAnt (703 ) + saldoCuentaAnt (704 ) + saldoCuentaAnt (706 ) + saldoCuentaAnt (708 ) + saldoCuentaAnt (709)).toQString ( '.' ) + "')\n\
\
doc.set_cell_value(2,10, 'string', ' " + trUtf8("b) Prestaciones de servicios.") + "')\n\
doc.set_cell_value(4,10, 'float', '" + ( saldoCuenta (705)).toQString ( '.' ) + "')\n\
doc.set_cell_value(5,10, 'float', '"+ ( saldoCuentaAnt (705)).toQString ( '.' ) + "')\n\
\
doc.set_cell_property('bold', True)\n\
doc.set_cell_value(2,11, 'string', ' " + trUtf8("2. Variación de existencias de productos terminados y en curso de fabricación.") + "')\n\
doc.set_cell_value(4,11, 'float', '" + ( saldoCuenta (6930 ) + saldoCuenta (71 ) + saldoCuenta (7930)).toQString ( '.' ) + "')\n\
doc.set_cell_value(5,11, 'float', '"+ ( saldoCuentaAnt (6930 ) + saldoCuentaAnt (71 ) + saldoCuentaAnt (7930)).toQString ( '.' ) + "')\n\
doc.set_cell_property('bold', False)\n\
\
doc.set_cell_property('bold', True)\n\
doc.set_cell_value(2,12, 'string', ' " + trUtf8("3. Trabajos realizados por la empresa para su activo.") + "')\n\
doc.set_cell_value(4,12, 'float', '" + ( saldoCuenta (73)).toQString ( '.' ) + "')\n\
doc.set_cell_value(5,12, 'float', '"+ ( saldoCuentaAnt (73)).toQString ( '.' ) + "')\n\
doc.set_cell_property('bold', False)\n\
\
doc.set_cell_property('bold', True)\n\
doc.set_cell_value(2,13, 'string', ' " + trUtf8("4. Aprovisionamientos.") + "')\n\
doc.set_cell_value(4,13, 'formula', '=D14+D15+D16+D17')\n\
doc.set_cell_value(5,13, 'formula', '=E14+E15+E16+E17')\n\
doc.set_cell_property('bold', False)\n\
\
doc.set_cell_value(2,14, 'string', ' " + trUtf8("a) Consumo de mercaderías.") + "')\n\
doc.set_cell_value(4,14, 'float', '" + ( saldoCuenta (600 ) + saldoCuenta (6060 ) + saldoCuenta (6080 ) + saldoCuenta (6090 ) + saldoCuenta (610)).toQString ( '.' ) + "')\n\
doc.set_cell_value(5,14, 'float', '"+ ( saldoCuentaAnt (600 ) + saldoCuentaAnt (6060 ) + saldoCuentaAnt (6080 ) + saldoCuentaAnt (6090 ) + saldoCuentaAnt (610)).toQString ( '.' ) + "')\n\
\
doc.set_cell_value(2,15, 'string', ' " + trUtf8("b) Consumo de materias primas y otras materias consumibles.") + "')\n\
doc.set_cell_value(4,15, 'float', '" + ( saldoCuenta (601 ) + saldoCuenta (602 ) + saldoCuenta (6061 ) + saldoCuenta (6062 ) + saldoCuenta (6081 ) + saldoCuenta (6082 ) + saldoCuenta (6091 ) + saldoCuenta (6092 ) + saldoCuenta (611 ) + saldoCuenta (612)).toQString ( '.' ) + "')\n\
doc.set_cell_value(5,15, 'float', '"+ ( saldoCuentaAnt (601 ) + saldoCuentaAnt (602 ) + saldoCuentaAnt (6061 ) + saldoCuentaAnt (6062 ) + saldoCuentaAnt (6081 ) + saldoCuentaAnt (6082 ) + saldoCuentaAnt (6091 ) + saldoCuentaAnt (6092 ) + saldoCuentaAnt (611 ) + saldoCuentaAnt (612)).toQString ( '.' ) + "')\n\
\
doc.set_cell_value(2,16, 'string', ' " + trUtf8("c) Trabajos realizados por otras empresas.") + "')\n\
doc.set_cell_value(4,16, 'float', '" + ( saldoCuenta (607)).toQString ( '.' ) + "')\n\
doc.set_cell_value(5,16, 'float', '"+ ( saldoCuentaAnt (607)).toQString ( '.' ) + "')\n\
\
doc.set_cell_value(2,17, 'string', ' " + trUtf8("d) Deterioro de mercaderías, materias primas y otros aprovisionamientos.") + "')\n\
doc.set_cell_value(4,17, 'float', '" + ( saldoCuenta (6931 ) + saldoCuenta (6932 ) + saldoCuenta (6933 ) + saldoCuenta (7931 ) + saldoCuenta (7932 ) + saldoCuenta (7933)).toQString ( '.' ) + "')\n\
doc.set_cell_value(5,17, 'float', '"+ ( saldoCuentaAnt (6931 ) + saldoCuentaAnt (6932 ) + saldoCuentaAnt (6933 ) + saldoCuentaAnt (7931 ) + saldoCuentaAnt (7932 ) + saldoCuentaAnt (7933)).toQString ( '.' ) + "')\n\
\
doc.set_cell_property('bold', True)\n\
doc.set_cell_value(2,18, 'string', ' " + trUtf8("5. Otros ingresos de explotación.") + "')\n\
doc.set_cell_value(4,18, 'formula', '=D19+D20')\n\
doc.set_cell_value(5,18, 'formula', '=E19+E20')\n\
doc.set_cell_property('bold', False)\n\
\
doc.set_cell_value(2,19, 'string', ' " + trUtf8("a) Ingresos accesorios y otros de gestiÃ³n corriente.") + "')\n\
doc.set_cell_value(4,19, 'float', '" + ( saldoCuenta (75)).toQString ( '.' ) + "')\n\
doc.set_cell_value(5,19, 'float', '"+ ( saldoCuentaAnt (75)).toQString ( '.' ) + "')\n\
\
doc.set_cell_value(2,20, 'string', ' " + trUtf8("b) Subvenciones de explotación incorporadas al resultado del ejercicio.") + "')\n\
doc.set_cell_value(4,20, 'float', '" + ( saldoCuenta (740 ) + saldoCuenta (747)).toQString ( '.' ) + "')\n\
doc.set_cell_value(5,20, 'float', '"+ ( saldoCuentaAnt (740 ) + saldoCuentaAnt (747)).toQString ( '.' ) + "')\n\
\
doc.set_cell_property('bold', True)\n\
doc.set_cell_value(2,21, 'string', ' " + trUtf8("6. Gastos de personal.") + "')\n\
doc.set_cell_value(4,21, 'formula', '=D22+D23+D24')\n\
doc.set_cell_value(5,21, 'formula', '=E22+E23+E24')\n\
doc.set_cell_property('bold', False)\n\
\
doc.set_cell_value(2,22, 'string', ' " + trUtf8("a) Sueldos, salarios y asimilados.") + "')\n\
doc.set_cell_value(4,22, 'float', '" + ( saldoCuenta (640 ) + saldoCuenta (641 ) + saldoCuenta (6450)).toQString ( '.' ) + "')\n\
doc.set_cell_value(5,22, 'float', '"+ ( saldoCuentaAnt (640 ) + saldoCuentaAnt (641 ) + saldoCuentaAnt (6450)).toQString ( '.' ) + "')\n\
\
doc.set_cell_value(2,23, 'string', ' " + trUtf8("b) Cargas sociales.") + "')\n\
doc.set_cell_value(4,23, 'float', '" + ( saldoCuenta (642 ) + saldoCuenta (643 ) + saldoCuenta (649)).toQString ( '.' ) + "')\n\
doc.set_cell_value(5,23, 'float', '"+ ( saldoCuentaAnt (642 ) + saldoCuentaAnt (643 ) + saldoCuentaAnt (649)).toQString ( '.' ) + "')\n\
\
doc.set_cell_value(2,24, 'string', ' " + trUtf8("c) Provisiones.") + "')\n\
doc.set_cell_value(4,24, 'float', '" + ( saldoCuenta (644 ) + saldoCuenta (6457 ) + saldoCuenta (7950 ) + saldoCuenta (7957)).toQString ( '.' ) + "')\n\
doc.set_cell_value(5,24, 'float', '"+ ( saldoCuentaAnt (644 ) + saldoCuentaAnt (6457 ) + saldoCuentaAnt (7950 ) + saldoCuentaAnt (7957)).toQString ( '.' ) + "')\n\
\
doc.set_cell_property('bold', True)\n\
doc.set_cell_value(2,25, 'string', ' " + trUtf8("7. Otros gastos de explotación.") + "')\n\
doc.set_cell_value(4,25, 'formula', '=D26+D27+D28+D29')\n\
doc.set_cell_value(5,25, 'formula', '=E26+E27+E28+E29')\n\
doc.set_cell_property('bold', False)\n\
\
doc.set_cell_value(2,26, 'string', ' " + trUtf8("a) Servicios exteriores.") + "')\n\
doc.set_cell_value(4,26, 'float', '" + ( saldoCuenta (62)).toQString ( '.' ) + "')\n\
doc.set_cell_value(5,26, 'float', '"+ ( saldoCuentaAnt (62)).toQString ( '.' ) + "')\n\
\
doc.set_cell_value(2,27, 'string', ' " + trUtf8("b) Tributos.") + "')\n\
doc.set_cell_value(4,27, 'float', '" + ( saldoCuenta (631 ) + saldoCuenta (634 ) + saldoCuenta (636 ) + saldoCuenta (639)).toQString ( '.' ) + "')\n\
doc.set_cell_value(5,27, 'float', '"+ ( saldoCuentaAnt (631 ) + saldoCuentaAnt (634 ) + saldoCuentaAnt (636 ) + saldoCuentaAnt (639)).toQString ( '.' ) + "')\n\
\
doc.set_cell_value(2,28, 'string', ' " + trUtf8("c) Pérdidas, deterioro y variación de provisiones por operaciones comerciales.") + "')\n\
doc.set_cell_value(4,28, 'float', '" + ( saldoCuenta (650 ) + saldoCuenta (694 ) + saldoCuenta (695 ) + saldoCuenta (794 ) + saldoCuenta (7954)).toQString ( '.' ) + "')\n\
doc.set_cell_value(5,28, 'float', '"+ ( saldoCuentaAnt (650 ) + saldoCuentaAnt (694 ) + saldoCuentaAnt (695 ) + saldoCuentaAnt (794 ) + saldoCuentaAnt (7954)).toQString ( '.' ) + "')\n\
\
doc.set_cell_value(2,29, 'string', ' " + trUtf8("d) Otros gastos de gestión corriente") + "')\n\
doc.set_cell_value(4,29, 'float', '" + ( saldoCuenta (651 ) + saldoCuenta (659)).toQString ( '.' ) + "')\n\
doc.set_cell_value(5,29, 'float', '"+ ( saldoCuentaAnt (651 ) + saldoCuentaAnt (659)).toQString ( '.' ) + "')\n\
\
doc.set_cell_property('bold', True)\n\
doc.set_cell_value(2,30, 'string', ' " + trUtf8("8. Amortización del inmovilizado.") + "')\n\
doc.set_cell_value(4,30, 'float', '" + ( saldoCuenta (68)).toQString ( '.' ) + "')\n\
doc.set_cell_value(5,30, 'float', '"+ ( saldoCuentaAnt (68)).toQString ( '.' ) + "')\n\
doc.set_cell_property('bold', False)\n\
\
doc.set_cell_property('bold', True)\n\
doc.set_cell_value(2,31, 'string', ' " + trUtf8("9. Imputación de subvenciones de inmovilizado no financiero y otras.") + "')\n\
doc.set_cell_value(4,31, 'float', '" + ( saldoCuenta (746)).toQString ( '.' ) + "')\n\
doc.set_cell_value(5,31, 'float', '"+ ( saldoCuentaAnt (746)).toQString ( '.' ) + "')\n\
doc.set_cell_property('bold', False)\n\
\
doc.set_cell_property('bold', True)\n\
doc.set_cell_value(2,32, 'string', ' " + trUtf8("10. Excesos de provisiones.") + "')\n\
doc.set_cell_value(4,32, 'float', '" + ( saldoCuenta (7951 ) + saldoCuenta (7952 ) + saldoCuenta (7955 ) + saldoCuenta (7956)).toQString ( '.' ) + "')\n\
doc.set_cell_value(5,32, 'float', '"+ ( saldoCuentaAnt (7951 ) + saldoCuentaAnt (7952 ) + saldoCuentaAnt (7955 ) + saldoCuentaAnt (7956)).toQString ( '.' ) + "')\n\
doc.set_cell_property('bold', False)\n\
\
doc.set_cell_property('bold', True)\n\
doc.set_cell_value(2,33, 'string', ' " + trUtf8("11. Deterioro y resultado por enajenaciones del inmovilizado.") + "')\n\
doc.set_cell_value(4,33, 'formula', '=D34+D35')\n\
doc.set_cell_value(5,33, 'formula', '=E34+E35')\n\
doc.set_cell_property('bold', False)\n\
\
doc.set_cell_value(2,34, 'string', ' " + trUtf8("a) Deterioros y pérdidas.") + "')\n\
doc.set_cell_value(4,34, 'float', '" + ( saldoCuenta (690 ) + saldoCuenta (691 ) + saldoCuenta (692 ) + saldoCuenta (790 ) + saldoCuenta (791 ) + saldoCuenta (792)).toQString ( '.' ) + "')\n\
doc.set_cell_value(5,34, 'float', '"+ ( saldoCuentaAnt (690 ) + saldoCuentaAnt (691 ) + saldoCuentaAnt (692 ) + saldoCuentaAnt (790 ) + saldoCuentaAnt (791 ) + saldoCuentaAnt (792)).toQString ( '.' ) + "')\n\
\
doc.set_cell_value(2,35, 'string', ' " + trUtf8("b) Resultados por enajenaciones y otras.") + "')\n\
doc.set_cell_value(4,35, 'float', '" + ( saldoCuenta (670 ) + saldoCuenta (671 ) + saldoCuenta (672 ) + saldoCuenta (770 ) + saldoCuenta (771 ) + saldoCuenta (772)).toQString ( '.' ) + "')\n\
doc.set_cell_value(5,35, 'float', '"+ ( saldoCuentaAnt (670 ) + saldoCuentaAnt (671 ) + saldoCuentaAnt (672 ) + saldoCuentaAnt (770 ) + saldoCuentaAnt (771 ) + saldoCuentaAnt (772)).toQString ( '.' ) + "')\n\
\
\
\
doc.set_cell_property('bold', True)\n\
doc.set_cell_value(2,37, 'string', ' " + trUtf8("A.1) RESULTADO DE EXPLOTACIÓN") + "')\n\
doc.set_cell_value(4,37, 'formula', '=D8+D11+D12+D13+D18+D21+D25+D30+D31+D32+D33')\n\
doc.set_cell_value(5,37, 'formula', '=E8+E11+E12+E13+E18+E21+E25+E30+E31+E32+E33')\n\
doc.set_cell_value(2,38, 'string', ' " + trUtf8("(1+2+3+4+5+6+7+8+9+10+11)") + "')\n\
doc.set_cell_property('bold', False)\n\
\
\
\
doc.set_cell_property('bold', True)\n\
doc.set_cell_value(2,40, 'string', ' " + trUtf8("12. Ingresos financieros.") + "')\n\
doc.set_cell_value(4,40, 'formula', '=D41+D44')\n\
doc.set_cell_value(5,40, 'formula', '=E41+E44')\n\
doc.set_cell_property('bold', False)\n\
\
doc.set_cell_value(2,41, 'string', ' " + trUtf8("a) De participaciones en instrumentos de patrimonio.") + "')\n\
doc.set_cell_value(4,41, 'formula', '=D42+D43')\n\
doc.set_cell_value(5,41, 'formula', '=E42+E43')\n\
\
doc.set_cell_value(2,42, 'string', ' " + trUtf8("a1) En empresas del grupo y asociadas.") + "')\n\
doc.set_cell_value(4,42, 'float', '" + ( saldoCuenta (7600 ) + saldoCuenta (7601)).toQString ( '.' ) + "')\n\
doc.set_cell_value(5,42, 'float', '"+ ( saldoCuentaAnt (7600 ) + saldoCuentaAnt (7601)).toQString ( '.' ) + "')\n\
\
doc.set_cell_value(2,43, 'string', ' " + trUtf8("a2) En terceros.") + "')\n\
doc.set_cell_value(4,43, 'float', '" + ( saldoCuenta (7602 ) + saldoCuenta (7603)).toQString ( '.' ) + "')\n\
doc.set_cell_value(5,43, 'float', '"+ ( saldoCuentaAnt (7602 ) + saldoCuentaAnt (7603)).toQString ( '.' ) + "')\n\
\
doc.set_cell_value(2,44, 'string', ' " + trUtf8("b) De valores negociables y otros instrumentos financieros.") + "')\n\
doc.set_cell_value(4,44, 'formula', '=D45+D46')\n\
doc.set_cell_value(5,44, 'formula', '=E45+E46')\n\
\
doc.set_cell_value(2,45, 'string', ' " + trUtf8("b1) De empresas del grupo y asociadas.") + "')\n\
doc.set_cell_value(4,45, 'float', '" + ( saldoCuenta (7610 ) + saldoCuenta (7611 ) + saldoCuenta (76200 ) + saldoCuenta (76201 ) + saldoCuenta (76210 ) + saldoCuenta (76211)).toQString ( '.' ) + "')\n\
doc.set_cell_value(5,45, 'float', '"+ ( saldoCuentaAnt (7610 ) + saldoCuentaAnt (7611 ) + saldoCuentaAnt (76200 ) + saldoCuentaAnt (76201 ) + saldoCuentaAnt (76210 ) + saldoCuentaAnt (76211)).toQString ( '.' ) + "')\n\
\
doc.set_cell_value(2,46, 'string', ' " + trUtf8("b2) De terceros.") + "')\n\
doc.set_cell_value(4,46, 'float', '" + ( saldoCuenta (7612 ) + saldoCuenta (7613 ) + saldoCuenta (76202 ) + saldoCuenta (76203 ) + saldoCuenta (76212 ) + saldoCuenta (76213 ) + saldoCuenta (767 ) + saldoCuenta (769)).toQString ( '.' ) + "')\n\
doc.set_cell_value(5,46, 'float', '"+ ( saldoCuentaAnt (7612 ) + saldoCuentaAnt (7613 ) + saldoCuentaAnt (76202 ) + saldoCuentaAnt (76203 ) + saldoCuentaAnt (76212 ) + saldoCuentaAnt (76213 ) + saldoCuentaAnt (767 ) + saldoCuentaAnt (769)).toQString ( '.' ) + "')\n\
\
doc.set_cell_property('bold', True)\n\
doc.set_cell_value(2,47, 'string', ' " + trUtf8("13. Gastos financieros.") + "')\n\
doc.set_cell_value(4,47, 'formula', '=D48+D49+D50')\n\
doc.set_cell_value(5,47, 'formula', '=E48+E49+E50')\n\
doc.set_cell_property('bold', False)\n\
\
doc.set_cell_value(2,48, 'string', ' " + trUtf8("a) Por deudas con empresas del grupo y asociadas.") + "')\n\
doc.set_cell_value(4,48, 'float', '" + ( saldoCuenta (6610 ) + saldoCuenta (6611 ) + saldoCuenta (6615 ) + saldoCuenta (6616 ) + saldoCuenta (6620 ) + saldoCuenta (6621 ) + saldoCuenta (6640 ) + saldoCuenta (6641 ) + saldoCuenta (6650 ) + saldoCuenta (6651 ) + saldoCuenta (6654 ) + saldoCuenta (6655)).toQString ( '.' ) + "')\n\
doc.set_cell_value(5,48, 'float', '"+ ( saldoCuentaAnt (6610 ) + saldoCuentaAnt (6611 ) + saldoCuentaAnt (6615 ) + saldoCuentaAnt (6616 ) + saldoCuentaAnt (6620 ) + saldoCuentaAnt (6621 ) + saldoCuentaAnt (6640 ) + saldoCuentaAnt (6641 ) + saldoCuentaAnt (6650 ) + saldoCuentaAnt (6651 ) + saldoCuentaAnt (6654 ) + saldoCuentaAnt (6655)).toQString ( '.' ) + "')\n\
\
doc.set_cell_value(2,49, 'string', ' " + trUtf8("b) Por deudas con terceros.") + "')\n\
doc.set_cell_value(4,49, 'float', '" + ( saldoCuenta (6612 ) + saldoCuenta (6613 ) + saldoCuenta (6617 ) + saldoCuenta (6618 ) + saldoCuenta (6622 ) + saldoCuenta (6623 ) + saldoCuenta (6624 ) + saldoCuenta (6642 ) + saldoCuenta (6643 ) + saldoCuenta (6652 ) + saldoCuenta (6653 ) + saldoCuenta (6656 ) + saldoCuenta (6657 ) + saldoCuenta (669)).toQString ( '.' ) + "')\n\
doc.set_cell_value(5,49, 'float', '"+ ( saldoCuentaAnt (6612 ) + saldoCuentaAnt (6613 ) + saldoCuentaAnt (6617 ) + saldoCuentaAnt (6618 ) + saldoCuentaAnt (6622 ) + saldoCuentaAnt (6623 ) + saldoCuentaAnt (6624 ) + saldoCuentaAnt (6642 ) + saldoCuentaAnt (6643 ) + saldoCuentaAnt (6652 ) + saldoCuentaAnt (6653 ) + saldoCuentaAnt (6656 ) + saldoCuentaAnt (6657 ) + saldoCuentaAnt (669)).toQString ( '.' ) + "')\n\
\
doc.set_cell_value(2,50, 'string', ' " + trUtf8("c) Por actualización de provisiones") + "')\n\
doc.set_cell_value(4,50, 'float', '" + ( saldoCuenta (660)).toQString ( '.' ) + "')\n\
doc.set_cell_value(5,50, 'float', '"+ ( saldoCuentaAnt (660)).toQString ( '.' ) + "')\n\
\
doc.set_cell_property('bold', True)\n\
doc.set_cell_value(2,51, 'string', ' " + trUtf8("14. Variación de valor razonable en instrumentos financieros.") + "')\n\
doc.set_cell_value(4,51, 'formula', '=D52+D53')\n\
doc.set_cell_value(5,51, 'formula', '=E52+E53')\n\
doc.set_cell_property('bold', False)\n\
\
doc.set_cell_value(2,52, 'string', ' " + trUtf8("a) Cartera de negociación y otros.") + "')\n\
doc.set_cell_value(4,52, 'float', '" + ( saldoCuenta (6630 ) + saldoCuenta (6631 ) + saldoCuenta (6633 ) + saldoCuenta (7630 ) + saldoCuenta (7631 ) + saldoCuenta (7633)).toQString ( '.' ) + "')\n\
doc.set_cell_value(5,52, 'float', '"+ ( saldoCuentaAnt (6630 ) + saldoCuentaAnt (6631 ) + saldoCuentaAnt (6633 ) + saldoCuentaAnt (7630 ) + saldoCuentaAnt (7631 ) + saldoCuentaAnt (7633)).toQString ( '.' ) + "')\n\
\
doc.set_cell_value(2,53, 'string', ' " + trUtf8("b) Imputación al resultado del ejercicio por activos financieros disponibles para la venta.") + "')\n\
doc.set_cell_value(4,53, 'float', '" + ( saldoCuenta (6632 ) + saldoCuenta (7632)).toQString ( '.' ) + "')\n\
doc.set_cell_value(5,53, 'float', '"+ ( saldoCuentaAnt (6632 ) + saldoCuentaAnt (7632)).toQString ( '.' ) + "')\n\
\
doc.set_cell_property('bold', True)\n\
doc.set_cell_value(2,54, 'string', ' " + trUtf8("15. Diferencias de cambio.") + "')\n\
doc.set_cell_value(4,54, 'float', '" + ( saldoCuenta (668 ) + saldoCuenta (768)).toQString ( '.' ) + "')\n\
doc.set_cell_value(5,54, 'float', '"+ ( saldoCuentaAnt (668 ) + saldoCuentaAnt (768)).toQString ( '.' ) + "')\n\
doc.set_cell_property('bold', False)\n\
\
doc.set_cell_property('bold', True)\n\
doc.set_cell_value(2,55, 'string', ' " + trUtf8("16. Deterioro y resultado por enajenaciones de instrumentos financieros.") + "')\n\
doc.set_cell_value(4,55, 'formula', '=D56+D57')\n\
doc.set_cell_value(5,55, 'formula', '=E56+E57')\n\
doc.set_cell_property('bold', False)\n\
\
doc.set_cell_value(2,56, 'string', ' " + trUtf8("a) Deterioros y pérdidas.") + "')\n\
doc.set_cell_value(4,56, 'float', '" + ( saldoCuenta (696 ) + saldoCuenta (697 ) + saldoCuenta (698 ) + saldoCuenta (699 ) + saldoCuenta (796 ) + saldoCuenta (797 ) + saldoCuenta (798 ) + saldoCuenta (799)).toQString ( '.' ) + "')\n\
doc.set_cell_value(5,56, 'float', '"+ ( saldoCuentaAnt (696 ) + saldoCuentaAnt (697 ) + saldoCuentaAnt (698 ) + saldoCuentaAnt (699 ) + saldoCuentaAnt (796 ) + saldoCuentaAnt (797 ) + saldoCuentaAnt (798 ) + saldoCuentaAnt (799)).toQString ( '.' ) + "')\n\
\
doc.set_cell_value(2,57, 'string', ' " + trUtf8("b) Resultados por enajenaciones y otras.") + "')\n\
doc.set_cell_value(4,57, 'float', '" + ( saldoCuenta (666 ) + saldoCuenta (667 ) + saldoCuenta (673 ) + saldoCuenta (675 ) + saldoCuenta (766 ) + saldoCuenta (773 ) + saldoCuenta (775)).toQString ( '.' ) + "')\n\
doc.set_cell_value(5,57, 'float', '"+ ( saldoCuentaAnt (666 ) + saldoCuentaAnt (667 ) + saldoCuentaAnt (673 ) + saldoCuentaAnt (675 ) + saldoCuentaAnt (766 ) + saldoCuentaAnt (773 ) + saldoCuentaAnt (7750)).toQString ( '.' ) + "')\n\
\
\
doc.set_cell_property('bold', True)\n\
doc.set_cell_value(2,59, 'string', ' " + trUtf8("A.2) RESULTADO FINANCIERO (12+13+14+15+16 )") + "')\n\
doc.set_cell_value(4,59, 'formula', '=D40+D47+D51+D54+D55')\n\
doc.set_cell_value(5,59, 'formula', '=E40+E47+E51+E54+E55')\n\
doc.set_cell_property('bold', False)\n\
\
\
doc.set_cell_property('bold', True)\n\
doc.set_cell_value(2,61, 'string', ' " + trUtf8("A.3) RESULTADO ANTES DE IMPUESTOS (A.1+A.2)") + "')\n\
doc.set_cell_value(4,61, 'formula', '=D37+D59')\n\
doc.set_cell_value(5,61, 'formula', '=E37+E59')\n\
doc.set_cell_property('bold', False)\n\
\
\
doc.set_cell_property('bold', True)\n\
doc.set_cell_value(2,63, 'string', ' " + trUtf8("17. Impuestos sobre beneficios.") + "')\n\
doc.set_cell_value(4,63, 'float', '" + ( saldoCuenta (6300 ) + saldoCuenta (6301 ) + saldoCuenta (633 ) + saldoCuenta (638)).toQString ( '.' ) + "')\n\
doc.set_cell_value(5,63, 'float', '"+ ( saldoCuentaAnt (6300 ) + saldoCuentaAnt (6301 ) + saldoCuentaAnt (633 ) + saldoCuentaAnt (638)).toQString ( '.' ) + "')\n\
doc.set_cell_property('bold', False)\n\
\
\
doc.set_cell_property('bold', True)\n\
doc.set_cell_value(2,65, 'string', ' " + trUtf8("A.4) RESULTADO DEL EJERCICIO PROCEDENTE DE OPERACIONES CONTINUADAS (A.3+17)") + "')\n\
doc.set_cell_value(4,65, 'formula', '=D61+D63')\n\
doc.set_cell_value(5,65, 'formula', '=E61+E63')\n\
doc.set_cell_property('bold', False)\n\
\
\
\
doc.set_cell_property('bold', True)\n\
doc.set_cell_value(2,67, 'string', ' " + trUtf8("B) OPERACIONES INTERRUMPIDAS") + "')\n\
doc.set_cell_property('bold', False)\n\
\
\
doc.set_cell_property('bold', True)\n\
doc.set_cell_value(2,69, 'string', ' " + trUtf8("18. Resultado del ejercicio procedente de operaciones interrumpidas neto de impuestos.") + "')\n\
doc.set_cell_property('bold', False)\n\
\
\
\
doc.set_cell_property('bold', True)\n\
doc.set_cell_value(2,71, 'string', ' " + trUtf8("A.5) RESULTADO DEL EJERCICIO (A.4+18)") + "')\n\
doc.set_cell_value(4,71, 'formula', '=D65+D69')\n\
doc.set_cell_value(5,71, 'formula', '=E65+E69')\n\
doc.set_cell_property('bold', False)\n\
\
\
doc.new_sheet(\" " + trUtf8("INGRESOS Y GASTOS")+"\")\n\n\
doc.set_column_property(1, 'width', '0.5in')\n\
doc.set_column_property(2, 'width', '5in')\n\
\
doc.set_cell_property('bold', True)\n\
doc.set_cell_value(2,2, 'string', ' " + trUtf8("ESTADO DE INGRESOS Y GASTOS RECONOCIDOS CORRESPONDIENTE AL EJERCICIO TERMINADO") + "')\n\
doc.set_cell_value(2,4, 'string', ' " + trUtf8("INGRESOS Y GASTOS") + "')\n\
\
doc.set_cell_value(3,4, 'string', ' " + trUtf8("NOTAS") + "')\n\
doc.set_cell_value(4,4, 'string', ' " + trUtf8("Ejercicio N") + "')\n\
doc.set_cell_value(5,4, 'string', ' " + trUtf8("Ejercicio N-1") + "')\n\
doc.set_cell_property('bold', False)\n\
\
\
\
doc.set_cell_property('bold', True)\n\
doc.set_cell_value(2,6, 'string', ' " + trUtf8("A) Resultado de la cuenta de perdidas y ganancias") + "')\n\
doc.set_cell_value(4,6, 'formula', '="+trUtf8("PERDIDAS_Y_GANANCIAS")+".D65')\n\
doc.set_cell_value(5,6, 'formula', '=PERDIDAS_Y_GANANCIAS.E65')\n\
\
doc.set_cell_value(2,8, 'string', ' " + trUtf8("Ingresos y gastos imputados directamente al patrimonio neto") + "')\n\
doc.set_cell_property('bold', False)\n\
\
\
\
doc.set_cell_property('bold', True)\n\
doc.set_cell_value(2,9, 'string', ' " + trUtf8("I. Por valoración de instrumentos financieros.") + "')\n\
doc.set_cell_value(4,9, 'formula', '=D10+D11')\n\
doc.set_cell_value(5,9, 'formula', '=E10+E11')\n\
doc.set_cell_property('bold', False)\n\
\
doc.set_cell_value(2,10, 'string', ' " + trUtf8("1. Activos financieros disponibles para la venta.") + "')\n\
doc.set_cell_value(4,10, 'float', '" + ( saldoCuenta (800 ) + saldoCuenta (900)).toQString ( '.' ) + "')\n\
doc.set_cell_value(5,10, 'float', '"+ ( saldoCuentaAnt (800 ) + saldoCuentaAnt (900)).toQString ( '.' ) + "')\n\
\
doc.set_cell_value(2,11, 'string', ' " + trUtf8("2. Otros ingresos/gastos.") + "')\n\
doc.set_cell_value(4,11, 'float', '" + ( saldoCuenta (89 ) + saldoCuenta (991 ) + saldoCuenta (992)).toQString ( '.' ) + "')\n\
doc.set_cell_value(5,11, 'float', '"+ ( saldoCuentaAnt (89 ) + saldoCuentaAnt (991 ) + saldoCuentaAnt (992)).toQString ( '.' ) + "')\n\
\
doc.set_cell_property('bold', True)\n\
doc.set_cell_value(2,12, 'string', ' " + trUtf8("II. Por coberturas de flujos de efectivo.") + "')\n\
doc.set_cell_value(4,12, 'float', '" + ( saldoCuenta (810 ) + saldoCuenta (910)).toQString ( '.' ) + "')\n\
doc.set_cell_value(5,12, 'float', '"+ ( saldoCuentaAnt (810 ) + saldoCuentaAnt (910)).toQString ( '.' ) + "')\n\
doc.set_cell_property('bold', False)\n\
\
doc.set_cell_property('bold', True)\n\
doc.set_cell_value(2,13, 'string', ' " + trUtf8("III. Subvenciones, donaciones y legados recibidos.") + "')\n\
doc.set_cell_value(4,13, 'float', '" + ( saldoCuenta (94)).toQString ( '.' ) + "')\n\
doc.set_cell_value(5,13, 'float', '"+ ( saldoCuentaAnt (94)).toQString ( '.' ) + "')\n\
doc.set_cell_property('bold', False)\n\
\
doc.set_cell_property('bold', True)\n\
doc.set_cell_value(2,14, 'string', ' " + trUtf8("IV. Por ganancias y pérdidas actuariales y otros ajustes") + "')\n\
doc.set_cell_value(4,14, 'float', '" + ( saldoCuenta (85 ) + saldoCuenta (95)).toQString ( '.' ) + "')\n\
doc.set_cell_value(5,14, 'float', '"+ ( saldoCuentaAnt (85 ) + saldoCuentaAnt (95)).toQString ( '.' ) + "')\n\
doc.set_cell_property('bold', False)\n\
\
doc.set_cell_property('bold', True)\n\
doc.set_cell_value(2,15, 'string', ' " + trUtf8("V. Efecto impositivo.") + "')\n\
doc.set_cell_value(4,15, 'float', '" + ( saldoCuenta (8300 ) + saldoCuenta (8301 ) + saldoCuenta (833 ) + saldoCuenta (834 ) + saldoCuenta (835 ) + saldoCuenta (838)).toQString ( '.' ) + "')\n\
doc.set_cell_value(5,15, 'float', '"+ ( saldoCuentaAnt (8300 ) + saldoCuentaAnt (8301 ) + saldoCuentaAnt (833 ) + saldoCuentaAnt (834 ) + saldoCuentaAnt (835 ) + saldoCuentaAnt (838)).toQString ( '.' ) + "')\n\
doc.set_cell_property('bold', False)\n\
\
\
\
doc.set_cell_property('bold', True)\n\
doc.set_cell_value(2,17, 'string', ' " + trUtf8("B) Total Ingresos y gastos imputados directamente al patrimonio neto (I+II+III+IV+V)") + "')\n\
doc.set_cell_value(4,17, 'formula', '=D9+D12+D13+D14+D15')\n\
doc.set_cell_value(5,17, 'formula', '=E9+E12+E13+E14+E15')\n\
\
doc.set_cell_value(2,19, 'string', ' " + trUtf8("Transferencias a la cuenta de pérdidas y ganancias") + "')\n\
doc.set_cell_property('bold', False)\n\
\
\
\
doc.set_cell_property('bold', True)\n\
doc.set_cell_value(2,20, 'string', ' " + trUtf8("VI. Por valoración de instrumentos financieros.") + "')\n\
doc.set_cell_value(4,20, 'formula', '=D21+D22')\n\
doc.set_cell_value(5,20, 'formula', '=E21+E22')\n\
doc.set_cell_property('bold', False)\n\
\
doc.set_cell_value(2,21, 'string', ' " + trUtf8("1.Activos financieros disponibles para la venta.") + "')\n\
doc.set_cell_value(4,21, 'float', '" + ( saldoCuenta (802 ) + saldoCuenta (902)).toQString ( '.' ) + "')\n\
doc.set_cell_value(5,21, 'float', '"+ ( saldoCuentaAnt (802 ) + saldoCuentaAnt (902)).toQString ( '.' ) + "')\n\
\
doc.set_cell_value(2,22, 'string', ' " + trUtf8("2. Otros ingresos/gastos.") + "')\n\
doc.set_cell_value(4,22, 'float', '" + ( saldoCuenta (993 ) + saldoCuenta (994)).toQString ( '.' ) + "')\n\
doc.set_cell_value(5,22, 'float', '"+ ( saldoCuentaAnt (993 ) + saldoCuentaAnt (994)).toQString ( '.' ) + "')\n\
\
doc.set_cell_property('bold', True)\n\
doc.set_cell_value(2,23, 'string', ' " + trUtf8("VII. Por coberturas de flujos de efectivo.") + "')\n\
doc.set_cell_value(4,23, 'float', '" + ( saldoCuenta (812 ) + saldoCuenta (912)).toQString ( '.' ) + "')\n\
doc.set_cell_value(5,23, 'float', '"+ ( saldoCuentaAnt (812 ) + saldoCuentaAnt (912)).toQString ( '.' ) + "')\n\
doc.set_cell_property('bold', False)\n\
\
doc.set_cell_property('bold', True)\n\
doc.set_cell_value(2,24, 'string', ' " + trUtf8("VIII. Subvenciones, donaciones y legados recibidos.") + "')\n\
doc.set_cell_value(4,24, 'float', '" + ( saldoCuenta (84)).toQString ( '.' ) + "')\n\
doc.set_cell_value(5,24, 'float', '"+ ( saldoCuentaAnt (84)).toQString ( '.' ) + "')\n\
doc.set_cell_property('bold', False)\n\
\
doc.set_cell_property('bold', True)\n\
doc.set_cell_value(2,25, 'string', ' " + trUtf8("IX. Efecto impositivo.") + "')\n\
doc.set_cell_value(4,25, 'float', '" + ( saldoCuenta (8301 ) + saldoCuenta (836 ) + saldoCuenta (837)).toQString ( '.' ) + "')\n\
doc.set_cell_value(5,25, 'float', '"+ ( saldoCuentaAnt (8301 ) + saldoCuentaAnt (836 ) + saldoCuentaAnt (837)).toQString ( '.' ) + "')\n\
doc.set_cell_property('bold', False)\n\
\
\
\
doc.set_cell_property('bold', True)\n\
doc.set_cell_value(2,27, 'string', ' " + trUtf8("C) Total transferencias a la cuenta de pérdidas y ganancias (VI+VII+VIII+IX)") + "')\n\
doc.set_cell_value(4,27, 'formula', '=D20+D23+D24+D25')\n\
doc.set_cell_value(5,27, 'formula', '=E20+E23+E24+E25')\n\
doc.set_cell_property('bold', False)\n\
\
\
\
doc.set_cell_property('bold', True)\n\
doc.set_cell_value(2,29, 'string', ' " + trUtf8("TOTAL DE INGRESOS Y GASTOS RECONOCIDOS (A+ B +C)") + "')\n\
doc.set_cell_value(4,29, 'formula', '=D6+D17+D27')\n\
doc.set_cell_value(5,29, 'formula', '=E6+E17+E27')\n\
doc.set_cell_property('bold', False)\n\
\
\
# Guardamos el documento\n\
doc.save(\"canualesods.ods\")\n");
}


