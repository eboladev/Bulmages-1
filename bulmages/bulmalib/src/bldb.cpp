/***************************************************************************
 *   Copyright (C) 2003 by Tomeu Borras Riera                              *
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

#include <QFile>
#include <QTextStream>
#include <QLocale>
#include <QDebug>
#include <QTextCodec>
#include <QXmlStreamReader>
#include <QDomDocument>
#include <QDomNode>

#include "bldb.h"
#include "blplugins.h"
#include "local_BlI18n.h"

///
/**
**/
bool BlDbField::cambiado()
{
    blDebug ( "BlDbField::cambiado", 0 );
    blDebug ( "END BlDbField::cambiado", 0 );
    return m_valorcampo != m_valorcampoorig;
}


///
/**
**/
void BlDbField::resetCambio()
{
    blDebug ( "BlDbField::resetCambio", 0 );
    m_valorcampoorig = m_valorcampo;
    blDebug ( "END BlDbField::resetCambio", 0 );
}


///
/**
**/
BlDbField::~BlDbField()
{
    blDebug ( "BlDbField::~BlDbField", 0 );
    blDebug ( "END BlDbField::~BlDbField", 0 );
}


///
/**
\return
**/
BlPostgreSqlClient *BlDbField::dbConnection()
{
    blDebug ( "BlDbField::dbConnection", 0 );
    blDebug ( "END BlDbField::dbConnection", 0 );
    return m_dbConnection;
}


///
/**
\param comp
**/
void BlDbField::setDbConnection ( BlPostgreSqlClient *comp )
{
    blDebug ( "BlDbField::setDbConnection", 0 );
    m_dbConnection = comp;
    blDebug ( "END BlDbField::setDbConnection", 0 );
}


///
/**
\return
**/
BlDbField::DbType BlDbField::dbFieldType()
{
    blDebug ( "BlDbField::tipo", 0 );
    blDebug ( "END BlDbField::tipo", 0 );
    return m_tipo;
}


///
/**
\param val
\return
**/
int BlDbField::set ( QString val )
{
    blDebug ( "BlDbField::set", 0 );
    m_valorcampo = val;
    blDebug ( "END BlDbField::set", 0 );
    return 0;
}


///
/**
\return
**/
int BlDbField::restrictcampo()
{
    blDebug ( "BlDbField::restrictcampo", 0 );
    blDebug ( "END BlDbField::restrictcampo", 0 );
    return m_restrict;
}


///
/**
\return
**/
QString BlDbField::nomcampo()
{
    blDebug ( "BlDbField::nomcampo", 0 );
    blDebug ( "END BlDbField::nomcampo", 0 );
    return m_nomcampo;
}


///
/**
\return
**/
QString BlDbField::nompresentacion()
{
    blDebug ( "BlDbField::nompresentacion", 0 );
    blDebug ( "END BlDbField::nompresentacion", 0 );
    return m_nompresentacion;
}


///
/**
\return
**/
QString BlDbField::valorcampo()
{
    blDebug ( "BlDbField::valorcampo", 0 );
    QString valor = m_valorcampo;
    if ( m_tipo == DbNumeric ) {
        QLocale locale;
        if ( locale.decimalPoint() == '.' ) {
            valor.replace ( ",", locale.decimalPoint () );
        } else {
            valor.replace ( ".", locale.decimalPoint () );
        } // end if
    } // end if
    blDebug ( "END BlDbField::valorcampo", 0 );
    return valor;
}


///
/**
\param com
\param nom
\param typ
\param res
\param nomp
**/
BlDbField::BlDbField ( BlPostgreSqlClient *com, QString nom, DbType typ, int res, QString nomp )
{
    blDebug ( "BlDbField::BlDbField", 0 );
    setDbConnection(com);
    m_nomcampo = nom;
    m_tipo = typ;
    m_restrict = res;
    m_nompresentacion = nomp;
    m_valorcampo = "";
    m_valorcampoorig = "";
    blDebug ( "END BlDbField::BlDbField", 0 );
}


