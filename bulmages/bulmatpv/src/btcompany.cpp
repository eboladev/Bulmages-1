/***************************************************************************
 *   Copyright (C) 2007 by Tomeu Borras Riera                              *
 *   tborras@conetxia.com                                                  *
 *                                                                         *
 *   Copyright (C) 2009 by Arturo Martin Llado                             *
 *   amartin@conetxia.com                                                  *
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
#include "blplugins.h"
#include "btbulmatpv.h"

/** No precisa de operaciones en su construccion.
*/
/**
\param bges
**/
BtCompany::BtCompany ( BtBulmaTPV* bges ) : BlMainCompany(), BtInput ( this )
{
    blDebug ( "BtCompany::BtCompany", 0 );
    m_bulmaTPV = bges;

    m_decimalesCantidad = 2;
    m_decimalesPrecio = 2;

    blDebug ( "END BtCompany::BtCompany", 0 );
}

/// El destructor de la clase BtCompany borra toda la memoria almacenada.
/**
**/
BtCompany::~BtCompany()
{
    blDebug ( "BtCompany::~BtCompany", 0 );

/// Pruebas de exportacion a XML
/// ======================================
    /// Guardamos los tickets para poder recuperarlos despues
    QFile file ( g_confpr->valor ( CONF_DIR_USER ) + "tickets_bulmatpv.xml" );
    /// Guardado del orden y de configuraciones varias.
    if ( file.open ( QIODevice::WriteOnly ) ) {
        QTextStream stream ( &file );
        stream << exportXML();
        file.close();
    } // end if

/// ======================================


    while ( !m_listaTickets.isEmpty() )
        delete m_listaTickets.takeFirst();

    /// Guardamos la configuracion.
    guardaConf();

    blDebug ( "END BtCompany::~BtCompany", 0 );
}

int BtCompany::decimalesCantidad() {
   return m_decimalesCantidad;
}

void BtCompany::setDecimalesCantidad( int numd) {
   m_decimalesCantidad = numd;
}

int BtCompany::decimalesPrecio() {
   return m_decimalesPrecio;
}

void BtCompany::setDecimalesPrecio(int numd) {
   m_decimalesPrecio = numd;
}

/** Crea todas las ventanas que aparecen creadas al inicio del programa.
    Todas estas ventanas son principalmente los listados mas utilizados a partir de los
    cuales se pueden crear nuevas fichas y manejar todo.
*/
/**
\param splash
\return
**/
void BtCompany::createMainWindows ( BlSplashScreen *splash )
{
    blDebug ( "BtCompany::createMainWindows", 0 );

    /// Vamos a cargar los datos de precision y decimales.
    QString query = "SELECT  pg_attribute.atttypmod / 65536 AS decimal, pg_attribute.atttypmod - pg_attribute.atttypmod / 65536 * 65536 - 4 AS precision  FROM pg_attribute WHERE attname='cantlalbaran'";
    BlDbRecordSet *cur = loadQuery ( query );
    if ( !cur->eof() ) {
       m_decimalesCantidad = cur->valor("precision").toInt();
    } // end if
    delete cur;



    /// Establecemos el porcentaje del carga de informaci&oacute;n en las diferentes ventanas.
    /// pb = 0%
    splash->mensaje ( _ ( "Inicializando listado de articulos" ) );
    splash->setBarraProgreso ( 30 );
    m_progressbar->setValue ( 30 );

    /// Creamos un nuevo ticket vacio.
    m_ticketActual = newBtTicket();
    
    if ( !m_ticketActual )
        blDebug ( "error en el sistema, reservando memoria.", 0 );

    setTicketActual(m_ticketActual);
    listaTickets()->append(m_ticketActual);

    /// Hacemos la sincronizacioin del XML por si hubo tickets sin guardar
    QFile file ( g_confpr->valor ( CONF_DIR_USER ) + "tickets_bulmatpv.xml" );

    if (file.exists()) {
        if ( !file.open ( QIODevice::ReadOnly ) ) {
            blDebug ( "END BtCompany::syncXML", 0, "Fichero no se puede abrir" );
            return;
        } // end if
        QString result (file.readAll());
        file.close();
        syncXML(result);    
    } // end if
    
    /// Disparamos los plugins.
    int res = g_plugins->lanza ( "BtCompany_createMainWindows_Post", this );
    
    if ( res != 0 ) {
        return;
    } // end if

    cargaConf();

    /// Ponemos el titulo de la ventana
    m_bulmaTPV->statusBar() ->showMessage ( dbName(), 2000 );
   m_bulmaTPV->setWindowTitle ( g_confpr->valor ( CONF_MAIN_WINDOW_TITLE ).isEmpty() ? _( "Terminal Punto de Venta GPL" ) : g_confpr->valor ( CONF_MAIN_WINDOW_TITLE ) + " :: " + currentUser() + "@" + dbName() ); 
    /// Hacemos la comprobacion de Z
    compruebaUltimaZ();


    blDebug ( "END BtCompany::createMainWindows", 0 );
}

