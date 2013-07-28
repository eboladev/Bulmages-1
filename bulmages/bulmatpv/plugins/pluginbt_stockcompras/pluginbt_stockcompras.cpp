/***************************************************************************
 *
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

#include <QtWidgets/QPushButton>

#include "pluginbt_stockcompras.h"
#include "blfunctions.h"
#include "btcompany.h"
#include "compra.h"
#include "blplugins.h"
#include "btticket.h"
#include "blapplication.h"
#include "bldockwidget.h"


BlDockWidget *g_doc1;
MyDevButton1 * g_plug;


///
/**
**/
MyDevButton1::MyDevButton1 ( const QString & text, QWidget * parent, BtCompany *emp ) : QPushButton ( text,  parent )
{
    BL_FUNC_DEBUG
    m_emp = emp;
    connect ( this, SIGNAL ( released() ), this, SLOT ( on_click() ) );
    
}


///
/**
**/
MyDevButton1::~MyDevButton1()
{
    BL_FUNC_DEBUG
}


///
/**
\return
**/
void MyDevButton1::on_click (  )
{
    BL_FUNC_DEBUG
    Compra *camb = new Compra ( m_emp, 0 );
    camb->show();
    
}


///
/**
\return
**/
int entryPoint ( BtBulmaTPV *tpv )
{
    BL_FUNC_DEBUG

    /// Inicializa el sistema de traducciones 'gettext'.
    setlocale ( LC_ALL, "" );
    blBindTextDomain ( "pluginbt_stockcompras", g_confpr->value( CONF_DIR_TRADUCCION ).toLatin1().constData() );

    /*
        g_doc1 = new BlDockWidget ( "Articulo", tpv, "articulodock" );
        g_doc1->setFeatures ( QDockWidget::AllDockWidgetFeatures );
        g_doc1->setGeometry ( 100, 100, 100, 500 );
        g_doc1->resize ( 330, 400 );
        tpv->addDockWidget ( Qt::TopDockWidgetArea, g_doc1 );
        g_doc1->show();
    */
    
    return 0;
}

int BtCompany_createMainWindows_Post ( BtCompany *etpv )
{
    BL_FUNC_DEBUG
    // ============ Pruebas con abrevs
    g_plug = new MyDevButton1 ( "STOCK", etpv, etpv );

    g_plug->setMinimumSize ( QSize ( 72, 72 ) );
    g_plug->setMaximumSize ( QSize ( 200, 72 ) );
    /*
        g_doc1->setWidget ( g_plug );
    */
    QFrame *fr = g_main->findChild<QFrame *> ( "mui_frameabrevs" );
    if ( fr ) {
        QHBoxLayout *m_hboxLayout1 = fr->findChild<QHBoxLayout *> ( "hboxLayout1" );
        if ( !m_hboxLayout1 ) {
            m_hboxLayout1 = new QHBoxLayout ( fr );
            m_hboxLayout1->setSpacing ( 5 );
            m_hboxLayout1->setMargin ( 5 );
            m_hboxLayout1->setObjectName ( QString::fromUtf8 ( "hboxLayout1" ) );
        } // end if
        m_hboxLayout1->addWidget ( g_plug );
    } // end if*/
    return 0;
}