/// Devuelve el valor del campo preparado para ser insertado en la base de datos.
/**
\param error
\return
**/
QString BlDbField::valorcampoprep ( int &error )
{
    blDebug ( "BlDbField::valorcampoprep", 0 );
    error = 0;
    QString valor = "";
    if ( ( m_restrict & DbNotNull ) && ! ( m_restrict & DbAuto ) ) {
        if ( m_valorcampo == "" ) {
            blMsgWarning ( "El campo '" + m_nompresentacion + "' no puede estar vacio." );
            error = -20200;
            blDebug ( "END BlDbField::valorcampoprep", 0, m_nomcampo + " " + m_valorcampo + "-->" + valor );
            return valor;
        } // end if
    } // end if

    switch ( m_tipo ) {
    case DbInt:
        if ( m_valorcampo == "" ) {
            valor = "NULL";
        } else {
            m_valorcampo.replace ( ",", "." );
            valor = "'" + m_dbConnection->sanearCadena ( m_valorcampo ) + "'";
        } // end if
        break;
    case DbVarChar:
        if ( m_valorcampo == "" ) {
            valor = "NULL";
        } else {
            valor = "'" + m_dbConnection->sanearCadena ( m_valorcampo ) + "'";
        } // end if
        break;
    case DbDate:
        if ( m_valorcampo == "" ) {
            valor = "NULL";
        } else {
            valor = "'" + m_dbConnection->sanearCadena ( m_valorcampo ) + "'";
        } // end if
        break;
    case DbNumeric:
        if ( m_valorcampo == "" ) {
            valor = "NULL";
        } else {
            m_valorcampo.replace ( ",", "." );
            valor =  "'" + m_dbConnection->sanearCadena ( m_valorcampo ) + "'";
        } // end if
        break;
    case DbBoolean:
        if ( m_valorcampo == "" ) {
            valor = "NULL";
        } else if ( m_valorcampo == "f" || m_valorcampo == "t" ) {
            valor = "'" + m_dbConnection->sanearCadena ( m_valorcampo ) + "'";
        } else {
            valor = m_dbConnection->sanearCadena ( m_valorcampo );
        } // end if
        break;
    default:
        error = -1;
    } // end switch

    blDebug ( "END BlDbField::valorcampoprep", 0, m_nomcampo + " " + m_valorcampo + "-->" + valor );
    return valor;
}


/// Hace la exportacion del campo a XML
QString BlDbField::exportXML() {
    blDebug ( "BlDbField::exportXML", 0 );
    QString val;
    int error;

    val = "<BLDBFIELD>\n";
    val += "\t<NOMCAMPO>" + blXMLEncode(m_nomcampo) + "</NOMCAMPO>\n";
    val += "\t<VALORCAMPO>"+ blXMLEncode(m_valorcampo)+"</VALORCAMPO>\n";
    val += "\t<VALORCAMPOORIG>"+ blXMLEncode(m_valorcampoorig)+"</VALORCAMPOORIG>\n";
    if (m_valorcampo != "") {
        val += "\t<VALORCAMPOPREP>"+ blXMLEncode(valorcampoprep(error))+"</VALORCAMPOPREP>\n";
    } // end if
    val += "\t<NOMPRESENTACION>"+ blXMLEncode(m_nompresentacion)+"</NOMPRESENTACION>\n";
    val += "\t<RESTRICCIONES>"+ blXMLEncode(QString::number(m_restrict))+"</RESTRICCIONES>\n";
    val += "\t<TIPO>"+ blXMLEncode(QString::number(m_tipo))+"</TIPO>\n";
    val += "</BLDBFIELD>\n";
    return val;
    blDebug ( "END BlDbField::exportXML", 0 );
}





void BlDbField::syncXML(const QString &text) {
    blDebug ( "BlDbField::syncXML", 0 );

    QDomDocument doc ( "mydocument" );

    if ( !doc.setContent ( text ) ) {
        blDebug ( "END BlDbField::syncXML", 0, "XML Invalido" );
        return;
    } // end if

    QDomElement docElem = doc.documentElement();
    QDomElement principal = docElem.firstChildElement ( "NOMCAMPO" );
    if (principal.text() == m_nomcampo) {
        principal = docElem.firstChildElement ( "VALORCAMPO" );
        m_valorcampo = principal.text();
    } // end if


    blDebug ( "BlDbField::syncXML", 0 );
}


/// Constructor de la clase que toma por defecto la base de datos con la que va a operar.
/**
\param con Puntero a la base de datos con la que vamos a trabajar.
**/
BlDbRecord::BlDbRecord ( BlMainCompany *con )
{
    blDebug ( "BlDbRecord::BlDbRecord", 0 );
    m_dbConnection = con;
    m_nuevoCampo = TRUE;
    m_tablename="";
    blDebug ( "END BlDbRecord::BlDbRecord", 0 );
}


/// Destructor de la clase
/**
Limpia y libera toda la memoria que se haya podido reservar.
Esta operacion no contempla que haya habido cambios en el registro sin guardar.
**/
BlDbRecord::~BlDbRecord()
{
    blDebug ( "BlDbRecord::~BlDbRecord", 0 );
    m_lista.clear();
    blDebug ( "END BlDbRecord::~BlDbRecord", 0 );
}

/// Establece la base de datos que debe utilizar la clase.
/**
Esta clase funciona siempre ligada a una base de datos. Esta se especifica
en el constructor de la clase.
\param comp Puntero a la base de datos que debe utilizar la clase.
**/
void BlDbRecord::setDbConnection ( BlMainCompany *comp )
{
    blDebug ( "BlDbRecord::setDbConnection", 0 );
    m_dbConnection = comp;
    blDebug ( "END BlDbRecord::setDbConnection", 0 );
}


/// Devuelve el puntero de la base de datos con la que ha sido inicializada la clase.
/**
\return Puntero a la base de datos con la que se ha inicializado la clase.
**/
BlMainCompany *BlDbRecord::dbConnection()
{
    blDebug ( "BlDbRecord::dbConnection", 0 );
    blDebug ( "END BlDbRecord::dbConnection", 0 );
    return m_dbConnection;
}


