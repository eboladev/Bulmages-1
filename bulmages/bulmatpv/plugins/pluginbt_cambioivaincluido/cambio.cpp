#include <QWidget>

#include "cambio.h"
#include "blfixed.h"
#include "blplugins.h"


/// Una factura puede tener multiples bases imponibles. Por eso definimos el tipo base
/// como un QMap.
typedef QMap<QString, BlFixed> base;


Cambio::Cambio ( BtCompany *emp, QWidget *parent ) : BlWidget ( emp, parent )
{
    setupUi ( this );

    m_value = 0;
    base basesimp;
    base basesimpreqeq;
    BlDbRecord *linea;

    BtTicket *tick = emp->ticketActual();


    /// Disparamos los plugins.
    int res = g_plugins->run ( "Cambio_ivainc_pre", this );
    if ( res != 0 ) {
        return;
    } // end if
    

    /// Impresion de los contenidos.
    QString l;
    BlFixed irpf ( "0" );

    BlDbRecordSet *cur = emp->loadQuery ( "SELECT * FROM configuracion WHERE nombre = 'IRPF'" );
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
	cant.setPrecision(emp->decimalesCantidad());
        BlFixed pvpund ( linea->dbValue ( "pvpivainclalbaran" ) );
	pvpund.setPrecision(emp->decimalesCantidad());
        BlFixed desc1 ( linea->dbValue ( "descuentolalbaran" ) );
        BlFixed cantpvp = cant * pvpund;
	cantpvp.setPrecision(emp->decimalesCantidad());
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
        totreqeq = totreqeq + parreqeq;
    } // end for

    BlFixed totirpf = totbaseimp * irpf / 100;

    BlFixed total =  totbaseimp;

    mui_total->setText ( total.toQString() );

}


Cambio::~Cambio()
{}


void Cambio::on_mui_pago_textChanged()
{
    BlFixed total ( mui_total->text().replace ( ",", "." ) );
    BlFixed pago ( mui_pago->text().replace ( ",", "." ) );
    BlFixed cambio = pago - total;
    mui_cambio->setText ( cambio.toQString() );
}


void Cambio::on_mui_cancelar_clicked()
{
    m_value = -1;
    ( ( QDialog * ) parent() )->accept();
}

void Cambio::on_mui_cobrar_clicked()
{
    ( ( BtCompany * ) mainCompany() )->ticketActual()->setDbValue ( "pagado", mui_pago->text() );
    ( ( BtCompany * ) mainCompany() )->ticketActual()->setDbValue ( "cambio", mui_cambio->text() );
    ( ( BtCompany * ) mainCompany() )->ticketActual()->setDbValue ( "total", mui_total->text() );
    ( ( QDialog * ) parent() )->accept();
}

void Cambio::on_mui_visa_clicked()
{
    ( ( BtCompany * ) mainCompany() )->ticketActual()->setDbValue ( "idforma_pago", g_confpr->value( CONF_IDFORMA_PAGO_VISA ) );
    ( ( BtCompany * ) mainCompany() )->ticketActual()->setDbValue ( "pagado", mui_pago->text() );
    ( ( BtCompany * ) mainCompany() )->ticketActual()->setDbValue ( "cambio", mui_cambio->text() );
    ( ( BtCompany * ) mainCompany() )->ticketActual()->setDbValue ( "total", mui_total->text() );
    ( ( QDialog * ) parent() )->accept();
}

