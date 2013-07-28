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

#include <QtWidgets/QDockWidget>

#include "pluginbt_printercocina.h"
#include "blfunctions.h"
#include "btcompany.h"
#include "blescprinter.h"

typedef QMap<QString, BlFixed> base;
int impresionCocina(BtTicket *);

/// Necesitamos una lista de elementos borrados para poder tenerlos en cuenta.


int BtTicket_borrarLinea ( BtTicket *tick ) {

  BlDbRecord *item = (BlDbRecord *) g_plugParams;
  if (BlFixed(item->dbValue("unidadescocina")) != 0) {
      item->setDbValue("cantlalbaran","0");
      impresionCocina(tick);
  } // end if
  return 0;
}

int BtTicket_agregarLinea_Post ( BtTicket *tick )
{
    BL_FUNC_DEBUG
    
    BlDbRecord *item = (BlDbRecord *) g_plugParams;

    item->addDbField ( "unidadescocina", BlDbField::DbNumeric, BlDbField::DbNoSave, _( "Unidades en Cocina" ) );
    item->setDbValue("unidadescocina", "0");
    
    
    
    return 0;
}



/// Vamos a hacer que la impresion de cocina tambien utilize una plantilla.


int impresionCocina(BtTicket *tick) {
    BlDebug::blDebug ("PluginBt_PrinterCocina::impresionCocina", 0);
    

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
        empresa.nombre = cur->value( "valor" );
    delete cur;

    cur = tick->mainCompany() ->loadQuery ( "SELECT * FROM configuracion WHERE nombre='CIF'" );
    if ( !cur->eof() )
        empresa.nombre += "\n" + cur->value( "valor" );
    delete cur;

    cur = tick->mainCompany() ->loadQuery ( "SELECT * FROM configuracion WHERE nombre='DireccionCompleta'" );
    if ( !cur->eof() )
        empresa.direccionCompleta = cur->value( "valor" );
    delete cur;

    cur = tick->mainCompany() ->loadQuery ( "SELECT * FROM configuracion WHERE nombre='CodPostal'" );
    if ( !cur->eof() )
        empresa.codigoPostal = cur->value( "valor" ).toLatin1();
    delete cur;

    cur = tick->mainCompany() ->loadQuery ( "SELECT * FROM configuracion WHERE nombre='Ciudad'" );
    if ( !cur->eof() )
        empresa.ciudad = cur->value( "valor" );
    delete cur;

    cur = tick->mainCompany() ->loadQuery ( "SELECT * FROM configuracion WHERE nombre='Telefono'" );
    if ( !cur->eof() )
        empresa.telefono = cur->value( "valor" );
    delete cur;

    cur = tick->mainCompany() ->loadQuery ( "SELECT * FROM configuracion WHERE nombre='Provincia'" );
    if ( !cur->eof() )
        empresa.provincia = cur->value( "valor" );
    delete cur;

    fecha.dia = QDate::currentDate().toString ( "d-M-yyyy" );
    fecha.hora = QTime::currentTime().toString ( "HH:mm" );

    trabajador.id = tick->dbValue ( "idtrabajador" );
    cur = tick->mainCompany() ->loadQuery ( "SELECT * FROM trabajador WHERE idtrabajador=" + tick->dbValue ( "idtrabajador" ) );
    if ( !cur->eof() )
        trabajador.nombre = cur->value( "nomtrabajador" );
    delete cur;

    cur = tick->mainCompany() ->loadQuery ( "SELECT * FROM cliente WHERE idcliente=" + tick->dbValue ( "idcliente" ) );
    if ( !cur->eof() ) {
        cliente.cif = cur->value( "cifcliente" ).toLatin1();
        cliente.nombre = cur->value( "nomcliente" ).toLatin1();
    } // end if
    delete cur;

    cur = tick->mainCompany() ->loadQuery ( "SELECT * FROM almacen WHERE idalmacen=" + tick->dbValue ( "idalmacen" ) );
    if ( !cur->eof() )
        almacen.nombre = cur->value( "nomalmacen" ).toLatin1() ;
    delete cur;

    BlDbRecord *linea;
    base totales;

    BlDbRecordSet *curimpresoras = tick -> mainCompany() -> loadQuery ( "SELECT * FROM printercocina" );
    while ( !curimpresoras->eof() ) {
      bool hayalgoqueimprimir = false;

      BlEscPrinter pr ( g_confpr->value(CONF_DIR_USER) + "bulmatpv_cocina.txt" );
      pr.initializePrinter();
      pr.setCharacterCodeTable ( page19 );
/*
      pr.setJustification ( BlEscPrinter::center );

//      if ( g_confpr->value( CONF_TPV_PRINTER_LOGO ) != "" ) {
//          pr.printImage ( g_confpr->value( CONF_TPV_PRINTER_LOGO ) );
//      } // end if


      pr.initializePrinter();
      pr.setCharacterCodeTable ( page19 );

      pr.printText ( "\n" );
      pr.printText ( fecha.dia + " " + fecha.hora + "\n" );
      pr.printText ( "Cliente: " + cliente.cif + " " + cliente.nombre + "\n" );

      pr.setJustification ( BlEscPrinter::center );
      pr.setCharacterSize ( CHAR_WIDTH_2 | CHAR_HEIGHT_2 );

      pr.printText ( "Ticket:  " + tick->dbValue("numalbaran") + "\n" );
      pr.printText (  tick->dbValue("nomticket") + "\n" );

      pr.printText ( "\n" );

      pr.initializePrinter();
      pr.setCharacterCodeTable ( page19 );
//      pr.setCharacterPrintMode ( CHARACTER_FONTB_SELECTED );
      pr.setCharacterSize ( CHAR_WIDTH_1 | CHAR_HEIGHT_1 );

      pr.turnWhiteBlack ( 1 );
      pr.printText ( "Uds PRODUCTO                              \n" );
      pr.turnWhiteBlack ( 0 );

      pr.setCharacterSize ( CHAR_WIDTH_1 | CHAR_HEIGHT_2 );
*/
      /// Establecemos la variable de idprintercocina para que el sistema pueda identificarla.
      tick->clearVars();
      
       /// Iteramos sobre las lineas del ticket para ver que pasa.
       for ( int i = 0; i < tick->listaLineas() ->size(); ++i ) {
          linea = tick->listaLineas() ->at ( i );
          BlDbRecordSet *curarticulos = tick -> mainCompany() -> loadQuery( "SELECT idprintercocina FROM articulo WHERE idarticulo = " + linea->dbValue ( "idarticulo" ) );
          /// Si la linea tiene que salir por cocina hacemos lo pertinente.
          if (curarticulos ->value("idprintercocina") == curimpresoras -> value("idprintercocina")) {
            BlFixed undcocina ( linea->dbValue("unidadescocina"));
            BlFixed cantlalbaran (linea->dbValue("cantlalbaran"));
            if (undcocina != cantlalbaran) {
              BlFixed resta = cantlalbaran - undcocina;
              hayalgoqueimprimir = true;
              pr.printText ( resta.toQString().rightJustified ( 5, ' ', true ) + " " );
              pr.printText ( blStringToUsAscii(linea->dbValue ( "desclalbaran" ).leftJustified ( 27, ' ', true )) + " " );
              pr.printText ( "\n" );

	      /// Imprime los componentes del articulo si la cantidad es positiva.
	      if (resta > 0) {
	                BlDbRecordSet *componentes = tick -> mainCompany() -> loadQuery( "SELECT sinacentos(articulo.nomarticulo) AS nomarticulo2, comparticulo.cantcomparticulo AS cantarticulo2  FROM comparticulo LEFT JOIN articulo ON comparticulo.idcomponente = articulo.idarticulo WHERE comparticulo.idarticulo = " + linea->dbValue ( "idarticulo" ) );
		        while ( !componentes->eof() ) {
	            	    pr.printText("* <!-- RIGHTJUSTIFIED \"" + componentes->value("cantarticulo2") + "\" \"5\" \" \" \"true\"--> <!-- LEFTJUSTIFIED \"" + componentes->value("nomarticulo2") + "\" \"18\" \" \" \"true\"-->\n");
	    		    componentes->nextRecord();
			} // end while
	      } // end if

              linea->setDbValue("unidadescocina", linea->dbValue("cantlalbaran"));
	      tick->setVar("unidadescocina"+linea->dbValue("idarticulo"), resta.toQString());
            } // end if
          } // end if
          delete curarticulos;
       } // end for

       /// Terminamos la impresion
      pr.setCharacterSize ( CHAR_WIDTH_1 | CHAR_HEIGHT_1 );
/*
      QByteArray qba = tick->dbValue ( "refalbaran" ).toLatin1();
      char* barcode = qba.data();
      pr.setJustification ( BlEscPrinter::center );
      pr.setBarcodeFormat ( 2, 50, both, fontB );
      pr.printBarCode ( code39, qba.size(), barcode );
      pr.cutPaperAndFeed ( true, 10 );
      pr.print();
*/

      /// Establecemos la variable cambioscocina para que incorpore los cambios realizados.
      tick->setVar("cambioscocina", pr.buffer());
      tick->setVar("idprintercocina", curimpresoras->value("idprintercocina"));
//      tick->clearVars();

      tick->generateRML("ticket_cocina.txt");
      
       /// Si realmente hay algo que imprimir entonces lo sacamos.
       if (hayalgoqueimprimir) {
			  blRawPrint("ticket_cocina.txt", true, curimpresoras->value("colaprintercocina"));
       } // end if

       curimpresoras->nextRecord();
    } // end while
    delete curimpresoras;

    BlDebug::blDebug ("END PluginBt_PrinterCocina::impresionCocina", 0);
}




int BtTicket_imprimir_Post(BtTicket *tick)
{
    BL_FUNC_DEBUG
    impresionCocina(tick);
    
    return 1;
}

int Abrevs_on_mui_aparcar_clicked( Abrevs *av)
{
    BL_FUNC_DEBUG
    impresionCocina(((BtCompany *)av->mainCompany())->ticketActual());
    
    return 0;
}

int ticket_aparcado_remotamente(BtTicket *tick) {
  impresionCocina(tick);
  return 0;
}



int BtCompany_cobrar_1(BtCompany *comp) {
    BL_FUNC_DEBUG
    impresionCocina(comp->ticketActual());
    
    return 0;
}
