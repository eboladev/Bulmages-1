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
#include <QScrollArea>
#include <QScrollBar>
#include <QSizePolicy>
#include <QList>

#include "artgraficosdb.h"
#include "blconfiguration.h"
#include "bldockwidget.h"
#include "btwindowsstyle.h"


extern BlDockWidget *g_pantallas;

NodoArticulo::NodoArticulo() {}
NodoArticulo::~NodoArticulo() {}

FamiliaArticulos::FamiliaArticulos() {}
FamiliaArticulos::~FamiliaArticulos() {}

BtLabel::BtLabel() {}
BtLabel::~BtLabel() {}
void BtLabel::mousePressEvent ( QMouseEvent * e ) {
     if (e->button() == Qt::LeftButton) {
         int cellwidth = g_confpr->value(CONF_TPV_CELL_WIDTH ).toInt();
         /// Evitamos un posible SegFault por division por 0.
         if (cellwidth == 0) cellwidth = 1;
//         blMsgInfo(QString::number(e->x()));
         emit (cellPressed(e->x() / cellwidth, e->y() / cellwidth));
         e->accept();
     }// end if
}


ArtGraficosDb::ArtGraficosDb ( BlMainCompany *emp, QWidget *parent ) : BlWidget ( emp, parent )
{
    BL_FUNC_DEBUG

    setupUi ( this );
    setObjectName ( "ArtGraficosDb" );

    m_numPantallas = 0;
    m_pantallaActual = 0;
    m_paginadoListaPantallas = (g_confpr->value(CONF_TPV_CATEGORY_TREE) == "TRUE") ? true : false;
    
    m_widget = 0;
    m_signalMapperCategory = 0;
    m_signalMapperChildCategory = 0;
 
    m_padreFamiliaAnterior = "0";
    
    /// Vaciamos el stack de widgets
    while (mui_stack->currentWidget()) mui_stack->removeWidget(mui_stack->currentWidget());

	
    // Crea las pantallas en funcion de las familias de la BD
    ponPantallas();

    // Renderiza las pantallas y las almacena para mostrar cuando sea necesario.
    renderPantallas ();

    ponListaPantallas();
    // Muestra la primera pantalla creada
    //muestraPantalla ( 0 );

    
}


ArtGraficosDb::~ArtGraficosDb()
{
    BL_FUNC_DEBUG

    /// Libera la memoria de las pantallas.
    //limpiaListaPantallas();
    
    for ( int i = 0; i < m_pantallas.count(); i++ ) {
        QWidget *pantalla = m_pantallas.at ( i );
        delete pantalla;
    } // end for

    
}


void ArtGraficosDb::cellClicked ( int row, int column )
{
    BL_FUNC_DEBUG

    // Obtenemos el item de la celda
    BtLabel *label = ( BtLabel * ) ( (QScrollArea *) mui_stack->currentWidget())->widget();

    // Celda vacia. No hacemos nada.
    if ( label == NULL ) {
	
        return;
    }

    QString artvarios = g_confpr->value( CONF_ARTICULOS_VARIOS );

    QString codigo = label->m_codigoarticulo[row][column];


    if ( ! artvarios.contains ( codigo ) ) {
        ( ( BtCompany * ) mainCompany() )->ticketActual()->insertarArticuloCodigo ( codigo );
    } else {
        ( ( BtCompany * ) mainCompany() )->ticketActual()->insertarArticuloCodigoNL ( codigo );
    } // end if

    
}


void ArtGraficosDb::on_mui_botonSiguiente_pressed()
{
    BL_FUNC_DEBUG



    muestraPantalla ( m_pantallaActual + 1 );

/*
    setUpdatesEnabled ( true );
    repaint();
    setUpdatesEnabled ( false );
*/

    
}


void ArtGraficosDb::on_mui_botonAnterior_pressed()
{
    BL_FUNC_DEBUG

    muestraPantalla ( m_pantallaActual - 1 );

/*
    setUpdatesEnabled ( true );
    repaint();
    setUpdatesEnabled ( false );
*/
    
}


