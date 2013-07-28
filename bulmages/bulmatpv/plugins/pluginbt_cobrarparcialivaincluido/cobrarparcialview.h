/***************************************************************************
 *   Copyright (C) 2011 by Fco. Javier M. C.                               *
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

#ifndef COBRARPARCIAL_H
#define COBRARPARCIAL_H

#include <QtCore/QObject>

#include "blwidget.h"
#include "btcompany.h"
#include "btticket.h"
#include "ui_cobrarparcialbase.h"


class CobrarParcialView : public BlWidget, public Ui_CobrarParcialBase
{
    Q_OBJECT

private:
    BtTicket *m_ticketOrigen;
    BtTicket *m_ticketDestino;

public:
    CobrarParcialView(BtCompany *emp, QWidget *parent);
    ~CobrarParcialView();
    void pintar();
    void pintarOrigen();
    void pintarDestino();
    void intercambiarLineasTicket(BtTicket *origen, BtTicket *destino, bool lineaCompleta = false, float cantidad = 1);
    void pintarTotal(BtTicket *tick, QLineEdit *lineEdit);


public slots:
    void on_mui_aceptar_clicked();
    void on_mui_cancelar_clicked();
    void on_mui_unidad2destino_clicked();
    void on_mui_unidad2origen_clicked();
    void on_mui_linea2destino_clicked();
    void on_mui_linea2origen_clicked();
    void on_mui_destino_arriba_clicked();
    void on_mui_destino_abajo_clicked();
    void on_mui_origen_arriba_clicked();
    void on_mui_origen_abajo_clicked();

};


#endif

