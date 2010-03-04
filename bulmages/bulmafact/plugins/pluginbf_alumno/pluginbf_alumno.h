/***************************************************************************
 *   Copyright (C) 2005 by Tomeu Borras Riera                              *
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

#include "blpostgresqlclient.h"
#include "bfbulmafact.h"
#include "blwidget.h"
#include "bfbuscarreferencia.h"
#include "bfsubform.h"
#include "pdefs_pluginbf_alumno.h"


extern "C" PLUGINBF_ALUMNO_EXPORT int entryPoint ( BfBulmaFact * );
extern "C" PLUGINBF_ALUMNO_EXPORT int BfCompany_createMainWindows_Post(BfCompany *);
extern "C" PLUGINBF_ALUMNO_EXPORT int BfSubForm_pressedAsterisk(BfSubForm *);
// extern "C" PLUGINBF_ALUMNO_EXPORT int BlSubForm_BlSubForm_Post ( BlSubForm * );
extern "C" PLUGINBF_ALUMNO_EXPORT int BlSubForm_BlSubForm_Post ( BlSubForm * );


class MyPlugProf : public QObject, BlMainCompanyPointer
{
    Q_OBJECT

public:
    BfBulmaFact *m_bges;

public:
    MyPlugProf();
    ~MyPlugProf();
    void inicializa ( BfBulmaFact * );

public slots:
    void elslot();
    void elslot1();
    void elslot2();
    void elslot3();
};



class MyPlugAl1 : public QObject
{
    Q_OBJECT

public:
    MyPlugAl1 ( BlSubForm * );
    ~MyPlugAl1();
    void editarAlumno( QString);
    void nuevoAlumno();
    void seleccionarAlumno(BfSubForm *);

public slots:
    virtual void s_pintaMenu ( QMenu * );
    virtual void s_trataMenu ( QAction * );
};