void BtCompany::z()
{
    blDebug( "BtCompany::z", 0 );

    QString queryfechas;
    
    if ( g_plugins->lanza ( "BtCompany_z", this ) )
        return;
    
    begin();

    /// Obtenemos fecha de la ultima Z
    QString query = "SELECT idz, fechaz FROM z ORDER BY idz DESC LIMIT 1";
    BlDbRecordSet *curFechaUltimaZ = loadQuery ( query );
    
    if ( curFechaUltimaZ->numregistros() == 0) {
        queryfechas = "SELECT DISTINCT fechaalbaran FROM albaran WHERE idz IS NULL ORDER BY fechaalbaran ASC";
    } else {
        queryfechas = "SELECT DISTINCT fechaalbaran FROM albaran WHERE idz IS NULL AND fechaalbaran >= '" + curFechaUltimaZ->valor("fechaz") + "' ORDER BY fechaalbaran ASC";
    } // end if
    
    /// Buscamos las fechas de los tickets que quedan pendientes de hacer Z
    /// Hacemos una Z por cada fecha que exista en los tickets y no tenga Z asociada
    BlDbRecordSet *curfechas = loadQuery ( queryfechas );
    
    while ( !curfechas->eof() ) {
        
        //blMsgInfo(curfechas->valor("fechaalbaran"));
        
        query = "SELECT count(idz) AS numtickets, sum(totalalbaran) AS total FROM albaran WHERE idz IS NULL AND ticketalbaran = TRUE AND fechaalbaran = '" + curfechas->valor("fechaalbaran") + "'";
        BlDbRecordSet *cur = loadQuery ( query );
        QString numtickets = cur->valor ( "numtickets" );
        QString total = cur->valor ( "total" );
        
        if ( total == "" )
            total = "0";
        
        delete cur;
        
        query = "INSERT INTO z (idalmacen, totalz, numtickets, fechaz) VALUES(" + g_confpr->valor ( CONF_IDALMACEN_DEFECTO ) + ", " + total + "," + numtickets + ", '" + curfechas->valor("fechaalbaran") + "')";
        runQuery ( query );
        query = "SELECT max(idz) AS id FROM z";
        cur = loadQuery ( query );
        QString idz = cur->valor ( "id" );
        
        delete cur;
        
        query = "UPDATE albaran SET idz = " + idz + " WHERE idz IS NULL AND ticketalbaran = TRUE AND fechaalbaran = '" + curfechas->valor("fechaalbaran") + "'";
        runQuery ( query );

        commit();
	
	m_ticketActual -> generaRML("informe_Z.txt");

	if (!g_confpr->valor ( CONF_CASHBOX_FILE).isEmpty() && g_confpr->valor ( CONF_CASHBOX_FILE) != "/dev/null") {
	    QString comando = "cat " + g_confpr->valor(CONF_DIR_USER) + "informe_Z.txt" + "  > " + g_confpr->valor ( CONF_CASHBOX_FILE );
	    system ( comando.toAscii().data() );
	} else if (g_confpr->valor(CONF_CUPS_DEFAULT_PRINTER).isEmpty() || g_confpr->valor(CONF_CUPS_DEFAULT_PRINTER) == "None") {
	    blDebug("Debe establecer el parametro CONF_CUPS_DEFAULT_PRINTER o CONF_CASHBOX_FILE para abrir el cajon " , 2);
	} else {
		blRawPrint("informe_Z.txt");
	} // end if 
	
	
/*
        QString querycont = "SELECT count(idalbaran) AS numtickets, sum(totalalbaran) as total FROM albaran WHERE idz = " + idz + " AND ticketalbaran = TRUE AND idforma_pago = " + g_confpr->valor ( CONF_IDFORMA_PAGO_CONTADO );
        BlDbRecordSet *cur1 = loadQuery ( querycont );
        QString numticketscont = cur1->valor ( "numtickets" );
        QString totalcont = cur1->valor ( "total" );
        
        if ( totalcont == "" )
            totalcont = "0";
            
        delete cur1;

        QString queryvisa = "SELECT count(idalbaran) AS numtickets, sum(totalalbaran) as total FROM albaran WHERE idz = " + idz + " AND ticketalbaran = TRUE AND idforma_pago = " + g_confpr->valor ( CONF_IDFORMA_PAGO_VISA );
        BlDbRecordSet *cur2 = loadQuery ( queryvisa );
        QString numticketsvisa = cur2->valor ( "numtickets" );
        QString totalvisa = cur2->valor ( "total" );
        
        if ( totalvisa == "" )
            totalvisa = "0";
            
        delete cur2;

    // ========================================

        QFile file ( g_confpr->valor ( CONF_TICKET_PRINTER_FILE ) );
        
        if ( !file.open ( QIODevice::WriteOnly | QIODevice::Unbuffered ) ) {
            blDebug ( "Error en la Impresion de ticket", 2 );
            return;
        } // end if
        
        file.write ( QString ( "Informe Z\n" ).toAscii() );
        file.write ( QString ( "=========\n" ).toAscii() );
        BlDbRecordSet *curemp = loadQuery ( "SELECT * FROM configuracion WHERE nombre='NombreEmpresa'" );
        
        if ( !curemp->eof() ) {
            file.write ( curemp->valor ( "valor" ).toAscii() );
            file.write ( "\n", 1 );
        } // end if
        
        delete curemp;
        
        file.write ( QString ( "====================================\n" ).toAscii() );
        cur = loadQuery ( "SELECT * FROM configuracion WHERE nombre='DireccionCompleta'" );
        
        if ( !cur->eof() ) {
            file.write ( cur->valor ( "valor" ).toAscii() );
            file.write ( "\n", 1 );
        } // end if
        
        delete cur;
        cur = loadQuery ( "SELECT * FROM configuracion WHERE nombre='CodPostal'" );
        
        if ( !cur->eof() ) {
            file.write ( cur->valor ( "valor" ).toAscii() );
        } // end if
        
        delete cur;

        file.write ( QString ( " " ).toAscii() );
        cur = loadQuery ( "SELECT * FROM configuracion WHERE nombre='Ciudad'" );
        
        if ( !cur->eof() ) {
            file.write ( cur->valor ( "valor" ).toAscii() );
            file.write ( QString ( " " ).toAscii() );
        } // end if
        
        delete cur;

        cur = loadQuery ( "SELECT * FROM configuracion WHERE nombre='Provincia'" );
        
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
        //QDate fecha = QDate::currentDate();
        //QString sfecha = fecha.toString ( "d-M-yyyy" );
        //file.write ( sfecha.toAscii() );
        file.write ( (curfechas->valor("fechaalbaran")).toAscii() );
        QTime hora = QTime::currentTime();
        QString stime = " " + hora.toString ( "HH:mm" );
        file.write ( stime.toAscii() );
        file.write ( "\n", 1 );

        /// Imprimimos el almacen
        cur = loadQuery ( "SELECT * FROM almacen WHERE idalmacen=" + g_confpr->valor ( CONF_IDALMACEN_DEFECTO ) );
        
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

        cur = loadQuery ( "SELECT * FROM familia" );
        
        while ( !cur->eof() ) {

            QString querycont = "SELECT sum(cantlalbaran) AS unidades, sum(pvpivainclalbaran * cantlalbaran) as total FROM lalbaran NATURAL LEFT JOIN articulo  WHERE idalbaran IN (SELECT idalbaran FROM albaran WHERE idz=" + idz + ")  AND idfamilia = " + cur->valor ( "idfamilia" );
            BlDbRecordSet *cur1 = loadQuery ( querycont );
            QString numticketscont = cur1->valor ( "unidades" );
            QString totalcont = cur1->valor ( "total" );
            
            if ( totalcont == "" )
                totalcont = "0";
            
            delete cur1;

            if ( totalcont != "" ) {
                file.write ( QString ( "Familia: " ).toAscii() );
                file.write ( cur->valor ( "nomfamilia" ).toAscii() );
                file.write ( "\n", 1 );

                str = "Und. Vendidas: " + numticketscont.rightJustified ( 10, ' ' );
                file.write ( str.rightJustified ( 42, ' ' ).toAscii() );
                file.write ( "\n", 1 );

                str = "Total:" + totalcont.rightJustified ( 10, ' ' );
                file.write ( str.rightJustified ( 42, ' ' ).toAscii() );
                file.write ( "\n", 1 );

                file.write ( QString ( "=======================\n" ).rightJustified ( 43, ' ' ).toAscii() );
            } // end if

            cur-> nextRecord();
            
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
*/        
        curfechas->nextRecord();
    
    } // end while

// ========================================

    blDebug( "END BtCompany::z", 0 );
}

