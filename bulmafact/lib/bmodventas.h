/***************************************************************************
 *   Copyright (C) 2004 by Josep Burcion                                   *
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
#ifndef BMODVENTAS_H
#define BMODVENTAS_H

#include ".ui/uiventas.h"


#include "bfempresa.h"
#include "balbaventa.h"
#include "bpediventa.h"
#include "bclientes.h"
#include "barticulos.h"
#include "bpasswd.h"
#include "bvisorempresas.h"

#include <qworkspace.h>
#include <qvbox.h>
#include <qframe.h>

class BfCursor;

class BModVentas : public UIVentas
{
    Q_OBJECT

public:
    BModVentas(QString* usuario, QString* passwd, QString* dataBase, QWidget * parent = 0, const char * name = 0, WFlags f = WType_TopLevel);
    ~BModVentas();
    
   
public slots:    
    virtual void albaranes();
    virtual void pedidos();
    virtual void fichaClientes();
    virtual void fichaArticulos();
    virtual void mostrar_selector();
    
private:
    void cargaUsuario();   
    int seleccionaEmpresa();   
    QWorkspace * zona0; 
    QString * Usuario;
    QString * Password;
    QString * DataBase;
    int intentosFallidosPassword;
    BfEmpresa* empresaTrabajo;

signals:
    virtual void clickEnlace();

};

#endif
