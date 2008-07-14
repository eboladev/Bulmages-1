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

#ifndef TICKET_H
#define TICKET_H

#include <QWidget>
#include <QCloseEvent>
#include <QLabel>

#include "funcaux.h"
#include "blwidget.h"
#include "dbrecord.h"
#include "fixed.h"

class Ticket :  public BLWidget, public DBRecord
{
    Q_OBJECT
private:
    QList<DBRecord *> *m_listaLineas;
    DBRecord *m_lineaActual;

public:
    virtual void pintar();

public:
    Ticket ( EmpresaBase *emp = NULL, QWidget *parent = 0 );
    virtual ~Ticket();
    DBRecord *agregarLinea();

    QList<DBRecord *> *listaLineas();
    /// Inserta o agrega cantidad de articulos al ticket
    /// El parametro nuevalinea indica que se inserte en nueva linea.
    DBRecord * insertarArticulo ( QString idArticulo, Fixed cantidad = Fixed ( "1" ), bool nuevaLinea = FALSE );
    void borrarArticulo ( DBRecord *linea, Fixed cantidad = Fixed ( "1" ) );
    void vaciarTicket();
    void subirPosArticulo ( DBRecord *linea, int filas = 1 );
    void bajarPosArticulo ( DBRecord *linea, int filas = 1 );
    void inicioPosTicket ( DBRecord * );
    void finPosTicket ( DBRecord * );
    DBRecord *lineaTicket ( int posicion );
    DBRecord *lineaActTicket();
    void setLineaActual ( DBRecord * );
    void setDescuentoGlobal ( Fixed descuento );
    virtual void imprimir();
    virtual void abrircajon();
    int guardar();
    int cargar ( QString );
    void borrarLinea ( DBRecord* linea );

public slots:
    virtual void subir();
    virtual void bajar();
    virtual void agregarCantidad ( QString cantidad );
    virtual void ponerCantidad ( QString cantidad );
    virtual void ponerPrecio ( QString precio );
    virtual void insertarArticuloCodigo ( QString );
    virtual void insertarArticuloCodigoNL ( QString );
};

#endif

