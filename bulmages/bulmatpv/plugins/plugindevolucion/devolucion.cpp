#include "devolucion.h"
#include "fixed.h"
#include <QWidget>
#include "escprinter.h"

/// Una factura puede tener multiples bases imponibles. Por eso definimos el tipo base
/// como un QMap.
typedef QMap<QString, Fixed> base;

Devolucion::Devolucion ( EmpresaTPV *emp, QWidget *parent ) : BLWidget ( emp, parent )
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
    m_ticket->guardar();
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
        Fixed iva;
        Fixed baseImponible;
        Fixed totalIva;
    }total;

    cursor2 *cur = empresaBase() ->cargacursor ( "SELECT * FROM configuracion WHERE nombre='NombreEmpresa'" );
    if ( !cur->eof() )
        empresa.nombre = cur->valor ( "valor" );
    delete cur;

    cur = empresaBase() ->cargacursor ( "SELECT * FROM configuracion WHERE nombre='DireccionCompleta'" );
    if ( !cur->eof() )
        empresa.direccionCompleta = cur->valor ( "valor" );
    delete cur;

    cur = empresaBase() ->cargacursor ( "SELECT * FROM configuracion WHERE nombre='CodPostal'" );
    if ( !cur->eof() )
        empresa.codigoPostal = cur->valor ( "valor" ).toAscii();
    delete cur;

    cur = empresaBase() ->cargacursor ( "SELECT * FROM configuracion WHERE nombre='Ciudad'" );
    if ( !cur->eof() )
        empresa.ciudad = cur->valor ( "valor" );
    delete cur;

    cur = empresaBase() ->cargacursor ( "SELECT * FROM configuracion WHERE nombre='Telefono'" );
    if ( !cur->eof() )
        empresa.telefono = cur->valor ( "valor" );
    delete cur;

    cur = empresaBase() ->cargacursor ( "SELECT * FROM configuracion WHERE nombre='Provincia'" );
    if ( !cur->eof() )
        empresa.provincia = cur->valor ( "valor" );
    delete cur;

    fecha.dia = QDate::currentDate().toString ( "d-M-yyyy" );
    fecha.hora = QTime::currentTime().toString ( "HH:mm" );

    trabajador.id = m_ticket->DBvalue ( "idtrabajador" );
    cur = empresaBase() ->cargacursor ( "SELECT * FROM trabajador WHERE idtrabajador=" + m_ticket->DBvalue ( "idtrabajador" ) );
    if ( !cur->eof() )
        trabajador.nombre = cur->valor ( "nomtrabajador" );
    delete cur;

    cur = empresaBase() ->cargacursor ( "SELECT * FROM cliente WHERE idcliente=" + m_ticket->DBvalue ( "idcliente" ) );
    if ( !cur->eof() ) {
        cliente.cif = cur->valor ( "cifcliente" ).toAscii();
        cliente.nombre = cur->valor ( "nomcliente" ).toAscii();
    } // end if
    delete cur;

    cur = empresaBase() ->cargacursor ( "SELECT * FROM almacen WHERE idalmacen=" + m_ticket->DBvalue ( "idalmacen" ) );
    if ( !cur->eof() )
        almacen.nombre = cur->valor ( "nomalmacen" ).toAscii() ;
    delete cur;


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
    pr.printText ( "Cliente, " + cliente.cif + " " + cliente.nombre + "\n" );
    pr.printText ( "\n" );

    pr.printText ( "Vale de Compra por valor de: \n" );
    pr.printText ( mui_difprice->text() );

    pr.printText ( "\n\n" );
    pr.setJustification ( left );
    pr.setCharacterPrintMode ( CHARACTER_FONTA_SELECTED );
    pr.printText ( "Le ha atendido " + trabajador.nombre + "\n" );
    pr.printText ( "\n" );

    pr.printText ( "Tel. " + empresa.telefono + "\n" );
    pr.printText ( "\n" );

    pr.setJustification ( center );
    pr.setColor ( red );
    pr.printText ( "*** GRACIAS POR SU VISITA ***\n" );


    QByteArray qba = m_ticket->DBvalue ( "refalbaran" ).toAscii();
    char* barcode = qba.data();
    pr.setJustification ( center );
    pr.setBarcodeFormat ( 2, 50, both, fontB );
    pr.printBarCode ( code39, qba.size(), barcode );
    pr.cutPaperAndFeed ( TRUE, 10 );
    pr.print();

    m_ticket->guardar();
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

    cursor2 *curs = empresaBase()->cargacursor ( query1 );
    if ( !curs->eof() ) {
        m_ticket = new Ticket ( empresaBase(), NULL );
        m_ticket->cargar ( curs->valor ( "idalbaran" ) );
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

    html1 += "Ticket: " + m_ticket->DBvalue ( "nomticket" ) + "<BR>";

    QString querytrab = "SELECT * FROM trabajador WHERE idtrabajador = " + m_ticket->DBvalue ( "idtrabajador" );
    cursor2 *curtrab = empresaBase() ->cargacursor ( querytrab );
    html1 += "Trabajador: " + m_ticket->DBvalue ( "idtrabajador" ) + " " + curtrab->valor ( "nomtrabajador" ) + "<BR>";
    delete curtrab;
    QString query = "SELECT * FROM cliente WHERE idcliente = " + m_ticket->DBvalue ( "idcliente" );
    cursor2 *cur1 = empresaBase() ->cargacursor ( query );
    html1 += "Cliente: " + m_ticket->DBvalue ( "idcliente" ) + " " + cur1->valor ( "nomcliente" ) + "<BR>";
    delete cur1;

    if (m_ticket->listaLineas()->size() > 0) {

	html += "<TABLE border=\"0\">";
	
	html += "<TR bgcolor = \"#CCCCCC\">";
	html += "<TD>SEL</TD>";
	for ( int z = 0; z < m_ticket->listaLineas()->at ( 0 )->lista()->size(); ++z ) {
		DBCampo *head = m_ticket->listaLineas()->at ( 0 )->lista()->at ( z );
		if ( head->nomcampo().left ( 2 ) != "id" && head->nomcampo().left ( 3 ) != "num" )
			html += "<TD>" + head->nomcampo().left ( 4 ) + "</TD>";
	} // end for
	html += "</TR>";
	
	DBRecord *item;
	for ( int i = 0; i < m_ticket->listaLineas() ->size(); ++i ) {
		item = m_ticket->listaLineas() ->at ( i );
	
		html += "<TR>";
		html += "<TD><A NAME=\"plus\" HREF=\"?op=plus&numlalbaran=" + item->DBvalue ( "numlalbaran" ) + "\">+</A>  <A HREF=\"?op=minus&numlalbaran=" + item->DBvalue ( "numlalbaran" ) + "\">-</A></td>";
		for ( int j = 0; j < item->lista()->size(); ++j ) {
		DBCampo *camp = item->lista()->at ( j );
		if ( camp->nomcampo().left ( 2 ) != "id" && camp->nomcampo().left ( 3 ) != "num" )
			html += "<TD>" + camp->valorcampo() + "</TD>";
		} // end for
	
		html += "</TR>";
	}// end for
	html += "</TABLE>";

    } // end if
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

    if ( m_totalin == "" ) {
        mui_total->setText ( total.toQString() );
        m_totalin = total.toQString();
    }
    mui_newtotal->setText ( total.toQString() );
    Fixed diff ( m_totalin );
    diff = diff - total;
    mui_difprice->setText ( diff.toQString() );


// ======================================
    /// Pintamos el HTML en el textBrowser
    mui_browser->setText ( html );
}


void Devolucion::on_mui_browser_anchorClicked ( const QUrl &anchor )
{
    if ( anchor.queryItemValue ( "op" ) == "minus" ) {

        if ( m_ticket->DBvalue ( "idalbaran" ).isEmpty() ) return;
        int sizein = m_ticket->listaLineas()->size();
        for ( int i = 0; i < sizein; ++i ) {
            DBRecord *item = m_ticket->listaLineas() ->at ( i );
            if ( item->DBvalue ( "numlalbaran" ) == anchor.queryItemValue ( "numlalbaran" ) ) {
                DBRecord *nitem = m_ticket->agregarLinea();
                QList<DBCampo *> *lista = item->lista();
                for ( int j = 0; j < lista->size(); ++j ) {
                    DBCampo * camp = lista->at ( j );
                    if ( camp->nomcampo() != "numlalbaran" ) {
                        nitem->setDBvalue ( camp->nomcampo(), camp->valorcampo() );
                    } // end if
                    if ( camp->nomcampo() == "cantlalbaran" && camp->valorcampo().toFloat() > 0 ) {
                        nitem->setDBvalue ( camp->nomcampo(), "-1" );
                    }// end if
                } // end if
            } // end for
        }// end for

    } // end if

    pintar();
}

