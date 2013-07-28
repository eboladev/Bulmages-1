/***************************************************************************
 *   Copyright (C) 2008 by Ricardo Diaz de la Calle <richard@galdi.es>     *
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

#include <QtWidgets/QMenu>
#include <QtWidgets/QAction>
#include <QtCore/QObject>
#include <QtWidgets/QMessageBox>

#include <cstdio>

#include "pluginbc_cuentasanuales2ods.h"
#include "bccompany.h"
#include "blfunctions.h"
#include "datosview.h"
#include "blprogressbar.h"


///
/**
**/
PluginBc_CuentasAnuales2ODS::PluginBc_CuentasAnuales2ODS()
{
    BL_FUNC_DEBUG
    
}


///
/**
**/
PluginBc_CuentasAnuales2ODS::~PluginBc_CuentasAnuales2ODS()
{
    BL_FUNC_DEBUG
    
}


bool PluginBc_CuentasAnuales2ODS::Arboles()
{
    BL_FUNC_DEBUG

    /// Para no acceder constantemete a la BD cada vez que se quiere averiguar el saldo
    /// de una cuenta, vamos a crear una estructura tipo arbol (usando la clase Arbol) donde,
    /// con un par consultas a BD, se averigüen todos los saldos y puedan estar disponibles en la RAM.

    /// Primero, averiguaremos la cantidad de ramas iniciales (tantos como
    /// numero de cuentas de nivel 2) y las vamos creando.
    dbConnection->begin();
    QString query = "SELECT *, nivel(codigo) AS nivel FROM cuenta ORDER BY codigo";
    BlDbRecordSet *ramas;
    ramas = dbConnection->loadQuery ( query, "Ramas" );
    dbConnection->commit();
    if ( ramas == NULL ) {
        blMsgInfo ( _ ( "Error con la base de datos" ) );
        return 0;
    }

    arbolEjercicioActual = new BcPlanContableArbol;
    arbolEjercicioAnterior = new BcPlanContableArbol;
    while ( !ramas->eof() ) {
        if ( ramas->value( "nivel" ).toInt() == 2 ) { /// Cuenta raiz.
            arbolEjercicioActual->nuevaRama ( ramas );
            arbolEjercicioAnterior->nuevaRama ( ramas );
        } // end if
        ramas->nextRecord();
    } // end while

    /// Inicializamos el arbol desde sus raices (desde sus cuentas de nivel 2)
    /// con el resto de cuentas (las hojas del arbol)
    arbolEjercicioActual->inicializa ( ramas );
    arbolEjercicioAnterior->inicializa ( ramas );

    /// Seguidamente, recopilamos todos los apuntes agrupados por cuenta para poder
    /// establecer asi los valores de cada cuenta para el Ejercicio N.
    dbConnection->begin();
    query = "SELECT cuenta.idcuenta, numapuntes, cuenta.codigo, saldoant, debe, haber,(COALESCE(saldoant,0) + COALESCE(saldop,0)) AS saldo, debeej, haberej, (COALESCE(saldoejp,0) + COALESCE(saldoant,0))AS saldoej  FROM (SELECT idcuenta, codigo FROM cuenta) AS cuenta NATURAL JOIN (SELECT idcuenta, count(idcuenta) AS numapuntes, COALESCE(sum(debe),0) AS debeej, COALESCE(sum(haber),0) AS haberej, (COALESCE(sum(debe),0)-COALESCE(sum(haber),0)) AS saldoejp FROM apunte WHERE EXTRACT(year FROM fecha) = '" + ejercicioActual_fechaBalance.right ( 4 ) + "' GROUP BY idcuenta) AS ejercicio LEFT OUTER JOIN (SELECT idcuenta, COALESCE(sum(debe),0) AS debe, COALESCE(sum(haber),0) AS haber, (COALESCE(sum(debe),0)-COALESCE(sum(haber),0)) AS saldop FROM apunte WHERE fecha >= '01/01/" + ejercicioActual_fechaBalance.right ( 4 ) + "' AND fecha <= '" + ejercicioActual_fechaBalance + "' AND conceptocontable !~* '.*asiento.*(cierre|regularizaci).*' GROUP BY idcuenta) AS periodo ON periodo.idcuenta=ejercicio.idcuenta LEFT OUTER JOIN (SELECT idcuenta, (COALESCE(sum(debe),0)-COALESCE(sum(haber),0)) AS saldoant FROM apunte WHERE fecha < '01/01/" + ejercicioActual_fechaBalance.right ( 4 ) + "' GROUP BY idcuenta) AS anterior ON cuenta.idcuenta=anterior.idcuenta ORDER BY codigo";


    BlDbRecordSet *hojas;
    hojas = dbConnection->loadQuery ( query, "Ejercicio N" );
    dbConnection->commit();
    if ( hojas == NULL ) {
        blMsgInfo ( _ ( "Error con la base de datos" ) );
        return 0;
    }

    /// Para cada cuenta con sus saldos ya calculados hay que actualizar las hojas del �rbol.
    while ( !hojas->eof() ) {
        arbolEjercicioActual->actualizaHojas ( hojas );
        hojas->nextRecord();
    } // end while

    /// Finalmente, recopilamos todos los apuntes agrupados por cuenta para poder
    /// establecer asi los valores de cada cuenta para el Ejercicio N-1.
    dbConnection->begin();
    query = "SELECT cuenta.idcuenta, numapuntes, cuenta.codigo, saldoant, debe, haber, (saldoant + saldop) AS saldo, debeej, haberej, (saldoant + saldoejp) AS saldoej FROM (SELECT idcuenta, codigo FROM cuenta) AS cuenta NATURAL JOIN (SELECT idcuenta, count(idcuenta) AS numapuntes, COALESCE(sum(debe),0) AS debeej, COALESCE (sum(haber),0) AS haberej, (COALESCE(sum(debe),0)-COALESCE(sum(haber),0)) AS saldoejp FROM apunte WHERE EXTRACT(year FROM fecha) = '" + ejercicioAnterior_fechaBalance.right ( 4 ) + "' GROUP BY idcuenta) AS ejercicio LEFT OUTER JOIN (SELECT idcuenta, COALESCE(sum(debe),0) AS debe, COALESCE(sum(haber),0) AS haber, (COALESCE(sum(debe),0)-COALESCE(sum(haber),0)) AS saldop FROM apunte WHERE fecha >= '01/01/" + ejercicioAnterior_fechaBalance.right ( 4 ) + "' AND fecha <= '" + ejercicioAnterior_fechaBalance + "' AND conceptocontable !~* '.*asiento.*(cierre|regularizaci).*' GROUP BY idcuenta) AS periodo ON periodo.idcuenta=ejercicio.idcuenta LEFT OUTER JOIN (SELECT idcuenta, (COALESCE(sum(debe),0)- COALESCE(sum(haber),0)) AS saldoant FROM apunte WHERE fecha < '01/01/" + ejercicioAnterior_fechaBalance.right ( 4 ) + "' GROUP BY idcuenta) AS anterior ON cuenta.idcuenta=anterior.idcuenta ORDER BY codigo";
    hojas = dbConnection->loadQuery ( query, "Ejercicio N-1" );
    dbConnection->commit();
    if ( hojas == NULL ) {
        blMsgInfo ( _ ( "Error con la base de datos" ) );
        return 0;
    }

    /// De nuevo, para cada cuenta con sus saldos ya calculados hay que actualizar las hojas del arbol.
    while ( !hojas->eof() ) {
        arbolEjercicioAnterior->actualizaHojas ( hojas );
        hojas->nextRecord();
    } // end while

    
    return 1;
}


