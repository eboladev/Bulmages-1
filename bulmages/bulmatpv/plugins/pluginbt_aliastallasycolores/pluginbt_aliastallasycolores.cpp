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

#include "pluginbt_aliastallasycolores.h"
#include "blfunctions.h"
#include "btcompany.h"


/// Una factura puede tener multiples bases imponibles. Por eso definimos el tipo base
/// como un QMap.
typedef QMap<QString, BlFixed> base;


int Ticket_agregarLinea_Post ( BtTicket *tick, BlDbRecord * &rec )
{
    rec->addDbField ( "idtc_talla", BlDbField::DbNumeric, BlDbField::DbNothing, _ ( "Talla" ) );
    rec->addDbField ( "idtc_color", BlDbField::DbNumeric, BlDbField::DbNothing, _ ( "Color" ) );
    rec->addDbField ( "nomtc_talla", BlDbField::DbVarChar, BlDbField::DbNoSave, _ ( "Talla" ) );
    rec->addDbField ( "nomtc_color", BlDbField::DbVarChar, BlDbField::DbNoSave, _ ( "Color" ) );

}


int Ticket_insertarArticuloNL_Post ( BtTicket *tick )
{
    QString query = "SELECT * FROM tc_articulo_alias LEFT JOIN tc_talla AS t1 ON tc_articulo_alias.idtc_talla = t1.idtc_talla LEFT JOIN tc_color AS t2 ON tc_articulo_alias.idtc_color = t2.idtc_color WHERE aliastc_articulo_tallacolor = '" + ( ( BtCompany * ) tick->mainCompany() )->valorBtInput() + "'";
    BlDbRecordSet *cur = tick->mainCompany() ->loadQuery ( query );
    if ( !cur->eof() ) {
        BlDbRecord * rec = tick->insertarArticulo ( cur->valor ( "idarticulo" ), BlFixed ( "1" ), TRUE );
        rec->setDbValue ( "idtc_talla", cur->valor ( "idtc_talla" ) );
        rec->setDbValue ( "idtc_color", cur->valor ( "idtc_color" ) );
        rec->setDbValue ( "nomtc_talla", cur->valor ( "nomtc_talla" ) );
        rec->setDbValue ( "nomtc_color", cur->valor ( "nomtc_color" ) );
        tick->pintar();
    } // end if
    delete cur;

    return 0;
}



int Ticket_insertarArticulo_Post ( BtTicket *tick )
{
    int valor = -1;
    _depura ( "pluginbt_aliastallasycolores::Ticket_insertarArticulo_Post", 0 );
    static int semaforo = 0;
    if ( semaforo == 0 ) {
        semaforo = 1;
        QString query = "SELECT * FROM tc_articulo_alias LEFT JOIN tc_talla AS t1 ON tc_articulo_alias.idtc_talla = t1.idtc_talla LEFT JOIN tc_color AS t2 ON tc_articulo_alias.idtc_color = t2.idtc_color WHERE aliastc_articulo_tallacolor = '" + ( ( BtCompany * ) tick->mainCompany() )->valorBtInput() + "'";
        BlDbRecordSet *cur = tick->mainCompany() ->loadQuery ( query );
        if ( !cur->eof() ) {
            BlDbRecord * rec = tick->insertarArticulo ( cur->valor ( "idarticulo" ), BlFixed ( "1" ), TRUE );
            rec->setDbValue ( "idtc_talla", cur->valor ( "idtc_talla" ) );
            rec->setDbValue ( "idtc_color", cur->valor ( "idtc_color" ) );
            rec->setDbValue ( "nomtc_talla", cur->valor ( "nomtc_talla" ) );
            rec->setDbValue ( "nomtc_color", cur->valor ( "nomtc_color" ) );
        } // end if
        delete cur;
        tick->pintar();
        valor = 0;
        semaforo = 0;
    } // end if
    _depura ( "END PluginBt_AliasTallasYColores::Ticket_insertarArticulo_Post", 0 );
    return valor;
}


