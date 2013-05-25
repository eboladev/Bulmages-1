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

#ifndef ARTGRAFICOSDB_H
#define ARTGRAFICOSDB_H

#include <QtXml/QDomDocument>
#include <QtWidgets/QTableWidget>
#include <QtCore/QSignalMapper>

#include "blfunctions.h"
#include "blwidget.h"
#include "ui_artgraficosdbbase.h"
#include "btcompany.h"
#include "bldockwidget.h"

// Clase que contiene la informacion necesaria de un articulo para la rejilla
class NodoArticulo
{

public:
    NodoArticulo();
    ~NodoArticulo();

public:
    QString m_codigoarticulo;
    QString m_nombrearticulo;
    bool m_etiquetavisible;
    QString m_etiquetaarticulo;
    QString m_colortpvarticulo;
};

// Clase que contiene la informacion necesario sobre la familia y su lista
// de articulos correspondientes
class FamiliaArticulos
{

public:
    FamiliaArticulos();
    ~FamiliaArticulos();

public:
    QString m_nombrefamilia;
    QString m_idfamilia;
    QList<NodoArticulo> m_listaarticulos;
};

// Clase que guarda la informacion de un articulo en pantalla
// Por cada elemento de la rejilla que contenga un articulo
// su informacion se guarda en esta clase (su codigo y su imagen, por ahora)
class BtLabel : public QLabel
{
Q_OBJECT
public:
    BtLabel();
    ~BtLabel();
    void mousePressEvent ( QMouseEvent * e );
public:
    QString m_codigoarticulo[100][100];      /// Como maximo 100 x 100 articulos es más que suficiente
signals:
    void cellPressed(int, int);
};

class ArtGraficosDb : public BlWidget, public Ui_ArtGraficosDbBase
{
    Q_OBJECT

public:
    int m_numPantallas;
    int m_pantallaActual;
    QList<FamiliaArticulos> m_listfamilias;
    QList<QWidget *> m_pantallas;
    QString m_padreFamiliaAnterior;
    //QString m_firstIdFamilia;
    QTableWidget *mui_list;
    QWidget *m_widget;

public:
    ArtGraficosDb ( BlMainCompany *emp, QWidget *parent );
    virtual ~ArtGraficosDb();
    void muestraPantalla ( int );
    void renderPantallas();
    void ponPantallas();
    void ponListaPantallas(int familiaMostrar = 0);
    
private:
     QSignalMapper *m_signalMapperCategory;
     QSignalMapper *m_signalMapperChildCategory;
    
public slots:
    virtual void on_mui_botonSiguiente_pressed();
    virtual void on_mui_botonAnterior_pressed();
    virtual void cellClicked ( int, int );
    virtual void categorySelected_pressed(const QString &idFamilia = "0");
    virtual void homeButton_clicked();
    virtual void previousButton_clicked();
    virtual void categoryChanged_clicked(const QString &idFamilia = "0");
};

#endif

