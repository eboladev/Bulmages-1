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

#ifndef BLDBSUBFORM_H
#define BLDBSUBFORM_H

#include <Qt>

#include "blpostgresqlclient.h"
#include "funcaux.h"
#include "qtable2.h"
#include "bldb.h"


/// Registro de \ref BlSubForm subformulario
/**
   Maneja un registro de un subformulario de tal forma que permite el trabajo mediante tablas de los registros.
   Es muy similar a la clase \ref BlDbRecord pero reimplementa el guardado
*/
class BlDbSubFormRecord: public BlDbRecord
{
public:
    BlDbSubFormRecord ( EmpresaBase *con );
    ~BlDbSubFormRecord();
    int addDBCampo ( QString nom, BlDbField::dbtype typ, int res, QString nomp = "" );
    void refresh();
    virtual int DBsave ( QString & );
};


/// Descripcion de campos en \ref BlSubForm subformularios.
/**
Junta la informacion de campo (Base de Datos) con  un QTableWidgetItem.
De esta forma los elementos adscritos a una talba y a un recordset son los mismos.
*/
class SDBCampo: public QTableWidgetItem2, public BlDbField
{
private:
    BlDbSubFormRecord *m_pare; ///< Puntero al \ref BlDbSubFormRecord al que pertenece. Acelera procesos.

public:
    SDBCampo ( BlDbSubFormRecord *par, BlPostgreSqlClient *com, QString nom, dbtype typ, int res, QString nomp = "" );
    virtual ~SDBCampo();
    BlDbSubFormRecord *pare();
    virtual int set ( QString val );
    void refresh();
    virtual bool operator< ( const QTableWidgetItem &other );
};

#endif

