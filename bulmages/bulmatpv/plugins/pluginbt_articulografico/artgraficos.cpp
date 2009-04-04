#include <QTableWidget>
#include <QLabel>
#include <QHeaderView>
#include <QFile>
#include <QDomDocument>
#include <QPicture>
#include <QPainter>
#include <QPushButton>

#include "artgraficos.h"
#include "blconfiguration.h"
#include "bldockwidget.h"


extern BlDockWidget *g_pantallas;


ArtGraficos::ArtGraficos ( BlMainCompany *emp, QWidget *parent ) : BlWidget ( emp, parent )
{
    _depura ( "ArtGraficos::~ArtGraficos", 0 );
    setupUi ( this );
    setObjectName ( "ArtGraficos" );
    m_numPantallas = 0;
    m_pantallaActual = 0;
    mui_list->horizontalHeader() ->hide();
    mui_list->verticalHeader() ->hide();
    cargaXML ( CONFIG_DIR_CONFIG + QString ( "pantallastpv.xml" ) );
	ponPantallas();
    muestraPantalla ( 0 );
    _depura ( "END ArtGraficos::~ArtGraficos", 0 );
}


ArtGraficos::~ArtGraficos()
{
    _depura ( "ArtGraficos::~ArtGraficos", 0 );
    _depura ( "END ArtGraficos::~ArtGraficos", 0 );
}


void ArtGraficos::on_mui_list_cellClicked ( int row, int column )
{
    QString artvarios = g_confpr->valor(CONF_ARTICULOS_VARIOS);
    QString codigo = m_articulos[row][column];
    if (! artvarios.contains(codigo)) {
	    ( ( BtCompany * ) mainCompany() ) ->ticketActual() ->insertarArticuloCodigo ( m_articulos[row][column] );
    } else {
	    ( ( BtCompany * ) mainCompany() ) ->ticketActual() ->insertarArticuloCodigoNL ( m_articulos[row][column]);
    } // end if
}

void ArtGraficos::on_mui_botonSiguiente_clicked()
{
    muestraPantalla ( m_pantallaActual + 1 );
}

void ArtGraficos::on_mui_botonAnterior_clicked()
{
    muestraPantalla ( m_pantallaActual - 1 );
}

