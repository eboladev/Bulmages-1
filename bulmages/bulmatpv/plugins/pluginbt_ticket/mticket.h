/***************************************************************************
 *   Copyright (C) 2007 by Tomeu Borras Riera                              *
 *   tborras@conetxia.com                                                  *
 *   http://www.iglues.org                                                 *
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

#ifndef MTICKET_H
#define MTICKET_H

#include <QLabel>
#include <QTableWidget>

#include "btticket.h"
#include "btcompany.h"
#include "ui_mticketbase.h"
#include "btsubform.h"


class MTicket : public BlWidget, public Ui_MTicketBase
{
    Q_OBJECT

private:
    QWidget* m_parent;
    BtCompany* m_btCompany;

public:
    MTicket ( BtCompany *btCompany, QWidget *parent );
    virtual ~MTicket();
    virtual void pintar();

public slots:
    virtual void on_mui_subir_clicked();
    virtual void on_mui_bajar_clicked();
    virtual void on_mui_borrar_clicked();
    virtual void on_mui_imprimir_clicked();
    virtual void on_mui_reimprimir_clicked();
    virtual void on_mui_borrarticket_clicked();
    virtual void on_mui_formatear_clicked();
    virtual void on_mui_nextLineIsInsert_toggled(bool checked);
};



#endif

