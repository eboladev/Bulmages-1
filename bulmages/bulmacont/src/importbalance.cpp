/***************************************************************************
 *   Copyright (C) 2004 by Tomeu Borras Riera                              *
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

#include <QString>

#include "importbalance.h"
#include "bccompany.h"
#include "blfunctions.h"

/// Constructor de la clase que inicializa la empresa actual.
/** La base de datos establece que de momento no se ha visitado ning&uacute;n tag .
    Establece que no hay ning&uacute;n dato recogido del XML.
    Inidca que el paso que vamos a procesar es el 0. */
/**
\param emp
**/
ImportBalance::ImportBalance ( BcCompany *emp ) : QXmlDefaultHandler(), BlMainCompanyPointer ( emp )
{
    _depura ( "ImportBalance::ImportBalance", 0 );
    m_tag = "";
    m_data = "";
    m_paso = 0;
    _depura ( "END ImportBalance::ImportBalance", 0 );
}


void ImportBalance::cambiapaso()
{
    m_paso = 1;
}


/** Dependiendo del paso en que nos encontremos hace la llamada a una funcio u a otra. */
/**
\param a
\param b
\param c
\param d
\return
**/
bool ImportBalance::startElement ( const QString &a, const QString &b, const QString &c, const QXmlAttributes &d )
{
    _depura ( "ImportBalance::startElement", 0 );
    if ( m_paso == 0 ) {
        return startElement1 ( a, b, c, d );
    } else {
        return startElement2 ( a, b, c, d );
    } // end if
    _depura ( "END ImportBalance::startElement", 0 );
}

/// Dependiendo del paso en que nos encontremos hace la llamada a una funcion u a otra.
/**
\param a
\param b
\param c
\return
**/
bool ImportBalance::endElement ( const QString &a, const QString &b, const QString &c )
{
    _depura ( "ImportBalance::endlement", 0 );
    if ( m_paso == 0 ) {
        return endElement1 ( a, b, c );
    } else {
        return endElement2 ( a, b, c );
    } // end if
    _depura ( "END ImportBalance::startElement", 0 );
}


/// En el primer paso de la importaci&oacute;n se hace la inserci&oacute;n de todos
/// los tags balance y mpatrimonial.
/** Tambi&eacute;n se guarda en m_tvalores los identificadores obtenidos puesto que
    posteriormente seran utilizados.
    Adem&aacute;s se asigna la variable global m_tag con el nombre del tag para poder
    hacer el almacenamiento de datos en el caso de que sea un tag secundario.
    \param qName el nombre del tag que se ha encontrado.
    \return TRUE para no detener la ejecucion del parser en caso de error. */
bool ImportBalance::startElement1 ( const QString&, const QString&, const QString& qName, const QXmlAttributes& )
{
    _depura ( "ImportBalance::startElement1", 0 );
    m_tag = qName;
    QString SQLQuery;
    if ( m_tag == "balance" ) {
        SQLQuery.sprintf ( "INSERT INTO balance (nombrebalance) VALUES ('Elemento importado')\n" );
        mainCompany() ->begin();
        mainCompany() ->runQuery ( SQLQuery );
        SQLQuery = "SELECT max(idbalance) AS idbalance FROM balance";
        BlDbRecordSet *cur = mainCompany() ->loadQuery ( SQLQuery, "unquerymas" );
        mainCompany() ->commit();
        if ( !cur->eof() ) {
            m_tvalores["idbalance"] = cur->valor ( "idbalance" );
        } // end if
        delete cur;
    } // end if
    if ( m_tag == "mpatrimonial" ) {
        SQLQuery.sprintf ( "INSERT INTO mpatrimonial (idbalance) VALUES (%s)\n", mainCompany() ->sanearCadena ( m_tvalores["idbalance"] ).toAscii().constData() );
        mainCompany() ->begin();
        mainCompany() ->runQuery ( SQLQuery );
        SQLQuery = "SELECT max(idmpatrimonial) AS idmpatrimonial FROM mpatrimonial";
        BlDbRecordSet *cur = mainCompany() ->loadQuery ( SQLQuery, "unquerymas" );
        mainCompany() ->commit();
        if ( !cur->eof() ) {
            m_tvalores["idmpatrimonial_nueva"] = cur->valor ( "idmpatrimonial" );
        } // end if
        delete cur;
    } // end if
    _depura ( "END ImportBalance::startElement1", 0 );
    return TRUE;
}