/// Establece cual es la tabla por defecto que se va a usar para hacer las operaciones.
/**
Esta clase funciona siempre con una tabla especificada. De esta forma todas las cargas, inserciones
y modificaciones se hacen sobre la tabla que se haya especificado.
\param nom Nombre de la tabla de la base de datos que se usa por defecto.
**/
void BlDbRecord::setDbTableName ( QString nom )
{
    blDebug ( "BlDbRecord::setDbTableName", 0 );
    m_tablename = nom;
    blDebug ( "END BlDbRecord::setDbTableName", 0 );
}


/// Establece el tipo de operacion que se realizara en la base de datos.
/**
Si se establece TRUE entonces se hara una insercion del registro en la tabla por
defecto. Si por el contrario se establece como FALSE se hara una modificacion de los
registros que coincidan en sus PrimaryKey con los datos del registro.

Este metodo es de uso interno. Es la propia clase la que regula si es una insercion
o no dependiendo de si se hecho una carga previa del registro.
\param n Registro para insertar o para modificar.
**/
void BlDbRecord::setNuevo ( bool n )
{
    blDebug ( "BlDbRecord::setNuevo", 0 );
    m_nuevoCampo = n;
    blDebug ( "END BlDbRecord::setNuevo", 0 );
}


/// Devuelve el nombre de la tabla de la base de datos con la que se opera por defecto
/**
\return El nombre de la tabla que usa la clase al  operar con la base de datos
**/
QString BlDbRecord::tableName()
{
    blDebug ( "BlDbRecord::tableName", 0 );
    blDebug ( "END BlDbRecord::tableName", 0 );
    return m_tablename;
}


/// Devuelve el nombre del campo clave primaria en la tabla por defecto.
/**
\return El nombre del campo que se considera Identificador. (Clave Primaria)
**/
QString BlDbRecord::fieldId()
{
    blDebug ( "BlDbRecord::fieldId", 0 );
    if (m_campoid.isEmpty()) {
      if ( exists ( "id" + m_tablename ) ) {
	  m_campoid = "id" + m_tablename;
      } // end if
    } // end if
    if (m_campoid.isEmpty() ) {

       if(exists ( "num" + m_tablename )) {
	  m_campoid = "num" + m_tablename;
	} // end if
    } // end if
    blDebug ( "END BlDbRecord::fieldId", 0, m_campoid );
    return m_campoid;
}


/// Establece el nombre del campo identificador en la tabla por defecto.
/**
\param nom El nombre del campo que se considera la clave primaria
**/
void BlDbRecord::setDbFieldId ( QString nom )
{
    blDebug ( "BlDbRecord::setDbFieldId", 0 );
    m_campoid = nom;
    blDebug ( "END BlDbRecord::setDbFieldId", 0 );
}


/// Devuelve la lista de campos con los que esta operando la clase.
/**
Normalmente coincide esta lista con la definicion de la tabla con la que opera.
\return Lista de campos con los que se esta operando.
**/
QList<BlDbField *> *BlDbRecord::lista()
{
    blDebug ( "BlDbRecord::lista", 0 );
    blDebug ( "END BlDbRecord::lista", 0 );
    return &m_lista;
}


/// Vacia la clase para que no tenga parametros por defecto.
/**
**/
void BlDbRecord::vaciar()
{
    blDebug ( "BlDbRecord::vaciar", 0 );
    DBclear();
    blDebug ( "END BlDbRecord::vaciar", 0 );
}



/// Efectura la carga de un Registro de la base de datos a partir de un recorset (\class BlDbRecordSet).
/**
\param cur recordset con resultados.
\return Si no hay errores devuelve 0. En caso contrario devuelve otro valor.
**/
int BlDbRecord::DBload ( BlDbRecordSet *cur )
{
    blDebug ( "BlDbRecord::DBload", 0 );
    try {
        m_nuevoCampo = FALSE;

        BlDbField *campo;
        int error = 0;
        for ( int i = 0; i < m_lista.size(); ++i ) {
            campo = m_lista.at ( i );
            if ( ! ( campo->restrictcampo() & BlDbField::DbNoLoad ) ) {
                QString nom = campo->nomcampo();
                QString val = cur->valor ( nom );
                if ( ( campo->restrictcampo() & BlDbField::DbPrimaryKey ) && ( val == "" ) )
                    m_nuevoCampo = TRUE;
                if ( ( campo->restrictcampo() & BlDbField::DbDupPrimaryKey ) && ( val == "" ) )
                    m_nuevoCampo = TRUE;
                error += campo->set ( val );
                /// Al ser una carga consideramos que los cambios estan inicializados.
                campo->resetCambio();
            } // end if
        } // end for
        blDebug ( "END BlDbRecord::DBload", 0 );
        return error;
    } catch ( ... ) {
        blDebug ( "EXCEPTION BlDbRecord::DBload", 0 );
        return -1;
    } // end try
}


/// Inicializa la clase limpiando posibles componentes utilizados con anterioridad.
/**
**/
void BlDbRecord::DBclear()
{
    blDebug ( "BlDbRecord::DBclear", 0 );
    m_nuevoCampo = TRUE;
    BlDbField *campo;
    for ( int i = 0; i < m_lista.size(); ++i ) {
        campo = m_lista.at ( i );
        campo->set ( "" );
    } // end for
    blDebug ( "END BlDbRecord::DBclear", 0 );
}


