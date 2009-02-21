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

#include "bldb.h"
#include "plugins.h"


///
/**
**/
bool BlDbField::cambiado()
{
    _depura ( "BlDbField::cambiado", 0 );
    _depura ( "END BlDbField::cambiado", 0 );
    return m_valorcampo != m_valorcampoorig;
}


///
/**
**/
void BlDbField::resetCambio()
{
    _depura ( "BlDbField::resetCambio", 0 );
    m_valorcampoorig = m_valorcampo;
    _depura ( "END BlDbField::resetCambio", 0 );
}


///
/**
**/
BlDbField::~BlDbField()
{
    _depura ( "BlDbField::~BlDbField", 0 );
    _depura ( "END BlDbField::~BlDbField", 0 );
}


///
/**
\return
**/
BlPostgreSqlClient *BlDbField::conexionbase()
{
    _depura ( "BlDbField::conexionbase", 0 );
    _depura ( "END BlDbField::conexionbase", 0 );
    return m_conexionbase;
}


///
/**
\param comp
**/
void BlDbField::setconexionbase ( BlPostgreSqlClient *comp )
{
    _depura ( "BlDbField::setconexionbase", 0 );
    m_conexionbase = comp;
    _depura ( "END BlDbField::setconexionbase", 0 );
}


///
/**
\return
**/
BlDbField::dbtype BlDbField::tipo()
{
    _depura ( "BlDbField::tipo", 0 );
    _depura ( "END BlDbField::tipo", 0 );
    return m_tipo;
}


///
/**
\param val
\return
**/
int BlDbField::set ( QString val )
{
    _depura ( "BlDbField::set", 0 );
    m_valorcampo = val;
    _depura ( "END BlDbField::set", 0 );
    return 0;
}


///
/**
\return
**/
int BlDbField::restrictcampo()
{
    _depura ( "BlDbField::restrictcampo", 0 );
    _depura ( "END BlDbField::restrictcampo", 0 );
    return m_restrict;
}


///
/**
\return
**/
QString BlDbField::nomcampo()
{
    _depura ( "BlDbField::nomcampo", 0 );
    _depura ( "END BlDbField::nomcampo", 0 );
    return m_nomcampo;
}


///
/**
\return
**/
QString BlDbField::nompresentacion()
{
    _depura ( "BlDbField::nompresentacion", 0 );
    _depura ( "END BlDbField::nompresentacion", 0 );
    return m_nompresentacion;
}


