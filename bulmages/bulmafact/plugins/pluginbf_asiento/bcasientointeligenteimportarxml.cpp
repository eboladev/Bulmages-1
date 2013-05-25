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

#include <QtCore/QString>

#include "bcasientointeligenteimportarxml.h"
#include "bfcompany.h"
#include "blfunctions.h"

/// Constructor de la clase que inicializa las variables y coge los datos de base de
/// datos a usar y empresa.
/** \param emp Empresa que construye el objeto y que sirve para inicializacion de la
    base de datos.
    Contiene la implementacion de la clase BcAsientoInteligenteImportarXML que sirve para importar
    ficheros de XML a la base de datos de BulmaCont. */
BcAsientoInteligenteImportarXML::BcAsientoInteligenteImportarXML ( BfCompany *emp ) : QXmlDefaultHandler(), BlMainCompanyPointer ( emp )
{
    BL_FUNC_DEBUG
    tag = "";
    data = "";
    
}


/// Se ha encontrado en el parser del XML un tag de inicio.
/** La acutaci&oacute;n normal es hacer una inserci&oacute;n del elemento e introducir
    en el Mapa tvalores el identificador encontrado para que al encontrar el tag de fin
    se haga un update del registro con los valores hayados en el XML.
    Se asigna a la variable global tag el nombre del tag para indicar que estamos
    dentro de dicho tag.
    \todo deber&iacute;a usarse una pila en lugar de una simple variable.
    \param qName Nombre del tag encontrado.
    \returns Devuelve true porque no queremos que se detenga la ejecuci&oacute;n del
    parseo aun habiendo encontrado errores. */
bool BcAsientoInteligenteImportarXML::startElement ( const QString&, const QString&, const QString& qName, const QXmlAttributes& )
{
    BL_FUNC_DEBUG
    tag = qName;
    QString SQLQuery;
    if ( tag == "ainteligente" ) {
        SQLQuery.sprintf ( "INSERT INTO ainteligente (descripcion) VALUES ('Elemento importado')\n" );
        mainCompany() ->begin();
        mainCompany() ->runQuery ( SQLQuery );
        SQLQuery = "SELECT max(idainteligente) AS idainteligente FROM ainteligente";
        BlDbRecordSet *cur = mainCompany() ->loadQuery ( SQLQuery, "unquerymas" );
        mainCompany() ->commit();
        if ( !cur->eof() ) {
            tvalores["idainteligente"] = cur->value( "idainteligente" );
        } // end if
        delete cur;
    } // end if
    if ( tag == "binteligente" ) {
        SQLQuery.sprintf ( "INSERT INTO binteligente (idainteligente) VALUES (%s)\n", mainCompany() ->sanearCadena ( tvalores["idainteligente"] ).toLatin1().constData() );
        mainCompany() ->begin();
        mainCompany() ->runQuery ( SQLQuery );
        SQLQuery = "SELECT max(idbinteligente) AS idbinteligente FROM binteligente";
        BlDbRecordSet *cur = mainCompany() ->loadQuery ( SQLQuery, "unquerymas" );
        mainCompany() ->commit();
        if ( !cur->eof() ) {
            tvalores["idbinteligente"] = cur->value( "idbinteligente" );
        } // end if
        delete cur;
    } // end if
    
    return true;
}


/// Se ha encontrado en el parseo del XML un tag de finalizaci&oacute;n.
/** La funci&oacute;n identificar&aacute; mediante el par&aacute;metro \ref qName
    que tag ha finalizado y har&aacute; los UPDATES necesarios en la
    base de datos con los valores almacenados en el mapa \ref tvalores.
    La variable global tag es puesta a "" para que no se hagan consideraciones
    en los demas elementos de que se est&aacute; en el tag que acaba de cerrarse.
    \todo Se deber&iacute;a vaciar el Mapa \ref tvalores para que no haya propagaci&oacute;n
    de valores en caso de ausencia de elementos en el XML.
    \todo Deber&iacute;a usarse una pila en lugar de la variable global tag.
    \todo Deberian agruparse los UPDATES de la base de datos para que ocupen menos
    tiempo de proceso.
    \param qName Nombre del tag del que se ha encontrado el final.
  */