void ArtGraficos::muestraPantalla ( int numpantalla )
{
    m_pantallaActual = numpantalla;

    QDomElement docElem = m_doc.documentElement();
//    QDomElement pantalla = docElem.firstChildElement("PANTALLA");

    m_numPantallas = docElem.elementsByTagName ( "PANTALLA" ).count();

    /// Simulamos un sistema ciclico.
    if ( m_pantallaActual > m_numPantallas - 1 ) m_pantallaActual = 0;
    if ( m_pantallaActual < 0 ) m_pantallaActual = m_numPantallas - 1;

    QDomElement pantalla = docElem.elementsByTagName ( "PANTALLA" ).at ( m_pantallaActual ).toElement();
    /// Cogemos la coordenada X
    QString grid = pantalla.firstChildElement ( "GRID" ).toElement().text();

    /// Cogemos la coordenada Y
    QString cellwidth = pantalla.firstChildElement ( "CELLWIDTH" ).toElement().text();

    /// Cogemos el titulo de la pantalla
    QString titulo = pantalla.firstChildElement ( "NOMBRE" ).toElement().text();
    mui_titulo->setText ( titulo );

    mui_list->clear();
    mui_list->setRowCount ( grid.toInt() );
    mui_list->setColumnCount ( grid.toInt() );

    for ( int row = 0; row < grid.toInt(); row++ )
        mui_list->setRowHeight ( row, cellwidth.toInt() );
    for ( int column  = 0; column < grid.toInt(); column++ )
        mui_list->setColumnWidth ( column, cellwidth.toInt() );



    /// Tratamos cada ventana
    QWidget *activewindow = NULL;
    QDomNodeList nodos = pantalla.elementsByTagName ( "ITEM" );
    int nitem = 0;

    for ( int row = 0; row < grid.toInt(); row++ ) {
        for ( int column  = 0; column < grid.toInt(); column++ ) {

            if ( nitem < nodos.count() ) {
                QDomNode ventana = nodos.item ( nitem );

		QString nombre = ventana.firstChildElement ( "TITULO").toElement().text();

                QDomElement e1 = ventana.toElement(); /// try to convert the node to an element.
                QString text = e1.text();

		QString codigo = ventana.firstChildElement ( "CODIGO").toElement().text();
		if (codigo.isEmpty()) {
			codigo = text;
		} // end if


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
         painter.begin(&picture);           // paint in picture
//	 painter.drawPixmap(0,0,cellwidth.toInt(),cellwidth.toInt(),QPixmap ( g_confpr->valor ( CONF_DIR_THUMB_ARTICLES ) + codigo + ".jpg" ));

	 painter.drawPixmap(0,0,cellwidth.toInt(),cellwidth.toInt(),QPixmap ( g_confpr->valor ( CONF_DIR_THUMB_ARTICLES ) + "blanco.jpg" ));

	 painter.drawPixmap(0,0,cellwidth.toInt(),cellwidth.toInt()-25,QPixmap ( g_confpr->valor ( CONF_DIR_THUMB_ARTICLES ) + codigo + ".jpg" ));

//         painter.drawEllipse(10,20, 80,70); // draw an ellipse
	 painter.setPen(QColor(0,0,0));
	 painter.setBackground(QColor(0,0,0));

	 painter.drawText(5,95,nombre);
//	 painter.setPen(QColor(0,25,0));
//	 painter.drawText(5,10,nombre);
//	 painter.drawText(5,15,nombre);
//	 painter.drawText(5,5,codigo);
//	 painter.drawText(5,10,cellwidth);

         painter.end();                     // painting done


       lab->setPicture ( picture );

                mui_list->setCellWidget ( row, column, lab );
                m_articulos[row][column] = codigo;
                nitem ++;
            } // end if

        }// end for
    } // end for


}

void ArtGraficos::cargaXML ( QString filename )
{
    QFile file ( filename );
    if ( !file.open ( QIODevice::ReadOnly ) )
        return;
    if ( !m_doc.setContent ( &file ) ) {
        file.close();
        return;
    }
    file.close();

}

void ArtGraficos::ponPantallas() {
	/// Creo el Widget que estara ubicado en el dockwidget que se ha creado en pluginartgraficos.cpp
	QWidget *widget = new QWidget;
    QVBoxLayout *hboxLayout1 = new QVBoxLayout;
    hboxLayout1->setSpacing ( 5 );
    hboxLayout1->setMargin ( 5 );
    hboxLayout1->setObjectName ( QString::fromUtf8 ( "hboxLayout1" ) );

	/// Itero sobre las pantallas para obtener los nombres de pantalla y crear los botones pretinentes.
	QDomElement docElem = m_doc.documentElement();
	QDomNodeList nodos = docElem.elementsByTagName("PANTALLA");

	int i=0;
	for (int i=0; i < nodos.count(); i++) {
		/// Cogemos el titulo de la pantalla
		QString titulo = nodos.item(i).firstChildElement( "NOMBRE" ).text();
		QPushButton *pb = new QPushButton( titulo, g_pantallas );
		pb->setText(titulo);
		pb->setObjectName(QString::number(i));
		/// Hago la conexcion del pulsado con el metodo pulsadoBoton para que se cambie la pantalla.
		connect (pb, SIGNAL(pressed()), this, SLOT(pulsadoBoton()));
		hboxLayout1->addWidget(pb);
	}

	/// Agrego el widget al BLDockWidget
	widget->setLayout(hboxLayout1);
	g_pantallas->setWidget(widget);
}


void ArtGraficos::pulsadoBoton() {
	muestraPantalla(sender()->objectName().toInt());
}