///
/**
\return
**/
QString BlDbField::valorcampo()
{
    _depura ( "BlDbField::valorcampo", 0 );
	QString valor = m_valorcampo;
	switch (m_tipo) {
		case DBnumeric:
				QLocale locale;
				if (locale.decimalPoint() == '.') 
					valor.replace ( ",", locale.decimalPoint () );
				else
					valor.replace ( ".", locale.decimalPoint () );
			break;
	} // end switch
    _depura ( "END BlDbField::valorcampo", 0 );
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
BlDbField::BlDbField ( BlPostgreSqlClient *com, QString nom, dbtype typ, int res, QString nomp )
{
    _depura ( "BlDbField::BlDbField", 0 );
    m_conexionbase = com;
    m_nomcampo = nom;
    m_tipo = typ;
    m_restrict = res;
    m_nompresentacion = nomp;
    m_valorcampo = "";
    m_valorcampoorig = "";
    _depura ( "END BlDbField::BlDbField", 0 );
}


/// Devuelve el valor del campo preparado para ser insertado en la base de datos.
/**
\param error
\return
**/
QString BlDbField::valorcampoprep ( int &error )
{
    _depura ( "BlDbField::valorcampoprep", 0 );
    error = 0;
    QString valor = "";
    if ( ( m_restrict & DBNotNull ) && ! ( m_restrict & DBAuto ) ) {
        if ( m_valorcampo == "" ) {
            mensajeAviso ( "El campo '" + m_nompresentacion + "' no puede estar vacio." );
            error = -20200;
            _depura ( "END BlDbField::valorcampoprep", 0, m_nomcampo + " " + m_valorcampo + "-->" + valor );
            return valor;
        } // end if
    } // end if

    switch ( m_tipo ) {
    case DBint:
        if ( m_valorcampo == "" ) {
            valor = "NULL";
        } else {
            m_valorcampo.replace ( ",", "." );
            valor = "'" + m_conexionbase->sanearCadena ( m_valorcampo ) + "'";
        } // end if
        break;
    case DBvarchar:
        if ( m_valorcampo == "" ) {
            valor = "NULL";
        } else {
            valor = "'" + m_conexionbase->sanearCadena ( m_valorcampo ) + "'";
        } // end if
        break;
    case DBdate:
        if ( m_valorcampo == "" ) {
            valor = "NULL";
        } else {
            valor = "'" + m_conexionbase->sanearCadena ( m_valorcampo ) + "'";
        } // end if
        break;
    case DBnumeric:
        if ( m_valorcampo == "" ) {
            valor = "NULL";
        } else {
            m_valorcampo.replace ( ",", "." );
            valor =  "'" + m_conexionbase->sanearCadena ( m_valorcampo ) + "'";
        } // end if
        break;
    case DBboolean:
        if ( m_valorcampo == "" ) {
            valor = "NULL";
        } else if ( m_valorcampo == "f" || m_valorcampo == "t" ) {
            valor = "'" + m_conexionbase->sanearCadena ( m_valorcampo ) + "'";
        } else {
            valor = m_conexionbase->sanearCadena ( m_valorcampo );
        } // end if
        break;
    default:
        error = -1;
    } // end switch

    _depura ( "END BlDbField::valorcampoprep", 0, m_nomcampo + " " + m_valorcampo + "-->" + valor );
    return valor;
}


/// Constructor de la clase que toma por defecto la base de datos con la que va a operar.
/**
\param con Puntero a la base de datos con la que vamos a trabajar.
**/
BlDbRecord::BlDbRecord ( BlMainCompany *con )
{
    _depura ( "BlDbRecord::BlDbRecord", 0 );
    m_conexionbase = con;
    m_nuevoCampo = TRUE;
    _depura ( "END BlDbRecord::BlDbRecord", 0 );
}


/// Destructor de la clase
/**
Limpia y libera toda la memoria que se haya podido reservar.
Esta operacion no contempla que haya habido cambios en el registro sin guardar.
**/
BlDbRecord::~BlDbRecord()
{
    _depura ( "BlDbRecord::~BlDbRecord", 0 );
    m_lista.clear();
    _depura ( "END BlDbRecord::~BlDbRecord", 0 );
}

/// Establece la base de datos que debe utilizar la clase.
/**
Esta clase funciona siempre ligada a una base de datos. Esta se especifica
en el constructor de la clase.
\param comp Puntero a la base de datos que debe utilizar la clase.
**/
void BlDbRecord::setconexionbase ( BlMainCompany *comp )
{
    _depura ( "BlDbRecord::setconexionbase", 0 );
    m_conexionbase = comp;
    _depura ( "END BlDbRecord::setconexionbase", 0 );
}


/// Devuelve el puntero de la base de datos con la que ha sido inicializada la clase.
/**
\return Puntero a la base de datos con la que se ha inicializado la clase.
**/
BlMainCompany *BlDbRecord::conexionbase()
{
    _depura ( "BlDbRecord::conexionbase", 0 );
    _depura ( "END BlDbRecord::conexionbase", 0 );
    return m_conexionbase;
}


/// Establece cual es la tabla por defecto que se va a usar para hacer las operaciones.
/**
Esta clase funciona siempre con una tabla especificada. De esta forma todas las cargas, inserciones
y modificaciones se hacen sobre la tabla que se haya especificado.
\param nom Nombre de la tabla de la base de datos que se usa por defecto.
**/
void BlDbRecord::setDbTableName ( QString nom )
{
    _depura ( "BlDbRecord::setDbTableName", 0 );
    m_tablename = nom;
    _depura ( "END BlDbRecord::setDbTableName", 0 );
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
    _depura ( "BlDbRecord::setNuevo", 0 );
    m_nuevoCampo = n;
    _depura ( "END BlDbRecord::setNuevo", 0 );
}


/// Devuelve el nombre de la tabla de la base de datos con la que se opera por defecto
/**
\return El nombre de la tabla que usa la clase al  operar con la base de datos
**/
QString BlDbRecord::tableName()
{
    _depura ( "BlDbRecord::tableName", 0 );
    _depura ( "END BlDbRecord::tableName", 0 );
    return m_tablename;
}


/// Devuelve el nombre del campo clave primaria en la tabla por defecto.
/**
\return El nombre del campo que se considera Identificador. (Clave Primaria)
**/
QString BlDbRecord::campoId()
{
    _depura ( "BlDbRecord::campoId", 0 );
    _depura ( "END BlDbRecord::campoId", 0 );
    return m_campoid;
}


/// Establece el nombre del campo identificador en la tabla por defecto.
/**
\param nom El nombre del campo que se considera la clave primaria
**/
void BlDbRecord::setDbFieldId ( QString nom )
{
    _depura ( "BlDbRecord::setDbFieldId", 0 );
    m_campoid = nom;
    _depura ( "END BlDbRecord::setDbFieldId", 0 );
}


/// Devuelve la lista de campos con los que esta operando la clase.
/**
Normalmente coincide esta lista con la definicion de la tabla con la que opera.
\return Lista de campos con los que se esta operando.
**/
QList<BlDbField *> *BlDbRecord::lista()
{
    _depura ( "BlDbRecord::lista", 0 );
    _depura ( "END BlDbRecord::lista", 0 );
    return &m_lista;
}


/// Vacia la clase para que no tenga parametros por defecto.
/**
**/
void BlDbRecord::vaciar()
{
    _depura ( "BlDbRecord::vaciar", 0 );
    DBclear();
    _depura ( "END BlDbRecord::vaciar", 0 );
}



/// Efectura la carga de un Registro de la base de datos a partir de un recorset (\class BlDbRecordSet).
/**
\param cur recordset con resultados.
\return Si no hay errores devuelve 0. En caso contrario devuelve otro valor.
**/
int BlDbRecord::DBload ( BlDbRecordSet *cur )
{
    _depura ( "BlDbRecord::DBload", 0 );
    try {
        m_nuevoCampo = FALSE;

        BlDbField *campo;
        int error = 0;
        for ( int i = 0; i < m_lista.size(); ++i ) {
            campo = m_lista.at ( i );
            if ( ! ( campo->restrictcampo() & BlDbField::DBNoLoad ) ) {
                QString nom = campo->nomcampo();
                QString val = cur->valor ( nom );
                if ( ( campo->restrictcampo() & BlDbField::DBPrimaryKey ) && ( val == "" ) )
                    m_nuevoCampo = TRUE;
                if ( ( campo->restrictcampo() & BlDbField::DBDupPrimaryKey ) && ( val == "" ) )
                    m_nuevoCampo = TRUE;
                error += campo->set ( val );
                /// Al ser una carga consideramos que los cambios estan inicializados.
                campo->resetCambio();
            } // end if
        } // end for
        _depura ( "END BlDbRecord::DBload", 0 );
        return error;
    } catch ( ... ) {
        _depura ( "EXCEPTION BlDbRecord::DBload", 0 );
        return -1;
    } // end try
}


/// Inicializa la clase limpiando posibles componentes utilizados con anterioridad.
/**
**/
void BlDbRecord::DBclear()
{
    _depura ( "BlDbRecord::DBclear", 0 );
    m_nuevoCampo = TRUE;
    BlDbField *campo;
    for ( int i = 0; i < m_lista.size(); ++i ) {
        campo = m_lista.at ( i );
        campo->set ( "" );
    } // end for
    _depura ( "END BlDbRecord::DBclear", 0 );
}


/// Guarda el registro actual contenido en la clase en la base de datos.
/**
\param id Devuelve el identificador (primary key) con que ha quedado guardado el registro.
**/
int BlDbRecord::DBsave ( QString &id )
{
    _depura ( "BlDbRecord::DBsave", 0, id );
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
            if ( campo->restrictcampo() & BlDbField::DBDupPrimaryKey ) {
                QString lin = campo->valorcampoprep ( err );
                if ( err )
                    throw ( err );
                querywhere += separadorwhere + campo->nompresentacion() + " = " + lin;
                separadorwhere = " AND ";
            } // end if

            /// Si el campo es requerido y no esta entonces salimos sin dar error.
            /// No es lo mismo que los not null ya que estos si dan error
            if ( ! ( campo->restrictcampo() & BlDbField::DBNoSave ) ) {
                if ( campo->restrictcampo() & BlDbField::DBRequired ) {
                    if ( campo->valorcampo() == "" ) {
                        _depura ( "END BlDbRecord::DBsave", 0, "Campo requerido vacio" );
                        return 0;
                    } // end if
                } // end if
                if ( campo->restrictcampo() & BlDbField::DBPrimaryKey ) {
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
            m_conexionbase->ejecuta ( query );
            _depura ( query, 0 );
            BlDbRecordSet *cur = m_conexionbase->loadQuery ( "SELECT " + m_campoid + " FROM " + m_tablename + " ORDER BY " + m_campoid + " DESC LIMIT 1" );
            id = cur->valor ( m_campoid );
            delete cur;
        } else {
            QString query = "UPDATE " + m_tablename + " SET " + queryupdate + " WHERE " + querywhere;
            _depura ( query, 0 );
            m_conexionbase->ejecuta ( query );
        } // end if
        m_nuevoCampo = FALSE;

	/// Emitimos la senyal apropiada en el qapplication2
	theApp->tablaCambiada1(m_tablename);

    } catch ( int error ) {
        _depura ( "END BlDbRecord::DBsave", 0, "Error de guardado" );
        throw error;
    } // end try
    _depura ( "END BlDbRecord::DBSave", 0 );
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
    _depura ( "BlDbRecord::setDbValue", 0 );
    BlDbField *campo;
    int error = 0;
    int i = 0;
    campo = m_lista.value ( i );

    while ( campo && campo->nomcampo() != nomb ) {
        campo = m_lista.value ( ++i ) ;
    } // end while

    if ( !campo ) {
        _depura ( "Campo " + nomb + " no encontrado", 2 );
        return -1;
    } // end if

    if ( campo->nomcampo() == nomb ) {
        error = campo->set ( valor );
    } // end if

    _depura ( "END BlDbRecord::setDbValue", 0 );
    return error;
}


/// Consulta el valor que tiene un campo del registro actual
/**
\param nomb Nombre del campo que queremos consultar.
\return Valor que contiene el campo.
**/
QString BlDbRecord::DBvalue ( QString nomb )
{
    _depura ( "BlDbRecord::DBvalue", 0, nomb );
    BlDbField *campo;
    QString valor = "";
    int i = 0;

    /// Recorremos la lista en busqueda del campo.
    campo = m_lista.value ( i );
    while ( campo && campo->nomcampo() != nomb )
        campo = m_lista.value ( ++i );

    if ( !campo ) {
        _depura ( "Campo " + nomb + " no encontrado", 2 );
    } else if ( campo->nomcampo() == nomb ) {
        valor = campo->valorcampo();
    } // end if
    _depura ( "END BlDbRecord::DBvalue", 0, nomb );
    return valor;
}


/// Indica si un campo esta inicializado en el registro o no.
/**
\param nomb Campo del que queremos saber si existe o no
\return TRUE si existe o FALSE si no existe
**/
bool BlDbRecord::exists ( QString nomb )
{
    _depura ( "BlDbRecord::exists", 0, nomb );
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
    _depura ( "END BlDbRecord::exists", 0, nomb );
    return existe;
}


/// Prepara un campo para un eventual almacenado en la base de datos.
/**
\param nomb campo a preparar para ser guardado
\return El valor del campo una vez preprocesado para el guardado.
**/
QString BlDbRecord::DBvalueprep ( QString nomb )
{
    _depura ( "BlDbRecord::DBvalueprep", 0 );
    BlDbField *campo;
    int i = 0;
    campo = m_lista.value ( i );
    while ( campo && campo->nomcampo() != nomb )
        campo = m_lista.value ( ++i );
    if ( !campo ) {
        mensajeAviso ( "No se ha encontrado el campo '" + nomb + "'." );
        return "";
    } // end if
    if ( campo->nomcampo() == nomb ) {
        int err;
        return campo->valorcampoprep ( err );
    } // end if
    _depura ( "END BlDbRecord::DBvalueprep", 0 );
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
int BlDbRecord::addDbField ( QString nom, BlDbField::dbtype typ, int res, QString nomp = "" )
{
    _depura ( "BlDbRecord::addDbField", 0 );
    BlDbField *camp = new BlDbField ( m_conexionbase, nom, typ, res, nomp );
    camp->set
    ( "" );
    m_lista.append ( camp );
    _depura ( "END BlDbRecord::addDbField", 0 );
    return 0;
}


/// Elimina el registro que tenemos de la base de dato (Si es que existe)
/**
\return si no se han producido errores devuelve 0 en caso contrario devuelve -1
**/
int BlDbRecord::borrar()
{
    _depura ( "BlDbRecord::borrar", 0 );
    try {
        BlDbField *campo;
        QString separadorwhere = "";
        QString querywhere = "";

        for ( int i = 0; i < m_lista.size(); ++i ) {
            campo = m_lista.at ( i );
            if ( campo->restrictcampo() & BlDbField::DBDupPrimaryKey ) {
                int err;
                QString lin = campo->valorcampoprep ( err );
                if ( err )
                    throw - 1;
                querywhere += separadorwhere + campo->nompresentacion() + " = " + lin;
                separadorwhere = " AND ";
            } // end if

            if ( ! ( campo->restrictcampo() & BlDbField::DBNoSave ) ) {
                if ( campo->restrictcampo() & BlDbField::DBPrimaryKey ) {
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
            m_conexionbase->ejecuta ( "DELETE FROM " + m_tablename + " WHERE " + querywhere );
        } // end if

        _depura ( "END BlDbRecord::borrar", 0 );
        return 0;
    } catch ( ... ) {
        mensajeInfo ( "se produjo un error al borrar el elemento" );
        _depura ( "BlDbRecord::borrar() Error al borrar elemento", 3 );
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
    _depura ( "BlDbRecord::guardar", 0 );
    QString id;
    try {
        DBsave ( id );
        setDbValue ( m_campoid, id );
        _depura ( "END BlDbRecord::guardar", 0 );
        return 0;
    } catch ( ... ) {
        mensajeError ( "BlDbRecord:: Se ha producido un error al guardar los datos." );
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
    _depura ( "BlDbRecord::cargar", 0 );
    try {
        QString query = "SELECT * FROM " + m_tablename + " WHERE " + m_campoid + " = " + id;
        BlDbRecordSet *cur = m_conexionbase->loadQuery ( query );
        if ( !cur ) throw - 1;
        if ( !cur->eof() ) {
            DBload ( cur );
        } // end if
        delete cur;
        _depura ( "END BlDbRecord::cargar", 0 );
        return 0;
    } catch ( ... ) {
        mensajeError ( "BlDbRecord::cargar Se ha producido un error al cargar el registro." );
        return -1;
    } // end try
}

void BlDbRecord::substrConf( QString &buff) {
    ///\TODO: Este tratamiento esta repetido en BlForm::trataTags y en PedidoProveedorView::imprimir.
    ///       Se puede simplificar?
    /// Tratamos la sustitucion de los valores de configuracion.
    for ( int i = 0; i < 500; i++ ) {
        if ( confpr->nombre ( i ) != "" ) {
            buff.replace ( "[" + confpr->nombre ( i ) + "]", confpr->valor ( i ) );
        } // end if
    } // end for
}

int BlDbRecord::trataTags ( QString &buff, int tipoEscape ) {
    QString fitxersortidatxt = "";

    substrConf(buff); 
    buff.replace ( "[ficha]", m_tablename );
    buff.replace ( "[story]", story() );

  return 1;
}


/// Este metodo es usado en las impresiones con RML para generar una cuadricula con el registro.
QString BlDbRecord::story ( void ) {
  
    QString fitxersortidatxt = "";
    BlDbField *campo;
    QLocale spanish ( QLocale::Spanish, QLocale::Spain );
    /// Impresion de la tabla de contenidos.
    for ( int i = 0; i < m_lista.size(); ++i ) {
        campo = m_lista.at ( i );
        fitxersortidatxt += "<tr>\n";
        fitxersortidatxt += "   <td>" + xmlEscape(campo->nomcampo()) + "</td>\n";
        fitxersortidatxt += "   <td>" + xmlEscape(campo->nompresentacion()) + "</td>\n";
        if ( campo->tipo() & BlDbField::DBnumeric )
            fitxersortidatxt += "   <td>" + xmlEscape(spanish.toString ( campo->valorcampo().toDouble(), 'f', 2 )) + "</td>\n";
        else
            fitxersortidatxt += "   <td>" + xmlEscape(campo->valorcampo()) + "</td>\n";
        fitxersortidatxt += "</tr>";
        qDebug() << spanish.toString ( campo->valorcampo().toDouble(), 'f', 2 );
    } // end for

  return fitxersortidatxt;
}

int BlDbRecord::generaRML ( const QString &arch )
{
    _depura ( "BlDbRecord::generaRML", 0 );

    /// Disparamos los plugins
    int res = g_plugins->lanza ( "BlDbRecord_generaRML", this );
    if ( res != 0 ) {
        return 1;
    } // end if

    /// Dependiendo del tipo de archivo que se trate usaremos un parseo u otro.
    int tipoescape = 0;
    if (arch.right(3) == "rml" || arch.right(3) == "xml" ) 
	tipoescape = 1;
    else if (arch.right(3) == "pys")
	tipoescape = 2;


    QString archivo = confpr->valor ( CONF_DIR_OPENREPORTS ) + arch;
    QString archivod = confpr->valor ( CONF_DIR_USER ) + arch;
    QString archivologo = confpr->valor ( CONF_DIR_OPENREPORTS ) + "logo.jpg";

    /// Copiamos el archivo.
#ifdef WINDOWS

    archivo = "copy " + archivo + " " + archivod;
#else

    archivo = "cp " + archivo + " " + archivod;
#endif

    system ( archivo.toAscii().constData() );
    /// Copiamos el logo
#ifdef WINDOWS

    archivologo = "copy " + archivologo + " " + confpr->valor ( CONF_DIR_USER ) + "logo.jpg";
#else

    archivologo = "cp " + archivologo + " " + confpr->valor ( CONF_DIR_USER ) + "logo.jpg";
#endif

    system ( archivologo.toAscii().constData() );
    QFile file;
    file.setFileName ( archivod );
    file.open ( QIODevice::ReadOnly );
    bool ascii=FALSE; 
    /// Antes de abrir un fichero como QTextStream debemos saber en que codificacion 
    /// esta, si no leeremos basura (potencialmente). Los ficheros XML deberian 
    /// declararlo en la primera instruccion de proceso (<?xml ... encoding=""?>)
    /// Si no lo declaran deberian ser UTF-8. Para parsear la instruccion de proceso
    /// uso QXmlStreamReader. Para el resto del fichero dejo lo que estaba 
    /// hecho, que trabaja sobre una QString. Para pasarlo a SAX, DOM o QXmlStreamReader
    /// habria que cambiarlo todo y no se si vale la pena.
    QXmlStreamReader xmlReader(&file);
    while ((!xmlReader.atEnd())&&(!xmlReader.isStartDocument())) {
          xmlReader.readNext();
    };
    QTextCodec *codec=NULL;
    if (xmlReader.isStartDocument()) {
         _depura("El fitxer " + archivod + " te una codificacio ("+     
                     xmlReader.documentEncoding().toString()+") ",0);
         codec=QTextCodec::codecForName(xmlReader.documentEncoding().toString().toUtf8());
         if (!codec) { // sembla que no va, per UTF32 (=UCS4)
             _depura("El fitxer " + archivod + " te una codificacio ("+  
                     xmlReader.documentEncoding().toString()+") que no entenem",2);
//             file.close(); 
//             return 0;
	       codec=QTextCodec::codecForName("UTF-8");
         } else {
            _depura("Usarem un codec per a ",0,codec->name());
         }// end if
    } else {
         if (ascii=xmlReader.hasError()) {
            _depura("El fitxer " + archivod + " no l'hem pogut llegir be i no sabem quina codificacio te. S'imaginarem que es ASCII per si de cas");
         }// end if
         _depura("El fitxer " + archivod + " no se quina codificacio te. Deu ser UTF-8",0);
	 codec=QTextCodec::codecForName("UTF-8");
         // esto mantendria compatibilidad con el codigo anterior, pero 
         // me parece que no queremos porque el codigo anterior no admitia
         // contenidos no ASCII (bgtrml2pdf petaba) y los contenidos ASCII ya funcionan aunque 
         // los trates como UTF-8 
         // encoding=QtextCodec::codecForLocale();
    }// end if
    file.close();
    file.open ( QIODevice::ReadOnly );
    QTextStream stream ( &file );
    stream.setCodec(codec);
    QString buff = stream.readAll();
    file.close();

    /// Hacemos el tratamiento avanzado de TAGS
    if (!trataTags ( buff, tipoescape )) {
	return 0;
    } // end if

    if ( file.open ( QIODevice::WriteOnly ) ) {
        QTextStream stream ( &file );
        stream.setCodec(codec);
        if ((!ascii)&&(codec->canEncode(buff))) {  
           // para ficheros UTF-8, UTF-16, UTF-32 asi vale
           // para otros sin caracteres ajenos al encoding tambien vale
	   _depura("Llistat sense referències de caracters",0," ");
           stream << buff;
        } else { // para otros con caracteres no codificables 
                 // tenemos que usar referencias numericas de caracteres de XML
	   _depura("Llistat amb referències de caracters",0," ");
           QString::const_iterator i;
           for (i = buff.begin(); i != buff.end(); ++i) {
               if ((codec->canEncode(*i))&&((!ascii)||((*i).unicode()<128)) ) {
                  stream << *i; // si puedo lo escribo
               } else { // si no pogo referencia numerica de caracter &12345;
                    uint codepoint = 0; 
                    // el caracter puede no caber en un QChar.
                    if ((*i).isHighSurrogate()) {
                         // sospecho que o este caso o el siguiente nunca se
                         // dara pero no lo se seguro y si es asi no se cual
                         codepoint = QChar::surrogateToUcs4(*i,*++i);
                    }  else {
                       if ((*i).isLowSurrogate()) {
                         // sospecho que o este caso o el anterior nunca se
                         // dara pero no lo se seguro y si es asi no se cual
                        codepoint = QChar::surrogateToUcs4(*++i,*i);
                       } else {
                          // este caso es mas normal, caracter entre 0 i 2^16
                          codepoint = (*i).unicode();
                       }
                    }
                    _depura((QString)"escric "+*i+" com "+codepoint,0);
                    stream << "&#" << codepoint << ";" ;
               }
           }
  	   _depura("END Llistat amb referencies de caracters",0);
        }
        file.close();

    } // end if

    _depura ( "END BlDbRecord::generaRML", 0 );
    return 1;
}

QString BlDbRecord::nombrePlantilla(void) 
{
   return QString("ficha");
}

int BlDbRecord::generaRML ( void ) 
{
   return generaRML( nombrePlantilla() + ".rml" );
}
 
/// Realiza una impresion generica del registro a partir de la plantilla ficha.rml
/**
**/
void BlDbRecord::imprimir()
{
    /// Usa la plantilla ficha.rml para realizar la impresion.
    _depura ( "BlDbRecord::imprimir", 0 );

    if (generaRML()) {
        invocaPDF ( nombrePlantilla());
    }

    _depura ( "END BlDbRecord::imprimir", 0 );
}

