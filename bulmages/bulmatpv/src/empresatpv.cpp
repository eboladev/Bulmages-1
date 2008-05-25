/***************************************************************************
 *   Copyright (C) 2007 by Tomeu Borrás Riera                              *
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

#include <Qt>
#include <QObject>
#include <qnamespace.h>
#include <QFile>
#include <QTextStream>
#include <QDomDocument>

#include "stdio.h"
#include "plugins.h"

#include "bulmatpv.h"


/** No precisa de operaciones en su construccion.
*/
/**
\param bges
**/
EmpresaTPV::EmpresaTPV ( BulmaTPV *bges ) : EmpresaBase(), Input ( this )
{
    _depura ( "EmpresaTPV::EmpresaTPV", 0 );
    m_bulmaTPV = bges;
    _depura ( "END EmpresaTPV::EmpresaTPV", 0 );
}


/// El destructor de la clase EmpresaTPV borra toda la memoria almacenada.
/**
**/
EmpresaTPV::~EmpresaTPV()
{
    _depura ( "EmpresaTPV::~EmpresaTPV", 0 );

    while ( !m_listaTickets.isEmpty() )
        delete m_listaTickets.takeFirst();

    /// Guardamos la configuracion.
    guardaConf();

    _depura ( "END EmpresaTPV::~EmpresaTPV", 0 );
}


/** Crea todas las ventanas que aparecen creadas al inicio del programa.
    Todas estas ventanas son principalmente los listados mas utilizados a partir de los
    cuales se pueden crear nuevas fichas y manejar todo.
*/
/**
\param splash
\return
**/
void EmpresaTPV::createMainWindows ( Splash *splash )
{
    _depura ( "EmpresaTPV::createMainWindows", 0 );
    /// Establecemos el porcentaje del carga de informaci&oacute;n en las diferentes ventanas.
    /// pb = 0%
    splash->mensaje ( QApplication::translate ( "EmpresaTPV", "Inicializando listado de articulos" ) );
    splash->setBarraProgreso ( 30 );
    m_progressbar->setValue ( 30 );


    /// Creamos los nuevos tickets.
    m_ticketActual = newTicket();
    if ( !m_ticketActual )
        _depura ( "error en el sistema, reservando memoria.", 0 );
    m_listaTickets.append ( m_ticketActual );



    /// Disparamos los plugins.
    int res = g_plugins->lanza ( "EmpresaTPV_createMainWindows_Post", this );
    if ( res != 0 ) {
        return;
    } // end if

    cargaConf();

    /// Ponemos el titulo de la ventana
    m_bulmaTPV->statusBar() ->showMessage ( nameDB(), 2000 );
    m_bulmaTPV->setWindowTitle ( QApplication::translate ( "EmpresaTPV", "Terminal Punto de Venta GPL" ) + " :: " + nameDB() );

    _depura ( "END EmpresaTPV::createMainWindows", 0 );
}


