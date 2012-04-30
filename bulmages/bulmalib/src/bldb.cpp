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
#include "bluiloader.h"
#include <QPushButton>

#include "bldb.h"
#include "blplugins.h"
#include "local_blI18n.h"
#include "blescprinter.h"
#include "blsearchwidget.h"
#include "bldatesearch.h"

///
/**
**/
bool BlDbField::isFieldChanged()
{
    BL_FUNC_DEBUG
    
    return m_fieldValue != m_fieldPreviousValue;
}


///
/**
**/
void BlDbField::resetCambio()
{
    BL_FUNC_DEBUG
    m_fieldPreviousValue = m_fieldValue;
    
}


///
/**
**/
BlDbField::~BlDbField()
{
    BL_FUNC_DEBUG
    
}


///
/**
\return
**/
BlPostgreSqlClient *BlDbField::dbConnection()
{
    BL_FUNC_DEBUG
    
    return m_dbConnection;
}


///
/**
\param comp
**/
void BlDbField::setDbConnection ( BlPostgreSqlClient *dbClient )
{
    BL_FUNC_DEBUG
    m_dbConnection = dbClient;
    
}


///
/**
\return
**/
BlDbField::DbType BlDbField::fieldType()
{
    BL_FUNC_DEBUG
    return m_fieldType;
}


///
/**
\param val
\return
**/
int BlDbField::set ( QString val )
{
    BL_FUNC_DEBUG
    m_fieldValue = val;
    
    return 0;
}


///
/**
\return
**/
int BlDbField::fieldRestrictions()
{
    BL_FUNC_DEBUG
    
    return m_fieldRestrictions;
}


///
/**
\return
**/
QString BlDbField::fieldName()
{
    BL_FUNC_DEBUG
    
    return m_fieldName;
}


///
/**
\return
**/
QString BlDbField::fieldTitle()
{
    BL_FUNC_DEBUG
    
    return m_fieldTitle;
}


