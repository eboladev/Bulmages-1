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

#include "dbrecord.h"
#include "funcaux.h"

#include <QFile>
#include <QTextStream>
#include <QLocale>


///
/**
**/
bool DBCampo::cambiado()
{
    _depura ( "DBCampo::cambiado", 0 );
    _depura ( "END DBCampo::cambiado", 0 );
    return m_valorcampo != m_valorcampoorig;
}


///
/**
**/
void DBCampo::resetCambio()
{
    _depura ( "DBCampo::resetCambio", 0 );
    m_valorcampoorig = m_valorcampo;
    _depura ( "END DBCampo::resetCambio", 0 );
}


///
/**
**/
DBCampo::~DBCampo()
{
    _depura ( "DBCampo::~DBCampo", 0 );
    _depura ( "END DBCampo::~DBCampo", 0 );
}


///
/**
\return
**/
postgresiface2 *DBCampo::conexionbase()
{
    _depura ( "DBCampo::conexionbase", 0 );
    _depura ( "END DBCampo::conexionbase", 0 );
    return m_conexionbase;
}


///
/**
\param comp
**/
void DBCampo::setconexionbase ( postgresiface2 *comp )
{
    _depura ( "DBCampo::setconexionbase", 0 );
    m_conexionbase = comp;
    _depura ( "END DBCampo::setconexionbase", 0 );
}


///
/**
\return
**/
DBCampo::dbtype DBCampo::tipo()
{
    _depura ( "DBCampo::dbtype DBCampo::tipo", 0 );
    _depura ( "END DBCampo::dbtype DBCampo::tipo", 0 );
    return m_tipo;
}


///
/**
\param val
\return
**/
int DBCampo::set ( QString val )
{
    _depura ( "DBCampo::set", 0 );
    m_valorcampo = val;
    _depura ( "END DBCampo::set", 0 );
    return 0;
}


///
/**
\return
**/
int DBCampo::restrictcampo()
{
    _depura ( "DBCampo::restrictcampo", 0 );
    _depura ( "END DBCampo::restrictcampo", 0 );
    return m_restrict;
}


///
/**
\return
**/
QString DBCampo::nomcampo()
{
    _depura ( "DBCampo::nomcampo", 0 );
    _depura ( "END DBCampo::nomcampo", 0 );
    return m_nomcampo;
}


///
/**
\return
**/
QString DBCampo::nompresentacion()
{
    _depura ( "DBCampo::nompresentacion", 0 );
    _depura ( "END DBCampo::nompresentacion", 0 );
    return m_nompresentacion;
}


///
/**
\return
**/
QString DBCampo::valorcampo()
{
    _depura ( "DBCampo::valorcampo", 0 );
    _depura ( "END DBCampo::valorcampo", 0 );
    return m_valorcampo;
}


///
/**
\param com
\param nom
\param typ
\param res
\param nomp
**/
DBCampo::DBCampo ( postgresiface2 *com, QString nom, dbtype typ, int res, QString nomp )
{
    _depura ( "DBCampo::DBCampo", 0 );
    m_conexionbase = com;
    m_nomcampo = nom;
    m_tipo = typ;
    m_restrict = res;
    m_nompresentacion = nomp;
    m_valorcampo = "";
    m_valorcampoorig = "";
    _depura ( "END DBCampo::DBCampo", 0 );
}


///
/**
\param error
\return
**/
QString DBCampo::valorcampoprep ( int &error )
{
    _depura ( "DBCampo::valorcampoprep", 0 );
    error = 0;
    if ( ( m_restrict & DBNotNull ) && ! ( m_restrict & DBAuto ) ) {
        if ( m_valorcampo == "" ) {
            mensajeAviso ( "El campo '" + m_nompresentacion + "' no puede estar vacio." );
            error = -1;
            return "";
        } // end if
    } // end if
    switch ( m_tipo ) {
    case DBint:
        if ( m_valorcampo == "" )
            return "NULL";
        m_valorcampo.replace ( ",", "." );
        return "'" + m_conexionbase->sanearCadena ( m_valorcampo ) + "'";
    case DBvarchar:
        if ( m_valorcampo == "" )
            return "NULL";
        return "'" + m_conexionbase->sanearCadena ( m_valorcampo ) + "'";
    case DBdate:
        if ( m_valorcampo == "" )
            return "NULL";
        return "'" + m_conexionbase->sanearCadena ( m_valorcampo ) + "'";
    case DBnumeric:
        if ( m_valorcampo == "" )
            return "NULL";
        m_valorcampo.replace ( ",", "." );
        return "'" + m_conexionbase->sanearCadena ( m_valorcampo ) + "'";
    case DBboolean:
        if ( m_valorcampo == "" )
            return "NULL";
        if ( m_valorcampo == "f" || m_valorcampo == "t" )
            return "'" + m_conexionbase->sanearCadena ( m_valorcampo ) + "'";
        return m_conexionbase->sanearCadena ( m_valorcampo );
    } // end switch
    error = -1;
    _depura ( "Error en la conversion de tipos", 2 );
    _depura ( "END DBCampo::valorcampoprep", 10 );
    return "";
}


