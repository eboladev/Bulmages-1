/***************************************************************************
 *   Copyright (C) 2005 by Tomeu Borras Riera                              *
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

#ifdef TOMEU

#include "linprevcobro.h"
#include "bcasientointeligenteview.h"
#include "bcasientoview.h"
#include "pluginbc_asiento.h"

///
/**
\param comp
**/
linprevcobro::linprevcobro ( BcCompany *comp )
{
    BL_FUNC_DEBUG
    company = comp;
    dbConnection = comp->bdempresa();
    vacialinprevcobro();
    
}


///
/**
\param comp
\param idprevcobro
**/
linprevcobro::linprevcobro ( BcCompany *comp, QString idprevcobro )
{
    BL_FUNC_DEBUG
    company = comp;
    dbConnection = comp->bdempresa();
    QString SQLQuery = "SELECT * FROM prevcobro "
                       "LEFT JOIN  cuenta ON prevcobro.idcuenta = cuenta.idcuenta "
                       "LEFT JOIN (SELECT codigo AS codigoctacliente, descripcion AS nomctacliente, idcuenta AS idctacliente FROM cuenta) AS T1 ON T1.idctacliente = prevcobro.idctacliente "
                       "WHERE idprevcobro = " + idprevcobro;
    BlDbRecordSet *cur = dbConnection->loadQuery ( SQLQuery );
    if ( !cur->eof() ) {
        mdb_idprevcobro = cur->value( "idprevcobro" );
        mdb_fprevistaprevcobro = cur->value( "fprevistaprevcobro" );
        mdb_fcobroprevcobro = cur->value( "fcobroprevcobro" );
        mdb_idfpago = cur->value( "idfpago" );
        mdb_idcuenta = cur->value( "idcuenta" );
        mdb_idasiento = cur->value( "idasiento" );
        mdb_cantidadprevistaprevcobro = cur->value( "cantidadprevistaprevcobro" );
        mdb_cantidadprevcobro = cur->value( "cantidadprevcobro" );
        mdb_idregistroiva = cur->value( "idregistroiva" );
        mdb_tipoprevcobro = cur->value( "tipoprevcobro" );
        mdb_docprevcobro = cur->value( "docprevcobro" );
        mdb_codigocuenta = cur->value( "codigo" );
        mdb_nomcuenta = cur->value( "descripcion" );
        mdb_idctacliente = cur->value( "idctacliente" );
        mdb_codigoctacliente = cur->value( "codigoctacliente" );
        mdb_nomctacliente = cur->value( "nomctacliente" );
    } else {
        vacialinprevcobro();
    }// end if
    
}


///
/**
\param comp
\param a
\param b
\param c
\param d
\param e
\param f
\param g
\param h
\param i
\param j
\param k
\param l
\param m
\param idctacliente
\param codigoctacliente
\param nomctacliente
**/
linprevcobro::linprevcobro ( BcCompany *comp, QString a, QString b, QString c, QString d, QString e, QString f, QString g, QString h, QString i, QString j, QString k, QString l, QString m, QString idctacliente, QString codigoctacliente, QString nomctacliente )
{
    BL_FUNC_DEBUG
    company = comp;
    dbConnection = comp->bdempresa();
    mdb_idprevcobro = a;
    mdb_fprevistaprevcobro = b;
    mdb_fcobroprevcobro = c;
    mdb_idfpago = d;
    mdb_idcuenta = e;
    mdb_idasiento = f;
    mdb_cantidadprevistaprevcobro = g;
    mdb_cantidadprevcobro = h;
    mdb_idregistroiva = i;
    mdb_tipoprevcobro = j;
    mdb_docprevcobro = k;
    mdb_codigocuenta = l;
    mdb_nomcuenta = m;
    mdb_idctacliente = idctacliente;
    mdb_codigoctacliente = codigoctacliente;
    mdb_nomctacliente = nomctacliente;
    
}


