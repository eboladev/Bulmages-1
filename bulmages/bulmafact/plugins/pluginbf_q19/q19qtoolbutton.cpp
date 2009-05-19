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
#include <QString>
#include <QWidget>

#include "q19qtoolbutton.h"
#include "blfunctions.h"
#include "blfixed.h"
#include "cobroslist.h"
#include "cobroview.h"
#include "bfcompany.h"
#include "bldb.h"
//#include "facturapview.h"

///
/**
\param cob
\param parent
**/
Q19QToolButton::Q19QToolButton ( CobrosList *cob , QWidget *parent ) : QToolButton ( parent )
{
    _depura ( "Q19QToolButton::Q19QToolButton", 0 );
    m_cobrosList = cob;
    m_q19 = new Q19Writer( (BfCompany *) cob->mainCompany());
    setBoton();
    _depura ( "END Q19QToolButton::Q19QToolButton", 0 );
}


///
/**
**/
Q19QToolButton::~Q19QToolButton()
{
    _depura ( "Q19QToolButton::~Q19QToolButton", 0 );
     delete m_q19;
    _depura ( "END Q19QToolButton::~Q19QToolButton", 0 );
}


///
/**
**/
void Q19QToolButton::setBoton()
{
    _depura ( "Q19QToolButton::setBoton", 0 );
    connect ( this, SIGNAL ( clicked() ), this, SLOT ( click() ) );
    setObjectName ( QString::fromUtf8 ( "exporta" ) );
    setStatusTip ( "Generar Q19" );
    setToolTip ( "Generar archivo Q19 de los elementos seleccionados" );
    setMinimumSize ( QSize ( 32, 32 ) );
    setIcon ( QIcon ( g_confpr->valor ( CONF_PROGDATA ) + "icons/q19.png"  ) );
    setIconSize ( QSize ( 22, 22 ) );
    _depura ( "END Q19QToolButton::setBoton", 0 );
}


///
/**
\return
**/
void Q19QToolButton::click()
{
    _depura ( "Q19QToolButton::click", 0 );

    BlSubForm *sub = m_cobrosList->mui_list;
    QString ids="";
    int cobraments = 0;
   /// Reseteamos los valores
    for ( int i = 0; i < sub->rowCount(); i++ ) {
        BlDbSubFormRecord *rec = sub->lineaat ( i );
        rec->refresh();
        QString val = rec->dbValue ( "selector" );
        if ( val == "TRUE" ) {
           ids = ids + (ids.length()>0 ?",":"")+rec->dbValue ( "idcobro" );
           cobraments++;
        } // end if
    } // end for
    if (cobraments==0) {
       QMessageBox::critical(parentWidget(), _("Remesa vacia"),_("No hay recibos seleccionados. Puede utilizar la columna selector para seleccionar los recibos a incluir en la remesa bancaria."));
   } else { 
 
       m_companyact = ( BfCompany * ) m_cobrosList->mainCompany();
 
       _depura("buscaré ids ",0,ids);
       BlDbRecordSet *curcobro = m_companyact->loadQuery ( "SELECT cantcobro,'Ref:'||coalesce(refcobro,'')||'.'||coalesce(comentcobro,'') as comentcobro, fechavenccobro, cobro.idbanco, cobro.idcliente, idcobro, refcobro, cliente.* FROM cobro NATURAL LEFT JOIN cliente WHERE idcobro IN (" + ids +") ORDER BY cobro.idbanco,fechavenccobro,substring(cliente.bancocliente from 1 for 8),cliente.idcliente " );
       QStringList ids;
       m_q19->genera(curcobro,QString(""),&ids);
       if (!ids.isEmpty()) {
          m_companyact->runQuery("UPDATE cobro set previsioncobro = false WHERE idcobro IN  ("+ids.join(",")+")");
       }
       delete curcobro;
     }

    _depura ( "END Q19QToolButton::click", 0 );
}

