/***************************************************************************
 *   Copyright (C) 2007 by Tomeu Borras Riera                              *
 *   tborras@conetxia.com                                                  *
 *   http://www.iglues.org                                                 *
 *                                                                         *
 *   Copyright (C) 2009 by Arturo Martin Llado                             *
 *   amartin@conetxia.com                                                  *
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

#ifndef BTTICKET_H
#define BTTICKET_H

#include <QWidget>
#include <QCloseEvent>
#include <QLabel>
#include <QTime>

#include "blfunctions.h"
#include "blwidget.h"
#include "bldb.h"
#include "blfixed.h"


class BT_EXPORT BtTicket : public BlWidget, public BlDbRecord, public QTime
{
    Q_OBJECT

private:
    QList<BlDbRecord *> *m_listaLineas;
    BlDbRecord *m_lineaActual;
    QString m_nomTicketDefecto;
    /// Si esta en 'true' la variable 'nuevaLinea' del metodo  'insertarArticulo' se establece en true siempre.
    bool m_nextLineIsInsert;

public:
    QString m_textoXML; /* Usado al generar el XML del ticket para que los plugins puedan alterar el texto y agregar sus partes*/

public:
    void setNextLineIsInsert(bool nextLineIsTrue);
    bool nextLineIsInsert();
    virtual void pintar();
    BtTicket ( BlMainCompany *emp = NULL, QWidget *parent = 0 );
    virtual ~BtTicket();
    QString nomTicketDefecto();
    BlDbRecord *agregarLinea();
    QList<BlDbRecord *> *listaLineas();
    /// Inserta o agrega cantidad de articulos al ticket
    /// El parametro nuevalinea indica que se inserte en nueva linea.
    BlDbRecord * insertarArticulo ( QString idArticulo, BlFixed cantidad = BlFixed ( "1" ), bool nuevaLinea = FALSE );
    void borrarArticulo ( BlDbRecord *linea, BlFixed cantidad = BlFixed ( "1" ) );
    void vaciarBtTicket();
    void subirPosArticulo ( BlDbRecord *linea, int filas = 1 );
    void bajarPosArticulo ( BlDbRecord *linea, int filas = 1 );
    void inicioPosBtTicket ( BlDbRecord * );
    void finPosBtTicket ( BlDbRecord * );
    BlDbRecord *lineaBtTicket ( int posicion );
    BlDbRecord *lineaActBtTicket();
    void setLineaActual ( BlDbRecord * );
    void setDescuentoGlobal ( BlFixed descuento );
    virtual void imprimir(bool doSave = TRUE);
    virtual void abrircajon();
    int save();
    int load ( QString );
    void borrarLinea ( BlDbRecord *linea );
    virtual QString exportXML();
    virtual bool syncXML(const QString &text, bool insertarSiempre = FALSE);
    void agregarLog(const QString &log);
    
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
