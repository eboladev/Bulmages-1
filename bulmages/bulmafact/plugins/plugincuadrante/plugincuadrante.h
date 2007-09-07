/***************************************************************************
 *   Copyright (C) 2007 by Tomeu Borras Riera                              *
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

#ifdef Q_WS_WIN
# define MY_EXPORT __declspec(dllexport)
#else
# define MY_EXPORT
#endif

#include <QStringList>
#include <QWidget>
#include <QIcon>
#include <QObject>

#include "qapplication2.h"
#include "bulmafact.h"
#include "postgresiface2.h"
#include "blwidget.h"
#include "almacenview.h"
#include "trabajadorview.h"

extern "C" MY_EXPORT void entryPoint(Bulmafact *);
extern "C" MY_EXPORT int AlmacenView_AlmacenView(AlmacenView *);
extern "C" MY_EXPORT int TrabajadorView_TrabajadorView(TrabajadorView *);
extern "C" MY_EXPORT int TrabajadorView_on_mui_lista_currentItemChanged_Post(TrabajadorView *trab);
extern "C" MY_EXPORT int TrabajadorView_on_mui_guardar_clicked(TrabajadorView *trab);
extern QApplication2 *theApp;

class myplugin4 : public QObject, PEmpresaBase {
    Q_OBJECT

public:
    Bulmafact *m_bulmafact;

public:
    myplugin4();
    ~myplugin4();
    void inicializa(Bulmafact *);

public slots:
    void elslot();
};