/// Guarda el registro actual contenido en la clase en la base de datos.
/**
\param id Devuelve el identificador (primary key) con que ha quedado guardado el registro.
**/
int BlDbRecord::DBsave ( QString &id )
{
    blDebug ( "BlDbRecord::DBsave", 0, id );
    try {
        BlDbField *campo;
        QString listcampos = "";
        QString listvalores = "";
        QString queryupdate = "";
        QString separador = "";
        QString separador1 = "";
        QString separadorwhere = "";
        QString querywhere = "";
        int err = 0;
        for ( int i = 0; i < m_lista.size(); ++i ) {
            campo = m_lista.at ( i );

            /// Si el campo esta en DupPrimaryKey lo ponemos en el query.
            if ( campo->restrictcampo() & BlDbField::DbDupPrimaryKey ) {
                QString lin = campo->valorcampoprep ( err );
                if ( err )
                    throw ( err );
                querywhere += separadorwhere + campo->nompresentacion() + " = " + lin;
                separadorwhere = " AND ";
            } // end if

            /// Si el campo es requerido y no esta entonces salimos sin dar error.
            /// No es lo mismo que los not null ya que estos si dan error
            if ( ! ( campo->restrictcampo() & BlDbField::DbNoSave ) ) {
                if ( campo->restrictcampo() & BlDbField::DbRequired ) {
                    if ( campo->valorcampo() == "" ) {
                        blDebug ( "END BlDbRecord::DBsave", 0, "Campo requerido vacio" );
                        return 0;
                    } // end if
                } // end if
                if ( campo->restrictcampo() & BlDbField::DbPrimaryKey ) {
                    QString lin = campo->valorcampoprep ( err );
                    if ( err )
                        throw ( err );
                    querywhere += separadorwhere + campo->nomcampo() + " = " + lin;
                    separadorwhere = " AND ";
                } // end if
                if ( campo->valorcampoprep ( err ) != "" ) {
                    queryupdate += separador1 + campo->nomcampo() + "=" + campo->valorcampoprep ( err );
                    separador1 = ", ";
                } // end if
                if ( err )
                    throw ( err );
                if ( ( campo->valorcampoprep ( err ) != "NULL" ) && ( campo->valorcampoprep ( err ) != "" ) ) {
                    listcampos += separador + campo->nomcampo();
                    listvalores += separador + campo->valorcampoprep ( err );
                    if ( err )
                        throw ( err );
                    separador = ", ";
                } // end if
                /// Si es el ID entonces lo asignamos porque ya tiene el valor correspondiente.
                if ( m_campoid == campo->nomcampo() ) {
                    id = campo->valorcampo();
                } // end if
            } // end if
        } // end for
        if ( m_nuevoCampo ) {
            QString query = "INSERT INTO " + m_tablename + " (" + listcampos + ") VALUES (" + listvalores + ")";
            m_dbConnection->runQuery ( query );
            blDebug ( query, 0 );
            BlDbRecordSet *cur = m_dbConnection->loadQuery ( "SELECT " + m_campoid + " FROM " + m_tablename + " ORDER BY " + m_campoid + " DESC LIMIT 1" );
            id = cur->valor ( m_campoid );
            delete cur;
        } else {
            QString query = "UPDATE " + m_tablename + " SET " + queryupdate + " WHERE " + querywhere;
            blDebug ( query, 0 );
            m_dbConnection->runQuery ( query );
        } // end if
        m_nuevoCampo = FALSE;

        /// Emitimos la se&ntilde;al apropiada en el BlApplication.
        g_theApp->emitDbTableChanged ( m_tablename );

    } catch ( int error ) {
        blDebug ( "END BlDbRecord::DBsave", 0, "Error de guardado" );
        throw error;
    } // end try
    blDebug ( "END BlDbRecord::DBSave", 0 );
    return 0;
}


/// Establece un nuevo valor para un campo del registro con el que estamos tratando.
/**
\param nomb Nombre del campo cuyo valor vamos a establecer.
\param valor Valor que toma el campo.
\return
**/
int BlDbRecord::setDbValue ( QString nomb, QString valor )
{
    blDebug ( "BlDbRecord::setDbValue", 0 );
    BlDbField *campo;
    int error = 0;
    int i = 0;
    campo = m_lista.value ( i );

    while ( campo && campo->nomcampo() != nomb ) {
        campo = m_lista.value ( ++i ) ;
    } // end while

    if ( !campo ) {
        blDebug ( "Campo " + nomb + " no encontrado", 2 );
        return -1;
    } // end if

    if ( campo->nomcampo() == nomb ) {
        error = campo->set ( valor );
    } // end if

    blDebug ( "END BlDbRecord::setDbValue", 0 );
    return error;
}


