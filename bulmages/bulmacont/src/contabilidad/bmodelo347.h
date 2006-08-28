/***************************************************************************
 *   Copyright (C) 2003 by Josep Burcion                                   *
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
#ifndef BModelo347_H
#define BModelo347_H

#include "ui_modelo347base.h"
#include "funcaux.h"
#include "postgresiface2.h"
#include <qlineedit.h>
#include <q3table.h>


/** @author Josep Burcion */

class BModelo347 : public QDialog, public Ui_Modelo347Base
{

public:
    BModelo347(postgresiface2 *DBConn=0,QString ejerActual="",QWidget * parent = 0, const char * name = 0, Qt::WFlags f = Qt::WType_TopLevel);
    ~BModelo347();


public slots:
    virtual void click_recargar();
    virtual void click_imprimir();
    virtual void formatea_fecha();
private:
        postgresiface2 * DBConn;
	char *fichero;
        //QWorkspace * zona0;
        //BSelector * PunteroAlSelector;

};

#endif

