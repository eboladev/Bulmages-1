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
#include "logpass.h"
#include "postgresiface2.h"
#include <qlineedit.h>
#include <qstring.h>
#include <qlabel.h>

logpass::logpass(QWidget *parent, const char *name)
 : logpassbase(parent, name) {
 	validar();
}


logpass::~logpass() {
   delete metabase;
}

void logpass::validar() {
   login  = postgresiface2::sanearCadena(m_login->text());
   password = m_password->text();
   authOK = false;
   
   //Comprobamos si es un usuario válido
   metabase = new postgresiface2();
   if(!metabase->inicializa( "template1", login, password ) ) {
   	authOK = true;
   }// end if
   delete metabase;
    
   //Si es valido abrimos el selector y si no mostramos un error y limpiamos el formulario
   if (authOK) {
       	confpr->setValor(CONF_LOGIN_USER,login);
	confpr->setValor(CONF_PASSWORD_USER,password);
      close();
   } else {
      lblAuthError->setText(tr("Error: usuario y/o contraseña incorrectos"));
      m_login->setText("");
      m_password->setText("");
      m_login->setFocus();
   }// end if
}// end validar


