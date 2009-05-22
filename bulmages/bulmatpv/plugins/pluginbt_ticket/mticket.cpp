/***************************************************************************
 *   Copyright (C) 2005 by Tomeu Borras Riera                              *
 *   tborras@conetxia.com                                                  *
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

#include <QLabel>
#include <QTextBrowser>

#include "mticket.h"
#include "bldb.h"
#include "btbulmatpv.h"
#include "btsubform.h"

/// Una factura puede tener multiples bases imponibles. Por eso definimos el tipo base
/// como un QMap.
typedef QMap<QString, BlFixed> base;

MTicket::MTicket ( BtCompany *emp, QWidget *parent ) : BlWidget ( emp, parent )
{
    _depura ( "MTicket::MTicket", 0 );
    setupUi ( this );
    setFocusPolicy ( Qt::NoFocus );
    emp->pWorkspace() ->addWindow ( this );
    setWindowTitle ( "Ticket" );
    _depura ( "END MTicket::MTicket", 0 );
}

MTicket::~MTicket()
{
    _depura ( "MTicket::~MTicket", 0 );
    _depura ( "END MTicket::~MTicket", 0 );
}

void MTicket::pintar()
{
    _depura ( "MTicket::pintar", 0 );

    if ( g_plugins->lanza ( "MTicket_pintar", this ) ) {
        _depura ( "END MTicket::pintar", 0 );
        return;
    } // end if

    BtTicket *tick =     ( ( BtCompany * ) mainCompany() ) ->ticketActual();
    //QString html = "<font size=\"1\">";
    QString html = "<p style=\"font-family:monospace; font-size: 12pt;\">";
    QString html1 = "<font size=\"1\">";

    html1 += "Ticket: " + tick->dbValue ( "nomticket" ) + "<BR>";

    QString querytrab = "SELECT * FROM trabajador WHERE idtrabajador = " + tick->dbValue ( "idtrabajador" );
    BlDbRecordSet *curtrab = mainCompany() ->loadQuery ( querytrab );
    html1 += "Trabajador: " + tick->dbValue ( "idtrabajador" ) + " " + curtrab->valor ( "nomtrabajador" ) + "<BR>";
    delete curtrab;
    
    QString query = "SELECT * FROM cliente WHERE idcliente = " + tick->dbValue ( "idcliente" );
    BlDbRecordSet *cur1 = mainCompany() ->loadQuery ( query );
    html1 += "Cliente: " + tick->dbValue ( "idcliente" ) + " " + cur1->valor ( "nomcliente" ) + "<BR>";
    delete cur1;

    html += "<TABLE border=\"0\">";
    BlDbRecord *item;
    
    for ( int i = 0; i < tick->listaLineas() ->size(); ++i ) {
        item = tick->listaLineas() ->at ( i );
        QString bgcolor = "#FFFFFF";
        if ( item == tick->lineaActBtTicket() ) bgcolor = "#CCCCFF";
        html += "<TR>";
        html += "<TD bgcolor=\"" + bgcolor + "\" align=\"right\" width=\"50\">" + item->dbValue ( "cantlalbaran" ) + "</TD>";
        html += "<TD bgcolor=\"" + bgcolor + "\">" + item->dbValue ( "nomarticulo" ) + "</TD>";
        BlFixed totalLinea ( "0.00" );
        totalLinea = BlFixed ( item->dbValue ( "cantlalbaran" ) ) * BlFixed ( item->dbValue ( "pvplalbaran" ) );
        html += "<TD bgcolor=\"" + bgcolor + "\" align=\"right\" width=\"50\">" + totalLinea.toQString(); + "</TD>";
        html += "</TR>";
    } // end for
    
    html += "</TABLE>";

// ======================================
    
    html += "<BR><HR><BR>";
    base basesimp;
    base basesimpreqeq;
    BlDbRecord *linea;
    
    /// Impresion de los contenidos.
    QString l;
    BlFixed irpf ( "0" );

    BlDbRecordSet *cur = mainCompany()->loadQuery ( "SELECT * FROM configuracion WHERE nombre = 'IRPF'" );
    
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
    mui_browser->setText ( html );
    
    _depura ( "END MTicket::pintar", 0 );
}

void MTicket::on_mui_subir_released()
{
    _depura ( "MTicket::on_mui_subir_released", 0 );
    
    /// Simulamos la pulsacion de la tecla arriba
    ( ( BtCompany * ) mainCompany() )->pulsaTecla ( Qt::Key_Up );
    
    _depura ( "END MTicket::on_mui_subir_released", 0 );
}

void MTicket::on_mui_bajar_released()
{
     _depura ( "MTicket::on_mui_bajar_released", 0 );
    
    /// Simulamos la pulsacion de la tecla abajo
    ( ( BtCompany * ) mainCompany() )->pulsaTecla ( Qt::Key_Down );
    
    _depura ( "END MTicket::on_mui_bajar_released", 0 );
}

void MTicket::on_mui_borrar_released()
{
     _depura ( "MTicket::on_mui_borrar_released", 0 );
    
    BtTicket * tick = ( ( BtCompany * ) mainCompany() )->ticketActual();
    tick->ponerCantidad ( "0" );

    pintar();
    
    _depura ( "END MTicket::on_mui_borrar_released", 0 );
}

void MTicket::on_mui_imprimir_released()
{
     _depura ( "MTicket::on_mui_imprimir_released", 0 );
    
    /// Llamamos al atajo de teclado que llama a BtTicket::imprimir()
    ( ( BtCompany * ) mainCompany() )->pulsaTecla ( Qt::Key_F2 );
    
    _depura ( "END MTicket::on_mui_imprimir_released", 0 );
}

void MTicket::on_mui_reimprimir_released()
{
    _depura ( "MTicket::on_mui_reimprimir_released", 0 );
    
    BtTicket *previousTicket = new BtTicket( ( BtCompany * ) mainCompany() );
    BlDbRecordSet *cur = mainCompany()->loadQuery ( "SELECT * FROM albaran WHERE ticketalbaran = TRUE ORDER BY idalbaran DESC LIMIT 1" );
    
    previousTicket->setDbValue("idalbaran", cur->valor("idalbaran"));
    
    // Cargamos las lineas de albaran
    cur = mainCompany()->loadQuery ( "SELECT * FROM lalbaran LEFT JOIN articulo ON lalbaran.idarticulo = articulo.idarticulo WHERE idalbaran = " + cur->valor("idalbaran") );
        
    while ( !cur->eof() ) {
        BlDbRecord *l = previousTicket->agregarLinea();
        l->DBload( cur );
        cur->nextRecord();
    } // end while
    
    delete cur;
    
    previousTicket->imprimir(FALSE);
    
    delete previousTicket;

    _depura ( "END MTicket::on_mui_reimprimir_released", 0 );
}