///
/**
\return
**/
QString BlDbField::fieldValue()
{
    BL_FUNC_DEBUG
    QString valor = m_fieldValue;
    if ( m_fieldType == DbNumeric ) {
        QLocale locale;
        if ( locale.decimalPoint() == '.' ) {
            valor.replace ( ",", locale.decimalPoint () );
        } else {
            valor.replace ( ".", locale.decimalPoint () );
        } // end if
    } // end if
    
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
BlDbField::BlDbField ( BlPostgreSqlClient *dbConnection, QString fieldName, DbType fieldType, int fieldRestrictions, QString fieldTitle )
{
    BL_FUNC_DEBUG
    setDbConnection(dbConnection);
    m_fieldName = fieldName;
    m_fieldType = fieldType;
    m_fieldRestrictions = fieldRestrictions;
    m_fieldTitle = fieldTitle;
    m_fieldValue = "";
    m_fieldPreviousValue = "";
    
}


/// Devuelve el valor del campo preparado para ser insertado en la base de datos.
/**
\param error
\return
**/
QString BlDbField::valorcampoprep ( int &error )
{
    BL_FUNC_DEBUG
    error = 0;
    QString valor = "";
    if ( ( m_fieldRestrictions & DbNotNull ) && ! ( m_fieldRestrictions & DbAuto ) ) {
        if ( m_fieldValue == "" ) {
            blMsgWarning ( "El campo '" + m_fieldTitle + "' no puede estar vacio." );
            error = -20200;
            return valor;
        } // end if
    } // end if

    switch ( m_fieldType ) {
    case DbInt:
        if ( m_fieldValue == "" ) {
            valor = "NULL";
        } else {
            m_fieldValue.replace ( ",", "." );
            valor = "'" + m_dbConnection->sanearCadena ( m_fieldValue ) + "'";
        } // end if
        break;
    case DbVarChar:
        if ( m_fieldValue == "" ) {
            valor = "NULL";
        } else {
            valor = "'" + m_dbConnection->sanearCadena ( m_fieldValue ) + "'";
        } // end if
        break;
    case DbDate:
        if ( m_fieldValue == "" ) {
            valor = "NULL";
        } else {
            valor = "'" + m_dbConnection->sanearCadena ( m_fieldValue ) + "'";
        } // end if
        break;
    case DbNumeric:
        if ( m_fieldValue == "" ) {
            valor = "NULL";
        } else {
            m_fieldValue.replace ( ",", "." );
            valor =  "'" + m_dbConnection->sanearCadena ( m_fieldValue ) + "'";
        } // end if
        break;
    case DbBoolean:
        if ( m_fieldValue == "" ) {
            valor = "NULL";
        } else if ( m_fieldValue == "f" || m_fieldValue == "t" ) {
            valor = "'" + m_dbConnection->sanearCadena ( m_fieldValue ) + "'";
        } else {
            valor = m_dbConnection->sanearCadena ( m_fieldValue );
        } // end if
        break;
    case DbTime:
        if ( m_fieldValue == "" ) {
            valor = "NULL";
        } else {
            valor = "'" + m_dbConnection->sanearCadena ( m_fieldValue ) + "'";
        } // end if
        break;
    default:
        error = -1;
    } // end switch

    
    return valor;
}


/// Hace la exportacion del campo a XML
QString BlDbField::exportXML() {
    BL_FUNC_DEBUG
    QString val;
    int error;

    val = "<BLDBFIELD>\n";
    val += "\t<NOMCAMPO>" + blXMLEncode(m_fieldName) + "</NOMCAMPO>\n";
    val += "\t<VALORCAMPO>"+ blXMLEncode(m_fieldValue)+"</VALORCAMPO>\n";
    val += "\t<VALORCAMPOORIG>"+ blXMLEncode(m_fieldPreviousValue)+"</VALORCAMPOORIG>\n";
    if (m_fieldValue != "") {
        val += "\t<VALORCAMPOPREP>"+ blXMLEncode(valorcampoprep(error))+"</VALORCAMPOPREP>\n";
    } // end if
    val += "\t<NOMPRESENTACION>"+ blXMLEncode(m_fieldTitle)+"</NOMPRESENTACION>\n";
    val += "\t<RESTRICCIONES>"+ blXMLEncode(QString::number(m_fieldRestrictions))+"</RESTRICCIONES>\n";
    val += "\t<TIPO>"+ blXMLEncode(QString::number(m_fieldType))+"</TIPO>\n";
    val += "</BLDBFIELD>\n";
    return val;
    
}





void BlDbField::syncXML(const QString &text) {
    BL_FUNC_DEBUG

    QDomDocument doc ( "mydocument" );

    if ( !doc.setContent ( text ) )  return;


    QDomElement docElem = doc.documentElement();
    QDomElement principal = docElem.firstChildElement ( "NOMCAMPO" );
    if (principal.text() == m_fieldName) {
        principal = docElem.firstChildElement ( "VALORCAMPO" );
        m_fieldValue = principal.text();
    } // end if

}


/// Constructor de la clase que toma por defecto la base de datos con la que va a operar.
/**
\param con Puntero a la base de datos con la que vamos a trabajar.
**/
BlDbRecord::BlDbRecord ( BlMainCompany *company )
{
    BL_FUNC_DEBUG
    m_dbConnection = company;
    m_nuevoCampo = TRUE;
    m_tableName = "";
    
}


/// Destructor de la clase
/**
Limpia y libera toda la memoria que se haya podido reservar.
Esta operacion no contempla que haya habido cambios en el registro sin guardar.
**/
BlDbRecord::~BlDbRecord()
{
    BL_FUNC_DEBUG
    while (!m_lista.isEmpty())
	delete m_lista.takeFirst();
}

/// Establece la base de datos que debe utilizar la clase.
/**
Esta clase funciona siempre ligada a una base de datos. Esta se especifica
en el constructor de la clase.
\param comp Puntero a la base de datos que debe utilizar la clase.
**/
void BlDbRecord::setDbConnection ( BlMainCompany *company )
{
    BL_FUNC_DEBUG
    m_dbConnection = company;
}


/// Devuelve el puntero de la base de datos con la que ha sido inicializada la clase.
/**
\return Puntero a la base de datos con la que se ha inicializado la clase.
**/
BlMainCompany *BlDbRecord::dbConnection()
{
    BL_FUNC_DEBUG
    
    return m_dbConnection;
}


/// Establece cual es la tabla por defecto que se va a usar para hacer las operaciones.
/**
Esta clase funciona siempre con una tabla especificada. De esta forma todas las cargas, inserciones
y modificaciones se hacen sobre la tabla que se haya especificado.
\param nom Nombre de la tabla de la base de datos que se usa por defecto.
**/
void BlDbRecord::setDbTableName ( QString tableName )
{
    BL_FUNC_DEBUG
    m_tableName = tableName;
    
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
    BL_FUNC_DEBUG
    m_nuevoCampo = n;
    
}


/// Devuelve el nombre de la tabla de la base de datos con la que se opera por defecto
/**
\return El nombre de la tabla que usa la clase al  operar con la base de datos
**/
QString BlDbRecord::tableName()
{
    BL_FUNC_DEBUG
    
    return m_tableName;
}


/// Devuelve el nombre del campo clave primaria en la tabla por defecto.
/**
\return El nombre del campo que se considera Identificador. (Clave Primaria)
**/
QString BlDbRecord::fieldId()
{
    BL_FUNC_DEBUG
    if (m_campoid.isEmpty()) {
      if ( exists ( "id" + m_tableName ) ) {
	  m_campoid = "id" + m_tableName;
      } // end if
    } // end if
    if (m_campoid.isEmpty() ) {

       if(exists ( "num" + m_tableName )) {
	  m_campoid = "num" + m_tableName;
	} // end if
    } // end if
    
    return m_campoid;
}


/// Establece el nombre del campo identificador en la tabla por defecto.
/**
\param nom El nombre del campo que se considera la clave primaria
**/
void BlDbRecord::setDbFieldId ( QString nom )
{
    BL_FUNC_DEBUG
    m_campoid = nom;
    
}


/// Devuelve la lista de campos con los que esta operando la clase.
/**
Normalmente coincide esta lista con la definicion de la tabla con la que opera.
\return Lista de campos con los que se esta operando.
**/
QList<BlDbField *> *BlDbRecord::lista()
{
    BL_FUNC_DEBUG
    
    return &m_lista;
}


/// Vacia la clase para que no tenga parametros por defecto.
/**
**/
void BlDbRecord::vaciar()
{
    BL_FUNC_DEBUG
    DBclear();
}



/// Efectura la carga de un Registro de la base de datos a partir de un recorset (\class BlDbRecordSet).
/**
\param cur recordset con resultados.
\return Si no hay errores devuelve 0. En caso contrario devuelve otro valor.
**/
int BlDbRecord::DBload ( BlDbRecordSet *cur )
{
    BL_FUNC_DEBUG
    try {
        m_nuevoCampo = FALSE;

        BlDbField *campo;
        int error = 0;
        for ( int i = 0; i < m_lista.size(); ++i ) {
            campo = m_lista.at ( i );
            if ( ! ( campo->fieldRestrictions() & BlDbField::DbNoLoad ) ) {
                QString nom = campo->fieldName();
                QString val = cur->value( nom );
                if ( ( campo->fieldRestrictions() & BlDbField::DbPrimaryKey ) && ( val == "" ) )
                    m_nuevoCampo = TRUE;
                if ( ( campo->fieldRestrictions() & BlDbField::DbDupPrimaryKey ) && ( val == "" ) )
                    m_nuevoCampo = TRUE;
                error += campo->set ( val );
                /// Al ser una carga consideramos que los cambios estan inicializados.
                campo->resetCambio();
            } // end if
        } // end for
        
        return error;
    } catch ( ... ) {
        
        return -1;
    } // end try
}


/// Inicializa la clase limpiando posibles componentes utilizados con anterioridad.
/**
**/
void BlDbRecord::DBclear()
{
    BL_FUNC_DEBUG
    m_nuevoCampo = TRUE;
    BlDbField *campo;
    for ( int i = 0; i < m_lista.size(); ++i ) {
        campo = m_lista.at ( i );
        campo->set ( "" );
    } // end for
    
}


/// Guarda el registro actual contenido en la clase en la base de datos.
/**
\param id Devuelve el identificador (primary key) con que ha quedado guardado el registro.
**/
int BlDbRecord::dbSave ( QString &id )
{
    BL_FUNC_DEBUG
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
            if ( campo->fieldRestrictions() & BlDbField::DbDupPrimaryKey ) {
                QString lin = campo->valorcampoprep ( err );
                if ( err )
                    throw ( err );
                querywhere += separadorwhere + campo->fieldTitle() + " = " + lin;
                separadorwhere = " AND ";
            } // end if

            /// Si el campo es requerido y no esta entonces salimos sin dar error.
            /// No es lo mismo que los not null ya que estos si dan error
            if ( ! ( campo->fieldRestrictions() & BlDbField::DbNoSave ) ) {
                if ( campo->fieldRestrictions() & BlDbField::DbRequired ) {
                    if ( campo->fieldValue() == "" ) {
		        
                        return 0;
                    } // end if
                } // end if
                if ( campo->fieldRestrictions() & BlDbField::DbPrimaryKey ) {
                    QString lin = campo->valorcampoprep ( err );
                    if ( err )
                        throw ( err );
                    querywhere += separadorwhere + campo->fieldName() + " = " + lin;
                    separadorwhere = " AND ";
                } // end if
                if ( campo->valorcampoprep ( err ) != "" ) {
                    queryupdate += separador1 + campo->fieldName() + "=" + campo->valorcampoprep ( err );
                    separador1 = ", ";
                } // end if
                if ( err )
                    throw ( err );
                if ( ( campo->valorcampoprep ( err ) != "NULL" ) && ( campo->valorcampoprep ( err ) != "" ) ) {
                    listcampos += separador + campo->fieldName();
                    listvalores += separador + campo->valorcampoprep ( err );
                    if ( err )
                        throw ( err );
                    separador = ", ";
                } // end if
                /// Si es el ID entonces lo asignamos porque ya tiene el valor correspondiente.
                if ( m_campoid == campo->fieldName() ) {
                    id = campo->fieldValue();
                } // end if
            } // end if
        } // end for
        if ( m_nuevoCampo ) {
            QString query = "INSERT INTO " + m_tableName + " (" + listcampos + ") VALUES (" + listvalores + ")";
            m_dbConnection->runQuery ( query );
            BlDebug::blDebug ( Q_FUNC_INFO, 0, QString(_("Consulta: '%1'.")).arg(query) );
            BlDbRecordSet *cur = m_dbConnection->loadQuery ( "SELECT " + m_campoid + " FROM " + m_tableName + " ORDER BY " + m_campoid + " DESC LIMIT 1" );
            id = cur->value( m_campoid );
            delete cur;
        } else {
            QString query = "UPDATE " + m_tableName + " SET " + queryupdate + " WHERE " + querywhere;
            BlDebug::blDebug ( Q_FUNC_INFO, 0, QString(_("Consulta: '%1'.")).arg(query) );
            m_dbConnection->runQuery ( query );
        } // end if
        m_nuevoCampo = FALSE;

        /// Emitimos la se&ntilde;al apropiada en el BlApplication.
        g_theApp->emitDbTableChanged ( m_tableName );

    } catch ( int error ) {
        
        throw error;
    } // end try
    
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
    BL_FUNC_DEBUG
    BlDebug::blDebug ( Q_FUNC_INFO, 0, nomb + " --> " + valor );
    
    BlDbField *campo;
    int error = 0;
    int i = 0;
    campo = m_lista.value ( i );

    while ( campo && campo->fieldName() != nomb ) {
        campo = m_lista.value ( ++i ) ;
    } // end while

    if ( !campo ) {
	BlDebug::blDebug ( Q_FUNC_INFO, 0, QString(_("Campo: '%1' no encontrado.")).arg(nomb) );
	blMsgError(QString(_("Campo: '%1' no encontrado.")).arg(nomb));
        return -1;
    } // end if

    if ( campo->fieldName() == nomb ) {
        error = campo->set ( valor );
    } // end if

    
    return error;
}