/// En el primer paso al encontrar un tag de cierre o bien es el de balance o bien es el
/// de mpatrimonial.
/** Lo que hacemos en ambos casos es hacer un update de la informaci&oacute;n puesto
    que toda la informacion que precisamos ya ha sido extraida del archivo XML.
    Al finalizar el m&eacute;todo establecemos el campo m_tag a "" para que no se
    asignen posibles transfugas a este valor el mapa m_identmasasp guarda la
    correlaci&oacute;n entre identificadores de masa para que las que son valores
    argumentados en el paso 1 puedan coincidir. (Es el problema de los identificadores
    autonum&eacute;ricos).
    \param qName El nombre del tag cerrado.
    \return TRUE para no detener la ejecucion del parser SAX. */
bool ImportBalance::endElement1 ( const QString&, const QString&, const QString& qName )
{
    _depura ( "ImportBalance::endElement1", 0 );
    m_tag = qName;
    QString SQLQuery;
    if ( qName == "balance" ) {
        SQLQuery.sprintf ( "UPDATE balance SET nombrebalance = '%s' WHERE idbalance = %s\n", m_tvalores["nombrebalance"].toAscii().constData(), m_tvalores["idbalance"].toAscii().constData() );
        mainCompany() ->begin();
        mainCompany() ->runQuery ( SQLQuery );
        mainCompany() ->commit();
    }
    if ( qName == "mpatrimonial" ) {
        /// En la inserci&oacute;n de masas patrimoniales hay que hacer una
        /// reconversi&oacute;n de la clave.
        /// Cuando todo haya terminado debemos actualizar el cambo idmpatrimonial de los
        /// compmasap para que la cosa funcione.
        m_identmasasp[m_tvalores["idmasa"]] = m_tvalores["idmpatrimonial_nueva"];
        SQLQuery.sprintf ( "UPDATE mpatrimonial SET descmpatrimonial = '%s' WHERE idmpatrimonial = %s\n", m_tvalores["descmpatrimonial"].toAscii().constData(), m_tvalores["idmpatrimonial_nueva"].toAscii().constData() );
        mainCompany() ->begin();
        mainCompany() ->runQuery ( SQLQuery );
        mainCompany() ->commit();
        if ( m_tvalores["orden"] != "" ) {
            SQLQuery.sprintf ( "UPDATE mpatrimonial SET orden = %s WHERE idmpatrimonial = %s\n", m_tvalores["orden"].toAscii().constData(), m_tvalores["idmpatrimonial_nueva"].toAscii().constData() );
            mainCompany() ->begin();
            mainCompany() ->runQuery ( SQLQuery );
            mainCompany() ->commit();
            m_tvalores["orden"] = "";
        } // end if
        if ( m_tvalores["tabulacion"] != "" ) {
            SQLQuery.sprintf ( "UPDATE mpatrimonial SET tabulacion = %s WHERE idmpatrimonial = %s\n", m_tvalores["tabulacion"].toAscii().constData(), m_tvalores["idmpatrimonial_nueva"].toAscii().constData() );
            mainCompany() ->begin();
            mainCompany() ->runQuery ( SQLQuery );
            mainCompany() ->commit();
            m_tvalores["tabulacion"] = "";
        } // end if
        if ( m_tvalores["opdesc"] != "" ) {
            SQLQuery.sprintf ( "UPDATE mpatrimonial SET opdesc = %s WHERE idmpatrimonial = %s\n", m_tvalores["opdesc"].toAscii().constData(), m_tvalores["idmpatrimonial_nueva"].toAscii().constData() );
            mainCompany() ->begin();
            mainCompany() ->runQuery ( SQLQuery );
            mainCompany() ->commit();
            m_tvalores["opdesc"] = "";
        } // end if
        if ( m_tvalores["tipompatrimonial"] != "" ) {
            SQLQuery.sprintf ( "UPDATE mpatrimonial SET tipompatrimonial = %s WHERE idmpatrimonial=%s\n", m_tvalores["tipompatrimonial"].toAscii().constData(), m_tvalores["idmpatrimonial_nueva"].toAscii().constData() );
            mainCompany() ->begin();
            mainCompany() ->runQuery ( SQLQuery );
            mainCompany() ->commit();
            m_tvalores["tipompatrimonial"] = "";
        } // end if
    } // end if
    m_tag = "";
    m_data = "";
    _depura ( "END ImportBalance::endElement1", 0 );
    return TRUE;
}


