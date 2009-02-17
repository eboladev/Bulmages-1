/***************************************************************************
 *   Copyright (C) 2007 by Tomeu Borras Riera                              *
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
EmpresaTPV::EmpresaTPV ( BulmaTPV *bges ) : BlMainCompany(), Input ( this )
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
void EmpresaTPV::createMainWindows ( BlSplashScreen *splash )
{
    _depura ( "EmpresaTPV::createMainWindows", 0 );
    /// Establecemos el porcentaje del carga de informaci&oacute;n en las diferentes ventanas.
    /// pb = 0%
    splash->mensaje ( _( "Inicializando listado de articulos" ) );
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
    m_bulmaTPV->setWindowTitle ( _( "Terminal Punto de Venta GPL" ) + " :: " + nameDB() );

    _depura ( "END EmpresaTPV::createMainWindows", 0 );
}


void EmpresaTPV::z()
{
	if (g_plugins->lanza("EmpresaTPV_z", this) )
		return;
    begin();

    QString query = "SELECT count(idz) AS numtickets, sum(totalalbaran) as total FROM albaran WHERE idz IS NULL AND ticketalbaran = TRUE ";
    BlDbRecordSet *cur = cargacursor ( query );
    QString numtickets = cur->valor ( "numtickets" );
    QString total = cur->valor ( "total" );
    if ( total == "" ) total = "0";
    delete cur;
    query = "INSERT INTO z (idalmacen, totalz, numtickets) VALUES(" + confpr->valor ( CONF_IDALMACEN_DEFECTO ) + ", "+total+","+numtickets+")";
    ejecuta ( query );
    query = "SELECT max(idz) AS id FROM z";
    cur = cargacursor ( query );
    QString idz = cur->valor ( "id" );
    delete cur;
    query = "UPDATE albaran set idz = " + idz + " WHERE idz IS NULL AND ticketalbaran = TRUE";
    ejecuta ( query );

//    query = "UPDATE z SET totalz = " + total + ", numtickets = " + numtickets + " WHERE idz =" + idz;
//    ejecuta ( query );
    commit();
    delete cur;

    QString querycont = "SELECT count(idalbaran) AS numtickets, sum(totalalbaran) as total FROM albaran WHERE idz = " + idz + " AND ticketalbaran = TRUE AND idforma_pago = " + confpr->valor ( CONF_IDFORMA_PAGO_CONTADO );
    BlDbRecordSet *cur1 = cargacursor ( querycont );
    QString numticketscont = cur1->valor ( "numtickets" );
    QString totalcont = cur1->valor ( "total" );
    if ( totalcont == "" ) totalcont = "0";
    delete cur1;

    QString queryvisa = "SELECT count(idalbaran) AS numtickets, sum(totalalbaran) as total FROM albaran WHERE idz = "+idz+" AND ticketalbaran = TRUE AND idforma_pago = "+ confpr->valor(CONF_IDFORMA_PAGO_VISA);

    BlDbRecordSet *cur2 = cargacursor ( queryvisa );
    QString numticketsvisa = cur2->valor ( "numtickets" );
    QString totalvisa = cur2->valor ( "total" );
    if ( totalvisa == "" ) totalvisa = "0";
    delete cur2;

// ========================================

    QFile file ( confpr->valor ( CONF_TICKET_PRINTER_FILE ) );
    if ( !file.open ( QIODevice::WriteOnly | QIODevice::Unbuffered ) ) {
        _depura ( "Error en la Impresion de ticket", 2 );
        return;
    } // end if
    file.write ( QString ( "Informe Z\n" ).toAscii() );
    file.write ( QString ( "=========\n" ).toAscii() );
    BlDbRecordSet *curemp = cargacursor ( "SELECT * FROM configuracion WHERE nombre='NombreEmpresa'" );
    if ( !curemp->eof() ) {
        file.write ( curemp->valor ( "valor" ).toAscii() );
        file.write ( "\n", 1 );
    } // end if
    delete curemp;
    file.write ( QString ( "====================================\n" ).toAscii() );
    cur = cargacursor ( "SELECT * FROM configuracion WHERE nombre='DireccionCompleta'" );
    if ( !cur->eof() ) {
        file.write ( cur->valor ( "valor" ).toAscii() );
        file.write ( "\n", 1 );
    } // end if
    delete cur;
    cur = cargacursor ( "SELECT * FROM configuracion WHERE nombre='CodPostal'" );
    if ( !cur->eof() ) {
        file.write ( cur->valor ( "valor" ).toAscii() );
    } // end if
    delete cur;

    file.write ( QString ( " " ).toAscii() );
    cur = cargacursor ( "SELECT * FROM configuracion WHERE nombre='Ciudad'" );
    if ( !cur->eof() ) {
        file.write ( cur->valor ( "valor" ).toAscii() );
        file.write ( QString ( " " ).toAscii() );
    } // end if
    delete cur;


    cur = cargacursor ( "SELECT * FROM configuracion WHERE nombre='Provincia'" );
    if ( !cur->eof() ) {
        file.write ( QString ( "(" ).toAscii() );
        file.write ( cur->valor ( "valor" ).toAscii() );
        file.write ( QString ( ")" ).toAscii() );
        file.write ( "\n", 1 );
    } // end if
    delete cur;

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

    str = "Num tickets Contado" + numticketscont.rightJustified ( 10, ' ' );
    file.write ( str.rightJustified ( 42, ' ' ).toAscii() );
    file.write ( "\n", 1 );

    str = "Total Contado" + totalcont.rightJustified ( 10, ' ' );
    file.write ( str.rightJustified ( 42, ' ' ).toAscii() );
    file.write ( "\n", 1 );

    str = "Num tickets Visa" + numticketsvisa.rightJustified ( 10, ' ' );
    file.write ( str.rightJustified ( 42, ' ' ).toAscii() );
    file.write ( "\n", 1 );

    str = "Total Visa" + totalvisa.rightJustified ( 10, ' ' );
    file.write ( str.rightJustified ( 42, ' ' ).toAscii() );
    file.write ( "\n", 1 );

// ============================================
    file.write ( QString ( "=======================\n" ).rightJustified ( 43, ' ' ).toAscii() );
    file.write ( QString ( "=======================\n" ).rightJustified ( 43, ' ' ).toAscii() );
    file.write ( QString ( "==== RESUMEN FAMILIAS ======\n" ).rightJustified ( 43, ' ' ).toAscii() );
// Informes por familias

    /// Imprimimos el almacen
    cur = cargacursor ( "SELECT * FROM familia");
    while ( !cur->eof() ) {

	QString querycont = "SELECT sum(cantlalbaran) AS unidades, sum(pvpivainclalbaran * cantlalbaran) as total FROM lalbaran NATURAL LEFT JOIN articulo  WHERE idalbaran IN (SELECT idalbaran FROM albaran WHERE idz="+idz+")  AND idfamilia = " + cur->valor("idfamilia");
	BlDbRecordSet *cur1 = cargacursor ( querycont );
	QString numticketscont = cur1->valor ( "unidades" );
	QString totalcont = cur1->valor ( "total" );
	if ( totalcont == "" ) totalcont = "0";
	delete cur1;

	if ( totalcont != "") {
		file.write ( QString ( "Familia: " ).toAscii() );
		file.write (cur->valor ( "nomfamilia" ).toAscii() );
		file.write ( "\n", 1 );
	
		str = "Und. Vendidas: " + numticketscont.rightJustified ( 10, ' ' );
		file.write ( str.rightJustified ( 42, ' ' ).toAscii() );
		file.write ( "\n", 1 );
		
		str = "Total:" + totalcont.rightJustified ( 10, ' ' );
		file.write ( str.rightJustified ( 42, ' ' ).toAscii() );
		file.write ( "\n", 1 );
	
		file.write ( QString ( "=======================\n" ).rightJustified ( 43, ' ' ).toAscii() );
	} // end if

	cur-> siguienteregistro();
    } // end if
    delete cur;
// Fin informes por familias

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
	if (g_plugins->lanza("EmpresaTPV_x", this) )
		return;

    QString query = "SELECT count(idalbaran) AS numtickets, sum(totalalbaran) as total FROM albaran WHERE idz IS NULL AND ticketalbaran = TRUE";
    BlDbRecordSet *cur = cargacursor ( query );
    QString numtickets = cur->valor ( "numtickets" );
    QString total = cur->valor ( "total" );
    if ( total == "" ) total = "0";
    delete cur;

    QString querycont = "SELECT count(idalbaran) AS numtickets, sum(totalalbaran) as total FROM albaran WHERE idz IS NULL AND ticketalbaran = TRUE AND idforma_pago = " + confpr->valor ( CONF_IDFORMA_PAGO_CONTADO );
    BlDbRecordSet *cur1 = cargacursor ( querycont );
    QString numticketscont = cur1->valor ( "numtickets" );
    QString totalcont = cur1->valor ( "total" );
    if ( totalcont == "" ) totalcont = "0";
    delete cur1;


    QString queryvisa = "SELECT count(idalbaran) AS numtickets, sum(totalalbaran) as total FROM albaran WHERE idz IS NULL AND ticketalbaran = TRUE AND idforma_pago = "+ confpr->valor(CONF_IDFORMA_PAGO_VISA);

    BlDbRecordSet *cur2 = cargacursor ( queryvisa );
    QString numticketsvisa = cur2->valor ( "numtickets" );
    QString totalvisa = cur2->valor ( "total" );
    if ( totalvisa == "" ) totalvisa = "0";
    delete cur2;


// ========================================

    QFile file ( confpr->valor ( CONF_TICKET_PRINTER_FILE ) );
    if ( !file.open ( QIODevice::WriteOnly | QIODevice::Unbuffered ) ) {
        _depura ( "Error en la Impresion de ticket", 2 );
        return;
    } // end if
    file.write ( QString ( "Informe X\n" ).toAscii() );
    file.write ( QString ( "=========\n" ).toAscii() );
    BlDbRecordSet *curemp = cargacursor ( "SELECT * FROM configuracion WHERE nombre='NombreEmpresa'" );
    if ( !curemp->eof() ) {
        file.write ( curemp->valor ( "valor" ).toAscii() );
        file.write ( "\n", 1 );
    } // end if
    delete curemp;
    file.write ( QString ( "====================================\n" ).toAscii() );
    cur = cargacursor ( "SELECT * FROM configuracion WHERE nombre='DireccionCompleta'" );
    if ( !cur->eof() ) {
        file.write ( cur->valor ( "valor" ).toAscii() );
        file.write ( "\n", 1 );
    } // end if
    ///file.write ( QString ( "C/LAS POZAS 181, LOCAL 43\n" ).toAscii() );
    delete cur;
    /// file.write ( QString ( "ALIMENTACION ECOLOGICA. HERBOLARIO\n" ).toAscii() );
    cur = cargacursor ( "SELECT * FROM configuracion WHERE nombre='CodPostal'" );
    if ( !cur->eof() ) {
        file.write ( cur->valor ( "valor" ).toAscii() );
    } // end if
    delete cur;

    file.write ( QString ( " " ).toAscii() );
    cur = cargacursor ( "SELECT * FROM configuracion WHERE nombre='Ciudad'" );
    if ( !cur->eof() ) {
        file.write ( cur->valor ( "valor" ).toAscii() );
        file.write ( QString ( " " ).toAscii() );
    } // end if
    delete cur;


    cur = cargacursor ( "SELECT * FROM configuracion WHERE nombre='Provincia'" );
    if ( !cur->eof() ) {
        file.write ( QString ( "(" ).toAscii() );
        file.write ( cur->valor ( "valor" ).toAscii() );
        file.write ( QString ( ")" ).toAscii() );
        file.write ( "\n", 1 );
    } // end if
    delete cur;

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

    str = "Num tickets Contado" + numticketscont.rightJustified ( 10, ' ' );
    file.write ( str.rightJustified ( 42, ' ' ).toAscii() );
    file.write ( "\n", 1 );

    str = "Total Contado" + totalcont.rightJustified ( 10, ' ' );
    file.write ( str.rightJustified ( 42, ' ' ).toAscii() );
    file.write ( "\n", 1 );

    str = "Num tickets Visa" + numticketsvisa.rightJustified ( 10, ' ' );
    file.write ( str.rightJustified ( 42, ' ' ).toAscii() );
    file.write ( "\n", 1 );

    str = "Total Visa" + totalvisa.rightJustified ( 10, ' ' );
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

    if ( g_plugins->lanza ( "EmpresaTPV_cobrar", this ) ) {
        return;
    } // end if

    if (m_ticketActual->DBvalue("idalbaran") != "") {
	    m_ticketActual->guardar();
    } else {
	    m_ticketActual->guardar();
	    m_ticketActual->imprimir();
    } // end if

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

    g_plugins->lanza ( "EmpresaTPV_cobrar_Post", this );

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


/// Guarda la configuracion de programa para poder recuperar algunas cosas de presentacion.
/**
**/
void EmpresaTPV::guardaConf()
{
    _depura ( "Company::guardaConf", 0 );
    QFile file ( confpr->valor ( CONF_DIR_USER ) + "bulmatpv_" + nameDB() + ".cfn" );
    /// Guardado del orden y de configuraciones varias.
    if ( file.open ( QIODevice::WriteOnly ) ) {
        QTextStream stream ( &file );
        stream << "<CONFIG>\n";
        stream << "\t<PRINCIPAL>\n";
        stream << "\t\t\t<X>" + QString::number ( m_bulmaTPV->x() ) + "</X>\n";
        stream << "\t\t\t<Y>" + QString::number ( m_bulmaTPV->y() ) + "</Y>\n";
        stream << "\t\t\t<WIDTH>" + QString::number ( m_bulmaTPV->width() ) + "</WIDTH>\n";
        stream << "\t\t\t<HEIGHT>" + QString::number ( m_bulmaTPV->height() ) + "</HEIGHT>\n";
	stream << "\t\t\t<TOOLBARSDOCKWIDGETS>" + QString(m_bulmaTPV->saveState().toBase64()) + "</TOOLBARSDOCKWIDGETS>\n";
        stream << "\t</PRINCIPAL>\n";
        stream << "</CONFIG>\n";
        file.close();
    } // end if
    _depura ( "END BfCompany::guardaConf", 0 );
}




