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
    el programa. Luego llama al m&eacute;todo cargaCanales que hace la carga de los canales
    a partir de la base de datos. */
/**
\param emp
\param parent
**/
BcCanalSeleccionarView::BcCanalSeleccionarView ( BcCompany *emp, QWidget *parent )
        : QWidget ( parent ), BlMainCompanyPointer ( emp )
{
    BL_FUNC_DEBUG
    setupUi ( this );
    numDigitos = ( ( BcCompany * ) mainCompany() ) ->numDigitosEmpresa();
    m_iterador = new QTreeWidgetItemIterator ( m_listCanales );

    m_listCanales->setColumnCount ( 5 );

    QStringList etiquetas;
    etiquetas << _ ( "nom_canal" ) << _ ( "desc_canal" ) << _ ( "Status" ) << _ ( "idcanal" ) << _ ( "Seleccion" );
    m_listCanales->setHeaderLabels ( etiquetas );

    cargaCanales();
    
}


///
/**
**/
BcCanalSeleccionarView::~BcCanalSeleccionarView()
{
    BL_FUNC_DEBUG
    delete m_iterador;
    
}


///
/**
**/
void BcCanalSeleccionarView::cargaCanales()
{
    BL_FUNC_DEBUG
    QMap <int, QTreeWidgetItem *> Lista;
    QTreeWidgetItem *it;

    int idcanal = 0;
    BlDbRecordSet *cursoraux1;
    /// Cogemos los canales y los ponemos donde toca.
    m_listCanales->clear();
    /// Ponemos el canal base (sin canal y lo checkeamos)
    it = new QTreeWidgetItem ( m_listCanales );
    Lista[idcanal] = it;
    it->setText ( 3, _("-1") );
    it->setText ( 1, _("Sin Canal") );
    it->setText ( 0, _("Sin Canal") );
    it->setCheckState ( 0, Qt::Checked );
    it->setExpanded ( TRUE );
    
    
    mainCompany() ->begin();
    cursoraux1 = mainCompany() ->loadQuery ( "SELECT * FROM canal" );
    mainCompany() ->commit();
    while ( !cursoraux1->eof() ) {
        idcanal = atoi ( cursoraux1->value( "idcanal" ).toAscii() );
        it = new QTreeWidgetItem ( m_listCanales );
        Lista[idcanal] = it;
        it->setText ( 3, cursoraux1->value( "idcanal" ) );
        it->setText ( 1, cursoraux1->value( "descripcion" ) );
        it->setText ( 0, cursoraux1->value( "nombre" ) );
        it->setCheckState ( 0, Qt::Checked );
        it->setExpanded ( TRUE );
        cursoraux1->nextRecord();
    } // end while

    delete cursoraux1;
    
}


/// Esta funci&oacute;n devuelve el primer canal seleccionado de la vista.
/// Devuelve el idcanal. Si no hay ning&uacute;n canal seleccionado devuelve
/// cero.
/**
\return
**/
int BcCanalSeleccionarView::firstCanal()
{
    BL_FUNC_DEBUG
    delete m_iterador;
    m_iterador = new QTreeWidgetItemIterator ( m_listCanales );
    ++ ( *m_iterador );
    return nextCanal();
}


bool BcCanalSeleccionarView::sinCanal()
{
    BL_FUNC_DEBUG
    QTreeWidgetItemIterator iterador( m_listCanales ) ;
    return ( *iterador ) ->checkState ( 0 ) == Qt::Checked ;
}


/// Esta funci&oacute;n devuelve el siguiente canal seleccionado de la vista.
/**
\return idcanal siguiente o 0 si no hay siguiente.
**/
int BcCanalSeleccionarView::nextCanal()
{
    BL_FUNC_DEBUG
    int idcanal = 0;
    while ( ( **m_iterador ) && idcanal == 0 ) {
        if ( ( **m_iterador ) ->checkState ( 0 ) == Qt::Checked ) {
            idcanal = ( **m_iterador ) ->text ( 3 ).toInt();
        } // end if
        ++ ( *m_iterador );
    } // end while

    
    return idcanal;
}


///
/**
\return
**/
QString BcCanalSeleccionarView::cadCanal()
{
    BL_FUNC_DEBUG

    int idcanal;
    QString ccanales = "";

    idcanal = firstCanal();
    while ( idcanal ) {
        if ( ccanales != "" )
            ccanales.sprintf ( "%s, %d", ccanales.toAscii().constData(), idcanal );
        else
            ccanales.sprintf ( "%d", idcanal );
        idcanal = nextCanal();
    } // end while

    
    return ccanales;
}


/// Esta funci&oacute;n devuelve el nombre de un canal determinado.
/**
\return
**/
QString BcCanalSeleccionarView::nomCanal()
{
    BL_FUNC_DEBUG
    QTreeWidgetItemIterator m_iterador ( m_listCanales );
    QString value = "";

    if ( ( *m_iterador ) ->checkState ( 0 ) == Qt::Checked ) {
        value = ( *m_iterador ) ->text ( 0 );
    } // end if

    delete *m_iterador;
    
    return value;
}


///
/**
**/
void BcCanalSeleccionarView::on_mui_todo_clicked()
{
    BL_FUNC_DEBUG
    QTreeWidgetItemIterator m_iterador ( m_listCanales );

    while ( *m_iterador ) {
        ( *m_iterador ) ->setCheckState ( 0, Qt::Checked );
        ++m_iterador;
    }

    delete *m_iterador;
    
}


///
/**
**/
void BcCanalSeleccionarView::on_mui_nada_clicked()
{
    BL_FUNC_DEBUG

    QTreeWidgetItemIterator m_iterador ( m_listCanales );

    while ( *m_iterador ) {
        ( *m_iterador ) ->setCheckState ( 0, Qt::Unchecked );
        ++m_iterador;
    }

    delete *m_iterador;
    
}


///
/**
**/
void BcCanalSeleccionarView::on_mui_invertir_clicked()
{
    BL_FUNC_DEBUG
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
    
}

