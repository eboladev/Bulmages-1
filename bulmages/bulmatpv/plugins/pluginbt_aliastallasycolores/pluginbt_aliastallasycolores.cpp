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

#include <QtWidgets/QDockWidget>
#include <QtWidgets/QLabel>
#include <QtWidgets/QTextBrowser>

#include "pluginbt_aliastallasycolores.h"
#include "blfunctions.h"
#include "btcompany.h"
#include "blescprinter.h"
#include "mticketivainc.h"
#include "bldb.h"
#include "btbulmatpv.h"
#include "btsubform.h"



/// Una factura puede tener multiples bases imponibles. Por eso definimos el tipo base
/// como un QMap.
typedef QMap<QString, BlFixed> base;

int BtTicket_agregarLinea_Post ( BtTicket *tick)
{
    BL_FUNC_DEBUG
    BlDbRecord *rec = (BlDbRecord * ) g_plugParams;
    rec->addDbField ( "idtc_talla", BlDbField::DbNumeric, BlDbField::DbNothing, _ ( "Talla" ) );
    rec->addDbField ( "idtc_color", BlDbField::DbNumeric, BlDbField::DbNothing, _ ( "Color" ) );
    rec->addDbField ( "nomtc_talla", BlDbField::DbVarChar, BlDbField::DbNoSave, _ ( "Talla" ) );
    rec->addDbField ( "nomtc_color", BlDbField::DbVarChar, BlDbField::DbNoSave, _ ( "Color" ) );
    
    return 0;
}

int BtTicket_insertarArticuloCodigo_Post ( BtTicket *tick )
{
    BL_FUNC_DEBUG
    int valor = -1;
    static int semaforo = 0;
    if ( semaforo == 0 ) {
        valor = 0;
        semaforo = 1;
        QString query = "SELECT * FROM tc_articulo_alias LEFT JOIN tc_talla AS t1 ON tc_articulo_alias.idtc_talla = t1.idtc_talla LEFT JOIN tc_color AS t2 ON tc_articulo_alias.idtc_color = t2.idtc_color WHERE aliastc_articulo_tallacolor = '" + ( ( BtCompany * ) tick->mainCompany() )->valorBtInput() + "'";
        BlDbRecordSet *cur = tick->mainCompany() ->loadQuery ( query );
        if ( !cur->eof() ) {
            BlDbRecord * rec = tick->insertarArticulo ( cur->value( "idarticulo" ), BlFixed ( "1" ), true );
            rec->setDbValue ( "idtc_talla", cur->value( "idtc_talla" ) );
            rec->setDbValue ( "idtc_color", cur->value( "idtc_color" ) );
            rec->setDbValue ( "nomtc_talla", cur->value( "nomtc_talla" ) );
            rec->setDbValue ( "nomtc_color", cur->value( "nomtc_color" ) );
        } else {
	    valor = -1;
	} // end if
        delete cur;
        tick->pintar();
        semaforo = 0;
    } // end if
    
    return valor;
}

int BtTicket_insertarArticuloCodigoNL_Post ( BtTicket *tick )
{
    BL_FUNC_DEBUG
    int valor = -1;
    static int semaforo = 0;
    if ( semaforo == 0 ) {
        valor = 0;
        semaforo = 1;
        QString query = "SELECT * FROM tc_articulo_alias LEFT JOIN tc_talla AS t1 ON tc_articulo_alias.idtc_talla = t1.idtc_talla LEFT JOIN tc_color AS t2 ON tc_articulo_alias.idtc_color = t2.idtc_color WHERE aliastc_articulo_tallacolor = '" + ( ( BtCompany * ) tick->mainCompany() )->valorBtInput() + "'";
        BlDbRecordSet *cur = tick->mainCompany() ->loadQuery ( query );
        if ( !cur->eof() ) {
            BlDbRecord * rec = tick->insertarArticulo ( cur->value( "idarticulo" ), BlFixed ( "1" ), true );
            rec->setDbValue ( "idtc_talla", cur->value( "idtc_talla" ) );
            rec->setDbValue ( "idtc_color", cur->value( "idtc_color" ) );
            rec->setDbValue ( "nomtc_talla", cur->value( "nomtc_talla" ) );
            rec->setDbValue ( "nomtc_color", cur->value( "nomtc_color" ) );
        } else {
	    valor = -1;
	} // end if
        delete cur;
        tick->pintar();
        semaforo = 0;
    } // end if
    
    return valor;
}



