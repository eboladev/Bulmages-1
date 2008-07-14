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

#include <QMenu>
#include <QAction>
#include <QObject>
#include <QMessageBox>

#include <cstdio>

#include "plugincanualesods.h"
#include "empresa.h"
#include "funcaux.h"
#include "datosview.h"
#include "blprogressbar.h"


///
/**
**/
pluginCAnualesODS::pluginCAnualesODS()
{
    _depura ( "pluginCAnualesODS::pluginCAnualesODS", 0 );
    _depura ( "END pluginCAnualesODS::pluginCAnualesODS", 0 );
}


///
/**
**/
pluginCAnualesODS::~pluginCAnualesODS()
{
    _depura ( "pluginCAnualesODS::~pluginCAnualesODS", 0 );
    _depura ( "END pluginCAnualesODS::~pluginCAnualesODS", 0 );
}


bool pluginCAnualesODS::Arboles()
{
    _depura ( "pluginCAnualesODS::Arboles", 0 );

    /// Para no acceder constantemete a la BD cada vez que se quiere averiguar el saldo
    /// de una cuenta, vamos a crear una estructura tipo árbol (usando la clase Arbol) donde,
    /// con un par consultas a BD, se averigüen todos los saldos y puedan estar disponibles en la RAM.

    /// Primero, averiguaremos la cantidad de ramas iniciales (tantos como
    /// numero de cuentas de nivel 2) y las vamos creando.
    conexionbase->begin();
    QString query = "SELECT *, nivel(codigo) AS nivel FROM cuenta ORDER BY codigo";
    cursor2 *ramas;
    ramas = conexionbase->cargacursor ( query, "Ramas" );
    conexionbase->commit();
    if ( ramas == NULL ) {
        mensajeInfo ( trUtf8 ( "Error con la base de datos" ) );
        return 0;
    }

    arbolEjercicioActual = new Arbol;
    arbolEjercicioAnterior = new Arbol;
    while ( !ramas->eof() ) {
        if ( ramas->valor ( "nivel" ).toInt() == 2 ) { /// Cuenta raíz.
            arbolEjercicioActual->nuevaRama ( ramas );
            arbolEjercicioAnterior->nuevaRama ( ramas );
        } // end if
        ramas->siguienteregistro();
    } // end while

    /// Inicializamos el árbol desde sus raices (desde sus cuentas de nivel 2)
    /// con el resto de cuentas (las hojas del árbol)
    arbolEjercicioActual->inicializa ( ramas );
    arbolEjercicioAnterior->inicializa ( ramas );

    /// Seguidamente, recopilamos todos los apuntes agrupados por cuenta para poder
    /// establecer asi los valores de cada cuenta para el Ejercicio N.
    conexionbase->begin();
    query = "SELECT cuenta.idcuenta, numapuntes, cuenta.codigo, saldoant, debe, haber, saldo, debeej, haberej, saldoej FROM (SELECT idcuenta, codigo FROM cuenta) AS cuenta NATURAL JOIN (SELECT idcuenta, count(idcuenta) AS numapuntes, sum(debe) AS debeej, sum(haber) AS haberej, (sum(debe)-sum(haber)) AS saldoej FROM apunte WHERE EXTRACT(year FROM fecha) = '" + ejercicioActual_fechaBalance.right ( 4 ) + "' GROUP BY idcuenta) AS ejercicio LEFT OUTER JOIN (SELECT idcuenta,sum(debe) AS debe, sum(haber) AS haber, (sum(debe)-sum(haber)) AS saldo FROM apunte WHERE fecha >= '01/01/" + ejercicioActual_fechaBalance.right ( 4 ) + "' AND fecha <= '" + ejercicioActual_fechaBalance + "' AND conceptocontable !~* '.*asiento.*(cierre|regularizaci).*' GROUP BY idcuenta) AS periodo ON periodo.idcuenta=ejercicio.idcuenta LEFT OUTER JOIN (SELECT idcuenta, (sum(debe)-sum(haber)) AS saldoant FROM apunte WHERE fecha < '01/01/" + ejercicioActual_fechaBalance.right ( 4 ) + "' GROUP BY idcuenta) AS anterior ON cuenta.idcuenta=anterior.idcuenta ORDER BY codigo";


    cursor2 *hojas;
    hojas = conexionbase->cargacursor ( query, "Ejercicio N" );
    conexionbase->commit();
    if ( hojas == NULL ) {
        mensajeInfo ( trUtf8 ( "Error con la base de datos" ) );
        return 0;
    }

    /// Para cada cuenta con sus saldos ya calculados hay que actualizar las hojas del �rbol.
    while ( !hojas->eof() ) {
        arbolEjercicioActual->actualizaHojas ( hojas );
        hojas->siguienteregistro();
    } // end while

    /// Finalmente, recopilamos todos los apuntes agrupados por cuenta para poder
    /// establecer así los valores de cada cuenta para el Ejercicio N-1.
    conexionbase->begin();
    query = "SELECT cuenta.idcuenta, numapuntes, cuenta.codigo, saldoant, debe, haber, saldo, debeej, haberej, saldoej FROM (SELECT idcuenta, codigo FROM cuenta) AS cuenta NATURAL JOIN (SELECT idcuenta, count(idcuenta) AS numapuntes,sum(debe) AS debeej, sum(haber) AS haberej, (sum(debe)-sum(haber)) AS saldoej FROM apunte WHERE EXTRACT(year FROM fecha) = '" + ejercicioAnterior_fechaBalance.right ( 4 ) + "' GROUP BY idcuenta) AS ejercicio LEFT OUTER JOIN (SELECT idcuenta,sum(debe) AS debe, sum(haber) AS haber, (sum(debe)-sum(haber)) AS saldo FROM apunte WHERE fecha >= '01/01/" + ejercicioAnterior_fechaBalance.right ( 4 ) + "' AND fecha <= '" + ejercicioAnterior_fechaBalance + "' AND conceptocontable !~* '.*asiento.*(cierre|regularizaci).*' GROUP BY idcuenta) AS periodo ON periodo.idcuenta=ejercicio.idcuenta LEFT OUTER JOIN (SELECT idcuenta, (sum(debe)-sum(haber)) AS saldoant FROM apunte WHERE fecha < '01/01/" + ejercicioAnterior_fechaBalance.right ( 4 ) + "' GROUP BY idcuenta) AS anterior ON cuenta.idcuenta=anterior.idcuenta ORDER BY codigo";
    hojas = conexionbase->cargacursor ( query, "Ejercicio N-1" );
    conexionbase->commit();
    if ( hojas == NULL ) {
        mensajeInfo ( trUtf8 ( "Error con la base de datos" ) );
        return 0;
    }

    /// De nuevo, para cada cuenta con sus saldos ya calculados hay que actualizar las hojas del árbol.
    while ( !hojas->eof() ) {
        arbolEjercicioAnterior->actualizaHojas ( hojas );
        hojas->siguienteregistro();
    } // end while

    _depura ( "END pluginCAnualesODS::Arboles", 0 );
    return 1;
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
// Fixed pluginCAnualesODS::saldoCuenta ( int cuenta )
// {
//     _depura ( "pluginCAnualesODS::saldoCuenta", 0 );
//
//     /// Ejercicio actual.
//     cursor2 *cur;
//     Fixed resultado;
//
// //     QString query = QString( "SELECT (SUM(apunte.debe) - SUM(apunte.haber)) AS saldo FROM cuenta LEFT JOIN apunte ON apunte.idcuenta = cuenta.idcuenta WHERE cuenta.codigo like '" + QString::number ( cuenta ) + "%' AND fecha >= '01/01/" + ejercicioActual_fechaBalance.right(4) + "' AND fecha <= '" + ejercicioActual_fechaBalance +"' and apunte.conceptocontable not ilike '%%asiento%%cierre%%' and apunte.conceptocontable not ilike '%%asiento%%regularizaci%%'" );
//
//     QString query = QString( "SELECT saldototal FROM (SELECT SUBSTR(codigo, 1, 3) AS cod, SUM(saldo) as saldototal FROM cuenta LEFT JOIN (SELECT idcuenta, (SUM(debe) - SUM(haber) ) AS saldo FROM apunte WHERE fecha >= '01/01/" + ejercicioActual_fechaBalance.right(4) + "' AND fecha <= '" + ejercicioActual_fechaBalance + "' AND conceptocontable ~* '.*asiento.*(cierre|regularizaci).*' GROUP BY idcuenta) AS saldo ON saldo.idcuenta=cuenta.idcuenta GROUP BY cod ORDER BY cod) AS saldo_ctas WHERE cod=" + QString::number ( cuenta ) );
//
//     conexionbase->begin();
//     cur = conexionbase->cargacursor ( query );
//     conexionbase->commit();
//     if (cur != NULL)
//  resultado = cur->valor ( "saldototal" );
//     else
//  mensajeInfo(trUtf8("Error con la base de datos"));
//
//     _depura ( "END pluginCAnualesODS::saldoCuenta", 0 );
//     return resultado;
// }


///
/**
\param cuenta
\return
**/
Fixed pluginCAnualesODS::saldoCuentaAnt ( int cuenta )
{
    _depura ( "pluginCAnualesODS::saldoCuentaAnt", 0 );

    Fixed resultado;
    QString cta = QString::number ( cuenta );

    if ( arbolEjercicioAnterior->irHoja ( cta, cta.length() ) )
        resultado = arbolEjercicioAnterior->hojaActual ( "saldo" );
    else
        resultado = Fixed ( "0.00" );

    _depura ( "pluginCAnualesODS::saldoCuentaAnt", 0 );
    return resultado;
}

///
/**
\param cuenta
\return
**/
// Fixed pluginCAnualesODS::saldoCuentaAnt ( int cuenta )
// {
//     _depura ( "pluginCAnualesODS::saldoCuentaAnt", 0 );
//
//     /// Ejercicio anterior.
//     cursor2 *cur;
//     Fixed resultado;
//
// //     QString query = QString( "SELECT (SUM(apunte.debe) - SUM(apunte.haber)) AS saldo FROM cuenta LEFT JOIN apunte ON apunte.idcuenta = cuenta.idcuenta WHERE cuenta.codigo like '" + QString::number ( cuenta ) + "%' AND fecha >= '01/01/" + ejercicioAnterior_fechaBalance.right(4) + "' AND fecha <= '" + ejercicioAnterior_fechaBalance +"' and apunte.conceptocontable not ilike '%%asiento%%cierre%%' and apunte.conceptocontable not ilike '%%asiento%%regularizaci%%'" );
//
//     QString query = QString( "SELECT saldototal FROM (SELECT SUBSTR(codigo, 1, 3) AS cod, SUM(saldo) as saldototal FROM cuenta LEFT JOIN (SELECT idcuenta, (SUM(debe) - SUM(haber) ) AS saldo FROM apunte WHERE fecha >= '01/01/" + ejercicioAnterior_fechaBalance.right(4) + "' AND fecha <= '" + ejercicioAnterior_fechaBalance + "' AND conceptocontable ~* '.*asiento.*(cierre|apertura|regularizaci).*' GROUP BY idcuenta) AS saldo ON saldo.idcuenta=cuenta.idcuenta GROUP BY cod ORDER BY cod) AS saldo_ctas WHERE cod=" + QString::number ( cuenta ) );
//
//     conexionbase->begin();
//     cur = conexionbase->cargacursor ( query );
//     conexionbase->commit();
//     if (cur != NULL)
//  resultado = cur->valor ( "saldototal" );
//     else
//  mensajeInfo(trUtf8("Error con la base de datos"));
//
//     _depura ( "END pluginCAnualesODS::saldoCuentaAnt", 0 );
//     return resultado;
// }


///
/**
\param cuenta
\return
**/
Fixed pluginCAnualesODS::saldoCuenta ( int cuenta )
{
    _depura ( "pluginCAnualesODS::saldoCuenta", 0 );

    Fixed resultado;
    QString cta = QString::number ( cuenta );

    if ( arbolEjercicioActual->irHoja ( cta, cta.length() ) )
        resultado = arbolEjercicioActual->hojaActual ( "saldo" );
    else
        resultado = Fixed ( "0.00" );

    _depura ( "pluginCAnualesODS::saldoCuenta", 0 );
    return resultado;
}


///
/**
\return
**/
bool pluginCAnualesODS::formDatosBalance ( CAnuales tipus )
{
    _depura ( "pluginCAnualesODS::formDatosBalance", 0 );
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

    if ( ejercicioActual_fechaBalance.isEmpty() ) {
        mensajeError ( trUtf8 ( "ERROR: Debe introducir una fecha en el balance actual." ) );
        resultado = 0;
    } else {
        if ( ejercicioAnterior_fechaBalance.isEmpty() ) {
            mensajeError ( trUtf8 ( "ERROR: Debe introducir una fecha en el balance anterior." ) );
            resultado = 0;
        }
    } // end if

    /// Se pulso el boton 'cancelar'
//     if ( resultado == 0 ) {
//         return;
//     } // end if

    _depura ( "END pluginCAnualesODS::formDatosBalance", 0 );

    return resultado;
}


//Cuentas Anuales Abreviadas Asociación Sin Lucro CAAASL
//Cuentas Anuales Plan General Contable hasta 2007 CAPGC07
//Cuentas Anuales PYMES 2008 CAPYMES08
//Cuentas Anuales Plan General Contable 2008 CAPGC08
//Cuentas Anuales Abreviadas Plan General Contable 2008 CAAPGC08


///
/**
**/
void pluginCAnualesODS::balanceSituacionODS ( CAnuales tipus )
{
    _depura ( "pluginCAnualesODS::balanceSituacionODS", 0 );
    /// Se genera el Balance de Situacion en formato ODS (Hoja de calculo OpenOffice.org).

    /// BUG: Se necesita usar .toQString('.') porque sino los decimales no
    /// aparecen bien en OpenOffice. Creo que es un bug del script de conversion de .py a .ods
    QString archivosalida;
    bool error = false;
    switch ( tipus ) {
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
        mensajeError ( trUtf8 ( "ERROR: Funcion no implementada todavia." ) );
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
        mensajeError ( trUtf8 ( "ERROR: No se ha podido crear el archivo" ) );

    cadena = " cd " + confpr->valor ( CONF_DIR_USER ) + "; python " + archivod;
    system ( cadena.toAscii() );
    cadena = "oocalc " + confpr->valor ( CONF_DIR_USER ) + "canualesods.ods &";
    system ( cadena.toAscii() );
    _depura ( "END pluginCAnualesODS::balanceSituacionODS", 0 );
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
void pluginCAnualesODS::inicializa ( Bulmacont *bcont )
{
    _depura ( "pluginCAnualesODS::inicializa", 0 );

    /// Creamos el men&uacute;.
    QMenu *pPluginMenu = new QMenu ( trUtf8 ( "&Cuentas Anuales" ) );

    QMenu *pgc08 = new QMenu ( trUtf8 ( "&PGC 2008" ) );
    QMenu *pgc07 = new QMenu ( trUtf8 ( "PGC &Anterior 2008" ) );

    QAction *accion4 = new QAction ( trUtf8 ( "&Cuentas Anuales PGC" ), 0 );
    accion4->setStatusTip ( trUtf8 ( "Cuentas Anuales Plan General Contable 2008" ) );
    accion4->setWhatsThis ( trUtf8 ( "Cuentas Anuales Plan General Contable 2008" ) );
    connect ( accion4, SIGNAL ( activated() ), this, SLOT ( balsitCAPGC08() ) );

    QAction *accion5 = new QAction ( trUtf8 ( "Cuentas Anuales &Abreviadas PGC" ), 0 );
    accion4->setStatusTip ( trUtf8 ( "Cuentas Anuales Abreviadas Plan General Contable 2008" ) );
    accion4->setWhatsThis ( trUtf8 ( "Cuentas Anuales Abreviadas Plan General Contable 2008" ) );
    connect ( accion5, SIGNAL ( activated() ), this, SLOT ( balsitCAAPGC08() ) );

    QAction *accion3 = new QAction ( trUtf8 ( "Cuentas Anuales &PYMES" ), 0 );
    accion3->setStatusTip ( trUtf8 ( "Cuentas Anuales PYMES 2008" ) );
    accion3->setWhatsThis ( trUtf8 ( "Cuentas Anuales PYMES 2008" ) );
    connect ( accion3, SIGNAL ( activated() ), this, SLOT ( balsitCAPYMES08() ) );

    /**********************************************/

    QAction *accion2 = new QAction ( trUtf8 ( "&Cuentas Anuales PGC" ), 0 );
    accion2->setStatusTip ( trUtf8 ( "Cuentas Anuales Plan General Contable hasta 2007" ) );
    accion2->setWhatsThis ( trUtf8 ( "Cuentas Anuales Plan General Contable hasta 2007" ) );
    connect ( accion2, SIGNAL ( activated() ), this, SLOT ( balsitCAPGC07() ) );

    QAction *accion1 = new QAction ( trUtf8 ( "Cuentas A. Abreviadas &Sin lucro" ), 0 );
    accion1->setStatusTip ( trUtf8 ( "Cuentas Anuales Abreviadas Asociación Sin ánimo de lucro" ) );
    accion1->setWhatsThis ( trUtf8 ( "Cuentas Anuales Abreviadas Asociación Sin ánimo de lucro" ) );
    connect ( accion1, SIGNAL ( activated() ), this, SLOT ( balsitCAAASL() ) );


    pPluginMenu->addMenu ( pgc08 );
    pPluginMenu->addSeparator();
    pPluginMenu->addMenu ( pgc07 );

    pgc08->addAction ( accion4 ); //PGC
    pgc08->addAction ( accion5 );
    pgc08->addAction ( accion3 );

    pgc07->addAction ( accion2 );
    pgc07->addAction ( accion1 );


    /// A&ntilde;adimos la nueva opci&oacute;n al men&uacute; principal del programa.
    bcont->menuBar() ->addMenu ( pPluginMenu );

    empresaact = bcont->empresaactual();
    conexionbase = empresaact->bdempresa();

    _depura ( "END pluginCAnualesODS::inicializa", 0 );
}


///
/**
\param bcont
**/
void entryPoint ( Bulmacont *bcont )
{
    _depura ( "Estoy dentro del plugin\n", 0 );
    pluginCAnualesODS *plug = new pluginCAnualesODS();
    plug->inicializa ( bcont );
    /// S&Oacute;LO A MODO DE EJEMPLO: se modifica el t&iacute;tulo de la ventana principal
    /// del programa para indicar que el plugin se ha cargado.
}


///
/**
\param cuentaPositiva
\return
**/
Fixed pluginCAnualesODS::cuentaPositiva ( Fixed valor )
{
    _depura ( "pluginCAnualesODS::cuentaPositiva", 0 );
    /// Comprueba que sea un número positivo
    Fixed resultado;
    if ( valor > Fixed ( "0.00" ) )
        resultado = valor;
    else
        resultado = Fixed ( "0.00" );
    return resultado;
}

///
/**
\param cuentaNegativa
\return
**/
Fixed pluginCAnualesODS::cuentaNegativa ( Fixed valor )
{
    _depura ( "pluginCAnualesODS::cuentaNegativa", 0 );
    /// Comprueba que sea un número negativo
    Fixed resultado;
    if ( valor < Fixed ( "0.00" ) )
        resultado = valor;
    else
        resultado = Fixed ( "0.00" );
    return resultado;
}

// CAnuales CAAASL, CAPGC07, CAPYMES08, CAPGC08, CAAPGC08}

void pluginCAnualesODS::balsitCAAASL ()
{
    CAnuales CA = CAAASL;
    mensajeAdvertenciaPGC ( CA );
    if ( formDatosBalance ( CA ) && Arboles() )
        balanceSituacionODS ( CA );
}

void pluginCAnualesODS::balsitCAPGC07 ()
{
    CAnuales CA = CAPGC07;
    mensajeAdvertenciaPGC ( CA );
    if ( formDatosBalance ( CA ) && Arboles() )
        balanceSituacionODS ( CA );
}

void pluginCAnualesODS::balsitCAPYMES08 ()
{
    CAnuales CA = CAPYMES08;
    mensajeAdvertenciaPGC ( CA );
    if ( formDatosBalance ( CA ) && Arboles() )
        balanceSituacionODS ( CA );
}

void pluginCAnualesODS::balsitCAPGC08()
{
    CAnuales CA = CAPGC08;
    mensajeAdvertenciaPGC ( CA );
    if ( formDatosBalance ( CA ) && Arboles() )
        balanceSituacionODS ( CA );
}

void pluginCAnualesODS::balsitCAAPGC08()
{
    CAnuales CA = CAAPGC08;
    mensajeAdvertenciaPGC ( CA );
    if ( formDatosBalance ( CA ) && Arboles() )
        balanceSituacionODS ( CA );
}

void pluginCAnualesODS::mensajeAdvertenciaPGC ( CAnuales tipus )
{
//Cuentas Anuales Abreviadas Asociación Sin Lucro CAAASL
//Cuentas Anuales Plan General Contable hasta 2007 CAPGC07
//Cuentas Anuales PYMES 2008 CAPYMES08
//Cuentas Anuales Plan General Contable 2008 CAPGC08
//Cuentas Anuales Abreviadas Plan General Contable 2008 CAAPGC08

    QString pgc;

    switch ( tipus ) {
    case CAAASL:
        pgc = trUtf8 ( "Cuentas Anuales Abreviadas para Asociacion Sin Lucro. " );
        break;

    case CAPGC07:
        pgc = trUtf8 ( "Cuentas Anuales del Plan General Contable hasta 2007. " );
        break;

    case CAPYMES08:
        pgc = trUtf8 ( "Cuentas Anuales para PYMES 2008. " ) ;
        break;

    case CAAPGC08:
        pgc = trUtf8 ( "Cuentas Anuales Abreviadas del Plan General Contable 2008. " );
        break;

    case CAPGC08:
        pgc = trUtf8 ( "Cuentas Anuales del Plan General Contable 2008. " );
        break;

    default:
        mensajeError ( trUtf8 ( "ERROR: Funcion no implementada todavia." ) );
        break;
    }

    mensajeAviso ( trUtf8 ( "Advertencia: ha escojido las " ) + pgc + trUtf8 ( "El resultado solo sera correcto si coincide con el plan contable de su empresa." ) );
}

