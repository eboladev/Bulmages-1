/***************************************************************************
 *   Copyright (C) 2005 by Tomeu Borras Riera                              *
 *   tborras@conetxia.com                                                  *
 *   Copyright (C) 2006 by Fco. Javier M. C.                               *
 *   fcojavmc@todo-redes.com                                               *
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

#include "plugintpvivainc.h"
#include "funcaux.h"
#include "empresatpv.h"
#include "escprinter.h"

typedef QMap<QString, Fixed> base;

int Ticket_agregarLinea_Post ( Ticket *tick, DBRecord * &item )
{
    item->addDBCampo ( "pvpivainclalbaran", DBCampo::DBint, DBCampo::DBNothing, QApplication::translate ( "Ticket", "Iva Inc" ) );
    return 0;
}


int Ticket_insertarArticulo_Post ( Ticket *tick )
{
    int valor = -1;
    _depura ( "pluginaliastpv::Ticket_insertarArticulo_Post", 0 );
    static int semaforo = 0;
    if ( semaforo == 0 ) {
        semaforo = 1;

        /// Buscamos los parametros en la base de datos.
        QString query = "SELECT * FROM articulo WHERE idarticulo = " + tick->lineaActTicket()->DBvalue("idarticulo");
        cursor2 *cur = tick->empresaBase() ->cargacursor ( query );
        if ( !cur->eof() ) {
            tick->lineaActTicket()->setDBvalue ( "pvpivainclalbaran", cur->valor ( "pvpivaincarticulo" ) );

        } // end if
        delete cur;

        valor = 0;
        semaforo = 0;
    } // end if
    _depura ( "END pluginaliastpv::Ticket_insertarArticulo_Post", 0 );
    return valor;
}


int Ticket_ponerPrecio_Post ( Ticket *tick ) {
	tick->lineaActTicket()->setDBvalue("pvpivainclalbaran", tick->lineaActTicket()->DBvalue("pvplalbaran"));
}


int Ticket_imprimir(Ticket *tick)
{


    struct empresastr {
        QString nombre;
        QString direccionCompleta;
        QString codigoPostal;
        QString ciudad;
        QString provincia;
        QString telefono;
    }empresa;

    struct clientestr {
        QString cif;
        QString nombre;
    }cliente;

    struct trabajadorstr {
        QString nombre;
        QString id;
    }trabajador;

    struct almacenstr {
        QString nombre;
    }almacen;

    struct fechastr {
        QString dia;
        QString hora;
    }fecha;

    struct totalstr {
        Fixed iva;
        Fixed baseImponible;
        Fixed totalIva;
    }total;

    cursor2 *cur = tick->empresaBase() ->cargacursor ( "SELECT * FROM configuracion WHERE nombre='NombreEmpresa'" );
    if ( !cur->eof() )
        empresa.nombre = cur->valor ( "valor" );
    delete cur;

    cur = tick->empresaBase() ->cargacursor ( "SELECT * FROM configuracion WHERE nombre='CIF'" );
    if ( !cur->eof() )
        empresa.nombre += "\n" + cur->valor ( "valor" );
    delete cur;

    cur = tick->empresaBase() ->cargacursor ( "SELECT * FROM configuracion WHERE nombre='DireccionCompleta'" );
    if ( !cur->eof() )
        empresa.direccionCompleta = cur->valor ( "valor" );
    delete cur;

    cur = tick->empresaBase() ->cargacursor ( "SELECT * FROM configuracion WHERE nombre='CodPostal'" );
    if ( !cur->eof() )
        empresa.codigoPostal = cur->valor ( "valor" ).toAscii();
    delete cur;

    cur = tick->empresaBase() ->cargacursor ( "SELECT * FROM configuracion WHERE nombre='Ciudad'" );
    if ( !cur->eof() )
        empresa.ciudad = cur->valor ( "valor" );
    delete cur;

    cur = tick->empresaBase() ->cargacursor ( "SELECT * FROM configuracion WHERE nombre='Telefono'" );
    if ( !cur->eof() )
        empresa.telefono = cur->valor ( "valor" );
    delete cur;

    cur = tick->empresaBase() ->cargacursor ( "SELECT * FROM configuracion WHERE nombre='Provincia'" );
    if ( !cur->eof() )
        empresa.provincia = cur->valor ( "valor" );
    delete cur;

    fecha.dia = QDate::currentDate().toString ( "d-M-yyyy" );
    fecha.hora = QTime::currentTime().toString ( "HH:mm" );

    trabajador.id = tick->DBvalue ( "idtrabajador" );
    cur = tick->empresaBase() ->cargacursor ( "SELECT * FROM trabajador WHERE idtrabajador=" + tick->DBvalue ( "idtrabajador" ) );
    if ( !cur->eof() )
        trabajador.nombre = cur->valor ( "nomtrabajador" );
    delete cur;

    cur = tick->empresaBase() ->cargacursor ( "SELECT * FROM cliente WHERE idcliente=" + tick->DBvalue ( "idcliente" ) );
    if ( !cur->eof() ) {
        cliente.cif = cur->valor ( "cifcliente" ).toAscii();
        cliente.nombre = cur->valor ( "nomcliente" ).toAscii();
    } // end if
    delete cur;

    cur = tick->empresaBase() ->cargacursor ( "SELECT * FROM almacen WHERE idalmacen=" + tick->DBvalue ( "idalmacen" ) );
    if ( !cur->eof() )
        almacen.nombre = cur->valor ( "nomalmacen" ).toAscii() ;
    delete cur;

    DBRecord *linea;
    base totales;

    /// Inicializamos los componentes.
    for ( int i = 0; i < tick->listaLineas() ->size(); ++i ) {
        linea = tick->listaLineas() ->at ( i );
	Fixed init("0.00");
        totales[linea->DBvalue ( "ivalalbaran" ) ] = init;
    } // end for


    for ( int i = 0; i < tick->listaLineas() ->size(); ++i ) {
        linea = tick->listaLineas() ->at ( i );
        Fixed cantidad = Fixed ( linea->DBvalue ( "cantlalbaran" ) );
	Fixed totlinea = cantidad * Fixed( linea->DBvalue("pvpivainclalbaran"));
        total.totalIva = total.totalIva + cantidad * Fixed ( linea->DBvalue ( "pvpivainclalbaran" ) );
        totales[linea->DBvalue ( "ivalalbaran" ) ] = totales[linea->DBvalue ( "ivalalbaran" ) ] + totlinea;
    } // end for

    EscPrinter pr ( confpr->valor ( CONF_TICKET_PRINTER_FILE ) );
    pr.initializePrinter();
    pr.setCharacterCodeTable ( page19 );
    pr.setJustification ( center );

    if ( confpr->valor ( CONF_TPV_PRINTER_LOGO ) != "" ) {
        pr.printImage ( confpr->valor ( CONF_TPV_PRINTER_LOGO ) );
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
    pr.printText ( "Cliente: " + cliente.cif + " " + cliente.nombre + "\n" );
    pr.printText ( "Num. Ticket:  " + tick->DBvalue("numalbaran") + "\n" );

    pr.printText ( "\n" );

    pr.turnWhiteBlack ( 1 );
    pr.printText ( " Uds. PRODUCTO � � � � � � �P.U. �IMPORTE \n" );

    pr.turnWhiteBlack ( 0 );
    pr.setCharacterPrintMode ( CHARACTER_FONTB_SELECTED );
    pr.setCharacterSize ( CHAR_WIDTH_1 | CHAR_HEIGHT_1 );



    for ( int i = 0; i < tick->listaLineas() ->size(); ++i ) {
        if ( i == tick->listaLineas()->size() - 1 )
            pr.setUnderlineMode ( 1 );
        linea = tick->listaLineas() ->at ( i );
        Fixed iva = Fixed ( linea->DBvalue ( "ivalalbaran" ) );
        Fixed pvp = Fixed ( linea->DBvalue ( "pvpivainclalbaran" ) );
        pvp = pvp + pvp * iva / Fixed ( "100" );
        Fixed pvptotal = Fixed ( linea->DBvalue ( "cantlalbaran" ) ) * pvp;
        pr.printText ( linea->DBvalue ( "cantlalbaran" ).rightJustified ( 5, ' ', TRUE ) + " �" );
        pr.printText ( linea->DBvalue ( "desclalbaran" ).leftJustified ( 27, ' ', true ) + " " );
        QString pvpstr = pvp.toQString();
        QString pvptotalstr = pvptotal.toQString();
        pr.printText ( QString ( pvpstr + "�" ).rightJustified ( 10, ' ', TRUE ) + " " );
        pr.printText ( QString ( pvptotalstr + "�" ).rightJustified ( 10, ' ', TRUE ) );
        pr.printText ( "\n" );
    }
    pr.setUnderlineMode ( 0 );
    pr.setJustification ( right );
    pr.setCharacterPrintMode ( CHARACTER_FONTA_SELECTED );


    base::Iterator it;
    for ( it = totales.begin(); it != totales.end(); ++it ) {
		QString sqlquery = "SELECT " +it.value().toQString('.') + "/ ( 1 + " + it.key() + "/100 ) AS base ";
		cursor2 *cur = tick->empresaBase()->cargacursor(sqlquery);
        	Fixed baseimp = Fixed(cur->valor("base"));;
		Fixed totiva = it.value() - baseimp;
		delete cur;
	    pr.printText ( "Base Imponible: "+ it.key() + "%  " + baseimp.toQString() + "�\n" );
    	pr.printText ( "IVA " +it.key() + "%  " + totiva.toQString() + "�\n" );
    } // end for



    pr.setCharacterPrintMode ( CHARACTER_FONTA_SELECTED | EMPHASIZED_MODE | DOUBLE_HEIGHT | DOUBLE_WIDTH );
    pr.printText ( "TOTAL: " + total.totalIva.toQString() + "�\n" );
    pr.printText ( "\n\n" );
    pr.setJustification ( left );
    pr.setCharacterPrintMode ( CHARACTER_FONTA_SELECTED );
    pr.printText ( "Le ha atendido " + trabajador.nombre + "\n" );
    pr.printText ( "\n" );

   pr.printText("Plazo maximo para cambio 15 dias, \n");
   pr.printText(" unicamente con ticket de compra. \n");
   pr.printText("\n");


    pr.printText ( "Tel. " + empresa.telefono + "\n" );
    pr.printText ( "\n" );

    pr.setJustification ( center );
    pr.setColor ( red );
    pr.printText ( "*** GRACIAS POR SU VISITA ***\n" );


    QByteArray qba = tick->DBvalue ( "refalbaran" ).toAscii();
    char* barcode = qba.data();
    pr.setJustification ( center );
    pr.setBarcodeFormat ( 2, 50, both, fontB );
    pr.printBarCode ( code39, qba.size(), barcode );
    pr.cutPaperAndFeed ( TRUE, 10 );
    pr.print();

    return 1;
}




int EmpresaTPV_z(EmpresaTPV * emp)
{

    emp->begin();
    QString query = "INSERT INTO z (idalmacen) VALUES(" + confpr->valor ( CONF_IDALMACEN_DEFECTO ) + ")";
    emp->ejecuta ( query );
    query = "SELECT max(idz) AS id FROM z";
    cursor2 *cur = emp->cargacursor ( query );
    QString idz = cur->valor ( "id" );
    delete cur;
    query = "UPDATE albaran set idz = " + idz + " WHERE idz IS NULL AND ticketalbaran = TRUE";
    emp->ejecuta ( query );
    query = "SELECT count(idz) AS numtickets, sum(totalalbaran) as total FROM albaran WHERE idz = " + idz;
    cur = emp->cargacursor ( query );
    QString numtickets = cur->valor ( "numtickets" );
    QString total = cur->valor ( "total" );
    if ( total == "" ) total = "0";
    query = "UPDATE z SET totalz = " + total + ", numtickets = " + numtickets + " WHERE idz =" + idz;
    emp->ejecuta ( query );
    emp->commit();
    delete cur;

    QString querycont = "SELECT count(idalbaran) AS numtickets, sum(totalalbaran) as total FROM albaran WHERE idz = " + idz + " AND ticketalbaran = TRUE AND idforma_pago = " + confpr->valor ( CONF_IDFORMA_PAGO_CONTADO );
    cursor2 *cur1 = emp->cargacursor ( querycont );
    QString numticketscont = cur1->valor ( "numtickets" );
    QString totalcont = cur1->valor ( "total" );
    if ( totalcont == "" ) totalcont = "0";
    delete cur1;

    QString queryvisa = "SELECT count(idalbaran) AS numtickets, sum(totalalbaran) as total FROM albaran WHERE idz = "+idz+" AND ticketalbaran = TRUE AND idforma_pago = "+ confpr->valor(CONF_IDFORMA_PAGO_VISA);

    cursor2 *cur2 = emp->cargacursor ( queryvisa );
    QString numticketsvisa = cur2->valor ( "numtickets" );
    QString totalvisa = cur2->valor ( "total" );
    if ( totalvisa == "" ) totalvisa = "0";
    delete cur2;

// ========================================

    QFile file ( confpr->valor ( CONF_TICKET_PRINTER_FILE ) );
    if ( !file.open ( QIODevice::WriteOnly | QIODevice::Unbuffered ) ) {
        _depura ( "Error en la Impresion de ticket", 2 );
        return -1;
    } // end if
    file.write ( QString ( "Informe Z\n" ).toAscii() );
    file.write ( QString ( "=========\n" ).toAscii() );
    cursor2 *curemp = emp->cargacursor ( "SELECT * FROM configuracion WHERE nombre='NombreEmpresa'" );
    if ( !curemp->eof() ) {
        file.write ( curemp->valor ( "valor" ).toAscii() );
        file.write ( "\n", 1 );
    } // end if
    delete curemp;
    file.write ( QString ( "====================================\n" ).toAscii() );
    cur = emp->cargacursor ( "SELECT * FROM configuracion WHERE nombre='DireccionCompleta'" );
    if ( !cur->eof() ) {
        file.write ( cur->valor ( "valor" ).toAscii() );
        file.write ( "\n", 1 );
    } // end if
    ///file.write ( QString ( "C/LAS POZAS 181, LOCAL 43\n" ).toAscii() );
    delete cur;
    /// file.write ( QString ( "ALIMENTACION ECOLOGICA. HERBOLARIO\n" ).toAscii() );
    cur = emp->cargacursor ( "SELECT * FROM configuracion WHERE nombre='CodPostal'" );
    if ( !cur->eof() ) {
        file.write ( cur->valor ( "valor" ).toAscii() );
    } // end if
    delete cur;

    file.write ( QString ( " " ).toAscii() );
    cur = emp->cargacursor ( "SELECT * FROM configuracion WHERE nombre='Ciudad'" );
    if ( !cur->eof() ) {
        file.write ( cur->valor ( "valor" ).toAscii() );
        file.write ( QString ( " " ).toAscii() );
    } // end if
    delete cur;


    cur = emp->cargacursor ( "SELECT * FROM configuracion WHERE nombre='Provincia'" );
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
    cur = emp->cargacursor ( "SELECT * FROM almacen WHERE idalmacen=" + confpr->valor ( CONF_IDALMACEN_DEFECTO ) );
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
	return -1;
}



int EmpresaTPV_x(EmpresaTPV *emp)
{

    QString query = "SELECT count(idalbaran) AS numtickets, sum(totalalbaran) as total FROM albaran WHERE idz IS NULL AND ticketalbaran = TRUE";
    cursor2 *cur = emp->cargacursor ( query );
    QString numtickets = cur->valor ( "numtickets" );
    QString total = cur->valor ( "total" );
    if ( total == "" ) total = "0";
    delete cur;

    QString querycont = "SELECT count(idalbaran) AS numtickets, sum(totalalbaran) as total FROM albaran WHERE idz IS NULL AND ticketalbaran = TRUE AND idforma_pago = " + confpr->valor ( CONF_IDFORMA_PAGO_CONTADO );
    cursor2 *cur1 = emp->cargacursor ( querycont );
    QString numticketscont = cur1->valor ( "numtickets" );
    QString totalcont = cur1->valor ( "total" );
    if ( totalcont == "" ) totalcont = "0";
    delete cur1;


    QString queryvisa = "SELECT count(idalbaran) AS numtickets, sum(totalalbaran) as total FROM albaran WHERE idz IS NULL AND ticketalbaran = TRUE AND idforma_pago = "+ confpr->valor(CONF_IDFORMA_PAGO_VISA);

    cursor2 *cur2 = emp->cargacursor ( queryvisa );
    QString numticketsvisa = cur2->valor ( "numtickets" );
    QString totalvisa = cur2->valor ( "total" );
    if ( totalvisa == "" ) totalvisa = "0";
    delete cur2;


// ========================================

    QFile file ( confpr->valor ( CONF_TICKET_PRINTER_FILE ) );
    if ( !file.open ( QIODevice::WriteOnly | QIODevice::Unbuffered ) ) {
        _depura ( "Error en la Impresion de ticket", 2 );
        return -1;
    } // end if
    file.write ( QString ( "Informe X\n" ).toAscii() );
    file.write ( QString ( "=========\n" ).toAscii() );
    cursor2 *curemp = emp->cargacursor ( "SELECT * FROM configuracion WHERE nombre='NombreEmpresa'" );
    if ( !curemp->eof() ) {
        file.write ( curemp->valor ( "valor" ).toAscii() );
        file.write ( "\n", 1 );
    } // end if
    delete curemp;
    file.write ( QString ( "====================================\n" ).toAscii() );
    cur = emp->cargacursor ( "SELECT * FROM configuracion WHERE nombre='DireccionCompleta'" );
    if ( !cur->eof() ) {
        file.write ( cur->valor ( "valor" ).toAscii() );
        file.write ( "\n", 1 );
    } // end if
    ///file.write ( QString ( "C/LAS POZAS 181, LOCAL 43\n" ).toAscii() );
    delete cur;
    /// file.write ( QString ( "ALIMENTACION ECOLOGICA. HERBOLARIO\n" ).toAscii() );
    cur = emp->cargacursor ( "SELECT * FROM configuracion WHERE nombre='CodPostal'" );
    if ( !cur->eof() ) {
        file.write ( cur->valor ( "valor" ).toAscii() );
    } // end if
    delete cur;

    file.write ( QString ( " " ).toAscii() );
    cur = emp->cargacursor ( "SELECT * FROM configuracion WHERE nombre='Ciudad'" );
    if ( !cur->eof() ) {
        file.write ( cur->valor ( "valor" ).toAscii() );
        file.write ( QString ( " " ).toAscii() );
    } // end if
    delete cur;


    cur = emp->cargacursor ( "SELECT * FROM configuracion WHERE nombre='Provincia'" );
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
    cur = emp->cargacursor ( "SELECT * FROM almacen WHERE idalmacen=" + confpr->valor ( CONF_IDALMACEN_DEFECTO ) );
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
	return -1;
}