/*
OK, aqui poden haver passat 3 coses.
1.- Petada de la base de dades perque el SQL es erroni o s'ha perdut la conexio.
 Aleshores el punter al cursor valdra NULL.
    El tractament d'aquesta excepcio se pot fer de dues formes:

    a) comproves que el cursor es distint de NULL abans de seguir.
 if (cur != NULL)

 b) Fas tractament de excepcions generalitzat.
     try {
         } catch (...) {
     // Si hi ha hagut el error estare en aquest punt i puc donar el missatge
        // de error i esborrar les variables que ja no faci servir.
        // Per exemple d'aquesta forma:
           blMsgInfo("Error con la base de datos");
           if (cur) delete cur;
        }


 2.- El query ha anat be pero no hi ha resultats.
     Aquesta no es una excepcio. Normalment pot passar i ha de esser contemplat.
        Imagina que per exemple amb un query buid intentem accedir a una dada del
  registre.

  valor = cur->value("idcuenta");

        Això petaria perque es un acces ilegal a una casella que no existeix.

         Per aixo normalment sempre despres de fer un loadQuery solem fer:
         if (! cur->eof()) {
         }

         o si el que volem es recorrer el query ho feim d'aquesta altre forma

         while (!cur->eof()) {
           cur->nextRecord();
         }

        Nota que pot esser normal obtenir un resultat buid, pero en canvi no sol
  esser gens normal el tenir un error de execucio (nomes quan ho estas
  programant).


 3.- El query ha anat be i hi ha resultats. Aqui no tens problemes,

*/


