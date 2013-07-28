/***************************************************************************
 *   Copyright (C) 2003 by Tomeu Borras Riera                              *
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

#include <QtWidgets/QMessageBox>

#include "bccuentasanualesview.h"
#include "bccuentasanualesimprimirview.h"
#include "bcbalanceimportarxml.h"
#include "bccompany.h"

#define COL_ARCHIVO 0
#define COL_NOMBRE 1


///
/**
\param emp
\param parent
**/
BcCuentasAnualesView::BcCuentasAnualesView ( BcCompany  *emp, QWidget *parent )
        : BcForm ( emp, parent )
{
    BL_FUNC_DEBUG
    setTitleName ( _ ( "Cuentas Anuales" ) );
    /// Establecemos cual va a ser la tabla en la que basarse para los permisos
    setDbTableName ( "asiento" );

//    this->setAttribute ( Qt::WA_DeleteOnClose );

    setupUi ( this );
    m_modo = 0;
    inicializaTabla();
    mainCompany() ->insertWindow ( windowTitle(), this );
    /// Llamamos a los scripts
    blScript(this);

    
}


///
/**
**/
BcCuentasAnualesView::~BcCuentasAnualesView()
{
    BL_FUNC_DEBUG
    mainCompany() ->removeWindow ( this );
    
}


///
/**
**/
void BcCuentasAnualesView::setModoSelector()
{
    BL_FUNC_DEBUG
    m_modo = 1;
    
}


///
/**
**/
void BcCuentasAnualesView::setModoEditor()
{
    BL_FUNC_DEBUG
    m_modo = 0;
    
}


///
/**
\return
**/
QString BcCuentasAnualesView::nomBalance()
{
    BL_FUNC_DEBUG
    
    return m_nomBalance;
}


///
/**
\return
**/
QString BcCuentasAnualesView::idBalance()
{
    BL_FUNC_DEBUG
    
    return m_idBalance;
}


///
/**
\return
**/
void BcCuentasAnualesView::inicializaTabla()
{
    BL_FUNC_DEBUG

    mui_listado->clear();
    mui_listado->setColumnCount ( 2 );
    QStringList headerlabels;
    headerlabels << _ ( "Archivo" ) << _ ( "Archivo" );
    mui_listado->setHorizontalHeaderLabels ( headerlabels );

    mui_listado->setColumnWidth ( COL_ARCHIVO, 290 );
    mui_listado->setColumnWidth ( COL_NOMBRE, 290 );
    mui_listado->hideColumn ( COL_ARCHIVO );

    QDir dir ( g_confpr->value( CONF_DIR_CANUALES ) );

    dir.setFilter ( QDir::Files );
    dir.setSorting ( QDir::Size | QDir::Reversed );

    QFileInfoList list = dir.entryInfoList();
    mui_listado->setRowCount ( list.size() );

    for ( int i = 0; i < list.size(); ++i ) {
        QFileInfo fileInfo = list.at ( i );
        QTableWidgetItem *newItem1 = new QTableWidgetItem ( fileInfo.filePath(), 0 );
        mui_listado->setItem ( i, COL_ARCHIVO, newItem1 );

        /// Cogemos el nombre y lo mostramos.
        QDomDocument doc;
        QFile f ( newItem1->text() );
        if ( !f.open ( QIODevice::ReadOnly ) ) {
            return;
        } // end if
        if ( !doc.setContent ( &f ) ) {
            f.close();
            return;
        } // end if
        f.close();
        QDomElement nodo = doc.namedItem ( "BALANCE" ).namedItem ( "TITULO" ).toElement();
        QTableWidgetItem *newItem2 = new QTableWidgetItem ( nodo.text(), 0 );

        mui_listado->setItem ( i, COL_NOMBRE, newItem2 );
    } // end for
    
}


///
/**
**/
void BcCuentasAnualesView::on_mui_listado_itemDoubleClicked ( QTableWidgetItem * )
{
    BL_FUNC_DEBUG
    imprimir();
    
}


///
/**
**/
void BcCuentasAnualesView::imprimir()
{
    BL_FUNC_DEBUG
    QString idbalance = mui_listado->item ( mui_listado->currentRow(), COL_ARCHIVO ) ->text();
    BcCuentasAnualesImprimirView *b = new BcCuentasAnualesImprimirView ( mainCompany(), 0 );
    b->setidbalance ( idbalance );
    b->exec();
    delete b;
    

}