/// Autonumeraci&oacute;n: establece el valor que sigue al ú&uacute;timo del campo en la tabla
/**
Si el campo no es num&eacute;rico ni fecha lo dejamos en blanco.
Usamos 1 como primer valor en los campos num&eacute;ricos. En el resto de tipos, el primer valor es en blanco.
Esta funci&oacute;n puede usarse con alg&uacute;n c&oacute;digo autonum&eacute;rico o fecha adicional
a parte del identificador que ya autonumera la base de datos.
\param nomb Nombre del campo
\param vinc Valor del incremento (0 copia el &uacute;ltimo valor)
\param cond Condición del WHERE, por defecto no hay filto
\return código de error de setDbValue
**/
int BlDbRecord::setDbValueNextToLast ( QString nomb, int vinc, QString cond )
{
    BL_FUNC_DEBUG

    /// Buscar el máximo valor del campo
    QString query = QString ( "SELECT MAX(%1) AS maxv FROM %2" ).arg ( nomb ).arg ( tableName() );
    if ( !cond.isEmpty() ) {
        query += " WHERE ";
        query += cond;
    } // end if
    BlDbRecordSet *reg = m_dbConnection->loadQuery ( query );
    QString maxv;
    if ( !reg->eof() )
        maxv = reg->value ( "maxv" );

    /// Conseguir el tipo del campo
    int tipo = -1;
    for ( int i = 0; i < m_lista.size() && tipo == -1; ++i)
        if ( m_lista.at(i)->fieldName() == nomb )
            tipo = m_lista.at(i)->fieldType();

    /// Calcular el siguiente valor si el tipo de campo lo permite
    int vini = 1;
    QString next;
    switch ( tipo ) {
    case BlDbField::DbInt:
        if ( maxv.isEmpty() )
            next = QString("%1").arg ( vini );
        else
            next = QString ( "%1" ).arg ( maxv.toInt() + vinc );
        break;
    case BlDbField::DbVarChar:
        blMsgError ( _("No se puede obtener el siguiente al ultimo en un campo de texto.") );
        break;
    case BlDbField::DbDate:
        if ( !maxv.isEmpty() ) {
            QDate fecha( maxv.section("/", 2, 2 ).toInt(), maxv.section(  "/", 1, 1 ).toInt(), maxv.section( "/", 0, 0 ).toInt() );
            next = fecha.addDays(vinc).toString ( "dd/MM/yyyy" );
        } // end if
        break;
    case BlDbField::DbNumeric:
        if ( maxv.isEmpty() )
            next = QString("%1").arg ( 0.0 + vini );
        else
            next = QString ( "%1" ).arg ( maxv.toDouble() + vinc );
        break;
    case BlDbField::DbBoolean:
        blMsgError ( _("No se puede obtener el siguiente al ultimo en un campo booleano.") );
        break;
    case BlDbField::DbTime:
	// TODO
        blMsgError ( _("No se puede obtener el siguiente al ultimo en un campo hora.") );
        break;
    default:
        blMsgError ( _("No se pudo determinar el tipo de campo para calcular el siguiente al ultimo.") );
    } // end switch

    return setDbValue ( nomb, next );
}


/// Consulta el valor que tiene un campo del registro actual
/**
\param nomb Nombre del campo que queremos consultar.
\return Valor que contiene el campo.
**/
QString BlDbRecord::dbValue ( QString nomb )
{
    BL_FUNC_DEBUG
    BlDbField *campo;
    QString valor = "";
    int i = 0;

    /// Recorremos la lista en busqueda del campo.
    campo = m_lista.value ( i );
    while ( campo && campo->fieldName() != nomb )
        campo = m_lista.value ( ++i );

    if ( !campo ) {
	BlDebug::blDebug ( Q_FUNC_INFO, 0, QString(_("Campo: '%1' no encontrado.")).arg(nomb) );
    } else if ( campo->fieldName() == nomb ) {
        valor = campo->fieldValue();
    } // end if
    
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
    BL_FUNC_DEBUG
    BlDbField *campo;
    bool existe = FALSE;
    int i = 0;

    if (!m_lista.isEmpty()) {
      campo = m_lista.value ( i );
      if (campo) {
	while (i < m_lista.size() && campo && campo->fieldName() != nomb ) {
	    campo = m_lista.value ( ++i );
	} // end while
	if ( campo ) {
	    if ( campo->fieldName() == nomb ) {
		existe = TRUE;
	    } // end if
	} // end if
      } // end if
    } // end if
 
    
    return existe;
}


/// Prepara un campo para un eventual almacenado en la base de datos.
/**
\param nomb campo a preparar para ser guardado
\return El valor del campo una vez preprocesado para el guardado.
**/
QString BlDbRecord::dbValueprep ( QString nomb )
{
    BL_FUNC_DEBUG
    BlDbField *campo;
    int i = 0;
    campo = m_lista.value ( i );

    while ( campo && campo->fieldName() != nomb ) {
        campo = m_lista.value ( ++i );
    } // end while

    if ( !campo ) {
        blMsgWarning ( "No se ha encontrado el campo '" + nomb + "'." );
        return "";
    } // end if

    if ( campo->fieldName() == nomb ) {
        int err;
        return campo->valorcampoprep ( err );
    } // end if

    
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
    BL_FUNC_DEBUG

    try {

        BlDbField *camp = new BlDbField ( m_dbConnection, nom, typ, res, nomp );
        camp->set( "" );
        m_lista.append ( camp );
	
        return 0;

    } catch(...) {
	BlDebug::blDebug ( Q_FUNC_INFO, 0, _("Error en la insercion del campo al registro.") );
	blMsgError(_("Error en la insercion del campo al registro."));
    } // end try

    
}


/// Elimina el registro que tenemos de la base de dato (Si es que existe)
/**
\return si no se han producido errores devuelve 0 en caso contrario devuelve -1
**/
int BlDbRecord::remove()
{
    BL_FUNC_DEBUG
    try {
        BlDbField *campo;
        QString separadorwhere = "";
        QString querywhere = "";

        for ( int i = 0; i < m_lista.size(); ++i ) {
            campo = m_lista.at ( i );
            if ( campo->fieldRestrictions() & BlDbField::DbDupPrimaryKey ) {
                int err;
                QString lin = campo->valorcampoprep ( err );
                if ( err )
                    throw - 1;
                querywhere += separadorwhere + campo->fieldTitle() + " = " + lin;
                separadorwhere = " AND ";
            } // end if

            if ( ! ( campo->fieldRestrictions() & BlDbField::DbNoSave ) ) {
                if ( campo->fieldRestrictions() & BlDbField::DbPrimaryKey ) {
                    int err;
                    QString lin = campo->valorcampoprep ( err );
                    if ( err )
                        throw - 1;
                    querywhere += separadorwhere + campo->fieldName() + " = " + lin;
                    separadorwhere = " AND ";
                } // end if
            } // end if
        } // end for

        if ( m_nuevoCampo == FALSE ) {
            m_dbConnection->runQuery ( "DELETE FROM " + m_tableName + " WHERE " + querywhere );
        } // end if

        
        return 0;
    } catch ( ... ) {
	BlDebug::blDebug ( Q_FUNC_INFO, 0, _("Error al borrar") );
	blMsgError(_("Error al borrar"));
        throw - 1;
    }
}


