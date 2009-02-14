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

#include <QPushButton>

#include "plugindevolucion2.h"
#include "blfunctions.h"
#include "empresatpv.h"
#include "devolucion2.h"
#include "plugins.h"
#include "ticket.h"
#include "blapplication.h"
#include "bldockwidget.h"


BlDockWidget *g_doc1;
MyDevButton * g_plug;


///
/**
**/
MyDevButton::MyDevButton ( const QString & text, QWidget * parent, EmpresaTPV *emp ) : QPushButton ( text,  parent )
{
    _depura ( "MyDevButton::MyDevButton", 0 );
    m_emp = emp;
    connect ( this, SIGNAL ( released() ), this, SLOT ( on_click() ) );
    _depura ( "END MyDevButton::MyDevButton", 0 );
}


///
/**
**/
MyDevButton::~MyDevButton()
{
    _depura ( "MyDevButton::~MyDevButton", 0 );
    _depura ( "END MyDevButton::~MyDevButton", 0 );
}


///
/**
\return
**/
void MyDevButton::on_click (  )
{
    _depura ( "MyDevButton::cambia", 0 );
    Devolucion *camb = new Devolucion ( m_emp, 0 );
    camb->show();
    _depura ( "END MyDevButton::cambia", 0 );
}


///
/**
\return
**/
int entryPoint ( BulmaTPV *tpv )
{
    _depura ( "entryPoint", 0 );

    /// Inicializa el sistema de traducciones 'gettext'.
    setlocale(LC_ALL, "");
    bindtextdomain ("plugindevolucion2", confpr->valor(CONF_DIR_TRADUCCION).toAscii().constData());

    g_doc1 = new BlDockWidget ( _("Articulo"), tpv, "articulodock" );
    g_doc1->setFeatures ( QDockWidget::AllDockWidgetFeatures );
    g_doc1->setGeometry ( 100, 100, 100, 500 );
    g_doc1->resize ( 330, 400 );
    tpv->addDockWidget ( Qt::TopDockWidgetArea, g_doc1 );
    g_doc1->show();
    _depura ( "END entryPoint", 0 );
    return 0;
}

int EmpresaTPV_createMainWindows_Post ( EmpresaTPV *etpv )
{
    // ============ Pruebas con abrevs
    g_plug = new MyDevButton ( "DEVOLUCION", etpv, etpv );
    g_plug->setMinimumSize ( QSize ( 72, 72 ) );
    g_plug->setMaximumSize ( QSize ( 200, 72 ) );
    g_doc1->setWidget ( g_plug );

    /* QFrame *fr = g_main->findChild<QFrame *> ( "mui_frameabrevs" );
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