///
/**
**/
linprevcobro::~linprevcobro()
{
    BL_FUNC_DEBUG
    
}


///
/**
**/
void linprevcobro::vacialinprevcobro()
{
    BL_FUNC_DEBUG
    mdb_idprevcobro = "";
    mdb_fprevistaprevcobro = "";
    mdb_fcobroprevcobro = "";
    mdb_idfpago = "";
    mdb_idcuenta = "";
    mdb_idasiento = "";
    mdb_cantidadprevistaprevcobro = "";
    mdb_cantidadprevcobro = "";
    mdb_idregistroiva = "";
    mdb_tipoprevcobro = "";
    mdb_docprevcobro = "";
    mdb_codigocuenta = "";
    mdb_nomcuenta = "";
    mdb_idctacliente = "";
    mdb_codigoctacliente = "";
    mdb_nomctacliente = "";
    
}


///
/**
**/
void linprevcobro::remove()
{
    BL_FUNC_DEBUG
    if ( mdb_idprevcobro != "" ) {
        dbConnection->begin();
        dbConnection->runQuery ( "DELETE FROM prevcobro WHERE idprevcobro = " + mdb_idprevcobro );
        dbConnection->commit();
        vacialinprevcobro();
    } // end if
    
}


///
/**
**/
void linprevcobro::guardalinprevcobro()
{
    BL_FUNC_DEBUG
    if ( mdb_idfpago == "" ) {
        mdb_idfpago = "NULL";
    } // end if
    if ( mdb_idasiento == "" ) {
        mdb_idasiento = "NULL";
    } // end if
    if ( mdb_idregistroiva == "" ) {
        mdb_idregistroiva = "NULL";
    } // end if
    /// Seg&uacute;n esta la l&iacute;nea en la base de datos o no se hace una cosa u otra.
    if ( mdb_idprevcobro == "" ) {
        QString SQLQuery = "INSERT INTO prevcobro (fprevistaprevcobro, fcobroprevcobro, idfpago, idcuenta, idasiento, cantidadprevistaprevcobro, cantidadprevcobro, idregistroiva, tipoprevcobro, docprevcobro, idctacliente) VALUES ('" +
                           dbConnection->sanearCadena ( mdb_fprevistaprevcobro ) + "'," +
                           "'" + dbConnection->sanearCadena ( mdb_fcobroprevcobro ) + "'," +
                           dbConnection->sanearCadena ( mdb_idfpago ) + "," +
                           dbConnection->sanearCadena ( mdb_idcuenta ) + "," +
                           dbConnection->sanearCadena ( mdb_idasiento ) + "," +
                           dbConnection->sanearCadena ( mdb_cantidadprevistaprevcobro ) + ", " +
                           dbConnection->sanearCadena ( mdb_cantidadprevcobro ) + ", " +
                           dbConnection->sanearCadena ( mdb_idregistroiva ) + ", " +
                           "'" + dbConnection->sanearCadena ( mdb_tipoprevcobro ) + "', " +
                           "'" + dbConnection->sanearCadena ( mdb_docprevcobro ) + "', " +
                           dbConnection->sanearCadena ( mdb_idctacliente ) + ")";
        dbConnection->begin();
        dbConnection->runQuery ( SQLQuery );
        BlDbRecordSet *cur = dbConnection->loadQuery ( "SELECT MAX(idprevcobro) AS m FROM prevcobro " );
        if ( !cur->eof() ) {
            mdb_idprevcobro = cur->value( "m" );
        } // end if
        delete cur;
        dbConnection->commit();
    } else {
        QString SQLQuery = "UPDATE prevcobro SET ";
        SQLQuery += " fprevistaprevcobro = '" + dbConnection->sanearCadena ( mdb_fprevistaprevcobro ) + "' ";
        SQLQuery += " ,fcobroprevcobro = '" + dbConnection->sanearCadena ( mdb_fcobroprevcobro ) + "' ";
        SQLQuery += " ,idfpago = " + dbConnection->sanearCadena ( mdb_idfpago ) + " ";
        SQLQuery += " ,idcuenta = " + dbConnection->sanearCadena ( mdb_idcuenta ) + " ";
        SQLQuery += " ,idasiento = " + dbConnection->sanearCadena ( mdb_idasiento ) + " ";
        SQLQuery += " ,cantidadprevistaprevcobro = " + dbConnection->sanearCadena ( mdb_cantidadprevistaprevcobro ) + " ";
        SQLQuery += " ,cantidadprevcobro = " + dbConnection->sanearCadena ( mdb_cantidadprevcobro ) + " ";
        SQLQuery += " ,idregistroiva = " + dbConnection->sanearCadena ( mdb_idregistroiva ) + " ";
        SQLQuery += " ,tipoprevcobro = '" + dbConnection->sanearCadena ( mdb_tipoprevcobro ) + "' ";
        SQLQuery += " ,docprevcobro = '" + dbConnection->sanearCadena ( mdb_docprevcobro ) + "' ";
        SQLQuery += " ,idctacliente = " + dbConnection->sanearCadena ( mdb_idctacliente ) + " ";
        SQLQuery += " WHERE idprevcobro = " + dbConnection->sanearCadena ( mdb_idprevcobro );
        dbConnection->begin();
        dbConnection->runQuery ( SQLQuery );
        dbConnection->commit();
    } // end if
    
}


