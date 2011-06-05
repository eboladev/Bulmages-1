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
#include <QLineEdit>
#include <QTableWidget>

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
    blDebug ( "MyPlugArticuloSubForm::MyPlugArticuloSubForm", 0 );
    blDebug ( ("END ", Q_FUNC_INFO), 0 );
}


///
/**
**/
MyPlugArticuloSubForm::~MyPlugArticuloSubForm()
{
    blDebug ( "MyPlugArticuloSubForm::~MyPlugArticuloSubForm", 0 );
    blDebug ( ("END ", Q_FUNC_INFO), 0 );
}


///
/**
**/
void MyPlugArticuloSubForm::elslot ( QString item )
{
    blDebug ( "MyPlugArticuloSubForm::elslot", 0 );
    QString idarticulo =  m_lan->mui_list->dbValue ( "idarticulo" );
    m_etpv->ticketActual() ->insertarArticulo ( idarticulo, BlFixed ( "1" ) );
    blDebug ( ("END ", Q_FUNC_INFO), 0 );
}


void MyPlugArticuloSubForm::elslot1 ( QTableWidgetItem *item )
{
    blDebug ( "MyPlugArticuloSubForm::elslot", 0 );
    QString idarticulo =  m_lan->mui_list->dbValue ( "idarticulo" );
    m_etpv->ticketActual() ->insertarArticulo ( idarticulo, BlFixed ( "1" ) );
    blDebug ( ("END ", Q_FUNC_INFO), 0 );
}

///
/**
\param bges
**/
void MyPlugArticuloSubForm::inicializa ( BtCompany *etpv )
{
    blDebug ( "MyPlugArticuloSubForm::inicializa", 0 );
    m_etpv = etpv;
    blDebug ( ("END ", Q_FUNC_INFO), 0 );
}

#endif

///
/**
\return
**/
int entryPoint ( BtBulmaTPV *tpv )
{
    blDebug ( "entryPoint", 0 );

    /// Inicializa el sistema de traducciones 'gettext'.
    setlocale ( LC_ALL, "" );
    blBindTextDomain ( "pluginbt_articulosubform", g_confpr->value( CONF_DIR_TRADUCCION ).toAscii().constData() );
    blDebug ( ("END ", Q_FUNC_INFO), 0 );
    return 0;
}


int BtCompany_createMainWindows_Post ( BtCompany *etpv )
{
    blDebug("BtCompany_createMainWindows_Post",0);
    ArticuloList1 *lan = new ArticuloList1 ( ( BfCompany * ) etpv, NULL, 0, BL_SELECT_MODE );
    ((BtBulmaTPV *)g_main)->setCentralBtWidget ( lan );
    blDebug("END BtCompany_createMainWindows_Post",0);

    return 0;
}
