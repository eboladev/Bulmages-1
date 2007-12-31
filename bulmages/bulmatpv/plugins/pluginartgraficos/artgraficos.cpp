#include "artgraficos.h"
#include <QTableWidget>
#include <QLabel>
#include <QHeaderView>
#include <QFile>
#include <QDomDocument>
#include "funcaux.h"
#include "configuracion.h"

ArtGraficos::ArtGraficos ( EmpresaBase *emp, QWidget *parent ) : BLWidget ( emp, parent )
{
    setupUi ( this );
    m_numPantallas = 0;
    m_pantallaActual = 0;
    mui_list->horizontalHeader() ->hide();
    mui_list->verticalHeader() ->hide();
    cargaXML ( "/etc/bulmages/pantallastpv.xml" );
    muestraPantalla ( 0 );
}


ArtGraficos::~ArtGraficos()
{}

void ArtGraficos::on_mui_list_cellClicked ( int row, int column )
{
    ( ( EmpresaTPV * ) empresaBase() ) ->ticketActual() ->insertarArticuloCodigo ( m_articulos[row][column] );
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
                QDomElement e1 = ventana.toElement(); /// try to convert the node to an element.
                QString text = e1.text();

                /// Creamos el elemento y lo ponemos en la tabla.
                QLabel *lab = new QLabel ( NULL );
                /// El escalado completo tarda demasiado.
//                lab->setPixmap(QPixmap("/var/bulmages/articles/"+text+".jpg", "no", Qt::OrderedDither | Qt::OrderedAlphaDither | Qt::AvoidDither).scaled(cellwidth.toInt(),cellwidth.toInt()));
                /// El escalado lateral tambien tarda demasiado
//                lab->setPixmap(QPixmap("/var/bulmages/articles/"+text+".jpg").scaledToWidth(cellwidth.toInt()));
                /// Ponemos las imagenes sin escalado.
                lab->setPixmap ( QPixmap ( confpr->valor ( CONF_DIR_THUMB_ARTICLES ) + text + ".jpg" ) );

                mui_list->setCellWidget ( row, column, lab );
                m_articulos[row][column] = text;
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


