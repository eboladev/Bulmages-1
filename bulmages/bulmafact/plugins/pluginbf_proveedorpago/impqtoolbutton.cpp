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

#include "impqtoolbutton.h"
#include "blfunctions.h"
#include "facturapview.h"
#include "albaranproveedorview.h"
#include "pedidoproveedorview.h"
#include "blfixed.h"
#include "bfcompany.h"
#include "bldb.h"
#include "pagoview.h"


///
/**
\param pres
\param ped
\param alb
\param fac
\param parent
**/
GenPagoQToolButton::GenPagoQToolButton ( QWidget *fac , QWidget *parent ) : QToolButton ( parent )
{
    BL_FUNC_DEBUG
    m_object = fac;
    setBoton();
    
}


///
/**
**/
GenPagoQToolButton::~GenPagoQToolButton()
{
    BL_FUNC_DEBUG
    
}


///
/**
**/
void GenPagoQToolButton::setBoton()
{
    BL_FUNC_DEBUG
    connect ( this, SIGNAL ( clicked() ), this, SLOT ( click() ) );
    setObjectName ( QString::fromUtf8 ( "exporta" ) );
    setStatusTip ( _("Generar pago") );
    setToolTip ( _("Generar pago") );
    setMinimumSize ( QSize ( 32, 32 ) );
    setMaximumSize ( QSize ( 32, 32 ) );
    setIcon ( QIcon ( QString::fromUtf8 ( ":/Images/pay.png" ) ) );
    setIconSize ( QSize ( 22, 22 ) );
    setContentsMargins ( 0, 0, 0, 0 );
    
}