/// Guarda el registro actual en la base de datos
/**
Esta funcion, de un nivel algo superior a la llamada dbSave hace el guardado y maneja las
excepciones que se hayan podido producir
\return si no se producen errores devuelve 0 en caso contrario genera una excepcion
**/
int BlDbRecord::save()
{
    BL_FUNC_DEBUG
    QString id;
    try {
        dbSave ( id );
        setDbValue ( m_campoid, id );
        
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
int BlDbRecord::load ( QString id )
{
    BL_FUNC_DEBUG
    try {
        QString query = "SELECT * FROM " + m_tableName + " WHERE " + m_campoid + " = " + id;
        BlDbRecordSet *cur = m_dbConnection->loadQuery ( query );
        if ( !cur ) throw - 1;
        if ( !cur->eof() ) {
            DBload ( cur );
        } // end if
        delete cur;
        
        return 0;
    } catch ( ... ) {
        blMsgError ( "BlDbRecord::cargar Se ha producido un error al cargar el registro." );
        return -1;
    } // end try
}

void BlDbRecord::substrConf ( QString &buff )
{
    BL_FUNC_DEBUG
    ///\TODO: Este tratamiento esta repetido en BlForm::parseTags y en PedidoProveedorView::imprimir.
    ///       Se puede simplificar?
    /// Tratamos la sustitucion de los valores de configuracion.
    for ( int i = 0; i < 1000; i++ ) {
        if ( g_confpr->name( i ) != "" ) {
            buff.replace ( "[" + g_confpr->name( i ) + "]", g_confpr->value( i ) );
        } // end if
    } // end for
}

void BlDbRecord::substrConf ( QByteArray &buff )
{
    ///\TODO: Este tratamiento esta repetido en BlForm::parseTags y en PedidoProveedorView::imprimir.
    ///       Se puede simplificar?
    /// Tratamos la sustitucion de los valores de configuracion.
    for ( int i = 0; i < 1000; i++ ) {
        if ( g_confpr->name( i ) != "" ) {
            buff.replace ( QString("[" + g_confpr->name( i ) + "]").toAscii(), g_confpr->value( i ).toAscii() );
        } // end if
    } // end for
}

int BlDbRecord::parseTags ( QString &buff, int tipoEscape )
{
    QString fitxersortidatxt = "";

    substrConf ( buff );
    buff.replace ( "[ficha]", m_tableName );
    buff.replace ( "[story]", story() );

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
        fitxersortidatxt += "   <td>" + blXMLEscape ( campo->fieldName() ) + "</td>\n";
        fitxersortidatxt += "   <td>" + blXMLEscape ( campo->fieldTitle() ) + "</td>\n";
        if ( campo->fieldType() & BlDbField::DbNumeric )
            fitxersortidatxt += "   <td>" + blXMLEscape ( spanish.toString ( campo->fieldValue().toDouble(), 'f', 2 ) ) + "</td>\n";
        else
            fitxersortidatxt += "   <td>" + blXMLEscape ( campo->fieldValue() ) + "</td>\n";
        fitxersortidatxt += "</tr>";
        qDebug() << spanish.toString ( campo->fieldValue().toDouble(), 'f', 2 );
    } // end for

    return fitxersortidatxt;
}

int BlDbRecord::generateRML ( const QString &arch )
{
    BL_FUNC_DEBUG
    /// Disparamos los plugins
    int res = g_plugins->run ( "BlDbRecord_generateRML", this );
    if ( res != 0 ) {
        return 1;
    } // end if

    /// Dependiendo del tipo de archivo que se trate usaremos un parseo u otro.
    int tipoescape = 0;
    if ( arch.right ( 3 ) == "rml" || arch.right ( 3 ) == "xml" )
        tipoescape = 1;
    else if ( arch.right ( 3 ) == "pys" )
        tipoescape = 2;

    /// Copiamos el archivo.
    QString archivo = g_confpr->value( CONF_DIR_OPENREPORTS ) + arch;
    QString archivod = g_confpr->value( CONF_DIR_USER ) + arch;
    if(!blCopyFile(archivo, archivod)){
        blMsgError(_("Error al copiar el archivo RML [ bldb->generateRML() ]"));
    } // end if


    /// Copiamos el logo
    QString ownlogo = g_confpr->value( CONF_DIR_USER ) + "logo.jpg";
    QString archivologo = g_confpr->value( CONF_DIR_OPENREPORTS ) + "logo.jpg";
    if(!blCopyFile(archivologo, ownlogo)){
        blMsgError(_("Error al copiar el archivo de logo [ bldb->generateRML() ]"));
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
        BlDebug::blDebug ( "El fitxer " + archivod + " te una codificacio (" +
                  xmlReader.documentEncoding().toString() + ") ", 0 );
        codec = QTextCodec::codecForName ( xmlReader.documentEncoding().toString().toUtf8() );
        if ( !codec ) { // sembla que no va, per UTF32 (=UCS4)
            BlDebug::blDebug ( "El fitxer " + archivod + " te una codificacio (" +
                      xmlReader.documentEncoding().toString() + ") que no entenem", 0 );
//             file.close();
//             return 0;
            codec = QTextCodec::codecForName ( "UTF-8" );
        } else {
            BlDebug::blDebug ( "Usarem un codec per a ", 0, codec->name() );
        }// end if
    } else {
	ascii = xmlReader.hasError();
        if ( ascii ) {
            BlDebug::blDebug ( "El fitxer " + archivod + " no l'hem pogut llegir be i no sabem quina codificacio te. S'imaginarem que es ASCII per si de cas" );
        }// end if
        BlDebug::blDebug ( "El fitxer " + archivod + " no se quina codificacio te. Deu ser UTF-8", 0 );
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
    if ( !parseTags ( buff, tipoescape ) ) {
        return 0;
    } // end if

    if ( file.open ( QIODevice::WriteOnly ) ) {
        QTextStream stream ( &file );
        stream.setCodec ( codec );
        if ( ( !ascii ) && ( codec->canEncode ( buff ) ) ) {
            // para ficheros UTF-8, UTF-16, UTF-32 asi vale
            // para otros sin caracteres ajenos al encoding tambien vale
            BlDebug::blDebug ( "Llistat sense referències de caracters", 0, " " );
            stream << buff;
        } else { // para otros con caracteres no codificables
            // tenemos que usar referencias numericas de caracteres de XML
            BlDebug::blDebug ( "Llistat amb referències de caracters", 0, " " );
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
                    BlDebug::blDebug ( ( QString ) "escric " + *i + " com " + codepoint, 0 );
                    stream << "&#" << codepoint << ";" ;
                } // end if
            } // end for
            
        }
        file.close();
    } // end if

} else {
    QByteArray buff = file.readAll();
    file.close();

    /// Hacemos el tratamiento avanzado de TAGS
    if ( !parseTags ( buff, tipoescape ) ) {
        return 0;
    } // end if

    if ( file.open ( QIODevice::WriteOnly ) ) {
        file.write(buff);
        file.close();
    } // end if
} // end if
    
    return 1;
}

QString BlDbRecord::templateName ( void ) {
    BL_FUNC_DEBUG
	  QFile f( g_confpr->value( CONF_DIR_OPENREPORTS ) + tableName() + ".rml");
	  if (f.exists()) {
	      return ( tableName() );
	  } // end if
    return QString ( "ficha" );
}


int BlDbRecord::generateRML ( void ) {
    BL_FUNC_DEBUG
    return generateRML ( templateName() + ".rml" );
}

/// Realiza una impresion generica del registro a partir de la plantilla ficha.rml
/**
**/
void BlDbRecord::imprimir() {
    BL_FUNC_DEBUG
    /// Usa la plantilla template y si esta no existe usa la plantilla ficha  para realizar la impresion.
    /// De esta forma evitamos que al darle a la opcion imprimir Ficha, aparezca un error al no encontrarse el archivo correspondiente a la ficha.
    BlDebug::blDebug ( "BlDbRecord::imprimir", 0, templateName() );
    if ( generateRML() ) {
      QFile f( g_confpr->value( CONF_DIR_OPENREPORTS ) + templateName() + ".rml");
      if (f.exists()) {
	  blCreateAndLoadPDF ( templateName() );
      } // end if
    } // end if
}




/// Hace la exportacion del campo a XML
QString BlDbRecord::exportXML() {
    BL_FUNC_DEBUG
    QString val;
    int error;
    BlDbField *campo;

    val = "<BLDBRECORD>\n";
    val += "\t<TABLENAME>" + blXMLEncode(m_tableName) + "</TABLENAME>\n";
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
    
}

void BlDbRecord::syncXML(const QString &text) {
    BL_FUNC_DEBUG

    BlDbField *campo;
    QDomDocument doc ( "mydocument" );

    if ( !doc.setContent ( text ) ) {
        
        return;
    } // end if


    QDomElement docElem = doc.documentElement();
    QDomElement principal = docElem.firstChildElement ( "TABLENAME" );
    m_tableName = principal.text();
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
}

// =================== GENERAR RML PARA TICKETS y TXT