void EmpresaTPV::z()
{
    begin();
    QString query = "INSERT INTO z (idalmacen) VALUES(" + confpr->valor ( CONF_IDALMACEN_DEFECTO ) + ")";
    ejecuta ( query );
    query = "SELECT max(idz) AS id FROM z";
    cursor2 *cur = cargacursor ( query );
    QString idz = cur->valor ( "id" );
    delete cur;
    query = "UPDATE albaran set idz = " + idz + " WHERE idz IS NULL AND ticketalbaran = TRUE";
    ejecuta ( query );
    query = "SELECT count(idz) AS numtickets, sum(totalalbaran) as total FROM albaran WHERE idz = " + idz;
    cur = cargacursor ( query );
    QString numtickets = cur->valor ( "numtickets" );
    QString total = cur->valor ( "total" );
    if ( total == "" ) total = "0";
    query = "UPDATE z SET totalz = " + total + ", numtickets = " + numtickets + " WHERE idz =" + idz;
    ejecuta ( query );
    commit();
    delete cur;


// ========================================

    QFile file ( confpr->valor ( CONF_TICKET_PRINTER_FILE ) );
    if ( !file.open ( QIODevice::WriteOnly | QIODevice::Unbuffered ) ) {
        _depura ( "Error en la Impresion de ticket", 2 );
    } // end if
    file.write ( QString ( "Informe Z\n" ).toAscii() );
    file.write ( QString ( "=========\n" ).toAscii() );
    file.write ( QString ( "Empresa S.L\n" ).toAscii() );
    file.write ( QString ( "====================================\n" ).toAscii() );
    file.write ( QString ( "Direccion\n" ).toAscii() );
    file.write ( QString ( "CP: 07000 Palma de Mallorca\n" ).toAscii() );
    file.write ( QString ( "Tel: 971 00 00 00\n" ).toAscii() );
    /// Imprimimos espacios
    file.write ( "\n \n", 3 );


    /// Imprimimos la fecha
    file.write ( QString ( "Fecha: " ).toAscii() );
    QDate fecha = QDate::currentDate();
    QString sfecha = fecha.toString ( "d-M-yyyy" );
    file.write ( sfecha.toAscii() );
    QTime hora = QTime::currentTime();
    QString stime = " " + hora.toString ( "HH:mm" );
    file.write ( stime.toAscii() );
    file.write ( "\n", 1 );

    /// Imprimimos el almacen
    cur = cargacursor ( "SELECT * FROM almacen WHERE idalmacen=" + confpr->valor ( CONF_IDALMACEN_DEFECTO ) );
    if ( !cur->eof() ) {
        file.write ( QString ( "Almacen: " ).toAscii() );
        file.write ( cur->valor ( "nomalmacen" ).toAscii() );
        file.write ( "\n", 1 );
    } // end if
    delete cur;


    file.write ( "\n", 1 );
    file.write ( "\n", 1 );

// ============================================



    file.write ( QString ( "=======================\n" ).rightJustified ( 43, ' ' ).toAscii() );


    QString str = "Num tickets " + numtickets.rightJustified ( 10, ' ' );
    file.write ( str.rightJustified ( 42, ' ' ).toAscii() );
    file.write ( "\n", 1 );

    str = "Total " + total.rightJustified ( 10, ' ' );
    file.write ( str.rightJustified ( 42, ' ' ).toAscii() );
    file.write ( "\n", 1 );


// ============================================


    /// Imprimimos espacios
    file.write ( "\n \n \n \n", 7 );

    /// Preparamos para un codigo de barras
    /// Especificamos la altura del codigo de barras
    file.write ( "\x1Dh\x40", 3 );

    /// Especificamos que los caracteres vayan debajo del codigo de barras
    file.write ( "\x1DH\x02", 3 );

    /// Establecemos el tipo de codificacion para el codigo de barras
    file.write ( "\x1D", 1 );
    file.write ( "f\x01", 2 );

    /// Ponemos el ancho de la fuente a uno
    file.write ( "\x1D\x77\x01", 3 );
    /// Imprimimos la palabra top con el juego de caracteres 04
    file.write ( "\x1Dk\x04", 3 );
    file.write ( QString ( "ZZZ" ).toAscii() );
    file.write ( " ", 1 );
    file.write ( idz.toAscii() );
    file.write ( "\x00", 1 );

    /// Imprimimos espacios
    file.write ( "\n \n \n \n \n", 9 );


    /// El corte de papel.
    file.write ( "\x1D\x56\x01", 3 );
    file.close();

// ========================================
}



void EmpresaTPV::x()
{
    QString query = "SELECT count(idalbaran) AS numtickets, sum(totalalbaran) as total FROM albaran WHERE idz IS NULL AND ticketalbaran = TRUE";
    cursor2 *cur = cargacursor ( query );
    QString numtickets = cur->valor ( "numtickets" );
    QString total = cur->valor ( "total" );
    if ( total == "" ) total = "0";
    delete cur;


// ========================================

    QFile file ( confpr->valor ( CONF_TICKET_PRINTER_FILE ) );
    if ( !file.open ( QIODevice::WriteOnly | QIODevice::Unbuffered ) ) {
        _depura ( "Error en la Impresion de ticket", 2 );
    } // end if
    file.write ( QString ( "Informe X\n" ).toAscii() );
    file.write ( QString ( "=========\n" ).toAscii() );
    file.write ( QString ( "Empresa S.L.\n" ).toAscii() );
    file.write ( QString ( "====================================\n" ).toAscii() );
    file.write ( QString ( "Direccion\n" ).toAscii() );
    file.write ( QString ( "CP: 07000 Palma de Mallorca\n" ).toAscii() );
    file.write ( QString ( "Tel: 971 00 00 00\n" ).toAscii() );
    /// Imprimimos espacios
    file.write ( "\n \n", 3 );


    /// Imprimimos la fecha
    file.write ( QString ( "Fecha: " ).toAscii() );
    QDate fecha = QDate::currentDate();
    QString sfecha = fecha.toString ( "d-M-yyyy" );
    file.write ( sfecha.toAscii() );
    QTime hora = QTime::currentTime();
    QString stime = " " + hora.toString ( "HH:mm" );
    file.write ( stime.toAscii() );
    file.write ( "\n", 1 );

    /// Imprimimos el almacen
    cur = cargacursor ( "SELECT * FROM almacen WHERE idalmacen=" + confpr->valor ( CONF_IDALMACEN_DEFECTO ) );
    if ( !cur->eof() ) {
        file.write ( QString ( "Almacen: " ).toAscii() );
        file.write ( cur->valor ( "nomalmacen" ).toAscii() );
        file.write ( "\n", 1 );
    } // end if
    delete cur;


    file.write ( "\n", 1 );
    file.write ( "\n", 1 );

// ============================================



    file.write ( QString ( "=======================\n" ).rightJustified ( 43, ' ' ).toAscii() );


    QString str = "Num tickets " + numtickets.rightJustified ( 10, ' ' );
    file.write ( str.rightJustified ( 42, ' ' ).toAscii() );
    file.write ( "\n", 1 );

    str = "Total " + total.rightJustified ( 10, ' ' );
    file.write ( str.rightJustified ( 42, ' ' ).toAscii() );
    file.write ( "\n", 1 );


// ============================================


    /// Imprimimos espacios
    file.write ( "\n \n \n \n", 7 );

    /// Imprimimos espacios
    file.write ( "\n \n \n \n \n", 9 );


    /// El corte de papel.
    file.write ( "\x1D\x56\x01", 3 );
    file.close();

// ========================================
}