/// En el segundo paso se registran compmasap y compbalance.
/** Estos elementos no requieren que se haga primero una inserci&oacute;n y luego una
    actualizaci&oacute;n.
    \param qName el nombre del tag abierto.
    \return TRUE para no detener la ejecuci&oacute;n del algoritmo. */
bool ImportBalance::startElement2 ( const QString&, const QString&, const QString &qName, const QXmlAttributes& )
{
    _depura ( "ImportBalance::startElement2", 0 );
    m_tag = qName;
    _depura ( "END ImportBalance::startElement2", 0 );
    return TRUE;
}


/// En el segundo paso introducimos los compmasap y los compbalance.
/** Siempre se insertan por flanco final porque no tienen dependencias de nada y son
    sencillos. Se usa el mapa m_identmasasp para saber que identificadores de masa se
    han usado en el paso 0.
    \param qName contiene el valor del tag que se acaba de cerrar.
    \return TRUE para no detener nunca la ejecucion del algoritmo.  */
bool ImportBalance::endElement2 ( const QString&, const QString&, const QString &qName )
{
    _depura ( "ImportBalance::endElement2", 0 );
    m_tag = qName;
    QString SQLQuery;
    /// En el segundo paso se hacen las inserciones por el flanco de final en lugar de por
    /// el flanco de principio.
    /// As&iacute; nos aseguramos que ya existen los valores de idmpatrimonial y
    /// masaperteneciente.
    if ( m_tag == "compmasap" ) {
        SQLQuery.sprintf ( "INSERT INTO compmasap (masaperteneciente) VALUES (%s)\n", mainCompany() ->sanearCadena ( m_identmasasp[m_tvalores["masaperteneciente"]] ).toAscii().constData() );
        mainCompany() ->begin();
        mainCompany() ->runQuery ( SQLQuery );
        SQLQuery = "SELECT max(idcompmasap) AS idcompmasap FROM compmasap";
        BlDbRecordSet *cur = mainCompany() ->loadQuery ( SQLQuery, "unquerymas" );
        mainCompany() ->commit();
        if ( !cur->eof() ) {
            m_tvalores["idcompmasap"] = cur->valor ( "idcompmasap" );
        } // end if
        delete cur;
    } // end if
    if ( m_tag == "compbalance" ) {
        SQLQuery.sprintf ( "INSERT INTO compbalance (idbalance) VALUES (%s)\n", mainCompany() ->sanearCadena ( m_tvalores["idbalance"] ).toAscii().constData() );
        mainCompany() ->begin();
        mainCompany() ->runQuery ( SQLQuery );
        SQLQuery = "SELECT max(idcompbalance) AS idcompbalance FROM compbalance";
        BlDbRecordSet *cur = mainCompany() ->loadQuery ( SQLQuery, "unquerymas" );
        mainCompany() ->commit();
        if ( !cur->eof() ) {
            m_tvalores["idcompbalance"] = cur->valor ( "idcompbalance" );
        } // end if
        delete cur;
    } // end if
    /// Como la inserci&oacute;n est&aacute; hecha, podemos hacer los updates sin miedo a
    /// que no exista el registro.
    if ( m_tag == "compmasap" ) {
        /// Actualizamos el idmasapatrimonial del compmasap, que es el que m&aacute;s
        /// dolores de cabeza causa.
        if ( m_tvalores["idmpatrimonial"] != "" && m_tvalores["codigo"] == "" ) {
            SQLQuery.sprintf ( "UPDATE compmasap SET idmpatrimonial = %s WHERE idcompmasap = %s\n", mainCompany() ->sanearCadena ( m_identmasasp[m_tvalores["idmpatrimonial"]] ).toAscii().constData(), mainCompany() ->sanearCadena ( m_tvalores["idcompmasap"] ).toAscii().constData() );
            mainCompany() ->begin();
            mainCompany() ->runQuery ( SQLQuery );
            mainCompany() ->commit();
        } // end if
        if ( m_tvalores["codigo"] != "" ) {
            SQLQuery.sprintf ( "UPDATE compmasap SET idcuenta = id_cuenta('%s') WHERE idcompmasap = %s\n",
                               mainCompany() ->sanearCadena ( m_tvalores["codigo"] ).toAscii().constData(),
                               mainCompany() ->sanearCadena ( m_tvalores["idcompmasap"] ).toAscii().constData() );
            mainCompany() ->begin();
            mainCompany() ->runQuery ( SQLQuery );
            mainCompany() ->commit();
        } // end if
        SQLQuery.sprintf ( "UPDATE compmasap SET signo = '%s' WHERE idcompmasap = %s\n",
                           mainCompany() ->sanearCadena ( m_tvalores["signo"] ).toAscii().constData(),
                           mainCompany() ->sanearCadena ( m_tvalores["idcompmasap"] ).toAscii().constData() );
        mainCompany() ->begin();
        mainCompany() ->runQuery ( SQLQuery );
        mainCompany() ->commit();
        SQLQuery.sprintf ( "UPDATE compmasap SET nombre = '%s' WHERE idcompmasap = %s\n",
                           mainCompany() ->sanearCadena ( m_tvalores["nombre"] ).toAscii().constData(),
                           mainCompany() ->sanearCadena ( m_tvalores["idcompmasap"] ).toAscii().constData() );
        mainCompany() ->begin();
        mainCompany() ->runQuery ( SQLQuery );
        mainCompany() ->commit();
        m_tvalores["codigo"] = "";
        m_tvalores["idmpatrimonial"] = "";
    } // end if
    if ( m_tag == "compbalance" ) {
        /// Con los componentes del balance tambi&eacute;n intervienen las masas patrimoniales.
        SQLQuery.sprintf ( "UPDATE compbalance SET idmpatrimonial = %s WHERE idcompbalance = %s\n",
                           mainCompany() ->sanearCadena ( m_identmasasp[m_tvalores["idmpatrimonial"]] ).toAscii().constData(),
                           mainCompany() ->sanearCadena ( m_tvalores["idcompbalance"] ).toAscii().constData() );
        mainCompany() ->begin();
        mainCompany() ->runQuery ( SQLQuery );
        mainCompany() ->commit();
        SQLQuery.sprintf ( "UPDATE compbalance SET orden = %s WHERE idcompbalance = %s\n",
                           mainCompany() ->sanearCadena ( m_tvalores["orden"] ).toAscii().constData(),
                           mainCompany() ->sanearCadena ( m_tvalores["idcompbalance"] ).toAscii().constData() );
        mainCompany() ->begin();
        mainCompany() ->runQuery ( SQLQuery );
        mainCompany() ->commit();
        SQLQuery.sprintf ( "UPDATE compbalance SET concepto = '%s' WHERE idcompbalance = %s\n",
                           mainCompany() ->sanearCadena ( m_tvalores["concepto"] ).toAscii().constData(),
                           mainCompany() ->sanearCadena ( m_tvalores["idcompbalance"] ).toAscii().constData() );
        mainCompany() ->begin();
        mainCompany() ->runQuery ( SQLQuery );
        mainCompany() ->commit();
        SQLQuery.sprintf ( "UPDATE compbalance SET tabulacion = %s WHERE idcompbalance = %s\n",
                           mainCompany() ->sanearCadena ( m_tvalores["tabulacion"] ).toAscii().constData(),
                           mainCompany() ->sanearCadena ( m_tvalores["idcompbalance"] ).toAscii().constData() );
        mainCompany() ->begin();
        mainCompany() ->runQuery ( SQLQuery );
        mainCompany() ->commit();
        m_tvalores["codigo"] = "";
        m_tvalores["idmpatrimonial"] = "";
    } // end if
    m_tag = "";
    m_data = "";
    _depura ( "END ImportBalance::endElement2", 0 );
    return TRUE;
}


/// Ha ocurrido una inserci&oacute;n de valor en un tag y almacenamos el valor de la
/// tupla (tag, valor) en el mapa m_tvalores.
/** Para que pueda ser utilizado posteriormente
    \param ch El valor del tag abierto.
    \return TRUE porque no nos interesa abortar la ejecucion del algoritmo. */
bool ImportBalance::characters ( const QString& ch )
{
    _depura ( "ImportBalance::characters", 0 );
    if ( m_tag != "" ) {
        m_data = ch;
        m_tvalores[m_tag] = m_data;
    } // end if
    _depura ( "END ImportBalance::characters", 0 );
    return TRUE;
}