/// Consulta el valor que tiene un campo del registro actual
/**
\param nomb Nombre del campo que queremos consultar.
\return Valor que contiene el campo.
**/
QString BlDbRecord::dbValue ( QString nomb )
{
    blDebug ( "BlDbRecord::dbValue", 0, nomb );
    BlDbField *campo;
    QString valor = "";
    int i = 0;

    /// Recorremos la lista en busqueda del campo.
    campo = m_lista.value ( i );
    while ( campo && campo->nomcampo() != nomb )
        campo = m_lista.value ( ++i );

    if ( !campo ) {
        blDebug ( "Campo " + nomb + " no encontrado", 2 );
    } else if ( campo->nomcampo() == nomb ) {
        valor = campo->valorcampo();
    } // end if
    blDebug ( "END BlDbRecord::dbValue", 0, nomb );
    return valor;
}


/// Indica si un campo esta inicializado en el registro o no.
/**
\param nomb Campo del que queremos saber si existe o no
\return TRUE si existe o FALSE si no existe
**/
bool BlDbRecord::exists ( QString nomb )
{
    blDebug ( "BlDbRecord::exists", 0, nomb );
    BlDbField *campo;
    bool existe = FALSE;
    int i = 0;
    campo = m_lista.value ( i );
    while ( campo && campo->nomcampo() != nomb )
        campo = m_lista.value ( ++i );
    if ( campo ) {
        if ( campo->nomcampo() == nomb ) {
            existe = TRUE;
        } // end if
    } // end if
    blDebug ( "END BlDbRecord::exists", 0, nomb );
    return existe;
}


/// Prepara un campo para un eventual almacenado en la base de datos.
/**
\param nomb campo a preparar para ser guardado
\return El valor del campo una vez preprocesado para el guardado.
**/
QString BlDbRecord::dbValueprep ( QString nomb )
{
    blDebug ( "BlDbRecord::dbValueprep", 0 );
    BlDbField *campo;
    int i = 0;
    campo = m_lista.value ( i );

    while ( campo && campo->nomcampo() != nomb ) {
        campo = m_lista.value ( ++i );
    } // end while

    if ( !campo ) {
        blMsgWarning ( "No se ha encontrado el campo '" + nomb + "'." );
        return "";
    } // end if

    if ( campo->nomcampo() == nomb ) {
        int err;
        return campo->valorcampoprep ( err );
    } // end if

    blDebug ( "END BlDbRecord::dbValueprep", 0 );
    return "";
}


/// Agrega un nuevo campo a la definicion del registro actual
/**
Al iniciar la clase no tiene ninguna definicion de la tabla con la que va a operar ni
de los campos que contiene dicha tabla. Mediante este metodo podemos inicialiar
la clase para que aprenda a operar con una tabla determinada.
\param nom Nombre del campo
\param typ Tipo del campo
\param res Restricciones a considerar en el campo
\param nomp Nombre para mostrar en los mensajes que involucren al campo
\return
**/
int BlDbRecord::addDbField ( QString nom, BlDbField::DbType typ, int res, QString nomp = "" )
{
    blDebug ( "BlDbRecord::addDbField", 0 );

    try {

        BlDbField *camp = new BlDbField ( m_dbConnection, nom, typ, res, nomp );
        camp->set
        ( "" );
        m_lista.append ( camp );

        blDebug ( "END BlDbRecord::addDbField", 0 );
        return 0;


    } catch(...) {
        blDebug("Error en la inserción del campo al registro");
    } // end try




}


/// Elimina el registro que tenemos de la base de dato (Si es que existe)
/**
\return si no se han producido errores devuelve 0 en caso contrario devuelve -1
**/
int BlDbRecord::borrar()
{
    blDebug ( "BlDbRecord::borrar", 0 );
    try {
        BlDbField *campo;
        QString separadorwhere = "";
        QString querywhere = "";

        for ( int i = 0; i < m_lista.size(); ++i ) {
            campo = m_lista.at ( i );
            if ( campo->restrictcampo() & BlDbField::DbDupPrimaryKey ) {
                int err;
                QString lin = campo->valorcampoprep ( err );
                if ( err )
                    throw - 1;
                querywhere += separadorwhere + campo->nompresentacion() + " = " + lin;
                separadorwhere = " AND ";
            } // end if

            if ( ! ( campo->restrictcampo() & BlDbField::DbNoSave ) ) {
                if ( campo->restrictcampo() & BlDbField::DbPrimaryKey ) {
                    int err;
                    QString lin = campo->valorcampoprep ( err );
                    if ( err )
                        throw - 1;
                    querywhere += separadorwhere + campo->nomcampo() + " = " + lin;
                    separadorwhere = " AND ";
                } // end if
            } // end if
        } // end for

        if ( m_nuevoCampo == FALSE ) {
            m_dbConnection->runQuery ( "DELETE FROM " + m_tablename + " WHERE " + querywhere );
        } // end if

        blDebug ( "END BlDbRecord::borrar", 0 );
        return 0;
    } catch ( ... ) {
        blMsgInfo ( "se produjo un error al borrar el elemento" );
        blDebug ( "BlDbRecord::borrar() Error al borrar elemento", 3 );
        throw - 1;
    }
}