void BtCompany::x()
{

    blDebug( "BtCompany::x", 0 );

    if ( g_plugins->lanza ( "BtCompany_x", this ) )
        return;

    m_ticketActual -> generaRML("informe_X.txt");

    if (!g_confpr->valor ( CONF_CASHBOX_FILE).isEmpty() && g_confpr->valor ( CONF_CASHBOX_FILE) != "/dev/null") {
        QString comando = "cat " + g_confpr->valor(CONF_DIR_USER) + "informe_X.txt" + "  > " + g_confpr->valor ( CONF_CASHBOX_FILE );
        system ( comando.toAscii().data() );
    } else if (g_confpr->valor(CONF_CUPS_DEFAULT_PRINTER).isEmpty() || g_confpr->valor(CONF_CUPS_DEFAULT_PRINTER) == "None") {
        blDebug("Debe establecer el parametro CONF_CUPS_DEFAULT_PRINTER o CONF_CASHBOX_FILE para abrir el cajon " , 2);
    } else {
        QString comando = "lp -d" + g_confpr->valor(CONF_CUPS_DEFAULT_PRINTER) + " " + g_confpr->valor(CONF_DIR_USER) + "informe_X.txt";
        system ( comando.toAscii().data() );
    } // end if    


/*
    QString query = "SELECT count(idalbaran) AS numtickets, sum(totalalbaran) as total FROM albaran WHERE idz IS NULL AND ticketalbaran = TRUE";
    BlDbRecordSet *cur = loadQuery ( query );
    QString numtickets = cur->valor ( "numtickets" );
    QString total = cur->valor ( "total" );
    
    if ( total == "" )
        total = "0";
    
    delete cur;

    QString querycont = "SELECT count(idalbaran) AS numtickets, sum(totalalbaran) as total FROM albaran WHERE idz IS NULL AND ticketalbaran = TRUE AND idforma_pago = " + g_confpr->valor ( CONF_IDFORMA_PAGO_CONTADO );
    BlDbRecordSet *cur1 = loadQuery ( querycont );
    QString numticketscont = cur1->valor ( "numtickets" );
    QString totalcont = cur1->valor ( "total" );
    
    if ( totalcont == "" )
        totalcont = "0";
    
    delete cur1;

    QString queryvisa = "SELECT count(idalbaran) AS numtickets, sum(totalalbaran) as total FROM albaran WHERE idz IS NULL AND ticketalbaran = TRUE AND idforma_pago = " + g_confpr->valor ( CONF_IDFORMA_PAGO_VISA );
    BlDbRecordSet *cur2 = loadQuery ( queryvisa );
    QString numticketsvisa = cur2->valor ( "numtickets" );
    QString totalvisa = cur2->valor ( "total" );
    
    if ( totalvisa == "" )
        totalvisa = "0";
    
    delete cur2;

// ========================================

    QFile file ( g_confpr->valor ( CONF_TICKET_PRINTER_FILE ) );
    
    if ( !file.open ( QIODevice::WriteOnly | QIODevice::Unbuffered ) ) {
        blDebug ( "Error en la Impresion de ticket", 2 );
        return;
    } // end if
    
    file.write ( QString ( "Informe X\n" ).toAscii() );
    file.write ( QString ( "=========\n" ).toAscii() );
    BlDbRecordSet *curemp = loadQuery ( "SELECT * FROM configuracion WHERE nombre='NombreEmpresa'" );
    
    if ( !curemp->eof() ) {
        file.write ( curemp->valor ( "valor" ).toAscii() );
        file.write ( "\n", 1 );
    } // end if
    
    delete curemp;
    
    file.write ( QString ( "====================================\n" ).toAscii() );
    cur = loadQuery ( "SELECT * FROM configuracion WHERE nombre='DireccionCompleta'" );
    
    if ( !cur->eof() ) {
        file.write ( cur->valor ( "valor" ).toAscii() );
        file.write ( "\n", 1 );
    } // end if
    
    ///file.write ( QString ( "C/LAS POZAS 181, LOCAL 43\n" ).toAscii() );
    delete cur;
    
    /// file.write ( QString ( "ALIMENTACION ECOLOGICA. HERBOLARIO\n" ).toAscii() );
    cur = loadQuery ( "SELECT * FROM configuracion WHERE nombre='CodPostal'" );
    
    if ( !cur->eof() ) {
        file.write ( cur->valor ( "valor" ).toAscii() );
    } // end if
    
    delete cur;

    file.write ( QString ( " " ).toAscii() );
    cur = loadQuery ( "SELECT * FROM configuracion WHERE nombre='Ciudad'" );
    
    if ( !cur->eof() ) {
        file.write ( cur->valor ( "valor" ).toAscii() );
        file.write ( QString ( " " ).toAscii() );
    } // end if
    
    delete cur;

    cur = loadQuery ( "SELECT * FROM configuracion WHERE nombre='Provincia'" );
    
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
    cur = loadQuery ( "SELECT * FROM almacen WHERE idalmacen=" + g_confpr->valor ( CONF_IDALMACEN_DEFECTO ) );
    
    if ( !cur->eof() ) {
        file.write ( QString ( "Almacen: " ).toAscii() );
        file.write ( cur->valor ( "nomalmacen" ).toAscii() );
        file.write ( "\n", 1 );
    } // end if
    
    delete cur;

    file.write ( "\n", 1 );
    file.write ( "\n", 1 );

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

    /// Imprimimos espacios
    file.write ( "\n \n \n \n", 7 );

    /// Imprimimos espacios
    file.write ( "\n \n \n \n \n", 9 );

    /// El corte de papel.
    file.write ( "\x1D\x56\x01", 3 );
    file.close();
*/
    blDebug( "END BtCompany::x", 0 );
}

