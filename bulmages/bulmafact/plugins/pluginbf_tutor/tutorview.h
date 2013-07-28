/***************************************************************************
 *   Copyright (C) 2009 by Tomeu Borras Riera                              *
 *   tborras@conetxia.com                                                  *
 *   http://www.iglues.org                                                 *
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

#ifndef TUTORVIEW_H
#define TUTORVIEW_H

#include <QtCore/QObject>

#include "blpostgresqlclient.h"
#include "bfform.h"


class BfCompany;


class ListAlumnosTutorView : public BfSubForm
{
    Q_OBJECT

public:
    ListAlumnosTutorView ( QWidget *parent = 0 );
    ~ListAlumnosTutorView() {};
    
public slots:
    virtual void load ( QString idcontrato );
};


#include "ui_tutorbase.h"


class TutorView : public BfForm, public Ui_TutorBase
{
    Q_OBJECT

public:
    TutorView ( BfCompany *, QWidget * );
    ~TutorView();
    virtual void imprimir();
    virtual QString templateName(void) ;
    virtual int afterSave();
    virtual int beforeDelete();
    virtual int cargarPost(QString id);
    
public slots:
   virtual void on_mui_sociocliente_toggled(bool);
};

#endif

