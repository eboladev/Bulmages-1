/***************************************************************************
 *   Copyright (C) 2007 by Tomeu Borras Riera                              *
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

#include <QWidget>

#include "fichacfg.h"


///  Inicializa el objeto y hace todas las conexiones necesarias.
/**
\param emp
\param parent
\param f
**/
FichaCfg::FichaCfg ( BlMainCompany *emp, BlForm *parent, Qt::WFlags f ) : BlWidget ( emp, parent, f )
{
    _depura ( "FichaCfg::FichaCfg", 0 );
    setupUi ( this );
    setAttribute ( Qt::WA_DeleteOnClose );
    m_BlForm = parent;
    setGeometry ( 0, 0, m_BlForm->width(), m_BlForm->height() );
    showMaximized();
    mui_tablename->setText ( m_BlForm->tableName() );

    /// Listamos los atributos de la tabla.
    QString fields = "SELECT a.attnum AS ordinal_position, a.attname AS column_name, t.typname AS data_type, a.attlen AS character_maximum_length, a.atttypmod AS modifier, a.attnotnull AS notnull, a.atthasdef AS hasdefault FROM pg_class c, pg_attribute a, pg_type t WHERE c.relname = '" + m_BlForm->tableName() + "' AND a.attnum > 0 AND a.attrelid = c.oid AND a.atttypid = t.oid ORDER BY a.attnum;";
    BlDbRecordSet *cur = mainCompany() ->cargacursor ( fields );

    mui_databasetable->setRowCount ( cur->numregistros() );
    mui_databasetable->setColumnCount ( cur->numcampos() );
    int i = 0;
    while ( !cur->eof() ) {
        for ( int j = 0 ; j < cur->numcampos() ; j++ ) {
            QTableWidgetItem *newItem = new QTableWidgetItem ( cur->valor ( j, i ) );
            mui_databasetable->setItem ( i, j, newItem );
        } // end for
        cur->siguienteregistro();
        i++;
    } // end while
    delete cur;

    /// Miramos los temas de BlDbRecord.
    mui_formtablename->setText ( m_BlForm->tableName() );
    mui_formcampoindice->setText ( m_BlForm->campoId() );

    QList<BlDbField *> *lista = m_BlForm->lista();

    mui_formelements->setRowCount ( lista->size() );
    mui_formelements->setColumnCount ( 3 );

    /// Impresion de la tabla de contenidos.
    for ( int j = 0; j < lista->size(); ++j ) {
        BlDbField *campo = lista->at ( j );
        QTableWidgetItem *newItem = new QTableWidgetItem ( campo->nomcampo() );
        mui_formelements->setItem ( j, 0, newItem );

        QTableWidgetItem *newItem1 = new QTableWidgetItem ( campo->nompresentacion() );
        mui_formelements->setItem ( j, 1, newItem1 );

        QTableWidgetItem *newItem2 = new QTableWidgetItem ( campo->valorcampo() );
        mui_formelements->setItem ( j, 2, newItem2 );
    } // end for

    /// Privilegios de usuario.
    QString queryusers = "SELECT usename, has_table_privilege(pg_user.usename, '" + m_BlForm->tableName() + "', 'SELECT') AS pselect" \
                         ", has_table_privilege(pg_user.usename, '" + m_BlForm->tableName() + "', 'INSERT') AS pinsert" \
                         ", has_table_privilege(pg_user.usename, '" + m_BlForm->tableName() + "', 'UPDATE') AS pupdate" \
                         ", has_table_privilege(pg_user.usename, '" + m_BlForm->tableName() + "', 'RULE') AS prules  FROM pg_user";
    cur = mainCompany() ->cargacursor ( queryusers );

    mui_usertable->setRowCount ( cur->numregistros() );
    mui_usertable->setColumnCount ( cur->numcampos() );
    i = 0;
    while ( !cur->eof() ) {
        for ( int j = 0 ; j < cur->numcampos() ; j++ ) {
            QTableWidgetItem *newItem = new QTableWidgetItem ( cur->valor ( j, i ) );
            mui_usertable->setItem ( i, j, newItem );
        } // end for
        cur->siguienteregistro();
        i++;
    } // end while
    delete cur;

    cur = mainCompany() ->cargacursor ( "SELECT user" );
    if ( !cur ) return;

    mui_currentuser->setText ( cur->valor ( "current_user" ) );
    delete cur;
    _depura ( "END FichaCfg::FichaCfg", 0 );
}


/// No requiere de acciones especiales en el destructor.
/**
**/
FichaCfg::~FichaCfg()
{
    _depura ( "FichaCfg::~FichaCfg", 0 );
    _depura ( "END FichaCfg::~FichaCfg", 0 );
}

