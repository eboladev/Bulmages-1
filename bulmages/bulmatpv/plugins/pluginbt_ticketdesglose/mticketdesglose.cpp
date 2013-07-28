
#include <QtWidgets/QLabel>
#include <QtWidgets/QTextBrowser>

#include "mticketdesglose.h"
#include "bldb.h"
#include "btbulmatpv.h"
#include "btsubform.h"


extern QTextBrowser *g_browser;

/// Una factura puede tener multiples bases imponibles. Por eso definimos el tipo base
/// como un QMap.
typedef QMap<QString, BlFixed> base;


MTicketDesglose::MTicketDesglose ( BtCompany *emp, QWidget *parent ) : QTextBrowser ( parent ) , BlMainCompanyPointer ( emp )
{
    BL_FUNC_DEBUG
//    emp->pWorkspace() ->addSubWindow ( this );
//    setWindowTitle ( "Ticket" );
    
}


MTicketDesglose::~MTicketDesglose()
{
    BL_FUNC_DEBUG
    
}


void MTicketDesglose::pintar()
{
    BL_FUNC_DEBUG
    BtTicket *tick =     ( ( BtCompany * ) mainCompany() ) ->ticketActual();
    //QString html = "<font size=\"1\">";
    QString html = "<p style=\"font-family:monospace; font-size: 12pt;\">";
    QString html1 = "<font size=\"1\">";

    html1 += "Ticket: " + tick->dbValue ( "nomticket" ) + "<BR>";

    QString querytrab = "SELECT * FROM trabajador WHERE idtrabajador = " + tick->dbValue ( "idtrabajador" );
    BlDbRecordSet *curtrab = mainCompany() ->loadQuery ( querytrab );
    html1 += "Trabajador: " + tick->dbValue ( "idtrabajador" ) + " " + curtrab->value( "nomtrabajador" ) + "<BR>";
    delete curtrab;
    QString query = "SELECT * FROM cliente WHERE idcliente = " + tick->dbValue ( "idcliente" );
    BlDbRecordSet *cur1 = mainCompany() ->loadQuery ( query );
    html1 += "Cliente: " + tick->dbValue ( "idcliente" ) + " " + cur1->value( "nomcliente" ) + "<BR>";
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
        html += "<TD bgcolor=\"" + bgcolor + "\" align=\"right\" width=\"50\">" + totalLinea.toQString() + "</TD>";
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

    BlDbRecordSet *cur = mainCompany() ->loadQuery ( "SELECT * FROM configuracion WHERE nombre = 'IRPF'" );
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
    /*
        BlDbSubFormRecord *linea1;
        if (m_listadescuentos->rowCount()) {
            for (int i = 0; i < m_listadescuentos->rowCount(); ++i) {
                linea1 = m_listadescuentos->lineaat(i);
                BlFixed propor(linea1->dbValue("proporcion" + m_listadescuentos->tableName()).toLatin1().constData());
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
        BlFixed piva ( it.key().toLatin1().constData() );
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
        BlFixed preqeq ( it.key().toLatin1().constData() );
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
    setText ( html1 );
    
}

