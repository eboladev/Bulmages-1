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

#include "pluginbt_articulosubform.h"
#include "blfunctions.h"
#include "btcompany.h"
#include "bfbuscararticulo.h"
#include "articulolisttpv.h"

#ifdef NOCOMPILAR

class MyPlugArticuloSubForm : public QObject
{
    Q_OBJECT

private:
    BtCompany *m_etpv;
    ArticuloList1 *m_lan;

public:
    MyPlugArticuloSubForm();
    ~MyPlugArticuloSubForm();
    void inicializa ( BtCompany *etpv );

public slots:
    virtual void elslot ( QString );
    virtual void elslot1 ( QTableWidgetItem * );
};


///
/**
**/
MyPlugArticuloSubForm::MyPlugArticuloSubForm()
{
    BL_FUNC_DEBUG
    
}


///
/**
**/
MyPlugArticuloSubForm::~MyPlugArticuloSubForm()
{
    BL_FUNC_DEBUG
    
}


///
/**
**/
void MyPlugArticuloSubForm::elslot ( QString item )
{
    BL_FUNC_DEBUG
    QString idarticulo =  m_lan->mui_list->dbValue ( "idarticulo" );
    m_etpv->ticketActual() ->insertarArticulo ( idarticulo, BlFixed ( "1" ) );
    
}


void MyPlugArticuloSubForm::elslot1 ( QTableWidgetItem *item )
{
    BL_FUNC_DEBUG
    QString idarticulo =  m_lan->mui_list->dbValue ( "idarticulo" );
    m_etpv->ticketActual() ->insertarArticulo ( idarticulo, BlFixed ( "1" ) );
    
}

///
/**
\param bges
**/
void MyPlugArticuloSubForm::inicializa ( BtCompany *etpv )
{
    BL_FUNC_DEBUG
    m_etpv = etpv;
    
}

#endif


ArticuloList1 *g_subform = NULL;

///
/**
\return
**/
int entryPoint ( BtBulmaTPV *tpv )
{
    BL_FUNC_DEBUG

    /// Inicializa el sistema de traducciones 'gettext'.
    setlocale ( LC_ALL, "" );
    blBindTextDomain ( "pluginbt_articulosubform", g_confpr->value( CONF_DIR_TRADUCCION ).toLatin1().constData() );
    
    return 0;
}


int BtCompany_createMainWindows_Post ( BtCompany *etpv )
{
    BL_FUNC_DEBUG

    BlDebug::blDebug("BtCompany_createMainWindows_Post",0);
    g_subform = new ArticuloList1 ( ( BfCompany * ) etpv, etpv, 0, BL_SELECT_MODE );
    ((BtBulmaTPV *)g_main)->setCentralBtWidget ( g_subform );
    BlDebug::blDebug("END BtCompany_createMainWindows_Post",0);

    return 0;
}

int BtBulmaTPV_closeEvent(BtBulmaTPV *etpv) {
    BL_FUNC_DEBUG
    delete g_subform;
    
    return 0;
}
