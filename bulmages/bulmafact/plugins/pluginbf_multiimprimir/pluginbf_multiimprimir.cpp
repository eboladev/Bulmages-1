/***************************************************************************
 *   Copyright (C) 2005 by Tomeu Borras Riera                              *
 *   tborras@conetxia.com                                                  *
 *   Copyright (C) 2006 by Fco. Javier M. C.                               *
 *   fcojavmc@todo-redes.com                                               *
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

#include <QToolButton>

#include "pluginbf_multiimprimir.h"
#include "blfunctions.h"
#include "presupuestolist.h"
#include "impmultqtoolbutton.h"


///
/**
\return
**/
int entryPoint ( BfBulmaFact * )
{
    BL_FUNC_DEBUG

    /// Inicializa el sistema de traducciones 'gettext'.
    setlocale ( LC_ALL, "" );
    blBindTextDomain ( "pluginbf_multiimprimir", g_confpr->value( CONF_DIR_TRADUCCION ).toAscii().constData() );

    return 0;
}


///
/**
\param l
\return
**/
int PedidosClienteListSubform_PedidosClienteListSubform ( PedidosClienteListSubform *l )
{
    BL_FUNC_DEBUG
    l->addSubFormHeader ( "selector", BlDbField::DbBoolean, BlDbField::DbNoSave | BlDbField::DbNoLoad, BlSubFormHeader::DbNone | BlSubFormHeader::DbNoWrite, "selector" );
    
    return 0;
}


///
/**
\param l
\return
**/
int AlbaranClienteListSubform_AlbaranClienteListSubform ( AlbaranClienteListSubform *l )
{
    BL_FUNC_DEBUG
    if ( !l->existsHeader ( "selector" ) ) {
        l->addSubFormHeader ( "selector", BlDbField::DbBoolean, BlDbField::DbNoSave | BlDbField::DbNoLoad, BlSubFormHeader::DbNone | BlSubFormHeader::DbNoWrite, "selector" );
    } // end if
    
    return 0;
}


///
/**
\param l
\return
**/
int FacturasListSubform_FacturasListSubform ( FacturasListSubform *l )
{
    BL_FUNC_DEBUG
    l->addSubFormHeader ( "selector", BlDbField::DbBoolean, BlDbField::DbNoSave | BlDbField::DbNoLoad, BlSubFormHeader::DbNone | BlSubFormHeader::DbNoWrite, "selector" );
    
    return 0;
}


///
/**
\param l
\return
**/
int PresupuestoListSubForm_PresupuestoListSubForm ( PresupuestoListSubForm *l )
{
    BL_FUNC_DEBUG
    l->addSubFormHeader ( "selector", BlDbField::DbBoolean, BlDbField::DbNoSave | BlDbField::DbNoLoad, BlSubFormHeader::DbNone | BlSubFormHeader::DbNoWrite, "selector" );
    
    return 0;
}


///
/**
\param l
\return
**/
int CobrosListSubForm_CobrosListSubForm ( CobrosListSubForm *l )
{
    BL_FUNC_DEBUG
    l->addSubFormHeader ( "selector", BlDbField::DbBoolean, BlDbField::DbNoSave | BlDbField::DbNoLoad, BlSubFormHeader::DbNone | BlSubFormHeader::DbNoWrite, "selector" );
    
    return 0;
}



///
/**
\param l
\return
**/
int RecibosListSubForm_RecibosListSubForm ( RecibosListSubForm *l )
{
    BL_FUNC_DEBUG
    l->addSubFormHeader ( "selector", BlDbField::DbBoolean, BlDbField::DbNoSave | BlDbField::DbNoLoad, BlSubFormHeader::DbNone | BlSubFormHeader::DbNoWrite, "selector" );
    
    return 0;
}

