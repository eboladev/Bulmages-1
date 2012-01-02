#include <QWidget>

#include "devolucion.h"
#include "blfixed.h"
#include "blescprinter.h"


/// Una factura puede tener multiples bases imponibles. Por eso definimos el tipo base
/// como un QMap.
typedef QMap<QString, BlFixed> base;


Devolucion::Devolucion ( BtCompany *emp, QWidget *parent ) : BlWidget ( emp, parent )
{
    setupUi ( this );

    m_ticket = NULL;
    mui_browser->setOpenLinks ( FALSE );
    m_totalin = "";
}


Devolucion::~Devolucion()
{}

void Devolucion::on_mui_devolver_clicked()
{
    m_ticket->save();
    m_ticket->imprimir();
    ( ( QDialog * ) parent() )->accept();
}


void Devolucion::on_mui_cancelar_clicked()
{
    m_value = -1;
    ( ( QDialog * ) parent() )->accept();
}

void Devolucion::on_mui_vale_clicked()
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
        BlFixed iva;
        BlFixed baseImponible;
        BlFixed totalIva;
    }total;

    BlDbRecordSet *cur = mainCompany() ->loadQuery ( "SELECT * FROM configuracion WHERE nombre='NombreEmpresa'" );
    if ( !cur->eof() )
        empresa.nombre = cur->value( "valor" );
    delete cur;

    cur = mainCompany() ->loadQuery ( "SELECT * FROM configuracion WHERE nombre='DireccionCompleta'" );
    if ( !cur->eof() )
        empresa.direccionCompleta = cur->value( "valor" );
    delete cur;

    cur = mainCompany() ->loadQuery ( "SELECT * FROM configuracion WHERE nombre='CodPostal'" );
    if ( !cur->eof() )
        empresa.codigoPostal = cur->value( "valor" ).toAscii();
    delete cur;

    cur = mainCompany() ->loadQuery ( "SELECT * FROM configuracion WHERE nombre='Ciudad'" );
    if ( !cur->eof() )
        empresa.ciudad = cur->value( "valor" );
    delete cur;

    cur = mainCompany() ->loadQuery ( "SELECT * FROM configuracion WHERE nombre='Telefono'" );
    if ( !cur->eof() )
        empresa.telefono = cur->value( "valor" );
    delete cur;

    cur = mainCompany() ->loadQuery ( "SELECT * FROM configuracion WHERE nombre='Provincia'" );
    if ( !cur->eof() )
        empresa.provincia = cur->value( "valor" );
    delete cur;

    fecha.dia = QDate::currentDate().toString ( "d-M-yyyy" );
    fecha.hora = QTime::currentTime().toString ( "HH:mm" );

    trabajador.id = m_ticket->dbValue ( "idtrabajador" );
    cur = mainCompany() ->loadQuery ( "SELECT * FROM trabajador WHERE idtrabajador=" + m_ticket->dbValue ( "idtrabajador" ) );
    if ( !cur->eof() )
        trabajador.nombre = cur->value( "nomtrabajador" );
    delete cur;

    cur = mainCompany() ->loadQuery ( "SELECT * FROM cliente WHERE idcliente=" + m_ticket->dbValue ( "idcliente" ) );
    if ( !cur->eof() ) {
        cliente.cif = cur->value( "cifcliente" ).toAscii();
        cliente.nombre = cur->value( "nomcliente" ).toAscii();
    } // end if
    delete cur;

    cur = mainCompany() ->loadQuery ( "SELECT * FROM almacen WHERE idalmacen=" + m_ticket->dbValue ( "idalmacen" ) );
    if ( !cur->eof() )
        almacen.nombre = cur->value( "nomalmacen" ).toAscii() ;
    delete cur;


    BlEscPrinter pr ( g_confpr->value( CONF_TICKET_PRINTER_FILE ) );
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

    pr.printText ( "Vale de Compra por valor de: \n" );
    pr.printText ( mui_difprice->text() );

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


    QByteArray qba = m_ticket->dbValue ( "refalbaran" ).toAscii();
    char* barcode = qba.data();
    pr.setJustification ( BlEscPrinter::center );
    pr.setBarcodeFormat ( 2, 50, both, fontB );
    pr.printBarCode ( code39, qba.size(), barcode );
    pr.cutPaperAndFeed ( TRUE, 10 );
    pr.print();

    m_ticket->save();
    m_ticket->imprimir();
    ( ( QDialog * ) parent() )->accept();
}

void Devolucion::on_mui_ref_returnPressed()
{
    if ( m_ticket )  {
        delete m_ticket;
        m_ticket = NULL;
    } // end if

    QString query1 = "SELECT * FROM albaran WHERE refalbaran = '" + mui_ref->text() + "'";

    BlDbRecordSet *curs = mainCompany()->loadQuery ( query1 );
    if ( !curs->eof() ) {
        m_ticket = new BtTicket ( mainCompany(), NULL );
        m_ticket->load ( curs->value( "idalbaran" ) );
    }
    delete curs;
    m_totalin = "";
    pintar();
}

