/***************************************************************************
 *   Copyright (C) 2006 by Tomeu Borras Riera                              *
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

#ifndef ASIENTO1_H
#define ASIENTO1_H

#include <QString>

#include "fixed.h"
#include "fichabc.h"
#include "listlinasiento1view.h"


class Empresa;


/// Clase Asiento1.
/** */
class Asiento1 : public FichaBc {
    Q_OBJECT

public:
    enum estadoasiento {ASVacio = 0, ASAbierto = 1, ASCerrado = 2};

protected:
    ListLinAsiento1View *listalineas;

public:
    Asiento1(Empresa *, QWidget *parent);
    virtual ~Asiento1();
    Empresa *companyact();
    int cargar(QString);
    Fixed totaldebe(QString);
    Fixed totalhaber(QString);
    void vaciar();
    void abrir();
    void cerrar();
    estadoasiento estadoAsiento1();
    int guardar();
    void setidasiento(QString val);
    QString idasiento();
    /// Establece cual es la lista subformulario del presupuesto.
    void setListLinAsiento1(ListLinAsiento1View *a);
    virtual int borrar(bool);
    virtual void pintaidasiento(QString);
    virtual void pintadescripcion(QString);
    virtual void pintafecha(QString);
    virtual void pintacomentariosasiento(QString);
    virtual void pintaordenasiento(QString);
    virtual void pintaclase(QString);
    virtual void calculaypintatotales(QString);
    virtual void pintar();
    virtual void trataestadoAsiento1();
    virtual void asiento_cierre();
    virtual void asiento_apertura();
    virtual void asiento_regularizacion(QString, QString);
};

#endif

