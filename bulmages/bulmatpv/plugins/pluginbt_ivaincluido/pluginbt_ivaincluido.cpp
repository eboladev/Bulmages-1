/***************************************************************************
 *   Copyright (C) 2005 by Tomeu Borras Riera                              *
 *   tborras@conetxia.com                                                  *
 *                                                                         *
 *   Copyright (C) 2006 by Fco. Javier M. C.                               *
 *   fcojavmc@todo-redes.com                                               *
 *                                                                         *
 *   Copyright (C) 2009 by Arturo Martin Llado                             *
 *   amartin@conetxia.com                                                  *
 *   http://www.iglues.org                                                 *
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

#include <QDockWidget>

#include "pluginbt_ivaincluido.h"
#include "blfunctions.h"
#include "btcompany.h"
#include "btescprinter.h"

typedef QMap<QString, BlFixed> base;

int BtTicket_agregarLinea_Post ( BtTicket *tick )
{
    _depura ( "PluginBt_IvaIncluido::BtTicket_agregarLinea_Post", 0 );
    
    BlDbRecord *item = (BlDbRecord *) g_plugParams;

    item->addDbField ( "pvpivainclalbaran", BlDbField::DbInt, BlDbField::DbNothing, _( "IVA inc." ) );
    
    _depura ( "END PluginBt_IvaIncluido::BtTicket_agregarLinea_Post", 0 );
    
    return 0;
}

int BtTicket_insertarArticulo_Post ( BtTicket *tick )
{

    _depura ( "PluginBt_IvaIncluido::BtTicket_insertarArticulo_Post", 0 );
    
    int valor = -1;
    static int semaforo = 0;
    
    if ( semaforo == 0 ) {
        semaforo = 1;

        /// Buscamos los parametros en la base de datos.
        QString query = "SELECT pvpivaincarticulo FROM articulo WHERE idarticulo = " + tick->lineaActBtTicket()->dbValue("idarticulo");
        BlDbRecordSet *cur = tick->mainCompany() ->loadQuery ( query );
        if ( !cur->eof() ) {
            tick->lineaActBtTicket()->setDbValue ( "pvpivainclalbaran", cur->valor ( "pvpivaincarticulo" ) );

        } // end if
        delete cur;

        valor = 0;
        semaforo = 0;
    } // end if
    
    _depura ( "END PluginBt_IvaIncluido::BtTicket_insertarArticulo_Post", 0 );

    return valor;
}

int BtTicket_ponerPrecio_Post ( BtTicket *tick ) {
    _depura ( "PluginBt_IvaIncluido::BtTicket_ponerPrecio_Post", 0 );
	
    tick->lineaActBtTicket()->setDbValue("pvpivainclalbaran", tick->lineaActBtTicket()->dbValue("pvplalbaran"));
    
    _depura ( "END PluginBt_IvaIncluido::BtTicket_ponerPrecio_Post", 0 );
    return 0;
}

int BtTicket_imprimir(BtTicket *tick)
{
    _depura ( "PluginBt_IvaIncluido::BtTicket_imprimir", 0 );
    
    struct empresastr {
        QString nombre;
        QString direccionCompleta;
        QString codigoPostal;
        QString ciudad;
        QString provincia;
        QString telefono;
    } empresa;

    struct clientestr {
        QString cif;
        QString nombre;
    } cliente;

    struct trabajadorstr {
        QString nombre;
        QString id;
    } trabajador;

    struct almacenstr {
        QString nombre;
    } almacen;

    struct fechastr {
        QString dia;
        QString hora;
    } fecha;

    struct totalstr {
        BlFixed iva;
        BlFixed baseImponible;
        BlFixed totalIva;
    } total;

    BlDbRecordSet *cur = tick->mainCompany() ->loadQuery ( "SELECT * FROM configuracion WHERE nombre='NombreEmpresa'" );
    if ( !cur->eof() )
        empresa.nombre = cur->valor ( "valor" );
    delete cur;

    cur = tick->mainCompany() ->loadQuery ( "SELECT * FROM configuracion WHERE nombre='CIF'" );
    if ( !cur->eof() )
        empresa.nombre += "\n" + cur->valor ( "valor" );
    delete cur;

    cur = tick->mainCompany() ->loadQuery ( "SELECT * FROM configuracion WHERE nombre='DireccionCompleta'" );
    if ( !cur->eof() )
        empresa.direccionCompleta = cur->valor ( "valor" );
    delete cur;

    cur = tick->mainCompany() ->loadQuery ( "SELECT * FROM configuracion WHERE nombre='CodPostal'" );
    if ( !cur->eof() )
        empresa.codigoPostal = cur->valor ( "valor" ).toAscii();
    delete cur;

    cur = tick->mainCompany() ->loadQuery ( "SELECT * FROM configuracion WHERE nombre='Ciudad'" );
    if ( !cur->eof() )
        empresa.ciudad = cur->valor ( "valor" );
    delete cur;

    cur = tick->mainCompany() ->loadQuery ( "SELECT * FROM configuracion WHERE nombre='Telefono'" );
    if ( !cur->eof() )
        empresa.telefono = cur->valor ( "valor" );
    delete cur;

    cur = tick->mainCompany() ->loadQuery ( "SELECT * FROM configuracion WHERE nombre='Provincia'" );
    if ( !cur->eof() )
        empresa.provincia = cur->valor ( "valor" );
    delete cur;

    fecha.dia = QDate::currentDate().toString ( "d-M-yyyy" );
    fecha.hora = QTime::currentTime().toString ( "HH:mm" );

    trabajador.id = tick->dbValue ( "idtrabajador" );
    cur = tick->mainCompany() ->loadQuery ( "SELECT * FROM trabajador WHERE idtrabajador=" + tick->dbValue ( "idtrabajador" ) );
    if ( !cur->eof() )
        trabajador.nombre = cur->valor ( "nomtrabajador" );
    delete cur;

    cur = tick->mainCompany() ->loadQuery ( "SELECT * FROM cliente WHERE idcliente=" + tick->dbValue ( "idcliente" ) );
    if ( !cur->eof() ) {
	if (cur->valor ("idcliente") != g_confpr->valor(CONF_IDCLIENTE_DEFECTO)) {
        cliente.cif = cur->valor ( "cifcliente" ).toAscii();
        cliente.nombre = cur->valor ( "nomcliente" ).toAscii();
	} else {
	cliente.cif = "";
        cliente.nombre = "";
	} // end if
    } // end if
    delete cur;

    cur = tick->mainCompany() ->loadQuery ( "SELECT * FROM almacen WHERE idalmacen=" + tick->dbValue ( "idalmacen" ) );
    if ( !cur->eof() )
        almacen.nombre = cur->valor ( "nomalmacen" ).toAscii() ;
    delete cur;

    BlDbRecord *linea;
    base totales;

    /// Inicializamos los componentes.
    for ( int i = 0; i < tick->listaLineas() ->size(); ++i ) {
        linea = tick->listaLineas() ->at ( i );
	    BlFixed init("0.00");
        totales[linea->dbValue ( "ivalalbaran" ) ] = init;
    } // end for

    for ( int i = 0; i < tick->listaLineas() ->size(); ++i ) {
        linea = tick->listaLineas() ->at ( i );
        BlFixed cantidad = BlFixed ( linea->dbValue ( "cantlalbaran" ) );
	    BlFixed totlinea = cantidad * BlFixed( linea->dbValue("pvpivainclalbaran"));
        total.totalIva = total.totalIva + cantidad * BlFixed ( linea->dbValue ( "pvpivainclalbaran" ) );
        totales[linea->dbValue ( "ivalalbaran" ) ] = totales[linea->dbValue ( "ivalalbaran" ) ] + totlinea;
    } // end for

    BtEscPrinter pr ( g_confpr->valor(CONF_DIR_USER) + "bulmatpv_ticket.txt" );
    pr.initializePrinter();
    pr.setCharacterCodeTable ( page19 );
    pr.setJustification ( BtEscPrinter::center );

    if ( g_confpr->valor ( CONF_TPV_PRINTER_LOGO ) != "" ) {
        pr.printImage ( g_confpr->valor ( CONF_TPV_PRINTER_LOGO ) );
    } // end if
    
    pr.printText ( empresa.nombre + "\n" );
    pr.setCharacterPrintMode ( CHARACTER_FONTB_SELECTED );
    pr.setCharacterSize ( CHAR_WIDTH_1 | CHAR_HEIGHT_1 );
    pr.setColor ( red );
    pr.printText ( empresa.direccionCompleta + "\n" );
    pr.initializePrinter();
    pr.setCharacterCodeTable ( page19 );

    pr.printText ( "\n" );
    pr.printText ( fecha.dia + " " + fecha.hora + "\n" );
    if (cliente.cif != "") 
	pr.printText ( "Cliente: " + cliente.cif + " " + cliente.nombre + "\n" );
    pr.printText ( "Num. Ticket:  " + tick->dbValue("numalbaran") + "\n" );

    pr.printText ( "\n" );

    pr.turnWhiteBlack ( 1 );
    pr.printText ( "Uds PRODUCTO � � � � � � � P.U. � IMPORTE \n" );

    pr.turnWhiteBlack ( 0 );
    pr.setCharacterPrintMode ( CHARACTER_FONTB_SELECTED );
    pr.setCharacterSize ( CHAR_WIDTH_1 | CHAR_HEIGHT_1 );

    for ( int i = 0; i < tick->listaLineas() ->size(); ++i ) {
        
        if ( i == tick->listaLineas()->size() - 1 )
            pr.setUnderlineMode ( 1 );
        
        linea = tick->listaLineas()->at ( i );
        //BlFixed iva = BlFixed ( linea->dbValue ( "ivalalbaran" ) );
        BlFixed pvp = BlFixed ( linea->dbValue ( "pvpivainclalbaran" ) );
        //pvp = pvp + pvp * iva / BlFixed ( "100" );
        BlFixed pvptotal = BlFixed ( linea->dbValue ( "cantlalbaran" ) ) * pvp;
        pr.printText ( linea->dbValue ( "cantlalbaran" ).rightJustified ( 5, ' ', TRUE ) + " " );
        pr.printText ( linea->dbValue ( "desclalbaran" ).leftJustified ( 27, ' ', TRUE ) + " " );
        QString pvpstr = pvp.toQString();
        QString pvptotalstr = pvptotal.toQString();
        pr.printText ( QString ( pvpstr + "�" ).rightJustified ( 10, ' ', TRUE ) + " " );
        pr.printText ( QString ( pvptotalstr + "�" ).rightJustified ( 10, ' ', TRUE ) );
        pr.printText ( "\n" );
    }
    
    pr.setUnderlineMode ( 0 );
    pr.setJustification ( BtEscPrinter::right );
    pr.setCharacterPrintMode ( CHARACTER_FONTA_SELECTED );    

    base::Iterator it;
    
    for ( it = totales.begin(); it != totales.end(); ++it ) {
		QString tipoIva = it.key();
		
		QString sqlquery = "SELECT (" +it.value().toQString('.') + "/ ( 1 + " + tipoIva.replace(",",".") + "/100 ))::NUMERIC(12,2) AS base, " + it.value().toQString('.') + "- ("+it.value().toQString('.') + "/ ( 1 + " + tipoIva.replace(",",".") + "/100 ))::NUMERIC(12,2) AS iva";
		BlDbRecordSet *cur = tick->mainCompany()->loadQuery(sqlquery);
	    pr.printText ( "Base Imponible: " + cur-> valor("base") + "�\n" );
    	pr.printText ( "IVA " +it.key() + "%  " + cur->valor("iva") + "�\n" );
		delete cur;
    } // end for

    pr.setCharacterPrintMode ( CHARACTER_FONTA_SELECTED | EMPHASIZED_MODE | DOUBLE_HEIGHT | DOUBLE_WIDTH );
    pr.printText ( "TOTAL: " + total.totalIva.toQString() + "�\n" );
    pr.printText ( "\n\n" );
    pr.setJustification ( BtEscPrinter::left );
    pr.setCharacterPrintMode ( CHARACTER_FONTA_SELECTED );
    pr.printText ( "Le ha atendido " + trabajador.nombre + "\n" );
    pr.printText ( "\n" );


    pr.printText ( "\n" );

    pr.setJustification ( BtEscPrinter::center );
    pr.setColor ( red );
    pr.printText ( "*** GRACIAS POR SU VISITA ***\n" );

    QByteArray qba = tick->dbValue ( "refalbaran" ).toAscii();
    char* barcode = qba.data();
    pr.setJustification ( BtEscPrinter::center );
    pr.setBarcodeFormat ( 2, 50, both, fontB );
    pr.printBarCode ( code39, qba.size(), barcode );
    pr.cutPaperAndFeed ( TRUE, 10 );
    pr.print();



/// Si queremos imprimir con CUPS lo hacemos de esta otra forma
    if (!g_confpr->valor ( CONF_TICKET_PRINTER_FILE).isEmpty() && g_confpr->valor ( CONF_TICKET_PRINTER_FILE) != "/dev/null") {
        QString comando = "cat " + g_confpr->valor(CONF_DIR_USER) + "bulmatpv_ticket.txt" + "  > " + g_confpr->valor ( CONF_TICKET_PRINTER_FILE );
        system ( comando.toAscii().data() );
    } else if (g_confpr->valor(CONF_CUPS_DEFAULT_PRINTER).isEmpty() || g_confpr->valor(CONF_CUPS_DEFAULT_PRINTER) == "None") {
        _depura("Debe establecer el parametro CONF_CUPS_DEFAULT_PRINTER o CONF_TICKET_PRINTER_FILE para imprimir el ticket " , 2);
    } else {
        QString comando = "cupsdoprint -P" + g_confpr->valor(CONF_CUPS_DEFAULT_PRINTER) + " " + g_confpr->valor(CONF_DIR_USER) + "bulmatpv_ticket.txt";
        system ( comando.toAscii().data() );
    } // end if

    _depura ( "END PluginBt_IvaIncluido::BtTicket_imprimir", 0 );    
    return 1;
}

int BtCompany_z(BtCompany * emp)
{
    _depura ( "PluginBt_IvaIncluido::BtCompany_z", 0 );
    
    QString queryfechas;
    
    emp->begin();
    
    /// Obtenemos fecha de la ultima Z
    QString query = "SELECT idz, fechaz FROM z ORDER BY idz DESC LIMIT 1";
    BlDbRecordSet *curFechaUltimaZ = emp->loadQuery ( query );

    if ( curFechaUltimaZ->numregistros() == 0) {
        queryfechas = "SELECT DISTINCT fechaalbaran FROM albaran WHERE idz IS NULL ORDER BY fechaalbaran ASC";
    } else {
        queryfechas = "SELECT DISTINCT fechaalbaran FROM albaran WHERE idz IS NULL AND fechaalbaran > '" + curFechaUltimaZ->valor("fechaz") + "' ORDER BY fechaalbaran ASC";
    } // end if

    /// Buscamos las fechas de los tickets que quedan pendientes de hacer Z
    /// Hacemos una Z por cada fecha que exista en los tickets y no tenga Z asociada
    BlDbRecordSet *curfechas = emp->loadQuery ( queryfechas );
    
    while ( !curfechas->eof() ) {
        //mensajeInfo(curfechas->valor("fechaalbaran"));
        
        query = "INSERT INTO z (idalmacen) VALUES(" + g_confpr->valor ( CONF_IDALMACEN_DEFECTO ) + ")";
        emp->runQuery ( query );
        
        query = "SELECT max(idz) AS id FROM z";
        BlDbRecordSet *cur = emp->loadQuery ( query );
        QString idz = cur->valor ( "id" );
        delete cur;
        
        query = "UPDATE albaran SET idz = " + idz + " WHERE idz IS NULL AND ticketalbaran = TRUE AND fechaalbaran = '" + curfechas->valor("fechaalbaran") + "'";
        emp->runQuery ( query );
        
        query = "SELECT count(idz) AS numtickets, sum(totalalbaran) AS total FROM albaran WHERE idz = " + idz;
        cur = emp->loadQuery ( query );
        
        QString numtickets = cur->valor ( "numtickets" );
        QString total = cur->valor ( "total" );

        if ( total == "" )
            total = "0";
        
        query = "UPDATE z SET totalz = " + total + ", numtickets = " + numtickets + ", fechaz = '" + curfechas->valor("fechaalbaran") + "' WHERE idz = " + idz;
        emp->runQuery ( query );
        emp->commit();
        
        delete cur;

        QString querycont = "SELECT count(idalbaran) AS numtickets, sum(totalalbaran) as total FROM albaran WHERE idz = " + idz + " AND ticketalbaran = TRUE AND idforma_pago = " + g_confpr->valor ( CONF_IDFORMA_PAGO_CONTADO );
        BlDbRecordSet *cur1 = emp->loadQuery ( querycont );
        QString numticketscont = cur1->valor ( "numtickets" );
        QString totalcont = cur1->valor ( "total" );
        
        if ( totalcont == "" )
            totalcont = "0";
        
        delete cur1;

        QString queryvisa = "SELECT count(idalbaran) AS numtickets, sum(totalalbaran) as total FROM albaran WHERE idz = " + idz + " AND ticketalbaran = TRUE AND idforma_pago = " + g_confpr->valor(CONF_IDFORMA_PAGO_VISA);

        BlDbRecordSet *cur2 = emp->loadQuery ( queryvisa );
        QString numticketsvisa = cur2->valor ( "numtickets" );
        QString totalvisa = cur2->valor ( "total" );

        if ( totalvisa == "" )
            totalvisa = "0";
        
        delete cur2;

        // ========================================

        QFile file (  g_confpr->valor(CONF_DIR_USER) + "bulmatpv_z.txt" );
        if ( !file.open ( QIODevice::WriteOnly | QIODevice::Unbuffered ) ) {
            _depura ( "Error en la Impresion de ticket", 2 );
            return -1;
        } // end if
        
        file.write ( QString ( "Informe Z\n" ).toAscii() );
        file.write ( QString ( "=========\n" ).toAscii() );
        
        BlDbRecordSet *curemp = emp->loadQuery ( "SELECT * FROM configuracion WHERE nombre='NombreEmpresa'" );
        if ( !curemp->eof() ) {
            file.write ( curemp->valor ( "valor" ).toAscii() );
            file.write ( "\n", 1 );
        } // end if
        delete curemp;
        
        file.write ( QString ( "====================================\n" ).toAscii() );
        
        cur = emp->loadQuery ( "SELECT * FROM configuracion WHERE nombre='DireccionCompleta'" );
        if ( !cur->eof() ) {
            file.write ( cur->valor ( "valor" ).toAscii() );
            file.write ( "\n", 1 );
        } // end if
        delete cur;

        cur = emp->loadQuery ( "SELECT * FROM configuracion WHERE nombre='CodPostal'" );
        if ( !cur->eof() ) {
            file.write ( cur->valor ( "valor" ).toAscii() );
        } // end if
        delete cur;

        file.write ( QString ( " " ).toAscii() );
        
        cur = emp->loadQuery ( "SELECT * FROM configuracion WHERE nombre='Ciudad'" );
        if ( !cur->eof() ) {
            file.write ( cur->valor ( "valor" ).toAscii() );
            file.write ( QString ( " " ).toAscii() );
        } // end if
        delete cur;

        cur = emp->loadQuery ( "SELECT * FROM configuracion WHERE nombre='Provincia'" );
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
        cur = emp->loadQuery ( "SELECT * FROM almacen WHERE idalmacen=" + g_confpr->valor ( CONF_IDALMACEN_DEFECTO ) );
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
        // ============================================
        
        file.write ( QString ( "=======================\n" ).rightJustified ( 43, ' ' ).toAscii() );
        file.write ( QString ( "=======================\n" ).rightJustified ( 43, ' ' ).toAscii() );
        file.write ( QString ( "==== RESUMEN FAMILIAS ======\n" ).rightJustified ( 43, ' ' ).toAscii() );
        
        // Informes por familias

        /// Imprimimos el almacen
        cur = emp->loadQuery ( "SELECT * FROM familia");
        
        while ( !cur->eof() ) {
            file.write ( QString ( "Familia: " ).toAscii() );
            file.write (cur->valor ( "nombrefamilia" ).toAscii() );
            file.write ( "\n", 1 );

            QString querycont = "SELECT sum(cantlalbaran) AS unidades, sum(pvpivainclalbaran * cantlalbaran)::NUMERIC(12,2) as total FROM lalbaran NATURAL LEFT JOIN articulo WHERE idalbaran IN (SELECT idalbaran FROM albaran WHERE idz="+idz+")  AND idfamilia = " + cur->valor("idfamilia");
            BlDbRecordSet *cur1 = emp->loadQuery ( querycont );
            QString numticketscont = cur1->valor ( "unidades" );
            QString totalcont = cur1->valor ( "total" );
            
            if ( totalcont == "" )
                totalcont = "0";
            
            delete cur1;

            str = "Und. Vendidas: " + numticketscont.rightJustified ( 10, ' ' );
            file.write ( str.rightJustified ( 42, ' ' ).toAscii() );
            file.write ( "\n", 1 );
        
            str = "Total:" + totalcont.rightJustified ( 10, ' ' );
            file.write ( str.rightJustified ( 42, ' ' ).toAscii() );
            file.write ( "\n", 1 );

            file.write ( QString ( "=======================\n" ).rightJustified ( 43, ' ' ).toAscii() );

            cur-> nextRecord();
        } // end while
        
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
    
        curfechas->nextRecord();
	if (!g_confpr->valor ( CONF_TICKET_PRINTER_FILE).isEmpty() && g_confpr->valor ( CONF_TICKET_PRINTER_FILE) != "/dev/null") {
	    QString comando = "cat " + g_confpr->valor(CONF_DIR_USER) + "bulmatpv_z.txt" + "  > " + g_confpr->valor ( CONF_TICKET_PRINTER_FILE );
	    system ( comando.toAscii().data() );
	} else if (g_confpr->valor(CONF_CUPS_DEFAULT_PRINTER).isEmpty() || g_confpr->valor(CONF_CUPS_DEFAULT_PRINTER) == "None") {
	    _depura("Debe establecer el parametro CONF_CUPS_DEFAULT_PRINTER o CONF_TICKET_PRINTER_FILE para abrir el cajon " , 2);
	} else {
	    QString comando = "cupsdoprint -P" + g_confpr->valor(CONF_CUPS_DEFAULT_PRINTER) + " " + g_confpr->valor(CONF_DIR_USER) + "bulmatpv_z.txt";
	    system ( comando.toAscii().data() );
	} // end if   
    } // end while


    _depura ( "END PluginBt_IvaIncluido::BtCompany_z", 0 );

    return -1;
}

int BtCompany_x(BtCompany *emp)
{
    _depura ( "PluginBt_IvaIncluido::BtCompany_x", 0 );
    
    QString query = "SELECT count(idalbaran) AS numtickets, sum(totalalbaran) as total FROM albaran WHERE idz IS NULL AND ticketalbaran = TRUE";
    BlDbRecordSet *cur = emp->loadQuery ( query );
    QString numtickets = cur->valor ( "numtickets" );
    QString total = cur->valor ( "total" );
    
    if ( total == "" )
        total = "0";
    
    delete cur;

    QString querycont = "SELECT count(idalbaran) AS numtickets, sum(totalalbaran) as total FROM albaran WHERE idz IS NULL AND ticketalbaran = TRUE AND idforma_pago = " + g_confpr->valor ( CONF_IDFORMA_PAGO_CONTADO );
    BlDbRecordSet *cur1 = emp->loadQuery ( querycont );
    QString numticketscont = cur1->valor ( "numtickets" );
    QString totalcont = cur1->valor ( "total" );
    
    if ( totalcont == "" )
        totalcont = "0";
    
    delete cur1;

    QString queryvisa = "SELECT count(idalbaran) AS numtickets, sum(totalalbaran) as total FROM albaran WHERE idz IS NULL AND ticketalbaran = TRUE AND idforma_pago = "+ g_confpr->valor(CONF_IDFORMA_PAGO_VISA);

    BlDbRecordSet *cur2 = emp->loadQuery ( queryvisa );
    QString numticketsvisa = cur2->valor ( "numtickets" );
    QString totalvisa = cur2->valor ( "total" );
    
    if ( totalvisa == "" )
        totalvisa = "0";
    
    delete cur2;

// ========================================

    QFile file ( g_confpr->valor(CONF_DIR_USER) + "bulmatpv_x.txt" );
    if ( !file.open ( QIODevice::WriteOnly | QIODevice::Unbuffered ) ) {
        _depura ( "Error en la Impresion de ticket", 2 );
        return -1;
    } // end if
    
    file.write ( QString ( "Informe X\n" ).toAscii() );
    file.write ( QString ( "=========\n" ).toAscii() );
    
    BlDbRecordSet *curemp = emp->loadQuery ( "SELECT * FROM configuracion WHERE nombre='NombreEmpresa'" );
    if ( !curemp->eof() ) {
        file.write ( curemp->valor ( "valor" ).toAscii() );
        file.write ( "\n", 1 );
    } // end if
    delete curemp;
    
    file.write ( QString ( "====================================\n" ).toAscii() );
    
    cur = emp->loadQuery ( "SELECT * FROM configuracion WHERE nombre='DireccionCompleta'" );
    if ( !cur->eof() ) {
        file.write ( cur->valor ( "valor" ).toAscii() );
        file.write ( "\n", 1 );
    } // end if
    delete cur;

    cur = emp->loadQuery ( "SELECT * FROM configuracion WHERE nombre='CodPostal'" );
    if ( !cur->eof() ) {
        file.write ( cur->valor ( "valor" ).toAscii() );
    } // end if
    delete cur;

    file.write ( QString ( " " ).toAscii() );
    
    cur = emp->loadQuery ( "SELECT * FROM configuracion WHERE nombre='Ciudad'" );
    if ( !cur->eof() ) {
        file.write ( cur->valor ( "valor" ).toAscii() );
        file.write ( QString ( " " ).toAscii() );
    } // end if
    delete cur;

    cur = emp->loadQuery ( "SELECT * FROM configuracion WHERE nombre='Provincia'" );
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
    cur = emp->loadQuery ( "SELECT * FROM almacen WHERE idalmacen=" + g_confpr->valor ( CONF_IDALMACEN_DEFECTO ) );
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

/// Si quisiesemos imprimir en modo raw lo hariamos de esta forma.
//    QString comando = "cat " + g_confpr->valor(CONF_DIR_USER) + "bulmatpv_x.txt" + "  > " + g_confpr->valor ( CONF_TICKET_PRINTER_FILE );
/// Si queremos imprimir con CUPS lo hacemos de esta otra forma
/// Si queremos imprimir con CUPS lo hacemos de esta otra forma
    if (!g_confpr->valor ( CONF_TICKET_PRINTER_FILE).isEmpty() && g_confpr->valor ( CONF_TICKET_PRINTER_FILE) != "/dev/null") {
        QString comando = "cat " + g_confpr->valor(CONF_DIR_USER) + "bulmatpv_x.txt" + "  > " + g_confpr->valor ( CONF_TICKET_PRINTER_FILE );
        system ( comando.toAscii().data() );
    } else if (g_confpr->valor(CONF_CUPS_DEFAULT_PRINTER).isEmpty() || g_confpr->valor(CONF_CUPS_DEFAULT_PRINTER) == "None") {
        _depura("Debe establecer el parametro CONF_CUPS_DEFAULT_PRINTER o CONF_TICKET_PRINTER_FILE para abrir el cajon " , 2);
    } else {
        QString comando = "cupsdoprint -P" + g_confpr->valor(CONF_CUPS_DEFAULT_PRINTER) + " " + g_confpr->valor(CONF_DIR_USER) + "bulmatpv_x.txt";
        system ( comando.toAscii().data() );
    } // end if
  

    _depura ( "END PluginBt_IvaIncluido::BtCompany_x", 0 );
    
	return -1;
}

int ArticuloListSubForm_ArticuloListSubForm_Post(ArticuloListSubForm *list) {
    _depura ( "PluginBt_IvaIncluido::ArticuloListSubForm_ArticuloListSubForm_Post", 0 );
    
	list->addSubFormHeader ( "pvpivaincarticulo", BlDbField::DbNumeric, BlDbField::DbNoSave, BlSubFormHeader::DbNone | BlSubFormHeader::DbNoWrite, "PVP Iva Inc." );
	
    _depura ( "END PluginBt_IvaIncluido::ArticuloListSubForm_ArticuloListSubForm_Post", 0 );
    
    return 0;
}