BtTicket *BtCompany::newBtTicket()
{
    blDebug ( "BtCompany::newBtTicket", 0 );
    
    /// Lanzamos los plugins necesarios.
    BtTicket *bud;
    
    if ( g_plugins->lanza ( "BtCompany_newBtTicket", this, ( void ** ) & bud ) )
        return bud;
        
    bud = new BtTicket ( this, NULL );
    bud->setDbValue("nomticket", bud->nomTicketDefecto());
    
    blDebug ( "END BtCompany::newBtTicket", 0 );
    
    return bud;
}

void BtCompany::cobrar(bool imprimir)
{
    blDebug ( "BtCompany::cobrar", 0 );

    QString idtrabajador = m_ticketActual->dbValue ( "idtrabajador" );

    if ( g_plugins->lanza ( "BtCompany_cobrar", this ) ) {
        return;
    } // end if

    // Si el albaran existe, es que ya lo hemos sacado como ticket por la impresora
    if ( m_ticketActual->dbValue ( "idalbaran" ) != "" ) {

        if ( m_ticketActual->guardar() == -1) {
            blDebug ( "Error en la llamada a guardar()", 0 );
            return;
        }// end if

        if (imprimir || g_confpr->valor(CONF_TPV_REIMPRIMIR) == "TRUE") {
            m_ticketActual->imprimir();
        }// end if


    // Si no, guardamos e imprimimos tambien si se nos indica
    } else {
    
        if ( m_ticketActual->guardar() == -1) {
            blDebug ( "Error en la llamada a guardar()", 0 );
            return;
        }// end if
    
        if (imprimir) {
            m_ticketActual->imprimir();
        }// end if
    
    } // end if
    m_ticketActual->abrircajon();

    if ( g_plugins->lanza ( "BtCompany_cobrar_1", this ) ) {
        return;
    } // end if


    m_listaTickets.removeAt ( m_listaTickets.indexOf ( m_ticketActual ) );
    m_ticketActual = NULL;

    BtTicket *ticket = NULL;
    BtTicket *ticketv = NULL;

    /// Buscamos el ticket vacio de este trabajador y lo pintamos
    for ( int i = 0; i < m_listaTickets.size(); ++i ) {
        ticket = m_listaTickets.at ( i );

        if ( ticket->nomTicketDefecto() == ticket->dbValue ( "nomticket" ) && idtrabajador == ticket->dbValue ( "idtrabajador" ) ) {
            setTicketActual ( ticket );
            ticket->pintar();
            ticketv = ticket;
        } // end if

    } // end for

    /// Si el trabajador no tiene ticket vacio lo creamos y le ponemos el idtrabajador.
    if ( !ticketv ) {
        BtTicket * tick = newBtTicket();
        tick->setDbValue ( "idtrabajador", idtrabajador );
        setTicketActual ( tick );
        m_listaTickets.append ( tick );
        tick->pintar();
    }// end if

    g_plugins->lanza ( "BtCompany_cobrar_Post", this );
    
    blDebug ( "END BtCompany::cobrar", 0 );
}

