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
#include <QtWidgets/QCheckBox>

#include "pluginbf_camarero.h"
#include "trabajadores.h"


///
/**
\return
**/
int entryPoint ( BfBulmaFact *bges )
{
    BL_FUNC_DEBUG

    /// El plugin necesita un parche en la base de datos para funcionar.
    bges->company()->dbPatchVersionCheck("PluginBf_Camarero", "0.12.1-0000");

    /// Inicializa el sistema de traducciones 'gettext'.
    setlocale ( LC_ALL, "" );
    blBindTextDomain ( "pluginbf_camarero", g_confpr->value( CONF_DIR_TRADUCCION ).toLatin1().constData() );

    return 0;
}

int BfCompany_createMainWindows_Post ( BfCompany *etpv )
{
    Trabajadores *trab = new Trabajadores ( etpv, 0, true );
    trab->exec();
    delete trab;
    return 0;
}


/// Al crear la ventana de trabajadores tambien creamos un combo box para el tipo de trabajador.
/**
\param trab
\return
**/
int TrabajadorView_TrabajadorView_Post ( TrabajadorView *trab )
{
    BL_FUNC_DEBUG

    QHBoxLayout *hboxLayout160 = new QHBoxLayout();
    hboxLayout160->setSpacing ( 2 );
    hboxLayout160->setMargin ( 0 );
    hboxLayout160->setObjectName ( QString::fromUtf8 ( "hboxLayout16" ) );

    QLabel *textLabel2_9_26 = new QLabel ( trab->m_frameplugin );
    textLabel2_9_26->setObjectName ( QString::fromUtf8 ( "textLabel2_9_2" ) );
    hboxLayout160->addWidget ( textLabel2_9_26 );
    textLabel2_9_26->setText ( _("Clave numerica") );

    QLineEdit *tipotraba = new QLineEdit ( trab->m_frameplugin );
    tipotraba->setObjectName ( QString::fromUtf8 ( "mui_passwordtrabajador" ) );
    tipotraba->setMaximumSize(200,25);
    hboxLayout160->addWidget ( tipotraba );
    
    QCheckBox *check1 = new QCheckBox (trab->m_frameplugin );
    check1->setText(_("Administrador"));
    check1->setObjectName ( QString::fromUtf8 ( "mui_admintrabajador"));
    hboxLayout160->addWidget ( check1 );
    
    QCheckBox *check2 = new QCheckBox (trab->m_frameplugin );
    check2->setText(_("Gerente"));
    check2->setObjectName ( QString::fromUtf8 ( "mui_gerentetrabajador"));
    hboxLayout160->addWidget ( check2 );

    /// Comprobamos que exista el layout.
    QVBoxLayout *m_hboxLayout1 = trab->m_frameplugin->findChild<QVBoxLayout *> ( "hboxLayout1" );
    if ( !m_hboxLayout1 ) {
        m_hboxLayout1 = new QVBoxLayout ( trab->m_frameplugin );
        m_hboxLayout1->setSpacing ( 0 );
        m_hboxLayout1->setMargin ( 0 );
        m_hboxLayout1->setObjectName ( QString::fromUtf8 ( "hboxLayout1" ) );
    } // end if

    m_hboxLayout1->addLayout ( hboxLayout160 );
    
    
    return 0;
}


///
/**
\param trab
\return
**/
int TrabajadorView_on_mui_guardar_clicked ( TrabajadorView *trab )
{

    QLineEdit * l = trab->findChild<QLineEdit *> ( "mui_passwordtrabajador" );
    QString query = "UPDATE trabajador SET ";
    query += " passwordtrabajador = '" + l->text() + "'";
    query += " WHERE idtrabajador=" + trab->mainCompany() ->sanearCadena ( trab->mdb_idtrabajador );
    trab->mainCompany() ->begin();
    trab->mainCompany() ->runQuery ( query );
    trab->mainCompany() ->commit();
    
    
    QCheckBox * l1 = trab->findChild<QCheckBox *> ( "mui_admintrabajador" );
    query = "UPDATE trabajador SET ";
    query += " admintrabajador = " + QString(l1->isChecked()?"true":"false");
    query += " WHERE idtrabajador=" + trab->mainCompany() ->sanearCadena ( trab->mdb_idtrabajador );
    trab->mainCompany() ->begin();
    trab->mainCompany() ->runQuery ( query );
    trab->mainCompany() ->commit();

    QCheckBox * l2 = trab->findChild<QCheckBox *> ( "mui_gerentetrabajador" );
    query = "UPDATE trabajador SET ";
    query += " gerentetrabajador = " + QString(l2->isChecked()?"true":"false");
    query += " WHERE idtrabajador=" + trab->mainCompany() ->sanearCadena ( trab->mdb_idtrabajador );
    trab->mainCompany() ->begin();
    trab->mainCompany() ->runQuery ( query );
    trab->mainCompany() ->commit();
    
    
    return 0;
}


///
/**
\param trab
\return
**/
int TrabajadorView_on_mui_lista_currentItemChanged_Post ( TrabajadorView *trab )
{
    BL_FUNC_DEBUG
    if (trab) {
	QLineEdit * l = trab->findChild<QLineEdit *> ( "mui_passwordtrabajador" );
	QCheckBox * l1 = trab->findChild<QCheckBox *> ( "mui_admintrabajador" );
	QCheckBox * l2 = trab->findChild<QCheckBox *> ( "mui_gerentetrabajador" );
	if (l) {
	    BlDbRecordSet *cur = trab->mainCompany() ->loadQuery ( "SELECT * FROM trabajador WHERE idtrabajador = " + trab->mdb_idtrabajador );
	    if ( !cur->eof() ) {
		l->setText ( cur->value( "passwordtrabajador" ) );
		l1->setChecked(cur -> value("admintrabajador") == "t");
		l2->setChecked(cur -> value("gerentetrabajador") == "t");
	    } // end if
	    delete cur;
	} // end if
    } // end if
    return 0;
}