///
/**
\param val
**/
void linprevcobro::setcodigocuenta ( QString val )
{
    BL_FUNC_DEBUG
    fprintf ( stderr, "setcodigocuenta(%s)\n", val.toLatin1().constData() );
    mdb_codigocuenta = blExtendStringWithZeros ( val, company->numDigitosEmpresa() );
    QString SQLQuery = "SELECT idcuenta, descripcion FROM cuenta WHERE codigo = '" + mdb_codigocuenta + "'";
    BlDbRecordSet *cur = dbConnection->loadQuery ( SQLQuery );
    if ( !cur->eof() ) {
        mdb_nomcuenta = cur->value( "descripcion" );
        mdb_idcuenta = cur->value( "idcuenta" );
    } // end if
    delete cur;
    
}


///
/**
\param val
**/
void linprevcobro::setidcuenta ( QString val )
{
    BL_FUNC_DEBUG
    fprintf ( stderr, "setidcuenta(%s)\n", val.toLatin1().constData() );
    mdb_idcuenta = val;
    QString SQLQuery = "SELECT codigo, descripcion FROM cuenta WHERE idcuenta = " + mdb_idcuenta + "";
    BlDbRecordSet *cur = dbConnection->loadQuery ( SQLQuery );
    if ( !cur->eof() ) {
        mdb_nomcuenta = cur->value( "descripcion" );
        mdb_codigocuenta = cur->value( "codigo" );
    } // end if
    delete cur;
    fprintf ( stderr, "end setidcuenta\n" );
    
}



/**
  * \brief SLOT que respoonde a la creacion de un asiento de cobro o pago a partir de la gestion de cobros y pagos.
  * Descripci&oacute;n:
  * 1.- Calculamos los campos Total, Tipo de Asiento (compra/venta), Cuenta bancaria y cuenta de cliente
  * 2.- Determinamos si es un cobro o un pago.
  * 3.- Cargamos la plantilla de cobro o pago y le metemos los valores necesarios
  * 4.- Generamos el asiento a partir del asiento inteligente.
  */
