#include "devolucion.h"
#include "fixed.h"
#include <QWidget>

/// Una factura puede tener multiples bases imponibles. Por eso definimos el tipo base
/// como un QMap.
typedef QMap<QString, Fixed> base;

Devolucion::Devolucion ( EmpresaTPV *emp, QWidget *parent ) : BLWidget ( emp, parent )
{
    setupUi ( this );

    m_ticket = NULL;
    /*
        m_value = 0;
        base basesimp;
        base basesimpreqeq;
        DBRecord *linea;

        Ticket *tick = emp->ticketActual();

        /// Impresion de los contenidos.
        QString l;
        Fixed irpf ( "0" );

        cursor2 *cur = emp->cargacursor ( "SELECT * FROM configuracion WHERE nombre = 'IRPF'" );
        if ( cur ) {
            if ( !cur->eof() ) {
                irpf = Fixed ( cur->valor ( "valor" ) );
            } // end if
            delete cur;
        } // end if


        Fixed descuentolinea ( "0.00" );
        for ( int i = 0; i < tick->listaLineas() ->size(); ++i ) {
            linea = tick->listaLineas() ->at ( i );
            Fixed cant ( linea->DBvalue ( "cantlalbaran" ) );
            Fixed pvpund ( linea->DBvalue ( "pvplalbaran" ) );
            Fixed desc1 ( linea->DBvalue ( "descuentolalbaran" ) );
            Fixed cantpvp = cant * pvpund;
            Fixed base = cantpvp - cantpvp * desc1 / 100;
            descuentolinea = descuentolinea + ( cantpvp * desc1 / 100 );
            basesimp[linea->DBvalue ( "ivalalbaran" ) ] = basesimp[linea->DBvalue ( "ivalalbaran" ) ] + base;
            basesimpreqeq[linea->DBvalue ( "reqeqlalbaran" ) ] = basesimpreqeq[linea->DBvalue ( "reqeqlalbaran" ) ] + base;
        } // end for

        Fixed basei ( "0.00" );
        base::Iterator it;
        for ( it = basesimp.begin(); it != basesimp.end(); ++it ) {
            basei = basei + it.value();
        } // end for

        /// Calculamos el total de los descuentos.
        /// De momento aqui no se usan descuentos generales en venta.
        Fixed porcentt ( "0.00" );

        /// Calculamos el total de base imponible.
        Fixed totbaseimp ( "0.00" );
        Fixed parbaseimp ( "0.00" );
        for ( it = basesimp.begin(); it != basesimp.end(); ++it ) {
            if ( porcentt > Fixed ( "0.00" ) ) {
                parbaseimp = it.value() - it.value() * porcentt / 100;
            } else {
                parbaseimp = it.value();
            } // end if
            totbaseimp = totbaseimp + parbaseimp;
        } // end for

        /// Calculamos el total de IVA.
        Fixed totiva ( "0.00" );
        Fixed pariva ( "0.00" );
        for ( it = basesimp.begin(); it != basesimp.end(); ++it ) {
            Fixed piva ( it.key().toAscii().constData() );
            if ( porcentt > Fixed ( "0.00" ) ) {
                pariva = ( it.value() - it.value() * porcentt / 100 ) * piva / 100;
            } else {
                pariva = it.value() * piva / 100;
            } // end if
            totiva = totiva + pariva;
        } // end for

        /// Calculamos el total de recargo de equivalencia.
        Fixed totreqeq ( "0.00" );
        Fixed parreqeq ( "0.00" );
        for ( it = basesimpreqeq.begin(); it != basesimpreqeq.end(); ++it ) {
            Fixed preqeq ( it.key().toAscii().constData() );
            if ( porcentt > Fixed ( "0.00" ) ) {
                parreqeq = ( it.value() - it.value() * porcentt / 100 ) * preqeq / 100;
            } else {
                parreqeq = it.value() * preqeq / 100;
            } // end if
            totreqeq = totreqeq + parreqeq;
        } // end for

        Fixed totirpf = totbaseimp * irpf / 100;

        Fixed total = totiva + totbaseimp + totreqeq - totirpf;

        mui_total->setText(total.toQString());
    */
}