BtTicket *BtCompany::ticketActual()
{
    blDebug ( "BtCompany::ticketActual", 0 );
    blDebug ( "END BtCompany::ticketActual", 0 );
    return m_ticketActual;
}

QList<BtTicket *> *BtCompany::listaTickets()
{
    blDebug ( "BtCompany::listaTickets", 0 );
    blDebug ( "END BtCompany::listaTickets", 0 );
    return & m_listaTickets;
}

void BtCompany::setTicketActual ( BtTicket *tick )
{
    blDebug ( "BtCompany::setTicketActual", 0 );
    g_plugins->lanza ( "BtCompany_setTicketActual", this );
    m_ticketActual = tick;
    g_plugins->lanza ( "BtCompany_setTicketActual_Post", this );
    blDebug ( "END BtCompany::setTicketActual", 0 );
}

/// Guarda la configuracion de programa para poder recuperar algunas cosas de presentacion.
/**
**/
void BtCompany::guardaConf()
{
    blDebug ( "Company::guardaConf", 0 );
    QFile file ( g_confpr->valor ( CONF_DIR_USER ) + "bulmatpv_" + dbName() + ".cfn" );
    /// Guardado del orden y de configuraciones varias.
    if ( file.open ( QIODevice::WriteOnly ) ) {
        QTextStream stream ( &file );
        stream << "<CONFIG>\n";
        stream << "\t<PRINCIPAL>\n";
        stream << "\t\t\t<X>" + QString::number ( m_bulmaTPV->x() ) + "</X>\n";
        stream << "\t\t\t<Y>" + QString::number ( m_bulmaTPV->y() ) + "</Y>\n";
        stream << "\t\t\t<WIDTH>" + QString::number ( m_bulmaTPV->width() ) + "</WIDTH>\n";
        stream << "\t\t\t<HEIGHT>" + QString::number ( m_bulmaTPV->height() ) + "</HEIGHT>\n";
        stream << "\t\t\t<TOOLBARSDOCKWIDGETS>" + QString ( m_bulmaTPV->saveState().toBase64() ) + "</TOOLBARSDOCKWIDGETS>\n";
        stream << "\t</PRINCIPAL>\n";
        stream << "</CONFIG>\n";
        file.close();
    } // end if
    blDebug ( "END BfCompany::guardaConf", 0 );
}