/// Guarda la configuracion de programa para poder recuperar algunas cosas de presentacion.
/**
**/
void EmpresaTPV::guardaConf()
{
    _depura ( "EmpresaTPV::guardaConf", 0 );
    /*
        QFile file(confpr->valor(CONF_DIR_USER) + "bulmafact_" + nameDB() + ".cfn");
        /// Guardado del orden y de configuraciones varias.
        if (file.open(QIODevice::WriteOnly)) {
            QTextStream stream(&file);
            stream << "<CONFIG>\n";
            stream << "\t<PRINCIPAL>\n";
            stream << "\t\t\t<X>" + QString::number(m_bulmafact->x()) + "</X>\n";
            stream << "\t\t\t<Y>" + QString::number(m_bulmafact->y()) + "</Y>\n";
            stream << "\t\t\t<WIDTH>" + QString::number(m_bulmafact->width()) + "</WIDTH>\n";
            stream << "\t\t\t<HEIGHT>" + QString::number(m_bulmafact->height()) + "</HEIGHT>\n";
            stream << "\t\t\t<INDEXADOR>" + (m_bulmafact->actionIndexador->isChecked() ? QString("TRUE") : QString("FALSE")) + "</INDEXADOR>\n";
            stream << "\t</PRINCIPAL>\n";


            for (int i = 0; i < m_listventanas->numVentanas(); i++) {
            QObject *obj = m_listventanas->ventana(i);
                    QWidget *wid = (QWidget *) obj;
            stream << "\t<VENTANA>\n";
            stream << "\t\t<VNAME>" + obj->objectName() + "</VNAME>\n";
            stream << "\t\t<VX>" + QString::number(wid->parentWidget()->x()) + "</VX>\n";
            stream << "\t\t<VY>" + QString::number(wid->parentWidget()->y()) + "</VY>\n";
            stream << "\t\t<VWIDTH>" + QString::number(wid->width()) + "</VWIDTH>\n";
            stream << "\t\t<VHEIGHT>" + QString::number(wid->height()) + "</VHEIGHT>\n";
            stream << "\t\t<VVISIBLE>" + (wid->isVisible() ? QString("TRUE") : QString("FALSE")) + "</VVISIBLE>\n";
            stream << "\t\t<VMAXIMIZED>" + (wid->isMaximized() ? QString("TRUE") : QString("FALSE")) + "</VMAXIMIZED>\n";
            stream << "\t\t<VACTIVEWINDOW>" + (m_bulmafact->workspace()->activeWindow() == wid ? QString("TRUE") : QString("FALSE")) + "</VACTIVEWINDOW>";
            stream << "\t</VENTANA>\n";
        } // end for

            stream << "</CONFIG>\n";
            file.close();
        } // end if
    */
    _depura ( "END EmpresaTPV::guardaConf", 0 );
}