Devolucion::~Devolucion()
{}



void Devolucion::on_mui_cancelar_clicked()
{
    m_value = -1;
    ( ( QDialog * ) parent() )->accept();
}

void Devolucion::on_mui_ref_returnPressed()
{
    if ( m_ticket )  {
        delete m_ticket;
        m_ticket = NULL;
    } // end if

    QString query1 = "SELECT * FROM albaran WHERE refalbaran = '" + mui_ref->text() + "'";

    cursor2 *curs = empresaBase()->cargacursor ( query1 );
    if ( !curs->eof() ) {
        m_ticket = new Ticket ( empresaBase(), NULL );
        m_ticket->cargar ( curs->valor ( "idalbaran" ) );
    }
    delete curs;

    if ( !m_ticket ) return;

// ====================== PINTAMOS ========================
    QString html = "<p style=\"font-family:monospace; font-size: 12pt;\">";
    QString html1 = "<font size=\"1\">";

    html1 += "Ticket: " + m_ticket->DBvalue ( "nomticket" ) + "<BR>";

    QString querytrab = "SELECT * FROM trabajador WHERE idtrabajador = " + m_ticket->DBvalue ( "idtrabajador" );
    cursor2 *curtrab = empresaBase() ->cargacursor ( querytrab );
    html1 += "Trabajador: " + m_ticket->DBvalue ( "idtrabajador" ) + " " + curtrab->valor ( "nomtrabajador" ) + "<BR>";
    delete curtrab;
    QString query = "SELECT * FROM cliente WHERE idcliente = " + m_ticket->DBvalue ( "idcliente" );
    cursor2 *cur1 = empresaBase() ->cargacursor ( query );
    html1 += "Cliente: " + m_ticket->DBvalue ( "idcliente" ) + " " + cur1->valor ( "nomcliente" ) + "<BR>";
    delete cur1;

    html += "<TABLE border=\"0\">";
    DBRecord *item;
    for ( int i = 0; i < m_ticket->listaLineas() ->size(); ++i ) {
        item = m_ticket->listaLineas() ->at ( i );
        QString bgcolor = "#FFFFFF";
        if ( item == m_ticket->lineaActTicket() ) bgcolor = "#CCCCFF";
        html += "<TR>";
        html += "<TD bgcolor=\"" + bgcolor + "\" align=\"right\" width=\"50\">" + item->DBvalue ( "cantlalbaran" ) + "</TD>";
        html += "<TD bgcolor=\"" + bgcolor + "\">" + item->DBvalue ( "nomarticulo" ) + "</TD>";
        Fixed totalLinea ( "0.00" );
        totalLinea = Fixed ( item->DBvalue ( "cantlalbaran" ) ) * Fixed ( item->DBvalue ( "pvplalbaran" ) );
        html += "<TD bgcolor=\"" + bgcolor + "\" align=\"right\" width=\"50\">" + totalLinea.toQString(); + "</TD>";
        html += "</TR>";
    }// end for
    html += "</TABLE>";

// ======================================
    html += "<BR><HR><BR>";
    base basesimp;
    base basesimpreqeq;
    DBRecord *linea;
    /// Impresion de los contenidos.
    QString l;
    Fixed irpf ( "0" );

    cursor2 *cur = empresaBase() ->cargacursor ( "SELECT * FROM configuracion WHERE nombre = 'IRPF'" );
    if ( cur ) {
        if ( !cur->eof() ) {
            irpf = Fixed ( cur->valor ( "valor" ) );
        } // end if
        delete cur;
    } // end if


    Fixed descuentolinea ( "0.00" );
    for ( int i = 0; i < m_ticket->listaLineas() ->size(); ++i ) {
        linea = m_ticket->listaLineas() ->at ( i );
        Fixed cant ( linea->DBvalue ( "cantlalbaran" ) );
        Fixed pvpund ( linea->DBvalue ( "pvplalbaran" ) );
        Fixed desc1 ( linea->DBvalue ( "descuentolalbaran" ) );
        Fixed cantpvp = cant * pvpund;
        Fixed base = cantpvp - cantpvp * desc1 / 100;
        descuentolinea = descuentolinea + ( cantpvp * desc1 / 100 );
        basesimp[linea->DBvalue ( "ivalalbaran" ) ] = basesimp[linea->DBvalue ( "ivalalbaran" ) ] + base;
        basesimpreqeq[linea->DBvalue ( "reqeqlalbaran" ) ] = basesimpreqeq[linea->DBvalue ( "reqeqlalbaran" ) ] + base;
    } // end for

    Fixed basei ( "0.00" );
    base::Iterator it;
    for ( it = basesimp.begin(); it != basesimp.end(); ++it ) {
        basei = basei + it.value();
    } // end for

    /// Calculamos el total de los descuentos.
    /// De momento aqui no se usan descuentos generales en venta.
    Fixed porcentt ( "0.00" );
    /*
        SDBRecord *linea1;
        if (m_listadescuentos->rowCount()) {
            for (int i = 0; i < m_listadescuentos->rowCount(); ++i) {
                linea1 = m_listadescuentos->lineaat(i);
                Fixed propor(linea1->DBvalue("proporcion" + m_listadescuentos->tableName()).toAscii().constData());
                porcentt = porcentt + propor;
            } // end for
        } // end if
    */

    /// Calculamos el total de base imponible.
    Fixed totbaseimp ( "0.00" );
    Fixed parbaseimp ( "0.00" );
    for ( it = basesimp.begin(); it != basesimp.end(); ++it ) {
        if ( porcentt > Fixed ( "0.00" ) ) {
            parbaseimp = it.value() - it.value() * porcentt / 100;
        } else {
            parbaseimp = it.value();
        } // end if
        html1 += "Base Imp " + it.key() + "% " + parbaseimp.toQString() + "<BR>";
        totbaseimp = totbaseimp + parbaseimp;
    } // end for

    /// Calculamos el total de IVA.
    Fixed totiva ( "0.00" );
    Fixed pariva ( "0.00" );
    for ( it = basesimp.begin(); it != basesimp.end(); ++it ) {
        Fixed piva ( it.key().toAscii().constData() );
        if ( porcentt > Fixed ( "0.00" ) ) {
            pariva = ( it.value() - it.value() * porcentt / 100 ) * piva / 100;
        } else {
            pariva = it.value() * piva / 100;
        } // end if
        html1 += "IVA " + it.key() + "% " + pariva.toQString() + "<BR>";
        totiva = totiva + pariva;
    } // end for

    /// Calculamos el total de recargo de equivalencia.
    Fixed totreqeq ( "0.00" );
    Fixed parreqeq ( "0.00" );
    for ( it = basesimpreqeq.begin(); it != basesimpreqeq.end(); ++it ) {
        Fixed preqeq ( it.key().toAscii().constData() );
        if ( porcentt > Fixed ( "0.00" ) ) {
            parreqeq = ( it.value() - it.value() * porcentt / 100 ) * preqeq / 100;
        } else {
            parreqeq = it.value() * preqeq / 100;
        } // end if
        html1 += "R.Eq " + it.key() + "% " + parreqeq.toQString() + "<BR>";
        totreqeq = totreqeq + parreqeq;
    } // end for



    Fixed totirpf = totbaseimp * irpf / 100;

    html1 += "<B>Base Imp. " + totbaseimp.toQString() + "<BR>";
    html1 += "<B>IVA. " + totiva.toQString() + "<BR>";
    html1 += "<B>IRPF. " + totirpf.toQString() + "<BR>";

    Fixed total = totiva + totbaseimp + totreqeq - totirpf;
    html1 += "<B>Total: " + total.toQString() + "<BR>";



    html += "</p>";
    html1 += "</FONT>";

// ======================================
    /// Pintamos el HTML en el textBrowser
    mui_browser->setText ( html );

}