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

#include <QLineEdit>
#include <QString>
#include <QLabel>

#include "logpass.h"
#include "funcaux.h"

#include "postgresiface2.h"


logpass::logpass(QWidget *parent, const char *name) : QDialog(parent, name) {
    setupUi(this);
    QObject::connect(pushButton24, SIGNAL(clicked()), this, SLOT(validar()));
    validar();
}


logpass::~logpass() {}


/// Valida si postgres puede abrir bases de datos y si no es asi pide login y password.
void logpass::validar() {
    _depura("logpass::validar()");
    m_login->setText(postgresiface2::sanearCadena(m_login->text()));
    m_authOK = false;

    confpr->setValor(CONF_LOGIN_USER, m_login->text());
    confpr->setValor(CONF_PASSWORD_USER, m_password->text());

    /// Comprobamos si es un usuario valido.
    metabase = new postgresiface2();
    if(!metabase->inicializa("template1")) {
        m_authOK = true;
    } // end if
    delete metabase;

    /// Si es valido abrimos el selector y si no mostramos un error y limpiamos el formulario.
    if (m_authOK) {
        done(1);
    } else {
        lblAuthError->setText(tr("Error: usuario y/o contraseña incorrectos"));
        m_login->setText("");
        m_password->setText("");
        m_login->setFocus();
    } // end if
}

