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

#ifndef BLDB_H
#define BLDB_H

#include <QString>
#include <QList>

#include "blpostgresqlclient.h"
#include "blfunctions.h"
#include "blwidget.h"


/// Manejo de campos de un registro de la base de datos
/**
Describe el comportamiento programado de un campo de registro de base de datos.
Se usa en conjunción con BlDbRecord y suele ser invocada mediante el metodo \ref addDbField de esta.
Almacena toda la informacion relativa a un campo de un registro de base de datos, nombre, valor y
comportamiento.
Tambien prepara el campo para ser almacenado en la base de datos aunque el guardado real lo hace la
clase \ref BlDbRecord
\todo Esta clase deberia derivar de \ref BlMainCompanyPointer
*/
class BL_EXPORT BlDbField
{
public:
    /**
    Indica el tipo de dato al que pertenece el campo.
    - DbInt  . Entero
    - DbVarChar . String de longitud indeterminada.
    - DbDate . Fecha en formato Europeo (dd/mm/yyyy)
    - DbNumeric . Numero en punto fijo
    - DbBoolean . Valor Booleano (TRUE / FALSE) (t / f)
    */
    enum DbType {DbInt = 1, DbVarChar = 2, DbDate = 3, DbNumeric = 4, DbBoolean = 5};
    /** Indica las restricciones que soporta el dato.
    - DbNothing . Ninguna,
    - DbNotNull . No puede ser nulo,
    - DbPrimaryKey . Es clave primaria,
    - DbNoSave . No se debe almacenenar en la base de datos.
    - DbAuto . Es autonumerico,
    - DbDupPrimaryKey . Duplicado de la clave primaria (util para aquellos casos en que la clave principal
      puede ser modificada). En este caso el valor del campo comentario es el de la llave a la que duplica.
    - DbRequired . Requerido,
    - DbNoLoad . No debe cargarse desde la base de datos.
    **/
    enum DbRestrict {DbNothing = 0, DbNotNull = 1, DbPrimaryKey = 2,
                     DbNoSave = 4, DbAuto = 8, DbDupPrimaryKey = 16, DbRequired = 32, DbNoLoad = 64
                    };

private:
    QString m_nomcampo;          ///< El nombre del campo
    QString m_valorcampo;         ///< El valor actual del campo, en el registro, no en la base de datos
    QString m_nompresentacion;         ///< El nombre que se mostrara en los mensajes de error y presentacion
    int m_restrict;          ///< Las restricciones del campo
    DbType m_tipo;          ///< El tipo de dato almacenado
    BlPostgreSqlClient *m_dbConnection; ///< Puntero a la base de datos a utilizar
    QString m_valorcampoorig;         ///< indica el valor del campo en un estado anterior para determinar si ha habido cambios.

public:
    BlDbField ( BlPostgreSqlClient *com, QString nom, DbType typ, int res, QString nomp = "" );
    virtual ~BlDbField();
    bool cambiado();
    void resetCambio();
    BlPostgreSqlClient *dbConnection();
    void setDbConnection ( BlPostgreSqlClient *comp );
    DbType dbFieldType();
    virtual int set ( QString val );
    int restrictcampo();
    QString nomcampo();
    QString nompresentacion();
    QString valorcampo();
    QString valorcampoprep ( int &error );
    virtual QString exportXML();
    virtual void syncXML(const QString &);
};


/// Manejo de registros de la base de datos
/**
Sirve para operar de forma sencilla con registros de la base de datos.
Antes de ser utilizada debe inicializarse correctamente con la base de datos que
debe utilizar y la tabla con la que va a operar. Ademas debe describirse como
es la tabla con que se va a operar mediante \ref addDbField
El uso normal de esta clase es la carga de un registro, modificacion de sus campos
y almacenado del mismo.
\todo Esta clase deberia derivar de \ref BlMainCompanyPointer
*/
class BL_EXPORT BlDbRecord
{
protected:
    QList<BlDbField *> m_lista; ///< Lista de campos que conforman la tabla de la BD
    BlMainCompany *m_dbConnection; ///< Puntero a la base de datos con la que se opera
    QString m_tablename; ///< Nombre de la tabla por defecto que se utiliza
    QString m_campoid; ///< Nombre del campo identificador en la tabla
    bool m_nuevoCampo; ///< Indicador sobre si es un nuevo registro o un registro modificado

public:
    BlDbRecord ( BlMainCompany * );
    virtual ~BlDbRecord();
    void setDbConnection ( BlMainCompany *comp );
    BlMainCompany *dbConnection();
    int DBload ( BlDbRecordSet * );
    virtual int DBsave ( QString &id );
    virtual int setDbValue ( QString, QString );
    QString dbValue ( QString );
    bool exists ( QString );
    QString dbValueprep ( QString );
    void setDbTableName ( QString nom );
    void setNuevo ( bool n );
    QString tableName();
    QString fieldId();
    void setDbFieldId ( QString nom );
    int addDbField ( QString, BlDbField::DbType, int, QString );
    void DBclear();
    QList<BlDbField *> *lista();
    virtual int borrar();
    virtual int guardar();
    virtual void vaciar();
    virtual void imprimir();
    virtual int cargar ( QString );
    virtual int trataTags ( QString &buff, int tipoEscape = 0 );
    virtual int trataTags ( QByteArray &buff, int tipoEscape = 0 );
    virtual QString story ( void );
    virtual int generaRML ( const QString & );
    virtual void substrConf( QString &buff) ;
    virtual void substrConf( QByteArray &buff) ;
    virtual QString nombrePlantilla(void);
    virtual int generaRML ( void );
    virtual QString exportXML();
    virtual void syncXML(const QString &);

};

#endif