/// Guarda la configuracion de programa para poder recuperar algunas cosas de presentacion.
/**
\return
**/
void BtCompany::cargaConf()
{
    blDebug ( "BtCompany::cargaConf", 0 );
    
    QFile file ( g_confpr->valor ( CONF_DIR_USER ) + "bulmatpv_" + dbName() + ".cfn" );
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
    m_bulmaTPV->restoreState ( QByteArray::fromBase64 ( QByteArray ( principal.firstChildElement ( "TOOLBARSDOCKWIDGETS" ).toElement().text().toAscii() ) ) );

    blDebug ( "END BtCompany::cargaConf", 0 );
}

void BtCompany::compruebaUltimaZ()
{
    blDebug ( "BtCompany::compruebaUltimaZ", 0 );
        
    // Obtenemos numero de Zs hasta el momento (para saber si es superior a 0. Si no, estamos en
    // el caso de que es la primera Z)
    QString query = "SELECT COUNT(idz) AS numzetas FROM z";
    BlDbRecordSet *curNumzetas = loadQuery ( query );
    
    if ( (curNumzetas->valor("numzetas")).toInt() > 0) {
    
        // Obtenemos fecha de la ultima Z
        query = "SELECT idz, fechaz FROM z ORDER BY idz DESC LIMIT 1";
        BlDbRecordSet *curFechaUltimaZ = loadQuery ( query );
    
        // Obtenemos fecha actual
        query = "SELECT current_date AS fecha";
        BlDbRecordSet *curFechaActual = loadQuery ( query );
        
        // Obtenemos diferencia en dias entre la fecha de la ultima Z y la fecha actual
        query = "SELECT to_date('" + curFechaActual->valor("fecha") + "','dd/mm/yyyy') - to_date('" + curFechaUltimaZ->valor("fechaz") + "','dd/mm/yyyy') AS diferencia;";
        BlDbRecordSet *curDiferencia = loadQuery ( query );
                
        if ( (curDiferencia->valor("diferencia")).toInt() > 1 ) {
        
            // Comprobamos si hay tickets pendientes desde la ultima Z
            query = "SELECT COUNT(idalbaran) AS numtickets FROM albaran where ticketalbaran = TRUE AND idz IS NULL AND fechaalbaran > '" + curFechaUltimaZ->valor("fechaz") + "'";
            BlDbRecordSet *curNumtickets = loadQuery ( query );
            
            if ( (curNumtickets->valor("numtickets")).toInt() > 1 ) {

                QMessageBox msgBox(this);
                QAbstractButton *si = (QAbstractButton *) msgBox.addButton("Si", QMessageBox::ActionRole);
                QAbstractButton *no = (QAbstractButton *) msgBox.addButton("No", QMessageBox::ActionRole);
                msgBox.setEscapeButton(no);
                msgBox.setWindowTitle("Inconsistencia en Zetas");
                msgBox.setText("La fecha de la ultima Zeta es " + curFechaUltimaZ->valor("fechaz") + " y la fecha actual es " + curFechaActual->valor("fecha") + ". Hay tickets entre este periodo que tienen pendiente asociarse a una Zeta. ¿Desea hacer las Zetas necesarias para los tickets pendientes?");
                msgBox.exec();
                
                if (msgBox.clickedButton() == si) {
                    z();

/// Es demasiado duro salir del programa sin dar la opcion de continuar. Ademas la comprobacion esta mal ya que deberia tener en cuenta si el proximo ticket
/// Que se va a generar esta dentro del rango de fechas o no.
//		} else {
//                    exit(0);
                } // end if
            } // end if
                    
        }// end if
        
        /// Diferencia negativa de fechas (algo esta mal en la fecha del distema
        /// o ya se hizo alguna Z con la fecha mal establecida en este)
        if ( (curDiferencia->valor("diferencia")).toInt() < 0 ) {
        
            blMsgInfo("La fecha de la ultima Zeta es " + curFechaUltimaZ->valor("fechaz") + " y la fecha actual es " + curFechaActual->valor("fecha") + ". Parece que hay un problema con la fecha del sistema ya que no es coherente con la fecha de la ultima Zeta (el programa no se podra seguir usando hasta no resolver este problema)");
            exit(0);
                    
        } // end if
        
        delete curFechaUltimaZ;
        delete curFechaActual;
        delete curDiferencia;
    
    } // end if
    
    delete curNumzetas;
    
    blDebug ( "END BtCompany::compruebaUltimaZ", 0 );
}