/// Busca strings del tipo [xxxx] entro del texto pasado y los sustituye
/// Por valores existentes en la base de datos.
/// Tambien busca los parametros PARAM e IFACE para tambien tratarlos.
/// Devuelve 1 Si todo esta OK o 0 Si hay algun error
/**
\param buff El texto entero sobre el que se hace el reemplazo de sentencias.
**/
int BlDbRecord::parseTags ( QByteArray &buff, int tipoEscape )
{
    BL_FUNC_DEBUG


    substrConf ( buff );
    buff.replace ( "[ficha]", m_tableName.toAscii() );
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
            BlUiLoader loader(m_dbConnection);
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

	    
	    /// Vamos a buscar parametros tipo PARAM para configurar el dialogo.
	    QRegExp rx700 ( "<!--\\s*PARAM\\s*NAME\\s*=\\s*\"([^\"]*)\"\\s*TYPE\\s*=\\s*\"([^\"]*)\"\\s*LABEL\\s*=\\s*\"([^\"]*)\"\\s*TABLE\\s*=\\s*\"([^\"]*)\"\\s*TABLEID\\s*=\\s*\"([^\"]*)\"\\s*VALUES\\s*=\\s*\"([^\"]*)\"\\s*-->" );
	    rx700.setMinimal ( TRUE );
	    int pos1 = 0;
	    while ( ( pos1 = rx700.indexIn ( buff, pos1 ) ) != -1 ) {
		QString name = rx700.cap ( 1 );
		QString type = rx700.cap ( 2 );
		QString label = rx700.cap( 3 );
		QString tablename = rx700.cap ( 4 );
		QString fieldid = rx700.cap ( 5 );
		QString values = rx700.cap ( 6 );

			BlSearchWidget * search = iface->findChild<BlSearchWidget *>("mui_" + name);

			
			if (search) {
			    // Para no liarla parda, ponemos provincias y asi no habrá pedatas de momento.
			    search->setLabel ( label );
			    search->setTableName ( tablename );
			    search->setFieldId ( fieldid );
			    QStringList listvalues = values.split(",");
			    for ( int j = 0; j < listvalues.size(); ++j ) {
				search->m_valores[listvalues.at(j)] = "";
			    } // end for
			    search->setId ( "" );
			} // end if
			pos1 += rx700.matchedLength();
	      } // end while
	    
	      /// Vamos a buscar parametros tipo PARAM BLCOMBOBOX para configurar el dialogo.
	      QRegExp rx701 ( "<!--\\s*PARAM\\s*NAME\\s*=\\s*\"([^\"]*)\"\\s*TYPE\\s*=\\s*\"([^\"]*)\"\\s*QUERY\\s*=\\s*\"([^\"]*)\"\\s*TABLEID\\s*=\\s*\"([^\"]*)\"\\s*VALUES\\s*=\\s*\"([^\"]*)\"\\s*-->" );
	      rx701.setMinimal ( TRUE );
	      int pos2 = 0;
	      while ( ( pos2 = rx701.indexIn ( buff, pos2 ) ) != -1 ) {
		  QString name = rx701.cap ( 1 );
		  QString type = rx701.cap ( 2 );
		  QString query = rx701.cap ( 3 );
		  QString fieldid = rx701.cap ( 4 );
		  QString values = rx701.cap ( 5 );
			  BlComboBox * search = iface->findChild<BlComboBox *>("mui_" + name);
			  if (search) {
			      search->setQuery ( query );
			      search->setFieldId ( fieldid );
			      QStringList listvalues = values.split(",");
			      for ( int j = 0; j < listvalues.size(); ++j ) {
				  search->m_valores[listvalues.at(j)] = "";
			      } // end for
			      search->setId ( "" );
			  } // end if
			  pos2 += rx701.matchedLength();
		} // end while
	      
	      
            int ret = diag->exec();
            if ( ret ) {

        	/// Recorre los BlDateSearch.
                QList<BlDateSearch *> dl2 = iface->findChildren<BlDateSearch *>();
                QListIterator<BlDateSearch *> dit2 ( dl2 );
                while ( dit2.hasNext() ) {
                    BlDateSearch * item = dit2.next();
                    QString nombre = item->objectName().right ( item->objectName().size() - 4 );
                    QString valor = item->text();
                    addDbField ( nombre, BlDbField::DbDate, BlDbField::DbNoSave, nombre  );
                    setDbValue ( nombre, valor );
                } // end while
	      
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
		 
		/// Recorre los BlSearchWidget.
                QList<BlSearchWidget *> jqcb = iface->findChildren<BlSearchWidget *>();
                QListIterator<BlSearchWidget *> itjqcb ( jqcb );

                while ( itjqcb.hasNext() ) {
                    BlSearchWidget *item = itjqcb.next();
                    QString nombre = item->objectName().right ( item->objectName().size() - 4 );
                    QString valor = item->id();
                    addDbField ( nombre, BlDbField::DbInt, BlDbField::DbNoSave, nombre  );
                    setDbValue ( nombre, valor );
		 } // end while
		 
		/// Recorre los BlComboBox.
                QList<BlComboBox *> jqcb1 = iface->findChildren<BlComboBox *>();
                QListIterator<BlComboBox *> itjqcb1 ( jqcb1 );

                while ( itjqcb1.hasNext() ) {
                    BlComboBox *item = itjqcb1.next();
                    QString nombre = item->objectName().right ( item->objectName().size() - 4 );
                    QString valor = item->id();
                    addDbField ( nombre, BlDbField::DbInt, BlDbField::DbNoSave, nombre  );
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
            
	    delete layout;
 	    delete iface;
            delete diag;
            
            // Si se ha pulsado cancelar entonce se sale del informe.
            if ( !ret ) return 0;
            
        } else {
            blMsgInfo ( "Archivo de Interficie no existe" );
        } // end if

        buff.replace ( pos, rx70.matchedLength(), "" );
        pos = 0;
    } // end while


    /// Buscamos comentarios para poder quitarlos
    pos = buff.indexOf("<!-- COMMENT");
    cadant = buff.left(pos);
    buff = buff.right(buff.length()-pos);
    QRegExp rx541 ( "<!--\\s*COMMENT\\s*([^-]*)\\s*-->" );
    rx541.setMinimal ( TRUE );
    while ( ( pos = rx541.indexIn ( buff, 0 ) ) != -1 ) {
        buff.replace ( pos, rx541.matchedLength(), "" );
        pos = buff.indexOf("<!--");
    } // end while
    buff = cadant + buff;


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
    
    /// Inclusion de ficheros de codigo
    pos = buff.indexOf("<!-- INCLUDE");
    cadant = buff.left(pos);
    buff = buff.right(buff.length()-pos);
    QRegExp rx19 ( "<!--\\s*INCLUDE\\s*FILE\\s*=\\s*\"([^\"]*)\"\\s*-->" );
    rx19.setMinimal ( TRUE );
    while ( ( pos = rx19.indexIn ( buff, 0 ) ) != -1 ) {
        QString ldetalle = parseIncludeFileTXT ( rx19.cap ( 1 ), tipoEscape );
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
        QString ldetalle = parseExists ( rx9.cap ( 1 ), rx9.cap ( 2 ).toAscii() );
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
        QString ldetalle = parseIfQuery ( rx4.cap ( 1 ), rx4.cap ( 2 ).toAscii() );
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
        QByteArray ldetalle = parseQuery ( rx1.cap ( 1 ), rx1.cap ( 2 ).toAscii(), tipoEscape );
        buff.replace ( pos, rx1.matchedLength(), ldetalle );
        pos = buff.indexOf("<!--");
    } // end while
    buff = cadant + buff;

    
    /// Buscamos Query's por tratar con numeracion
    for (int aux = 0;aux < 10; aux++) {
      
	/// Buscamos Query's en condicional
	pos = buff.indexOf("<!-- IF QUERY");
	cadant = buff.left(pos);
	buff = buff.right(buff.length()-pos);
	QRegExp rx4 ( "<!--\\s*IF\\s*QUERY"+QString::number(aux)+"\\s*=\\s*\"([^\"]*)\"\\s*-->(.*)<!--\\s*END\\s*IF\\s*QUERY"+QString::number(aux)+"\\s*-->" );
	rx4.setMinimal ( TRUE );
	while ( ( pos = rx4.indexIn ( buff, 0 ) ) != -1 ) {
	    QString ldetalle = parseIfQuery ( rx4.cap ( 1 ), rx4.cap ( 2 ).toAscii() );
	    buff.replace ( pos, rx4.matchedLength(), ldetalle.toAscii() );
	    pos = buff.indexOf("<!--");
	} // end while
	buff = cadant + buff;
      
	pos = buff.indexOf("<!-- QUERY");
	cadant = buff.left(pos);
	buff = buff.right(buff.length()-pos);
	QRegExp rx1 ( "<!--\\s*QUERY"+QString::number(aux)+"\\s*=\\s*\"([^\"]*)\"\\s*-->(.*)<!--\\s*END\\s*QUERY"+QString::number(aux)+"\\s*-->" );
	rx1.setMinimal ( TRUE );
	while ( ( pos = rx1.indexIn ( buff, 0 ) ) != -1 ) {
	    QByteArray ldetalle = parseQuery ( rx1.cap ( 1 ), rx1.cap ( 2 ).toAscii(), tipoEscape );
	    buff.replace ( pos, rx1.matchedLength(), ldetalle );
	    pos = buff.indexOf("<!--");
	} // end while
	buff = cadant + buff;
    }// end for
    
    
    /// Buscamos SubQuery's en condicional
    pos = buff.indexOf("<!-- IF SUBQUERY");
    cadant = buff.left(pos);
    buff = buff.right(buff.length()-pos);
    QRegExp rx14 ( "<!--\\s*IF\\s*SUBQUERY\\s*=\\s*\"([^\"]*)\"\\s*-->(.*)<!--\\s*END\\s*IF\\s*SUBQUERY\\s*-->" );
    rx14.setMinimal ( TRUE );
    while ( ( pos = rx14.indexIn ( buff, 0 ) ) != -1 ) {
        QString ldetalle = parseIfQuery ( rx14.cap ( 1 ), rx14.cap ( 2 ).toAscii() );
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
        QString ldetalle = parseQuery ( rx7.cap ( 1 ), rx7.cap ( 2 ).toAscii(), tipoEscape );
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
        QString ldetalle = parseIf ( rx11.cap ( 1 ), rx11.cap ( 2 ).toAscii(), rx11.cap ( 3 ).toAscii() );
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
        QByteArray ldetalle = parseSetCharacterPrintMode ( rx39.cap ( 1 ), tipoEscape );
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
        QByteArray ldetalle = parseSetCharacterSpacing ( rx49.cap ( 1 ), tipoEscape );
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
        QByteArray ldetalle = parseSetCharacterCodeTable ( rx59.cap ( 1 ), tipoEscape );
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
        QByteArray ldetalle = parseSetUnderlineMode ( rx69.cap ( 1 ), tipoEscape );
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
        QByteArray ldetalle = parseSetCharacterSize ( rx79.cap ( 1 ), tipoEscape );
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
        QByteArray ldetalle = parseSetCharacterSize ( rx89.cap ( 1 ), tipoEscape );
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
        QByteArray ldetalle = parseSetDoubleStrike ( rx99.cap ( 1 ), tipoEscape );
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
        QByteArray ldetalle = parseTurnUpsideDown ( rx02.cap ( 1 ), tipoEscape );
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
        QByteArray ldetalle = parseTurn90CWRotation (rx12.cap(1), rx12.cap(2), tipoEscape );
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
        QByteArray ldetalle = parseTurnWhiteBlack( rx22.cap ( 1 ), tipoEscape );
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
        QByteArray ldetalle = parseSetColor( rx32.cap ( 1 ), tipoEscape );
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
        QByteArray ldetalle = parseHorizontalTab( tipoEscape );
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
        QByteArray ldetalle = parseSetHorizontalTabPos (rx52.cap(1), rx52.cap(2), tipoEscape );
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
        QByteArray ldetalle = parseSetLeftMargin( rx62.cap ( 1 ), tipoEscape );
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
        QByteArray ldetalle = parseSetPrintingAreaWidth( rx72.cap ( 1 ), tipoEscape );
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
        QByteArray ldetalle = parseSetHAbsolutePos( rx92.cap ( 1 ), tipoEscape );
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
        QByteArray ldetalle = parseSetHRelativePos( rx03.cap ( 1 ), tipoEscape );
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
        QByteArray ldetalle = parseSetBarcodeFormat( rx13.cap ( 1 ), rx13.cap ( 2 ),rx13.cap ( 3 ),rx13.cap ( 4 ),tipoEscape );
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
        QByteArray ldetalle = parsePrintBarCode( rx23.cap ( 1 ), rx23.cap ( 2 ),rx23.cap ( 3 ),tipoEscape );
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
        QByteArray ldetalle = parseSetBarCodeHeight( rx33.cap ( 1 ), tipoEscape );
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
        QByteArray ldetalle = parseSetBarCodeWidth( rx34.cap ( 1 ), tipoEscape );
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
        QByteArray ldetalle = parseSelectPageMode( tipoEscape );
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
        QByteArray ldetalle = parseSetPrintArea( rx36.cap ( 1 ), rx36.cap ( 2 ), rx36.cap ( 3 ), rx36.cap ( 4 ), tipoEscape );
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
        QByteArray ldetalle = parseSetPrintDirection( rx37.cap ( 1 ), tipoEscape );
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
        QByteArray ldetalle = parseSetVAbsolutePos( rx38.cap ( 1 ), tipoEscape );
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
        QByteArray ldetalle = parseSetVRelativePos( rx41.cap ( 1 ), tipoEscape );
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
        QByteArray ldetalle = parseCutPaper( rx45.cap ( 1 ), tipoEscape );
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
        QByteArray ldetalle = parseRightJustified( rx46.cap ( 1 ),  rx46.cap ( 2 ), rx46.cap ( 3 ),  rx46.cap ( 4 ),tipoEscape );
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
        QByteArray ldetalle = parseCutPaperAndFeed( rx44.cap ( 1 ),  rx44.cap ( 2 ),tipoEscape );
        buff.replace ( pos, rx44.matchedLength(), ldetalle );
        pos = buff.indexOf("<!--");
    } // end while
    buff = cadant + buff;
    
    profundidad --;
    if (profundidad == 0) {
      parseTagsPost(buff, tipoEscape);
    } // end if
    
    return 1;
}