int MTicket_pintar ( MTicket *mtick )
{
    _depura ( "MTicket_pintar", 0 );

    BtTicket *tick =     ( ( BtCompany * ) mtick->mainCompany() ) ->ticketActual();
    //QString html = "<font size=\"1\">";
    QString html = "<p style=\"font-family:monospace; font-size: 12pt;\">";
    QString html1 = "<font size=\"1\">";

    html1 += "Ticket: " + tick->dbValue ( "nomticket" ) + "<BR>";

    QString querytrab = "SELECT * FROM trabajador WHERE idtrabajador = " + tick->dbValue ( "idtrabajador" );
    BlDbRecordSet *curtrab = mtick->mainCompany() ->loadQuery ( querytrab );
    html1 += "Trabajador: " + tick->dbValue ( "idtrabajador" ) + " " + curtrab->valor ( "nomtrabajador" ) + "<BR>";
    delete curtrab;
    QString query = "SELECT * FROM cliente WHERE idcliente = " + tick->dbValue ( "idcliente" );
    BlDbRecordSet *cur1 = mtick->mainCompany() ->loadQuery ( query );
    html1 += "Cliente: " + tick->dbValue ( "idcliente" ) + " " + cur1->valor ( "nomcliente" ) + "<BR>";
    delete cur1;

    html += "<TABLE border=\"0\">";
    BlDbRecord *item;
    for ( int i = 0; i < tick->listaLineas() ->size(); ++i ) {
        item = tick->listaLineas() ->at ( i );
        QString bgcolor = "#FFFFFF";
        if ( item == tick->lineaActBtTicket() ) bgcolor = "#CCFFFF";
        html += "<TR>";
        html += "<TD bgcolor=\"" + bgcolor + "\" align=\"right\" width=\"50\">" + item->dbValue ( "cantlalbaran" ) + "</TD>";
        html += "<TD bgcolor=\"" + bgcolor + "\">" + item->dbValue ( "nomarticulo" ) + "</TD>";

        html += "<TD bgcolor=\"" + bgcolor + "\">" + item->dbValue ( "nomtc_talla" ) + "</TD>";
        html += "<TD bgcolor=\"" + bgcolor + "\">" + item->dbValue ( "nomtc_color" ) + "</TD>";

        BlFixed totalLinea ( "0.00" );
        totalLinea = BlFixed ( item->dbValue ( "cantlalbaran" ) ) * BlFixed ( item->dbValue ( "pvplalbaran" ) );
        html += "<TD bgcolor=\"" + bgcolor + "\" align=\"right\" width=\"50\">" + totalLinea.toQString(); + "</TD>";
        html += "</TR>";
    }// end for
    html += "</TABLE>";

// ======================================
    html += "<BR><HR><BR>";
    base basesimp;
    base basesimpreqeq;
    BlDbRecord *linea;
    /// Impresion de los contenidos.
    QString l;
    BlFixed irpf ( "0" );

    BlDbRecordSet *cur = mtick->mainCompany() ->loadQuery ( "SELECT * FROM configuracion WHERE nombre = 'IRPF'" );
    if ( cur ) {
        if ( !cur->eof() ) {
            irpf = BlFixed ( cur->valor ( "valor" ) );
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
    /*
        BlDbSubFormRecord *linea1;
        if (m_listadescuentos->rowCount()) {
            for (int i = 0; i < m_listadescuentos->rowCount(); ++i) {
                linea1 = m_listadescuentos->lineaat(i);
                BlFixed propor(linea1->dbValue("proporcion" + m_listadescuentos->tableName()).toAscii().constData());
                porcentt = porcentt + propor;
            } // end for
        } // end if
    */

    /// Calculamos el total de base imponible.
    BlFixed totbaseimp ( "0.00" );
    BlFixed parbaseimp ( "0.00" );
    for ( it = basesimp.begin(); it != basesimp.end(); ++it ) {
        if ( porcentt > BlFixed ( "0.00" ) ) {
            parbaseimp = it.value() - it.value() * porcentt / 100;
        } else {
            parbaseimp = it.value();
        } // end if
        html1 += "Base Imp " + it.key() + "% " + parbaseimp.toQString() + "<BR>";
        totbaseimp = totbaseimp + parbaseimp;
    } // end for

    /// Calculamos el total de IVA.
    BlFixed totiva ( "0.00" );
    BlFixed pariva ( "0.00" );
    for ( it = basesimp.begin(); it != basesimp.end(); ++it ) {
        BlFixed piva ( it.key().toAscii().constData() );
        if ( porcentt > BlFixed ( "0.00" ) ) {
            pariva = ( it.value() - it.value() * porcentt / 100 ) * piva / 100;
        } else {
            pariva = it.value() * piva / 100;
        } // end if
        html1 += "IVA " + it.key() + "% " + pariva.toQString() + "<BR>";
        totiva = totiva + pariva;
    } // end for

    /// Calculamos el total de recargo de equivalencia.
    BlFixed totreqeq ( "0.00" );
    BlFixed parreqeq ( "0.00" );
    for ( it = basesimpreqeq.begin(); it != basesimpreqeq.end(); ++it ) {
        BlFixed preqeq ( it.key().toAscii().constData() );
        if ( porcentt > BlFixed ( "0.00" ) ) {
            parreqeq = ( it.value() - it.value() * porcentt / 100 ) * preqeq / 100;
        } else {
            parreqeq = it.value() * preqeq / 100;
        } // end if
        html1 += "R.Eq " + it.key() + "% " + parreqeq.toQString() + "<BR>";
        totreqeq = totreqeq + parreqeq;
    } // end for



    BlFixed totirpf = totbaseimp * irpf / 100;

    html1 += "<B>Base Imp. " + totbaseimp.toQString() + "<BR>";
    html1 += "<B>IVA. " + totiva.toQString() + "<BR>";
    html1 += "<B>IRPF. " + totirpf.toQString() + "<BR>";

    BlFixed total = totiva + totbaseimp + totreqeq - totirpf;
    html1 += "<B>Total: " + total.toQString() + "<BR>";



    html += "</p>";
    html1 += "</FONT>";

// ======================================
    /// Pintamos el HTML en el textBrowser
    mtick->mui_browser->setText ( html );
    _depura ( "END MTicket::pintar", 0 );
    return -1;
}