int MTicket_pintar ( MTicket *mtick )
{
    BL_FUNC_DEBUG
    
    QString buscar;

    BtTicket *tick =     ( ( BtCompany * ) mtick->mainCompany() ) ->ticketActual();
    QString htmlContent = "<p style=\"font-family:monospace; font-size: 12pt;\">";
    QString plainTextContent = "";
    QString query;

    htmlContent += "Ticket: " + tick->dbValue ( "nomticket" ) + "<BR>";
    plainTextContent += "Ticket: " + tick->dbValue ( "nomticket" ) + "\n";

    query = "SELECT idtrabajador, nomtrabajador FROM trabajador WHERE idtrabajador = " + tick->dbValue ( "idtrabajador" );
    BlDbRecordSet *rsTrabajador = mtick->mainCompany()->loadQuery ( query );
    plainTextContent += "Trabajador: " + rsTrabajador->value( "nomtrabajador" ) + "\n";
    htmlContent += "Trabajador: " + rsTrabajador->value( "nomtrabajador" ) + "<br>";
    delete rsTrabajador;
    
    query = "SELECT idcliente, nomcliente FROM cliente WHERE idcliente = " + tick->dbValue ( "idcliente" );
    BlDbRecordSet *rsCliente = mtick->mainCompany()->loadQuery ( query );
    plainTextContent += "Cliente: " + rsCliente->value( "nomcliente" ) + "\n";
    htmlContent += "Cliente: " + rsCliente->value( "nomcliente" ) + "<br>";
    delete rsCliente;

    htmlContent += "<TABLE border=\"0\">";
    BlDbRecord *item;
    
    htmlContent += "<tr><td>" + QString(_("CANT:")) + "</td><td width=\"30%\">" + QString(_("ARTI:")) + "</td><td>" + QString(_("TALLA:")) + "</td><td>" + QString(_("COLOR:")) + "</td><td>" + QString(_("PREC:")) + "</td></tr>";
    htmlContent += "<TR><TD colspan=\"6\" width=\"100%\"><HR></TD></TR>";
    
    plainTextContent += "\n";
    plainTextContent += "  " + QString(_("CANT:").rightJustified( 7, ' ', true )) + "  " + QString(_("ARTI:").leftJustified ( 15, ' ', true )) + "  " + QString(_("TALLA:").leftJustified( 7, ' ', true ))+ "  " + QString(_("COLOR:").leftJustified( 7, ' ', true ))+ "  " + QString(_("PRECIO:").rightJustified( 9, ' ', true )) + "\n";
    plainTextContent += "-----------------------------------------------------------\n";
    
    
    for ( int i = 0; i < tick->listaLineas() ->size(); ++i ) {
        item = tick->listaLineas() ->at ( i );
        QString bgcolor = "#FFFFFF";
        if ( item == tick->lineaActBtTicket() ) { 
	  buscar = item->dbValue ( "nomarticulo" );
	  bgcolor = "#CCFFFF";
	  plainTextContent += "> ";
        } else {
            plainTextContent += "  ";
        } // end if
    
        htmlContent += "<TR>";
        htmlContent += "<TD bgcolor=\"" + bgcolor + "\" align=\"right\" width=\"50\">" + item->dbValue ( "cantlalbaran" ) + "</TD>";
	htmlContent += "<TD bgcolor=\"" + bgcolor + "\">" + item->dbValue ( "nomarticulo" ) + "</TD>";
        htmlContent += "<TD bgcolor=\"" + bgcolor + "\">" + item->dbValue ( "nomtc_talla" ) + "</TD>";
        htmlContent += "<TD bgcolor=\"" + bgcolor + "\">" + item->dbValue ( "nomtc_color" ) + "</TD>";
	
        BlFixed totalLinea ( "0.00" );
        totalLinea = BlFixed ( item->dbValue ( "cantlalbaran" ) ) * BlFixed ( item->dbValue ( "pvplalbaran" ) );
        htmlContent += "<TD bgcolor=\"" + bgcolor + "\" align=\"right\" width=\"50\">" + totalLinea.toQString() + "</TD>";
        htmlContent += "</TR>";
	  
	plainTextContent += item->dbValue("cantlalbaran").rightJustified ( 7, ' ', true ) + " ";
	plainTextContent += item->dbValue("nomarticulo").leftJustified ( 20, ' ', true ) + " ";
	plainTextContent += item->dbValue("nomtc_talla").leftJustified ( 7, ' ', true ) + " ";
	plainTextContent += item->dbValue("nomtc_color").leftJustified ( 7, ' ', true ) + " ";
	plainTextContent += totalLinea.toQString().leftJustified ( 9, ' ', true ) + "\n";
	
    }// end for
    
    
    htmlContent += "</TABLE>";
    htmlContent += "<BR><HR><BR>";
    plainTextContent += "\n";
    base basesimp;
    base basesimpreqeq;
    BlDbRecord *linea;
    /// Impresion de los contenidos.
    QString l;
    BlFixed irpf ( "0" );

    BlDbRecordSet *cur = mtick->mainCompany() ->loadQuery ( "SELECT * FROM configuracion WHERE nombre = 'IRPF'" );
    if ( cur ) {
        if ( !cur->eof() ) {
            irpf = BlFixed ( cur->value( "valor" ) );
        } // end if
        delete cur;
    } // end if


    BlFixed descuentolinea ( "0.00" );
    for ( int i = 0; i < tick->listaLineas() ->size(); ++i ) {
        linea = tick->listaLineas() ->at ( i );
        BlFixed cant ( linea->dbValue ( "cantlalbaran" ) );
        BlFixed pvpund ( linea->dbValue ( "pvplalbaran" ) );
        BlFixed desc1 ( linea->dbValue ( "descuentolalbaran" ) );
        BlFixed cantpvp = cant * pvpund;
        BlFixed base = cantpvp - cantpvp * desc1 / 100;
        descuentolinea = descuentolinea + ( cantpvp * desc1 / 100 );
        basesimp[linea->dbValue ( "ivalalbaran" ) ] = basesimp[linea->dbValue ( "ivalalbaran" ) ] + base;
        basesimpreqeq[linea->dbValue ( "reqeqlalbaran" ) ] = basesimpreqeq[linea->dbValue ( "reqeqlalbaran" ) ] + base;
    } // end for

    BlFixed basei ( "0.00" );
    base::Iterator it;
    for ( it = basesimp.begin(); it != basesimp.end(); ++it ) {
        basei = basei + it.value();
    } // end for

    /// Calculamos el total de los descuentos.
    /// De momento aqui no se usan descuentos generales en venta.
    BlFixed porcentt ( "0.00" );

    /// Calculamos el total de base imponible.
    BlFixed totbaseimp ( "0.00" );
    BlFixed parbaseimp ( "0.00" );
    for ( it = basesimp.begin(); it != basesimp.end(); ++it ) {
        if ( porcentt > BlFixed ( "0.00" ) ) {
            parbaseimp = it.value() - it.value() * porcentt / 100;
        } else {
            parbaseimp = it.value();
        } // end if
        plainTextContent += "Base Imp " + it.key() + "% " + parbaseimp.toQString() + "\n";
        totbaseimp = totbaseimp + parbaseimp;
    } // end for

    /// Calculamos el total de IVA.
    BlFixed totiva ( "0.00" );
    BlFixed pariva ( "0.00" );
    for ( it = basesimp.begin(); it != basesimp.end(); ++it ) {
        BlFixed piva ( it.key().toLatin1().constData() );
        if ( porcentt > BlFixed ( "0.00" ) ) {
            pariva = ( it.value() - it.value() * porcentt / 100 ) * piva / 100;
        } else {
            pariva = it.value() * piva / 100;
        } // end if
        plainTextContent += "IVA " + it.key() + "% " + pariva.toQString() + "\n";
        totiva = totiva + pariva;
    } // end for

    /// Calculamos el total de recargo de equivalencia.
    BlFixed totreqeq ( "0.00" );
    BlFixed parreqeq ( "0.00" );
    for ( it = basesimpreqeq.begin(); it != basesimpreqeq.end(); ++it ) {
        BlFixed preqeq ( it.key().toLatin1().constData() );
        if ( porcentt > BlFixed ( "0.00" ) ) {
            parreqeq = ( it.value() - it.value() * porcentt / 100 ) * preqeq / 100;
        } else {
            parreqeq = it.value() * preqeq / 100;
        } // end if
        plainTextContent += "R.Eq " + it.key() + "% " + parreqeq.toQString() + "\n";
        totreqeq = totreqeq + parreqeq;
    } // end for

    BlFixed totirpf = totbaseimp * irpf / 100;
    plainTextContent += "Base Imp. " + totbaseimp.toQString() + "\n";
    plainTextContent += "IVA. " + totiva.toQString() + "\n";
    plainTextContent += "IRPF. " + totirpf.toQString() + "\n";
    BlFixed total = totiva + totbaseimp + totreqeq - totirpf;
    plainTextContent += "Total: " + total.toQString();

    /// Pintamos el HTML en el textBrowser
    mtick->mui_browser->setText ( htmlContent );
    mtick->mui_plainText->setText ( plainTextContent );
    
    mtick->mui_browser->find ( buscar );
    QTextCursor cursor = mtick->mui_browser->textCursor();
    cursor.clearSelection();
    mtick->mui_browser->setTextCursor( cursor );
    
    
    return -1;
}



