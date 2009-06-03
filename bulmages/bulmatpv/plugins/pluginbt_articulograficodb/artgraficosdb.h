/***************************************************************************
 *   Copyright (C) 2009 by Arturo Martin Llado                             *
 *   amartin@conetxia.com                                                  *
 *                                                                         *
 *   Copyright (C) 2009 by Tomeu Borras Riera                              *
 *   tborras@conetxia.com                                                  *
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

#ifndef ARTGRAFICOS_H
#define ARTGRAFICOS_H

#include <QDomDocument>
#include <QTableWidget>

#include "blfunctions.h"
#include "blwidget.h"
#include "ui_artgraficosdbbase.h"
#include "btcompany.h"

class ArtGraficosDb : public BlWidget, public Ui_ArtGraficosDbBase
{
    Q_OBJECT

public:
    QString m_articulos[100][100];
    QDomDocument m_doc;
    int m_numPantallas;
    int m_pantallaActual;

public:
    ArtGraficosDb ( BlMainCompany *emp, QWidget *parent );
    virtual ~ArtGraficosDb();
    void cargaXML ( QString );
    void muestraPantalla ( int );
    void ponPantallas();

public slots:
    virtual void on_mui_list_cellClicked ( int, int );
    virtual void on_mui_botonSiguiente_released();
    virtual void on_mui_botonAnterior_released();
    virtual void pulsadoBoton();
};

#endif