/// Constructor de la clase que toma por defecto la base de datos con la que va a operar.
/**
\param con Puntero a la base de datos con la que vamos a trabajar.
**/
DBRecord::DBRecord ( EmpresaBase *con )
{
    _depura ( "DBRecord::DBRecord", 0 );
    m_conexionbase = con;
    m_nuevoCampo = TRUE;
    _depura ( "END DBRecord::DBRecord", 0 );
}


/// Destructor de la clase
/**
Limpia y libera toda la memoria que se haya podido reservar.
Esta operacion no contempla que haya habido cambios en el registro sin guardar.
**/
DBRecord::~DBRecord()
{
    _depura ( "DBRecord::~DBRecord", 0 );
    m_lista.clear();
    _depura ( "END DBRecord::~DBRecord", 0 );
}

/// Establece la base de datos que debe utilizar la clase.
/**
Esta clase funciona siempre ligada a una base de datos. Esta se especifica 
en el constructor de la clase.
\param comp Puntero a la base de datos que debe utilizar la clase.
**/
void DBRecord::setconexionbase ( EmpresaBase *comp )
{
    _depura ( "DBRecord::setconexionbase", 0 );
    m_conexionbase = comp;
    _depura ( "END DBRecord::setconexionbase", 0 );
}


/// Devuelve el puntero de la base de datos con la que ha sido inicializada la clase.
/**
\return Puntero a la base de datos con la que se ha inicializado la clase.
**/
EmpresaBase *DBRecord::conexionbase()
{
    _depura ( "DBRecord::conexionbase", 0 );
    _depura ( "END DBRecord::conexionbase", 0 );
    return m_conexionbase;
}


/// Establece cual es la tabla por defecto que se va a usar para hacer las operaciones.
/**
Esta clase funciona siempre con una tabla especificada. De esta forma todas las cargas, inserciones
y modificaciones se hacen sobre la tabla que se haya especificado.
\param nom Nombre de la tabla de la base de datos que se usa por defecto.
**/
void DBRecord::setDBTableName ( QString nom )
{
    _depura ( "DBRecord::setDBTableName", 0 );
    m_tablename = nom;
    _depura ( "END DBRecord::setDBTableName", 0 );
}


/// Establece el tipo de operacion que se realizara en la base de datos.
/**
Si se establece TRUE entonces se hará una inserción del registro en la tabla por
defecto. Si por el contrario se establece como FALSE se hará una modificacion de los
registros que coincidan en sus PrimaryKey con los datos del registro.
 
Este metodo es de uso interno. Es la propia clase la que regula si es una insercion
o no dependiendo de si se hecho una carga previa del registro.
\param n Registro para insertar o para modificar.
**/
void DBRecord::setNuevo ( bool n )
{
    _depura ( "DBRecord::setNuevo", 0 );
    m_nuevoCampo = n;
    _depura ( "END DBRecord::setNuevo", 0 );
}


/// Devuelve el nombre de la tabla de la base de datos con la que se opera por defecto
/**
\return El nombre de la tabla que usa la clase al  operar con la base de datos
**/
QString DBRecord::tableName()
{
    _depura ( "DBRecord::tableName", 0 );
    _depura ( "END DBRecord::tableName", 0 );
    return m_tablename;
}


/// Devuelve el nombre del campo clave primaria en la tabla por defecto.
/**
\return El nombre del campo que se considera Identificador. (Clave Primaria)
**/
QString DBRecord::campoId()
{
    _depura ( "DBRecord::campoId", 0 );
    _depura ( "END DBRecord::campoId", 0 );
    return m_campoid;
}


/// Establece el nombre del campo identificador en la talba por defecto.
/**
\param nom El nombre del campo que se considera la clave primaria
**/
void DBRecord::setDBCampoId ( QString nom )
{
    _depura ( "DBRecord::setDBCampoId", 0 );
    m_campoid = nom;
    _depura ( "END DBRecord::setDBCampoId", 0 );
}


