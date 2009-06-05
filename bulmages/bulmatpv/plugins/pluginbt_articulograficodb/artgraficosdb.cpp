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

#include <QTableWidget>
#include <QLabel>
#include <QHeaderView>
#include <QFile>
#include <QDomDocument>
#include <QPicture>
#include <QPainter>
#include <QPushButton>

#include "artgraficosdb.h"
#include "blconfiguration.h"
#include "bldockwidget.h"

extern BlDockWidget *g_pantallas;

NodoArticulo::NodoArticulo() {}
NodoArticulo::~NodoArticulo() {}

FamiliaArticulos::FamiliaArticulos() {}
FamiliaArticulos::~FamiliaArticulos() {}

ArtGraficosDb::ArtGraficosDb ( BlMainCompany *emp, QWidget *parent ) : BlWidget ( emp, parent )
{
    _depura ( "ArtGraficosDb::ArtGraficosDb", 0 );
    
    setupUi ( this );
    setObjectName ( "ArtGraficosDb" );
    
    m_numPantallas = 0;
    m_pantallaActual = 0;
    mui_list->horizontalHeader()->hide();
    mui_list->verticalHeader()->hide();
    
    // Crea las pantallas en funcion de las familias de la BD
    ponPantallas();
    // Muestra la primera pantalla creada
    muestraPantalla ( 0 );
    
    _depura ( "END ArtGraficosDb::ArtGraficosDb", 0 );
}

ArtGraficosDb::~ArtGraficosDb()
{
    _depura ( "ArtGraficosDb::~ArtGraficosDb", 0 );
    _depura ( "END ArtGraficosDb::~ArtGraficosDb", 0 );
}

void ArtGraficosDb::on_mui_list_cellClicked ( int row, int column )
{
    _depura ( "ArtGraficosDb::on_mui_list_cellClicked", 0 );

    QString artvarios = g_confpr->valor ( CONF_ARTICULOS_VARIOS );
    QString codigo = m_articulos[row][column];
    
    if ( ! artvarios.contains ( codigo ) ) {
        ( ( BtCompany * ) mainCompany() )->ticketActual()->insertarArticuloCodigo ( m_articulos[row][column] );
    } else {
        ( ( BtCompany * ) mainCompany() )->ticketActual()->insertarArticuloCodigoNL ( m_articulos[row][column] );
    } // end if
    
    _depura ( "END ArtGraficosDb::on_mui_list_cellClicked", 0 );
}

void ArtGraficosDb::on_mui_botonSiguiente_released()
{
    _depura ( "ArtGraficosDb::on_mui_botonSiguiente_released", 0 );

    muestraPantalla ( m_pantallaActual + 1 );
    
    _depura ( "END ArtGraficosDb::on_mui_botonSiguiente_released", 0 );
}

void ArtGraficosDb::on_mui_botonAnterior_released()
{
    _depura ( "ArtGraficosDb::on_mui_botonAnterior_released", 0 );
    
    muestraPantalla ( m_pantallaActual - 1 );
    
    _depura ( "END ArtGraficosDb::on_mui_botonAnterior_released", 0 );
}

