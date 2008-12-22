/***************************************************************************
 *   Copyright (C) 2004 by Tomeu Borras Riera                              *
 *   tborras@conetxia.com                                                  *
 *   http://www.iglues.org                                                 *
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

#ifndef DBRECORD_H
#define DBRECORD_H

#include <QString>
#include <QList>

#include "postgresiface2.h"
#include "funcaux.h"
#include "blwidget.h"

/// Manejo de campos de un registro de la base de datos
/**
Describe el comportamiento programado de un campo de registro de base de datos.
Se usa en conjunción con DBRecord y suele ser invocada mediante el metodo \ref addDBCampo de esta.
Almacena toda la información relativa a un campo de un registro de base de datos, nombre, valor y comportamiento
También prepara el campo para ser almacenado en la base de datos aunque el guardado real lo hace la
clase \ref DBRecord
\todo Esta clase deberia derivar de \ref PEmpresaBase
*/
class DBCampo
{
public:
    /**
    Indica el tipo de dato al que pertenece el campo.
    - DBint  . Entero
    - DBvarchar . String de longitud indeterminada.
    - DBdate . Fecha en formato Europeo (dd/mm/yyyy)
    - DBnumeric . Numero en punto fijo
    - DBboolean . Valor Booleano (TRUE / FALSE) (t / f)
    */
    enum dbtype {DBint = 1, DBvarchar = 2, DBdate = 3, DBnumeric = 4, DBboolean = 5};
    /** Indica las restricciones que soporta el dato.
    - DBNothing . Ninguna,
    - DBNotNull . No puede ser nulo,
    - DBPrimaryKey . Es clave primaria,
    - DBNoSave . No se debe almacenenar en la base de datos.
    - DBAuto . Es autonumerico,
    - DBDupPrimaryKey . Duplicado de la clave primaria (util para aquellos casos en que la clave principal puede ser modificada). En este caso el valor del campo comentario es el de la llave a la que duplica.
    - DBRequired . Requerido,
    - DBNoLoad . No debe cargarse desde la base de datos.
    **/
    enum dbrestrict {DBNothing = 0, DBNotNull = 1, DBPrimaryKey = 2,
                     DBNoSave = 4, DBAuto = 8, DBDupPrimaryKey = 16, DBRequired = 32, DBNoLoad = 64
                    };

private:
    QString m_nomcampo;          ///< El nombre del campo
    QString m_valorcampo;         ///< El valor actual del campo, en el registro, no en la base de datos
    QString m_nompresentacion;         ///< El nombre que se mostrara en los mensajes de error y presentacion
    int m_restrict;          ///< Las restricciones del campo
    dbtype m_tipo;          ///< El tipo de dato almacenado
    postgresiface2 *m_conexionbase; ///< Puntero a la base de datos a utilizar
    QString m_valorcampoorig;         ///< indica el valor del campo en un estado anterior para determinar si ha habido cambios.


public:
    DBCampo ( postgresiface2 *com, QString nom, dbtype typ, int res, QString nomp = "" );
    virtual ~DBCampo();
    bool cambiado();
    void resetCambio();
    postgresiface2 *conexionbase();
    void setconexionbase ( postgresiface2 *comp );
    dbtype tipo();
    virtual int set ( QString val );
    int restrictcampo();
    QString nomcampo();
    QString nompresentacion();
    QString valorcampo();
    QString valorcampoprep ( int &error );
};


/// Manejo de registros de la base de datos
/**
Sirve para operar de forma sencilla con registros de la base de datos.
Antes de ser utilizada debe inicializarse correctamente con la base de datos que
debe utilizar y la tabla con la que va a operar. Ademas debe describirse como
es la tabla con que se va a operar mediante \ref addDBCampo
El uso normal de esta clase es la carga de un registro, modificacion de sus campos
y almacenado del mismo.
\todo Esta clase deberia derivar de \ref PEmpresaBase
*/
class DBRecord
{
protected:
    QList<DBCampo *> m_lista; ///< Lista de campos que conforman la tabla de la BD
    EmpresaBase *m_conexionbase; ///< Puntero a la base de datos con la que se opera
    QString m_tablename; ///< Nombre de la tabla por defecto que se utiliza
    QString m_campoid; ///< Nombre del campo identificador en la tabla
    bool m_nuevoCampo; ///< Indicador sobre si es un nuevo registro o un registro modificado

public:
    DBRecord ( EmpresaBase * );
    virtual ~DBRecord();
    void setconexionbase ( EmpresaBase *comp );
    EmpresaBase *conexionbase();
    int DBload ( cursor2 * );
    virtual int DBsave ( QString &id );
    virtual int setDBvalue ( QString, QString );
    QString DBvalue ( QString );
    bool exists ( QString );
    QString DBvalueprep ( QString );
    void setDBTableName ( QString nom );
    void setNuevo ( bool n );
    QString tableName();
    QString campoId();
    void setDBCampoId ( QString nom );
    int addDBCampo ( QString, DBCampo::dbtype, int, QString );
    void DBclear();
    QList<DBCampo *> *lista();
    virtual int borrar();
    virtual int guardar();
    virtual void vaciar();
    virtual void imprimir();
    virtual int cargar ( QString );
    virtual int trataTags ( QString &buff );
    virtual QString story ( void );
    virtual int generaRML ( const QString & );
    virtual void substrConf( QString &buff) ;
    virtual QString nombrePlantilla(void);
    virtual int generaRML ( void );
};

#endif