///
/**
\param cuenta
\return
**/
// BlFixed PluginBc_CuentasAnuales2ODS::saldoCuenta ( int cuenta )
// {
//     BL_FUNC_DEBUG
//
//     /// Ejercicio actual.
//     BlDbRecordSet *cur;
//     BlFixed resultado;
//
// //     QString query = QString( "SELECT (SUM(apunte.debe) - SUM(apunte.haber)) AS saldo FROM cuenta LEFT JOIN apunte ON apunte.idcuenta = cuenta.idcuenta WHERE cuenta.codigo like '" + QString::number ( cuenta ) + "%' AND fecha >= '01/01/" + ejercicioActual_fechaBalance.right(4) + "' AND fecha <= '" + ejercicioActual_fechaBalance +"' and apunte.conceptocontable not ilike '%%asiento%%cierre%%' and apunte.conceptocontable not ilike '%%asiento%%regularizaci%%'" );
//
//     QString query = QString( "SELECT saldototal FROM (SELECT SUBSTR(codigo, 1, 3) AS cod, SUM(saldo) as saldototal FROM cuenta LEFT JOIN (SELECT idcuenta, (SUM(debe) - SUM(haber) ) AS saldo FROM apunte WHERE fecha >= '01/01/" + ejercicioActual_fechaBalance.right(4) + "' AND fecha <= '" + ejercicioActual_fechaBalance + "' AND conceptocontable ~* '.*asiento.*(cierre|regularizaci).*' GROUP BY idcuenta) AS saldo ON saldo.idcuenta=cuenta.idcuenta GROUP BY cod ORDER BY cod) AS saldo_ctas WHERE cod=" + QString::number ( cuenta ) );
//
//     dbConnection->begin();
//     cur = dbConnection->loadQuery ( query );
//     dbConnection->commit();
//     if (cur != NULL)
//  resultado = cur->value( "saldototal" );
//     else
//  blMsgInfo(_("Error con la base de datos"));
//
//     
//     return resultado;
// }


///
/**
\param cuenta
\return
**/
BlFixed PluginBc_CuentasAnuales2ODS::saldoCuentaAnt ( int cuenta )
{
    BL_FUNC_DEBUG

    BlFixed resultado;
    QString cta = QString::number ( cuenta );

    if ( arbolEjercicioAnterior->irHoja ( cta, cta.length() ) )
        resultado = arbolEjercicioAnterior->hojaActual ( "saldo" );
    else
        resultado = BlFixed ( "0.00" );

    
    return resultado;
}

