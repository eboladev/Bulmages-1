/***************************************************************************
 *   Copyright (C) 2005 by Tomeu Borras Riera                              *
 *   tborras@conetxia.com                                                  *
 *   Copyright (C) 2006 by Fco. Javier M. C. (Porting to QT4)              *
 *   fcojavmc@todo-redes.com                                               *
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

#ifndef LISTPROVINCIASVIEW_H
#define LISTPROVINCIASVIEW_H

#include "ui_listprovincias.h"


class company;


class ListProvinciasView : public QDialog, private Ui::ListProvinciasBase {
    Q_OBJECT

public:
    company *companyact;

public:
    ListProvinciasView(company *, QDialog *parent = 0);
    ~ListProvinciasView();
    void inicializa();
    int guardalinea(int);

public slots:
    virtual void on_botonnew_clicked();
    virtual void on_botonsave_clicked();
    virtual void on_botondelete_clicked();
};

#endif