void ArtGraficosDb::muestraPantalla ( int numPantalla )
{
    BL_FUNC_DEBUG

    /// Si no hay pantallas que mostrar, sale.
    if (m_numPantallas == 0) return;

    /// Hace un bucle sinfin de pantallas.
    if ( numPantalla < 0 ) {
        /// Muestra la ultima pantalla.
        numPantalla = m_numPantallas - 1;
    } else if ( numPantalla == m_numPantallas ) {
        /// Muestra la primera pantalla.
        numPantalla = 0;
    } // end if

    m_pantallaActual = numPantalla;
    QWidget *pantalla = m_pantallas.at ( numPantalla );

    mui_stack->setCurrentIndex ( numPantalla );
    mui_titulo->setText ( pantalla->accessibleName() );

    
}


void ArtGraficosDb::renderPantallas ()
{
    BL_FUNC_DEBUG

    m_numPantallas = m_listfamilias.count();

    /// Celdas por fila
    QString grid = g_confpr->value ( CONF_TPV_CELLS_PER_ROW );
	if (grid == "") grid = "8";

    /// Ancho y alto de la celda en pixeles
    QString cellwidth = g_confpr->value ( CONF_TPV_CELL_WIDTH );
	if (cellwidth == "") cellwidth= "100";

    /// Recorremos todas las familias para crear todas las pantallas.
    for ( int i = 0; i < m_numPantallas; i++ ) {

        /// Creamos una pantalla nueva
// QTableWidget *pantalla = new QTableWidget(mui_stack);
//  connect(pantalla, SIGNAL(cellPressed ( int, int )), this,  SLOT(cellClicked ( int, int )));


// pantalla->setHorizontalScrollMode(QAbstractItemView::ScrollPerPixel);
// pantalla->setVerticalScrollMode(QAbstractItemView::ScrollPerPixel);


		FamiliaArticulos familia = m_listfamilias.value ( i );

// pantalla->horizontalHeader()->hide();
// pantalla->verticalHeader()->hide();

        /// Calcula el numero de filas y columnas que tiene que tener la parrilla en pantalla
        int numarticulos = familia.m_listaarticulos.count();
		
        int numcols = grid.toInt();
        int numrows = numarticulos / grid.toInt();
        if ( numarticulos % grid.toInt() != 0 ) numrows++;

        QScrollArea *scroll = new QScrollArea();
        scroll->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
        
        
        BtLabel *lab = new BtLabel();
        QPicture *picture = new QPicture();
        lab->setFixedSize ( cellwidth.toInt() * (numcols), cellwidth.toInt() * (numrows +1) );
        lab->setAlignment ( Qt::AlignTop );
        lab->setAccessibleName ( familia.m_nombrefamilia );
        connect(lab, SIGNAL(cellPressed ( int, int )), this,  SLOT(cellClicked ( int, int )));
// pantalla->clear();
// pantalla->setRowCount ( numrows );
// pantalla->setColumnCount ( grid.toInt() );

        int numcells = grid.toInt() * numrows;

        /// Recorremos las filas para configurar el alto
// for ( int row = 0; row < numrows; row++ )
//     pantalla->setRowHeight ( row, cellwidth.toInt() );

        /// Recorremos las columnas para configurar el ancho
// for ( int column  = 0; column < grid.toInt(); column++ )
//     pantalla->setColumnWidth ( column, cellwidth.toInt() );

        int nitem = 0;
        NodoArticulo na;
        QPainter painter;
        painter.begin ( picture ); /// paint in picture

        painter.setPen ( QColor ( 0, 0, 0 ) );
        painter.setBackground ( QColor ( 0, 0, 0 ) );

        for ( int row = 0; row < numrows; row++ ) {
          for ( int column = 0; column < numcols; column++ ) {
                if ( nitem < numcells ) {
                    /// Obtenemos el articulo en la posicion nitem
                    na = familia.m_listaarticulos.value ( nitem );

					/// Mira si existe articulo para esa posicion.
					if (nitem < familia.m_listaarticulos.size()) {
					  
					  QString nombre = na.m_nombrearticulo;
					  QString codigo = na.m_codigoarticulo;

					  /// Creamos el elemento y lo ponemos en la tabla.

					  lab->m_codigoarticulo[column][row] = codigo;
					  
					  // Probamos con una Picture
					  
					  QFile f ( g_confpr->value ( CONF_DIR_THUMB_ARTICLES ) + codigo + ".jpg" );

					  // Si existe la imagen del articulo, esta se superpondra a la de blanco.jpg, dejando 20px por debajo
					  // para que se vea el texto negro sobre blanco
					  if ( f.exists() ) {
						  /// Es importante pasar el pixmap ya escalado porque sino en cada renderizado se reescala de nuevo el pixmap.
						  QPixmap p = QPixmap ( g_confpr->value ( CONF_DIR_THUMB_ARTICLES ) + codigo + ".jpg" ).scaled ( cellwidth.toInt() - 1, cellwidth.toInt() - 20 );
						  painter.drawPixmap ( cellwidth.toInt() *column, cellwidth.toInt() *row, p );
						  painter.drawText ( cellwidth.toInt() *column + 5, cellwidth.toInt() *row + ( g_confpr->value ( CONF_TPV_CELL_WIDTH ).toInt() - 5 ), nombre );
						  
					  } else {

						  QFile f1 ( g_confpr->value ( CONF_DIR_THUMB_ARTICLES ) + codigo + ".png" );
						  
						  /// Si existe la imagen del articulo, esta se superpondra a la de blanco.jpg, dejando 20px por debajo
						  /// para que se vea el texto negro sobre blanco
						  if ( f1.exists() ) {
							/// Es importante pasar el pixmap ya escalado porque sino en cada renderizado se reescala de nuevo el pixmap.
							QPixmap p = QPixmap ( g_confpr->value ( CONF_DIR_THUMB_ARTICLES ) + codigo + ".png" ).scaled ( cellwidth.toInt() - 1, cellwidth.toInt() - 20 );
							painter.drawPixmap ( cellwidth.toInt() *column, cellwidth.toInt() *row, p );
							painter.drawText ( cellwidth.toInt() *column + 5, cellwidth.toInt() *row + ( g_confpr->value ( CONF_TPV_CELL_WIDTH ).toInt() - 5 ), nombre );
							
						  } else {
							
							/// Pinta el fondo del recuadro.
							QPixmap fondoRecuadro = QPixmap( cellwidth.toInt(), cellwidth.toInt() );
							fondoRecuadro.fill(palette().color(QPalette::Window));			    
							painter.drawPixmap ( cellwidth.toInt() *column, cellwidth.toInt() *row, fondoRecuadro );
							
							/// Pinta un recuadro con el color establecido en la ficha del articulo o en su defecto con un color estandar.
							QPixmap recuadro = QPixmap( cellwidth.toInt() -1, cellwidth.toInt() - 20);
							
							QColor recuadroColor = QColor(na.m_colortpvarticulo);
							
							if ( recuadroColor.isValid() ) {
								recuadro.fill(recuadroColor);
							} else {
								recuadro.fill(Qt::gray);
							} // end if
							
							painter.drawPixmap ( cellwidth.toInt() *column, cellwidth.toInt() *row, recuadro );
							painter.drawText ( cellwidth.toInt() *column + 5, cellwidth.toInt() *row + ( g_confpr->value ( CONF_TPV_CELL_WIDTH ).toInt() - 5 ), nombre );
						  } // end if
					
					  } // end if

					  /// Si esta marcada la opcion de escribir etiqueta.
					  if (na.m_etiquetavisible) {
						painter.drawText ( cellwidth.toInt() *column + 5, (cellwidth.toInt() *row + ( g_confpr->value ( CONF_TPV_CELL_WIDTH ).toInt() - 5) / 2 ), na.m_etiquetaarticulo );
					  } // end if

                    } // end if
                    
                    nitem++;

                } // end if
            } // end for
        } // end for

//         painter.drawText ( cellwidth.toInt() * numcols/2 - 30 , cellwidth.toInt() *numrows + ( g_confpr->value ( CONF_TPV_CELL_WIDTH ).toInt() + 25 ), "Iglues/BulmaTPV" );

//         QPixmap p = QPixmap ( 100, 100  );
//         p.fill();
//         painter.drawPixmap ( 0, cellwidth.toInt() * (numrows+1), p );

        painter.end(); // painting done

        lab->setPicture ( *picture );
        scroll->setWidget(lab);
        lab->show();
	
	BtWindowsStyle *btwindowsstyle = new BtWindowsStyle();
	scroll->verticalScrollBar()->setStyle( btwindowsstyle );
	scroll->horizontalScrollBar()->setStyle( btwindowsstyle );


    /// Ya tenemos la pantalla creada y dibujada y ahora la guardamos en el QList.
        m_pantallas.append ( lab );
        mui_stack->addWidget ( scroll );
//        mui_stack->setSizePolicy( QSizePolicy() );

    } // end for
    
}