/// Guarda la configuracion de programa para poder recuperar algunas cosas de presentacion.
/**
\return
**/
void EmpresaTPV::cargaConf()
{
    _depura ( "EmpresaTPV::cargaConf", 0 );
    /*
        QFile file(confpr->valor(CONF_DIR_USER) + "bulmafact_" + nameDB() + ".cfn");
        QDomDocument doc("mydocument");
        if (!file.open(QIODevice::ReadOnly))
            return;
        if (!doc.setContent(&file)) {
            file.close();
            return;
        }
        file.close();

        // print out the element names of all elements that are direct children
        // of the outermost element.
        QDomElement docElem = doc.documentElement();
        QDomElement principal = docElem.firstChildElement("PRINCIPAL");
        /// Cogemos la coordenada X
        QString nx = principal.firstChildElement("X").toElement().text();

        /// Cogemos la coordenada Y
        QString ny = principal.firstChildElement("Y").toElement().text();

        /// Cogemos el ancho
        QString nwidth = principal.firstChildElement("WIDTH").toElement().text();

        /// Cogemos el alto
        QString nheight = principal.firstChildElement("HEIGHT").toElement().text();

        /// Establecemos la geometria de la ventana principal.
        m_bulmafact->setGeometry(nx.toInt(), ny.toInt(), nwidth.toInt(), nheight.toInt());

        /// Cogemos el indexador
        QString indexador = principal.firstChildElement("INDEXADOR").toElement().text();
        if (indexador == "TRUE") {
        s_indexadorCambiaEstado(TRUE);
        m_bulmafact->actionIndexador->setChecked(TRUE);
        } else {
        s_indexadorCambiaEstado(FALSE);
        m_bulmafact->actionIndexador->setChecked(FALSE);
        } // end if

        /// Tratamos cada ventana
       QWidget *activewindow = NULL;
       QDomNodeList nodos = docElem.elementsByTagName("VENTANA");
            for (int i = 0; i < nodos.count(); i++) {
                QDomNode ventana = nodos.item(i);
                QDomElement e1 = ventana.toElement(); /// try to convert the node to an element.
                if( !e1.isNull() ) { /// the node was really an element.
                 QString vname = e1.firstChildElement("VNAME").toElement().text();
                for (int j = 0; j < m_listventanas->numVentanas(); j++) {
                    QObject *obj = m_listventanas->ventana(j);
                    QWidget *wid = (QWidget *) obj;
                    if (obj->objectName() == vname) {
                        QString vx = e1.firstChildElement("VX").toElement().text();
                        QString vy = e1.firstChildElement("VY").toElement().text();
                        QString vwidth = e1.firstChildElement("VWIDTH").toElement().text();
                        QString vheight = e1.firstChildElement("VHEIGHT").toElement().text();
                        QString vvisible = e1.firstChildElement("VVISIBLE").toElement().text();
                        QString vmaximized = e1.firstChildElement("VMAXIMIZED").toElement().text();
                        QString vactivewindow = e1.firstChildElement("VACTIVEWINDOW").toElement().text();
                        /// Establecemos la geometria de la ventana principal.
                        wid->resize(vwidth.toInt(), vheight.toInt());
                        wid->parentWidget()->move(vx.toInt(), vy.toInt());
                        if ( vvisible == "TRUE") {
                            wid->showNormal();
                        } // end if
                        if ( vmaximized == "TRUE") {
                            wid->showMaximized();
                        }
                        if ( vactivewindow == "TRUE") {
                            activewindow = wid;
                        }
                    } // end if
                } // end for
                } // end if
            } // end for
        /// Si hay una ventana activa se pone como activa.
        if (activewindow)
            m_bulmafact->workspace()->setActiveWindow(activewindow);
    */
    _depura ( "END EmpresaTPV::cargaConf", 0 );
}



Ticket *EmpresaTPV::newTicket()
{
    _depura ( "EmpresaTPV::newTicket", 0 );
    /// Lanzamos los plugins necesarios.
    Ticket *bud;
    if ( g_plugins->lanza ( "EmpresaTPV_newTicket", this, ( void ** ) & bud ) )
        return bud;
    bud = new Ticket ( this, NULL );
    _depura ( "END EmpresaTPV::newTicket", 0 );
    return bud;
}

void EmpresaTPV::cobrar()
{
    QString idtrabajador = m_ticketActual->DBvalue ( "idtrabajador" );
    m_ticketActual->guardar();
    m_ticketActual->imprimir();
    m_listaTickets.removeAt ( m_listaTickets.indexOf ( m_ticketActual ) );
    m_ticketActual = NULL;


    Ticket *ticket = NULL;
    Ticket *ticketv = NULL;

    /// Buscamos el ticket vacio de este trabajador y lo pintamos
    for ( int i = 0; i < m_listaTickets.size(); ++i ) {
        ticket = m_listaTickets.at ( i );

        if ( "" == ticket->DBvalue ( "nomticket" ) && idtrabajador == ticket->DBvalue ( "idtrabajador" ) ) {
            setTicketActual ( ticket );
            ticket->pintar();
            ticketv = ticket;
        }// end if
    }// end for

    /// Si el trabajador no tiene ticket vacio lo creamos y le ponemos el idtrabajador.
    if ( !ticketv ) {
        Ticket * tick = newTicket();
        tick->setDBvalue ( "idtrabajador", idtrabajador );
        setTicketActual ( tick );
        m_listaTickets.append ( tick );
        tick->pintar();
    }// end if

}



Ticket *EmpresaTPV::ticketActual()
{
    return m_ticketActual;
}
QList<Ticket *> *EmpresaTPV::listaTickets()
{
    return & m_listaTickets;
}
void EmpresaTPV::setTicketActual ( Ticket *tick )
{
    m_ticketActual = tick;
}


