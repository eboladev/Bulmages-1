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

#include "selectccosteview.h"
#include "empresa.h"
#include "blfunctions.h"


///
/**
\param emp
\param parent
**/
SelectCCosteView::SelectCCosteView ( Empresa *emp, QWidget *parent )
        : QDialog ( parent ), PEmpresaBase ( emp )
{
    _depura ( "SelectCCosteView::SelectCCosteView", 0 );
    setupUi ( this );

    numdigitos = ( ( Empresa * ) empresaBase() ) ->numdigitosempresa();

    m_iterador = new QTreeWidgetItemIterator ( mui_listCostes );
    mui_listCostes->setColumnCount ( 5 );

    QStringList etiquetas;
    etiquetas << _( "nom_coste" ) << _( "desc_coste" ) << _( "Status" ) << _( "idc_coste" ) << _( "Seleccion" );
    mui_listCostes->setHeaderLabels ( etiquetas );
    cargacostes();
    _depura ( "END SelectCCosteView::SelectCCosteView", 0 );
}


///
/**
**/
SelectCCosteView::~SelectCCosteView()
{
    _depura ( "SelectCCosteView::~SelectCCosteView", 0 );
    delete m_iterador;
    _depura ( "END SelectCCosteView::~SelectCCosteView", 0 );
}


///
/**
**/
void SelectCCosteView::cargacostes()
{
    _depura ( "SelectCCosteView::cargacostes", 0 );
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
    cursoraux1 = empresaBase() ->cargacursor ( "SELECT * FROM c_coste WHERE padre ISNULL ORDER BY idc_coste" );
    while ( !cursoraux1->eof() ) {
        idc_coste = cursoraux1->valor ( "idc_coste" ).toInt();

        item = new QTreeWidgetItem ( mui_listCostes );
        item->setText ( 3, cursoraux1->valor ( "idc_coste" ) );
        item->setText ( 1, cursoraux1->valor ( "descripcion" ) );
        item->setText ( 0, cursoraux1->valor ( "nombre" ) );
        item->setCheckState ( 0, Qt::Unchecked );
// descomentarlo si Qt>=4.2        item->setExpanded(TRUE);
        Lista[idc_coste] = item;

        cursoraux1->siguienteregistro ();
    } // end while
    delete cursoraux1;
    /// Una vez que hemos puesto los centros de coste padre, todo lo dem&aacute;s es una
    /// Tarea de ir colocando centros de coste a sus respectivos
    /// deja de ser recursivo y pasa a ser lineal.
    cursoraux2 = empresaBase() ->cargacursor ( "SELECT * FROM c_coste WHERE padre IS NOT NULL ORDER BY idc_coste" );
    while ( !cursoraux2->eof() ) {
        padre = cursoraux2->valor ( "padre" ).toInt();
        idc_coste = cursoraux2->valor ( "idc_coste" ).toInt();
        fprintf ( stderr, "Cuentas de subnivel:%d", padre );

        item = new QTreeWidgetItem ( Lista[padre] );
        item->setText ( 3, cursoraux2->valor ( "idc_coste" ) );
        item->setText ( 1, cursoraux2->valor ( "descripcion" ) );
        item->setText ( 0, cursoraux2->valor ( "nombre" ) );
        item->setCheckState ( 0, Qt::Unchecked );
// descomentarlo si Qt>=4.2        item->setExpanded(TRUE);
        Lista[idc_coste] = item;

        cursoraux2->siguienteregistro();
    } // end while
    delete cursoraux2;
    _depura ( "END SelectCCosteView::cargacostes", 0 );
}


/// Esta funci&oaqcute;n devuelve el primer centro de coste seleccionado de la vista.
/// Devuelve el idc_coste. Si no hay ning&uacute;n centro de coste seleccionado devuelve
/// cero
/**
\return
**/
int SelectCCosteView::firstccoste()
{
    _depura ( "SelectCCosteView::firstccoste", 0 );
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

    _depura ( "END SelectCCosteView::firstccoste", 0, QString::number ( idccoste ) );
    return idccoste;
}


/// Esta funci&oacute;n devuelve el siguiente centro de coste seleccionado de la vista.
/**
\return
**/
int SelectCCosteView::nextccoste()
{
    _depura ( "SelectCCosteView::nextccoste", 0 );
    int idccoste = 0;

    while ( ( **m_iterador ) && idccoste == 0 ) {
        if ( ( **m_iterador ) ->checkState ( 0 ) == Qt::Checked ) {
            idccoste = ( **m_iterador ) ->text ( 3 ).toInt();
        } // end if
        ++ ( *m_iterador );
    } // end while
    _depura ( "END SelectCCosteView::nextccoste", 0 );
    return idccoste;
}


/// Esta funci&oacute;n prepara una lista separada por comas de los costes seleccionados.
/// Sirve para generar sentencias SQL.
/**
\return
**/
QString SelectCCosteView::cadcoste()
{
    _depura ( "SelectCCosteView::cadcoste", 0 );
    int idc_coste;
    QString ccostes = "";
    idc_coste = firstccoste();
    while ( idc_coste ) {
        if ( ccostes != "" )
            ccostes.sprintf ( "%s, %d", ccostes.toAscii().constData(), idc_coste );
        else
            ccostes.sprintf ( "%d", idc_coste );
        idc_coste = nextccoste();
    } /// end while
    _depura ( "END SelectCCosteView::cadcoste", 0 );
    return ccostes;
}


/// Esta funci&oacute;n devuelve el nombre del centro de coste actual
/// Si no existe devuelve ""
/**
\return
**/
QString SelectCCosteView::nomcoste()
{
    _depura ( "SelectCCosteView::nomcoste", 0 );
    QTreeWidgetItemIterator m_iterador ( mui_listCostes );

    fprintf ( stderr, "nomcoste()\n" );

    if ( ( *m_iterador ) ->checkState ( 0 ) == Qt::Checked ) {
        fprintf ( stderr, "nomcoste: %s\n", ( *m_iterador ) ->text ( 0 ).toAscii().constData() );
        return ( *m_iterador ) ->text ( 0 );
    } else {
        return "";
    } // end if

    delete *m_iterador;
    _depura ( "END SelectCCosteView::nomcoste", 0 );
    return "";
}


///
/**
**/
void SelectCCosteView::on_mui_todo_clicked()
{
    _depura ( "SelectCCosteView::on_mui_todo_clicked", 0 );
    QTreeWidgetItemIterator m_iterador ( mui_listCostes );

    while ( *m_iterador ) {
        ( *m_iterador ) ->setCheckState ( 0, Qt::Checked );
        ++m_iterador;
    } // end while

    delete *m_iterador;
    _depura ( "END SelectCCosteView::on_mui_todo_clicked", 0 );
}


///
/**
**/
void SelectCCosteView::on_mui_nada_clicked()
{
    _depura ( "SelectCCosteView::on_mui_nada_clicked", 0 );
    QTreeWidgetItemIterator m_iterador ( mui_listCostes );

    while ( *m_iterador ) {
        ( *m_iterador ) ->setCheckState ( 0, Qt::Unchecked );
        ++m_iterador;
    } // end while

    delete *m_iterador;
    _depura ( "END SelectCCosteView::on_mui_nada_clicked", 0 );
}


///
/**
**/
void SelectCCosteView::on_mui_invertir_clicked()
{
    _depura ( "SelectCCosteView::on_mui_invertir_clicked", 0 );
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
    _depura ( "END SelectCCosteView::on_mui_invertir_clicked", 0 );
}

