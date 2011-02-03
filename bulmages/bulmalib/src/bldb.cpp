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
#include <QHBoxLayout>
#include <QObject>
#include <QCheckBox>
#include <QInputDialog>
#include <QUiLoader>
#include <QPushButton>

#include "bldb.h"
#include "blplugins.h"
#include "local_blI18n.h"
#include "blescprinter.h"

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

NOTA: ESTA FUNCION TIENE ALGUN ERROR CON m_lista mal inicializado.
**/
bool BlDbRecord::exists ( QString nomb )
{
    blDebug ( "BlDbRecord::exists", 0, nomb );
    BlDbField *campo;
    bool existe = FALSE;
    int i = 0;
    blDebug ( "BlDbRecord::exists_0", 0, nomb );
    if (!m_lista.isEmpty()) {
      blDebug ( "BlDbRecord::exists_1", 0, nomb );
      campo = m_lista.value ( i );
      blDebug ( "BlDbRecord::exists_2", 0, campo->nomcampo() );
      if (campo) {
	while (i < m_lista.size() && campo && campo->nomcampo() != nomb ) {
	    campo = m_lista.value ( ++i );
	    blDebug ( "BlDbRecord::exists_3", 0, nomb );
	} // end while
	if ( campo ) {
	    if ( campo->nomcampo() == nomb ) {
		existe = TRUE;
	    } // end if
	    blDebug ( "BlDbRecord::exists_4", 0, nomb );
	} // end if
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

/*
int BlDbRecord::trataTags ( QByteArray &buff, int tipoEscape )
{
    QString fitxersortidatxt = "";

    substrConf ( buff );
    buff.replace ( "[ficha]", m_tablename.toAscii() );
    buff.replace ( "[story]", story().toAscii() );

    return 1;
}
*/


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
#ifdef Q_OS_WIN32
    archivo = "copy \"" + archivo + "\" \"" + archivod + "\"";
#else
    archivo = "cp " + archivo + " " + archivod;
#endif

    int result1 = system ( archivo.toAscii().constData() );
    if (result1 == -1) {
	blMsgError(_("Error al copiar el archivo RML [ bldb->generaRML() ]"));
    } // end if
    
    /// Copiamos el logo
#ifdef Q_OS_WIN32
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

// =================== GENERAR RML PARA TICKETS y TXT


/// Busca strings del tipo [xxxx] entro del texto pasado y los sustituye
/// Por valores existentes en la base de datos.
/// Tambien busca los parametros PARAM e IFACE para tambien tratarlos.
/// Devuelve 1 Si todo esta OK o 0 Si hay algun error
/**
\param buff El texto entero sobre el que se hace el reemplazo de sentencias.
**/
int BlDbRecord::trataTags ( QByteArray &buff, int tipoEscape )
{
    blDebug ( "BlDbRecord::trataTags", 0 );


    substrConf ( buff );
    buff.replace ( "[ficha]", m_tablename.toAscii() );
    buff.replace ( "[story]", story().toAscii() );

    
    static int profundidad = 0;
    profundidad ++;
    
    ///Buscamos interfaces, los preguntamos y los ponemos.
    int pos = 0;
    QByteArray cadant;

    QRegExp rx70 ( "<!--\\s*IFACE\\s*SRC\\s*=\\s*\"([^\"]*)\"\\s*-->" );
    rx70.setMinimal ( TRUE );
    while ( ( pos = rx70.indexIn ( buff, pos ) ) != -1 ) {
        QString cadarchivo = rx70.cap ( 1 );

        substrConf ( cadarchivo );

        QFile fichero ( cadarchivo );
        if ( fichero.exists() ) {
            QUiLoader loader;
            fichero.open ( QFile::ReadOnly );
            QWidget *iface = loader.load ( &fichero );
            fichero.close();
            QDialog *diag = new QDialog ( 0 );
            diag->setModal ( true );

            /// Creamos un layout donde estara el contenido de la ventana y la ajustamos al QDialog
            /// para que sea redimensionable y aparezca el titulo de la ventana.
            QHBoxLayout *layout = new QHBoxLayout;
            layout->addWidget ( iface );
            layout->setMargin ( 5 );
            layout->setSpacing ( 5 );
            diag->setLayout ( layout );
            diag->setWindowTitle ( iface->windowTitle() );
            diag->setGeometry ( 0, 0, iface->width(), iface->height() );
            QPushButton *button = iface->findChild<QPushButton *> ( "mui_aceptar" );
            button->connect ( button, SIGNAL ( released ( ) ), diag, SLOT ( accept() ) );
            QPushButton *button1 = iface->findChild<QPushButton *> ( "mui_cancelar" );
            button->connect ( button1, SIGNAL ( released ( ) ), diag, SLOT ( reject() ) );
            int ret = diag->exec();
            if ( ret ) {

         /// Recorre los QLineEdit.
                QList<QLineEdit *> l2 = iface->findChildren<QLineEdit *>();
                QListIterator<QLineEdit *> it2 ( l2 );
                while ( it2.hasNext() ) {
                    QLineEdit * item = it2.next();
                    QString nombre = item->objectName().right ( item->objectName().size() - 4 );
                    QString valor = item->text();
                    addDbField ( nombre, BlDbField::DbVarChar, BlDbField::DbNoSave, nombre  );
                    setDbValue ( nombre, valor );
                } // end while
                
                /// Recorre los QCheckBox.
                QList<QCheckBox *> qcb = iface->findChildren<QCheckBox *>();
                QListIterator<QCheckBox *> itqcb ( qcb );

                while ( itqcb.hasNext() ) {
                    QCheckBox *item = itqcb.next();
                    QString nombre = item->objectName().right ( item->objectName().size() - 4 );
                    QString valor = "false";
                    if ( item->isChecked() ) {
                        valor = "true";
                    } else {
                  valor = "false";
          } // end if
                    addDbField ( nombre, BlDbField::DbBoolean, BlDbField::DbNoSave, nombre  );
                    setDbValue ( nombre, valor );
                } // end while
                
      QVariant exportaRML = iface->property("exportaRML");
      if (exportaRML.isValid() ) {
         QStringList props = exportaRML.toStringList();
      
         QListIterator<QString> iProps(props);
         while (iProps.hasNext()) {
            QString camp = iProps.next();
            QStringList cami = camp.split(".");
            QObject *actual=iface;
            QListIterator<QString> iCami(cami);
            
            while(iCami.hasNext() && actual) {

                QString nom = iCami.next();
                QObject *fill = actual->findChild<QObject *>("mui_"+nom);

                if (!fill) {
               fill = actual->findChild<QObject *>(nom);
                } // end if

                if (fill) {
               actual = fill;
                } else {
                
               QVariant valor = actual->property(nom.toUtf8().data());
               m_variables[camp] =valor.toString();
               
               if (valor.canConvert<QObject*>()) {
                   actual = valor.value<QObject*>();
               } else {
                   actual = NULL;
               } // end if
            
                } // end if
                
            } // end while
            
         } // end while
         
      } // end if

            } // end if
            
            delete diag;
            
            // Si se ha pulsado cancelar entonce se sale del informe.
            if ( !ret ) return 0;
            
        } else {
            blMsgInfo ( "Archivo de Interficie no existe" );
        } // end if

        buff.replace ( pos, rx70.matchedLength(), "" );
        pos = 0;
    } // end while


    /// Buscamos establecimiento de variables y los ponemos en m_variables
    pos = buff.indexOf("<!-- SETVAR");
    cadant = buff.left(pos);
    buff = buff.right(buff.length()-pos);
    QRegExp rx54 ( "<!--\\s*SETVAR\\s*NAME\\s*=\\s*\"([^\"]*)\"\\s*VALUE\\s*=\\s*\"([^\"]*)\"\\s*-->" );
    rx54.setMinimal ( TRUE );
    while ( ( pos = rx54.indexIn ( buff, 0 ) ) != -1 ) {
        QByteArray valor = rx54.cap ( 2 ).toAscii();
        substrVars ( valor, tipoEscape );
        m_variables[rx54.cap ( 1 ) ] = valor;
        buff.replace ( pos, rx54.matchedLength(), "" );
        pos = buff.indexOf("<!--");
    } // end while
    buff = cadant + buff;
    
    /// Tratamos las variables establecidas.
    substrVars ( buff, tipoEscape );


    
/*    
    /// Buscamos algo de lineas de detalle
    /// Inclusion de ficheros de codigo
    pos = buff.indexOf("<!-- LINEAS DETALLE");
    cadant = buff.left(pos);
    buff = buff.right(buff.length()-pos);
    QRegExp rx ( "<!--\\s*LINEAS\\s*DETALLE\\s*-->(.*)<!--\\s*END\\s*LINEAS\\s*DETALLE\\s*-->" );
    rx.setMinimal ( TRUE );
    
    while ( ( pos = rx.indexIn ( buff, 0 ) ) != -1 ) {
        QByteArray ldetalle = trataLineasDetalle ( rx.cap ( 1 ).toAscii(), tipoEscape );
        buff.replace ( pos, rx.matchedLength(), ldetalle );
        pos = buff.indexOf("<!--");
    } // end while    
    buff = cadant + buff;
*/    
    
    /// Inclusion de ficheros de codigo
    pos = buff.indexOf("<!-- INCLUDE");
    cadant = buff.left(pos);
    buff = buff.right(buff.length()-pos);
    QRegExp rx19 ( "<!--\\s*INCLUDE\\s*FILE\\s*=\\s*\"([^\"]*)\"\\s*-->" );
    rx19.setMinimal ( TRUE );
    while ( ( pos = rx19.indexIn ( buff, 0 ) ) != -1 ) {
        QString ldetalle = trataIncludeFileTXT ( rx19.cap ( 1 ), tipoEscape );
        buff.replace ( pos, rx19.matchedLength(), ldetalle.toAscii() );
        pos = buff.indexOf("<!--");
    } // end while
    buff = cadant + buff;

    /// Buscamos Existencia de Ficheros
    pos = buff.indexOf("<!-- EXISTS FILE");
    cadant = buff.left(pos);
    buff = buff.right(buff.length()-pos);
    QRegExp rx9 ( "<!--\\s*EXISTS\\s*FILE\\s*=\\s*\"([^\"]*)\"\\s*-->(.*)<!--\\s*END\\s*EXISTS\\s*-->" );
    rx9.setMinimal ( TRUE );
    while ( ( pos = rx9.indexIn ( buff, 0 ) ) != -1 ) {
        QString ldetalle = trataExists ( rx9.cap ( 1 ), rx9.cap ( 2 ).toAscii() );
        buff.replace ( pos, rx9.matchedLength(), ldetalle.toAscii() );
        pos = buff.indexOf("<!--");
    } // end while
    buff = cadant + buff;
    
    /// Buscamos Query's en condicional
    pos = buff.indexOf("<!-- IF QUERY");
    cadant = buff.left(pos);
    buff = buff.right(buff.length()-pos);
    QRegExp rx4 ( "<!--\\s*IF\\s*QUERY\\s*=\\s*\"([^\"]*)\"\\s*-->(.*)<!--\\s*END\\s*IF\\s*QUERY\\s*-->" );
    rx4.setMinimal ( TRUE );
    while ( ( pos = rx4.indexIn ( buff, 0 ) ) != -1 ) {
        QString ldetalle = trataIfQuery ( rx4.cap ( 1 ), rx4.cap ( 2 ).toAscii() );
        buff.replace ( pos, rx4.matchedLength(), ldetalle.toAscii() );
        pos = buff.indexOf("<!--");
    } // end while
    buff = cadant + buff;
    
    
    /// Buscamos Query's por tratar
    pos = buff.indexOf("<!-- QUERY");
    cadant = buff.left(pos);
    buff = buff.right(buff.length()-pos);
    QRegExp rx1 ( "<!--\\s*QUERY\\s*=\\s*\"([^\"]*)\"\\s*-->(.*)<!--\\s*END\\s*QUERY\\s*-->" );
    rx1.setMinimal ( TRUE );
    while ( ( pos = rx1.indexIn ( buff, 0 ) ) != -1 ) {
        QByteArray ldetalle = trataQuery ( rx1.cap ( 1 ), rx1.cap ( 2 ).toAscii(), tipoEscape );
        buff.replace ( pos, rx1.matchedLength(), ldetalle );
        pos = buff.indexOf("<!--");
    } // end while
    buff = cadant + buff;
    
    /// Buscamos SubQuery's en condicional
    pos = buff.indexOf("<!-- IF SUBQUERY");
    cadant = buff.left(pos);
    buff = buff.right(buff.length()-pos);
    QRegExp rx14 ( "<!--\\s*IF\\s*SUBQUERY\\s*=\\s*\"([^\"]*)\"\\s*-->(.*)<!--\\s*END\\s*IF\\s*SUBQUERY\\s*-->" );
    rx14.setMinimal ( TRUE );
    while ( ( pos = rx14.indexIn ( buff, 0 ) ) != -1 ) {
        QString ldetalle = trataIfQuery ( rx14.cap ( 1 ), rx14.cap ( 2 ).toAscii() );
        buff.replace ( pos, rx14.matchedLength(), ldetalle.toAscii() );
        pos = buff.indexOf("<!--");
    } // end while
    buff = cadant + buff;
    
    /// Buscamos SubQuery's por tratar
    pos = buff.indexOf("<!-- SUBQUERY");
    cadant = buff.left(pos);
    buff = buff.right(buff.length()-pos);
    QRegExp rx7 ( "<!--\\s*SUBQUERY\\s*=\\s*\"([^\"]*)\"\\s*-->(.*)<!--\\s*END\\s*SUBQUERY\\s*-->" );
    rx7.setMinimal ( TRUE );
    while ( ( pos = rx7.indexIn ( buff, 0 ) ) != -1 ) {
        QString ldetalle = trataQuery ( rx7.cap ( 1 ), rx7.cap ( 2 ).toAscii(), tipoEscape );
        buff.replace ( pos, rx7.matchedLength(), ldetalle.toAscii() );
        pos = buff.indexOf("<!--");
    } // end while
    buff = cadant + buff;
    
    /// Buscamos Query's en condicional
    pos = buff.indexOf("<!-- IF");
    cadant = buff.left(pos);
    buff = buff.right(buff.length()-pos);
    QRegExp rx11 ( "<!--\\s*IF\\s*=\\s*\"([^\"]*)\"\\s*-->(.*)<!--\\s*ELSE\\s*-->(.*)<!--\\s*END\\s*IF\\s*-->" );
    rx11.setMinimal ( TRUE );
    while ( ( pos = rx11.indexIn ( buff, 0 ) ) != -1 ) {
        QString ldetalle = trataIf ( rx11.cap ( 1 ), rx11.cap ( 2 ).toAscii(), rx11.cap ( 3 ).toAscii() );
        buff.replace ( pos, rx11.matchedLength(), ldetalle.toAscii() );
        pos = buff.indexOf("<!--");
    } // end while
    buff = cadant + buff;
    
    /// Cambio del CharacterPrinterMode
    pos = buff.indexOf("<!-- SETCHARACTERPRINTMODE");
    cadant = buff.left(pos);
    buff = buff.right(buff.length()-pos);
    QRegExp rx39 ( "<!--\\s*SETCHARACTERPRINTMODE\\s*\"([^\"]*)\"\\s*-->" );
    rx39.setMinimal ( TRUE );
    while ( ( pos = rx39.indexIn ( buff, 0 ) ) != -1 ) {
        QByteArray ldetalle = trataSetCharacterPrintMode ( rx39.cap ( 1 ), tipoEscape );
        buff.replace ( pos, rx39.matchedLength(), ldetalle );
	buff = cadant + buff;
        pos = buff.indexOf("<!-- SETCHARACTERPRINTMODE");
	cadant = buff.left(pos);
	buff = buff.right(buff.length()-pos);
    } // end while
    buff = cadant + buff;

    /// Cambio del CharacterPrinterMode
    pos = buff.indexOf("<!-- SETCHARACTERSPACING");
    cadant = buff.left(pos);
    buff = buff.right(buff.length()-pos);
    QRegExp rx49 ( "<!--\\s*SETCHARACTERSPACING\\s*\"([^\"]*)\"\\s*-->" );
    rx49.setMinimal ( TRUE );
    while ( ( pos = rx49.indexIn ( buff, 0 ) ) != -1 ) {
        QByteArray ldetalle = trataSetCharacterSpacing ( rx49.cap ( 1 ), tipoEscape );
        buff.replace ( pos, rx49.matchedLength(), ldetalle );
        pos = buff.indexOf("<!--");
    } // end while
    buff = cadant + buff;
    
    /// Cambio del CharacterCodeTable
    pos = buff.indexOf("<!-- SETCHARACTERCODETABLE");
    cadant = buff.left(pos);
    buff = buff.right(buff.length()-pos);
    QRegExp rx59 ( "<!--\\s*SETCHARACTERCODETABLE\\s*\"([^\"]*)\"\\s*-->" );
    rx59.setMinimal ( TRUE );
    while ( ( pos = rx59.indexIn ( buff, 0 ) ) != -1 ) {
        QByteArray ldetalle = trataSetCharacterCodeTable ( rx59.cap ( 1 ), tipoEscape );
        buff.replace ( pos, rx59.matchedLength(), ldetalle );
        pos = buff.indexOf("<!--");
    } // end while
    buff = cadant + buff;
    
    /// Cambio del CharacterCodeTable
    pos = buff.indexOf("<!-- SETUNDERLINEMODE");
    cadant = buff.left(pos);
    buff = buff.right(buff.length()-pos);
    QRegExp rx69 ( "<!--\\s*SETUNDERLINEMODE\\s*\"([^\"]*)\"\\s*-->" );
    rx69.setMinimal ( TRUE );
    while ( ( pos = rx69.indexIn ( buff, 0 ) ) != -1 ) {
        QByteArray ldetalle = trataSetUnderlineMode ( rx69.cap ( 1 ), tipoEscape );
        buff.replace ( pos, rx69.matchedLength(), ldetalle );
	buff = cadant + buff;
        pos = buff.indexOf("<!-- SETUNDERLINEMODE");
	cadant = buff.left(pos);
	buff = buff.right(buff.length()-pos);
    } // end while
    buff = cadant + buff;
    
    
    /// Cambio del CharacterSize
    pos = buff.indexOf("<!-- SETCHARACTERSIZE");
    cadant = buff.left(pos);
    buff = buff.right(buff.length()-pos);
    QRegExp rx79 ( "<!--\\s*SETCHARACTERSIZE\\s*\"([^\"]*)\"\\s*-->" );
    rx79.setMinimal ( TRUE );
    while ( ( pos = rx79.indexIn ( buff, 0 ) ) != -1 ) {
        QByteArray ldetalle = trataSetCharacterSize ( rx79.cap ( 1 ), tipoEscape );
        buff.replace ( pos, rx79.matchedLength(), ldetalle );
	buff = cadant + buff;
        pos = buff.indexOf("<!-- SETCHARACTERSIZE");
	cadant = buff.left(pos);
	buff = buff.right(buff.length()-pos);
    } // end while
    buff = cadant + buff;
    
    /// Cambio del Smoothing
    pos = buff.indexOf("<!-- SETSMOOTHING");
    cadant = buff.left(pos);
    buff = buff.right(buff.length()-pos);
    QRegExp rx89 ( "<!--\\s*SETSMOOTHING\\s*\"([^\"]*)\"\\s*-->" );
    rx89.setMinimal ( TRUE );
    while ( ( pos = rx89.indexIn ( buff, 0 ) ) != -1 ) {
        QByteArray ldetalle = trataSetCharacterSize ( rx89.cap ( 1 ), tipoEscape );
        buff.replace ( pos, rx89.matchedLength(), ldetalle );
        pos = buff.indexOf("<!--");
    } // end while
    buff = cadant + buff;

    /// Cambio del DoubleStrike
    pos = buff.indexOf("<!-- SETDOUBLESTRIKE");
    cadant = buff.left(pos);
    buff = buff.right(buff.length()-pos);
    QRegExp rx99 ( "<!--\\s*SETDOUBLESTRIKE\\s*\"([^\"]*)\"\\s*-->" );
    rx99.setMinimal ( TRUE );
    while ( ( pos = rx99.indexIn ( buff, 0 ) ) != -1 ) {
        QByteArray ldetalle = trataSetDoubleStrike ( rx99.cap ( 1 ), tipoEscape );
        buff.replace ( pos, rx99.matchedLength(), ldetalle );
        pos = buff.indexOf("<!--");
    } // end while
    buff = cadant + buff;
    
    /// Cambio del turnUpsideDown
    pos = buff.indexOf("<!-- TURNUPSIDEDOWN");
    cadant = buff.left(pos);
    buff = buff.right(buff.length()-pos);
    QRegExp rx02 ( "<!--\\s*TURNUPSIDEDOWN\\s*\"([^\"]*)\"\\s*-->" );
    rx02.setMinimal ( TRUE );
    while ( ( pos = rx02.indexIn ( buff, 0 ) ) != -1 ) {
        QByteArray ldetalle = trataTurnUpsideDown ( rx02.cap ( 1 ), tipoEscape );
        buff.replace ( pos, rx02.matchedLength(), ldetalle );
        pos = buff.indexOf("<!--");
    } // end while
    buff = cadant + buff;
    
    /// Cambio del turn90CWRotation
    pos = buff.indexOf("<!-- TURN90CWROTATION");
    cadant = buff.left(pos);
    buff = buff.right(buff.length()-pos);
    QRegExp rx12 ( "<!--\\s*TURN90CWROTATION\\s*\"([^\"]*)\"\\s*\"([^\"]*)\"\\s*-->" );
    rx12.setMinimal ( TRUE );
    while ( ( pos = rx12.indexIn ( buff, 0 ) ) != -1 ) {
        QByteArray ldetalle = trataTurn90CWRotation (rx12.cap(1), rx12.cap(2), tipoEscape );
        buff.replace ( pos, rx12.matchedLength(), ldetalle );
	buff = cadant + buff;
        pos = buff.indexOf("<!-- TURN90CWROTATION");
	cadant = buff.left(pos);
	buff = buff.right(buff.length()-pos);
    } // end while
    buff = cadant + buff;
    
    /// Cambio del turnWhiteBlack
    pos = buff.indexOf("<!-- TURNWHITEBLACK");
    cadant = buff.left(pos);
    buff = buff.right(buff.length()-pos);
    QRegExp rx22 ( "<!--\\s*TURNWHITEBLACK\\s*\"([^\"]*)\"\\s*-->" );
    rx22.setMinimal ( TRUE );
    while ( ( pos = rx22.indexIn ( buff, 0 ) ) != -1 ) {
        QByteArray ldetalle = trataTurnWhiteBlack( rx22.cap ( 1 ), tipoEscape );
        buff.replace ( pos, rx22.matchedLength(), ldetalle );
	buff = cadant + buff;
        pos = buff.indexOf("<!-- TURNWHITEBLACK");
	cadant = buff.left(pos);
	buff = buff.right(buff.length()-pos);
    } // end while
    buff = cadant + buff;
    
    /// Cambio del setColor
    pos = buff.indexOf("<!-- SETCOLOR");
    cadant = buff.left(pos);
    buff = buff.right(buff.length()-pos);
    QRegExp rx32 ( "<!--\\s*SETCOLOR\\s*\"([^\"]*)\"\\s*-->" );
    rx32.setMinimal ( TRUE );
    while ( ( pos = rx32.indexIn ( buff, 0 ) ) != -1 ) {
        QByteArray ldetalle = trataSetColor( rx32.cap ( 1 ), tipoEscape );
        buff.replace ( pos, rx32.matchedLength(), ldetalle );
        pos = buff.indexOf("<!--");
    } // end while
    buff = cadant + buff;
    
    /// Cambio del HorizontalTab
    pos = buff.indexOf("<!-- HORIZONTALTAB");
    cadant = buff.left(pos);
    buff = buff.right(buff.length()-pos);
    QRegExp rx42 ( "<!--\\s*HORIZONTALTAB\\s*-->" );
    rx42.setMinimal ( TRUE );
    while ( ( pos = rx42.indexIn ( buff, 0 ) ) != -1 ) {
        QByteArray ldetalle = trataHorizontalTab( tipoEscape );
        buff.replace ( pos, rx42.matchedLength(), ldetalle );
        pos = buff.indexOf("<!--");
    } // end while
    buff = cadant + buff;
    
    /// Cambio del setHorizontalTabPos
    pos = buff.indexOf("<!-- SETHORIZONTALTABPOS");
    cadant = buff.left(pos);
    buff = buff.right(buff.length()-pos);
    QRegExp rx52 ( "<!--\\s*SETHORIZONTALTABPOS\\s*\"([^\"]*)\"\\s*\"([^\"]*)\"\\s*-->" );
    rx52.setMinimal ( TRUE );
    while ( ( pos = rx52.indexIn ( buff, 0 ) ) != -1 ) {
        QByteArray ldetalle = trataSetHorizontalTabPos (rx52.cap(1), rx52.cap(2), tipoEscape );
        buff.replace ( pos, rx52.matchedLength(), ldetalle );
        pos = buff.indexOf("<!--");
    } // end while
    buff = cadant + buff;
    
    
    /// Cambio del setLeftMargin
    pos = buff.indexOf("<!-- SETLEFTMARGIN");
    cadant = buff.left(pos);
    buff = buff.right(buff.length()-pos);
    QRegExp rx62 ( "<!--\\s*SETLEFTMARGIN\\s*\"([^\"]*)\"\\s*-->" );
    rx62.setMinimal ( TRUE );
    while ( ( pos = rx62.indexIn ( buff, 0 ) ) != -1 ) {
        QByteArray ldetalle = trataSetLeftMargin( rx62.cap ( 1 ), tipoEscape );
        buff.replace ( pos, rx62.matchedLength(), ldetalle );
        pos = buff.indexOf("<!--");
    } // end while
    buff = cadant + buff;
    
    /// Cambio del setPrintingAreaWidth
    pos = buff.indexOf("<!-- SETPRINTINGAREAWIDTH");
    cadant = buff.left(pos);
    buff = buff.right(buff.length()-pos);
    QRegExp rx72 ( "<!--\\s*SETPRINTINGAREAWIDTH\\s*\"([^\"]*)\"\\s*-->" );
    rx72.setMinimal ( TRUE );
    while ( ( pos = rx72.indexIn ( buff, 0 ) ) != -1 ) {
        QByteArray ldetalle = trataSetPrintingAreaWidth( rx72.cap ( 1 ), tipoEscape );
        buff.replace ( pos, rx72.matchedLength(), ldetalle );
        pos = buff.indexOf("<!--");
    } // end while
    buff = cadant + buff;
    
    /// Cambio del setJustification
    pos = buff.indexOf("<!-- SETHABSOLUTEPOS");
    cadant = buff.left(pos);
    buff = buff.right(buff.length()-pos);
    QRegExp rx92 ( "<!--\\s*SETHABSOLUTEPOS\\s*\"([^\"]*)\"\\s*-->" );
    rx92.setMinimal ( TRUE );
    while ( ( pos = rx92.indexIn ( buff, 0 ) ) != -1 ) {
        QByteArray ldetalle = trataSetHAbsolutePos( rx92.cap ( 1 ), tipoEscape );
        buff.replace ( pos, rx92.matchedLength(), ldetalle );
        pos = buff.indexOf("<!--");
    } // end while
    buff = cadant + buff;
    
    
    /// Cambio del setJustification
    pos = buff.indexOf("<!-- SETHRELATIVEPOS");
    cadant = buff.left(pos);
    buff = buff.right(buff.length()-pos);
    QRegExp rx03 ( "<!--\\s*SETHRELATIVEPOS\\s*\"([^\"]*)\"\\s*-->" );
    rx03.setMinimal ( TRUE );
    while ( ( pos = rx03.indexIn ( buff, 0 ) ) != -1 ) {
        QByteArray ldetalle = trataSetHRelativePos( rx03.cap ( 1 ), tipoEscape );
        buff.replace ( pos, rx03.matchedLength(), ldetalle );
        pos = buff.indexOf("<!--");
    } // end while
    buff = cadant + buff;
    
    /// Cambio del setBARCODEFORMAT
    pos = buff.indexOf("<!-- SETBARCODEFORMAT");
    cadant = buff.left(pos);
    buff = buff.right(buff.length()-pos);
    QRegExp rx13 ( "<!--\\s*SETBARCODEFORMAT\\s*\"([^\"]*)\"\\s*\"([^\"]*)\"\\s*\"([^\"]*)\"\\s*\"([^\"]*)\"\\s*-->" );
    rx13.setMinimal ( TRUE );
    while ( ( pos = rx13.indexIn ( buff, 0 ) ) != -1 ) {
        QByteArray ldetalle = trataSetBarcodeFormat( rx13.cap ( 1 ), rx13.cap ( 2 ),rx13.cap ( 3 ),rx13.cap ( 4 ),tipoEscape );
        buff.replace ( pos, rx13.matchedLength(), ldetalle );
        pos = buff.indexOf("<!--");
    } // end while
    buff = cadant + buff;
    
    /// Cambio del PRINTBARCODE
    pos = buff.indexOf("<!-- PRINTBARCODE");
    cadant = buff.left(pos);
    buff = buff.right(buff.length()-pos);
    QRegExp rx23 ( "<!--\\s*PRINTBARCODE\\s*\"([^\"]*)\"\\s*\"([^\"]*)\"\\s*\"([^\"]*)\"\\s*-->" );
    rx23.setMinimal ( TRUE );
    while ( ( pos = rx23.indexIn ( buff, 0 ) ) != -1 ) {
        QByteArray ldetalle = trataPrintBarCode( rx23.cap ( 1 ), rx23.cap ( 2 ),rx23.cap ( 3 ),tipoEscape );
        buff.replace ( pos, rx23.matchedLength(), ldetalle );
        pos = buff.indexOf("<!--");
    } // end while
    buff = cadant + buff;
    
    /// Cambio del SETBARCODEHEIGHT
    pos = buff.indexOf("<!-- SETBARCODEHEIGHT");
    cadant = buff.left(pos);
    buff = buff.right(buff.length()-pos);
    QRegExp rx33 ( "<!--\\s*SETBARCODEHEIGHT\\s*\"([^\"]*)\"\\s*-->" );
    rx33.setMinimal ( TRUE );
    while ( ( pos = rx33.indexIn ( buff, 0 ) ) != -1 ) {
        QByteArray ldetalle = trataSetBarCodeHeight( rx33.cap ( 1 ), tipoEscape );
        buff.replace ( pos, rx33.matchedLength(), ldetalle );
        pos = buff.indexOf("<!--");
    } // end while
    buff = cadant + buff;
    
    /// Cambio del SETBARCODEWIDTH
    pos = buff.indexOf("<!-- SETBARCODEWIDTH");
    cadant = buff.left(pos);
    buff = buff.right(buff.length()-pos);
    QRegExp rx34 ( "<!--\\s*SETBARCODEWIDTH\\s*\"([^\"]*)\"\\s*-->" );
    rx34.setMinimal ( TRUE );
    while ( ( pos = rx34.indexIn ( buff, 0 ) ) != -1 ) {
        QByteArray ldetalle = trataSetBarCodeWidth( rx34.cap ( 1 ), tipoEscape );
        buff.replace ( pos, rx34.matchedLength(), ldetalle );
        pos = buff.indexOf("<!--");
    } // end while
    buff = cadant + buff;
    
    /// Cambio del SELECTPAGEMODE
    pos = buff.indexOf("<!-- SELECTPAGEMODE");
    cadant = buff.left(pos);
    buff = buff.right(buff.length()-pos);
    QRegExp rx35 ( "<!--\\s*SELECTPAGEMODE\\s*-->" );
    rx35.setMinimal ( TRUE );
    while ( ( pos = rx35.indexIn ( buff, 0 ) ) != -1 ) {
        QByteArray ldetalle = trataSelectPageMode( tipoEscape );
        buff.replace ( pos, rx35.matchedLength(), ldetalle );
        pos = buff.indexOf("<!--");
    } // end while
    buff = cadant + buff;
    
    /// Cambio del SETBARCODEWIDTH
    pos = buff.indexOf("<!-- SETPRINTAREA");
    cadant = buff.left(pos);
    buff = buff.right(buff.length()-pos);
    QRegExp rx36 ( "<!--\\s*SETPRINTAREA\\s*\"([^\"]*)\"\\s*\"([^\"]*)\"\\s*\"([^\"]*)\"\\s*\"([^\"]*)\"\\s*-->" );
    rx36.setMinimal ( TRUE );
    while ( ( pos = rx36.indexIn ( buff, 0 ) ) != -1 ) {
        QByteArray ldetalle = trataSetPrintArea( rx36.cap ( 1 ), rx36.cap ( 2 ), rx36.cap ( 3 ), rx36.cap ( 4 ), tipoEscape );
        buff.replace ( pos, rx36.matchedLength(), ldetalle );
        pos = buff.indexOf("<!--");
    } // end while
    buff = cadant + buff;
    
    /// Cambio del SETPRINTDIRECTION
    pos = buff.indexOf("<!-- SETPRINTDIRECTION");
    cadant = buff.left(pos);
    buff = buff.right(buff.length()-pos);
    QRegExp rx37 ( "<!--\\s*SETPRINTDIRECTION\\s*\"([^\"]*)\"\\s*-->" );
    rx37.setMinimal ( TRUE );
    while ( ( pos = rx37.indexIn ( buff, 0 ) ) != -1 ) {
        QByteArray ldetalle = trataSetPrintDirection( rx37.cap ( 1 ), tipoEscape );
        buff.replace ( pos, rx37.matchedLength(), ldetalle );
        pos = buff.indexOf("<!--");
    } // end while
    buff = cadant + buff;
    
    /// Cambio del setJustification
    pos = buff.indexOf("<!-- SETVABSOLUTEPOS");
    cadant = buff.left(pos);
    buff = buff.right(buff.length()-pos);
    QRegExp rx38 ( "<!--\\s*SETVABSOLUTEPOS\\s*\"([^\"]*)\"\\s*-->" );
    rx38.setMinimal ( TRUE );
    while ( ( pos = rx38.indexIn ( buff, 0 ) ) != -1 ) {
        QByteArray ldetalle = trataSetVAbsolutePos( rx38.cap ( 1 ), tipoEscape );
        buff.replace ( pos, rx38.matchedLength(), ldetalle );
        pos = buff.indexOf("<!--");
    } // end while
    buff = cadant + buff;
    
    /// Cambio del setJustification
    pos = buff.indexOf("<!-- SETVRELATIVEPOS");
    cadant = buff.left(pos);
    buff = buff.right(buff.length()-pos);
    QRegExp rx41 ( "<!--\\s*SETVRELATIVEPOS\\s*\"([^\"]*)\"\\s*-->" );
    rx41.setMinimal ( TRUE );
    while ( ( pos = rx41.indexIn ( buff, pos ) ) != -1 ) {
        QByteArray ldetalle = trataSetVRelativePos( rx41.cap ( 1 ), tipoEscape );
        buff.replace ( pos, rx41.matchedLength(), ldetalle );
        pos = buff.indexOf("<!--");
    } // end while
    buff = cadant + buff;
    
    /// Cambio del cutPaper
    pos = buff.indexOf("<!-- CUTPAPER");
    cadant = buff.left(pos);
    buff = buff.right(buff.length()-pos);
    QRegExp rx45 ( "<!--\\s*CUTPAPER\\s*\"([^\"]*)\"\\s*-->" );
    rx45.setMinimal ( TRUE );
    while ( ( pos = rx45.indexIn ( buff, 0 ) ) != -1 ) {
        QByteArray ldetalle = trataCutPaper( rx45.cap ( 1 ), tipoEscape );
        buff.replace ( pos, rx45.matchedLength(), ldetalle );
        pos = 0;
    } // end while
    buff = cadant + buff;

    /// Cambio del RIGHTJUSTIFIED
    pos = buff.indexOf("<!-- RIGHTJUSTIFIED");
    cadant = buff.left(pos);
    buff = buff.right(buff.length()-pos);
    QRegExp rx46 ( "<!--\\s*RIGHTJUSTIFIED\\s*\"([^\"]*)\"\\s*\"([^\"]*)\"\\s*\"([^\"]*)\"\\s*\"([^\"]*)\"\\s*-->" );
    rx46.setMinimal ( TRUE );
    while ( ( pos = rx46.indexIn ( buff, 0 ) ) != -1 ) {
        QByteArray ldetalle = trataRightJustified( rx46.cap ( 1 ),  rx46.cap ( 2 ), rx46.cap ( 3 ),  rx46.cap ( 4 ),tipoEscape );
        buff.replace ( pos, rx46.matchedLength(), ldetalle );
	buff = cadant + buff;
        pos = buff.indexOf("<!-- RIGHTJUSTIFIED");
	cadant = buff.left(pos);
	buff = buff.right(buff.length()-pos);
    } // end while
    buff = cadant + buff;
    
    
    /// Cambio del cutPaperAndFeed
    pos = buff.indexOf("<!-- CUTPAPERANDFEED");
    cadant = buff.left(pos);
    buff = buff.right(buff.length()-pos);
    QRegExp rx44 ( "<!--\\s*CUTPAPERANDFEED\\s*\"([^\"]*)\"\\s*\"([^\"]*)\"\\s*-->" );
    rx44.setMinimal ( TRUE );
    while ( ( pos = rx44.indexIn ( buff, 0 ) ) != -1 ) {
        QByteArray ldetalle = trataCutPaperAndFeed( rx44.cap ( 1 ),  rx44.cap ( 2 ),tipoEscape );
        buff.replace ( pos, rx44.matchedLength(), ldetalle );
        pos = buff.indexOf("<!--");
    } // end while
    buff = cadant + buff;
    
    profundidad --;
    if (profundidad == 0) {
      trataTagsPost(buff, tipoEscape);
    } // end if
    
    
    blDebug ( "END BlDbRecord::trataTags", 0 );
    return 1;
}



/// Busca strings del tipo [xxxx] entro del texto pasado y los sustituye
/// Por valores existentes en la base de datos.
/// Tambien busca los parametros PARAM e IFACE para tambien tratarlos.
/// Devuelve 1 Si todo esta OK o 0 Si hay algun error
/**
\param buff El texto entero sobre el que se hace el reemplazo de sentencias.
**/
int BlDbRecord::trataTagsPost ( QByteArray &buff, int tipoEscape )
{
    blDebug ( "BlDbRecord::trataTagsPost", 0 );

    ///Buscamos interfaces, los preguntamos y los ponemos.
    int pos = 0;
    QByteArray cadant;

    /// Cambio del setJustification
    pos = buff.indexOf("<!-- SETJUSTIFICATION");
    cadant = buff.left(pos);
    buff = buff.right(buff.length()-pos);
    QRegExp rx82 ( "<!--\\s*SETJUSTIFICATION\\s*\"([^\"]*)\"\\s*-->" );
    rx82.setMinimal ( TRUE );
    while ( ( pos = rx82.indexIn ( buff, 0 ) ) != -1 ) {
        QByteArray ldetalle = trataSetJustification( rx82.cap ( 1 ), tipoEscape );
        buff.replace ( pos, rx82.matchedLength(), ldetalle );
	buff = cadant + buff;
	pos = buff.indexOf("<!-- SETJUSTIFICATION");
	cadant = buff.left(pos);
	buff = buff.right(buff.length() - pos );
    } // end while
    buff = cadant + buff;
    
    
    /// Cambio del PRINTBARCODE
    pos = buff.indexOf("<!-- PRINTBARCODE");
    cadant = buff.left(pos);
    buff = buff.right(buff.length()-pos);
    QRegExp rx23 ( "<!--\\s*PRINTBARCODE\\s*\"([^\"]*)\"\\s*\"([^\"]*)\"\\s*\"([^\"]*)\"\\s*-->" );
    rx23.setMinimal ( TRUE );
    while ( ( pos = rx23.indexIn ( buff, 0 ) ) != -1 ) {
        QByteArray ldetalle = trataPrintBarCode( rx23.cap ( 1 ), rx23.cap ( 2 ),rx23.cap ( 3 ),tipoEscape );
        buff.replace ( pos, rx23.matchedLength(), ldetalle );
        pos = buff.indexOf("<!--");
    } // end while
    buff = cadant + buff;
    
    /// Cambio del SETBARCODEHEIGHT
    pos = buff.indexOf("<!-- SETBARCODEHEIGHT");
    cadant = buff.left(pos);
    buff = buff.right(buff.length()-pos);
    QRegExp rx33 ( "<!--\\s*SETBARCODEHEIGHT\\s*\"([^\"]*)\"\\s*-->" );
    rx33.setMinimal ( TRUE );
    while ( ( pos = rx33.indexIn ( buff, 0 ) ) != -1 ) {
        QByteArray ldetalle = trataSetBarCodeHeight( rx33.cap ( 1 ), tipoEscape );
        buff.replace ( pos, rx33.matchedLength(), ldetalle );
        pos = buff.indexOf("<!--");
    } // end while
    buff = cadant + buff;
    

    /// Cambio del RIGHTJUSTIFIED
    pos = buff.indexOf("<!-- RIGHTJUSTIFIED");
    cadant = buff.left(pos);
    buff = buff.right(buff.length()-pos);
    QRegExp rx46 ( "<!--\\s*RIGHTJUSTIFIED\\s*\"([^\"]*)\"\\s*\"([^\"]*)\"\\s*\"([^\"]*)\"\\s*\"([^\"]*)\"\\s*-->" );
    rx46.setMinimal ( TRUE );
    while ( ( pos = rx46.indexIn ( buff, 0 ) ) != -1 ) {
        QByteArray ldetalle = trataRightJustified( rx46.cap ( 1 ),  rx46.cap ( 2 ), rx46.cap ( 3 ),  rx46.cap ( 4 ),tipoEscape );
        buff.replace ( pos, rx46.matchedLength(), ldetalle );
	buff = cadant + buff;
        pos = buff.indexOf("<!-- RIGHTJUSTIFIED");
	cadant = buff.left(pos);
	buff = buff.right(buff.length()-pos);
    } // end while
    buff = cadant + buff;
    
    /// Cambio del RIGHTJUSTIFIED
    pos = buff.indexOf("<!-- LEFTJUSTIFIED");
    cadant = buff.left(pos);
    buff = buff.right(buff.length()-pos);
    QRegExp rx47 ( "<!--\\s*LEFTJUSTIFIED\\s*\"([^\"]*)\"\\s*\"([^\"]*)\"\\s*\"([^\"]*)\"\\s*\"([^\"]*)\"\\s*-->" );
    rx47.setMinimal ( TRUE );
    while ( ( pos = rx47.indexIn ( buff, 0 ) ) != -1 ) {
        QByteArray ldetalle = trataLeftJustified( rx47.cap ( 1 ),  rx47.cap ( 2 ), rx47.cap ( 3 ),  rx47.cap ( 4 ),tipoEscape );
        buff.replace ( pos, rx47.matchedLength(), ldetalle );
	buff = cadant + buff;
        pos = buff.indexOf("<!-- LEFTJUSTIFIED");
	cadant = buff.left(pos);
	buff = buff.right(buff.length()-pos);
    } // end while
    buff = cadant + buff;
    
    /// Cambio del cutPaperAndFeed
    pos = buff.indexOf("<!-- CUTPAPERANDFEED");
    cadant = buff.left(pos);
    buff = buff.right(buff.length()-pos);
    QRegExp rx44 ( "<!--\\s*CUTPAPERANDFEED\\s*\"([^\"]*)\"\\s*\"([^\"]*)\"\\s*-->" );
    rx44.setMinimal ( TRUE );
    while ( ( pos = rx44.indexIn ( buff, 0 ) ) != -1 ) {
        QByteArray ldetalle = trataCutPaperAndFeed( rx44.cap ( 1 ),  rx44.cap ( 2 ),tipoEscape );
        buff.replace ( pos, rx44.matchedLength(), ldetalle );
        pos = buff.indexOf("<!--");
    } // end while
    buff = cadant + buff;

    /// Inclusion de imagenes
    pos = buff.indexOf("<!-- IMG");
    cadant = buff.left(pos);
    buff = buff.right(buff.length()-pos);
    QRegExp rx29 ( "<!--\\s*IMG\\s*SRC\\s*=\\s*\"([^\"]*)\"\\s*-->" );
    rx29.setMinimal ( TRUE );
    while ( ( pos = rx29.indexIn ( buff, 0 ) ) != -1 ) {
        QByteArray ldetalle = trataIncludeImg ( rx29.cap ( 1 ), tipoEscape );
        buff.replace ( pos, rx29.matchedLength(), ldetalle );
	buff = cadant + buff;
        pos = buff.indexOf("<!-- IMG");
	cadant = buff.left(pos);
	buff = buff.right(buff.length()-pos);
    } // end while
    buff = cadant + buff;

    /// Inclusion de imagenes
    pos = buff.indexOf("<!-- PNGRAW64");
    cadant = buff.left(pos);
    buff = buff.right(buff.length()-pos);
    QRegExp rx791 ( "<!--\\s*PNGRAW64\\s*DATA\\s*=\\s*\"([^\"]*)\"\\s*-->" );
    rx791.setMinimal ( TRUE );
    while ( ( pos = rx791.indexIn ( buff, 0 ) ) != -1 ) {
        QByteArray ldetalle = trataPngRaw64 ( rx791.cap ( 1 ).toAscii(), tipoEscape );
        buff.replace ( pos, rx791.matchedLength(), ldetalle );
	buff = cadant + buff;
        pos = buff.indexOf("<!-- PNGRAW64");
	cadant = buff.left(pos);
	buff = buff.right(buff.length()-pos);
    } // end while
    buff = cadant + buff;
    
    blDebug ( "END BlDbRecord::trataTagsPost", 0 );
    return 1;
}



/// Trata las lineas de detalle encontradas dentro de los tags <!--LINEAS DETALLE-->
/**
\param det Texto de entrada para ser tratado por iteracion.
\return Si el query tiene elementos lo devuelve el parametro. En caso contrario no devuelve nada.
**/
QByteArray BlDbRecord::trataIfQuery ( const QString &query, const QByteArray &datos )
{
    blDebug ( "BlDbRecord::trataIfQuery", 0 );
    QByteArray result = "";
    QByteArray query1 = query.toAscii();

    /// Buscamos parametros en el query y los ponemos.
    substrVars ( query1 );

    /// Cargamos el query y lo recorremos
    BlDbRecordSet *cur = m_dbConnection->loadQuery ( QString(query1) );
    if ( !cur ) return "";
    if ( !cur->eof() ) {
        result = datos;
    } // end while
    delete cur;
    blDebug ( "END BlDbRecord::trataIfQuery", 0 );
    return result;
}


/// Trata las lineas de detalle encontradas dentro de los tags <!--LINEAS DETALLE-->
/**
\param det Texto de entrada para ser tratado por iteracion.
\return Si el query tiene elementos lo devuelve el parametro. En caso contrario no devuelve nada.
**/
QByteArray BlDbRecord::trataIf ( const QString &query, const QByteArray &datos, const QByteArray &datos1 )
{
    blDebug ( "BlDbRecord::trataIfQuery", 0 );
    QByteArray result = "";
    QByteArray query1 = query.toAscii();

    /// Buscamos parametros en el query y los ponemos.
    substrVars ( query1 );

    QString query2 = "SELECT (" + QString(query1) + ") AS res";
    /// Cargamos el query y lo recorremos
    BlDbRecordSet *cur = m_dbConnection ->loadQuery ( query2 );
    if ( !cur ) return "";
    if ( !cur->eof() ) {
        if ( cur->valor ( "res" ) == "t" ) {
            result = datos;
        } else {
            result = datos1;
        } // end if
    } // end while
    delete cur;
    blDebug ( "END BlDbRecord::trataIf", 0 );
    return result;
}


/// Trata las lineas de detalle encontradas dentro de los tags <!--LINEAS DETALLE-->
/**
\param det Texto de entrada para ser tratado por iteracion.
\return
**/
QByteArray BlDbRecord::trataIncludeFileTXT ( const QString &file, int tipoEscape )
{
    blDebug ( "BlDbRecord::trataIncludeFileTXT", 0 );
    QByteArray read = "";
    QFile arch ( file );
    if ( arch.open ( QIODevice::ReadOnly ) ) {
       read = arch.readAll();
        arch.close();
    } // end if
    /// Buscamos parametros en el query y los ponemos.
    substrVars ( read, tipoEscape );


    blDebug ( "END BlDbRecord::trataIncludeFileTXT", 0 );
    return read;

}


/// Trata las inclusiones de imagenes
/**
\param det Texto de entrada para ser tratado por iteracion.
\return
**/
QByteArray BlDbRecord::trataIncludeImg ( const QString &file, int tipoEscape )
{
    blDebug ( "BlDbRecord::trataIncludeImg", 0 );
    BlEscPrinter pr;
    pr.clearBuffer();
    pr.printImage(file);
    blDebug ( "END BlDbRecord::trataIncludeImg", 0 );
    return pr.buffer();

}


/// Trata las inclusiones de imagenes
/**
\param det Texto de entrada para ser tratado por iteracion.
\return
**/
QByteArray BlDbRecord::trataPngRaw64 ( const QByteArray &data, int tipoEscape )
{
    blDebug ( "BlDbRecord::trataPngRaw64", 0 );
//    blMsgInfo("pngRawData");
    BlEscPrinter pr;
    pr.clearBuffer();
    QByteArray dataq = QByteArray::fromBase64(data);
    pr.printImageRaw(dataq);
    blDebug ( "END BlDbRecord::trataPngRaw64", 0 );
//    blMsgInfo(pr.buffer());
    return pr.buffer();

}

/// Trata el setcharacterprintermode
/**
\param det Texto de entrada para ser tratado por iteracion.
\return
**/
QByteArray BlDbRecord::trataSetCharacterPrintMode ( const QString &param, int tipoEscape )
{

    int modo=0;
    blDebug ( "BlDbRecord::trataSetCharacterPrintMode", 0 );
    BlEscPrinter pr;
    pr.clearBuffer();
    if (param.contains("CHARACTER_FONTA_SELECTED"))
        modo |= CHARACTER_FONTA_SELECTED;
    if (param.contains("CHARACTER_FONTB_SELECTED"))
        modo |= CHARACTER_FONTB_SELECTED;
    if (param.contains("EMPHASIZED_MODE"))
        modo |= EMPHASIZED_MODE;
    if (param.contains("DOUBLE_HEIGHT"))
        modo |= DOUBLE_HEIGHT;
    if (param.contains("DOUBLE_WIDTH"))
        modo |= DOUBLE_WIDTH;
    if (param.contains("UNDERLINE_MODE"))
        modo |= UNDERLINE_MODE;

    pr.setCharacterPrintMode(modo);
    blDebug ( "END BlDbRecord::trataSetCharacterPrintMode", 0 );
    return pr.buffer();

}

/// Trata el setcharacterprintermode
/**
\param det Texto de entrada para ser tratado por iteracion.
\return
**/
QByteArray BlDbRecord::trataSetCharacterSpacing ( const QString &param, int tipoEscape )
{

    int modo=0;
    blDebug ( "BlDbRecord::trataSetCharacterSpacing", 0 );
    BlEscPrinter pr;
    pr.clearBuffer();

    pr.setCharacterSpacing(param.toInt());
    blDebug ( "END BlDbRecord::trataSetCharacterSpacing", 0 );
    return pr.buffer();

}



/// Trata el setcharacterprintermode
/**
\param det Texto de entrada para ser tratado por iteracion.
\return
**/
QByteArray BlDbRecord::trataSetCharacterCodeTable ( const QString &param, int tipoEscape )
{
    blDebug ( "BlDbRecord::trataSetCharacterCodeTable", 0 );
    characterCodeTable codetable=page0;
    BlEscPrinter pr;
    pr.clearBuffer();
    if (param.contains("page0"))
        codetable = page0;
    if (param.contains("page1"))
        codetable = page1;
    if (param.contains("page2"))
        codetable = page2;
    if (param.contains("page3"))
        codetable = page3;
    if (param.contains("page4"))
        codetable = page4;
    if (param.contains("page5"))
        codetable = page5;

    pr.setCharacterCodeTable(codetable);
    blDebug ( "END BlDbRecord::trataSetCharacterCodeTable", 0 );
    return pr.buffer();

}

/// Trata el setcharacterprintermode
/**
\param det Texto de entrada para ser tratado por iteracion.
\return
**/
QByteArray BlDbRecord::trataSetUnderlineMode ( const QString &param, int tipoEscape )
{

    bool modo=FALSE;
    blDebug ( "BlDbRecord::trataSetUnderlineMode", 0 );
    BlEscPrinter pr;
    pr.clearBuffer();
    if (param.contains("TRUE"))
        modo = TRUE;
    if (param.contains("true"))
        modo = TRUE;
    if (param.contains("1"))
        modo = TRUE;

    pr.setUnderlineMode(modo);
    blDebug ( "END BlDbRecord::trataSetUnderlineMode", 0 );
    return pr.buffer();

}

/// Trata el SetCharacterSize
/**
\param det Texto de entrada para ser tratado por iteracion.
\return
**/
QByteArray BlDbRecord::trataSetCharacterSize ( const QString &param, int tipoEscape )
{

    int modo=0;
    blDebug ( "BlDbRecord::trataSetCharacterSpacing", 0 );
    BlEscPrinter pr;
    pr.clearBuffer();

    pr.setCharacterSize(param.toInt());
    blDebug ( "END BlDbRecord::trataSetCharacterSpacing", 0 );
    return pr.buffer();

}

/// Trata el setsmoothing
/**
\param det Texto de entrada para ser tratado por iteracion.
\return
**/
QByteArray BlDbRecord::trataSetSmoothing ( const QString &param, int tipoEscape )
{

    bool modo=FALSE;
    blDebug ( "BlDbRecord::trataSetSmoothing", 0 );
    BlEscPrinter pr;
    pr.clearBuffer();
    if (param.contains("TRUE"))
        modo = TRUE;
    if (param.contains("true"))
        modo = TRUE;
    if (param.contains("1"))
        modo = TRUE;

    pr.setSmoothing(modo);
    blDebug ( "END BlDbRecord::trataSetSmoothing", 0 );
    return pr.buffer();

}

/// Trata el trataSetDoubleStrike
/**
\param det Texto de entrada para ser tratado por iteracion.
\return
**/
QByteArray BlDbRecord::trataSetDoubleStrike ( const QString &param, int tipoEscape )
{

    bool modo=FALSE;
    blDebug ( "BlDbRecord::trataSetDoubleStrike", 0 );
    BlEscPrinter pr;
    pr.clearBuffer();
    if (param.contains("TRUE"))
        modo = TRUE;
    if (param.contains("true"))
        modo = TRUE;
    if (param.contains("1"))
        modo = TRUE;

    pr.setDoubleStrike(modo);
    blDebug ( "END BlDbRecord::trataSetDoubleStrike", 0 );
    return pr.buffer();

}

/// Trata el trataTurnUpsideDown
/**
\param det Texto de entrada para ser tratado por iteracion.
\return
**/
QByteArray BlDbRecord::trataTurnUpsideDown ( const QString &param, int tipoEscape )
{

    bool modo=FALSE;
    blDebug ( "BlDbRecord::trataTurnUpsideDown", 0 );
    BlEscPrinter pr;
    pr.clearBuffer();
    if (param.contains("TRUE"))
        modo = TRUE;
    if (param.contains("true"))
        modo = TRUE;
    if (param.contains("1"))
        modo = TRUE;

    pr.turnUpsideDown(modo);
    blDebug ( "END BlDbRecord::trataTurnUpsideDown", 0 );
    return pr.buffer();

}


/// Trata el trataTurn90CWRotation
/**
\param det Texto de entrada para ser tratado por iteracion.
\return
**/
QByteArray BlDbRecord::trataTurn90CWRotation ( const QString &param, const QString &param1, int tipoEscape )
{

    bool modo=FALSE;
    bool extra = FALSE;
    blDebug ( "BlDbRecord::trataTurn90CWRotation", 0 );
    BlEscPrinter pr;
    pr.clearBuffer();
    if (param.contains("TRUE"))
        modo = TRUE;
    if (param.contains("true"))
        modo = TRUE;
    if (param.contains("1"))
        modo = TRUE;

    if (param1.contains("TRUE") || param1.contains("t") || param1.contains("1"))
        extra = TRUE;

    pr.turn90CWRotation(modo,extra);
    blDebug ( "END BlDbRecord::trataTurn90CWRotation", 0 );
    return pr.buffer();

}


/// Trata el trataTurnWhiteBlack
/**
\param det Texto de entrada para ser tratado por iteracion.
\return
**/
QByteArray BlDbRecord::trataTurnWhiteBlack ( const QString &param, int tipoEscape )
{

    bool modo=FALSE;
    blDebug ( "BlDbRecord::trataTurnWhiteBlack", 0 );
    BlEscPrinter pr;
    pr.clearBuffer();
    if (param.contains("TRUE"))
        modo = TRUE;
    if (param.contains("true"))
        modo = TRUE;
    if (param.contains("1"))
        modo = TRUE;

    pr.turnWhiteBlack(modo);
    blDebug ( "END BlDbRecord::trataTurnWhiteBlack", 0 );
    return pr.buffer();

}

/// Trata el trataSetColor
/**
\param det Texto de entrada para ser tratado por iteracion.
\return
**/
QByteArray BlDbRecord::trataSetColor ( const QString &param, int tipoEscape )
{
    blDebug ( "BlDbRecord::trataSetColor", 0 );
    printColor color=black;
    BlEscPrinter pr;
    pr.clearBuffer();
    if (param.contains("BLACK"))
        color = black;
    if (param.contains("RED"))
        color = red;

    pr.setColor(color);
    blDebug ( "END BlDbRecord::trataSetColor", 0 );
    return pr.buffer();

}

/// Trata el trataSetColor
/**
\param det Texto de entrada para ser tratado por iteracion.
\return
**/
QByteArray BlDbRecord::trataHorizontalTab ( int tipoEscape )
{
    blDebug ( "BlDbRecord::trataHorizontalTab", 0 );
    BlEscPrinter pr;
    pr.clearBuffer();
    pr.horizontalTab();
    blDebug ( "END BlDbRecord::trataHorizontalTab", 0 );
    return pr.buffer();

}

/// Trata el trataTurn90CWRotation
/**
\param det Texto de entrada para ser tratado por iteracion.
\return
**/
QByteArray BlDbRecord::trataSetHorizontalTabPos ( const QString &param, const QString &param1, int tipoEscape )
{

    blDebug ( "BlDbRecord::trataSetHorizontalTabPos", 0 );
    BlEscPrinter pr;
    pr.clearBuffer();

    pr.setHorizontalTabPos(param.toInt(),param1.toLatin1().data());
    blDebug ( "END BlDbRecord::trataSetHorizontalTabPos", 0 );
    return pr.buffer();

}


/// Trata el trataSetLeftMargin
/**
\param det Texto de entrada para ser tratado por iteracion.
\return
**/
QByteArray BlDbRecord::trataSetLeftMargin ( const QString &param, int tipoEscape )
{

    int modo=0;
    blDebug ( "BlDbRecord::trataSetLeftMargin", 0 );
    BlEscPrinter pr;
    pr.clearBuffer();

    pr.setLeftMargin(param.toInt());
    blDebug ( "END BlDbRecord::trataSetLeftMargin", 0 );
    return pr.buffer();

}

/// Trata el trataSetPrintingAreaWidth
/**
\param det Texto de entrada para ser tratado por iteracion.
\return
**/
QByteArray BlDbRecord::trataSetPrintingAreaWidth ( const QString &param, int tipoEscape )
{

    int modo=0;
    blDebug ( "BlDbRecord::trataSetPrintingAreaWidth", 0 );
    BlEscPrinter pr;
    pr.clearBuffer();

    pr.setPrintingAreaWidth(param.toInt());
    blDebug ( "END BlDbRecord::trataSetPrintingAreaWidth", 0 );
    return pr.buffer();

}


/// Trata el trataSetJustification
/**
\param det Texto de entrada para ser tratado por iteracion.
\return
**/
QByteArray BlDbRecord::trataSetJustification ( const QString &param, int tipoEscape )
{

    BlEscPrinter::justification modo = BlEscPrinter::left ;
    blDebug ( "BlDbRecord::trataSetJustification", 0 );
    BlEscPrinter pr;
    pr.clearBuffer();
    if (param.contains("LEFT"))
        modo = BlEscPrinter::left;
    if (param.contains("CENTER"))
        modo = BlEscPrinter::center;
    if (param.contains("RIGHT"))
        modo = BlEscPrinter::right;

    pr.setJustification(modo);
    blDebug ( "END BlDbRecord::trataSetJustification", 0 );
    return pr.buffer();

}

/// Trata el trataSetHAbsolutePos
/**
\param det Texto de entrada para ser tratado por iteracion.
\return
**/
QByteArray BlDbRecord::trataSetHAbsolutePos ( const QString &param, int tipoEscape )
{

    int modo=0;
    blDebug ( "BlDbRecord::trataSetHAbsolutePos", 0 );
    BlEscPrinter pr;
    pr.clearBuffer();

    pr.setHAbsolutePos(param.toInt());
    blDebug ( "END BlDbRecord::trataSetHAbsolutePos", 0 );
    return pr.buffer();

}

/// Trata el trataSetHRelativePos
/**
\param det Texto de entrada para ser tratado por iteracion.
\return
**/
QByteArray BlDbRecord::trataSetHRelativePos ( const QString &param, int tipoEscape )
{

    int modo=0;
    blDebug ( "BlDbRecord::trataSetHRelativePos", 0 );
    BlEscPrinter pr;
    pr.clearBuffer();

    pr.setHRelativePos(param.toInt());
    blDebug ( "END BlDbRecord::trataSetHRelativePos", 0 );
    return pr.buffer();

}


/// Trata el trataSetBarcodeFormat
/**
\param det Texto de entrada para ser tratado por iteracion.
\return
**/
QByteArray BlDbRecord::trataSetBarcodeFormat ( const QString &param, const QString &param1,const QString &param2,const QString &param3,int tipoEscape )
{

    barCodeTextPos pos = notPrinted;
    printerFont    font = fontA;
    blDebug ( "BlDbRecord::trataSetHRelativePos", 0 );
    BlEscPrinter pr;
    pr.clearBuffer();

    if (param2.contains("NOTPRINTED"))
        pos = notPrinted;
    if (param2.contains("ABOVE"))
        pos = above;
    if (param2.contains("BELOW"))
        pos = below;
    if (param2.contains("BOTH"))
        pos = both;

    if (param3.contains("FONTB"))
        font = fontB;

    pr.setBarcodeFormat(param.toInt(), param1.toInt(), pos, font);
    blDebug ( "END BlDbRecord::trataSetHRelativePos", 0 );
    return pr.buffer();

}



/// Trata el trataPrintBarCode
/**
\param det Texto de entrada para ser tratado por iteracion.
\return
**/
QByteArray BlDbRecord::trataPrintBarCode ( const QString &param, const QString &param1,const QString &param2,int tipoEscape )
{

    barcodeSystem system = upca;
    blDebug ( "BlDbRecord::trataPrintBarCode", 0 );
    BlEscPrinter pr;
    pr.clearBuffer();

    if (param.contains("UPCA"))
        system = upca;
    if (param.contains("UPCE"))
        system = upce;
    if (param.contains("JAN13"))
        system = jan13;
    if (param.contains("JAN8"))
        system = jan8;
    if (param.contains("CODE39"))
        system = code39;
    if (param.contains("ITF"))
        system = itf;
    if (param.contains("CODABAR"))
        system = codabar;
    if (param.contains("CODE93"))
        system = code93;
    if (param.contains("CODE128"))
        system = code128;


    pr.printBarCode(system, param1.toInt(), param2.toAscii().data());
    blDebug ( "END BlDbRecord::trataPrintBarCode", 0 );
    return pr.buffer();

}


/// Trata el trataPrintBarCode
/**
\param det Texto de entrada para ser tratado por iteracion.
\return
**/
QByteArray BlDbRecord::trataRightJustified ( const QString &param, const QString &param1,const QString &param2,const QString &param3,int tipoEscape )
{

    blDebug ( "BlDbRecord::trataRightJustified", 0 );
    bool truncate = FALSE;
    if (param3.contains("TRUE"))
        truncate = TRUE;
    if (param3.contains("true"))
        truncate = TRUE;
    if (param3.contains("1"))
        truncate = TRUE;

    blDebug ( "END BlDbRecord::trataRightJustified", 0 );
    return param.rightJustified(param1.toInt(), param2.at(0), truncate).toAscii();
}

/// Trata el trataLeftJustified
/**
\param det Texto de entrada para ser tratado por iteracion.
\return
**/
QByteArray BlDbRecord::trataLeftJustified ( const QString &param, const QString &param1,const QString &param2,const QString &param3,int tipoEscape )
{

    blDebug ( "BlDbRecord::trataLeftJustified", 0 );
    bool truncate = FALSE;
    if (param3.contains("TRUE"))
        truncate = TRUE;
    if (param3.contains("true"))
        truncate = TRUE;
    if (param3.contains("1"))
        truncate = TRUE;

    blDebug ( "END BlDbRecord::trataLeftJustified", 0 );
    return param.leftJustified(param1.toInt(), param2.at(0), truncate).toAscii();
}


/// Trata el setBarCodeWidth
/**
\param det Texto de entrada para ser tratado por iteracion.
\return
**/
QByteArray BlDbRecord::trataSetBarCodeWidth ( const QString &param, int tipoEscape )
{

    int modo=0;
    blDebug ( "BlDbRecord::trataSetBarCodeWidth", 0 );
    BlEscPrinter pr;
    pr.clearBuffer();

    pr.setBarCodeWidth(param.toInt());
    blDebug ( "END BlDbRecord::trataSetBarCodeWidth", 0 );
    return pr.buffer();

}

/// Trata el setBarCodeHeight
/**
\param det Texto de entrada para ser tratado por iteracion.
\return
**/
QByteArray BlDbRecord::trataSetBarCodeHeight ( const QString &param, int tipoEscape )
{

    int modo=0;
    blDebug ( "BlDbRecord::trataSetBarCodeHeight", 0 );
    BlEscPrinter pr;
    pr.clearBuffer();

    pr.setBarCodeHeight(param.toInt());
    blDebug ( "END BlDbRecord::trataSetBarCodeHeight", 0 );
    return pr.buffer();

}

/// Trata el trataSelectPageMode
/**
\param det Texto de entrada para ser tratado por iteracion.
\return
**/
QByteArray BlDbRecord::trataSelectPageMode ( int tipoEscape )
{

    int modo=0;
    blDebug ( "BlDbRecord::trataSelectPageMode", 0 );
    BlEscPrinter pr;
    pr.clearBuffer();

    pr.selectPageMode();
    blDebug ( "END BlDbRecord::trataSelectPageMode", 0 );
    return pr.buffer();

}


/// Trata el trataSetPrintDirection
/**
\param det Texto de entrada para ser tratado por iteracion.
\return
**/
QByteArray BlDbRecord::trataSetPrintDirection ( const QString &param, int tipoEscape )
{

    printDirection direc = leftToRight;
    blDebug ( "BlDbRecord::trataSetPrintDirection", 0 );
    BlEscPrinter pr;
    pr.clearBuffer();

    if (param.contains("LEFTTORIGHT"))
        direc = leftToRight;
    if (param.contains("BOTTOMTOTOP"))
        direc = bottomTotop;
    if (param.contains("RIGHTTOLEFT"))
        direc = rightToLeft;
    if (param.contains("TOPTOBOTTOM"))
        direc = topToBottom;


    pr.setPrintDirection( direc);
    blDebug ( "END BlDbRecord::trataSetPrintDirection", 0 );
    return pr.buffer();

}




/// Trata el trataSetPrintArea
/**
\param det Texto de entrada para ser tratado por iteracion.
\return
**/
QByteArray BlDbRecord::trataSetPrintArea ( const QString &param,  const QString &param1,  const QString &param2,  const QString &param3, int tipoEscape )
{

    int modo=0;
    blDebug ( "BlDbRecord::trataSetPrintArea", 0 );
    BlEscPrinter pr;
    pr.clearBuffer();

    pr.setPrintArea(param.toInt(), param1.toInt(), param2.toInt(), param3.toInt());
    blDebug ( "END BlDbRecord::trataSetPrintArea", 0 );
    return pr.buffer();

}


/// Trata el trataSetVAbsolutePos
/**
\param det Texto de entrada para ser tratado por iteracion.
\return
**/
QByteArray BlDbRecord::trataSetVAbsolutePos ( const QString &param, int tipoEscape )
{

    int modo=0;
    blDebug ( "BlDbRecord::trataSetVAbsolutePos", 0 );
    BlEscPrinter pr;
    pr.clearBuffer();

    pr.setVAbsolutePos(param.toInt());
    blDebug ( "END BlDbRecord::trataSetVAbsolutePos", 0 );
    return pr.buffer();

}

/// Trata el trataSetVRelativePos
/**
\param det Texto de entrada para ser tratado por iteracion.
\return
**/
QByteArray BlDbRecord::trataSetVRelativePos ( const QString &param, int tipoEscape )
{

    int modo=0;
    blDebug ( "BlDbRecord::trataSetVRelativePos", 0 );
    BlEscPrinter pr;
    pr.clearBuffer();

    pr.setVRelativePos(param.toInt());
    blDebug ( "END BlDbRecord::trataSetVRelativePos", 0 );
    return pr.buffer();

}

/// Trata el trataCutPaper
/**
\param det Texto de entrada para ser tratado por iteracion.
\return
**/
QByteArray BlDbRecord::trataCutPaper ( const QString &param, int tipoEscape )
{

    bool modo=FALSE;
    blDebug ( "BlDbRecord::trataCutPaper", 0 );
    BlEscPrinter pr;
    pr.clearBuffer();
    if (param.contains("TRUE"))
        modo = TRUE;
    if (param.contains("true"))
        modo = TRUE;
    if (param.contains("1"))
        modo = TRUE;

    pr.cutPaper(modo);
    blDebug ( "END BlDbRecord::trataCutPaper", 0 );
    return pr.buffer();

}



/// Trata el trataCutPaperAndFeed
/**
\param det Texto de entrada para ser tratado por iteracion.
\return
**/
QByteArray BlDbRecord::trataCutPaperAndFeed ( const QString &param, const QString &param1, int tipoEscape )
{

    bool modo=FALSE;
    blDebug ( "BlDbRecord::trataCutPaperAndFeed", 0 );
    BlEscPrinter pr;
    pr.clearBuffer();
    if (param.contains("TRUE"))
        modo = TRUE;
    if (param.contains("true"))
        modo = TRUE;
    if (param.contains("1"))
        modo = TRUE;

    pr.cutPaperAndFeed(modo, param1.toInt());
    blDebug ( "END BlDbRecord::trataCutPaperAndFeed", 0 );
    return pr.buffer();

}

/// Sustituye valores en el texto pasado como variables por su valor.
/// tipoEscape puede ser 0 --> Sin parseo
///    1 --> ParseoXML
///    2 --> ParseoPython
void BlDbRecord::substrVars ( QByteArray &buff, int tipoEscape )
{

    int pos = 0;

    /// Tratamos la sustitucion de variables de m_variables
    QMapIterator<QString, QString> j ( m_globalvars );
    while ( j.hasNext() ) {
        j.next();
        buff.replace ( ("[" + j.key() + "]").toAscii(), j.value().toAscii() );
    } // end while
    
    
    /// Tratamos la sustitucion de variables de m_variables
    QMapIterator<QString, QString> i ( m_variables );
    while ( i.hasNext() ) {
        i.next();
        buff.replace ( ("[" + i.key() + "]").toAscii(), i.value().toAscii() );
    } // end while


    substrConf ( buff );

    pos =  0;

    /// Buscamos parametros en el query y los ponemos.
    QRegExp rx ( "\\[(\\w*)\\]" );
    while ( ( pos = rx.indexIn ( buff, pos ) ) != -1 ) {
        if ( exists ( rx.cap ( 1 ) ) ) {
                        
            switch ( tipoEscape ) {
            case 1:
                buff.replace ( pos, rx.matchedLength(), blXMLEscape ( dbValue ( rx.cap ( 1 ) ) ).toAscii() );
                break;
            case 2:
                buff.replace ( pos, rx.matchedLength(), blPythonEscape ( dbValue ( rx.cap ( 1 ) ) ).toAscii() );
                break;
            default:
                buff.replace ( pos, rx.matchedLength(), dbValue ( rx.cap ( 1 ) ).toAscii() );

            } // end switch

            pos = 0;
        } else {
            pos += rx.matchedLength();
        }
    } // end while

}



/// Trata las lineas de detalle encontradas dentro de los tags <!--LINEAS DETALLE-->
/**
\param det Texto de entrada para ser tratado por iteracion.
\return
**/
QByteArray BlDbRecord::trataQuery ( const QString &query, const QByteArray &datos, int tipoEscape )
{
    blDebug ( "BlDbRecord::trataQuery", 0 );
    QByteArray result = "";
    QByteArray query1 = query.toAscii();

    /// Buscamos parametros en el query y los ponemos.
    substrVars ( query1, tipoEscape );

    /// Cargamos el query y lo recorremos
    result = trataCursor ( m_dbConnection->loadQuery ( query1 ), datos, tipoEscape );
    blDebug ( "END BlDbRecord::trataQuery", 0 );
    return result;

}

/*
QByteArray BlDbRecord::trataLineasDetalle( const QByteArray &datos, int tipoEscape )
{
    blDebug ( "BlDbRecord::trataLineasDetalle", 0 );
//    blMsgInfo("lineas de detalle");
    QByteArray result = "";
    

    BlDbRecord *linea1;
    for ( int i = 0; i < m_listaLineas->size(); ++i ) {
        linea1 = m_listaLineas->at ( i );
        QByteArray salidatemp = datos;

        /// Buscamos cadenas perdidas adicionales que puedan quedar por poner.
        //blDebug("salidatemp =",0,salidatemp);
        QRegExp rx ( "\\[(\\w*)\\]" );
        int pos =  0;
        while ( ( pos = rx.indexIn ( salidatemp, pos ) ) != -1 ) {
            //blDebug("substituïm ",0,rx.cap(1));
            if ( linea1->exists ( rx.cap ( 1 ) ) ) {
                switch ( tipoEscape ) {
                case 1:
                    salidatemp.replace ( pos, rx.matchedLength(), blXMLEscape ( linea1->dbValue ( rx.cap ( 1 ) )).toAscii()  );
                    break;
                case 2:
                    salidatemp.replace ( pos, rx.matchedLength(), blPythonEscape ( linea1->dbValue ( rx.cap ( 1 ) )).toAscii()  );
                    break;
                default:
                    salidatemp.replace ( pos, rx.matchedLength(), linea1->dbValue ( rx.cap ( 1 )).toAscii() );
                    break;
                } // emd switch
                pos = 0;
            } else {
                pos += rx.matchedLength();
            }
        } // end while

        result += salidatemp;
    } // end for

    blDebug ( "END BlDbRecord::trataLineasDetalle", 0 );
    return result;
}
*/

QByteArray BlDbRecord::trataCursor ( BlDbRecordSet *cur, const QByteArray &datos, int tipoEscape )
{
    blDebug ( "BlDbRecord::trataCursor", 0 );
    QByteArray result = "";
    
    if ( !cur ) {
        blDebug ( "END BlDbRecord::trataCursor", 0 , "cur==NULL" );
        return "";
    };
    while ( !cur->eof() ) {
        QByteArray salidatemp = datos;

        /// Buscamos cadenas perdidas adicionales que puedan quedar por poner.
        //blDebug("salidatemp =",0,salidatemp);
        QRegExp rx ( "\\[(\\w*)\\]" );
        int pos =  0;
        while ( ( pos = rx.indexIn ( salidatemp, pos ) ) != -1 ) {
            //blDebug("substituïm ",0,rx.cap(1));
            if ( cur->numcampo ( rx.cap ( 1 ) ) != -1 ) {
                switch ( tipoEscape ) {
                case 1:
                    salidatemp.replace ( pos, rx.matchedLength(), blXMLEscape ( cur->valor ( rx.cap ( 1 ), -1, TRUE ) ).toAscii()  );
                    break;
                case 2:
                    salidatemp.replace ( pos, rx.matchedLength(), blPythonEscape ( cur->valor ( rx.cap ( 1 ), -1, TRUE ) ).toAscii()  );
                    break;
                default:
                    salidatemp.replace ( pos, rx.matchedLength(), cur->valor ( rx.cap ( 1 ), -1, TRUE ).toAscii() );
                    break;
                } // emd switch
                pos = 0;
            } else {
                pos += rx.matchedLength();
            }
        } // end while

        result += salidatemp;
        cur->nextRecord();
    } // end while
    delete cur;
    blDebug ( "END BlDbRecord::trataCursor", 0 );
    return result;
}


/// Trata las lineas de detalle encontradas dentro de los tags <!--LINEAS DETALLE-->
/**
\param det Texto de entrada para ser tratado por iteracion.
\return Si el query tiene elementos lo devuelve el parametro. En caso contrario no devuelve nada.
**/
QByteArray BlDbRecord::trataExists ( const QString &query, const QByteArray &datos )
{
    blDebug ( "BlDbRecord::trataExists", 0 );

    QByteArray result = "";
    QByteArray query1 = query.toAscii();

    /// Buscamos parametros en el query y los ponemos.
    substrVars ( query1 );

    QFile file ( query1 );
    if ( file.exists() )
        result = datos;
    blDebug ( "END BlDbRecord::trataExists", 0 );

    return result;
}


/// Permite que el programa introduzca variables de impresion propias sin tener 
/// Que introducir datos en el registro de base de datos de ficha.
void BlDbRecord::setVar(const QString &varname, const QString &varvalue) {
          m_globalvars[varname ] = varvalue;
}

/// Permite que el programa introduzca variables de impresion propias sin tener 
/// Que introducir datos en el registro de base de datos de ficha.
void BlDbRecord::clearVars() {
          m_globalvars.clear();
}