void ArtGraficosDb::ponPantallas()
{
    BL_FUNC_DEBUG
    
    BlDbRecordSet *familias;
    familias = mainCompany()->loadQuery ( "SELECT idfamilia, nombrefamilia, colortpvfamilia FROM familia WHERE visibletpvfamilia = TRUE ORDER BY ordentpvfamilia, nombrefamilia" );
    
   
    int i = 0;

    FamiliaArticulos fa;

    while ( !familias->eof() ) {
      
        fa.m_nombrefamilia = familias->value( "nombrefamilia" );
        fa.m_idfamilia = familias->value( "idfamilia" );

        /// Llenamos la lista de articulos
        BlDbRecordSet *articulos;
        articulos = mainCompany()->loadQuery ( "SELECT codigocompletoarticulo, nomarticulo, etiquetavisibletpvarticulo, abrevarticulo, colortpvarticulo FROM articulo WHERE visibletpvarticulo = TRUE AND idfamilia = " + fa.m_idfamilia + " ORDER BY ordentpvarticulo, nomarticulo" );

        while ( !articulos->eof() ) {

            NodoArticulo na;
            na.m_codigoarticulo = articulos->value( "codigocompletoarticulo" );
            na.m_nombrearticulo = articulos->value( "nomarticulo" );

	    if (articulos->value( "etiquetavisibletpvarticulo" ) == "f") {
	      na.m_etiquetavisible = FALSE;
	    } else {
	      na.m_etiquetavisible = TRUE;
	    } // end if
	    
	    na.m_etiquetaarticulo = articulos->value( "abrevarticulo" );
	    na.m_colortpvarticulo = articulos->value( "colortpvarticulo" );
            fa.m_listaarticulos.append ( na );
            articulos->nextRecord();

        } // end while

        m_listfamilias.append ( fa );
        fa.m_listaarticulos.clear();
        familias->nextRecord();
        i++;

        delete articulos;

    } // end while

    delete familias;
   
}   