int MTicketIVAInc_pintar ( MTicketIVAInc *mtick )
{
  BL_FUNC_DEBUG
  
  QString query;
  QString buscar;
  
  BtTicket *tick = ( ( BtCompany * ) mtick->mainCompany() )->ticketActual();
  QString plainTextContent = "";
  QString htmlContent = "<font size=\"4\", font-family:monospace>";
  
  htmlContent += "Ticket: " + tick->dbValue ( "nomticket" ) + "<BR>";
  plainTextContent += "Ticket: " + tick->dbValue ( "nomticket" ) + "\n";
  
  query = "SELECT idtrabajador, nomtrabajador FROM trabajador WHERE idtrabajador = " + tick->dbValue ( "idtrabajador" );
  BlDbRecordSet *rsTrabajador = mtick->mainCompany()->loadQuery ( query );
  plainTextContent += "Trabajador: " + rsTrabajador->value( "nomtrabajador" ) + "\n";
  htmlContent += "Trabajador: " + rsTrabajador->value( "nomtrabajador" ) + "<br>";
  delete rsTrabajador;
 
  query = "SELECT idcliente, nomcliente FROM cliente WHERE idcliente = " + tick->dbValue ( "idcliente" );
  BlDbRecordSet *rsCliente = mtick->mainCompany()->loadQuery ( query );
  plainTextContent += "Cliente: " + rsCliente->value( "nomcliente" ) + "\n";
  htmlContent += "Cliente: " + rsCliente->value( "nomcliente" ) + "<br>";
  delete rsCliente;
  
  htmlContent += "<br>";
  htmlContent += "<TABLE border=\"0\" width=\"100%\">";
  htmlContent += "<tr><td>" + QString(_("CANT:")) + "</td><td width=\"30%\">" + QString(_("ARTI:")) + "</td><td>" + QString(_("TALLA:")) + "</td><td>" + QString(_("COLOR:")) + "</td><td>" + QString(_("PREC:")) + "</td></tr>";
  htmlContent += "<tr><td colspan=\"5\" width=\"100%\" ><hr></td></tr>";
  plainTextContent += "\n";
  plainTextContent += "  " + QString(_("CANT:").rightJustified( 7, ' ', true )) + "  " + QString(_("ARTI:").leftJustified ( 15, ' ', true )) + "  " + QString(_("TALLA:").leftJustified( 7, ' ', true ))+ "  " + QString(_("COLOR:").leftJustified( 7, ' ', true ))+ "  " + QString(_("PRECIO:").leftJustified( 9, ' ', true )) + "\n";
  plainTextContent += "-----------------------------------------------------------\n";
  
//   if (tick->dbValue("nomticket") != "") {
//     htmlContent += "<TR><TD colspan=\"5\" align=\"center\"><B>" + tick->dbValue ( "nomticket" ) + "</B></td></tr>";
//   } // end if

  BlDbRecord *item;
  for ( int i = 0; i < tick->listaLineas()->size(); ++i ) {
      item = tick->listaLineas()->at ( i );
      QString bgcolor = "#FFFFFF";
      if ( item == tick->lineaActBtTicket() ) {
	    buscar = item->dbValue ( "nomarticulo" );
            bgcolor = "#CCCCFF";
            plainTextContent += "> ";
        } else {
            plainTextContent += "  ";
        } // end if
      htmlContent += "<TR>";
      htmlContent += "<TD bgcolor=\"" + bgcolor + "\" align=\"right\" width=\"50\">" + item->dbValue ( "cantlalbaran" ) + "</TD>";
      htmlContent += "<TD bgcolor=\"" + bgcolor + "\" whidth>" + item->dbValue ( "nomarticulo" ) + "</TD>";
      htmlContent += "<TD bgcolor=\"" + bgcolor + "\">" + item->dbValue ( "nomtc_talla" ) + "</TD>";
      htmlContent += "<TD bgcolor=\"" + bgcolor + "\">" + item->dbValue ( "nomtc_color" ) + "</TD>";
      BlFixed totalLinea ( "0.00" );
      totalLinea = BlFixed ( item->dbValue ( "cantlalbaran" ) ) * BlFixed ( item->dbValue ( "pvpivainclalbaran" ) );
      htmlContent += "<TD bgcolor=\"" + bgcolor + "\" align=\"right\" width=\"50\">" + totalLinea.toQString() + "</TD>";
      htmlContent += "</TR>";
      
      plainTextContent += item->dbValue("cantlalbaran").rightJustified ( 7, ' ', true ) + " ";
      plainTextContent += item->dbValue("nomarticulo").leftJustified ( 20, ' ', true ) + " ";
      plainTextContent += item->dbValue("nomtc_talla").leftJustified ( 7, ' ', true ) + " ";
      plainTextContent += item->dbValue("nomtc_color").leftJustified ( 7, ' ', true ) + " ";
      plainTextContent += totalLinea.toQString().leftJustified ( 9, ' ', true ) + "\n";
      
  } // end for
  
  htmlContent+= "</TABLE>";
  htmlContent += "<HR>";
  base basesimp;
  base basesimpreqeq;
  BlDbRecord *linea;
  
  QString l;
  BlFixed irpf ( "0" );

  BlDbRecordSet *cur = mtick->mainCompany()->loadQuery ( "SELECT * FROM configuracion WHERE nombre = 'IRPF'" );
  
  if ( cur ) {
      if ( !cur->eof() ) {
	  irpf = BlFixed ( cur->value( "valor" ) );
      } // end if
      delete cur;
  } // end if

  BlFixed descuentolinea ( "0.00" );
  
  for ( int i = 0; i < tick->listaLineas()->size(); ++i ) {
      linea = tick->listaLineas()->at ( i );
      BlFixed cant ( linea->dbValue ( "cantlalbaran" ) );
      BlFixed pvpund ( linea->dbValue ( "pvpivainclalbaran" ) );
      BlFixed desc1 ( linea->dbValue ( "descuentolalbaran" ) );
      BlFixed cantpvp = cant * pvpund;
      BlFixed iva ( linea->dbValue ( "ivalalbaran" ) );
      BlFixed base = cantpvp - cantpvp * desc1 / 100;
      base = base / ( BlFixed ( "1" ) + ( iva / BlFixed ( "100" ) ) );
      descuentolinea = descuentolinea + ( cantpvp * desc1 / 100 );
      basesimp[linea->dbValue ( "ivalalbaran" ) ] = basesimp[linea->dbValue ( "ivalalbaran" ) ] + base;
      basesimpreqeq[linea->dbValue ( "reqeqlalbaran" ) ] = basesimpreqeq[linea->dbValue ( "reqeqlalbaran" ) ] + base;
  } // end for

  BlFixed basei ( "0.00" );
  base::Iterator it;
  
  for ( it = basesimp.begin(); it != basesimp.end(); ++it ) {
      basei = basei + it.value();
  } // end for
  mtick->mui_browser->setText ( htmlContent );
  mtick->mui_plainText->setPlainText ( plainTextContent );
  
  mtick->mui_browser->find ( buscar );
  QTextCursor cursor = mtick->mui_browser->textCursor();
  cursor.clearSelection();
  mtick->mui_browser->setTextCursor( cursor );
  
  
  
  return -1;
  
  
 
//     BtTicket *tick = ( ( BtCompany * ) mtick->mainCompany() )->ticketActual();
//     QString html = "<p style=\"font-family:monospace; font-size: 12pt;\">";
//     QString html1 = "<font size=\"1\">";
// 
//     html1 += "Ticket: " + tick->dbValue ( "nomticket" ) + "<BR>";
// 
//     QString querytrab = "SELECT * FROM trabajador WHERE idtrabajador = " + tick->dbValue ( "idtrabajador" );
//     BlDbRecordSet *curtrab = mtick->mainCompany()->loadQuery ( querytrab );
//     html1 += "Trabajador: " + tick->dbValue ( "idtrabajador" ) + " " + curtrab->value( "nomtrabajador" ) + "<BR>";
//     delete curtrab;
//     
//     QString query = "SELECT * FROM cliente WHERE idcliente = " + tick->dbValue ( "idcliente" );
//     BlDbRecordSet *cur1 = mtick->mainCompany()->loadQuery ( query );
//     html1 += "Cliente: " + tick->dbValue ( "idcliente" ) + " " + cur1->value( "nomcliente" ) + "<BR>";
//     delete cur1;
// 
//     html += "<TABLE border=\"0\" width=\"100%\">";
//     if (tick->dbValue("nomticket") != "") {
//       html += "<TR><TD colspan=\"5\" align=\"center\"><B>" + tick->dbValue ( "nomticket" ) + "</B></td></tr>";
//     } // end if
// 
//     BlDbRecord *item;
//     for ( int i = 0; i < tick->listaLineas()->size(); ++i ) {
//         item = tick->listaLineas()->at ( i );
//         QString bgcolor = "#FFFFFF";
//         if ( item == tick->lineaActBtTicket() ) bgcolor = "#CCCCFF";
//         html += "<TR>";
//         html += "<TD bgcolor=\"" + bgcolor + "\" align=\"right\" width=\"50\">" + item->dbValue ( "cantlalbaran" ) + "</TD>";
//         html += "<TD bgcolor=\"" + bgcolor + "\">" + item->dbValue ( "nomarticulo" ) + "</TD>";
//         html += "<TD bgcolor=\"" + bgcolor + "\">" + item->dbValue ( "nomtc_talla" ) + "</TD>";
//         html += "<TD bgcolor=\"" + bgcolor + "\">" + item->dbValue ( "nomtc_color" ) + "</TD>";
//         BlFixed totalLinea ( "0.00" );
//         totalLinea = BlFixed ( item->dbValue ( "cantlalbaran" ) ) * BlFixed ( item->dbValue ( "pvpivainclalbaran" ) );
//         html += "<TD bgcolor=\"" + bgcolor + "\" align=\"right\" width=\"50\">" + totalLinea.toQString() + "</TD>";
//         html += "</TR>";
//     } // end for
//     
//     html += "</TABLE>";    
//     html += "<BR><HR><BR>";
//     base basesimp;
//     base basesimpreqeq;
//     BlDbRecord *linea;
//     
//     /// Impresion de los contenidos.
//     QString l;
//     BlFixed irpf ( "0" );
// 
//     BlDbRecordSet *cur = mtick->mainCompany()->loadQuery ( "SELECT * FROM configuracion WHERE nombre = 'IRPF'" );
//     
//     if ( cur ) {
//         if ( !cur->eof() ) {
//             irpf = BlFixed ( cur->value( "valor" ) );
//         } // end if
//         delete cur;
//     } // end if
// 
//     BlFixed descuentolinea ( "0.00" );
//     
//     for ( int i = 0; i < tick->listaLineas()->size(); ++i ) {
//         linea = tick->listaLineas()->at ( i );
//         BlFixed cant ( linea->dbValue ( "cantlalbaran" ) );
//         BlFixed pvpund ( linea->dbValue ( "pvpivainclalbaran" ) );
//         BlFixed desc1 ( linea->dbValue ( "descuentolalbaran" ) );
//         BlFixed cantpvp = cant * pvpund;
//         BlFixed iva ( linea->dbValue ( "ivalalbaran" ) );
//         BlFixed base = cantpvp - cantpvp * desc1 / 100;
//         base = base / ( BlFixed ( "1" ) + ( iva / BlFixed ( "100" ) ) );
//         descuentolinea = descuentolinea + ( cantpvp * desc1 / 100 );
//         basesimp[linea->dbValue ( "ivalalbaran" ) ] = basesimp[linea->dbValue ( "ivalalbaran" ) ] + base;
//         basesimpreqeq[linea->dbValue ( "reqeqlalbaran" ) ] = basesimpreqeq[linea->dbValue ( "reqeqlalbaran" ) ] + base;
//     } // end for
// 
//     BlFixed basei ( "0.00" );
//     base::Iterator it;
//     
//     for ( it = basesimp.begin(); it != basesimp.end(); ++it ) {
//         basei = basei + it.value();
//     } // end for
// 
//     /// Calculamos el total de los descuentos.
//     /// De momento aqui no se usan descuentos generales en venta.
//     BlFixed porcentt ( "0.00" );
// 
//     /// Calculamos el total de base imponible.
//     BlFixed totbaseimp ( "0.00" );
//     BlFixed parbaseimp ( "0.00" );
//     
//     for ( it = basesimp.begin(); it != basesimp.end(); ++it ) {
//         if ( porcentt > BlFixed ( "0.00" ) ) {
//             parbaseimp = it.value() - it.value() * porcentt / 100;
//         } else {
//             parbaseimp = it.value();
//         } // end if
//         html1 += "Base Imp " + it.key() + "% " + parbaseimp.toQString() + "<BR>";
//         totbaseimp = totbaseimp + parbaseimp;
//     } // end for
// 
//     /// Calculamos el total de IVA.
//     BlFixed totiva ( "0.00" );
//     BlFixed pariva ( "0.00" );
//     
//     for ( it = basesimp.begin(); it != basesimp.end(); ++it ) {
//         BlFixed piva ( it.key().toLatin1().constData() );
//         if ( porcentt > BlFixed ( "0.00" ) ) {
//             pariva = ( it.value() - it.value() * porcentt / 100 ) * piva / 100;
//         } else {
//             pariva = it.value() * piva / 100;
//         } // end if
//         html1 += "IVA " + it.key() + "% " + pariva.toQString() + "<BR>";
//         totiva = totiva + pariva;
//     } // end for
// 
//     /// Calculamos el total de recargo de equivalencia.
//     BlFixed totreqeq ( "0.00" );
//     BlFixed parreqeq ( "0.00" );
//     
//     for ( it = basesimpreqeq.begin(); it != basesimpreqeq.end(); ++it ) {
//         BlFixed preqeq ( it.key().toLatin1().constData() );
//         if ( porcentt > BlFixed ( "0.00" ) ) {
//             parreqeq = ( it.value() - it.value() * porcentt / 100 ) * preqeq / 100;
//         } else {
//             parreqeq = it.value() * preqeq / 100;
//         } // end if
//         html1 += "R.Eq " + it.key() + "% " + parreqeq.toQString() + "<BR>";
//         totreqeq = totreqeq + parreqeq;
//     } // end for
// 
//     BlFixed totirpf = totbaseimp * irpf / 100;
// 
//     html1 += "<B>Base Imp. " + totbaseimp.toQString() + "<BR>";
//     html1 += "<B>IVA. " + totiva.toQString() + "<BR>";
//     html1 += "<B>IRPF. " + totirpf.toQString() + "<BR>";
// 
//     BlFixed total = totiva + totbaseimp + totreqeq - totirpf;
//     html1 += "<B>Total: " + total.toQString() + "<BR>";
// 
//     html += "</p>";
//     html1 += "</FONT>";
// 
//     /// Pintamos el HTML en el textBrowser
//     mtick->mui_browser->setText ( html );
//     
//     return -1;
}




