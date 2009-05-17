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

#include <QMenu>
#include <QAction>
#include <QObject>
#include <QMessageBox>

#include <cstdio>

#include "pluginbc_caaslods.h"
#include "bccompany.h"
#include "blfunctions.h"
#include "datosview.h"
#include "blprogressbar.h"


BcCompany *g_comp = NULL;

///
/**
**/
pluginBC_caaslODS::pluginBC_caaslODS()
{
    _depura ( "pluginBC_caaslODS::pluginBC_caaslODS", 0 );
    _depura ( "END pluginBC_caaslODS::pluginBC_caaslODS", 0 );
}

///
/**
**/
pluginBC_caaslODS::~pluginBC_caaslODS()
{
    _depura ( "pluginBC_caaslODS::~pluginBC_caaslODS", 0 );
    _depura ( "END pluginBC_caaslODS::~pluginBC_caaslODS", 0 );
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
           mensajeInfo("Error con la base de datos");
           if (cur) delete cur;
        }


 2.- El query ha anat be pero no hi ha resultats.
     Aquesta no es una excepcio. Normalment pot passar i ha de esser contemplat.
        Imagina que per exemple amb un query buid intentem accedir a una dada del
  registre.

  valor = cur->valor("idcuenta");

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
// BlFixed pluginBC_caaslODS::saldoCuenta ( int cuenta )
// {
//     _depura ( "pluginBC_caaslODS::saldoCuenta", 0 );
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
//  resultado = cur->valor ( "saldototal" );
//     else
//  mensajeInfo(_("Error con la base de datos"));
//
//     _depura ( "END pluginBC_caaslODS::saldoCuenta", 0 );
//     return resultado;
// }



///
/**
\param cuenta
\return
**/
// BlFixed pluginBC_caaslODS::saldoCuentaAnt ( int cuenta )
// {
//     _depura ( "pluginBC_caaslODS::saldoCuentaAnt", 0 );
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
//  resultado = cur->valor ( "saldototal" );
//     else
//  mensajeInfo(_("Error con la base de datos"));
//
//     _depura ( "END pluginBC_caaslODS::saldoCuentaAnt", 0 );
//     return resultado;
// }





//Cuentas Anuales Abreviadas Asociacion Sin Lucro CAAASL
//Cuentas Anuales Plan General Contable hasta 2007 CAPGC07
//Cuentas Anuales PYMES 2008 CAPYMES08
//Cuentas Anuales Plan General Contable 2008 CAPGC08
//Cuentas Anuales Abreviadas Plan General Contable 2008 CAAPGC08

///
/**
\param bcont
**/
void pluginBC_caaslODS::inicializa ( BcBulmaCont *bcont )
{
    _depura ( "pluginBC_caaslODS::inicializa", 0 );

    /// Creamos el men&uacute;.
    QMenu *pPluginMenu = new QMenu ( _ ( "&Cuentas Anuales" ) );

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

    QAction *accion1 = new QAction ( _ ( "Cuentas A. Abreviadas &Sin lucro (Nuevo)" ), 0 );
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


    /// A&ntilde;adimos la nueva opci&oacute;n al men&uacute; principal del programa.
    bcont->menuBar() ->addMenu ( pPluginMenu );

    empresaact = bcont->empresaactual();
    dbConnection = empresaact->bdempresa();

    _depura ( "END pluginBC_caaslODS::inicializa", 0 );
}


///
/**
\param bcont
**/
int entryPoint ( BcBulmaCont *bcont )
{
    _depura ( "Estoy dentro del plugin\n", 0 );

    /// Inicializa el sistema de traducciones 'gettext'.
    setlocale ( LC_ALL, "" );
    bindtextdomain ( "pluginbc_caaslods", g_confpr->valor ( CONF_DIR_TRADUCCION ).toAscii().constData() );

    pluginBC_caaslODS *plug = new pluginBC_caaslODS();
    plug->inicializa ( bcont );
    /// S&Oacute;LO A MODO DE EJEMPLO: se modifica el t&iacute;tulo de la ventana principal
    /// del programa para indicar que el plugin se ha cargado.
    return 0;
}


// CAnuales CAAASL, CAPGC07, CAPYMES08, CAPGC08, CAAPGC08}
//Cuentas Anuales Abreviadas Asociacion Sin Lucro CAAASL
//Cuentas Anuales Plan General Contable hasta 2007 CAPGC07
//Cuentas Anuales PYMES 2008 CAPYMES08
//Cuentas Anuales Plan General Contable 2008 CAPGC08
//Cuentas Anuales Abreviadas Plan General Contable 2008 CAAPGC08

  

void pluginBC_caaslODS::balsitCAAASL ()
{
   mensajeAdvertenciaPGC ((CAnuales)CAAASL);
	formDatosBalance("inf_caaasl.pys");
}

void pluginBC_caaslODS::balsitCAPGC07 ()
{
   mensajeAdvertenciaPGC ( (CAnuales)CAPGC07);
   formDatosBalance("inf_capgc07.pys");
}

void pluginBC_caaslODS::balsitCAPYMES08 ()
{
   mensajeAdvertenciaPGC ( (CAnuales)CAPYMES08);
   formDatosBalance("inf_capymes08.pys");
}

void pluginBC_caaslODS::balsitCAPGC08()
{
   mensajeAdvertenciaPGC ( (CAnuales)CAPGC08 );
   formDatosBalance("inf_capgc08.pys");
}

void pluginBC_caaslODS::balsitCAAPGC08()
{
   mensajeAdvertenciaPGC ( (CAnuales)CAAPGC08 );
   formDatosBalance("inf_caapgc08.pys");
}

void pluginBC_caaslODS::mensajeAdvertenciaPGC ( CAnuales tipus )
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
        mensajeError ( _ ( "ERROR: Funcion no implementada todavia." ) );
        break;
    }

    mensajeAviso ( _ ( "Advertencia: ha escojido las " ) + pgc + _ ( "El resultado solo sera correcto si coincide con el plan contable de su empresa." ) );
}

///
/**
\return
**/
void pluginBC_caaslODS::formDatosBalance ( QString informe )
{
      _depura ( "pluginBC_caaslODS::formDatosBalance", 0 );

      QDialog *diag = new QDialog ( 0 );
      diag->setModal ( true );
      diag->setGeometry ( QRect ( 0, 0, 450, 350 ) );
      centrarEnPantalla ( diag );

      DatosView *pidefechas = new DatosView ( g_comp, diag, 0, BL_SELECT_MODE );

      pidefechas->m_informe = informe;

      /// Creamos un layout donde estara el contenido de la ventana y la ajustamos al QDialog
      /// para que sea redimensionable y aparezca el titulo de la ventana.
      QHBoxLayout *layout = new QHBoxLayout;
      layout->addWidget ( pidefechas );
      layout->setMargin ( 0 );
      layout->setSpacing ( 0 );
      diag->setLayout ( layout );
      diag->setWindowTitle ( pidefechas->windowTitle() );
      
      pidefechas->connect ( pidefechas->mui_aceptar, SIGNAL ( clicked () ), diag, SLOT ( accept() ) );
      pidefechas->connect ( pidefechas->mui_cancelar, SIGNAL ( clicked () ), diag, SLOT ( accept() ) );
      
      while(!pidefechas->resultado)
      {
         diag->exec();
      }
      
      delete diag;

      _depura ( "END pluginBC_caaslODS::formDatosBalance", 0 );
}

