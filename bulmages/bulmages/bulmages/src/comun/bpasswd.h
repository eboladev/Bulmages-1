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
#ifndef BPASSWD_H
#define BPASSWD_H

#include <uipassword.h>
#include <qlineedit.h>

/**
@author Josep Burcion
*/
class BPasswd : public UIPassword
{
    Q_OBJECT
public:
    BPasswd(QString * usuario, QString * passwd, QWidget * parent=0, const char * name=0, bool modal=FALSE, WFlags f=0);
    ~BPasswd();

        
private:
    QString * Usuario;
    QString * Password;

private slots:
    virtual void validar();
    virtual void returnPressedOnUser();
    virtual void returnPressedOnPasswd();

};

#endif
