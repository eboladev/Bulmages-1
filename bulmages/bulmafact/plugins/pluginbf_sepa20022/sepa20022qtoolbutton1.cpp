/***************************************************************************
 *   Copyright (C) 2006 by Arturo Martin Llado                             *
 *   amartin@conetxia.com                                                  *
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

#include <QtWidgets/QWidget>

/// Necesarios para importacion de efactura
#include <QtXml/QDomDocument>
#include <QtXml/QDomNodeList>
#include <QtXml/QDomNode>
#include <QtCore/QString>
#include <QtWidgets/QFileDialog>
#include <QtCore/QMap>
#include <QtCore/QList>

#include "sepa20022qtoolbutton1.h"
#include "blfunctions.h"
#include "blfixed.h"
#include "bfcompany.h"
#include "bldb.h"
#include "bfbuscarbanco.h"
#include "sepa20022view.h"

///
/**
\param cob
\param parent
**/
Sepa20022QToolButton1::Sepa20022QToolButton1 ( FacturasList *cob , QWidget *parent ) : QToolButton ( parent )
{
    BL_FUNC_DEBUG
    m_facturasList = cob;
    setBoton();
    
}


///
/**
**/
Sepa20022QToolButton1::~Sepa20022QToolButton1()
{
    BL_FUNC_DEBUG
    
}


///
/**
**/
void Sepa20022QToolButton1::setBoton()
{
    BL_FUNC_DEBUG
    connect ( this, SIGNAL ( clicked() ), this, SLOT ( click() ) );
    setObjectName ( QString::fromUtf8 ( "exporta" ) );
    setStatusTip ( _("Generar Sepa20022") );
    setToolTip ( _("Generar archivo Sepa20022 de los elementos seleccionados.") );
    setMinimumSize ( QSize ( 32, 32 ) );
    setIcon ( QIcon ( g_confpr->value( CONF_PROGDATA ) + "icons/sepa20022.png" ) );
    setIconSize ( QSize ( 22, 22 ) );
    
}


///
/**
\return
**/
void Sepa20022QToolButton1::click()
{
    BL_FUNC_DEBUG

    Sepa20022View *q19 = new Sepa20022View ( m_facturasList, ( BfCompany * ) m_facturasList->mainCompany(), 0 );
// m_facturasList->mainCompany()->insertWindow("Domiciliaciones Bancarias", q19);
    m_facturasList->mainCompany() ->pWorkspace() ->addSubWindow ( q19 );
    q19->show();

    /*
        QString fileName = QFileDialog::getSaveFileName ( this, _( "Save File" ),
                           "",
                           _( "*.q19" ) );
        BlFile file ( fileName );
        if ( !file.open ( QIODevice::WriteOnly | QIODevice::Text ) )
            return;

        QTextStream out ( &file );

        m_companyact = ( BfCompany * ) m_facturasList->mainCompany();
        BlSubForm *sub = m_facturasList->mui_list;

        BlFixed total ( "0.00" );
        int j = 0;
        /// Reseteamos los valores
        for ( int i = 0; i < sub->rowCount(); i++ ) {
            BlDbSubFormRecord *rec = sub->lineaat ( i );
            rec->refresh();
            QString val = rec->dbValue ( "selector" );
            if ( val == "true" ) {
                /// La primera vez se ponen las cabeceras
                if ( j == 0 ) {
                    cabeceraPresentador ( out, rec->dbValue ( "idfactura" ) );
                    cabeceraOrdenante ( out, rec->dbValue ( "idfactura" ) );
                }
                cobroSepa20022 ( out, rec->dbValue ( "idfactura" ) );
                j++;
                total = total + BlFixed ( rec->dbValue ( "totalfactura" ) );
            } // end if
        } // end for


    //    buscarBanco();

        totalOrdenante ( out, total.toQString().remove ( ',' ).remove ( '.' ), QString::number ( j ), QString::number ( j + 2 ) );
        totalGeneral ( out, total.toQString().remove ( ',' ).remove ( '.' ), QString::number ( j ), QString::number ( j + 4 ) );
        file.close();

    */
    

}