void ArtGraficosDb::ponListaPantallas(int familiaMostrar)
{
    BL_FUNC_DEBUG
    
    int contadorBotones = 0;
    int columnasBotones;
    int j;
    int indice;

    /// Creo el Widget que estara ubicado en el dockwidget que se ha creado en pluginbt_articulograficodb.cpp
    if (m_widget) {
	m_widget->hide();
	delete m_widget;
    } // end if
      
    m_widget = new QWidget(this);
    m_widget->show();
    m_signalMapperCategory = new QSignalMapper(m_widget);
    m_signalMapperChildCategory = new QSignalMapper(m_widget);
    
    QList <QVBoxLayout *> listaVBoxLayout;
    
    /// Establece el numero de columnas a mostrar. Minimo 1.
    if ( g_confpr->value( CONF_TPV_CATEGORIES_COLUMNS ).toInt() > 1 ) {
      columnasBotones = g_confpr->value( CONF_TPV_CATEGORIES_COLUMNS ).toInt();
    } else {
      columnasBotones = 1;
    } // end if
   
   
    QVBoxLayout *mainLayout = new QVBoxLayout(m_widget);
    mainLayout->setObjectName ( QString::fromUtf8 ( "mainLayout" ) );

    QHBoxLayout *topLayout = new QHBoxLayout;
    topLayout->setObjectName ( QString::fromUtf8 ( "topLayout" ) );

    QHBoxLayout *buttonsLayout = new QHBoxLayout;
    buttonsLayout->setObjectName ( QString::fromUtf8 ( "buttonsLayout" ) );
    
    
    QToolButton *previousButton = new QToolButton(m_widget);
    previousButton->setText("Atras");
    previousButton->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Expanding);
    previousButton->setFixedHeight(42);

    topLayout->addWidget( previousButton );
    connect(previousButton, SIGNAL(clicked()),this, SLOT(previousButton_clicked()), Qt::QueuedConnection);
    
    QToolButton *homeButton = new QToolButton(m_widget);
    homeButton->setText("Inicio");
    homeButton->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Expanding);
    homeButton->setFixedHeight(42);
    
    topLayout->addWidget( homeButton );
    connect(homeButton, SIGNAL(clicked()),this, SLOT(homeButton_clicked()), Qt::QueuedConnection);
        

    /// Crea los QVBoxLayout necesarios para colocar los botones.
    for (j = 0; j < columnasBotones; j++) {
	QVBoxLayout *columnButtonLayout = new QVBoxLayout;
	
	columnButtonLayout->setObjectName(QString("columnButtonLayout") + QString::number(j));
	listaVBoxLayout.append ( columnButtonLayout );
	buttonsLayout->addLayout ( columnButtonLayout );
    } // end for
    
 
    BlDbRecordSet *familias;
    //familias = mainCompany()->loadQuery ( "SELECT idfamilia, nombrefamilia, colortpvfamilia FROM familia WHERE visibletpvfamilia = TRUE ORDER BY ordentpvfamilia, nombrefamilia" );
    
    if (familiaMostrar == 0) {
      
      if (m_paginadoListaPantallas) {
	
	  familias = mainCompany()->loadQuery ( "SELECT t1.idfamilia, t1.nombrefamilia, t1.colortpvfamilia, t1.padrefamilia, (SELECT count(t2.idfamilia) FROM familia AS t2 WHERE visibletpvfamilia = TRUE AND t2.padrefamilia = t1.idfamilia) AS hijos, (SELECT t3.padrefamilia FROM familia AS t3 WHERE t3.idfamilia = t1.padrefamilia) AS padreanterior FROM familia AS t1 WHERE t1.visibletpvfamilia = TRUE AND t1.padrefamilia IS NULL ORDER BY t1.ordentpvfamilia, t1.nombrefamilia" );

      } else {
	
	familias = mainCompany()->loadQuery ( "SELECT idfamilia, nombrefamilia, colortpvfamilia FROM familia WHERE visibletpvfamilia = TRUE ORDER BY ordentpvfamilia, nombrefamilia" );
	
      } //end if
	
    } else {
      
	familias = mainCompany()->loadQuery ( "SELECT t1.idfamilia, t1.nombrefamilia, t1.colortpvfamilia, t1.padrefamilia, (SELECT count(t2.idfamilia) FROM familia AS t2 WHERE visibletpvfamilia = TRUE AND t2.padrefamilia = t1.idfamilia) AS hijos, (SELECT t3.padrefamilia FROM familia AS t3 WHERE t3.idfamilia = t1.padrefamilia) AS padreanterior FROM familia AS t1 WHERE t1.visibletpvfamilia = TRUE AND t1.padrefamilia = " + QString::number(familiaMostrar) + " ORDER BY t1.ordentpvfamilia, t1.nombrefamilia" );
	
    } // end if
    
    int i = 0;

    while ( !familias->eof() ) {
      
        QString titulo = familias->value( "nombrefamilia" );
	m_padreFamiliaAnterior = familias->value( "padreanterior" );
	
	if (i == 0) {
	  categorySelected_pressed( familias->value( "idfamilia" ) );
	} // end if
		
	QHBoxLayout* categoryButton = new QHBoxLayout;
	categoryButton->setObjectName(QString("hboxBotones") + QString::number(i));
	categoryButton->setSpacing(0);
	
	QPushButton *childCategoryButton = new QPushButton ( ">", m_widget );
	childCategoryButton->setFixedWidth(42);
	childCategoryButton->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Expanding);
	
	if (m_paginadoListaPantallas) {
	  
	  if (familias->value( "hijos" ).toInt() == 0) {
	    childCategoryButton->hide();
	  } // end if
	  
	  if (familias->value( "padrefamilia" ).toInt() == 0) {
	      homeButton->hide();
	      previousButton->hide();
	  } // end if
	  
	} else {
	  
	  childCategoryButton->hide();
	  homeButton->hide();
	  previousButton->hide();
	  
	} //end if
	
        QPushButton *button = new QPushButton ( titulo, m_widget );
	button->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
	
	connect(button, SIGNAL(pressed()), m_signalMapperCategory, SLOT(map()));
	m_signalMapperCategory->setMapping(button, familias->value( "idfamilia" ));

	connect(childCategoryButton, SIGNAL(clicked()), m_signalMapperChildCategory, SLOT(map()));
	m_signalMapperChildCategory->setMapping(childCategoryButton, familias->value( "idfamilia" ));
	

	QColor buttonColor = QColor(familias->value( "colortpvfamilia" ));

	if ( buttonColor.isValid() ) {
	    /// Si el color es valido entonces lo utiliza para pintar el boton de ese color.
	    QString buttonStyles = "QPushButton { background-color: " + familias->value( "colortpvfamilia" ) + "; }";
	    button->setStyleSheet(buttonStyles);
	    style()->unpolish(button);
	    style()->polish(button);

  	    childCategoryButton->setStyleSheet(buttonStyles);
	    style()->unpolish(childCategoryButton);
	    style()->polish(childCategoryButton);

	} // end if
	
        button->setText ( titulo );
        button->setObjectName ( QString::number ( i ) );

	/// Divide a partes iguales los botones entre las columnas.
	indice = contadorBotones % columnasBotones;

	categoryButton->addWidget( button );
	categoryButton->addWidget( childCategoryButton );
	
	listaVBoxLayout[indice]->addLayout ( categoryButton );

	familias->nextRecord();
        i++;
	contadorBotones++;
	
    } // end while

    connect(m_signalMapperCategory, SIGNAL(mapped(const QString &)), this, SLOT(categorySelected_pressed(const QString &)) );
    connect(m_signalMapperChildCategory, SIGNAL(mapped(const QString &)), this, SLOT(categoryChanged_clicked(const QString &)), Qt::QueuedConnection);
    
    delete familias;

    /// Rellena con botones vacios para que todas las columnas tengan el mismo numero de botones.
    indice++;
    while (indice < columnasBotones) {
        QPushButton *button = new QPushButton (m_widget);
	button->setEnabled(FALSE);
	button->setFlat(TRUE);
	button->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
	listaVBoxLayout[indice]->addWidget ( button );
	indice++;
    } // end while

    mainLayout->addLayout ( topLayout );
    mainLayout->addLayout ( buttonsLayout );
    
    /// Agrego el widget al BLDockWidget
    m_widget->setLayout ( mainLayout );
   
    g_pantallas->setWidget ( m_widget );
   
}   


void ArtGraficosDb::homeButton_clicked()
{
   BL_FUNC_DEBUG

   ponListaPantallas( 0 );
} 

void ArtGraficosDb::previousButton_clicked()
{
   BL_FUNC_DEBUG

   ponListaPantallas(m_padreFamiliaAnterior.toInt());
}  


void ArtGraficosDb::categorySelected_pressed(const QString &idFamilia)
{
    BL_FUNC_DEBUG

    /// Busco la pantalla a mostrar en funcion del idFamilia.  
    for (int i = 0; i < m_listfamilias.size(); ++i) {
	if (m_listfamilias.at(i).m_idfamilia == idFamilia) {
	    muestraPantalla(i);
	    break;
	} // end if
    } // end for

}


void ArtGraficosDb::categoryChanged_clicked(const QString &idFamilia)
{
   BL_FUNC_DEBUG

   ponListaPantallas(idFamilia.toInt());
}