/// Hace la exportacion del campo a XML
QString BtCompany::exportXML() {
    blDebug ( "BtCompany::exportXML", 0 );
    QString val;
    int error;
    BlDbField *campo;

    val = "<BTCOMPANY>\n";
    val += "\t<LISTATICKETS>\n";
    BtTicket *linea1;
    for ( int i = 0; i < m_listaTickets.size(); ++i ) {
        linea1 = m_listaTickets.at ( i );
        val += "\t\t" + linea1->exportXML().replace("\t<","\t\t\t<").replace("\n<","\n\t\t<");
    } // end for
    val += "\t</LISTATICKETS>\n";
    val += "</BTCOMPANY>\n";

    return val;
    blDebug ( "END BtCompany::exportXML", 0 );
}


/// Sincroniza la lista de tickets con los guardados en el xml pasado.
/// Creando los que no estan y modificando los que estan.
void BtCompany::syncXML(const QString &textxml) {
    blDebug ( "BtCompany::syncXML", 0 );


    QDomDocument doc ( "mydocument" );
    if ( !doc.setContent ( textxml ) ) {
        blDebug ( "END BtCompany::syncXML", 0, "XML Invalido" );
        return;
    } // end if


    QDomElement docElem = doc.documentElement();
    QDomElement principal = docElem.firstChildElement ( "LISTATICKETS" );
    /// Cogemos la coordenada X
    QDomNodeList nodos = principal.elementsByTagName ( "BTTICKET" );
    int i = 0;
    while (i < nodos.count() ) {

        bool sincronizado = 0; /// Indica si el nodo leido ya ha sido sincronizado o no.
        QDomNode ventana = nodos.item ( i++ );
        QDomElement e1 = ventana.toElement(); /// try to convert the node to an element.
        if ( !e1.isNull() ) { /// the node was really an element.
            QString nodoText = e1.text();
            /// Pasamos el XML a texto para poder procesarlo como tal.
            QString result;
            QTextStream stream ( &result );
            ventana.save(stream,5);

            /// El ticket en blanco no se sincroniza
            QString nomticket = e1.firstChildElement("NOMTICKET").text();
            if (nomticket != "" && nomticket != ticketActual()->dbValue("nomticket")) {

               /// Para cada elemento de la lista de tickets intentamos sincronizar el ticket. (Basandonos en el id y en el nombre de mesa).
	       /// Excepto el ticket actual que no debemos tocarlo ya que esta en funcionamiento.
               BtTicket *linea1;
               int x = 0;
               while ( x < m_listaTickets.size() - 1 && !sincronizado) {
                   linea1 = m_listaTickets.at ( x++ );
		   sincronizado = (linea1->syncXML(result, false));
               } // end while

            /// Si ho ha habido sincronizacion con ningun elemento
            /// Creamos un ticket nuevo y lo sincronizamos.
             if (!sincronizado) {
        	 /// Hacemos la sincronizacion forzada ya que no ha habido coincidencias anteriores.
                 BtTicket *tick = newBtTicket();
                 tick->setDbValue ( "idtrabajador", ticketActual() ->dbValue ( "idtrabajador" ) );
                 tick->syncXML(result, true);
                 listaTickets() ->prepend ( tick );
             } // end if
           } // end if
        } // end if
    } // end while


    /// Borramos los nodos que no aparecen en la sincronizacion ya que al parecer se deben haber borrado.
    int x = 0;
    while (x < m_listaTickets.size() -1 ) {
        bool encontrado = FALSE;
	BtTicket *linea1 = m_listaTickets.at(x);
	int i = 0;
	while (i < nodos.count() && !encontrado) {
	    QString nombre1 = nodos.item(i++).toElement().firstChildElement("NOMTICKET").text();
	    if (nombre1 == linea1->dbValue("nomticket")) {
		encontrado = TRUE;
	    } // end if
	} // end while
	
	if (!encontrado) {
	    m_listaTickets.removeAt(x);
	} else {
	    x++;
	} // end if
    } // end while

    blDebug ( "BtCompany::syncXML", 0 );
}



void BtCompany::joinTickets(BtTicket *origin, BtTicket *destiny)
{
      /// TODO:
      /// Se ha imprimido?
      /// Se ha creado un albaran? se tiene que borrar el albaran?

    if ( QMessageBox::warning ( this,
                                _ ( "Unir tickets" ),
                                QString(_("Unir ticket '")) + origin->dbValue ( "nomticket" ) + QString(_("' en '")) + destiny->dbValue ( "nomticket" ) + QString(_("'?")),
                                QMessageBox::Ok,
                                QMessageBox::Cancel ) == QMessageBox::Ok ) {
              
	/// Transpasa todas las lineas al ticket destino.
	while (!origin->listaLineas()->isEmpty()) {
	    destiny->listaLineas()->append( origin->listaLineas()->takeFirst() );
	} // while

	/// Elimina el ticket original.
	m_listaTickets.removeAll(origin);
    } // end if      

}

