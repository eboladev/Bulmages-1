/***************************************************************************
 *   Copyright (C) 2006 by Tomeu Borras Riera                              *
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

#ifndef FICHA_H
#define FICHA_H

#include <QWidget>
#include <QCloseEvent>

#include "dialogchanges.h"
#include "funcaux.h"


class Ficha : public QWidget, public dialogChanges {
    Q_OBJECT

public:
    Ficha(QWidget *parent = 0, Qt::WFlags f = 0);
    ~Ficha() {}
    ;
    virtual int guardar() {
        _depura("Ficha::guardar existe solo para ser derivado", 2);
        return 0;
    };
    virtual int borrar() {
        _depura("Ficha::borrar existe solo para ser derivado", 2);
        return 0;
    };
    virtual void closeEvent(QCloseEvent *);

public slots:
    virtual void on_mui_aceptar_clicked();
    virtual void on_mui_borrar_clicked();
    /// TODO: Se tiene que pasar de cancelar a cerrar en todas las ventanas.
    /// Despues quedara obsoleto.
    virtual void on_mui_cancelar_clicked() {
        close();
    };
    virtual void on_mui_cerrar_clicked() {
        close();
    };
    virtual void on_mui_guardar_clicked() {
	guardar();
    };
};

#endif