void Devolucion::pintar()
{
    if ( !m_ticket ) return;

// ====================== PINTAMOS ========================
    QString html = "<p style=\"font-family:monospace; font-size: 8pt;\">";
    QString html1 = "<font size=\"1\">";

    html1 += "Ticket: " + m_ticket->dbValue ( "nomticket" ) + "<BR>";

    QString querytrab = "SELECT * FROM trabajador WHERE idtrabajador = " + m_ticket->dbValue ( "idtrabajador" );
    BlDbRecordSet *curtrab = mainCompany() ->loadQuery ( querytrab );
    html1 += "Trabajador: " + m_ticket->dbValue ( "idtrabajador" ) + " " + curtrab->value( "nomtrabajador" ) + "<BR>";
    delete curtrab;
    QString query = "SELECT * FROM cliente WHERE idcliente = " + m_ticket->dbValue ( "idcliente" );
    BlDbRecordSet *cur1 = mainCompany() ->loadQuery ( query );
    html1 += "Cliente: " + m_ticket->dbValue ( "idcliente" ) + " " + cur1->value( "nomcliente" ) + "<BR>";
    delete cur1;

    if ( m_ticket->listaLineas()->size() > 0 ) {

        html += "<TABLE border=\"0\">";

        html += "<TR bgcolor = \"#CCCCCC\">";
        html += "<TD>SEL</TD>";
        for ( int z = 0; z < m_ticket->listaLineas()->at ( 0 )->lista()->size(); ++z ) {
            BlDbField *head = m_ticket->listaLineas()->at ( 0 )->lista()->at ( z );
            if ( head->fieldName().left ( 2 ) != "id" && head->fieldName().left ( 3 ) != "num" )
                html += "<TD>" + head->fieldName().left ( 4 ) + "</TD>";
        } // end for
        html += "</TR>";

        BlDbRecord *item;
        for ( int i = 0; i < m_ticket->listaLineas() ->size(); ++i ) {
            item = m_ticket->listaLineas() ->at ( i );

            html += "<TR>";
            html += "<TD><A NAME=\"plus\" HREF=\"?op=plus&numlalbaran=" + item->dbValue ( "numlalbaran" ) + "\">+</A>  <A HREF=\"?op=minus&numlalbaran=" + item->dbValue ( "numlalbaran" ) + "\">-</A></td>";
            for ( int j = 0; j < item->lista()->size(); ++j ) {
                BlDbField *camp = item->lista()->at ( j );
                if ( camp->fieldName().left ( 2 ) != "id" && camp->fieldName().left ( 3 ) != "num" )
                    html += "<TD>" + camp->fieldValue() + "</TD>";
            } // end for

            html += "</TR>";
        }// end for
        html += "</TABLE>";

    } // end if
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
    for ( int i = 0; i < m_ticket->listaLineas() ->size(); ++i ) {
        linea = m_ticket->listaLineas() ->at ( i );
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

    if ( m_totalin == "" ) {
        mui_total->setText ( total.toQString() );
        m_totalin = total.toQString();
    }
    mui_newtotal->setText ( total.toQString() );
    BlFixed diff ( m_totalin );
    diff = diff - total;
    mui_difprice->setText ( diff.toQString() );


// ======================================
    /// Pintamos el HTML en el textBrowser
    mui_browser->setText ( html );
}


void Devolucion::on_mui_browser_anchorClicked ( const QUrl &anchor )
{
    if ( anchor.queryItemValue ( "op" ) == "minus" ) {

        if ( m_ticket->dbValue ( "idalbaran" ).isEmpty() ) return;
        int sizein = m_ticket->listaLineas()->size();
        for ( int i = 0; i < sizein; ++i ) {
            BlDbRecord *item = m_ticket->listaLineas() ->at ( i );
            if ( item->dbValue ( "numlalbaran" ) == anchor.queryItemValue ( "numlalbaran" ) ) {
                BlDbRecord *nitem = m_ticket->agregarLinea();
                QList<BlDbField *> *lista = item->lista();
                for ( int j = 0; j < lista->size(); ++j ) {
                    BlDbField * camp = lista->at ( j );
                    if ( camp->fieldName() != "numlalbaran" ) {
                        nitem->setDbValue ( camp->fieldName(), camp->fieldValue() );
                    } // end if
                    if ( camp->fieldName() == "cantlalbaran" && camp->fieldValue().toFloat() > 0 ) {
                        nitem->setDbValue ( camp->fieldName(), "-1" );
                    }// end if
                } // end if
            } // end for
        }// end for

    } // end if

    pintar();
}