/// Guarda el registro actual en la base de datos
/**
Esta funcion, de un nivel algo superior a la llamada DBsave hace el guardado y maneja las
excepciones que se hayan podido producir
\return si no se producen errores devuelve 0 en caso contrario genera una excepcion
**/
int BlDbRecord::guardar()
{
    blDebug ( "BlDbRecord::guardar", 0 );
    QString id;
    try {
        DBsave ( id );
        setDbValue ( m_campoid, id );
        blDebug ( "END BlDbRecord::guardar", 0 );
        return 0;
    } catch ( ... ) {
        blMsgError ( "BlDbRecord:: Se ha producido un error al guardar los datos." );
        throw -100;
    } // end try
}



/// Esta funcion carga un registro..
/**
\param id Identificador del registro a cargar en la tabla por defecto.
\return Si no hay errores devuelve 0 sino devuelve -1
**/
int BlDbRecord::cargar ( QString id )
{
    blDebug ( "BlDbRecord::cargar", 0 );
    try {
        QString query = "SELECT * FROM " + m_tablename + " WHERE " + m_campoid + " = " + id;
        BlDbRecordSet *cur = m_dbConnection->loadQuery ( query );
        if ( !cur ) throw - 1;
        if ( !cur->eof() ) {
            DBload ( cur );
        } // end if
        delete cur;
        blDebug ( "END BlDbRecord::cargar", 0 );
        return 0;
    } catch ( ... ) {
        blMsgError ( "BlDbRecord::cargar Se ha producido un error al cargar el registro." );
        return -1;
    } // end try
}

void BlDbRecord::substrConf ( QString &buff )
{
    ///\TODO: Este tratamiento esta repetido en BlForm::trataTags y en PedidoProveedorView::imprimir.
    ///       Se puede simplificar?
    /// Tratamos la sustitucion de los valores de configuracion.
    for ( int i = 0; i < 1000; i++ ) {
        if ( g_confpr->nombre ( i ) != "" ) {
            buff.replace ( "[" + g_confpr->nombre ( i ) + "]", g_confpr->valor ( i ) );
        } // end if
    } // end for
}

void BlDbRecord::substrConf ( QByteArray &buff )
{
    ///\TODO: Este tratamiento esta repetido en BlForm::trataTags y en PedidoProveedorView::imprimir.
    ///       Se puede simplificar?
    /// Tratamos la sustitucion de los valores de configuracion.
    for ( int i = 0; i < 1000; i++ ) {
        if ( g_confpr->nombre ( i ) != "" ) {
            buff.replace ( QString("[" + g_confpr->nombre ( i ) + "]").toAscii(), g_confpr->valor ( i ).toAscii() );
        } // end if
    } // end for
}

int BlDbRecord::trataTags ( QString &buff, int tipoEscape )
{
    QString fitxersortidatxt = "";

    substrConf ( buff );
    buff.replace ( "[ficha]", m_tablename );
    buff.replace ( "[story]", story() );

    return 1;
}

int BlDbRecord::trataTags ( QByteArray &buff, int tipoEscape )
{
    QString fitxersortidatxt = "";

    substrConf ( buff );
    buff.replace ( "[ficha]", m_tablename.toAscii() );
    buff.replace ( "[story]", story().toAscii() );

    return 1;
}



/// Este metodo es usado en las impresiones con RML para generar una cuadricula con el registro.
QString BlDbRecord::story ( void )
{

    QString fitxersortidatxt = "";
    BlDbField *campo;
    QLocale spanish ( QLocale::Spanish, QLocale::Spain );
    /// Impresion de la tabla de contenidos.
    for ( int i = 0; i < m_lista.size(); ++i ) {
        campo = m_lista.at ( i );
        fitxersortidatxt += "<tr>\n";
        fitxersortidatxt += "   <td>" + blXMLEscape ( campo->nomcampo() ) + "</td>\n";
        fitxersortidatxt += "   <td>" + blXMLEscape ( campo->nompresentacion() ) + "</td>\n";
        if ( campo->dbFieldType() & BlDbField::DbNumeric )
            fitxersortidatxt += "   <td>" + blXMLEscape ( spanish.toString ( campo->valorcampo().toDouble(), 'f', 2 ) ) + "</td>\n";
        else
            fitxersortidatxt += "   <td>" + blXMLEscape ( campo->valorcampo() ) + "</td>\n";
        fitxersortidatxt += "</tr>";
        qDebug() << spanish.toString ( campo->valorcampo().toDouble(), 'f', 2 );
    } // end for

    return fitxersortidatxt;
}