///
/**
\param l
\return
**/
int CobrosList_CobrosList ( CobrosList *l )
{
    BL_FUNC_DEBUG

    SelQToolButton *sel = new SelQToolButton ( NULL, NULL, NULL, NULL, l, l->mui_plugbotones );
    ImpQToolButton *imp = new ImpQToolButton ( NULL, NULL, NULL, NULL, l, l->mui_plugbotones );
    ProcesarQToolButton * procesar = new ProcesarQToolButton ( NULL, NULL, NULL, NULL, l, l->mui_plugbotones );
    SumarQToolButton * sumar = new SumarQToolButton ( NULL, NULL, NULL, NULL, l, l->mui_plugbotones );

    QHBoxLayout *m_hboxLayout1 = l->mui_plugbotones->findChild<QHBoxLayout *> ( "hboxLayout1" );
    if ( !m_hboxLayout1 ) {
        m_hboxLayout1 = new QHBoxLayout ( l->mui_plugbotones );
        m_hboxLayout1->setSpacing ( 5 );
        m_hboxLayout1->setMargin ( 5 );
        m_hboxLayout1->setObjectName ( QString::fromUtf8 ( "hboxLayout1" ) );
    } // end if
    m_hboxLayout1->addWidget ( sel );
    m_hboxLayout1->addWidget ( imp );
    m_hboxLayout1->addWidget ( procesar);
    m_hboxLayout1->addWidget ( sumar);


    
    return 0;
}


///
/**
\param l
\return
**/
int PresupuestoList_PresupuestoList ( PresupuestoList *l )
{
    BL_FUNC_DEBUG

    SelQToolButton *sel = new SelQToolButton ( l, NULL, NULL, NULL, NULL, l->mui_plugbotones );
    ImpQToolButton *imp = new ImpQToolButton ( l, NULL, NULL, NULL, NULL, l->mui_plugbotones );
    EmailQToolButton *email = new EmailQToolButton ( l, NULL, NULL, NULL, NULL, l->mui_plugbotones );
    ProcesarQToolButton * procesar = new ProcesarQToolButton ( l, NULL, NULL, NULL, NULL, l->mui_plugbotones );
    SumarQToolButton * sumar = new SumarQToolButton ( l, NULL, NULL, NULL, NULL, l->mui_plugbotones );

    QHBoxLayout *m_hboxLayout1 = l->mui_plugbotones->findChild<QHBoxLayout *> ( "hboxLayout1" );
    if ( !m_hboxLayout1 ) {
        m_hboxLayout1 = new QHBoxLayout ( l->mui_plugbotones );
        m_hboxLayout1->setSpacing ( 5 );
        m_hboxLayout1->setMargin ( 5 );
        m_hboxLayout1->setObjectName ( QString::fromUtf8 ( "hboxLayout1" ) );
    } // end if
    m_hboxLayout1->addWidget ( sel );
    m_hboxLayout1->addWidget ( imp );
    m_hboxLayout1->addWidget ( email );
    m_hboxLayout1->addWidget ( procesar);
    m_hboxLayout1->addWidget ( sumar);


    
    return 0;
}


///
/**
\param l
\return
**/
int PedidosClienteList_PedidosClienteList ( PedidosClienteList *l )
{
    BL_FUNC_DEBUG

    SelQToolButton *sel = new SelQToolButton ( NULL, l, NULL, NULL, NULL, l->mui_plugbotones );
    ImpQToolButton *imp = new ImpQToolButton ( NULL, l, NULL, NULL, NULL, l->mui_plugbotones );
    EmailQToolButton *email = new EmailQToolButton ( NULL, l, NULL, NULL, NULL, l->mui_plugbotones );
    ProcesarQToolButton * procesar = new ProcesarQToolButton ( NULL, l, NULL, NULL, NULL, l->mui_plugbotones );
    SumarQToolButton * sumar = new SumarQToolButton ( NULL, l, NULL, NULL, NULL, l->mui_plugbotones );

    QHBoxLayout *m_hboxLayout1 = l->mui_plugbotones->findChild<QHBoxLayout *> ( "hboxLayout1" );
    if ( !m_hboxLayout1 ) {
        m_hboxLayout1 = new QHBoxLayout ( l->mui_plugbotones );
        m_hboxLayout1->setSpacing ( 5 );
        m_hboxLayout1->setMargin ( 5 );
        m_hboxLayout1->setObjectName ( QString::fromUtf8 ( "hboxLayout1" ) );
    } // end if

    m_hboxLayout1->addWidget ( sel );
    m_hboxLayout1->addWidget ( imp );
    m_hboxLayout1->addWidget ( email );
    m_hboxLayout1->addWidget ( procesar);
    m_hboxLayout1->addWidget ( sumar);


    
    return 0;
}


