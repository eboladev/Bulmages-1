/***************************************************************************
 *   Copyright (C) 2004 by Marcos Mezo                                     *
 *   mmezo@selexco.net                                                     *
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

#ifndef CLIENTEVIEW_H
#define CLIENTEVIEW_H

#include "cliente.h"
#include "dialogchanges.h"
#include "ui_clientebase.h"


class company;

class ClienteView :  public QWidget, public Ui_ClienteBase, public Cliente, public dialogChanges  {
    Q_OBJECT

public:
    ClienteView(company *emp, QWidget *parent = 0, const char *name = 0);
    ~ClienteView();

public:
    int cargar(QString client);
    virtual int guardar();
    void deleteClient();
    void closeEvent(QCloseEvent *);
    void emptyForm();
    void pintaidcliente(QString ) {}
    ;
    void pintanomcliente(QString val) {
        m_nomcliente->setText(val);
    };
    void pintanomaltcliente(QString val) {
        m_nomaltcliente->setText(val);
    };
    void pintacifcliente(QString val) {
        m_cifcliente->setText(val);
    };
    void pintabancocliente(QString val) {
        m_bancocliente->setText(val);
    };
    void pintadircliente(QString val) {
        m_dircliente->setText(val);
    };
    void pintapoblcliente(QString val) {
        m_poblcliente->setText(val);
    };
    void pintacpcliente(QString val) {
        m_cpcliente->setText(val);
    };
    void pintatelcliente(QString val) {
        m_telcliente->setText(val);
    };
    void pintafaxcliente(QString val) {
        m_faxcliente->setText(val);
    };
    void pintamailcliente(QString val) {
        m_mailcliente->setText(val);
    };
    void pintaurlcliente(QString val) {
        m_urlcliente->setText(val);
    };
    void pintafaltacliente(QString ) {}
    ;
    void pintafbajacliente(QString ) {}
    ;
    void pintacomentcliente(QString val) {
        m_comentcliente->setText(val);
    };
    void pintainactivocliente(QString ) {}
    ;
    void pintaprovcliente(QString val) {
        m_provcliente->setProvincia(val);
    };

public slots:
    virtual void on_mui_guardar_clicked();
    virtual void on_mui_borrar_clicked();
    virtual void on_mui_aceptar_clicked() {
        if (!guardar())
        close();
    };
};

#endif