int BlDbRecord::generaRML ( const QString &arch )
{
    blDebug ( "BlDbRecord::generaRML", 0 );

    /// Disparamos los plugins
    int res = g_plugins->lanza ( "BlDbRecord_generaRML", this );
    if ( res != 0 ) {
        return 1;
    } // end if

    /// Dependiendo del tipo de archivo que se trate usaremos un parseo u otro.
    int tipoescape = 0;
    if ( arch.right ( 3 ) == "rml" || arch.right ( 3 ) == "xml" )
        tipoescape = 1;
    else if ( arch.right ( 3 ) == "pys" )
        tipoescape = 2;


    QString archivo = g_confpr->valor ( CONF_DIR_OPENREPORTS ) + arch;
    QString archivod = g_confpr->valor ( CONF_DIR_USER ) + arch;
    QString archivologo = g_confpr->valor ( CONF_DIR_OPENREPORTS ) + "logo.jpg";

    /// Copiamos el archivo.
#ifdef WIN32
    archivo = "copy \"" + archivo + "\" \"" + archivod + "\"";
#else
    archivo = "cp " + archivo + " " + archivod;
#endif

    int result1 = system ( archivo.toAscii().constData() );
    if (result1 == -1) {
	blMsgError(_("Error al copiar el archivo RML [ bldb->generaRML() ]"));
    } // end if
    
    /// Copiamos el logo
#ifdef WIN32
    archivologo = "copy \"" + archivologo + "\" \"" + g_confpr->valor ( CONF_DIR_USER ) + "logo.jpg \"";
#else
    archivologo = "cp " + archivologo + " " + g_confpr->valor ( CONF_DIR_USER ) + "logo.jpg";
#endif

    int result2 = system ( archivologo.toAscii().constData() );
    if (result2 == -1) {
	blMsgError(_("Error al copiar el archivo de logo [ bldb->generaRML() ]"));
    } // end if
    
    QFile file;
    file.setFileName ( archivod );
    file.open ( QIODevice::ReadOnly );
    bool ascii = FALSE;
    /// Antes de abrir un fichero como QTextStream debemos saber en que codificacion
    /// esta, si no leeremos basura (potencialmente). Los ficheros XML deberian
    /// declararlo en la primera instruccion de proceso (<?xml ... encoding=""?>)
    /// Si no lo declaran deberian ser UTF-8. Para parsear la instruccion de proceso
    /// uso QXmlStreamReader. Para el resto del fichero dejo lo que estaba
    /// hecho, que trabaja sobre una QString. Para pasarlo a SAX, DOM o QXmlStreamReader
    /// habria que cambiarlo todo y no se si vale la pena.
    QXmlStreamReader xmlReader ( &file );
    while ( ( !xmlReader.atEnd() ) && ( !xmlReader.isStartDocument() ) ) {
        xmlReader.readNext();
    };
    QTextCodec *codec = NULL;
    if ( xmlReader.isStartDocument() ) {
        blDebug ( "El fitxer " + archivod + " te una codificacio (" +
                  xmlReader.documentEncoding().toString() + ") ", 0 );
        codec = QTextCodec::codecForName ( xmlReader.documentEncoding().toString().toUtf8() );
        if ( !codec ) { // sembla que no va, per UTF32 (=UCS4)
            blDebug ( "El fitxer " + archivod + " te una codificacio (" +
                      xmlReader.documentEncoding().toString() + ") que no entenem", 0 );
//             file.close();
//             return 0;
            codec = QTextCodec::codecForName ( "UTF-8" );
        } else {
            blDebug ( "Usarem un codec per a ", 0, codec->name() );
        }// end if
    } else {
	ascii = xmlReader.hasError();
        if ( ascii ) {
            blDebug ( "El fitxer " + archivod + " no l'hem pogut llegir be i no sabem quina codificacio te. S'imaginarem que es ASCII per si de cas" );
        }// end if
        blDebug ( "El fitxer " + archivod + " no se quina codificacio te. Deu ser UTF-8", 0 );
        codec = QTextCodec::codecForName ( "UTF-8" );
        // esto mantendria compatibilidad con el codigo anterior, pero
        // me parece que no queremos porque el codigo anterior no admitia
        // contenidos no ASCII (bgtrml2pdf petaba) y los contenidos ASCII ya funcionan aunque
        // los trates como UTF-8
        // encoding=QtextCodec::codecForLocale();
    }// end if
    file.close();
    file.open ( QIODevice::ReadOnly );
    QTextStream stream ( &file );
    stream.setCodec ( codec );
if (tipoescape != 0) {
    QString buff = stream.readAll();
    file.close();

    /// Hacemos el tratamiento avanzado de TAGS
    if ( !trataTags ( buff, tipoescape ) ) {
        return 0;
    } // end if

    if ( file.open ( QIODevice::WriteOnly ) ) {
        QTextStream stream ( &file );
        stream.setCodec ( codec );
        if ( ( !ascii ) && ( codec->canEncode ( buff ) ) ) {
            // para ficheros UTF-8, UTF-16, UTF-32 asi vale
            // para otros sin caracteres ajenos al encoding tambien vale
            blDebug ( "Llistat sense referències de caracters", 0, " " );
            stream << buff;
        } else { // para otros con caracteres no codificables
            // tenemos que usar referencias numericas de caracteres de XML
            blDebug ( "Llistat amb referències de caracters", 0, " " );
            QString::const_iterator i;
            for ( i = buff.begin(); i != buff.end(); ++i ) {
                if ( ( codec->canEncode ( *i ) ) && ( ( !ascii ) || ( ( *i ).unicode() < 128 ) ) ) {
                    stream << *i; // si puedo lo escribo
                } else { // si no pogo referencia numerica de caracter &12345;
                    uint codepoint = 0;
                    // el caracter puede no caber en un QChar.
                    if ( ( *i ).isHighSurrogate() ) {
                        // sospecho que o este caso o el siguiente nunca se
                        // dara pero no lo se seguro y si es asi no se cual
                        QChar high = *i;
                        i++;
                        if ( i == buff.end() )
                            break;
                        QChar low = *i;
                        codepoint = QChar::surrogateToUcs4 ( high, low );
                    }  else {
                        if ( ( *i ).isLowSurrogate() ) {
                            // sospecho que o este caso o el anterior nunca se
                            // dara pero no lo se seguro y si es asi no se cual
                            QChar low = *i;
                            i++;
                            if ( i == buff.end() )
                                break;
                            QChar high = *i;
                            codepoint = QChar::surrogateToUcs4 ( high, low );
                        } else {
                            // este caso es mas normal, caracter entre 0 i 2^16
                            codepoint = ( *i ).unicode();
                        } // end if
                    } // end if
                    blDebug ( ( QString ) "escric " + *i + " com " + codepoint, 0 );
                    stream << "&#" << codepoint << ";" ;
                } // end if
            } // end for
            blDebug ( "END Llistat amb referencies de caracters", 0 );
        }
        file.close();
    } // end if

} else {
    QByteArray buff = file.readAll();
    file.close();

    /// Hacemos el tratamiento avanzado de TAGS
    if ( !trataTags ( buff, tipoescape ) ) {
        return 0;
    } // end if

    if ( file.open ( QIODevice::WriteOnly ) ) {
        file.write(buff);
        file.close();
    } // end if
} // end if
    blDebug ( "END BlDbRecord::generaRML", 0 );
    return 1;
}

