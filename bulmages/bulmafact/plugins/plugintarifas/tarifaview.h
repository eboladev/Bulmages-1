/***************************************************************************
 *   Copyright (C) 2004 by Alvaro de Miguel                                *
 *   alvaro_demiguel@gmail.com                                             *
 *   Copyright (C) 2008 by Fco. Javier M. C.                               *
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

#ifndef TARIFAVIEW_H
#define TARIFAVIEW_H

#include "ui_tarifabase.h"
#include "blpostgresqlclient.h"
#include "dialogchanges.h"
#include "fichabf.h"


class TarifaView : public FichaBf, public Ui_TarifaBase
{
    Q_OBJECT

private:
    QString m_idtarifa;
    QString m_nomtarifa;
    BlDbRecordSet *m_cursorcombo;

public:
    TarifaView ( BfCompany *emp, QWidget *parent = 0 );
    ~TarifaView();
    BfCompany *companyact();

public:
    void pintar();
    int cargar ( QString );
    int guardar();
    int cargarcomboiva ( QString );
    QString formaQuery ( QString );
    void on_mui_borrar_clicked();

public slots:
    virtual void on_mui_crear_clicked();
    virtual void on_mui_actualizar_clicked();

signals:
     void guardartarifa();
};

#endif

