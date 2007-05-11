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
#include "ficha.h"
#include "ui_clientebase.h"


/** Clase que se encarga de la pantalla de cliente.
    Deriva de la clase Ficha para la estandarizacion de pantallas y
    de la clase Cliente para el manejo con la base de datos.
    Se encarga de recibir y lanzar eventos.
*/
class ClienteView : public Cliente, public Ui_ClienteBase  {
    Q_OBJECT

public:
    ClienteView(company *emp, QWidget *parent = 0);
    ~ClienteView();

public:
    int cargar(QString client);
    virtual int guardar();
    void deleteClient();
    void emptyForm();
    void pintaidcliente(QString);
    void pintanomcliente(QString val);
    void pintanomaltcliente(QString val);
    void pintacifcliente(QString val);
    void pintacodcliente(QString val);
    void pintacorpcliente(QString val);
    void pintabancocliente(QString val);
    void pintadircliente(QString val);
    void pintapoblcliente(QString val);
    void pintacpcliente(QString val);
    void pintatelcliente(QString val);
    void pintateltrabcliente(QString val);
    void pintamovilcliente(QString val);
    void pintafaxcliente(QString val);
    void pintamailcliente(QString val);
    void pintaurlcliente(QString val);
    void pintafaltacliente(QString);
    void pintafbajacliente(QString);
    void pintacomentcliente(QString val);
    void pintainactivocliente(QString);
    void pintaprovcliente(QString val);
    void pintaregimenfiscalcliente(QString val);
    void pintarecargoeqcliente(QString val);
    void pintaidforma_pago(QString val);

public slots:
    /// \TODO: Muchos metodos aqui implementados deberian estar en Ficha.
    virtual void on_mui_guardar_clicked();
    virtual void on_mui_borrar_clicked();
    virtual void on_mui_aceptar_clicked();
    virtual void on_mui_informe_clicked();
    virtual void on_mui_imprimir_clicked();
};

#endif

