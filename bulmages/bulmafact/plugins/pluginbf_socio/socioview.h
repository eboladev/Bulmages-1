/***************************************************************************
  *   Copyright (C) 2009 by Xavier Rusiñol Escriu                           *
 *   russi7@gmail.com                                                      *
 *                                                                         *
 *   Copyright (C) 2009 by Arturo Martin Llado                             *
 *   amartin@conetxia.com                                                  *
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

#ifndef SOCIOVIEW_H
#define SOCIOVIEW_H

#include <QtCore/QObject>

#include "blpostgresqlclient.h"
#include "bfform.h"

class BfCompany;

class ListAlumnosSocioView : public BfSubForm
{
    Q_OBJECT

public:
    ListAlumnosSocioView ( QWidget *parent = 0 );
    ~ListAlumnosSocioView() {};
    
public slots:
    virtual void load ( QString idcontrato );
};

#include "ui_sociobase.h"

class SocioView : public BfForm, public Ui_TutorBase
{
    Q_OBJECT

public:
    SocioView ( BfCompany *, QWidget * );
    ~SocioView();
    virtual void imprimir();
    virtual QString templateName(void) ;
    virtual int afterSave();
    virtual int beforeDelete();
    virtual int cargarPost(QString id);
    
public slots:
   virtual void on_mui_sociocliente_toggled(bool);
};

#endif
