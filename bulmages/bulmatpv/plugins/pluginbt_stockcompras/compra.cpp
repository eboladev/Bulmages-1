#include <QtWidgets/QWidget>

#include <vector>

#include "compra.h"
#include "blfixed.h"


/// Una factura puede tener multiples bases imponibles. Por eso definimos el tipo base
/// como un QMap.
typedef QMap<QString, BlFixed> base;


Compra::Compra ( BtCompany *emp, QWidget *parent ) : BlWidget ( emp, parent )
{
    setupUi ( ( QDialog* ) this );
    /*
        this->mui_tablesFrame->setDisabled ( true );

        m_ticket = NULL;
    */
    /*
        m_value = 0;
        base basesimp;
        base basesimpreqeq;
        BlDbRecord *linea;

        Ticket *tick = emp->ticketActual();

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
            totreqeq = totreqeq + parreqeq;
        } // end for

        BlFixed totirpf = totbaseimp * irpf / 100;

        BlFixed total = totiva + totbaseimp + totreqeq - totirpf;

        mui_total->setText(total.toQString());
    */
}

void Compra::on_mui_aceptar_clicked()
{
    close();

}

void Compra::on_mui_codigoarticulo_returnPressed()
{

    QString texto = "<TABLE width=\"100%\">";
    QString tventas = "0";
    QString tcompras = "0";
    QString bgcolor = "#FFFFFF";
    texto += "<TR bgcolor=\"#BBBBBB\">";
    texto += "<TD width=\"100\">Color</TD>";
    texto += "<TD width=\"100\">Talla</TD>";
    texto += "<TD width=\"100\">Comprados</TD>";
    texto += "<TD width=\"100\">Vendidos</TD>";
    texto += "<TD width=\"100\">Stock</TD>";
    texto += "</TR>";
    QString query = "SELECT * FROM articulo LEFT JOIN tc_articulo_alias AS t3 ON articulo.idarticulo = t3.idarticulo  LEFT JOIN tc_talla AS t1 ON t3.idtc_talla = t1.idtc_talla LEFT JOIN tc_color AS t2 ON t3.idtc_color = t2.idtc_color WHERE t3.aliastc_articulo_tallacolor = '" + mui_codigoarticulo->text() + "' OR articulo.codigocompletoarticulo = '" + mui_codigoarticulo->text() + "' ORDER BY nomtc_color, nomtc_talla";
    BlDbRecordSet *cur = mainCompany()->loadQuery ( query );
    while ( ! cur->eof() ) {
        QString query1 = "SELECT SUM(cantlalbaranp) AS suma FROM lalbaranp WHERE idarticulo=" + cur->value( "idarticulo" ) + "  AND idtc_talla=" + cur->value( "idtc_talla" ) + " AND idtc_color=" + cur->value( "idtc_color" );
        QString query2 = "SELECT SUM(cantlalbaran) AS suma FROM lalbaran WHERE idarticulo=" + cur->value( "idarticulo" ) + "  AND idtc_talla=" + cur->value( "idtc_talla" ) + " AND idtc_color=" + cur->value( "idtc_color" );
        BlDbRecordSet *cur1 = mainCompany()->loadQuery ( query1 );
        BlDbRecordSet *cur2 = mainCompany()->loadQuery ( query2 );
        if ( !cur2->eof() ) {
            tventas = cur2->value( "suma" );
        } // end if
        if ( !cur1->eof() ) {
            tcompras = cur1->value( "suma" );
        } // end if

        if ( !cur1->eof() ) {
            texto += "<TR bgcolor=\"" + bgcolor + "\">";
            texto += "<TD>" + cur->value( "nomtc_color" ) + "</TD>";
            texto += "<TD>" + cur->value( "nomtc_talla" ) + "</TD>";
            texto += "<TD align=\"right\">" + tcompras + "</TD>";
            texto += "<TD align=\"right\">" + tventas + "</TD>";
            texto += "<TD align=\"right\">" + QString::number ( tcompras.toFloat() - tventas.toFloat() ) + "</TD>";
            texto += "</TR>";
        } // end if
        delete cur1;
        delete cur2;
        cur->nextRecord();
        if ( bgcolor == "#FFFFFF" ) {
            bgcolor = "#CCCCFF";
        } else {
            bgcolor =  "#FFFFFF";
        } // end if
    } // end while
    delete cur;
    mui_subform->setText ( texto );
}

Compra::~Compra()
{}