///
/**
**/
void GenPagoQToolButton::click()
{
    BL_FUNC_DEBUG

    if ( m_object->objectName() == "FacturaProveedorBase" ) {

	FacturaProveedorView *fpv = ( FacturaProveedorView * ) m_object;
        int nuevo = 1;
        /// Comprobamos que no haya ya un pago con la misma referencia y lo ponemos
        QString query = "SELECT * FROM pago WHERE refpago ='" + fpv->dbValue ( "reffacturap" ) + "'";
        BlDbRecordSet *cur = fpv->mainCompany()->loadQuery ( query );
        if ( cur->numregistros() > 0 ) {
            QMessageBox msgBox;
            msgBox.setText ( _( "Ya existe un pago con esta referencia\n" ) );
            msgBox.setInformativeText ( _( "Desea abrir el pago existente, registrar un nuevo pago o salir?" ) );
            msgBox.addButton ( _( "Crear" ), QMessageBox::ActionRole );
            QPushButton *openButton = msgBox.addButton ( QMessageBox::Open );
            QPushButton *abortButton = msgBox.addButton ( QMessageBox::Cancel );
            msgBox.setDefaultButton ( QMessageBox::Cancel );
            msgBox.exec();
            /// Se ha pulsado sobre la opcion abrir
            if ( msgBox.clickedButton() == openButton ) {
                while ( !cur->eof() ) {
		    PagoView *bud = new PagoView ( ( BfCompany * ) fpv->mainCompany(), 0 );
		    fpv->mainCompany() ->m_pWorkspace->addSubWindow ( bud );
                    bud->load ( cur->value( "idpago" ) );
                    bud->pintar();
                    bud->show();
                    cur->nextRecord();
                } // end while
                nuevo = 0;
            } // end if

            /// Se ha pulsado sobre la opcion cancelar
            if ( msgBox.clickedButton() == abortButton )
                nuevo = 0;
        } // end if
        delete cur;

        /// Creacion de un pago nuevo a partir de la factura.
        if ( nuevo ) {
	    PagoView *bud = new PagoView ( fpv->mainCompany(), 0 );
	    fpv->mainCompany() ->m_pWorkspace->addSubWindow ( bud );
	    bud->setDbValue ( "idproveedor", fpv->dbValue ( "idproveedor" ) );
	    bud->setDbValue ( "cantpago", fpv->m_totalfacturap->text() );
	    bud->setDbValue ( "refpago", fpv->dbValue ( "reffacturap" ) );
	    bud->setDbValue ( "comentpago", fpv->dbValue ( "descfacturap" ) );
	    bud->pintar();
	    bud->show();
        } // end if

	/// Marcamnos la factura como procesada ya que el cobro es inicialmente por el importe total
	fpv->mui_procesadafacturap->setChecked(true);
    } // end if

    if ( m_object->objectName() == "AlbaranProveedorBase" ) {
      
	AlbaranProveedorView *fpv = ( AlbaranProveedorView * ) m_object;
        int nuevo = 1;
        /// Comprobamos que no haya ya un pago con la misma referencia y lo ponemos
        QString query = "SELECT * FROM pago WHERE refpago ='" + fpv->dbValue ( "refalbaranp" ) + "'";
        BlDbRecordSet *cur = fpv->mainCompany()->loadQuery ( query );
        if ( cur->numregistros() > 0 ) {
            QMessageBox msgBox;
            msgBox.setText ( _( "Ya existe un pago con esta referencia\n" ) );
            msgBox.setInformativeText ( _( "Desea abrir el pago existente, registrar un nuevo pago o salir?" ) );
            msgBox.addButton ( _( "Crear" ), QMessageBox::ActionRole );
            QPushButton *openButton = msgBox.addButton ( QMessageBox::Open );
            QPushButton *abortButton = msgBox.addButton ( QMessageBox::Cancel );
            msgBox.setDefaultButton ( QMessageBox::Cancel );
            msgBox.exec();
            /// Se ha pulsado sobre la opcion abrir
            if ( msgBox.clickedButton() == openButton ) {
                while ( !cur->eof() ) {
		    PagoView *bud = new PagoView ( ( BfCompany * ) fpv->mainCompany(), 0 );
		    fpv->mainCompany() ->m_pWorkspace->addSubWindow ( bud );
                    bud->load ( cur->value( "idpago" ) );
                    bud->pintar();
                    bud->show();
                    cur->nextRecord();
                } // end while
                nuevo = 0;
            } // end if

            /// Se ha pulsado sobre la opcion cancelar
            if ( msgBox.clickedButton() == abortButton )
                nuevo = 0;
        } // end if
        delete cur;

        /// Creacion de un pago nuevo a partir del albaran.
        if ( nuevo ) {
	    PagoView *bud = new PagoView ( ( BfCompany * ) fpv->mainCompany(), NULL );
	    fpv->mainCompany() ->m_pWorkspace->addSubWindow ( bud );
	    bud->setDbValue ( "idproveedor", fpv->dbValue ( "idproveedor" ) );
	    bud->setDbValue ( "cantpago", fpv->m_totalalbaranp->text() );
	    bud->setDbValue ( "refpago", fpv->dbValue ( "refalbaranp" ) );
	    bud->setDbValue ( "comentpago", fpv->dbValue ( "descalbaranp" ) );
	    bud->pintar();
	    bud->show();
        } // end if

    } // end if

    if ( m_object->objectName() == "PedidoProveedorBase" ) {
      
	PedidoProveedorView *fpv = ( PedidoProveedorView * ) m_object;
        int nuevo = 1;
        /// Comprobamos que no haya ya un pago con la misma referencia y lo ponemos
        QString query = "SELECT * FROM pago WHERE refpago ='" + fpv->dbValue ( "refpedidoproveedor" ) + "'";
        BlDbRecordSet *cur = fpv->mainCompany()->loadQuery ( query );
        if ( cur->numregistros() > 0 ) {
            QMessageBox msgBox;
            msgBox.setText ( _( "Ya existe un pago con esta referencia\n" ) );
            msgBox.setInformativeText ( _( "Desea abrir el pago existente, registrar un nuevo pago o salir?" ) );
            msgBox.addButton ( _( "Crear" ), QMessageBox::ActionRole );
            QPushButton *openButton = msgBox.addButton ( QMessageBox::Open );
            QPushButton *abortButton = msgBox.addButton ( QMessageBox::Cancel );
            msgBox.setDefaultButton ( QMessageBox::Cancel );
            msgBox.exec();
            /// Se ha pulsado sobre la opcion abrir
            if ( msgBox.clickedButton() == openButton ) {
                while ( !cur->eof() ) {
		    PagoView *bud = new PagoView ( ( BfCompany * ) fpv->mainCompany(), 0 );
		    fpv->mainCompany() ->m_pWorkspace->addSubWindow ( bud );
                    bud->load ( cur->value( "idpago" ) );
                    bud->pintar();
                    bud->show();
                    cur->nextRecord();
                } // end while
                nuevo = 0;
            } // end if

            /// Se ha pulsado sobre la opcion cancelar
            if ( msgBox.clickedButton() == abortButton )
                nuevo = 0;
        } // end if
        delete cur;

        /// Creacion de un pago nuevo a partir de la factura.
        if ( nuevo ) {
	    PagoView *bud = new PagoView ( ( BfCompany * ) fpv->mainCompany(), NULL );
	    fpv->mainCompany() ->m_pWorkspace->addSubWindow ( bud );
	    bud->setDbValue ( "idproveedor", fpv->dbValue ( "idproveedor" ) );
	    bud->setDbValue ( "cantpago", fpv->mui_totalPedido->text() );
	    bud->setDbValue ( "refpago", fpv->dbValue ( "refpedidoproveedor" ) );
	    bud->setDbValue ( "comentpago", fpv->dbValue ( "descpedidoproveedor" ) );
	    bud->pintar();
	    bud->show();
        } // end if

    } // end if

}


