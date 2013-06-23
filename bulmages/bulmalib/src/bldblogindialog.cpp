/***************************************************************************
 *   Copyright (C) 2004 by Tomeu Borras                                    *
 *   tborras@conetxia.com                                                  *
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

#include <QtWidgets/QLineEdit>
#include <QtCore/QString>
#include <QtWidgets/QLabel>

#include "bldblogindialog.h"
#include "blpostgresqlclient.h"


///
/**
\return
**/
bool BlDbLoginDialog::authOK()
{
    BL_FUNC_DEBUG
    
    return m_authOK;
}


///
/**
\param parent
\param name
**/
BlDbLoginDialog::BlDbLoginDialog ( QWidget *parent, const char *name ) : QDialog ( parent )
{
    BL_FUNC_DEBUG
    setWindowTitle ( name );
    setupUi ( this );
    grpAuthError->setVisible ( true );
    QObject::connect ( pbValidar, SIGNAL ( clicked() ), this, SLOT ( validate() ) );
    QObject::connect ( pbCerrar, SIGNAL ( clicked() ), this, SLOT ( close() ) ); 
}


///
/**
**/
BlDbLoginDialog::~BlDbLoginDialog()
{
    BL_FUNC_DEBUG
    
}


/// Valida si postgres puede abrir bases de datos y si no es asi pide login y password.
/**
**/
void BlDbLoginDialog::validate()
{
    BL_FUNC_DEBUG
    m_authOK = false;

    g_confpr->setValue ( CONF_LOGIN_USER, m_login->text() );
    g_confpr->setValue ( CONF_PASSWORD_USER, m_password->text() );
    g_confpr->setValue ( CONF_SERVIDOR, m_host->text() );
    g_confpr->setValue ( CONF_PUERTO, m_port->text() );
    g_confpr->setValue ( CONF_DBNAME, m_db->text());

    /// Comprobamos si es un usuario v&aacute;lido.
    metabase = new BlPostgreSqlClient();
    if ( !metabase->inicializa (m_db->text() ) ) {
        m_authOK = true;
    } else {
	g_confpr->setValue( CONF_DBNAME, ""); // Vaciamos el DBNAME para que saque el selector.
    } // end if
    delete metabase;
    if ( !m_authOK ) {
        metabase = new BlPostgreSqlClient();
        if ( !metabase->inicializa ( "template1" ) ) {
            m_authOK = true;
        } // end if
        delete metabase;
    } // end if

    /// Si es v&aacute;lido abrimos el selector y si no mostramos un error y limpiamos
    /// el formulario.
    if ( m_authOK ) {
        grpAuthError->setVisible ( false );
        done ( 1 );
    } else {
        grpAuthError->setVisible ( true );
        lblAuthError->setText ( _ ( "Error: usuario y/o contrasenya incorrectos" ) );
        m_login->setFocus();
    } // end if
    
}

