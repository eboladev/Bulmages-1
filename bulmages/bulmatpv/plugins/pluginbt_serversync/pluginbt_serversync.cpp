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

#include <QtWidgets/QToolButton>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QTableWidget>
#include <QtCore/QFile>
#include <QtCore/QTextStream>

#include "pluginbt_serversync.h"
#include "blfunctions.h"
#include "btticket.h"
#include "btcompany.h"
#include "serversync.h"
#include "bldockwidget.h"

BlDockWidget *g_doc1 = NULL;
ServerSync *g_bud = NULL;

///
/**
\return
**/
int entryPoint ( BtBulmaTPV *tpv )
{
    BL_FUNC_DEBUG

    /// Inicializa el sistema de traducciones 'gettext'.
    setlocale(LC_ALL, "");
    blBindTextDomain ("pluginbt_serversync", g_confpr->value(CONF_DIR_TRADUCCION).toLatin1().constData());

    /// Vamos a probar con un docwindow.
    g_doc1 = new BlDockWidget ( "Servidor", tpv, "servidor" );
    g_doc1->setFeatures ( QDockWidget::AllDockWidgetFeatures );
    
    g_doc1->setGeometry ( 100, 100, 100, 500 );
    g_doc1->resize ( 330, 400 );

    /// Presentamos el widget
    tpv->addDockWidget ( Qt::RightDockWidgetArea, g_doc1 );

    g_doc1->loadConfig();

    g_doc1->show();

    
    
    return 0;
}

///
/**
\return
**/
int exitPoint ( BtBulmaTPV *tpv )
{
    BL_FUNC_DEBUG
    
    delete g_doc1;
    
    
    
    return 0;
}

int BtCompany_createMainWindows_Post ( BtCompany *etpv )
{
    BL_FUNC_DEBUG
    
    g_bud =  new ServerSync ( etpv, g_doc1 );
    g_doc1->setWidget ( ( QWidget * ) g_bud );
    
    
    
    return 0;
}



int Abrevs_on_mui_aparcar_clicked_Post(Abrevs *av) {
   QString texto = ((BtCompany *)av->mainCompany())->exportXML();
   g_bud->send(texto);
}

int Abrevs_on_mui_recuperar_clicked_Post(Abrevs *av) {
   QString texto = ((BtCompany *)av->mainCompany())->exportXML();
   g_bud->send(texto);
}

int BtCompany_cobrar_Post(BtCompany *etpv)
{
   QString texto = etpv->exportXML();
   g_bud->send(texto);  
}

