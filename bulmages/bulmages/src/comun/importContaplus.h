/***************************************************************************
 *   Copyright (C) 2003 by Tomeu Borras                                    *
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

#ifndef _IMPORTCONTAPLUS_H_
#define _IMPORTCONTAPLUS_H_

#include <QDialog>

#include "ui_importContaplusBase.h"
#include "pgimportfiles.h"
#include "postgresiface2.h"


class importContaplus: public QDialog, public Ui_importContaplusBase, public pgimportfiles {
    Q_OBJECT

private:
    postgresiface2 *conexionbase;

public:
    importContaplus(postgresiface2 *, QWidget *, const char *, Qt::WFlags);
    ~importContaplus() {}
    ;
    void alerta(int, int);
    void mensajeria(QString);

public slots:
    virtual void botonBuscarSubCta();
    virtual void botonBuscarDiario();
    virtual void botonBuscarXML();
    virtual void botonImportar();
    virtual void botonExportar();
};

#endif

