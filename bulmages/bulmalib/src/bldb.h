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
    QMap<QString,QString> m_variables;   /// Son elementos que han sido incrustados e inicializados por programas externos, generalmente el RML
    QMap<QString,QString> m_globalvars;   /// Son elementos que han sido incrustados e inicializados por programas externos, generalmente el RML
    
public:
    BlDbRecord ( BlMainCompany * );
    virtual ~BlDbRecord();
    void setDbConnection ( BlMainCompany *comp );
    BlMainCompany *dbConnection();
    int DBload ( BlDbRecordSet * );
    virtual int dbSave ( QString &id );
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
//    virtual int trataTags ( QByteArray &buff, int tipoEscape = 0 );
    virtual QString story ( void );
    virtual int generaRML ( const QString & );
    virtual void substrConf( QString &buff) ;
    virtual void substrConf( QByteArray &buff) ;
    virtual QString nombrePlantilla(void);
    virtual int generaRML ( void );
    virtual QString exportXML();
    virtual void syncXML(const QString &);

    //===========
    virtual int trataTags ( QByteArray &buff, int tipoEscape = 0 );
    virtual int trataTagsPost ( QByteArray &buff, int tipoEscape = 0 );
    virtual QByteArray trataIncludeFileTXT ( const QString &file, int tipoEscape = 0 );
    virtual QByteArray trataIncludeImg ( const QString &file, int tipoEscape = 0 );
    virtual QByteArray trataPngRaw64 ( const QByteArray &data, int tipoEscape = 0 );
    virtual QByteArray trataSetCharacterPrintMode( const QString &param, int tipoEscape = 0 );
    virtual QByteArray trataSetCharacterSpacing( const QString &param, int tipoEscape = 0 );
    virtual QByteArray trataSetCharacterCodeTable( const QString &param, int tipoEscape = 0 );
    virtual QByteArray trataSetUnderlineMode( const QString &param, int tipoEscape = 0 );
    virtual QByteArray trataSetCharacterSize( const QString &param, int tipoEscape = 0 );
    virtual QByteArray trataSetSmoothing( const QString &param, int tipoEscape = 0 );
    virtual QByteArray trataSetDoubleStrike( const QString &param, int tipoEscape = 0 );
    virtual QByteArray trataTurnUpsideDown( const QString &param, int tipoEscape = 0 );
    virtual QByteArray trataTurn90CWRotation( const QString &param, const QString &param1, int tipoEscape = 0 );
    virtual QByteArray trataQuery ( const QString &query, const QByteArray &datos, int tipoEscape = 0 );
    virtual QByteArray trataTurnWhiteBlack( const QString &param, int tipoEscape = 0 );
    virtual QByteArray trataSetColor( const QString &param, int tipoEscape = 0 );
    virtual QByteArray trataHorizontalTab( int tipoEscape = 0 );
    virtual QByteArray trataSetHorizontalTabPos( const QString &param, const QString &param1, int tipoEscape = 0 );
    virtual QByteArray trataSetLeftMargin( const QString &param, int tipoEscape = 0 );
    virtual QByteArray trataSetPrintingAreaWidth( const QString &param, int tipoEscape = 0 );
    virtual QByteArray trataSetJustification( const QString &param, int tipoEscape = 0 );
    virtual QByteArray trataSetHAbsolutePos( const QString &param, int tipoEscape = 0 );
    virtual QByteArray trataSetHRelativePos( const QString &param, int tipoEscape = 0 );
    virtual QByteArray trataSetBarcodeFormat( const QString &param, const QString &param1, const QString &param2, const QString &param3, int tipoEscape = 0 );
    virtual QByteArray trataPrintBarCode( const QString &param, const QString &param1, const QString &param2, int tipoEscape = 0 );
    virtual QByteArray trataSetBarCodeWidth( const QString &param, int tipoEscape = 0 );
    virtual QByteArray trataSetBarCodeHeight( const QString &param, int tipoEscape = 0 );
    virtual QByteArray trataSelectPageMode(int tipoEscape = 0 );
    virtual QByteArray trataSetPrintArea( const QString &param, const QString &param1, const QString &param2, const QString &param3, int tipoEscape = 0 );
    virtual QByteArray trataSetPrintDirection( const QString &param, int tipoEscape = 0 );
    virtual QByteArray trataSetVAbsolutePos( const QString &param, int tipoEscape = 0 );
    virtual QByteArray trataSetVRelativePos( const QString &param, int tipoEscape = 0 );
    virtual QByteArray trataCutPaper( const QString &param, int tipoEscape = 0 );
    virtual QByteArray trataCutPaperAndFeed( const QString &param,  const QString &param1, int tipoEscape = 0 );
    virtual QByteArray trataRightJustified( const QString &param, const QString &param1, const QString &param2, const QString &param3, int tipoEscape = 0 );
    virtual QByteArray trataLeftJustified( const QString &param, const QString &param1, const QString &param2, const QString &param3, int tipoEscape = 0 );
//    virtual QByteArray trataLineasDetalle( const QByteArray &datos, int tipoEscape=0 );
    QByteArray trataCursor ( BlDbRecordSet *cur, const QByteArray &datos, int tipoEscape = 0 );
    virtual QByteArray trataIfQuery ( const QString &query, const QByteArray &datos );
    virtual QByteArray trataIf ( const QString &query, const QByteArray &datos, const QByteArray &datos1 );
    virtual QByteArray trataExists ( const QString &query, const QByteArray &datos );
    void substrVars(QByteArray &buff, int tipoEscape = 0);
    void setVar(const QString &varname, const QString &varvalue);
    void clearVars();
    
};

#endif