QString BlDbRecord::nombrePlantilla ( void )
{
    return QString ( "ficha" );
}

int BlDbRecord::generaRML ( void )
{
    return generaRML ( nombrePlantilla() + ".rml" );
}

/// Realiza una impresion generica del registro a partir de la plantilla ficha.rml
/**
**/
void BlDbRecord::imprimir()
{
    /// Usa la plantilla ficha.rml para realizar la impresion.
    blDebug ( "BlDbRecord::imprimir", 0, nombrePlantilla() );

    if ( generaRML() ) {
        blCreateAndLoadPDF ( nombrePlantilla() );
    } // end if

    blDebug ( "END BlDbRecord::imprimir", 0 );
}




/// Hace la exportacion del campo a XML
QString BlDbRecord::exportXML() {
    blDebug ( "BlDbRecord::exportXML", 0 );
    QString val;
    int error;
    BlDbField *campo;

    val = "<BLDBRECORD>\n";
    val += "\t<TABLENAME>" + blXMLEncode(m_tablename) + "</TABLENAME>\n";
    val += "\t<CAMPOID>" + blXMLEncode(m_campoid) + "</CAMPOID>\n";
    val += "\t<NUEVOCAMPO>" + blXMLEncode(QString(m_nuevoCampo ? "TRUE" : "FALSE")) + "</NUEVOCAMPO>\n";

    int i = 0;
    campo = m_lista.value ( i );
    while ( campo ) {
        val += "\t" + campo->exportXML().replace("\t<","\t\t<").replace("\n<","\n\t<");
        campo = m_lista.value ( ++i );
    } // end while

    val += "</BLDBRECORD>\n";

    return val;
    blDebug ( "END BlDbRecord::exportXML", 0 );
}

void BlDbRecord::syncXML(const QString &text) {
    blDebug ( "BlDbRecord::syncXML", 0 );

    BlDbField *campo;
    QDomDocument doc ( "mydocument" );

    if ( !doc.setContent ( text ) ) {
        blDebug ( "END BlDbRecord::syncXML", 0, "XML Invalido" );
        return;
    } // end if


    QDomElement docElem = doc.documentElement();
    QDomElement principal = docElem.firstChildElement ( "TABLENAME" );
    m_tablename = principal.text();
    principal = docElem.firstChildElement ( "CAMPOID" );
    m_campoid = principal.text();
    principal = docElem.firstChildElement ( "NUEVOCAMPO" );
    if (principal.text() == "TRUE") {
        m_nuevoCampo = TRUE;
    } else {
        m_nuevoCampo = FALSE;
    } // end if

    /// Cogemos la coordenada X
    QDomNodeList nodos = docElem.elementsByTagName ( "BLDBFIELD" );
    int j = 0;
    while (j < nodos.count()) {
        QDomNode ventana = nodos.item ( j++ );
        QDomElement e1 = ventana.toElement(); /// try to convert the node to an element.
        if ( !e1.isNull() ) { /// the node was really an element.
            QString nodoText = e1.text();
            /// Pasamos el XML a texto para poder procesarlo como tal.
            QString result;
            QTextStream stream ( &result );
            ventana.save(stream,5);
            /// Para cada uno de los campos sincronizamos.
            int i = 0;
            campo = m_lista.value ( i++ );
            while ( campo ) {
                campo->syncXML(result);
                campo = m_lista.value ( i++ );
            } // end while

        } // end if
    } // end while
    blDebug ( "BlDbRecord::syncXML", 0 );
}


