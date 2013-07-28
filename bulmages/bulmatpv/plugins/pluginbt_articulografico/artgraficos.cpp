#include <QtWidgets/QTableWidget>
#include <QtWidgets/QLabel>
#include <QtWidgets/QHeaderView>
#include <QtCore/QFile>
#include <QtXml/QDomDocument>
#include <QtGui/QPicture>
#include <QtGui/QPainter>
#include <QtWidgets/QPushButton>
#include <QtCore/QTextStream>

#include "artgraficos.h"
#include "blconfiguration.h"
#include "bldockwidget.h"


extern BlDockWidget *g_pantallas;


ArtGraficos::ArtGraficos ( BlMainCompany *emp, QWidget *parent ) : BlWidget ( emp, parent )
{
    BL_FUNC_DEBUG
    setupUi ( this );
    setObjectName ( "ArtGraficos" );
    m_numPantallas = 0;
    m_pantallaActual = 0;
    mui_list->horizontalHeader() ->hide();
    mui_list->verticalHeader() ->hide();
    cargaXML ( CONFIG_DIR_CONFIG + QString ( "pantallastpv.xml" ) );
    ponPantallas();
    muestraPantalla ( 0 );
    
}


ArtGraficos::~ArtGraficos()
{
    BL_FUNC_DEBUG
    
}