/// Devuelve la lista de campos con los que esta operando la clase.
/**
Normalmente coincide esta lista con la definicion de la tabla con la que opera.
\return Lista de campos con los que se esta operando.
**/
QList<DBCampo *> *DBRecord::lista()
{
    _depura ( "DBRecord::lista", 0 );
    _depura ( "END DBRecord::lista", 0 );
    return &m_lista;
}


/// Vacia la clase para que no tenga parametros por defecto.
/**
**/
void DBRecord::vaciar()
{
    _depura ( "DBRecord::vaciar", 0 );
    DBclear();
    _depura ( "END DBRecord::vaciar", 0 );
}



/// Efectura la carga de un Registro de la base de datos a partir de un recorset (\class cursor2).
/**
\param cur recordset con resultados.
\return Si no hay errores devuelve 0. En caso contrario devuelve otro valor.
**/
int DBRecord::DBload ( cursor2 *cur )
{
    _depura ( "DBRecord::DBload", 0 );
    try {
        m_nuevoCampo = FALSE;

        DBCampo *campo;
        int error = 0;
        for ( int i = 0; i < m_lista.size(); ++i ) {
            campo = m_lista.at ( i );
            if ( ! ( campo->restrictcampo() & DBCampo::DBNoLoad ) ) {
                QString nom = campo->nomcampo();
                QString val = cur->valor ( nom );
                if ( ( campo->restrictcampo() & DBCampo::DBPrimaryKey ) && ( val == "" ) )
                    m_nuevoCampo = TRUE;
                if ( ( campo->restrictcampo() & DBCampo::DBDupPrimaryKey ) && ( val == "" ) )
                    m_nuevoCampo = TRUE;
                error += campo->set ( val );
                /// Al ser una carga consideramos que los cambios estan inicializados.
                campo->resetCambio();
            } // end if
        } // end for
        _depura ( "END DBRecord::DBload", 0 );
        return error;
    } catch ( ... ) {
        _depura ( "EXCEPTION DBRecord::DBload", 0 );
        return -1;
    } // end try
}


/// Inicializa la clase limpiando posibles componentes utilizados con anterioridad.
/**
**/
void DBRecord::DBclear()
{
    _depura ( "DBRecord::DBclear", 0 );
    m_nuevoCampo = TRUE;
    DBCampo *campo;
    for ( int i = 0; i < m_lista.size(); ++i ) {
        campo = m_lista.at ( i );
        campo->set ( "" );
    } // end for
    _depura ( "END DBRecord::DBclear", 0 );
}