///
/**
\param l
\return
**/
int AlbaranClienteList_AlbaranClienteList ( AlbaranClienteList *l )
{
    BL_FUNC_DEBUG

    SelQToolButton *sel = new SelQToolButton ( NULL,  NULL, l, NULL, NULL, l->mui_plugbotones );
    ImpQToolButton *imp = new ImpQToolButton ( NULL,  NULL, l, NULL, NULL, l->mui_plugbotones );
    EmailQToolButton *email = new EmailQToolButton ( NULL,  NULL, l, NULL, NULL, l->mui_plugbotones );
    ProcesarQToolButton * procesar = new ProcesarQToolButton ( NULL, NULL, l, NULL, NULL, l->mui_plugbotones );
    SumarQToolButton * sumar = new SumarQToolButton ( NULL, NULL, l, NULL, NULL, l->mui_plugbotones );


    QHBoxLayout *m_hboxLayout1 = l->mui_plugbotones->findChild<QHBoxLayout *> ( "hboxLayout1" );
    if ( !m_hboxLayout1 ) {
        m_hboxLayout1 = new QHBoxLayout ( l->mui_plugbotones );
        m_hboxLayout1->setSpacing ( 5 );
        m_hboxLayout1->setMargin ( 5 );
        m_hboxLayout1->setObjectName ( QString::fromUtf8 ( "hboxLayout1" ) );
    } // end if
    m_hboxLayout1->addWidget ( sel );
    m_hboxLayout1->addWidget ( imp );
    m_hboxLayout1->addWidget ( email );
    m_hboxLayout1->addWidget ( procesar);
    m_hboxLayout1->addWidget ( sumar);



    
    return 0;
}


///
/**
\param l
\return
**/
int FacturasList_FacturasList ( FacturasList *l )
{
    BL_FUNC_DEBUG

    SelQToolButton *sel = new SelQToolButton ( NULL, NULL, NULL, l, NULL, l->mui_plugbotones );
    ImpQToolButton *imp = new ImpQToolButton ( NULL, NULL, NULL, l, NULL, l->mui_plugbotones );
    EmailQToolButton *email = new EmailQToolButton ( NULL, NULL, NULL, l, NULL, l->mui_plugbotones );
    ProcesarQToolButton * procesar = new ProcesarQToolButton ( NULL, NULL, NULL, l, NULL, l->mui_plugbotones );
    SumarQToolButton * sumar = new SumarQToolButton ( NULL, NULL, NULL, l, NULL, l->mui_plugbotones );

    QHBoxLayout *m_hboxLayout1 = l->mui_plugbotones->findChild<QHBoxLayout *> ( "hboxLayout1" );
    if ( !m_hboxLayout1 ) {
        m_hboxLayout1 = new QHBoxLayout ( l->mui_plugbotones );
        m_hboxLayout1->setSpacing ( 5 );
        m_hboxLayout1->setMargin ( 5 );
        m_hboxLayout1->setObjectName ( QString::fromUtf8 ( "hboxLayout1" ) );
    } // end if
    m_hboxLayout1->addWidget ( sel );
    m_hboxLayout1->addWidget ( imp );
    m_hboxLayout1->addWidget ( email );
    m_hboxLayout1->addWidget ( procesar);
    m_hboxLayout1->addWidget ( sumar);

    
    return 0;
}


///
/**
\param l
\return
**/
int RecibosList_RecibosList ( RecibosList *l )
{
    BL_FUNC_DEBUG

    SelQToolButton *sel = new SelQToolButton ( NULL, NULL, NULL, NULL, NULL, l->mui_plugbotones );
    ImpQToolButton *imp = new ImpQToolButton ( NULL, NULL, NULL, NULL, NULL, l->mui_plugbotones );

    QHBoxLayout *m_hboxLayout1 = l->mui_plugbotones->findChild<QHBoxLayout *> ( "hboxLayout1" );
    if ( !m_hboxLayout1 ) {
        m_hboxLayout1 = new QHBoxLayout ( l->mui_plugbotones );
        m_hboxLayout1->setSpacing ( 5 );
        m_hboxLayout1->setMargin ( 5 );
        m_hboxLayout1->setObjectName ( QString::fromUtf8 ( "hboxLayout1" ) );
    } // end if
    m_hboxLayout1->addWidget ( sel );
    m_hboxLayout1->addWidget ( imp );

    
    return 0;
}