void ArtGraficos::on_mui_list_cellClicked ( int row, int column )
{
    BL_FUNC_DEBUG

    QString artvarios = g_confpr->value( CONF_ARTICULOS_VARIOS );
    QString codigo = m_articulos[row][column];
    QString idArticulo = m_nodarticulos[row][column].firstChildElement("IDARTICULO").toElement().text();
    BtTicket *tick = (( BtCompany * ) mainCompany() ) ->ticketActual();


    /// Buscamos si ya hay una linea con el articulo que buscamos
    BlDbRecord *m_lineaActual = NULL;
    BlDbRecord *item;
    bool nuevaLinea = artvarios.contains ( codigo );


    for ( int i = 0; i < tick->listaLineas() ->size(); ++i ) {
        item = tick->listaLineas() ->at ( i );
        if ( item->dbValue ( "idarticulo" ) == idArticulo )
            m_lineaActual = item;
    }// end for

    if ( m_lineaActual && nuevaLinea == false ) {
        /// Ya hay una linea con este articulo (es un agregado)
        BlFixed cantidadib ( m_lineaActual->dbValue ( "cantlalbaran" ) );
        BlFixed cant1 = cantidadib + BlFixed("1");
        m_lineaActual->setDbValue ( "cantlalbaran", cant1.toQString() );
    } else {
        /// No hay ningun item con este articulo (es una insercion)
        m_lineaActual = tick->agregarLinea();
        m_lineaActual->setDbValue ( "idarticulo", idArticulo );
	    QString a("1.00");
        m_lineaActual->setDbValue ( "cantlalbaran", "1.00" );
	m_lineaActual->setDbValue ( "pvpivainclalbaran", m_nodarticulos[row][column].firstChildElement("PVPIVAINCARTICULO").toElement().text());
	m_lineaActual->setDbValue ( "pvplalbaran", m_nodarticulos[row][column].firstChildElement("PVPARTICULO").toElement().text());
	m_lineaActual->setDbValue ( "codigocompletoarticulo", m_nodarticulos[row][column].firstChildElement("PVPIVAINCARTICULO").toElement().text());
	m_lineaActual->setDbValue ( "nomarticulo", m_nodarticulos[row][column].firstChildElement("NOMARTICULO").toElement().text());
	m_lineaActual->setDbValue ( "desclalbaran", m_nodarticulos[row][column].firstChildElement("NOMARTICULO").toElement().text());
	m_lineaActual->setDbValue ( "ivalalbaran", m_nodarticulos[row][column].firstChildElement("IVAARTICULO").toElement().text());

    } // end if

    /// Pintamos el ticket ya que se ha modificado.
    tick->pintar();

    BlDebug::blDebug("END ArtGraficos::on_mui_list_cellClicked");
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

    QDomNodeList nodos = pantalla.elementsByTagName ( "ITEM" );
    int nitem = 0;

    for ( int row = 0; row < grid.toInt(); row++ ) {
        for ( int column  = 0; column < grid.toInt(); column++ ) {

            if ( nitem < nodos.count() ) {
                QDomNode ventana = nodos.item ( nitem );

                QString nombre = ventana.firstChildElement ( "TITULO" ).toElement().text();

                QDomElement e1 = ventana.toElement(); /// try to convert the node to an element.
                QString text = e1.text();

                QString codigo = ventana.firstChildElement ( "CODIGO" ).toElement().text();
                if ( codigo.isEmpty() ) {
                    codigo = text;
                } // end if

                QString pvp = ventana.firstChildElement ( "PVPIVAINCARTICULO" ).toElement().text();
                if ( pvp.isEmpty() ) {
                    pvp = text;
                } // end if


                /// Creamos el elemento y lo ponemos en la tabla.
                QLabel *lab = new QLabel ( NULL );
                /// El escalado completo tarda demasiado.
//                lab->setPixmap(QPixmap("/var/bulmages/articles/"+text+".jpg", "no", Qt::OrderedDither | Qt::OrderedAlphaDither | Qt::AvoidDither).scaled(cellwidth.toInt(),cellwidth.toInt()));
                /// El escalado lateral tambien tarda demasiado
//                lab->setPixmap(QPixmap("/var/bulmages/articles/"+text+".jpg").scaledToWidth(cellwidth.toInt()));
                /// Ponemos las imagenes sin escalado.
//                lab->setPixmap ( QPixmap ( g_confpr->value( CONF_DIR_THUMB_ARTICLES ) + text + ".jpg" ) );

// Probamos con una Picture
                QPicture picture;
                QPainter painter;
                painter.begin ( &picture );        // paint in picture
//  painter.drawPixmap(0,0,cellwidth.toInt(),cellwidth.toInt(),QPixmap ( g_confpr->value( CONF_DIR_THUMB_ARTICLES ) + codigo + ".jpg" ));

//                painter.drawPixmap ( 0, 0, cellwidth.toInt(), cellwidth.toInt(), QPixmap ( g_confpr->value( CONF_DIR_THUMB_ARTICLES ) + "blanco.jpg" ) );

                painter.drawPixmap ( 0, 0, cellwidth.toInt(), cellwidth.toInt() - 25, QPixmap ( g_confpr->value( CONF_DIR_THUMB_ARTICLES ) + codigo + ".jpg" ) );

//         painter.drawEllipse(10,20, 80,70); // draw an ellipse
                painter.setPen ( QColor ( 0, 0, 0 ) );
                painter.setBackground ( QColor ( 0, 0, 0 ) );

                painter.drawText ( 5, 95, nombre );

		painter.drawText(5, 10, pvp);
//  painter.setPen(QColor(0,25,0));
//  painter.drawText(5,10,nombre);
//  painter.drawText(5,15,nombre);
//  painter.drawText(5,5,codigo);
//  painter.drawText(5,10,cellwidth);

                painter.end();                     // painting done


                lab->setPicture ( picture );

                mui_list->setCellWidget ( row, column, lab );
                m_articulos[row][column] = codigo;
		m_nodarticulos[row][column] = nodos.item ( nitem );
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

    /// Cogemos parametros adicionales para que no haya que ir a buscarlos a la base de datos y asi mejore la velocidad.

    /// Itero sobre las pantallas para obtener los nombres de pantalla y crear los botones pretinentes.
    QDomElement docElem = m_doc.documentElement();
    QDomNodeList nodos = docElem.elementsByTagName ( "ITEM" );

    for ( int i = 0; i < nodos.count(); i++ ) {
        /// Cogemos el titulo de la pantalla
        QString codigo = nodos.item ( i ).firstChildElement ( "CODIGO" ).text();
        /// Buscamos los parametros en la base de datos.
        QString query = "SELECT * FROM articulo LEFT JOIN (SELECT idtipo_iva, porcentasa_iva, fechatasa_iva FROM tasa_iva ) AS t1 ON articulo.idtipo_iva = t1.idtipo_iva WHERE codigocompletoarticulo = '" + codigo + "' ORDER BY t1.fechatasa_iva LIMIT 1";
        BlDbRecordSet *cur = mainCompany() ->loadQuery ( query );
        if ( !cur->eof() ) {

		QDomElement tag = m_doc.createElement( "PVPIVAINCARTICULO" );
		tag.appendChild( m_doc.createTextNode( cur->value("pvpivaincarticulo") ) );
		nodos.item(i).appendChild( tag );

		QDomElement tag4 = m_doc.createElement( "PVPARTICULO" );
		tag4.appendChild( m_doc.createTextNode( cur->value("pvparticulo") ) );
		nodos.item(i).appendChild( tag4 );

		QDomElement tag1 = m_doc.createElement( "IDARTICULO" );
		tag1.appendChild( m_doc.createTextNode( cur->value("idarticulo") ) );
		nodos.item(i).appendChild( tag1 );

		QDomElement tag2 = m_doc.createElement( "NOMARTICULO" );
		tag2.appendChild( m_doc.createTextNode( cur->value("nomarticulo") ) );
		nodos.item(i).appendChild( tag2 );

		QDomElement tag3 = m_doc.createElement( "IVAARTICULO" );
		tag3.appendChild( m_doc.createTextNode( cur->value("porcentasa_iva") ) );
		nodos.item(i).appendChild( tag3 );

		/// Ponemos todos los campos del registro cargado para que esten cacheados.
		for (int j = 0; j < cur->numcampos(); j++) {
			QDomElement tag3 = m_doc.createElement(cur->fieldName(j) );
			tag3.appendChild( m_doc.createTextNode( cur->value(j) ) );
			nodos.item(i).appendChild( tag3 );
		} // end for

        } // end if
        delete cur;

    } // end for


}

void ArtGraficos::ponPantallas()
{
    /// Creo el Widget que estara ubicado en el dockwidget que se ha creado en pluginartgraficos.cpp
    QWidget *widget = new QWidget;
    QVBoxLayout *hboxLayout1 = new QVBoxLayout;
    hboxLayout1->setSpacing ( 5 );
    hboxLayout1->setMargin ( 5 );
    hboxLayout1->setObjectName ( QString::fromUtf8 ( "hboxLayout1" ) );

    /// Itero sobre las pantallas para obtener los nombres de pantalla y crear los botones pretinentes.
    QDomElement docElem = m_doc.documentElement();
    QDomNodeList nodos = docElem.elementsByTagName ( "PANTALLA" );

    for ( int i = 0; i < nodos.count(); i++ ) {
        /// Cogemos el titulo de la pantalla
        QString titulo = nodos.item ( i ).firstChildElement ( "NOMBRE" ).text();
        QPushButton *pb = new QPushButton ( titulo, g_pantallas );
        pb->setText ( titulo );
        pb->setObjectName ( QString::number ( i ) );
        /// Hago la conexcion del pulsado con el metodo pulsadoBoton para que se cambie la pantalla.
        connect ( pb, SIGNAL ( pressed() ), this, SLOT ( pulsadoBoton() ) );
        hboxLayout1->addWidget ( pb );
    }

    /// Agrego el widget al BLDockWidget
    widget->setLayout ( hboxLayout1 );
    g_pantallas->setWidget ( widget );
}


void ArtGraficos::pulsadoBoton()
{
    muestraPantalla ( sender()->objectName().toInt() );
}
