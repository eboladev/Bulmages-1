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

#include "bccentrocosteseleccionarview.h"
#include "bccompany.h"
#include "blfunctions.h"


///
/**
\param emp
\param parent
**/
BcCentroCosteSeleccionarView::BcCentroCosteSeleccionarView ( BcCompany *emp, QWidget *parent )
        : QWidget ( parent ), BlMainCompanyPointer ( emp )
{
    BL_FUNC_DEBUG
    setupUi ( this );

    numDigitos = ( ( BcCompany * ) mainCompany() ) ->numDigitosEmpresa();

    m_iterador = new QTreeWidgetItemIterator ( mui_listCostes );
    mui_listCostes->setColumnCount ( 5 );

    QStringList etiquetas;
    etiquetas << _ ( "nom_coste" ) << _ ( "desc_coste" ) << _ ( "Status" ) << _ ( "idc_coste" ) << _ ( "Seleccion" );
    mui_listCostes->setHeaderLabels ( etiquetas );
    cargacostes();
    
}


///
/**
**/
BcCentroCosteSeleccionarView::~BcCentroCosteSeleccionarView()
{
    BL_FUNC_DEBUG
    delete m_iterador;
    
}


///
/**
**/
void BcCentroCosteSeleccionarView::cargacostes()
{
    BL_FUNC_DEBUG
    /// Rellenamnos la listbox que va a sustituir al combobox correspondiente.
    /// Para que en los listados puedan salir m&aacute;s cosas de las que se dicen.
    fprintf ( stderr, "Ahora nos toca rellenar las listas.\n" );
    QMap <int, QTreeWidgetItem *> Lista;
    QTreeWidgetItem *item;
    int padre;
    int idc_coste = 0;
    BlDbRecordSet *cursoraux1, *cursoraux2;

    /// Cogemos los centros de coste principales y los ponemos donde toca.
    mui_listCostes->clear();
    cursoraux1 = mainCompany() ->loadQuery ( "SELECT * FROM c_coste WHERE padre ISNULL ORDER BY idc_coste" );
    while ( !cursoraux1->eof() ) {
        idc_coste = cursoraux1->value( "idc_coste" ).toInt();

        item = new QTreeWidgetItem ( mui_listCostes );
        item->setText ( 3, cursoraux1->value( "idc_coste" ) );
        item->setText ( 1, cursoraux1->value( "descripcion" ) );
        item->setText ( 0, cursoraux1->value( "nombre" ) );
        item->setCheckState ( 0, Qt::Unchecked );
// descomentarlo si Qt>=4.2        item->setExpanded(true);
        Lista[idc_coste] = item;

        cursoraux1->nextRecord ();
    } // end while
    delete cursoraux1;
    /// Una vez que hemos puesto los centros de coste padre, todo lo dem&aacute;s es una
    /// Tarea de ir colocando centros de coste a sus respectivos
    /// deja de ser recursivo y pasa a ser lineal.
    cursoraux2 = mainCompany() ->loadQuery ( "SELECT * FROM c_coste WHERE padre IS NOT NULL ORDER BY idc_coste" );
    while ( !cursoraux2->eof() ) {
        padre = cursoraux2->value( "padre" ).toInt();
        idc_coste = cursoraux2->value( "idc_coste" ).toInt();
        fprintf ( stderr, "Cuentas de subnivel:%d", padre );

        item = new QTreeWidgetItem ( Lista[padre] );
        item->setText ( 3, cursoraux2->value( "idc_coste" ) );
        item->setText ( 1, cursoraux2->value( "descripcion" ) );
        item->setText ( 0, cursoraux2->value( "nombre" ) );
        item->setCheckState ( 0, Qt::Unchecked );
// descomentarlo si Qt>=4.2        item->setExpanded(true);
        Lista[idc_coste] = item;

        cursoraux2->nextRecord();
    } // end while
    delete cursoraux2;
    
}


