/***************************************************************************
 *   Copyright (C) 2004 by Josep Burcion                                   *
 *   josep@burcion.com                                                     *
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
 ***************************************************************************/
#include "bpasswd.h"

BPasswd::BPasswd ( QString * usuario, QString * passwd, QWidget * parent, const char * name, bool modal, WFlags f )
        : UIPassword ( parent, name, modal, f )
{

    Usuario = usuario;
    Password = passwd;
    userLine->setText ( *Usuario );
}

void BPasswd::returnPressedOnUser()
                    {
                        passwdLine->setFocus();
                    }

                    void BPasswd::returnPressedOnPasswd()
                                        {
                                            validar();
                                        }

                                        void BPasswd::validar()
                                        {
                                            *Usuario = userLine->text();
                                            *Password = passwdLine->text();
                                            delete this;
                                        }

                                        BPasswd::~BPasswd()
                                        {}