void ArtGraficosDb::muestraPantalla ( int numpantalla )
{
    _depura ( "ArtGraficosDb::muestraPantalla", 0 );

    m_pantallaActual = numpantalla;
    
    QDomElement docElem = m_doc.documentElement();
    QDomElement pantalla = docElem.firstChildElement("PANTALLA");

    //m_numPantallas = docElem.elementsByTagName ( "PANTALLA" ).count();
    m_numPantallas = m_listfamilias.count();

    /// Simulamos un sistema ciclico.
    if ( m_pantallaActual > m_numPantallas - 1 ) m_pantallaActual = 0;
    if ( m_pantallaActual < 0 ) m_pantallaActual = m_numPantallas - 1;
    
    // Obtenemos datos de la familia de esta pantalla
    FamiliaArticulos fa = m_listfamilias.value(m_pantallaActual);
    
//     mensajeInfo("Estamos en la pantalla " + QString::number(m_pantallaActual) + " y hay " + QString::number(m_numPantallas) + " pantallas");

//     QDomElement pantalla = docElem.elementsByTagName ( "PANTALLA" ).at ( m_pantallaActual ).toElement();

    //QString grid = pantalla.firstChildElement ( "GRID" ).toElement().text();
    QString grid = "8";

    //QString cellwidth = pantalla.firstChildElement ( "CELLWIDTH" ).toElement().text();
    QString cellwidth = "75";

    /// Cogemos el titulo de la pantalla
//     QString titulo = m_listfamilias.value(m_pantallaActual);
    QString titulo = fa.m_nombrefamilia;
    mui_titulo->setText ( titulo );

    // Hacemos una parrilla cuadrada
    mui_list->clear();
    mui_list->setRowCount ( grid.toInt() );
    mui_list->setColumnCount ( grid.toInt() );

    // Recorremos las filas para configurar el alto
    for ( int row = 0; row < grid.toInt(); row++ )
        mui_list->setRowHeight ( row, cellwidth.toInt() );
        
    // Recorremos las columnas para configurar el ancho
    for ( int column  = 0; column < grid.toInt(); column++ )
        mui_list->setColumnWidth ( column, cellwidth.toInt() );

    /// Tratamos cada ventana
    QWidget *activewindow = NULL;
    QDomNodeList nodos = pantalla.elementsByTagName ( "ITEM" );
    
    int nitem = 0;
    int numarticulos = fa.m_listaarticulos.count();
    NodoArticulo na;

    for ( int row = 0; row < grid.toInt(); row++ ) {
    
        for ( int column  = 0; column < grid.toInt(); column++ ) {

            //if ( nitem < nodos.count() ) {
            if ( nitem < numarticulos ) {
            
                // Obtenemos el articulo en la posicion nitem
                na = fa.m_listaarticulos.value(nitem);
            
                QDomNode ventana = nodos.item ( nitem );

                //QString nombre = ventana.firstChildElement ( "TITULO" ).toElement().text();
                QString nombre = na.m_nombrearticulo;
                QString codigo = na.m_codigoarticulo;
                
                /*
                QDomElement e1 = ventana.toElement(); /// try to convert the node to an element.
                QString text = e1.text();

                QString codigo = ventana.firstChildElement ( "CODIGO" ).toElement().text();
                if ( codigo.isEmpty() ) {
                    codigo = text;
                } // end if
                */

                /// Creamos el elemento y lo ponemos en la tabla.
                QLabel *lab = new QLabel ( NULL );
                /// El escalado completo tarda demasiado.
//                lab->setPixmap(QPixmap("/var/bulmages/articles/"+text+".jpg", "no", Qt::OrderedDither | Qt::OrderedAlphaDither | Qt::AvoidDither).scaled(cellwidth.toInt(),cellwidth.toInt()));
                /// El escalado lateral tambien tarda demasiado
//                lab->setPixmap(QPixmap("/var/bulmages/articles/"+text+".jpg").scaledToWidth(cellwidth.toInt()));
                /// Ponemos las imagenes sin escalado.
//                lab->setPixmap ( QPixmap ( g_confpr->valor ( CONF_DIR_THUMB_ARTICLES ) + text + ".jpg" ) );

                // Probamos con una Picture
                QPicture picture;
                QPainter painter;
                painter.begin ( &picture );        // paint in picture
                
//  painter.drawPixmap(0,0,cellwidth.toInt(),cellwidth.toInt(),QPixmap ( g_confpr->valor ( CONF_DIR_THUMB_ARTICLES ) + codigo + ".jpg" ));

                painter.drawPixmap ( 0, 0, cellwidth.toInt(), cellwidth.toInt(), QPixmap ( g_confpr->valor ( CONF_DIR_THUMB_ARTICLES ) + "blanco.jpg" ) );

                painter.drawPixmap ( 0, 0, cellwidth.toInt(), cellwidth.toInt() - 25, QPixmap ( g_confpr->valor ( CONF_DIR_THUMB_ARTICLES ) + codigo + ".jpg" ) );

//         painter.drawEllipse(10,20, 80,70); // draw an ellipse

                painter.setPen ( QColor ( 0, 0, 0 ) );
                painter.setBackground ( QColor ( 0, 0, 0 ) );

                painter.drawText ( 5, 95, nombre );
                
                //  painter.setPen(QColor(0,25,0));
                //  painter.drawText(5,10,nombre);
                //  painter.drawText(5,15,nombre);
                //  painter.drawText(5,5,codigo);
                //  painter.drawText(5,10,cellwidth);

                painter.end();                     // painting done

                lab->setPicture ( picture );

                mui_list->setCellWidget ( row, column, lab );
                m_articulos[row][column] = codigo;
                nitem++;
            
            } // end if

        } // end for
        
    } // end for

    _depura ( "END ArtGraficosDb::muestraPantalla", 0 );
}