/// Busca strings del tipo [xxxx] entro del texto pasado y los sustituye
/// Por valores existentes en la base de datos.
/// Tambien busca los parametros PARAM e IFACE para tambien tratarlos.
/// Devuelve 1 Si todo esta OK o 0 Si hay algun error
/**
\param buff El texto entero sobre el que se hace el reemplazo de sentencias.
**/
int BlDbRecord::parseTagsPost ( QByteArray &buff, int tipoEscape )
{
    BL_FUNC_DEBUG

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
        QByteArray ldetalle = parseSetJustification( rx82.cap ( 1 ), tipoEscape );
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
        QByteArray ldetalle = parsePrintBarCode( rx23.cap ( 1 ), rx23.cap ( 2 ),rx23.cap ( 3 ),tipoEscape );
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
        QByteArray ldetalle = parseSetBarCodeHeight( rx33.cap ( 1 ), tipoEscape );
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
        QByteArray ldetalle = parseRightJustified( rx46.cap ( 1 ),  rx46.cap ( 2 ), rx46.cap ( 3 ),  rx46.cap ( 4 ),tipoEscape );
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
        QByteArray ldetalle = parseLeftJustified( rx47.cap ( 1 ),  rx47.cap ( 2 ), rx47.cap ( 3 ),  rx47.cap ( 4 ),tipoEscape );
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
        QByteArray ldetalle = parseCutPaperAndFeed( rx44.cap ( 1 ),  rx44.cap ( 2 ),tipoEscape );
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
        QByteArray ldetalle = parseIncludeImg ( rx29.cap ( 1 ), tipoEscape );
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
        QByteArray ldetalle = parsePngRaw64 ( rx791.cap ( 1 ).toAscii(), tipoEscape );
        buff.replace ( pos, rx791.matchedLength(), ldetalle );
	buff = cadant + buff;
        pos = buff.indexOf("<!-- PNGRAW64");
	cadant = buff.left(pos);
	buff = buff.right(buff.length()-pos);
    } // end while
    buff = cadant + buff;
    
    
    return 1;
}



/// Trata las lineas de detalle encontradas dentro de los tags <!--LINEAS DETALLE-->
/**
\param det Texto de entrada para ser tratado por iteracion.
\return Si el query tiene elementos lo devuelve el parametro. En caso contrario no devuelve nada.
**/
QByteArray BlDbRecord::parseIfQuery ( const QString &query, const QByteArray &datos )
{
    BL_FUNC_DEBUG
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
    
    return result;
}


/// Trata las lineas de detalle encontradas dentro de los tags <!--LINEAS DETALLE-->
/**
\param det Texto de entrada para ser tratado por iteracion.
\return Si el query tiene elementos lo devuelve el parametro. En caso contrario no devuelve nada.
**/
QByteArray BlDbRecord::parseIf ( const QString &query, const QByteArray &datos, const QByteArray &datos1 )
{
    BL_FUNC_DEBUG
    QByteArray result = "";
    QByteArray query1 = query.toAscii();

    /// Buscamos parametros en el query y los ponemos.
    substrVars ( query1 );

    QString query2 = "SELECT (" + QString(query1) + ") AS res";
    /// Cargamos el query y lo recorremos
    BlDbRecordSet *cur = m_dbConnection ->loadQuery ( query2 );
    if ( !cur ) return "";
    if ( !cur->eof() ) {
        if ( cur->value( "res" ) == "t" ) {
            result = datos;
        } else {
            result = datos1;
        } // end if
    } // end while
    delete cur;
    
    return result;
}


/// Trata las lineas de detalle encontradas dentro de los tags <!--LINEAS DETALLE-->
/**
\param det Texto de entrada para ser tratado por iteracion.
\return
**/
QByteArray BlDbRecord::parseIncludeFileTXT ( const QString &file, int tipoEscape )
{
    BL_FUNC_DEBUG
    QByteArray read = "";
    QFile arch ( file );
    if ( arch.open ( QIODevice::ReadOnly ) ) {
       read = arch.readAll();
        arch.close();
    } // end if
    /// Buscamos parametros en el query y los ponemos.
    substrVars ( read, tipoEscape );


    
    return read;

}


/// Trata las inclusiones de imagenes
/**
\param det Texto de entrada para ser tratado por iteracion.
\return
**/
QByteArray BlDbRecord::parseIncludeImg ( const QString &file, int tipoEscape )
{
    BL_FUNC_DEBUG
    BlEscPrinter pr;
    pr.clearBuffer();
    pr.printImage(file);
    
    return pr.buffer();

}


/// Trata las inclusiones de imagenes
/**
\param det Texto de entrada para ser tratado por iteracion.
\return
**/
QByteArray BlDbRecord::parsePngRaw64 ( const QByteArray &data, int tipoEscape )
{
    BL_FUNC_DEBUG
//    blMsgInfo("pngRawData");
    BlEscPrinter pr;
    pr.clearBuffer();
    QByteArray dataq = QByteArray::fromBase64(data);
    pr.printImageRaw(dataq);
    
//    blMsgInfo(pr.buffer());
    return pr.buffer();

}

/// Trata el setcharacterprintermode
/**
\param det Texto de entrada para ser tratado por iteracion.
\return
**/
QByteArray BlDbRecord::parseSetCharacterPrintMode ( const QString &param, int tipoEscape )
{
    BL_FUNC_DEBUG
    int modo=0;

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
    
    return pr.buffer();

}

/// Trata el setcharacterprintermode
/**
\param det Texto de entrada para ser tratado por iteracion.
\return
**/
QByteArray BlDbRecord::parseSetCharacterSpacing ( const QString &param, int tipoEscape )
{
    BL_FUNC_DEBUG
    int modo=0;

    BlEscPrinter pr;
    pr.clearBuffer();

    pr.setCharacterSpacing(param.toInt());
    
    return pr.buffer();

}



/// Trata el setcharacterprintermode
/**
\param det Texto de entrada para ser tratado por iteracion.
\return
**/
QByteArray BlDbRecord::parseSetCharacterCodeTable ( const QString &param, int tipoEscape )
{
    BL_FUNC_DEBUG
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
    
    return pr.buffer();

}