int BtTicket_imprimir(BtTicket *tick)
{
    BlDebug::blDebug("pluginbt_aliastallasycolores::BtTicket_imprimir",0);

    if ( tick->listaLineas()->size() ) {
        if( tick->listaLineas()->at ( 0 )->exists ( "pvpivainclalbaran" ) ) {
	    BtTicket_imprimirIVAInc(tick);
	    BlDebug::blDebug("END pluginbt_aliastallasycolores::BtTicket_imprimir",0);
	    return -1;
	} // end if
    } else {
        BlDebug::blDebug("END pluginbt_aliastallasycolores::BtTicket_imprimir",0);
        return -1;
    } // end if
    
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

    BlDbRecordSet *cur = tick->mainCompany()->loadQuery ( "SELECT * FROM configuracion WHERE nombre='NombreEmpresa'" );
    if ( !cur->eof() )
        empresa.nombre = cur->value( "valor" );
    delete cur;
    
    cur = tick->mainCompany()->loadQuery ( "SELECT * FROM configuracion WHERE nombre='CIF'" );
    if ( !cur->eof() )
        empresa.nombre += "\n" + cur->value( "valor" );
    delete cur;

    cur = tick->mainCompany()->loadQuery ( "SELECT * FROM configuracion WHERE nombre='DireccionCompleta'" );
    if ( !cur->eof() )
        empresa.direccionCompleta = cur->value( "valor" );
    delete cur;

    cur = tick->mainCompany()->loadQuery ( "SELECT * FROM configuracion WHERE nombre='CodPostal'" );
    if ( !cur->eof() )
        empresa.codigoPostal = cur->value( "valor" ).toLatin1();
    delete cur;

    cur = tick->mainCompany()->loadQuery ( "SELECT * FROM configuracion WHERE nombre='Ciudad'" );
    if ( !cur->eof() )
        empresa.ciudad = cur->value( "valor" );
    delete cur;

    cur = tick->mainCompany()->loadQuery ( "SELECT * FROM configuracion WHERE nombre='Telefono'" );
    if ( !cur->eof() )
        empresa.telefono = cur->value( "valor" );
    delete cur;

    cur = tick->mainCompany()->loadQuery ( "SELECT * FROM configuracion WHERE nombre='Provincia'" );
    if ( !cur->eof() )
        empresa.provincia = cur->value( "valor" );
    delete cur;

    fecha.dia = QDate::currentDate().toString ( "d-M-yyyy" );
    fecha.hora = QTime::currentTime().toString ( "HH:mm" );

    trabajador.id = tick->dbValue ( "idtrabajador" );
    cur = tick->mainCompany()->loadQuery ( "SELECT * FROM trabajador WHERE idtrabajador=" + tick->dbValue ( "idtrabajador" ) );
    if ( !cur->eof() )
        trabajador.nombre = cur->value( "nomtrabajador" );
    
    delete cur;
    
    cur = tick->mainCompany()->loadQuery ( "SELECT * FROM cliente WHERE idcliente=" + tick->dbValue ( "idcliente" ) );
    if ( !cur->eof() ) {
        cliente.cif = cur->value( "cifcliente" ).toLatin1();
        cliente.nombre = cur->value( "nomcliente" ).toLatin1();
    } // end if
    delete cur;

    cur = tick->mainCompany()->loadQuery ( "SELECT * FROM almacen WHERE idalmacen=" + tick->dbValue ( "idalmacen" ) );
    if ( !cur->eof() )
        almacen.nombre = cur->value( "nomalmacen" ).toLatin1() ;
    delete cur;

    BlDbRecord *linea;
    
    if ( tick->listaLineas()->size() ) {
        total.iva = BlFixed ( tick->listaLineas()->at ( 0 )->dbValue ( "ivalalbaran" ) );
    }
        
    for ( int i = 0; i < tick->listaLineas()->size(); ++i ) {
        linea = tick->listaLineas()->at ( i );
        BlFixed cantidad = BlFixed ( linea->dbValue ( "cantlalbaran" ) );
        total.baseImponible = total.baseImponible + cantidad * BlFixed ( linea->dbValue ( "pvplalbaran" ) );
    } // end for
    
    total.totalIva = total.baseImponible + total.baseImponible * total.iva / BlFixed ( "100" );

    BlEscPrinter pr ( g_confpr->value(CONF_DIR_USER) + "bulmatpv_ticket_tc.esc" );
    pr.initializePrinter();
    pr.setCharacterCodeTable ( page19 );
    pr.setJustification ( BlEscPrinter::center );

    if ( g_confpr->value( CONF_TPV_PRINTER_LOGO ) != "" ) {
        pr.printImage ( g_confpr->value( CONF_TPV_PRINTER_LOGO ) );
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
    pr.printText ( "Cliente, " + cliente.cif + " " + cliente.nombre + "\n" );
    pr.printText ( "\n" );

    pr.turnWhiteBlack ( 1 );
    pr.printText ( " Uds. PRODUCTO � � � � � �    P.U. IMPORTE\n" );

    pr.turnWhiteBlack ( 0 );
    pr.setCharacterPrintMode ( CHARACTER_FONTB_SELECTED );
    pr.setCharacterSize ( CHAR_WIDTH_1 | CHAR_HEIGHT_1 );

    for ( int i = 0; i < tick->listaLineas() ->size(); ++i ) {
        if ( i == tick->listaLineas()->size() - 1 )
            pr.setUnderlineMode ( 1 );
            
        linea = tick->listaLineas() ->at ( i );
        BlFixed iva = BlFixed ( linea->dbValue ( "ivalalbaran" ) );
        BlFixed pvp = BlFixed ( linea->dbValue ( "pvplalbaran" ) );
        pvp = pvp + pvp * iva / BlFixed ( "100" );
        BlFixed pvptotal = BlFixed ( linea->dbValue ( "cantlalbaran" ) ) * pvp;
        pr.printText ( linea->dbValue ( "cantlalbaran" ).rightJustified ( 5, ' ', true ) + "  " );
        pr.printText ( linea->dbValue ( "desclalbaran" ).leftJustified ( 27, ' ', true ) + " " );
        QString pvpstr = pvp.toQString();
        QString pvptotalstr = pvptotal.toQString();
        pr.printText ( QString ( pvpstr + "�" ).rightJustified ( 10, ' ', true ) + " " );
        pr.printText ( QString ( pvptotalstr + "�" ).rightJustified ( 10, ' ', true ) );
        pr.printText ( "\n" );
    } // end for
    
    pr.setUnderlineMode ( 0 );
    pr.setJustification ( BlEscPrinter::right );
    pr.setCharacterPrintMode ( CHARACTER_FONTA_SELECTED );
    pr.printText ( "Base Imponible: " + total.baseImponible.toQString() + "�\n" );
    pr.printText ( "IVA " + total.iva.toQString() + "%:" + ( total.totalIva - total.baseImponible ).toQString() + "�\n" );
    pr.setCharacterPrintMode ( CHARACTER_FONTA_SELECTED | EMPHASIZED_MODE | DOUBLE_HEIGHT | DOUBLE_WIDTH );
    pr.printText ( "TOTAL: " + total.totalIva.toQString() + "�\n" );
    pr.printText ( "\n\n" );
    pr.setJustification ( BlEscPrinter::left );
    pr.setCharacterPrintMode ( CHARACTER_FONTA_SELECTED );
    pr.printText ( "Le ha atendido " + trabajador.nombre + "\n" );
    pr.printText ( "\n" );
    pr.printText ( "Tel. " + empresa.telefono + "\n" );
    pr.printText ( "\n" );
    pr.setJustification ( BlEscPrinter::center );
    pr.setColor ( red );
    pr.printText ( "*** GRACIAS POR SU VISITA ***\n" );
    QByteArray qba = tick->dbValue ( "refalbaran" ).toLatin1();
    char* barcode = qba.data();
    pr.setJustification ( BlEscPrinter::center );
    pr.setBarcodeFormat ( 2, 50, both, fontB );
    pr.printBarCode ( code39, qba.size(), barcode );
    pr.cutPaperAndFeed ( true, 10 );
    pr.print();
    
    /// Si queremos imprimir con CUPS lo hacemos de esta otra forma
    if (!g_confpr->value( CONF_TICKET_PRINTER_FILE).isEmpty() && g_confpr->value( CONF_TICKET_PRINTER_FILE) != "/dev/null") {
        QString comando = "cat " + g_confpr->value(CONF_DIR_USER) + "bulmatpv_ticket_tc.esc" + "  > " + g_confpr->value( CONF_TICKET_PRINTER_FILE );
        system ( comando.toLatin1().data() );
    } else if (g_confpr->value(CONF_CUPS_DEFAULT_PRINTER).isEmpty() || g_confpr->value(CONF_CUPS_DEFAULT_PRINTER) == "None") {
        BlDebug::blDebug("Debe establecer el parametro CONF_CUPS_DEFAULT_PRINTER o CONF_TICKET_PRINTER_FILE para imprimir el ticket " , 2);
    } else {
		blRawPrint ("bulmatpv_ticket_tc.esc");
    } // end if
    
    BlDebug::blDebug("END pluginbt_aliastallasycolores::BtTicket_imprimir",0);
    return -1;
}


int BtTicket_imprimirIVAInc(BtTicket *tick)
{
    BL_FUNC_DEBUG
    
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
	if (cur->value("idcliente") != g_confpr->value(CONF_IDCLIENTE_DEFECTO)) {
        cliente.cif = cur->value( "cifcliente" ).toLatin1();
        cliente.nombre = cur->value( "nomcliente" ).toLatin1();
	} else {
	cliente.cif = "";
        cliente.nombre = "";
	} // end if
    } // end if
    delete cur;

    cur = tick->mainCompany() ->loadQuery ( "SELECT * FROM almacen WHERE idalmacen=" + tick->dbValue ( "idalmacen" ) );
    if ( !cur->eof() )
        almacen.nombre = cur->value( "nomalmacen" ).toLatin1() ;
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

    BlEscPrinter pr ( g_confpr->value(CONF_DIR_USER) + "bulmatpv_ticket_tc_ivainc.esc" );
    pr.initializePrinter();
    pr.setCharacterCodeTable ( page19 );
    pr.setJustification ( BlEscPrinter::center );

    if ( g_confpr->value( CONF_TPV_PRINTER_LOGO ) != "" ) {
        pr.printImage ( g_confpr->value( CONF_TPV_PRINTER_LOGO ) );
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
        BlFixed pvp = BlFixed ( linea->dbValue ( "pvpivainclalbaran" ) );	
        BlFixed pvptotal = BlFixed ( linea->dbValue ( "cantlalbaran" ) ) * pvp;
        pr.printText ( linea->dbValue ( "cantlalbaran" ).rightJustified ( 3, ' ', true ) + " " );
        pr.printText ( linea->dbValue ( "desclalbaran" ).leftJustified ( 21, ' ', true ) + " " );
        pr.printText ( linea->dbValue ( "nomtc_talla" ).leftJustified ( 3, ' ', true ) + " " );
        pr.printText ( linea->dbValue ( "nomtc_color" ).leftJustified ( 3, ' ', true ) + " " );	
		
        QString pvpstr = pvp.toQString();
        QString pvptotalstr = pvptotal.toQString();
        pr.printText ( QString ( pvpstr + "�" ).rightJustified ( 10, ' ', true ) + " " );
        pr.printText ( QString ( pvptotalstr + "�" ).rightJustified ( 10, ' ', true ) );
        pr.printText ( "\n" );
    } // end for
    pr.setUnderlineMode ( 0 );
    pr.setJustification ( BlEscPrinter::right );
    pr.setCharacterPrintMode ( CHARACTER_FONTA_SELECTED );    
    base::Iterator it;
    for ( it = totales.begin(); it != totales.end(); ++it ) {
      QString tipoIva = it.key();	
      QString sqlquery = "SELECT (" +it.value().toQString('.') + "/ ( 1 + " + tipoIva.replace(",",".") + "/100 ))::NUMERIC(12,2) AS base, " + it.value().toQString('.') + "- ("+it.value().toQString('.') + "/ ( 1 + " + tipoIva.replace(",",".") + "/100 ))::NUMERIC(12,2) AS iva";
      BlDbRecordSet *cur = tick->mainCompany()->loadQuery(sqlquery);
      pr.printText ( "Base Imponible: " + cur->value("base") + "�\n" );
      pr.printText ( "IVA " +it.key() + "%  " + cur->value("iva") + "�\n" );
      delete cur;
    } // end for
    pr.setCharacterPrintMode ( CHARACTER_FONTA_SELECTED | EMPHASIZED_MODE | DOUBLE_HEIGHT | DOUBLE_WIDTH );
    pr.printText ( "TOTAL: " + total.totalIva.toQString() + "�\n" );
    pr.printText ( "\n\n" );
    pr.setJustification ( BlEscPrinter::left );
    pr.setCharacterPrintMode ( CHARACTER_FONTA_SELECTED );
    pr.printText ( "Le ha atendido " + trabajador.nombre + "\n" );
    pr.printText ( "\n" );
    pr.printText ( "\n" );
    pr.setJustification ( BlEscPrinter::center );
    pr.setColor ( red );
    pr.printText ( "*** GRACIAS POR SU VISITA ***\n" );
    QByteArray qba = tick->dbValue ( "refalbaran" ).toLatin1();
    char* barcode = qba.data();
    pr.setJustification ( BlEscPrinter::center );
    pr.setBarcodeFormat ( 2, 50, both, fontB );
    pr.printBarCode ( code39, qba.size(), barcode );
    pr.cutPaperAndFeed ( true, 10 );
    pr.print();

    /// Si queremos imprimir con CUPS lo hacemos de esta otra forma
    if (!g_confpr->value( CONF_TICKET_PRINTER_FILE).isEmpty() && g_confpr->value( CONF_TICKET_PRINTER_FILE) != "/dev/null") {
        QString comando = "cat " + g_confpr->value(CONF_DIR_USER) + "bulmatpv_ticket_tc_ivainc.esc" + "  > " + g_confpr->value( CONF_TICKET_PRINTER_FILE );
        system ( comando.toLatin1().data() );
    } else if (g_confpr->value(CONF_CUPS_DEFAULT_PRINTER).isEmpty() || g_confpr->value(CONF_CUPS_DEFAULT_PRINTER) == "None") {
        BlDebug::blDebug("Debe establecer el parametro CONF_CUPS_DEFAULT_PRINTER o CONF_TICKET_PRINTER_FILE para imprimir el ticket " , 2);
    } else {
        QString comando = "lp -d" + g_confpr->value(CONF_CUPS_DEFAULT_PRINTER) + " " + g_confpr->value(CONF_DIR_USER) + "bulmatpv_ticket_tc_ivainc.esc";
        system ( comando.toLatin1().data() );
    } // end if
    
    return 1;
}


