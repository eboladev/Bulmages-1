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

BtLabel::BtLabel() {}
BtLabel::~BtLabel() {}


ArtGraficosDb::ArtGraficosDb ( BlMainCompany *emp, QWidget *parent ) : BlWidget ( emp, parent ) {
    _depura ( "ArtGraficosDb::ArtGraficosDb", 0 );
    
    setupUi ( this );
    setObjectName ( "ArtGraficosDb" );
    
    m_numPantallas = 0;
    m_pantallaActual = 0;
    
    /// Inicializa la variable.
    mui_list = new QTableWidget();

    // Crea las pantallas en funcion de las familias de la BD
    ponPantallas();
    
    // Renderiza las pantallas y las almacena para mostrar cuando sea necesario.
    renderPantallas ();
    // Muestra la primera pantalla creada
    muestraPantalla ( 0 );

    _depura ( "END ArtGraficosDb::ArtGraficosDb", 0 );
}

ArtGraficosDb::~ArtGraficosDb() {
    _depura ( "ArtGraficosDb::~ArtGraficosDb", 0 );
    
    /// Libera la memoria de las pantallas.
    for (int i = 0; i < m_pantallas.count(); i++) {
	QTableWidget *pantalla = m_pantallas.at(i);
	delete pantalla;
    } // end for    
    
    _depura ( "END ArtGraficosDb::~ArtGraficosDb", 0 );
}

void ArtGraficosDb::cellClicked ( int row, int column )
{
    _depura ( "ArtGraficosDb::cellClicked", 0 );

    // Obtenemos el item de la celda
    BtLabel *label = (BtLabel *) mui_list->cellWidget(row, column);
  
    // Celda vacia. No hacemos nada.
    if (label == NULL) { return; }

    QString artvarios = g_confpr->valor ( CONF_ARTICULOS_VARIOS );
   
    if ( ! artvarios.contains ( label->m_codigoarticulo ) ) {
        ( ( BtCompany * ) mainCompany() )->ticketActual()->insertarArticuloCodigo ( label->m_codigoarticulo );
    } else {
        ( ( BtCompany * ) mainCompany() )->ticketActual()->insertarArticuloCodigoNL ( label->m_codigoarticulo );
    } // end if



    _depura ( "END ArtGraficosDb::cellClicked", 0 );
}

void ArtGraficosDb::on_mui_botonSiguiente_pressed()
{
    _depura ( "ArtGraficosDb::on_mui_botonSiguiente_pressed", 0 );



    muestraPantalla ( m_pantallaActual + 1 );

    setUpdatesEnabled(true);
    repaint();
    setUpdatesEnabled(false);

    
    _depura ( "END ArtGraficosDb::on_mui_botonSiguiente_pressed", 0 );
}

void ArtGraficosDb::on_mui_botonAnterior_pressed()
{
    _depura ( "ArtGraficosDb::on_mui_botonAnterior_pressed", 0 );


    
    muestraPantalla ( m_pantallaActual - 1 ); 

    setUpdatesEnabled(true);
    repaint();
    setUpdatesEnabled(false);

    _depura ( "END ArtGraficosDb::on_mui_botonAnterior_pressed", 0 );
}


void ArtGraficosDb::muestraPantalla ( int numPantalla ) {
    _depura ( "ArtGraficosDb::muestraPantalla", 0 );


    /// Hace un bucle sinfin de pantallas.
    if (numPantalla < 0) {
	/// Muestra la ultima pantalla.
	numPantalla = m_numPantallas - 1;
    } else if (numPantalla == m_numPantallas) {
	/// Muestra la primera pantalla.
	numPantalla = 0;
    } // end if
  


    m_pantallaActual = numPantalla;
    QTableWidget *pantalla = m_pantallas.at(numPantalla);

/*    mui_render->takeWidget()->hide();
    pantalla->show();
    mui_render->setWidget(pantalla);
//if (mui_list) mui_list->hide();
//    pantalla->showMaximized();
//    mui_render->setWidgetResizable(TRUE);
//    pantalla->resize();
//    pantalla->show();
*/
    mui_stack->setCurrentWidget(pantalla);
    mui_list = pantalla;
    mui_titulo->setText( pantalla->accessibleName() );


    _depura ( "END ArtGraficosDb::muestraPantalla", 0 );
}