/// Trata el setcharacterprintermode
/**
\param det Texto de entrada para ser tratado por iteracion.
\return
**/
QByteArray BlDbRecord::parseSetUnderlineMode ( const QString &param, int tipoEscape )
{
    BL_FUNC_DEBUG
    bool modo=FALSE;
    BlEscPrinter pr;
    pr.clearBuffer();
    if (param.contains("TRUE"))
        modo = TRUE;
    if (param.contains("true"))
        modo = TRUE;
    if (param.contains("1"))
        modo = TRUE;

    pr.setUnderlineMode(modo);
    
    return pr.buffer();

}

/// Trata el SetCharacterSize
/**
\param det Texto de entrada para ser tratado por iteracion.
\return
**/
QByteArray BlDbRecord::parseSetCharacterSize ( const QString &param, int tipoEscape )
{
    BL_FUNC_DEBUG
    int modo=0;

    BlEscPrinter pr;
    pr.clearBuffer();

    pr.setCharacterSize(param.toInt());
    
    return pr.buffer();

}

/// Trata el setsmoothing
/**
\param det Texto de entrada para ser tratado por iteracion.
\return
**/
QByteArray BlDbRecord::parseSetSmoothing ( const QString &param, int tipoEscape )
{
    BL_FUNC_DEBUG
    bool modo=FALSE;

    BlEscPrinter pr;
    pr.clearBuffer();
    if (param.contains("TRUE"))
        modo = TRUE;
    if (param.contains("true"))
        modo = TRUE;
    if (param.contains("1"))
        modo = TRUE;

    pr.setSmoothing(modo);
    
    return pr.buffer();

}

/// Trata el parseSetDoubleStrike
/**
\param det Texto de entrada para ser tratado por iteracion.
\return
**/
QByteArray BlDbRecord::parseSetDoubleStrike ( const QString &param, int tipoEscape )
{
    BL_FUNC_DEBUG
    bool modo=FALSE;

    BlEscPrinter pr;
    pr.clearBuffer();
    if (param.contains("TRUE"))
        modo = TRUE;
    if (param.contains("true"))
        modo = TRUE;
    if (param.contains("1"))
        modo = TRUE;

    pr.setDoubleStrike(modo);
    
    return pr.buffer();

}

/// Trata el parseTurnUpsideDown
/**
\param det Texto de entrada para ser tratado por iteracion.
\return
**/
QByteArray BlDbRecord::parseTurnUpsideDown ( const QString &param, int tipoEscape )
{
    BL_FUNC_DEBUG
    bool modo=FALSE;

    BlEscPrinter pr;
    pr.clearBuffer();
    if (param.contains("TRUE"))
        modo = TRUE;
    if (param.contains("true"))
        modo = TRUE;
    if (param.contains("1"))
        modo = TRUE;

    pr.turnUpsideDown(modo);
    
    return pr.buffer();

}


/// Trata el parseTurn90CWRotation
/**
\param det Texto de entrada para ser tratado por iteracion.
\return
**/
QByteArray BlDbRecord::parseTurn90CWRotation ( const QString &param, const QString &param1, int tipoEscape )
{
    BL_FUNC_DEBUG
    bool modo=FALSE;
    bool extra = FALSE;
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
    
    return pr.buffer();

}


/// Trata el parseTurnWhiteBlack
/**
\param det Texto de entrada para ser tratado por iteracion.
\return
**/
QByteArray BlDbRecord::parseTurnWhiteBlack ( const QString &param, int tipoEscape )
{
    BL_FUNC_DEBUG
    bool modo=FALSE;
    BlEscPrinter pr;
    pr.clearBuffer();
    if (param.contains("TRUE"))
        modo = TRUE;
    if (param.contains("true"))
        modo = TRUE;
    if (param.contains("1"))
        modo = TRUE;

    pr.turnWhiteBlack(modo);
    
    return pr.buffer();

}

/// Trata el parseSetColor
/**
\param det Texto de entrada para ser tratado por iteracion.
\return
**/
QByteArray BlDbRecord::parseSetColor ( const QString &param, int tipoEscape )
{
    BL_FUNC_DEBUG
    printColor color=black;
    BlEscPrinter pr;
    pr.clearBuffer();
    if (param.contains("BLACK"))
        color = black;
    if (param.contains("RED"))
        color = red;

    pr.setColor(color);
    
    return pr.buffer();

}

/// Trata el parseSetColor
/**
\param det Texto de entrada para ser tratado por iteracion.
\return
**/
QByteArray BlDbRecord::parseHorizontalTab ( int tipoEscape )
{
    BL_FUNC_DEBUG
    BlEscPrinter pr;
    pr.clearBuffer();
    pr.horizontalTab();
    
    return pr.buffer();

}

/// Trata el parseTurn90CWRotation
/**
\param det Texto de entrada para ser tratado por iteracion.
\return
**/
QByteArray BlDbRecord::parseSetHorizontalTabPos ( const QString &param, const QString &param1, int tipoEscape )
{

    BL_FUNC_DEBUG
    BlEscPrinter pr;
    pr.clearBuffer();

    pr.setHorizontalTabPos(param.toInt(),param1.toLatin1().data());
    
    return pr.buffer();

}


/// Trata el parseSetLeftMargin
/**
\param det Texto de entrada para ser tratado por iteracion.
\return
**/
QByteArray BlDbRecord::parseSetLeftMargin ( const QString &param, int tipoEscape )
{
    BL_FUNC_DEBUG
    int modo=0;
    BlEscPrinter pr;
    pr.clearBuffer();

    pr.setLeftMargin(param.toInt());
    
    return pr.buffer();

}

/// Trata el parseSetPrintingAreaWidth
/**
\param det Texto de entrada para ser tratado por iteracion.
\return
**/
QByteArray BlDbRecord::parseSetPrintingAreaWidth ( const QString &param, int tipoEscape )
{
    BL_FUNC_DEBUG
    int modo=0;
    BlEscPrinter pr;
    pr.clearBuffer();

    pr.setPrintingAreaWidth(param.toInt());
    
    return pr.buffer();

}


/// Trata el parseSetJustification
/**
\param det Texto de entrada para ser tratado por iteracion.
\return
**/
QByteArray BlDbRecord::parseSetJustification ( const QString &param, int tipoEscape )
{
    BL_FUNC_DEBUG
    BlEscPrinter::justification modo = BlEscPrinter::left ;
    BlEscPrinter pr;
    pr.clearBuffer();
    if (param.contains("LEFT"))
        modo = BlEscPrinter::left;
    if (param.contains("CENTER"))
        modo = BlEscPrinter::center;
    if (param.contains("RIGHT"))
        modo = BlEscPrinter::right;

    pr.setJustification(modo);
    
    return pr.buffer();

}

/// Trata el parseSetHAbsolutePos
/**
\param det Texto de entrada para ser tratado por iteracion.
\return
**/
QByteArray BlDbRecord::parseSetHAbsolutePos ( const QString &param, int tipoEscape )
{
    BL_FUNC_DEBUG
    int modo=0;
    BlEscPrinter pr;
    pr.clearBuffer();

    pr.setHAbsolutePos(param.toInt());
    
    return pr.buffer();

}

/// Trata el parseSetHRelativePos
/**
\param det Texto de entrada para ser tratado por iteracion.
\return
**/
QByteArray BlDbRecord::parseSetHRelativePos ( const QString &param, int tipoEscape )
{
    BL_FUNC_DEBUG
    int modo=0;
    BlEscPrinter pr;
    pr.clearBuffer();

    pr.setHRelativePos(param.toInt());
    
    return pr.buffer();

}


/// Trata el parseSetBarcodeFormat
/**
\param det Texto de entrada para ser tratado por iteracion.
\return
**/
QByteArray BlDbRecord::parseSetBarcodeFormat ( const QString &param, const QString &param1,const QString &param2,const QString &param3,int tipoEscape )
{
    BL_FUNC_DEBUG
    barCodeTextPos pos = notPrinted;
    printerFont    font = fontA;
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
    
    return pr.buffer();

}



/// Trata el parsePrintBarCode
/**
\param det Texto de entrada para ser tratado por iteracion.
\return
**/
QByteArray BlDbRecord::parsePrintBarCode ( const QString &param, const QString &param1,const QString &param2,int tipoEscape )
{
    BL_FUNC_DEBUG
    barcodeSystem system = upca;
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
    
    return pr.buffer();

}


/// Trata el parsePrintBarCode
/**
\param det Texto de entrada para ser tratado por iteracion.
\return
**/
QByteArray BlDbRecord::parseRightJustified ( const QString &param, const QString &param1,const QString &param2,const QString &param3,int tipoEscape )
{

    BL_FUNC_DEBUG
    bool truncate = FALSE;
    if (param3.contains("TRUE"))
        truncate = TRUE;
    if (param3.contains("true"))
        truncate = TRUE;
    if (param3.contains("1"))
        truncate = TRUE;

    
    return param.rightJustified(param1.toInt(), param2.at(0), truncate).toAscii();
}

