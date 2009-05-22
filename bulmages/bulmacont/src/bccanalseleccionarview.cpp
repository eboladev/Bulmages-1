/***************************************************************************
 *   Copyright (C) 2003 by Tomeu Borras                                    *
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

#include "bccanalseleccionarview.h"
#include "bccompany.h"
#include "blfunctions.h"


/// Inicializa las variables de empresa y de base de datos.
/** Luego crea las columnas para el objeto m_listCanales que es la lista en que se basa
    el programa. Luego llama al m&eacute;todo cargacanales que hace la carga de los canales
    a partir de la base de datos. */
/**
\param emp
\param parent
**/
BcCanalSeleccionarView::BcCanalSeleccionarView ( BcCompany *emp, QWidget *parent )
        : QDialog ( parent ), BlMainCompanyPointer ( emp )
{
    _depura ( "BcCanalSeleccionarView::BcCanalSeleccionarView", 0 );
    setupUi ( this );
    numdigitos = ( ( BcCompany * ) mainCompany() ) ->numdigitosempresa();
    m_iterador = new QTreeWidgetItemIterator ( m_listCanales );

    m_listCanales->setColumnCount ( 5 );

    QStringList etiquetas;
    etiquetas << _ ( "nom_canal" ) << _ ( "desc_canal" ) << _ ( "Status" ) << _ ( "idcanal" ) << _ ( "Seleccion" );
    m_listCanales->setHeaderLabels ( etiquetas );

    cargacanales();
    _depura ( "END BcCanalSeleccionarView::BcCanalSeleccionarView", 0 );
}


///
/**
**/
BcCanalSeleccionarView::~BcCanalSeleccionarView()
{
    _depura ( "BcCanalSeleccionarView::~BcCanalSeleccionarView", 0 );
    delete m_iterador;
    _depura ( "END BcCanalSeleccionarView::~BcCanalSeleccionarView", 0 );
}


///
/**
**/
void BcCanalSeleccionarView::cargacanales()
{
    _depura ( "BcCanalSeleccionarView::cargacanales", 0 );
    QMap <int, QTreeWidgetItem *> Lista;
    QTreeWidgetItem *it;

    int idcanal = 0;
    BlDbRecordSet *cursoraux1;
    /// Cogemos los canales y los ponemos donde toca.
    m_listCanales->clear();
    mainCompany() ->begin();
    cursoraux1 = mainCompany() ->loadQuery ( "SELECT * FROM canal", "canalillos" );
    mainCompany() ->commit();
    while ( !cursoraux1->eof() ) {
        idcanal = atoi ( cursoraux1->valor ( "idcanal" ).toAscii() );
        it = new QTreeWidgetItem ( m_listCanales );
        Lista[idcanal] = it;
        it->setText ( 3, cursoraux1->valor ( "idcanal" ) );
        it->setText ( 1, cursoraux1->valor ( "descripcion" ) );
        it->setText ( 0, cursoraux1->valor ( "nombre" ) );
        it->setCheckState ( 0, Qt::Unchecked );
        it->setExpanded ( TRUE );
        cursoraux1->nextRecord();
    } // end while

    delete cursoraux1;
    _depura ( "END BcCanalSeleccionarView::cargacanales", 0 );
}


/// Esta funci&oacute;n devuelve el primer canal seleccionado de la vista.
/// Devuelve el idcanal. Si no hay ning&uacute;n canal seleccionado devuelve
/// cero.
/**
\return
**/
int BcCanalSeleccionarView::firstcanal()
{
    _depura ( "BcCanalSeleccionarView::firstcanal", 0 );
    delete m_iterador;
    m_iterador = new QTreeWidgetItemIterator ( m_listCanales );
    _depura ( "END BcCanalSeleccionarView::firstcanal", 0 );
    return nextcanal();
}


/// Esta funci&oacute;n devuelve el siguiente canal seleccionado de la vista.
/**
\return
**/
int BcCanalSeleccionarView::nextcanal()
{
    _depura ( "BcCanalSeleccionarView::nextcanal", 0 );
    int idcanal = 0;
    fprintf ( stderr, "nextcanal\n" );
    while ( ( **m_iterador ) && idcanal == 0 ) {
        if ( ( **m_iterador ) ->checkState ( 0 ) == Qt::Checked ) {
            idcanal = ( **m_iterador ) ->text ( 3 ).toInt();
            fprintf ( stderr, "siguiente canal:%d\n", idcanal );
            return idcanal;
        } // end if
        ++ ( *m_iterador );
    } // end while

    _depura ( "END BcCanalSeleccionarView::nextcanal", 0 );
    return idcanal;
}


///
/**
\return
**/
QString BcCanalSeleccionarView::cadcanal()
{
    _depura ( "BcCanalSeleccionarView::cadcanal", 0 );

    int idcanal;
    QString ccanales = "";

    idcanal = firstcanal();
    while ( idcanal ) {
        if ( ccanales != "" )
            ccanales.sprintf ( "%s, %d", ccanales.toAscii().constData(), idcanal );
        else
            ccanales.sprintf ( "%d", idcanal );
        idcanal = nextcanal();
    } // end while

    _depura ( "END BcCanalSeleccionarView::cadcanal", 0 );
    return ccanales;
}


/// Esta funci&oacute;n devuelve el nombre de un canal determinado.
/**
\return
**/
QString BcCanalSeleccionarView::nomcanal()
{
    _depura ( "BcCanalSeleccionarView::nomcanal", 0 );
    QTreeWidgetItemIterator m_iterador ( m_listCanales );
    QString value = "";

    if ( ( *m_iterador ) ->checkState ( 0 ) == Qt::Checked ) {
        value = ( *m_iterador ) ->text ( 0 );
    } // end if

    delete *m_iterador;
    _depura ( "END BcCanalSeleccionarView::nomcanal", 0 );
    return value;
}


///
/**
**/
void BcCanalSeleccionarView::on_mui_todo_released()
{
    _depura ( "BcCanalSeleccionarView::on_mui_todo_released", 0 );
    QTreeWidgetItemIterator m_iterador ( m_listCanales );

    while ( *m_iterador ) {
        ( *m_iterador ) ->setCheckState ( 0, Qt::Checked );
        ++m_iterador;
    }

    delete *m_iterador;
    _depura ( "END BcCanalSeleccionarView::on_mui_todo_released", 0 );
}


///
/**
**/
void BcCanalSeleccionarView::on_mui_nada_released()
{
    _depura ( "BcCanalSeleccionarView::on_mui_nada_released", 0 );

    QTreeWidgetItemIterator m_iterador ( m_listCanales );

    while ( *m_iterador ) {
        ( *m_iterador ) ->setCheckState ( 0, Qt::Unchecked );
        ++m_iterador;
    }

    delete *m_iterador;
    _depura ( "END BcCanalSeleccionarView::on_mui_nada_released", 0 );
}


///
/**
**/
void BcCanalSeleccionarView::on_mui_invertir_released()
{
    _depura ( "BcCanalSeleccionarView::on_mui_invertir_released", 0 );
    QTreeWidgetItemIterator m_iterador ( m_listCanales );

    while ( *m_iterador ) {
        if ( ( *m_iterador ) ->checkState ( 0 ) == Qt::Unchecked ) {
            ( *m_iterador ) ->setCheckState ( 0, Qt::Checked );
        } else {
            ( *m_iterador ) ->setCheckState ( 0, Qt::Unchecked );
        } // end if
        ++m_iterador;
    } // end while

    delete *m_iterador;
    _depura ( "END BcCanalSeleccionarView::on_mui_invertir_released", 0 );
}