///
/**
\param cuenta
\return
**/
// BlFixed PluginBc_CuentasAnuales2ODS::saldoCuentaAnt ( int cuenta )
// {
//     BL_FUNC_DEBUG
//
//     /// Ejercicio anterior.
//     BlDbRecordSet *cur;
//     BlFixed resultado;
//
// //     QString query = QString( "SELECT (SUM(apunte.debe) - SUM(apunte.haber)) AS saldo FROM cuenta LEFT JOIN apunte ON apunte.idcuenta = cuenta.idcuenta WHERE cuenta.codigo like '" + QString::number ( cuenta ) + "%' AND fecha >= '01/01/" + ejercicioAnterior_fechaBalance.right(4) + "' AND fecha <= '" + ejercicioAnterior_fechaBalance +"' and apunte.conceptocontable not ilike '%%asiento%%cierre%%' and apunte.conceptocontable not ilike '%%asiento%%regularizaci%%'" );
//
//     QString query = QString( "SELECT saldototal FROM (SELECT SUBSTR(codigo, 1, 3) AS cod, SUM(saldo) as saldototal FROM cuenta LEFT JOIN (SELECT idcuenta, (SUM(debe) - SUM(haber) ) AS saldo FROM apunte WHERE fecha >= '01/01/" + ejercicioAnterior_fechaBalance.right(4) + "' AND fecha <= '" + ejercicioAnterior_fechaBalance + "' AND conceptocontable ~* '.*asiento.*(cierre|apertura|regularizaci).*' GROUP BY idcuenta) AS saldo ON saldo.idcuenta=cuenta.idcuenta GROUP BY cod ORDER BY cod) AS saldo_ctas WHERE cod=" + QString::number ( cuenta ) );
//
//     dbConnection->begin();
//     cur = dbConnection->loadQuery ( query );
//     dbConnection->commit();
//     if (cur != NULL)
//  resultado = cur->value( "saldototal" );
//     else
//  blMsgInfo(_("Error con la base de datos"));
//
//     
//     return resultado;
// }


///
/**
\param cuenta
\return
**/
BlFixed PluginBc_CuentasAnuales2ODS::saldoCuenta ( int cuenta )
{
    BL_FUNC_DEBUG

    BlFixed resultado;
    QString cta = QString::number ( cuenta );

    if ( arbolEjercicioActual->irHoja ( cta, cta.length() ) )
        resultado = arbolEjercicioActual->hojaActual ( "saldo" );
    else
        resultado = BlFixed ( "0.00" );

    
    return resultado;
}


///
/**
\return
**/
bool PluginBc_CuentasAnuales2ODS::formDatosBalance ( CAnuales tipus )
{
    BL_FUNC_DEBUG
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
        blMsgError ( _ ( "ERROR: Debe introducir una fecha en el balance actual." ) );
        resultado = 0;
    } else {
        if ( ejercicioAnterior_fechaBalance.isEmpty() ) {
            blMsgError ( _ ( "ERROR: Debe introducir una fecha en el balance anterior." ) );
            resultado = 0;
        }
    } // end if

    /// Se pulso el boton 'cancelar'
//     if ( resultado == 0 ) {
//         return;
//     } // end if

    

    return resultado;
}


//Cuentas Anuales Abreviadas Asociacion Sin Lucro CAAASL
//Cuentas Anuales Plan General Contable hasta 2007 CAPGC07
//Cuentas Anuales PYMES 2008 CAPYMES08
//Cuentas Anuales Plan General Contable 2008 CAPGC08
//Cuentas Anuales Abreviadas Plan General Contable 2008 CAAPGC08


///
/**
**/
void PluginBc_CuentasAnuales2ODS::balanceSituacionODS ( CAnuales tipus )
{
    BL_FUNC_DEBUG
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
        blMsgError ( _ ( "ERROR: Funcion no implementada todavia." ) );
        break;
    }

    QString archivod = g_confpr->value( CONF_DIR_USER ) + "canualesods.py";

    QString cadena = "rm " + g_confpr->value( CONF_DIR_USER ) + "canualesods.ods";
    system ( cadena.toLatin1() );
    cadena = "rm " + archivod;
    system ( cadena.toLatin1() );

    QFile file ( archivod );
    if ( file.open ( QIODevice::WriteOnly ) ) {
        QTextStream stream ( &file );
        stream.setCodec ( "UTF-8" );
        stream << archivosalida.toLatin1();
        file.close();
    } else
        blMsgError ( _ ( "ERROR: No se ha podido crear el archivo" ) );

    cadena = " cd " + g_confpr->value( CONF_DIR_USER ) + "; python " + archivod;
    system ( cadena.toLatin1() );
    cadena = g_confpr->value( CONF_ODS ) + " " + g_confpr->value( CONF_DIR_USER ) + "canualesods.ods &";
    system ( cadena.toLatin1() );
    
}