/**
\return
**/
int linprevcobro::creaPago()
{
    BL_FUNC_DEBUG
    /// Si la previsi&oacute;n no esta guardada en la base de datos salimos para que no
    /// haya problemas.
    if ( idprevcobro() == "" )
        return 0;
    QString idainteligente;
    QString total = cantidadprevcobro();
    QString codcuenta = codigoctacliente();
    /// Hay que hacer que en cobros y pagos aparezca el c&oacute;digo de cuenta.
    QString codbanco = codigocuenta();
    QString tipo = tipoprevcobro();
    QString fecha = fcobroprevcobro();

    if ( tipo == "t" ) {
        /// Buscamos cual es el asiento inteligente que realiza los cobros.
        QString query = "SELECT * FROM ainteligente, configuracion WHERE descripcion = valor AND configuracion.nombre = 'Cobro'";
        BlDbRecordSet *cur = dbConnection->loadQuery ( query );
        if ( !cur->eof() ) {
            idainteligente = cur->value( "idainteligente" );
        } else {
            return 0;
        }  // end if
        delete cur;
    } else {
        /// Buscamos cual es el asiento inteligente que realiza los pagos.
        QString query = "SELECT * FROM ainteligente, configuracion WHERE descripcion = valor AND configuracion.nombre = 'Pago'";
        BlDbRecordSet *cur = dbConnection->loadQuery ( query );
        if ( !cur->eof() ) {
            idainteligente = cur->value( "idainteligente" );
        } else {
            return 0;
        } // end if
        delete cur;
    }// end if
    /// Para saber si al final se ha hecho el asiento o no almacenamos el valor
    /// actual del asiento para ver despues cual es el numero de asiento y comprobar
    /// si ha cambiado o no.
//    QString idasiento = company->intapuntsempresa() ->idasiento();
    QString idasiento = g_asiento ->idasiento();

    QDialog *diag = new QDialog ( 0 );
    diag->setModal ( true );
    /// Se va a generar el asiento.
    int numasiento = 0;
    BcAsientoInteligenteView *nueva = new BcAsientoInteligenteView ( company, diag );
    nueva->inicializa ( numasiento );
    nueva->muestraPlantilla ( idainteligente );
    nueva->setFechaAsiento ( fecha );
    nueva->setValores ( "$fecha$", fecha );
    nueva->setValores ( "$codbanco$", codbanco );
    nueva->setValores ( "$codcuenta$", codcuenta );
    nueva->setValores ( "$total$", total );
    nueva->setModo ( 1 );

    diag->exec();

//    QString idasiento1 = company->intapuntsempresa() ->idasiento();
    QString idasiento1 = g_asiento ->idasiento();
    if ( idasiento1 == idasiento ) {
        delete diag;
        return 0;
    } // end if
    mdb_idasiento = idasiento1;
    guardalinprevcobro();
    delete diag;
    
    return 1;
}


///
/**
\param val
**/
void linprevcobro::setcodigoctacliente ( QString val )
{
    BL_FUNC_DEBUG
    fprintf ( stderr, "setcodigocuenta(%s)\n", val.toLatin1().constData() );
    mdb_codigoctacliente = blExtendStringWithZeros ( val, company->numDigitosEmpresa() );
    QString SQLQuery = "SELECT idcuenta, descripcion FROM cuenta WHERE codigo = '" + mdb_codigoctacliente + "'";
    BlDbRecordSet *cur = dbConnection->loadQuery ( SQLQuery );
    if ( !cur->eof() ) {
        mdb_nomctacliente = cur->value( "descripcion" );
        mdb_idctacliente = cur->value( "idcuenta" );
    }// end if
    delete cur;
    
}


///
/**
\param val
**/
void linprevcobro::setidctacliente ( QString val )
{
    BL_FUNC_DEBUG
    fprintf ( stderr, "setidcuenta(%s)\n", val.toLatin1().constData() );
    mdb_idctacliente = val;
    QString SQLQuery = "SELECT codigo, descripcion FROM cuenta WHERE idcuenta = " + mdb_idctacliente + "";
    BlDbRecordSet *cur = dbConnection->loadQuery ( SQLQuery );
    if ( !cur->eof() ) {
        mdb_nomctacliente = cur->value( "descripcion" );
        mdb_codigoctacliente = cur->value( "codigo" );
    } // end if
    delete cur;
    fprintf ( stderr, "end setidcuenta\n" );
    
}

#endif
