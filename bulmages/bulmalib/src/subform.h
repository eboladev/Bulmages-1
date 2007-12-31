/***************************************************************************
 *   Copyright (C) 2005 by Tomeu Borras Riera                              *
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

#ifndef SUBFORM_H
#define SUBFORM_H

#include <Qt>

#include "postgresiface2.h"
#include "funcaux.h"
#include "qtable2.h"
#include "dbrecord.h"

/// Describe una cabecera de recordset preparada para trabajar con SubFormularios \ref SubForm3
/**
Al crear un subformulario \ref subform3, todos los registros tienen exactamente las mismas cabeceras.
Esta clase sirve para describir una cabecera. Es analoga a la clase \ref DBCampo pero en lugar de
describir un unico registro describe a varios.
*/
class SHeader
{
public:
    /** Opciones para la columna del SubFormulario
    - DBNone . Opciones por defecto.
    - DBReadOnly . La columna es de solo lectura.
    - DBNoView . Por defecto la columna es invisible aunque puede verse si el usuario lo desea
    - DBNoWrite . El usuario no puede escribir en esta columna.
    - DBBlockView . El usuario no podrá ver esta columna ni aun configurandola
    */
    enum dboptions {DBNone = 0, DBReadOnly = 1, DBNoView = 2, DBNoWrite = 4, DBBlockView = 8};

protected:
    QString m_nomcampo;   ///< El nombre de la cabecera en el recordset
    QString m_nompresentacion;  ///< El nombre para mostrar en los errores
    unsigned int m_restricciones; ///< Las restricciones de la columna en el recordset
    DBCampo::dbtype m_tipo;  ///< El tipo de los datos
    unsigned int m_options;  ///< Las opciones para la columna en el SubFormulario

public:
    SHeader ( QString nom, DBCampo::dbtype typ, int res, int opt, QString nomp = "" );
    ~SHeader();
    unsigned int options();
    unsigned int restricciones();
    DBCampo::dbtype tipo();
    QString nompresentacion();
    int restrictcampo();
    QString nomcampo();
};

/// Registro de \ref subform3 subformulario
/**
   Maneja un registro de un subformulario de tal forma que permite el trabajo mediante tablas de los registros.
   Es muy similar a la clase \ref DBRecord pero reimplementa el guardado
*/
class SDBRecord: public DBRecord
{
public:
    SDBRecord ( EmpresaBase *con );
    ~SDBRecord();
    int addDBCampo ( QString nom, DBCampo::dbtype typ, int res, QString nomp = "" );
    void refresh();
    virtual int DBsave ( QString & );
};


/// Descripcion de campos en \ref subform3 subformularios.
/**
Junta la información de campo (Base de Datos) con  un QTableWidgetItem.
De esta forma los elementos adscritos a una talba y a un recordset son los mismos.
*/
class SDBCampo: public QTableWidgetItem2, public DBCampo
{
private:
    SDBRecord *m_pare; ///< Puntero al \ref SDBRecord al que pertenece. Acelera procesos.

public:
    SDBCampo ( SDBRecord *par, postgresiface2 *com, QString nom, dbtype typ, int res, QString nomp = "" );
    virtual ~SDBCampo();
    SDBRecord *pare();
    virtual int set ( QString val );
    void refresh();
    virtual bool operator< ( const QTableWidgetItem &other );
};

#endif