/// Esta funci&oaqcute;n devuelve el primer centro de coste seleccionado de la vista.
/// Devuelve el idc_coste. Si no hay ning&uacute;n centro de coste seleccionado devuelve
/// cero
/**
\return
**/
int BcCentroCosteSeleccionarView::firstccoste()
{
    BL_FUNC_DEBUG
    delete m_iterador;
    m_iterador = new QTreeWidgetItemIterator ( mui_listCostes );
    int idccoste = 0;

    while ( ( **m_iterador ) && idccoste == 0 ) {
        if ( ( **m_iterador ) ->checkState ( 0 ) == Qt::Checked ) {
            idccoste = ( **m_iterador ) ->text ( 3 ).toInt();
//            return idccoste;
        } // end if
        ++ ( *m_iterador );
    } // end while

    
    return idccoste;
}


/// Esta funci&oacute;n devuelve el siguiente centro de coste seleccionado de la vista.
/**
\return
**/
int BcCentroCosteSeleccionarView::nextccoste()
{
    BL_FUNC_DEBUG
    int idccoste = 0;

    while ( ( **m_iterador ) && idccoste == 0 ) {
        if ( ( **m_iterador ) ->checkState ( 0 ) == Qt::Checked ) {
            idccoste = ( **m_iterador ) ->text ( 3 ).toInt();
        } // end if
        ++ ( *m_iterador );
    } // end while
    
    return idccoste;
}


/// Esta funci&oacute;n prepara una lista separada por comas de los costes seleccionados.
/// Sirve para generar sentencias SQL.
/**
\return
**/
QString BcCentroCosteSeleccionarView::cadcoste()
{
    BL_FUNC_DEBUG
    int idc_coste;
    QString ccostes = "";
    idc_coste = firstccoste();
    while ( idc_coste ) {
        if ( ccostes != "" )
            ccostes.sprintf ( "%s, %d", ccostes.toLatin1().constData(), idc_coste );
        else
            ccostes.sprintf ( "%d", idc_coste );
        idc_coste = nextccoste();
    } /// end while
    
    return ccostes;
}


/// Esta funci&oacute;n devuelve el nombre del centro de coste actual
/// Si no existe devuelve ""
/**
\return
**/
QString BcCentroCosteSeleccionarView::nomcoste()
{
    BL_FUNC_DEBUG
    QTreeWidgetItemIterator m_iterador ( mui_listCostes );

    fprintf ( stderr, "nomcoste()\n" );

    if ( ( *m_iterador ) ->checkState ( 0 ) == Qt::Checked ) {
        fprintf ( stderr, "nomcoste: %s\n", ( *m_iterador ) ->text ( 0 ).toLatin1().constData() );
        return ( *m_iterador ) ->text ( 0 );
    } else {
        return "";
    } // end if

    delete *m_iterador;
    
    return "";
}


///
/**
**/
void BcCentroCosteSeleccionarView::on_mui_todo_clicked()
{
    BL_FUNC_DEBUG
    QTreeWidgetItemIterator m_iterador ( mui_listCostes );

    while ( *m_iterador ) {
        ( *m_iterador ) ->setCheckState ( 0, Qt::Checked );
        ++m_iterador;
    } // end while

    delete *m_iterador;
    
}


///
/**
**/
void BcCentroCosteSeleccionarView::on_mui_nada_clicked()
{
    BL_FUNC_DEBUG
    QTreeWidgetItemIterator m_iterador ( mui_listCostes );

    while ( *m_iterador ) {
        ( *m_iterador ) ->setCheckState ( 0, Qt::Unchecked );
        ++m_iterador;
    } // end while

    delete *m_iterador;
    
}


///
/**
**/
void BcCentroCosteSeleccionarView::on_mui_invertir_clicked()
{
    BL_FUNC_DEBUG
    QTreeWidgetItemIterator m_iterador ( mui_listCostes );

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