void ArtGraficosDb::renderPantallas ()
{
    _depura ( "ArtGraficosDb::renderPantallas", 0 );

    m_numPantallas = m_listfamilias.count();

    /// Celdas por fila
    QString grid = g_confpr->valor ( CONF_TPV_CELLS_PER_ROW );

    /// Ancho y alto de la celda en pixeles
    QString cellwidth = g_confpr->valor ( CONF_TPV_CELL_WIDTH );

    /// Recorremos todas las familias para crear todas las pantallas.
    for (int i = 0; i < m_numPantallas; i++) {
    
	/// Creamos una pantalla nueva
	QTableWidget *pantalla = new QTableWidget(mui_stack);
	connect(pantalla, SIGNAL(cellClicked ( int, int )), this,  SLOT(cellClicked ( int, int )));
	
	pantalla->setHorizontalScrollMode(QAbstractItemView::ScrollPerPixel);
	pantalla->setVerticalScrollMode(QAbstractItemView::ScrollPerPixel);

	FamiliaArticulos familia = m_listfamilias.value(i);
	pantalla->setAccessibleName(familia.m_nombrefamilia);
	
	pantalla->horizontalHeader()->hide();
	pantalla->verticalHeader()->hide();

        /// Calcula el numero de filas y columnas que tiene que tener la parrilla en pantalla
	int numarticulos = familia.m_listaarticulos.count();
	int numrows = numarticulos / grid.toInt();
	if ( numarticulos % grid.toInt() !=0 ) numrows++;

	pantalla->clear();
	pantalla->setRowCount ( numrows );
	pantalla->setColumnCount ( grid.toInt() );

	int numcells = grid.toInt() * numrows;
	
	/// Recorremos las filas para configurar el alto
	for ( int row = 0; row < numrows; row++ )
	    pantalla->setRowHeight ( row, cellwidth.toInt() );
        
	/// Recorremos las columnas para configurar el ancho
	for ( int column  = 0; column < grid.toInt(); column++ )
	    pantalla->setColumnWidth ( column, cellwidth.toInt() );
	
	int nitem = 0;
	NodoArticulo na;

	for ( int row = 0; row < numrows; row++ ) {
	    for ( int column = 0; column < grid.toInt(); column++ ) {
		if ( nitem < numcells ) {
		    // Obtenemos el articulo en la posicion nitem
		    na = familia.m_listaarticulos.value(nitem);
		
		    QString nombre = na.m_nombrearticulo;
		    QString codigo = na.m_codigoarticulo;
		    
		    /// Creamos el elemento y lo ponemos en la tabla.
		    BtLabel *lab = new BtLabel();
		    lab->m_codigoarticulo = codigo;
		    
		    // Probamos con una Picture
		    QPicture picture;
		    QPainter painter;
		    painter.begin ( &picture ); // paint in picture
		    

          QFile f(g_confpr->valor ( CONF_DIR_THUMB_ARTICLES ) + codigo + ".jpg");

		    // Si existe la imagen del articulo, esta se superpondra a la de blanco.jpg, dejando 20px por debajo
		    // para que se vea el texto negro sobre blanco
          if (f.exists()) {
            /// Es importante pasar el pixmap ya escalado porque sino en cada renderizado se reescala de nuevo el pixmap.
            QPixmap p= QPixmap( g_confpr->valor ( CONF_DIR_THUMB_ARTICLES ) + codigo + ".jpg" ).scaled(cellwidth.toInt(),cellwidth.toInt()-20);
            painter.drawPixmap ( 0, 0, p );
          } else {
            painter.setPen ( QColor ( 0, 0, 0 ) );
            painter.setBackground ( QColor ( 0, 0, 0 ) );
            painter.drawText ( 5, (g_confpr->valor ( CONF_TPV_CELL_WIDTH ).toInt() - 25), nombre );
          } // end if
		    
		    painter.end(); // painting done

		    lab->setPicture ( picture );

		    pantalla->setCellWidget ( row, column, lab );

		    nitem++;
		
		} // end if
	    } // end for
	} // end for

	/// Ya tenemos la pantalla creada y dibujada y ahora la guardamos en el QList.
    m_pantallas.append(pantalla);
    mui_stack->addWidget(pantalla);

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
    
    setUpdatesEnabled(true);
    repaint();
    setUpdatesEnabled(false);
    

    _depura ( "END ArtGraficosDb::pulsadoBoton", 0 );
}