/// Guarda la configuracion de programa para poder recuperar algunas cosas de presentacion.
/**
\return
**/
void EmpresaTPV::cargaConf()
{
    _depura ( "Company::cargaConf", 0 );
    QFile file ( confpr->valor ( CONF_DIR_USER ) + "bulmatpv_" + nameDB() + ".cfn" );
    QDomDocument doc ( "mydocument" );
    if ( !file.open ( QIODevice::ReadOnly ) )
        return;
    if ( !doc.setContent ( &file ) ) {
        file.close();
        return;
    }
    file.close();

    // print out the element names of all elements that are direct children
    // of the outermost element.
    QDomElement docElem = doc.documentElement();
    QDomElement principal = docElem.firstChildElement ( "PRINCIPAL" );
    /// Cogemos la coordenada X
    QString nx = principal.firstChildElement ( "X" ).toElement().text();

    /// Cogemos la coordenada Y
    QString ny = principal.firstChildElement ( "Y" ).toElement().text();

    /// Cogemos el ancho
    QString nwidth = principal.firstChildElement ( "WIDTH" ).toElement().text();

    /// Cogemos el alto
    QString nheight = principal.firstChildElement ( "HEIGHT" ).toElement().text();

    /// Establecemos la geometria de la ventana principal.
    m_bulmaTPV->setGeometry ( nx.toInt(), ny.toInt(), nwidth.toInt(), nheight.toInt() );

    /// Cogemos el ancho del indexador
    m_bulmaTPV->restoreState( QByteArray::fromBase64(QByteArray(principal.firstChildElement ( "TOOLBARSDOCKWIDGETS" ).toElement().text().toAscii())) );

    _depura ( "END BfCompany::cargaConf", 0 );
}




