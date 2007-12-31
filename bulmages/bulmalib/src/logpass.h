/***************************************************************************
 *   Copyright (C) 2004 by Tomeu Borras                                    *
 *   tborras@conetxia.com                                                  *
 *   http://www.iglues.org                                                 *
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

#ifndef LOGPASS_H
#define LOGPASS_H

#include <QDialog>

#include "ui_logpassbase.h"
#include "postgresiface2.h"


/// Presenta un di&aacute;logo que pregunta el usuario y la contrase&ntilde;a
/// para acceder a la base de datos.
class logpass : public QDialog, public Ui_logpassbase
{
    Q_OBJECT

private:
    bool m_authOK;

private:
    postgresiface2 *metabase;

public:
    logpass ( QWidget *parent = 0, const char *name = 0 );
    ~logpass();
    bool authOK();

public slots:
    virtual void validar();
};

#endif