void ArtGraficosDb::ponPantallas()
{
    _depura ( "ArtGraficosDb::ponPantallas", 0 );

    /// Creo el Widget que estara ubicado en el dockwidget que se ha creado en pluginbt_articulograficodb.cpp
    QWidget *widget = new QWidget;
    QVBoxLayout *hboxLayout1 = new QVBoxLayout;
    hboxLayout1->setSpacing ( 5 );
    hboxLayout1->setMargin ( 5 );
    hboxLayout1->setObjectName ( QString::fromUtf8 ( "hboxLayout1" ) );
        
    BlDbRecordSet *familias;
    familias = mainCompany()->loadQuery ( "SELECT idfamilia, nombrefamilia FROM familia ORDER BY nombrefamilia" );
    
    int i = 0;
    
    FamiliaArticulos fa;
    
    while ( !familias->eof() ) {
    
        QString titulo = familias->valor("nombrefamilia");
        QPushButton *pb = new QPushButton ( titulo, g_pantallas );
        
        pb->setText ( titulo );
        //pb->setObjectName ( QString::fromUtf8 ("familia_pb_") + QString::number ( i ) );
        pb->setObjectName ( QString::number ( i ) );
        
        /// Hago la conexion del pulsado con el metodo pulsadoBoton para que se cambie la pantalla.
        connect ( pb, SIGNAL ( pressed() ), this, SLOT ( pulsadoBoton() ) );
        hboxLayout1->addWidget ( pb );
        
        fa.m_nombrefamilia = familias->valor("nombrefamilia");
        fa.m_idfamilia = familias->valor("idfamilia");
        
        // Llenamos la lista de articulos
        BlDbRecordSet *articulos;
        articulos = mainCompany()->loadQuery ( "SELECT codigocompletoarticulo, nomarticulo FROM articulo WHERE idfamilia = " + fa.m_idfamilia + " ORDER BY nomarticulo" );
        
        while ( !articulos->eof() ) {
        
            NodoArticulo na;
            na.m_codigoarticulo = articulos->valor("codigocompletoarticulo");
            na.m_nombrearticulo = articulos->valor("nomarticulo");
            fa.m_listaarticulos.append(na);
            articulos->nextRecord();
        
        }
        
        m_listfamilias.append(fa);
        fa.m_listaarticulos.clear();
        familias->nextRecord();
        i++;
        
        delete articulos;
    
    } // end while
    
    delete familias;
    
    /// Agrego el widget al BLDockWidget
    widget->setLayout ( hboxLayout1 );
    g_pantallas->setWidget ( widget );
    
    _depura ( "END ArtGraficosDb::ponPantallas", 0 );
}

void ArtGraficosDb::pulsadoBoton()
{
    _depura ( "ArtGraficosDb::pulsadoBoton", 0 );
    
    muestraPantalla ( sender()->objectName().toInt() );
    
    _depura ( "END ArtGraficosDb::pulsadoBoton", 0 );
}