/// Guarda el registro actual contenido en la clase en la base de datos.
/**
\param id Devuelve el identificador (primary key) con que ha quedado guardado el registro.
**/
int DBRecord::DBsave ( QString &id )
{
    _depura ( "DBRecord::DBsave - " + id, 0 );
    try {
        DBCampo *campo;
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
            if ( campo->restrictcampo() & DBCampo::DBDupPrimaryKey ) {
                QString lin = campo->valorcampoprep ( err );
                if ( err )
                    throw ( -1 );
                querywhere += separadorwhere + campo->nompresentacion() + " = " + lin;
                separadorwhere = " AND ";
            } // end if

            /// Si el campo es requerido y no esta entonces salimos sin dar error.
            /// No es lo mismo que los not null ya que estos si dan error
            if ( ! ( campo->restrictcampo() & DBCampo::DBNoSave ) ) {
                if ( campo->restrictcampo() & DBCampo::DBRequired ) {
                    if ( campo->valorcampo() == "" )
                        return 0;
                } // end if
                if ( campo->restrictcampo() & DBCampo::DBPrimaryKey ) {
                    QString lin = campo->valorcampoprep ( err );
                    if ( err )
                        throw - 1;
                    querywhere += separadorwhere + campo->nomcampo() + " = " + lin;
                    separadorwhere = " AND ";
                } // end if
                if ( campo->valorcampoprep ( err ) != "" ) {
                    queryupdate += separador1 + campo->nomcampo() + "=" + campo->valorcampoprep ( err );
                    separador1 = ", ";
                } // end if
                if ( err )
                    throw - 1;
                if ( ( campo->valorcampoprep ( err ) != "NULL" ) && ( campo->valorcampoprep ( err ) != "" ) ) {
                    listcampos += separador + campo->nomcampo();
                    listvalores += separador + campo->valorcampoprep ( err );
                    if ( err )
                        throw - 1;
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
            cursor2 *cur = m_conexionbase->cargacursor ( "SELECT " + m_campoid + " FROM " + m_tablename + " ORDER BY " + m_campoid + " DESC LIMIT 1" );
            id = cur->valor ( m_campoid );
            delete cur;
        } else {
            QString query = "UPDATE " + m_tablename + " SET " + queryupdate + " WHERE " + querywhere;
            _depura ( query, 0 );
            m_conexionbase->ejecuta ( query );
        } // end if
        m_nuevoCampo = FALSE;
        return 0;
    } catch ( ... ) {
        _depura ( "EXCEPTION DBRecord::DBsave", 0 );
        throw - 1;
    } // end try
    _depura ( "END DBRecord::DBSave", 0 );
}


/// Establece un nuevo valor para un campo del registro con el que estamos tratando.
/**
\param nomb Nombre del campo cuyo valor vamos a establecer.
\param valor Valor que toma el campo.
\return
**/
int DBRecord::setDBvalue ( QString nomb, QString valor )
{
    _depura ( "DBRecord::setDBvalue", 0 );
    DBCampo *campo;
    int error = 0;
    int i = 0;
    campo = m_lista.value ( i );
    while ( campo && campo->nomcampo() != nomb )
        campo = m_lista.value ( ++i ) ;
    if ( !campo ) {
        _depura ( "Campo " + nomb + " no encontrado", 2 );
        return -1;
    } // end if
    if ( campo->nomcampo() == nomb )
        error = campo->set ( valor );
    _depura ( "END DBRecord::setDBvalue", 0 );
    return error;
}


/// Consulta el valor que tiene un campo del registro actual
/**
\param nomb Nombre del campo que queremos consultar.
\return Valor que contiene el campo.
**/
QString DBRecord::DBvalue ( QString nomb )
{
    _depura ( "DBRecord::value", 0, nomb );
    DBCampo *campo;
    int i = 0;
    campo = m_lista.value ( i );
    while ( campo && campo->nomcampo() != nomb )
        campo = m_lista.value ( ++i );
    if ( !campo ) {
        _depura ( "Campo " + nomb + " no encontrado", 2 );
        return "";
    } // end if
    if ( campo->nomcampo() == nomb ) {
        return campo->valorcampo();
    } // end if
    _depura ( "END DBRecord::value", 0, nomb );
    return "";
}


/// Indica si un campo esta inicializado en el registro o no.
/**
\param nomb Campo del que queremos saber si existe o no
\return TRUE si existe o FALSE si no existe
**/
bool DBRecord::exists ( QString nomb )
{
    _depura ( "DBRecord::exists", 0, nomb );
    DBCampo *campo;
    int i = 0;
    campo = m_lista.value ( i );
    while ( campo && campo->nomcampo() != nomb )
        campo = m_lista.value ( ++i );
    if ( !campo ) {
        return FALSE;
    } // end if
    if ( campo->nomcampo() == nomb ) {
        return TRUE;
    } // end if
    _depura ( "END DBRecord::exists", 0, nomb );
    return FALSE;
}


/// Prepara un campo para un eventual almacenado en la base de datos.
/**
\param nomb campo a preparar para ser guardado
\return El valor del campo una vez preprocesado para el guardado.
**/
QString DBRecord::DBvalueprep ( QString nomb )
{
    _depura ( "DBRecord::DBvalueprep", 0 );
    DBCampo *campo;
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
    _depura ( "END DBRecord::DBvalueprep", 0 );
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
int DBRecord::addDBCampo ( QString nom, DBCampo::dbtype typ, int res, QString nomp = "" )
{
    _depura ( "DBRecord::addDBCampo", 0 );
    DBCampo *camp = new DBCampo ( m_conexionbase, nom, typ, res, nomp );
    camp->set
    ( "" );
    m_lista.append ( camp );
    _depura ( "END DBRecord::addDBCampo", 0 );
    return 0;
}


/// Elimina el registro que tenemos de la base de dato (Si es que existe)
/**
\return si no se han producido errores devuelve 0 en caso contrario devuelve -1
**/
int DBRecord::borrar()
{
    _depura ( "DBRecord::borrar", 0 );
    try {
        DBCampo *campo;
        QString separadorwhere = "";
        QString querywhere = "";

        for ( int i = 0; i < m_lista.size(); ++i ) {
            campo = m_lista.at ( i );
            if ( campo->restrictcampo() & DBCampo::DBDupPrimaryKey ) {
                int err;
                QString lin = campo->valorcampoprep ( err );
                if ( err )
                    throw - 1;
                querywhere += separadorwhere + campo->nompresentacion() + " = " + lin;
                separadorwhere = " AND ";
            } // end if

            if ( ! ( campo->restrictcampo() & DBCampo::DBNoSave ) ) {
                if ( campo->restrictcampo() & DBCampo::DBPrimaryKey ) {
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

        _depura ( "END DBRecord::borrar", 0 );
        return 0;
    } catch ( ... ) {
        mensajeInfo ( "se produjo un error al borrar el elemento" );
        _depura ( "DBRecord::borrar() Error al borrar elemento", 3 );
        throw - 1;
    }
}


/// Guarda el registro actual en la base de datos
/**
Esta funcion, de un nivel algo superior a la llamada DBsave hace el guardado y maneja las 
excepciones que se hayan podido producir
\return si no se producen errores devuelve 0 en caso contrario genera una excepcion
**/
int DBRecord::guardar()
{
    _depura ( "DBRecord::guardar", 0 );
    QString id;
    try {
        DBsave ( id );
        setDBvalue ( m_campoid, id );
        _depura ( "END DBRecord::guardar", 0 );
        return 0;
    } catch ( ... ) {
        mensajeError ( "DBRecord:: Se ha producido un error al guardar los datos." );
        throw -100;
    } // end try
}



/// Esta funcion carga un registro..
/**
\param id Identificador del registro a cargar en la tabla por defecto.
\return Si no hay errores devuelve 0 sino devuelve -1
**/
int DBRecord::cargar ( QString id )
{
    _depura ( "DBRecord::cargar", 0 );
    try {
        QString query = "SELECT * FROM " + m_tablename + " WHERE " + m_campoid + " = " + id;
        cursor2 *cur = m_conexionbase->cargacursor ( query );
        if ( !cur ) throw - 1;
        if ( !cur->eof() ) {
            DBload ( cur );
        } // end if
        delete cur;
        _depura ( "END DBRecord::cargar", 0 );
        return 0;
    } catch ( ... ) {
        mensajeError ( "DBRecord::cargar Se ha producido un error al cargar el registro." );
        return -1;
    } // end try
}


/// Realiza una impresion generica del registro a partir de la plantilla ficha.rml
/**
**/
void DBRecord::imprimir()
{
    /// Impresion de un Pedido de Proveedor
    /** Usa la plantilla pedidoproveedor.rml */
    _depura ( "DBRecord::imprimir", 0 );
    DBCampo *campo;
    QString archivo = confpr->valor ( CONF_DIR_OPENREPORTS ) + "ficha.rml";
    QString archivod = confpr->valor ( CONF_DIR_USER ) + "ficha.rml";
    QString archivologo = confpr->valor ( CONF_DIR_OPENREPORTS ) + "logo.jpg";

    /// Copiamos el archivo.
#ifdef WINDOWS

    archivo = "copy " + archivo + " " + archivod;
#else

    archivo = "cp " + archivo + " " + archivod;
#endif

    system ( archivo.toAscii().constData() );

    /// Copiamos el logo.
#ifdef WINDOWS

    archivologo = "copy " + archivologo + " " + confpr->valor ( CONF_DIR_USER ) + "logo.jpg";
#else

    archivologo = "cp " + archivologo + " " + confpr->valor ( CONF_DIR_USER ) + "logo.jpg";
#endif

    system ( archivologo.toAscii().constData() );

    QFile file;
    file.setFileName ( archivod );
    file.open ( QIODevice::ReadOnly );
    QTextStream stream ( &file );
    QString buff = stream.readAll();
    file.close();
    QString fitxersortidatxt = "";

    /// Impresion de la tabla de contenidos.
    for ( int i = 0; i < m_lista.size(); ++i ) {
        campo = m_lista.at ( i );
        fitxersortidatxt += "<tr>\n";
        fitxersortidatxt += "   <td>" + campo->nomcampo() + "</td>\n";
        fitxersortidatxt += "   <td>" + campo->nompresentacion() + "</td>\n";
        fitxersortidatxt += "   <td>" + campo->valorcampo() + "</td>\n";
        fitxersortidatxt += "</tr>";
    } // end for

    buff.replace ( "[ficha]", m_tablename );
    buff.replace ( "[story]", fitxersortidatxt );

    if ( file.open ( QIODevice::WriteOnly ) ) {
        QTextStream stream ( &file );
        stream << buff;
        file.close();
    } // end if

    invocaPDF ( "ficha" );
    _depura ( "END DBRecord::imprimir", 0 );
}