bool BcAsientoInteligenteImportarXML::endElement ( const QString&, const QString&, const QString& qName )
{
    BL_FUNC_DEBUG
    QString SQLQuery;
    if ( qName == "ainteligente" ) {
        SQLQuery.sprintf ( "UPDATE ainteligente SET descripcion = '%s' WHERE idainteligente = %s\n", tvalores["descripcion"].toLatin1().constData(), tvalores["idainteligente"].toLatin1().constData() );
        mainCompany() ->begin();
        mainCompany() ->runQuery ( SQLQuery );
        mainCompany() ->commit();
        SQLQuery.sprintf ( "UPDATE ainteligente SET comentariosasiento = '%s' WHERE idainteligente = %s\n", tvalores["comentariosasiento"].toLatin1().constData(), tvalores["idainteligente"].toLatin1().constData() );
        mainCompany() ->begin();
        mainCompany() ->runQuery ( SQLQuery );
        mainCompany() ->commit();
    } // end if
    if ( qName == "binteligente" ) {
        SQLQuery.sprintf ( "UPDATE binteligente SET fecha = '%s' WHERE idbinteligente = %s\n", tvalores["fecha"].toLatin1().constData(), tvalores["idbinteligente"].toLatin1().constData() );
        mainCompany() ->begin();
        mainCompany() ->runQuery ( SQLQuery );
        mainCompany() ->commit();
        SQLQuery.sprintf ( "UPDATE binteligente SET conceptocontable = '%s' WHERE idbinteligente=%s\n", tvalores["conceptocontable"].toLatin1().constData(), tvalores["idbinteligente"].toLatin1().constData() );
        mainCompany() ->begin();
        mainCompany() ->runQuery ( SQLQuery );
        mainCompany() ->commit();
        SQLQuery.sprintf ( "UPDATE binteligente SET codcuenta = '%s' WHERE idbinteligente = %s\n", tvalores["codcuenta"].toLatin1().constData(), tvalores["idbinteligente"].toLatin1().constData() );
        mainCompany() ->begin();
        mainCompany() ->runQuery ( SQLQuery );
        mainCompany() ->commit();
        SQLQuery.sprintf ( "UPDATE binteligente SET descripcion = '%s' WHERE idbinteligente = %s\n", tvalores["descripcionb"].toLatin1().constData(), tvalores["idbinteligente"].toLatin1().constData() );
        mainCompany() ->begin();
        mainCompany() ->runQuery ( SQLQuery );
        mainCompany() ->commit();
        SQLQuery.sprintf ( "UPDATE binteligente SET debe = '%s' WHERE idbinteligente = %s\n", tvalores["debe"].toLatin1().constData(), tvalores["idbinteligente"].toLatin1().constData() );
        mainCompany() ->begin();
        mainCompany() ->runQuery ( SQLQuery );
        mainCompany() ->commit();
        SQLQuery.sprintf ( "UPDATE binteligente SET haber = '%s' WHERE idbinteligente = %s\n", tvalores["haber"].toLatin1().constData(), tvalores["idbinteligente"].toLatin1().constData() );
        mainCompany() ->begin();
        mainCompany() ->runQuery ( SQLQuery );
        mainCompany() ->commit();
        SQLQuery.sprintf ( "UPDATE binteligente SET contrapartida = '%s' WHERE idbinteligente = %s\n", tvalores["contrapartida"].toLatin1().constData(), tvalores["idbinteligente"].toLatin1().constData() );
        mainCompany() ->begin();
        mainCompany() ->runQuery ( SQLQuery );
        mainCompany() ->commit();
        SQLQuery.sprintf ( "UPDATE binteligente SET comentario = '%s' WHERE idbinteligente = %s\n", tvalores["comentario"].toLatin1().constData(), tvalores["idbinteligente"].toLatin1().constData() );
        mainCompany() ->begin();
        mainCompany() ->runQuery ( SQLQuery );
        mainCompany() ->commit();
        SQLQuery.sprintf ( "UPDATE binteligente SET canal = '%s' WHERE idbinteligente = %s\n", tvalores["canal"].toLatin1().constData(), tvalores["idbinteligente"].toLatin1().constData() );
        mainCompany() ->begin();
        mainCompany() ->runQuery ( SQLQuery );
        mainCompany() ->commit();
        SQLQuery.sprintf ( "UPDATE binteligente SET idc_coste = '%s' WHERE idbinteligente = %s\n", tvalores["idc_coste"].toLatin1().constData(), tvalores["idbinteligente"].toLatin1().constData() );
        mainCompany() ->begin();
        mainCompany() ->runQuery ( SQLQuery );
        mainCompany() ->commit();
        SQLQuery.sprintf ( "UPDATE binteligente SET marcaconciliacion = '%s' WHERE idbinteligente = %s\n", tvalores["marcaconciliacion"].toLatin1().constData(), tvalores["idbinteligente"].toLatin1().constData() );
        mainCompany() ->begin();
        mainCompany() ->runQuery ( SQLQuery );
        mainCompany() ->commit();
    } // end if
    tag = "";
    data = "";
    
    return true;
}


/// Se ha encontrado informaci&oacute;n de contenido en el XML.
/** La funci&oacute;n almacena en el mapa de clase \ref tvalores la informaci&oacute;n
    contenida asignandolo al ltimo tag que se ha abierto. */
/**
\param ch
\return
**/
bool BcAsientoInteligenteImportarXML::characters ( const QString& ch )
{
    BL_FUNC_DEBUG
    if ( tag != "" ) {
        data = ch;
        tvalores[tag] = data;
    } // end if
    
    return true;
}

