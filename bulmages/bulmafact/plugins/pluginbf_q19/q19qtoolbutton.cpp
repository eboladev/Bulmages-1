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

/// Necesarios para importacion de efactura
#include <QtCore/QString>
#include <QtWidgets/QWidget>

#include "q19qtoolbutton.h"
#include "blfunctions.h"
#include "blfixed.h"
#include "cobroslist.h"
#include "cobroview.h"
#include "bfcompany.h"
#include "bldb.h"

///
/**
\param cob
\param parent
**/
Q19QToolButton::Q19QToolButton ( CobrosList *cob , QWidget *parent ) : QToolButton ( parent )
{
    BL_FUNC_DEBUG
    m_cobrosList = cob;
    m_recibosList=NULL;
    m_q19 = new Q19Writer( (BfCompany *) cob->mainCompany());
    setBoton();
    
}

///
/**
\param cob
\param parent
**/
Q19QToolButton::Q19QToolButton ( RecibosList *cob , QWidget *parent ) : QToolButton ( parent )
{
    BL_FUNC_DEBUG
    m_recibosList = cob;
    m_cobrosList = NULL;
    m_q19 = new Q19Writer( (BfCompany *) cob->mainCompany());
    setBoton();
    
}

///
/**
**/
Q19QToolButton::~Q19QToolButton()
{
    BL_FUNC_DEBUG
     delete m_q19;
    
}


///
/**
**/
void Q19QToolButton::setBoton()
{
    BL_FUNC_DEBUG
    connect ( this, SIGNAL ( clicked() ), this, SLOT ( click() ) );
    setObjectName ( QString::fromUtf8 ( "exporta" ) );
    setStatusTip ( _("Generar Q19") );
    setToolTip ( _("Generar archivo Q19 de los elementos seleccionados.") );
    setMinimumSize ( QSize ( 32, 32 ) );
    setIcon ( QIcon ( g_confpr->value( CONF_PROGDATA ) + "icons/q19.png"  ) );
    setIconSize ( QSize ( 22, 22 ) );
    
}


///
/**
\return
**/
void Q19QToolButton::click()
{
    BL_FUNC_DEBUG
    BlSubForm *sub = NULL;
    if (m_cobrosList) {
       sub = m_cobrosList->mui_list;
    } else {
       sub = m_recibosList->mui_list;
    } // end if

    QString ids="";
    int cobraments = 0;
   /// Reseteamos los valores
    for ( int i = 0; i < sub->rowCount(); i++ ) {
        BlDbSubFormRecord *rec = sub->lineaat ( i );
        rec->refresh();
        QString val = rec->dbValue ( "selector" );
        if ( val == "true" ) {
	   if (m_cobrosList) {
	      ids = ids + (ids.length()>0 ?",":"")+rec->dbValue ( "idcobro" );
	   } else {
	      ids = ids + (ids.length()>0 ?",":"")+rec->dbValue ( "idrecibo" );
	   } // end if
           cobraments++;
        } // end if
    } // end for
    if (cobraments==0) {
       QMessageBox::critical(parentWidget(), _("Remesa vacia"),_("No hay recibos seleccionados. Puede utilizar la columna selector para seleccionar los recibos a incluir en la remesa bancaria."));
   } else { 
       if (m_cobrosList) {
	  m_companyact = ( BfCompany * ) m_cobrosList->mainCompany();
       } else {
	  m_companyact = ( BfCompany * ) m_recibosList->mainCompany();
       } // end if

       QString query = "";
       if (m_cobrosList) {
	  query = "SELECT cantcobro,'Ref:'||coalesce(refcobro,'')||'.'||coalesce(comentcobro,'') as comentcobro, fechavenccobro, cobro.idbanco, cobro.idcliente, idcobro, refcobro, cliente.* FROM cobro NATURAL LEFT JOIN cliente WHERE idcobro IN (" + ids +") ORDER BY cobro.idbanco,fechavenccobro,substring(cliente.bancocliente from 1 for 8),cliente.idcliente ";
       } else {
	  query = "SELECT cantrecibo AS cantcobro,'Ref:'||coalesce(descrecibo,'')||'.'||coalesce(fecharecibo::VARCHAR,'') as comentcobro, fecharecibo AS fechavenccobro, forma_pago.idbanco, recibo.idcliente, idrecibo AS idcobro, fecharecibo::VARCHAR as refcobro, cliente.* FROM recibo LEFT JOIN cliente ON cliente.idcliente = recibo.idcliente LEFT JOIN forma_pago ON recibo.idforma_pago = forma_pago.idforma_pago WHERE idrecibo IN (" + ids +") ORDER BY forma_pago.idbanco,fecharecibo,substring(cliente.bancocliente from 1 for 8),cliente.idcliente ";
       } // end if
       BlDbRecordSet *curcobro = m_companyact->loadQuery ( query );
       QStringList ids;
       m_q19->genera(curcobro,QString(""),&ids);
       if (!ids.isEmpty()) {
	  if (m_cobrosList) {
	      m_companyact->runQuery("UPDATE cobro set previsioncobro = false WHERE idcobro IN  ("+ids.join(",")+")");
	  } else {
	      m_companyact->runQuery("UPDATE recibo set pagadorecibo = true WHERE idrecibo IN  ("+ids.join(",")+")");
	  } // end if
       }
       delete curcobro;
     }

    
}