/// Trata el parseLeftJustified
/**
\param det Texto de entrada para ser tratado por iteracion.
\return
**/
QByteArray BlDbRecord::parseLeftJustified ( const QString &param, const QString &param1,const QString &param2,const QString &param3,int tipoEscape )
{

    BL_FUNC_DEBUG
    bool truncate = FALSE;
    if (param3.contains("TRUE"))
        truncate = TRUE;
    if (param3.contains("true"))
        truncate = TRUE;
    if (param3.contains("1"))
        truncate = TRUE;

    
    return param.leftJustified(param1.toInt(), param2.at(0), truncate).toAscii();
}


/// Trata el setBarCodeWidth
/**
\param det Texto de entrada para ser tratado por iteracion.
\return
**/
QByteArray BlDbRecord::parseSetBarCodeWidth ( const QString &param, int tipoEscape )
{
    BL_FUNC_DEBUG
    int modo=0;
    BlEscPrinter pr;
    pr.clearBuffer();

    pr.setBarCodeWidth(param.toInt());
    
    return pr.buffer();

}

/// Trata el setBarCodeHeight
/**
\param det Texto de entrada para ser tratado por iteracion.
\return
**/
QByteArray BlDbRecord::parseSetBarCodeHeight ( const QString &param, int tipoEscape )
{
    BL_FUNC_DEBUG
    int modo=0;
    BlEscPrinter pr;
    pr.clearBuffer();

    pr.setBarCodeHeight(param.toInt());
    
    return pr.buffer();

}

/// Trata el parseSelectPageMode
/**
\param det Texto de entrada para ser tratado por iteracion.
\return
**/
QByteArray BlDbRecord::parseSelectPageMode ( int tipoEscape )
{
    BL_FUNC_DEBUG
    int modo=0;
    BlEscPrinter pr;
    pr.clearBuffer();

    pr.selectPageMode();
    
    return pr.buffer();

}


/// Trata el parseSetPrintDirection
/**
\param det Texto de entrada para ser tratado por iteracion.
\return
**/
QByteArray BlDbRecord::parseSetPrintDirection ( const QString &param, int tipoEscape )
{
    BL_FUNC_DEBUG
    printDirection direc = leftToRight;
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
    
    return pr.buffer();

}




/// Trata el parseSetPrintArea
/**
\param det Texto de entrada para ser tratado por iteracion.
\return
**/
QByteArray BlDbRecord::parseSetPrintArea ( const QString &param,  const QString &param1,  const QString &param2,  const QString &param3, int tipoEscape )
{
    BL_FUNC_DEBUG
    int modo=0;

    BlEscPrinter pr;
    pr.clearBuffer();

    pr.setPrintArea(param.toInt(), param1.toInt(), param2.toInt(), param3.toInt());
    
    return pr.buffer();

}


/// Trata el parseSetVAbsolutePos
/**
\param det Texto de entrada para ser tratado por iteracion.
\return
**/
QByteArray BlDbRecord::parseSetVAbsolutePos ( const QString &param, int tipoEscape )
{
    BL_FUNC_DEBUG
    int modo=0;

    BlEscPrinter pr;
    pr.clearBuffer();

    pr.setVAbsolutePos(param.toInt());
    
    return pr.buffer();

}

/// Trata el parseSetVRelativePos
/**
\param det Texto de entrada para ser tratado por iteracion.
\return
**/
QByteArray BlDbRecord::parseSetVRelativePos ( const QString &param, int tipoEscape )
{
    BL_FUNC_DEBUG
    int modo=0;
    BlEscPrinter pr;
    pr.clearBuffer();

    pr.setVRelativePos(param.toInt());
    
    return pr.buffer();

}

/// Trata el parseCutPaper
/**
\param det Texto de entrada para ser tratado por iteracion.
\return
**/
QByteArray BlDbRecord::parseCutPaper ( const QString &param, int tipoEscape )
{
    BL_FUNC_DEBUG
    bool modo=FALSE;
    BlEscPrinter pr;
    pr.clearBuffer();
    if (param.contains("TRUE"))
        modo = TRUE;
    if (param.contains("true"))
        modo = TRUE;
    if (param.contains("1"))
        modo = TRUE;

    pr.cutPaper(modo);
    
    return pr.buffer();

}



/// Trata el parseCutPaperAndFeed
/**
\param det Texto de entrada para ser tratado por iteracion.
\return
**/
QByteArray BlDbRecord::parseCutPaperAndFeed ( const QString &param, const QString &param1, int tipoEscape )
{
    BL_FUNC_DEBUG
    bool modo=FALSE;
    BlEscPrinter pr;
    pr.clearBuffer();
    if (param.contains("TRUE"))
        modo = TRUE;
    if (param.contains("true"))
        modo = TRUE;
    if (param.contains("1"))
        modo = TRUE;

    pr.cutPaperAndFeed(modo, param1.toInt());
    
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

    /// Buscamos parametros en el query y los ponemos de forma literal.
    QRegExp rx1 ( "\\[(\\w*),l\\]" );
    while ( ( pos = rx1.indexIn ( buff, pos ) ) != -1 ) {
        if ( exists ( rx1.cap ( 1 ) ) ) {
            buff.replace ( pos, rx1.matchedLength(), dbValue ( rx1.cap ( 1 ) ).toAscii() );
            pos = 0;
        } else {
            pos += rx1.matchedLength();
        }
    } // end while
    
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
QByteArray BlDbRecord::parseQuery ( const QString &query, const QByteArray &datos, int tipoEscape )
{
    BL_FUNC_DEBUG
    QByteArray result = "";
    QByteArray query1 = query.toAscii();

    /// Buscamos parametros en el query y los ponemos.
    substrVars ( query1, tipoEscape );

    /// Cargamos el query y lo recorremos
    result = parseRecordset ( m_dbConnection->loadQuery ( query1 ), datos, tipoEscape );
    
    return result;

}


QByteArray BlDbRecord::parseRecordset ( BlDbRecordSet *cur, const QByteArray &datos, int tipoEscape )
{
    BL_FUNC_DEBUG
    QByteArray result = "";
    
    if ( !cur ) {
        
        return "";
    };
    while ( !cur->eof() ) {
        QByteArray salidatemp = datos;

	
        int pos =  0;
	
        /// Buscamos cadenas perdidas adicionales que puedan quedar por poner.
        //BlDebug::blDebug("salidatemp =",0,salidatemp);
        QRegExp rx1 ( "\\[(\\w*),l\\]" );
        while ( ( pos = rx1.indexIn ( salidatemp, pos ) ) != -1 ) {
            //BlDebug::blDebug("substituïm ",0,rx.cap(1));
            if ( cur->numcampo ( rx1.cap ( 1 ) ) != -1 ) {
                salidatemp.replace ( pos, rx1.matchedLength(), cur->value( rx1.cap ( 1 ), -1, TRUE ).toAscii() );
                pos = 0;
            } else {
                pos += rx1.matchedLength();
            }
        } // end while

	pos = 0;

        /// Buscamos cadenas perdidas adicionales que puedan quedar por poner.
        //BlDebug::blDebug("salidatemp =",0,salidatemp);
        QRegExp rx ( "\\[(\\w*)\\]" );
        while ( ( pos = rx.indexIn ( salidatemp, pos ) ) != -1 ) {
            //BlDebug::blDebug("substituïm ",0,rx.cap(1));
            if ( cur->numcampo ( rx.cap ( 1 ) ) != -1 ) {
                switch ( tipoEscape ) {
                case 1:
                    salidatemp.replace ( pos, rx.matchedLength(), blXMLEscape ( cur->value( rx.cap ( 1 ), -1, TRUE ) ).toAscii()  );
                    break;
                case 2:
                    salidatemp.replace ( pos, rx.matchedLength(), blPythonEscape ( cur->value( rx.cap ( 1 ), -1, TRUE ) ).toAscii()  );
                    break;
                default:
                    salidatemp.replace ( pos, rx.matchedLength(), cur->value( rx.cap ( 1 ), -1, TRUE ).toAscii() );
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
    
    return result;
}


/// Trata las lineas de detalle encontradas dentro de los tags <!--LINEAS DETALLE-->
/**
\param det Texto de entrada para ser tratado por iteracion.
\return Si el query tiene elementos lo devuelve el parametro. En caso contrario no devuelve nada.
**/
QByteArray BlDbRecord::parseExists ( const QString &query, const QByteArray &datos )
{
    BL_FUNC_DEBUG

    QByteArray result = "";
    QByteArray query1 = query.toAscii();

    /// Buscamos parametros en el query y los ponemos.
    substrVars ( query1 );

    QFile file ( query1 );
    if ( file.exists() )
        result = datos;
    

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