//Cuentas Anuales Abreviadas Asociacion Sin Lucro CAAASL
//Cuentas Anuales Plan General Contable hasta 2007 CAPGC07
//Cuentas Anuales PYMES 2008 CAPYMES08
//Cuentas Anuales Plan General Contable 2008 CAPGC08
//Cuentas Anuales Abreviadas Plan General Contable 2008 CAAPGC08

///
/**
\param bcont
**/
void PluginBc_CuentasAnuales2ODS::inicializa ( BcBulmaCont *bcont )
{
    BL_FUNC_DEBUG


    /// Miramos si existe la opcion de menu antes del menu Herramientas
    QMenu *pPluginMenu = bcont->newMenu ( _("&Cuentas Anuales"), "menuCAnuales", "menuHerramientas" );
    
    
    QMenu *pgc08 = new QMenu ( _ ( "&PGC 2008" ) );
    QMenu *pgc07 = new QMenu ( _ ( "PGC &Anterior 2008" ) );

    QAction *accion4 = new QAction ( _ ( "&Cuentas Anuales PGC" ), 0 );
    accion4->setStatusTip ( _ ( "Cuentas Anuales Plan General Contable 2008" ) );
    accion4->setWhatsThis ( _ ( "Cuentas Anuales Plan General Contable 2008" ) );
    connect ( accion4, SIGNAL ( activated() ), this, SLOT ( balsitCAPGC08() ) );

    QAction *accion5 = new QAction ( _ ( "Cuentas Anuales &Abreviadas PGC" ), 0 );
    accion4->setStatusTip ( _ ( "Cuentas Anuales Abreviadas Plan General Contable 2008" ) );
    accion4->setWhatsThis ( _ ( "Cuentas Anuales Abreviadas Plan General Contable 2008" ) );
    connect ( accion5, SIGNAL ( activated() ), this, SLOT ( balsitCAAPGC08() ) );

    QAction *accion3 = new QAction ( _ ( "Cuentas Anuales &PYMES" ), 0 );
    accion3->setStatusTip ( _ ( "Cuentas Anuales PYMES 2008" ) );
    accion3->setWhatsThis ( _ ( "Cuentas Anuales PYMES 2008" ) );
    connect ( accion3, SIGNAL ( activated() ), this, SLOT ( balsitCAPYMES08() ) );

    /**********************************************/

    QAction *accion2 = new QAction ( _ ( "&Cuentas Anuales PGC" ), 0 );
    accion2->setStatusTip ( _ ( "Cuentas Anuales Plan General Contable hasta 2007" ) );
    accion2->setWhatsThis ( _ ( "Cuentas Anuales Plan General Contable hasta 2007" ) );
    connect ( accion2, SIGNAL ( activated() ), this, SLOT ( balsitCAPGC07() ) );

    QAction *accion1 = new QAction ( _ ( "Cuentas A. Abreviadas &Sin lucro" ), 0 );
    accion1->setStatusTip ( _ ( "Cuentas Anuales Abreviadas Asociacion Sin animo de lucro" ) );
    accion1->setWhatsThis ( _ ( "Cuentas Anuales Abreviadas Asociacion Sin animo de lucro" ) );
    connect ( accion1, SIGNAL ( activated() ), this, SLOT ( balsitCAAASL() ) );


    pPluginMenu->addMenu ( pgc08 );
    pPluginMenu->addSeparator();
    pPluginMenu->addMenu ( pgc07 );

    pgc08->addAction ( accion4 ); //PGC
    pgc08->addAction ( accion5 );
    pgc08->addAction ( accion3 );

    pgc07->addAction ( accion2 );
    pgc07->addAction ( accion1 );



    empresaact = bcont->company();
    dbConnection = empresaact->bdempresa();
}


///
/**
\param bcont
**/
int entryPoint ( BcBulmaCont *bcont )
{
    BL_FUNC_DEBUG

    /// Inicializa el sistema de traducciones 'gettext'.
    setlocale ( LC_ALL, "" );
    blBindTextDomain ( "plugincanualesods", g_confpr->value( CONF_DIR_TRADUCCION ).toLatin1().constData() );

    PluginBc_CuentasAnuales2ODS *plug = new PluginBc_CuentasAnuales2ODS();
    plug->inicializa ( bcont );

    return 0;
}


