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
#include <QCheckBox>

#include "pluginbf_camarero.h"


///
/**
\return
**/
int entryPoint ( BfBulmaFact * )
{
    blDebug ( "Estoy dentro del plugin de iva incluido", 0 );
    /// Inicializa el sistema de traducciones 'gettext'.
    setlocale ( LC_ALL, "" );
    blBindTextDomain ( "pluginbf_camarero", g_confpr->valor ( CONF_DIR_TRADUCCION ).toAscii().constData() );

    return 0;
}


/// Al crear la ventana de trabajadores tambien creamos un combo box para el tipo de trabajador.
/**
\param trab
\return
**/
int TrabajadorView_TrabajadorView_Post ( TrabajadorView *trab )
{
    blDebug ( "PluginBfCamarero::TrabajadorView_TrabajadorView_Post", 0 );

    QHBoxLayout *hboxLayout160 = new QHBoxLayout();
    hboxLayout160->setSpacing ( 2 );
    hboxLayout160->setMargin ( 0 );
    hboxLayout160->setObjectName ( QString::fromUtf8 ( "hboxLayout16" ) );

    QLabel *textLabel2_9_26 = new QLabel ( trab->m_frameplugin );
    textLabel2_9_26->setObjectName ( QString::fromUtf8 ( "textLabel2_9_2" ) );
    hboxLayout160->addWidget ( textLabel2_9_26 );
    textLabel2_9_26->setText ( "Contraseña" );

    QLineEdit *tipotraba = new QLineEdit ( trab->m_frameplugin );
    tipotraba->setObjectName ( QString::fromUtf8 ( "mui_passwordtrabajador" ) );
    tipotraba->setMaximumSize(200,25);
    hboxLayout160->addWidget ( tipotraba );
    
    QCheckBox *check1 = new QCheckBox (trab->m_frameplugin );
    check1->setText("Administrador");
    check1->setObjectName ( QString::fromUtf8 ( "mui_admintrabajador"));
    hboxLayout160->addWidget ( check1 );
    
    QCheckBox *check2 = new QCheckBox (trab->m_frameplugin );
    check2->setText("Gerente");
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
    
    blDebug ( _("END PluginBfCamarero::TrabajadorView_TrabajadorView_Post"), 0 );
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
    query += " admintrabajador = " + QString(l1->isChecked()?"TRUE":"FALSE");
    query += " WHERE idtrabajador=" + trab->mainCompany() ->sanearCadena ( trab->mdb_idtrabajador );
    trab->mainCompany() ->begin();
    trab->mainCompany() ->runQuery ( query );
    trab->mainCompany() ->commit();

    QCheckBox * l2 = trab->findChild<QCheckBox *> ( "mui_gerentetrabajador" );
    query = "UPDATE trabajador SET ";
    query += " gerentetrabajador = " + QString(l2->isChecked()?"TRUE":"FALSE");
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
    blDebug("pluginbf_camarero::TrabajadorView_on_mui_lista_currentItemChanged_Post");
    if (trab) {
	QLineEdit * l = trab->findChild<QLineEdit *> ( "mui_passwordtrabajador" );
	QCheckBox * l1 = trab->findChild<QCheckBox *> ( "mui_admintrabajador" );
	QCheckBox * l2 = trab->findChild<QCheckBox *> ( "mui_gerentetrabajador" );
	if (l) {
	    BlDbRecordSet *cur = trab->mainCompany() ->loadQuery ( "SELECT * FROM trabajador WHERE idtrabajador = " + trab->mdb_idtrabajador );
	    if ( !cur->eof() ) {
		l->setText ( cur->valor ( "passwordtrabajador" ) );
		l1->setChecked(cur -> valor("admintrabajador") == "t");
		l2->setChecked(cur -> valor("gerentetrabajador") == "t");
	    } // end if
	    delete cur;
	} // end if
    } // end if
    blDebug("END pluginbf_camarero::TrabajadorView_on_mui_lista_currentItemChanged_Post");
    return 0;
}