///
/**
\param cuentaPositiva
\return
**/
BlFixed PluginBc_CuentasAnuales2ODS::cuentaPositiva ( BlFixed valor )
{
    BL_FUNC_DEBUG
    /// Comprueba que sea un numero positivo
    BlFixed resultado;
    if ( valor > BlFixed ( "0.00" ) )
        resultado = valor;
    else
        resultado = BlFixed ( "0.00" );
    return resultado;
}

///
/**
\param cuentaNegativa
\return
**/
BlFixed PluginBc_CuentasAnuales2ODS::cuentaNegativa ( BlFixed valor )
{
    BL_FUNC_DEBUG
    /// Comprueba que sea un numero negativo
    BlFixed resultado;
    if ( valor < BlFixed ( "0.00" ) )
        resultado = valor;
    else
        resultado = BlFixed ( "0.00" );
    return resultado;
}

// CAnuales CAAASL, CAPGC07, CAPYMES08, CAPGC08, CAAPGC08}

void PluginBc_CuentasAnuales2ODS::balsitCAAASL ()
{
    CAnuales CA = CAAASL;
    mensajeAdvertenciaPGC ( CA );
    if ( formDatosBalance ( CA ) && Arboles() )
        balanceSituacionODS ( CA );
}

void PluginBc_CuentasAnuales2ODS::balsitCAPGC07 ()
{
    CAnuales CA = CAPGC07;
    mensajeAdvertenciaPGC ( CA );
    if ( formDatosBalance ( CA ) && Arboles() )
        balanceSituacionODS ( CA );
}

void PluginBc_CuentasAnuales2ODS::balsitCAPYMES08 ()
{
    CAnuales CA = CAPYMES08;
    mensajeAdvertenciaPGC ( CA );
    if ( formDatosBalance ( CA ) && Arboles() )
        balanceSituacionODS ( CA );
}

void PluginBc_CuentasAnuales2ODS::balsitCAPGC08()
{
    CAnuales CA = CAPGC08;
    mensajeAdvertenciaPGC ( CA );
    if ( formDatosBalance ( CA ) && Arboles() )
        balanceSituacionODS ( CA );
}

void PluginBc_CuentasAnuales2ODS::balsitCAAPGC08()
{
    CAnuales CA = CAAPGC08;
    mensajeAdvertenciaPGC ( CA );
    if ( formDatosBalance ( CA ) && Arboles() )
        balanceSituacionODS ( CA );
}

void PluginBc_CuentasAnuales2ODS::mensajeAdvertenciaPGC ( CAnuales tipus )
{
//Cuentas Anuales Abreviadas Asociacion Sin Lucro CAAASL
//Cuentas Anuales Plan General Contable hasta 2007 CAPGC07
//Cuentas Anuales PYMES 2008 CAPYMES08
//Cuentas Anuales Plan General Contable 2008 CAPGC08
//Cuentas Anuales Abreviadas Plan General Contable 2008 CAAPGC08

    QString pgc;

    switch ( tipus ) {
    case CAAASL:
        pgc = _ ( "Cuentas Anuales Abreviadas para Asociacion Sin Lucro. " );
        break;

    case CAPGC07:
        pgc = _ ( "Cuentas Anuales del Plan General Contable hasta 2007. " );
        break;

    case CAPYMES08:
        pgc = _ ( "Cuentas Anuales para PYMES 2008. " ) ;
        break;

    case CAAPGC08:
        pgc = _ ( "Cuentas Anuales Abreviadas del Plan General Contable 2008. " );
        break;

    case CAPGC08:
        pgc = _ ( "Cuentas Anuales del Plan General Contable 2008. " );
        break;

    default:
        blMsgError ( _ ( "ERROR: Funcion no implementada todavia." ) );
        break;
    }

    blMsgWarning ( _ ( "Advertencia: ha escojido las " ) + pgc + _ ( "El resultado solo sera correcto si coincide con el plan contable de su empresa." ) );
}

