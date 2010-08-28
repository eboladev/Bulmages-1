/***************************************************************************
 *   Copyright (C) 2007 by Tomeu Borras Riera                              *
 *   tborras@conetxia.com                                                  *
 *   http://www.iglues.org                                                 *
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

#include <QWidget>
#include <QCloseEvent>
#include <QFile>
#include <QDomDocument>
#include <QDomNode>
#include <QTextStream>
#include <QHBoxLayout>
#include <QObject>
#include <QPushButton>


#include <QCheckBox>
#include <QInputDialog>
#include <QUiLoader>




#include "blmaincompany.h"
#include "btticket.h"
#include "blplugins.h"
#include "btescprinter.h"

/// Una factura puede tener multiples bases imponibles. Por eso definimos el tipo base
/// como un QMap.
typedef QMap<QString, BlFixed> base;

BtTicket::BtTicket ( BlMainCompany *emp, QWidget *parent ) : BlWidget ( emp, parent ), BlDbRecord ( emp )
{
    blDebug ( "BtTicket::BtTicket", 0 );
    /// Inicializamos los parametros del ticket para la base de datos.
    setDbTableName ( "albaran" );
    setDbFieldId ( "idalbaran" );
    addDbField ( "descalbaran", BlDbField::DbVarChar, BlDbField::DbNothing, _( "Descripcion" ) );
    addDbField ( "nomticket", BlDbField::DbVarChar, BlDbField::DbNoSave, _( "Nombre" ) );
    addDbField ( "idalbaran", BlDbField::DbInt, BlDbField::DbPrimaryKey, _( "Id albaran" ) );
    addDbField ( "fechaalbaran", BlDbField::DbDate, BlDbField::DbNothing, _( "Id fechaalbaran" ) );
    addDbField ( "horaalbaran", BlDbField::DbVarChar, BlDbField::DbNoSave, _( "Hora" ) );
    addDbField ( "idtrabajador", BlDbField::DbInt, BlDbField::DbNotNull, _( "Trabajador" ) );
    addDbField ( "idcliente", BlDbField::DbInt, BlDbField::DbNotNull, _( "Cliente" ) );
    addDbField ( "idalmacen", BlDbField::DbInt, BlDbField::DbNotNull, _( "Almacen" ) );
    addDbField ( "numalbaran", BlDbField::DbInt, BlDbField::DbNothing, _( "Num" ) );
    addDbField ( "refalbaran", BlDbField::DbVarChar, BlDbField::DbNothing, _( "Referencia" ) );
    addDbField ( "ticketalbaran", BlDbField::DbBoolean, BlDbField::DbNothing, _( "Ticket" ) );
    addDbField ( "idforma_pago", BlDbField::DbInt, BlDbField::DbNothing, _( "Id forma de pago" ) );
    addDbField ( "bloqueadoticket", BlDbField::DbBoolean, BlDbField::DbNoSave, _( "Bloqueado" ) );

    setDbValue ( "ticketalbaran", "TRUE" );
    setDbValue ( "idalmacen", g_confpr->valor ( CONF_IDALMACEN_DEFECTO ) );
    setDbValue ( "idcliente", g_confpr->valor ( CONF_IDCLIENTE_DEFECTO ) );
    setDbValue ( "idtrabajador", g_confpr->valor ( CONF_IDTRABAJADOR_DEFECTO ) );
    setDbValue ( "descalbaran", "Ticket de venta" );
    setDbValue ( "idforma_pago", g_confpr->valor ( CONF_IDFORMA_PAGO_CONTADO ) );

    m_lineaActual = NULL;
    m_listaLineas = new QList<BlDbRecord *>;
    
    m_nomTicketDefecto = _("Ticket actual");

    g_plugins->lanza ( "BtTicket_BtTicket_Post", this );

    start();
    
    blDebug ( "END BtTicket::BtTicket", 0 );
}

BtTicket::~BtTicket()
{
    blDebug ( "BtTicket::~BtTicket", 0 );
    blDebug ( "END BtTicket::~BtTicket", 0 );
}


QString BtTicket::nomTicketDefecto()
{
    blDebug ( "BtTicket::nomTicketDefecto", 0 );
    return m_nomTicketDefecto;
    blDebug ( "END BtTicket::nomTicketDefecto", 0 );
}


BlDbRecord * BtTicket::agregarLinea()
{
    blDebug ( "BtTicket::agregarLinea", 0 );

    /// Creamos un nuevo BlDbRecord y lo inicializamos.
    BlDbRecord * item = new BlDbRecord ( mainCompany() );
    item->setDbTableName ( "lalbaran" );
    item->setDbFieldId ( "numlalbaran" );
    item->addDbField ( "idalbaran", BlDbField::DbInt, BlDbField::DbNotNull, _( "Id albaran" ) );
    item->addDbField ( "numlalbaran", BlDbField::DbInt, BlDbField::DbPrimaryKey, _( "Id linea albaran" ) );
    item->addDbField ( "cantlalbaran", BlDbField::DbNumeric, BlDbField::DbNotNull, _( "Cantidad" ) );
    item->addDbField ( "pvplalbaran", BlDbField::DbNumeric, BlDbField::DbNotNull, _( "Precio" ) );
    item->addDbField ( "ivalalbaran", BlDbField::DbNumeric, BlDbField::DbNotNull, _( "IVA" ) );
    item->addDbField ( "descuentolalbaran", BlDbField::DbNumeric, BlDbField::DbNothing, _( "Descuento" ) );
    item->addDbField ( "ordenlalbaran", BlDbField::DbNumeric, BlDbField::DbNotNull, _( "Orden" ) );
    item->addDbField ( "reqeqlalbaran", BlDbField::DbNumeric, BlDbField::DbNothing, _( "Req. eq." ) );
//    item->addDbField ( "lotelalbaran", BlDbField::DbNumeric, BlDbField::DbNotNull, _( "Lote" ) );
    item->addDbField ( "idarticulo", BlDbField::DbInt, BlDbField::DbNotNull, _( "Id articulo" ) );
    item->addDbField ( "codigocompletoarticulo", BlDbField::DbVarChar, BlDbField::DbNoSave, _( "Codigo articulo" ) );
    item->addDbField ( "nomarticulo", BlDbField::DbVarChar, BlDbField::DbNoSave, _( "Nombre articulo" ) );
    item->addDbField ( "desclalbaran", BlDbField::DbVarChar, BlDbField::DbNothing, _( "Nombre articulo" ) );

    item->setDbValue ( "descuentolalbaran", "0" );

    blDebug ( "Hacemos el append", 0 );

    /// Agregamos el BlDbRecord a la lista de lineas de ticket.
    m_listaLineas->append ( item );

    g_plugParams = (void *) item;
    g_plugins->lanza("BtTicket_agregarLinea_Post", this);

    blDebug ( "END BtTicket::agregarLinea", 0 );
    return item;
}

void BtTicket::pintar()
{
    blDebug ( "BtTicket::pintar", 0, "Metodo para reimplementar en clases derivadas" );
    /// Disparamos los plugins.
    int res = g_plugins->lanza ( "BtTicket_pintar", this );
    if ( res != 0 ) {
        return;
    } // end if
    blDebug ( "END BtTicket::pintar", 0 );
}

QList<BlDbRecord *> *BtTicket::listaLineas()
{
    return m_listaLineas;
}


BlDbRecord *BtTicket::insertarArticulo ( QString idArticulo, BlFixed cantidad, bool nuevaLinea )
{
    blDebug ( "BtTicket::insertarArticulo", 0 );
    /// Buscamos si ya hay una linea con el articulo que buscamos
    m_lineaActual = NULL;
    BlDbRecord *item;

    for ( int i = 0; i < listaLineas() ->size(); ++i ) {
        item = listaLineas() ->at ( i );
        if ( item->dbValue ( "idarticulo" ) == idArticulo )
            m_lineaActual = item;
    }// end for

    if ( m_lineaActual && nuevaLinea == FALSE ) {
        /// Ya hay una linea con este articulo (es un agregado)
        BlFixed cantidadib ( m_lineaActual->dbValue ( "cantlalbaran" ) );
        BlFixed cant1 = cantidadib + cantidad;
        m_lineaActual->setDbValue ( "cantlalbaran", cant1.toQString('.') );
    } else {
        /// No hay ningun item con este articulo (es una insercion)
        m_lineaActual = agregarLinea();
        m_lineaActual->setDbValue ( "idarticulo", idArticulo );
	     QString a = cantidad.toQString();
        m_lineaActual->setDbValue ( "cantlalbaran", cantidad.toQString('.') );

        /// Buscamos los parametros en la base de datos.
        QString query = "SELECT pvparticulo, codigocompletoarticulo, nomarticulo, porcentasa_iva FROM articulo LEFT JOIN (SELECT idtipo_iva, porcentasa_iva, fechatasa_iva FROM tasa_iva ) AS t1 ON articulo.idtipo_iva = t1.idtipo_iva WHERE idarticulo = " + idArticulo + " ORDER BY t1.fechatasa_iva LIMIT 1";
        BlDbRecordSet *cur = mainCompany() ->loadQuery ( query );

        if ( !cur->eof() ) {
            m_lineaActual->setDbValue ( "pvplalbaran", cur->valor ( "pvparticulo" ) );
            m_lineaActual->setDbValue ( "codigocompletoarticulo", cur->valor ( "codigocompletoarticulo" ) );
            m_lineaActual->setDbValue ( "nomarticulo", cur->valor ( "nomarticulo" ) );
            m_lineaActual->setDbValue ( "desclalbaran", cur->valor ( "nomarticulo" ) );
            m_lineaActual->setDbValue ( "ivalalbaran", cur->valor ( "porcentasa_iva") );

        } // end if
        delete cur;
    } // end if

    /// Disparamos los plugins.
    g_plugins->lanza ( "BtTicket_insertarArticulo_Post", this );

    /// Pintamos el ticket ya que se ha modificado.
    pintar();

    blDebug ( "END BtTicket::insertarArticulo", 0 );
    return m_lineaActual;
}


void  BtTicket::borrarArticulo ( BlDbRecord *linea, BlFixed cantidad )
{
    /// Comprueba que haya un articulo seleccionado.
    if ( m_lineaActual == NULL ) {
        return;
    } // end if

    m_lineaActual->borrar();
}

void  BtTicket::vaciarBtTicket()
{}

void  BtTicket::subirPosArticulo ( BlDbRecord *linea, int filas )
{}

void  BtTicket::bajarPosArticulo ( BlDbRecord *linea, int filas )
{}

void  BtTicket::inicioPosBtTicket ( BlDbRecord * )
{}

void  BtTicket::finPosBtTicket ( BlDbRecord * )
{}

BlDbRecord * BtTicket::lineaBtTicket ( int posicion )
{
    return NULL;
}

BlDbRecord *BtTicket::lineaActBtTicket()
{
    return m_lineaActual;
}

void BtTicket::setLineaActual ( BlDbRecord *rec )
{
    m_lineaActual = rec;
}

void  BtTicket::setDescuentoGlobal ( BlFixed descuento )
{}

void BtTicket::abrircajon()
{
    QString filestr = g_confpr->valor(CONF_DIR_USER) + "bulmatpv_abrircajon.txt";
    
    QFile file ( filestr );
    
    if ( !file.open ( QIODevice::WriteOnly | QIODevice::Unbuffered ) ) {
        blDebug ( "Error en la Impresion de ticket", 2 );
    } // end if

    QStringList secuencia = g_confpr->valor (CONF_CASHBOX_OPEN_CODE).split(",");

    /// El comando de apertura de cajon
    for (int i = 0; i < secuencia.size(); ++i) {
	    QString cad = QChar(secuencia.at(i).toInt());
	    file.write ( cad.toAscii(), 1 );
    } // end for

    file.close();


    if (!g_confpr->valor ( CONF_CASHBOX_FILE).isEmpty() && g_confpr->valor ( CONF_CASHBOX_FILE) != "/dev/null") {
        QString comando = "cat " + g_confpr->valor(CONF_DIR_USER) + "bulmatpv_abrircajon.txt" + "  > " + g_confpr->valor ( CONF_CASHBOX_FILE );
        system ( comando.toAscii().data() );
    } else if (g_confpr->valor(CONF_CUPS_DEFAULT_PRINTER).isEmpty() || g_confpr->valor(CONF_CUPS_DEFAULT_PRINTER) == "None") {
        blDebug("Debe establecer el parametro CONF_CUPS_DEFAULT_PRINTER o CONF_CASHBOX_FILE para abrir el cajon " , 2);
    } else {
        QString comando = "lp -d" + g_confpr->valor(CONF_CUPS_DEFAULT_PRINTER) + " " + g_confpr->valor(CONF_DIR_USER) + "bulmatpv_abrircajon.txt";
        system ( comando.toAscii().data() );
    } // end if


}


/*
void  BtTicket::imprimir()
{
    base basesimp;
    base basesimpreqeq;

    QFile file ( g_confpr->valor ( CONF_TICKET_PRINTER_FILE ) );
    if ( !file.open ( QIODevice::WriteOnly | QIODevice::Unbuffered ) ) {
        blDebug ( "Error en la Impresion de ticket", 2 );
    } // end if
    BlDbRecordSet *cur = mainCompany() ->loadQuery ( "SELECT * FROM configuracion WHERE nombre='NombreEmpresa'" );
    if ( !cur->eof() ) {
        file.write ( cur->valor ( "valor" ).toAscii() );
        file.write ( "\n", 1 );
    } // end if
    delete cur;
    cur = mainCompany() ->loadQuery ( "SELECT * FROM configuracion WHERE nombre='DireccionCompleta'" );
    if ( !cur->eof() ) {
        file.write ( cur->valor ( "valor" ).toAscii() );
        file.write ( "\n", 1 );
    } // end if
    ///file.write ( QString ( "C/LAS POZAS 181, LOCAL 43\n" ).toAscii() );
    delete cur;
    /// file.write ( QString ( "ALIMENTACION ECOLOGICA. HERBOLARIO\n" ).toAscii() );
    cur = mainCompany() ->loadQuery ( "SELECT * FROM configuracion WHERE nombre='CodPostal'" );
    if ( !cur->eof() ) {
        file.write ( cur->valor ( "valor" ).toAscii() );
    } // end if
    delete cur;

    file.write ( QString ( " " ).toAscii() );
    cur = mainCompany() ->loadQuery ( "SELECT * FROM configuracion WHERE nombre='Ciudad'" );
    if ( !cur->eof() ) {
        file.write ( cur->valor ( "valor" ).toAscii() );
        file.write ( QString ( " " ).toAscii() );
    } // end if
    delete cur;


    cur = mainCompany() ->loadQuery ( "SELECT * FROM configuracion WHERE nombre='Provincia'" );
    if ( !cur->eof() ) {
        file.write ( QString ( "(" ).toAscii() );
        file.write ( cur->valor ( "valor" ).toAscii() );
        file.write ( QString ( ")" ).toAscii() );
        file.write ( "\n", 1 );
    } // end if
    delete cur;

    /// Imprimimos el numero de BtTicket
    file.write ( QString ( "BtTicket: " ).toAscii() );
    file.write ( dbValue ( "numalbaran" ).toAscii() );
    file.write ( "\n", 1 );


    /// Imprimimos la fecha
    file.write ( QString ( "Fecha: " ).toAscii() );
    QDate fecha = QDate::currentDate();
    QString sfecha = fecha.toString ( "d-M-yyyy" );
    file.write ( sfecha.toAscii() );
    QTime hora = QTime::currentTime();
    QString stime = " " + hora.toString ( "HH:mm" );
    file.write ( stime.toAscii() );
    file.write ( "\n", 1 );


    /// Imprimimos el trabajador
    file.write ( QString ( "Trabajador: " ).toAscii() );
    cur = mainCompany() ->loadQuery ( "SELECT * FROM trabajador WHERE idtrabajador=" + dbValue ( "idtrabajador" ) );
    if ( !cur->eof() ) {
        file.write ( dbValue ( "idtrabajador" ).toAscii() );
        file.write ( " " );
        file.write ( cur->valor ( "nomtrabajador" ).toAscii() );
        file.write ( "\n", 1 );
    } // end if
    delete cur;

    /// Imprimimos el cliente
    cur = mainCompany() ->loadQuery ( "SELECT * FROM cliente WHERE idcliente=" + dbValue ( "idcliente" ) );
    if ( !cur->eof() ) {
        file.write ( QString ( "Cliente: " ).toAscii() );
        file.write ( cur->valor ( "cifcliente" ).toAscii() );
        file.write ( " ", 1 );
        file.write ( cur->valor ( "nomcliente" ).toAscii() );
        file.write ( "\n", 1 );
    } // end if
    delete cur;

    /// Imprimimos el almacen
    cur = mainCompany() ->loadQuery ( "SELECT * FROM almacen WHERE idalmacen=" + dbValue ( "idalmacen" ) );
    if ( !cur->eof() ) {
        file.write ( QString ( "Almacen: " ).toAscii() );
        file.write ( cur->valor ( "nomalmacen" ).toAscii() );
        file.write ( "\n", 1 );
    } // end if
    delete cur;


    file.write ( "\n", 1 );
    file.write ( "\n", 1 );

// ============================================
    /// Impresion de los contenidos.
    QString l;
    BlFixed irpf ( "0" );

    cur = mainCompany() ->loadQuery ( "SELECT * FROM configuracion WHERE nombre = 'IRPF'" );
    if ( cur ) {
        if ( !cur->eof() ) {
            irpf = BlFixed ( cur->valor ( "valor" ) );
        } // end if
        delete cur;
    } // end if

    BlDbRecord *linea;
    BlFixed descuentolinea ( "0.00" );
    for ( int i = 0; i < listaLineas() ->size(); ++i ) {
        linea = listaLineas() ->at ( i );
        BlFixed cant ( linea->dbValue ( "cantlalbaran" ) );
        BlFixed pvpund ( linea->dbValue ( "pvplalbaran" ) );
        BlFixed desc1 ( linea->dbValue ( "descuentolalbaran" ) );
        BlFixed cantpvp = cant * pvpund;
        BlFixed base = cantpvp - cantpvp * desc1 / 100;
        BlFixed iva = ( linea->dbValue ( "ivalalbaran" ) );
        BlFixed percentiva = ( iva / 100 );
        descuentolinea = descuentolinea + ( cantpvp * desc1 / 100 );
        BlFixed subtotreq = ( base +  linea->dbValue ( "reqeqlalbaran" ) );
        BlFixed subtotaliva = ( subtotreq * percentiva ) + subtotreq;
        BlFixed totalunidad = pvpund + ( ( pvpund - ( pvpund * desc1 / 100 ) ) * percentiva );
        basesimp[linea->dbValue ( "ivalalbaran" ) ] = basesimp[linea->dbValue ( "ivalalbaran" ) ] + base;
        basesimpreqeq[linea->dbValue ( "reqeqlalbaran" ) ] = basesimpreqeq[linea->dbValue ( "reqeqlalbaran" ) ] + base;
        /// Hacemos la impresion
        QString str =  linea->dbValue ( "desclalbaran" ).leftJustified ( 22, ' ', TRUE ) + linea->dbValue ( "cantlalbaran" ).rightJustified ( 1, ' ' ) + QString ( " " ) + totalunidad.toQString().rightJustified ( 2, ' ' )  + subtotaliva.toQString().rightJustified ( 8, ' ' );
        file.write ( str.toAscii() );
        file.write ( "\n", 1 );
    } // end for


    file.write ( "\n", 1 );
    file.write ( "\n", 1 );

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
        QString str = "Base Imp" + it.key() + "% " + parbaseimp.toQString().rightJustified ( 10, ' ' );
//        file.write(str.rightJustified(42,' ').toAscii());
//        file.write ( "\n", 1);
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
        QString str = "IVA" + it.key() + "% " + pariva.toQString().rightJustified ( 10, ' ' );
//        file.write(str.rightJustified(42,' ').toAscii());
//        file.write ( "\n", 1);

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
        QString str = "R. Eq" + it.key() + "% " + parreqeq.toQString().rightJustified ( 10, ' ' );
//        file.write(str.rightJustified(42,' ').toAscii());
//        file.write ( "\n", 1);
        totreqeq = totreqeq + parreqeq;
    } // end for



    BlFixed totirpf = totbaseimp * irpf / 100;

//    file.write (QString("=======================\n").rightJustified(43,' ').toAscii());


    QString str = "B.IMP " + totbaseimp.toQString().rightJustified ( 10, ' ' );
//    file.write(str.rightJustified(42,' ').toAscii());
//    file.write ( "\n", 1);

    str = "IVA " + totiva.toQString().rightJustified ( 10, ' ' );
//    file.write(str.rightJustified(42,' ').toAscii());
//    file.write ( "\n", 1);

    str = "IRPF " + totirpf.toQString().rightJustified ( 10, ' ' );
//    file.write(str.rightJustified(42,' ').toAscii());
//    file.write ( "\n", 1);


    str = "( IVA INCLUIDO )";
    file.write ( str.rightJustified ( 42, ' ' ).toAscii() );
    file.write ( "\n", 1 );



    /// Imprimimos el total
    file.write ( QString ( "____________________\n" ).rightJustified ( 43, ' ' ).toAscii() );

    BlFixed total = totiva + totbaseimp + totreqeq - totirpf;
    str = "TOTAL " + total.toQString().rightJustified ( 10, ' ' );
    file.write ( str.rightJustified ( 42, ' ' ).toAscii() );
    file.write ( "\n", 1 );

    /// Lanzamiento de un plugin extra�o hacia el total
    QString stotal = total.toQString();
    g_plugins->lanza("BtTicket_total", &stotal);

    str = "* IVA INCLUIDO *";
    file.write ( str.rightJustified ( 20, ' ' ).toAscii() );
    file.write ( "\n", 1 );

    /// Imprimimos el trabajador

    file.write ( QString ( "LE ATENDIO: " ).toAscii() );
    cur = mainCompany() ->loadQuery ( "SELECT * FROM trabajador WHERE idtrabajador=" + dbValue ( "idtrabajador" ) );
    if ( !cur->eof() ) {
        file.write ( dbValue ( "idtrabajador" ).toAscii() );
        file.write ( " " );
        file.write ( cur->valor ( "nomtrabajador" ).toAscii() );
        file.write ( "\n", 1 );
    } // end if
    delete cur;

// ============================================


    /// Imprimimos espacios
    file.write ( "\n \n \n \n", 7 );

    /// Preparamos para un codigo de barras
    /// Especificamos la altura del codigo de barras
    file.write ( "\x1Dh\x40", 3 );
    /// Especificamos que los caracteres vayan debajo del codigo de barras
    file.write ( "\x1DH\x02", 3 );
    /// Establecemos el tipo de codificacion para el codigo de barras
    file.write ( "\x1D", 1 );
    file.write ( "f\x01", 2 );
    ;
    /// Ponemos el ancho de la fuente a uno
    file.write ( "\x1D\x77\x01", 3 );
    /// Imprimimos la palabra top con el juego de caracteres 04
    file.write ( "\x1Dk\x04", 3 );
    file.write ( QString ( "ALB" ).toAscii() );
    file.write ( " ", 1 );
    file.write ( dbValue ( "idalbaran" ).toAscii() );
    file.write ( " ", 1 );
    file.write ( dbValue ( "numalbaran" ).toAscii() );
    file.write ( " ", 1 );
    file.write ( dbValue ( "refalbaran" ).toAscii() );
    file.write ( "\x00", 1 );
    file.write ( "\n", 1 );
    file.write ( QString ( "TELF. " ).toAscii() );
    cur = mainCompany() ->loadQuery ( "SELECT * FROM configuracion WHERE nombre='telefono'" );
    if ( !cur->eof() ) {
        file.write ( cur->valor ( "valor" ).toAscii() );
        file.write ( QString ( " " ).toAscii() );
    } // end if
    delete cur;

    file.write ( "\n", 1 );

    cur = mainCompany() ->loadQuery ( "SELECT * FROM configuracion WHERE nombre='web'" );
    if ( !cur->eof() ) {
        file.write ( cur->valor ( "valor" ).toAscii() );
        file.write ( QString ( " " ).toAscii() );
    } // end if
    delete cur;

    file.write ( "\n", 1 );


    /// Imprimimos espacios
    file.write ( "\n \n \n \n \n", 9 );


    /// El corte de papel.
    file.write ( "\x1D\x56\x01", 3 );
    file.close();

    /// La apertura del cajon.
    abrircajon();
}
*/

// =========================

/*
void BtTicket::imprimir(bool save)
{

    blDebug("BtTicket::imprimir",0);

    if (save) {
        
        if ( guardar() == -1) {
            blDebug ( "Error en la llamada a guardar()", 0 );
            return;
        } // end if
        
    } // end if


    
    /// Disparamos los plugins.
    int res = g_plugins->lanza ( "BtTicket_imprimir", this );
    if ( res != 0 ) {
        g_plugins->lanza("BtTicket_imprimir_Post", this);
        blDebug("END BtTicket::imprimir",0);
        return;
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

    BlDbRecordSet *cur = mainCompany()->loadQuery ( "SELECT * FROM configuracion WHERE nombre='NombreEmpresa'" );
    if ( !cur->eof() )
        empresa.nombre = cur->valor ( "valor" );
    delete cur;
    
    cur = mainCompany()->loadQuery ( "SELECT * FROM configuracion WHERE nombre='CIF'" );
    if ( !cur->eof() )
        empresa.nombre += "\n" + cur->valor ( "valor" );
    delete cur;

    cur = mainCompany()->loadQuery ( "SELECT * FROM configuracion WHERE nombre='DireccionCompleta'" );
    if ( !cur->eof() )
        empresa.direccionCompleta = cur->valor ( "valor" );
    delete cur;

    cur = mainCompany()->loadQuery ( "SELECT * FROM configuracion WHERE nombre='CodPostal'" );
    if ( !cur->eof() )
        empresa.codigoPostal = cur->valor ( "valor" ).toAscii();
    delete cur;

    cur = mainCompany()->loadQuery ( "SELECT * FROM configuracion WHERE nombre='Ciudad'" );
    if ( !cur->eof() )
        empresa.ciudad = cur->valor ( "valor" );
    delete cur;

    cur = mainCompany()->loadQuery ( "SELECT * FROM configuracion WHERE nombre='Telefono'" );
    if ( !cur->eof() )
        empresa.telefono = cur->valor ( "valor" );
    delete cur;

    cur = mainCompany()->loadQuery ( "SELECT * FROM configuracion WHERE nombre='Provincia'" );
    if ( !cur->eof() )
        empresa.provincia = cur->valor ( "valor" );
    delete cur;

    fecha.dia = QDate::currentDate().toString ( "d-M-yyyy" );
    fecha.hora = QTime::currentTime().toString ( "HH:mm" );

    trabajador.id = dbValue ( "idtrabajador" );
    cur = mainCompany()->loadQuery ( "SELECT * FROM trabajador WHERE idtrabajador=" + dbValue ( "idtrabajador" ) );
    if ( !cur->eof() )
        trabajador.nombre = cur->valor ( "nomtrabajador" );
    delete cur;

    cur = mainCompany()->loadQuery ( "SELECT * FROM cliente WHERE idcliente=" + dbValue ( "idcliente" ) );
    if ( !cur->eof() ) {
        cliente.cif = cur->valor ( "cifcliente" ).toAscii();
        cliente.nombre = cur->valor ( "nomcliente" ).toAscii();
    } // end if
    delete cur;

    cur = mainCompany()->loadQuery ( "SELECT * FROM almacen WHERE idalmacen=" + dbValue ( "idalmacen" ) );
    if ( !cur->eof() )
        almacen.nombre = cur->valor ( "nomalmacen" ).toAscii() ;
    delete cur;

    BlDbRecord *linea;
    
    if ( listaLineas()->size() ) {
        total.iva = BlFixed ( listaLineas()->at ( 0 )->dbValue ( "ivalalbaran" ) );
    }
        
    for ( int i = 0; i < listaLineas()->size(); ++i ) {
        linea = listaLineas()->at ( i );
        BlFixed cantidad = BlFixed ( linea->dbValue ( "cantlalbaran" ) );
        total.baseImponible = total.baseImponible + cantidad * BlFixed ( linea->dbValue ( "pvplalbaran" ) );
    } // end for
    
    total.totalIva = total.baseImponible + total.baseImponible * total.iva / BlFixed ( "100" );

    BtEscPrinter pr ( g_confpr->valor ( CONF_TICKET_PRINTER_FILE ) );
    pr.initializePrinter();
    pr.setCharacterCodeTable ( page19 );
    pr.setJustification ( BtEscPrinter::center );

    if ( g_confpr->valor ( CONF_TPV_PRINTER_LOGO ) != "" ) {
        pr.printImage ( g_confpr->valor ( CONF_TPV_PRINTER_LOGO ) );
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

    for ( int i = 0; i < listaLineas() ->size(); ++i ) {
        if ( i == listaLineas()->size() - 1 )
            pr.setUnderlineMode ( 1 );
            
        linea = listaLineas() ->at ( i );
        BlFixed iva = BlFixed ( linea->dbValue ( "ivalalbaran" ) );
        BlFixed pvp = BlFixed ( linea->dbValue ( "pvplalbaran" ) );
        pvp = pvp + pvp * iva / BlFixed ( "100" );
        BlFixed pvptotal = BlFixed ( linea->dbValue ( "cantlalbaran" ) ) * pvp;
        pr.printText ( linea->dbValue ( "cantlalbaran" ).rightJustified ( 5, ' ', TRUE ) + "  " );
        pr.printText ( linea->dbValue ( "desclalbaran" ).leftJustified ( 27, ' ', true ) + " " );
        QString pvpstr = pvp.toQString();
        QString pvptotalstr = pvptotal.toQString();
        pr.printText ( QString ( pvpstr + "�" ).rightJustified ( 10, ' ', TRUE ) + " " );
        pr.printText ( QString ( pvptotalstr + "�" ).rightJustified ( 10, ' ', TRUE ) );
        pr.printText ( "\n" );
    }
    
    pr.setUnderlineMode ( 0 );
    pr.setJustification ( BtEscPrinter::right );
    pr.setCharacterPrintMode ( CHARACTER_FONTA_SELECTED );
    pr.printText ( "Base Imponible: " + total.baseImponible.toQString() + "�\n" );
    pr.printText ( "IVA " + total.iva.toQString() + "%:" + ( total.totalIva - total.baseImponible ).toQString() + "�\n" );
    pr.setCharacterPrintMode ( CHARACTER_FONTA_SELECTED | EMPHASIZED_MODE | DOUBLE_HEIGHT | DOUBLE_WIDTH );
    pr.printText ( "TOTAL: " + total.totalIva.toQString() + "�\n" );
    pr.printText ( "\n\n" );
    pr.setJustification ( BtEscPrinter::left );
    pr.setCharacterPrintMode ( CHARACTER_FONTA_SELECTED );
    pr.printText ( "Le ha atendido " + trabajador.nombre + "\n" );
    pr.printText ( "\n" );


    pr.printText ( "Tel. " + empresa.telefono + "\n" );
    pr.printText ( "\n" );

    pr.setJustification ( BtEscPrinter::center );
    pr.setColor ( red );
    pr.printText ( "*** GRACIAS POR SU VISITA ***\n" );

    QByteArray qba = dbValue ( "refalbaran" ).toAscii();
    char* barcode = qba.data();
    pr.setJustification ( BtEscPrinter::center );
    pr.setBarcodeFormat ( 2, 50, both, fontB );
    pr.printBarCode ( code39, qba.size(), barcode );
    pr.cutPaperAndFeed ( TRUE, 10 );
    pr.print();

    g_plugins->lanza("BtTicket_imprimir_Post", this);
    blDebug("END BtTicket::imprimir",0);
}

*/


void BtTicket::imprimir(bool save)
{

    blDebug("BtTicket::imprimir",0);

    if (save) {
        
        if ( guardar() == -1) {
            blDebug ( "Error en la llamada a guardar()", 0 );
            return;
        } // end if
        
    } // end if
    
    /// Disparamos los plugins.
    int res = g_plugins->lanza ( "BtTicket_imprimir", this );
    if ( res != 0 ) {
        g_plugins->lanza("BtTicket_imprimir_Post", this);
        blDebug("END BtTicket::imprimir",0);
        return;
    } // end if
    
    generaRML("ticket_normal.txt");


    if (!g_confpr->valor ( CONF_CASHBOX_FILE).isEmpty() && g_confpr->valor ( CONF_CASHBOX_FILE) != "/dev/null") {
        QString comando = "cat " + g_confpr->valor(CONF_DIR_USER) + "ticket_normal.txt" + "  > " + g_confpr->valor ( CONF_CASHBOX_FILE );
        system ( comando.toAscii().data() );
    } else if (g_confpr->valor(CONF_CUPS_DEFAULT_PRINTER).isEmpty() || g_confpr->valor(CONF_CUPS_DEFAULT_PRINTER) == "None") {
        blDebug("Debe establecer el parametro CONF_CUPS_DEFAULT_PRINTER o CONF_CASHBOX_FILE para abrir el cajon " , 2);
    } else {
        QString comando = "lp -d" + g_confpr->valor(CONF_CUPS_DEFAULT_PRINTER) + " " + g_confpr->valor(CONF_DIR_USER) + "ticket_normal.txt";
        system ( comando.toAscii().data() );
    } // end if    

    
    g_plugins->lanza("BtTicket_imprimir_Post", this);
    blDebug("END BtTicket::imprimir",0);
}


// ===========================

void BtTicket::subir()
{
    blDebug("BtTicket::subir");
    int i;

    if ( listaLineas()->count() > 0) {
       /// Si no hay ninguna linea actual cogemos la ultima linea del ticket como linea actual
      if (m_lineaActual == NULL) {
            i = listaLineas() ->size();
      } else {
            i = listaLineas() ->indexOf ( lineaActBtTicket() );
      }// end if

        if ( i > 0 ) i--;
        setLineaActual ( listaLineas() ->at ( i ) );
        pintar();
    } // end if
    blDebug("END BtTicket::subir");
}

void BtTicket::bajar()
{
    blDebug("BtTicket::bajar");
    if ( listaLineas()->count() > 0 ) {
        int i = listaLineas() ->indexOf ( lineaActBtTicket() );
        if ( i < listaLineas() ->size() - 1 ) i++;
        setLineaActual ( listaLineas() ->at ( i ) );
        pintar();
    } // end if
    blDebug("END BtTicket::bajar");
}

void BtTicket::agregarCantidad ( QString cantidad )
{
    BlFixed cant ( cantidad );
    
    /// Comprueba la existencia de la linea de ticket.
    if ( m_lineaActual == NULL ) {
        blMsgWarning ( "No existe linea" );
        return;
    } // end if
    
    BlFixed cantorig ( m_lineaActual->dbValue ( "cantlalbaran" ) );
    BlFixed suma = cant + cantorig;
    
    if ( suma == BlFixed ( "0.00" ) ) {
        borrarLinea ( m_lineaActual );
    } else {
        m_lineaActual->setDbValue ( "cantlalbaran", suma.toQString() );
    } // end if
    
    pintar();
}

void BtTicket::ponerCantidad ( QString cantidad )
{
    BlFixed cant ( cantidad );

    /// En el TPV si no se ponen decimales por defecto se extiende a 2. Aunque deberia ser un parametro o la longitud de los campos de la
    /// Base de Datos.
    if (cant.precision() < 2)
        cant.setPrecision(2);

    
    /// Comprueba la existencia de la linea de ticket.
    if ( m_lineaActual == NULL ) {
        blMsgWarning ( "No existe linea" );
        return;
    } // end if
    
    if (cantidad.isEmpty()) {
	return;
    } // end if

    if ( cant == 0 ) {
        borrarLinea ( m_lineaActual );
        //listaLineas() ->removeAt ( listaLineas() ->indexOf ( m_lineaActual ));
        //m_lineaActual = listaLineas() ->at ( 0 );
    } else {
        m_lineaActual->setDbValue ( "cantlalbaran", cant.toQString('0', cant.precision()) );
    } // end if
    
    pintar();
}

void BtTicket::ponerPrecio ( QString precio )
{
    BlFixed valor ( precio );
    
    /// Comprueba la existencia de la linea de ticket.
    if ( m_lineaActual == NULL ) {
        blMsgWarning ( "No existe linea" );
        return;
    } // end if
    
    /// En el TPV si no se ponen decimales por defecto se extiende a 2. Aunque deberia ser un parametro o la longitud de los campos de la
    /// Base de Datos.
    if (valor.precision() < 2)
        valor.setPrecision(2);

    m_lineaActual->setDbValue ( "pvplalbaran", valor.toQString('0', valor.precision()) );

    g_plugins->lanza ( "BtTicket_ponerPrecio_Post", this );

    pintar();
}

void BtTicket::insertarArticuloCodigo ( QString codigo )
{
    blDebug ( "BtTicket::insertarArticuloCodigo", 0 );

    QString query = "SELECT idarticulo FROM articulo WHERE codigocompletoarticulo= '" + codigo + "'";
    BlDbRecordSet *cur = mainCompany() ->loadQuery ( query );
    
    if ( !cur->eof() ) {
        insertarArticulo ( cur->valor ( "idarticulo" ), BlFixed ( "1" ) );
    } // end if
    
    delete cur;

    g_plugins->lanza ( "BtTicket_insertarArticuloCodigo_Post", this );
    
    blDebug ( "END BtTicket::insertarArticuloCodigo", 0 );

}


void BtTicket::insertarArticuloCodigoNL ( QString codigo )
{
    blDebug("BtTicket::insertarArticuloCodigoNL",0);
    
    QString query = "SELECT idarticulo FROM articulo WHERE codigocompletoarticulo= '" + codigo + "'";
    BlDbRecordSet *cur = mainCompany() ->loadQuery ( query );
    
    if ( !cur->eof() ) {
        insertarArticulo ( cur->valor ( "idarticulo" ), BlFixed ( "1" ), TRUE );
    } // end if
    
    delete cur;
    
    g_plugins->lanza ( "BtTicket_insertarArticuloCodigoNL_Post", this );
    
    blDebug("END BtTicket::insertarArticuloCodigoNL",0);
}


int BtTicket::cargar ( QString id )
{
    try {
        
        QString query = "SELECT * FROM albaran WHERE idalbaran = " + id;
        BlDbRecordSet *cur = mainCompany() ->loadQuery ( query );
        
        if ( cur ) {
            if ( !cur->eof() ) {
                DBload ( cur );
            }
            delete cur;
        } // end if
        
        cur = mainCompany() ->loadQuery ( "SELECT * FROM lalbaran LEFT JOIN articulo ON lalbaran.idarticulo = articulo.idarticulo WHERE idalbaran = " + id );
        
        while ( !cur->eof() ) {
            BlDbRecord *l = agregarLinea();
            l->DBload ( cur );
            cur->nextRecord();
        } // end while
        
        delete cur;
        
    } catch ( ... ) {
        blMsgInfo ( "Error en la carga" );
    }
    
    return 0;
}

/// Guarda los datos de la ficha en la base de datos.
/**
  Este metodo guarda los contenidos de la Ficha (siempre que esta haya sido inicializada).
  Luego llama a plugins y a guardarPost por si se quieren hacer acciones adicionales de guardado.
  Tras guardar todos los elementos hace una carga.
\return 0 Si no hay problemas. -1 Si ha habido problemas.
**/
int BtTicket::guardar()
{
    blDebug ( "BtTicket::guardar", 0 );

    try {

        if ( listaLineas() ->count() == 0 ) {
                blMsgWarning ( _( "El ticket esta vacio." ) );
                return -1;
        } // end if

        QString id;
        mainCompany() ->begin();
        DBsave ( id );

        BlDbRecord *item;
        
        for ( int i = 0; i < listaLineas() ->size(); ++i ) {
            QString id1;
            item = listaLineas() ->at ( i );
            int f = item->setDbValue ( "idalbaran", id );
            item->setDbValue ( "ordenlalbaran", QString::number ( i ) );
            item->DBsave ( id1 );
        } // end for
	    fprintf(stderr, "G\n");
        
        setDbValue("idalbaran", id);
        BlDbRecordSet *cur = mainCompany() ->loadQuery ( "SELECT * FROM albaran WHERE idalbaran = " + id );
        setDbValue("refalbaran", cur->valor("refalbaran"));
        setDbValue("numalbaran", cur->valor("numalbaran"));

        mainCompany() ->commit();

        delete cur;
        blDebug ( "END BtTicket::guardar", 0 );
        
        return 0;
        
    } catch ( ... ) {
    
        blMsgInfo ( "Error inesperado con la base de datos" );
        mainCompany() ->rollback();
        
        return -1;
        
    } // end try
}

void BtTicket::borrarLinea ( BlDbRecord *linea )
{
    if ( linea == NULL )
        return;

    g_plugParams = (void *) linea;
    g_plugins->lanza("BtTicket_borrarLinea", this);
       
    int numlinea = listaLineas()->indexOf ( linea );

    if (dbValue("numalbaran") != "") {
	blMsgInfo("Operacion no permitida. Debe Cobrar el Ticket");
	return;
    } // end if

    if ( linea == m_lineaActual ) {
        listaLineas() ->removeAt ( listaLineas() ->indexOf ( linea ) );
        
        if ( numlinea > listaLineas()->count() - 1 ) {
            m_lineaActual = listaLineas()->count() > 0 ? listaLineas()->at ( listaLineas()->count() - 1 ) : NULL;
        } else {
            m_lineaActual = listaLineas()->count() > 0 ? listaLineas() ->at ( numlinea ) : NULL;
        }
    } else {
        listaLineas() ->removeAt ( listaLineas() ->indexOf ( linea ) );
    } // end if
}


/// Hace la exportacion del campo a XML
QString BtTicket::exportXML() {
    blDebug ( "BtTicket::exportXML", 0 );
    QString val;
    int error;
    BlDbField *campo;

    val = "<BTTICKET>\n";
    val += "\t<NOMTICKET>" + dbValue("nomticket") + "</NOMTICKET>\n"; 
    val += "\t" + BlDbRecord::exportXML().replace("\t<","\t\t<").replace("\n<","\n\t<");
    val += "\t<LISTALINEAS>\n";
    BlDbRecord *linea1;
    for ( int i = 0; i < m_listaLineas->size(); ++i ) {
        linea1 = m_listaLineas->at ( i );
        val += "\t\t" + linea1->exportXML().replace("\t<","\t\t\t<").replace("\n<","\n\t\t<");
    } // end for
    val += "\t</LISTALINEAS>\n";
    val += "</BTTICKET>\n";

    return val;
    blDebug ( "END BtTicket::exportXML", 0 );
}


bool BtTicket::syncXML(const QString &text, bool insertarSiempre) {
    blDebug ( "BtTicket::syncXML", 0 );


    QDomDocument doc ( "mydocument" );

    if ( !doc.setContent ( text ) ) {
        blDebug ( "END BtCompany::syncXML", 0, "XML Invalido" );
        return 0;
    } // end if

    QDomElement docElem = doc.documentElement();
    QDomElement principal = docElem.firstChildElement ( "BLDBRECORD" );
    /// Comprobamos que los nombres coinciden y si no coinciden salimos pq no ha lugar a sincronizacion.
    if (!insertarSiempre) {
	QDomElement nomticket = docElem.firstChildElement ( "NOMTICKET" );
	if (nomticket.text() != dbValue("nomticket") ) {
	    blDebug("END BtTicket::syncXML", 0);
	    return 0;
	} // end if
    } // end if
    
    QString result;
    QTextStream stream ( &result );
    principal.save(stream,5);
    m_listaLineas->clear();
    BlDbRecord::syncXML(result);

    /// Cogemos la coordenada X

    docElem = doc.documentElement();
    principal = docElem.firstChildElement ( "LISTALINEAS" );
    /// Cogemos la coordenada X
    QDomNodeList nodos = principal.elementsByTagName ( "BLDBRECORD" );
    int i = 0;
    while (i < nodos.count()) {
        QDomNode ventana = nodos.item ( i++ );
        QDomElement e1 = ventana.toElement(); /// try to convert the node to an element.
        if ( !e1.isNull() ) { /// the node was really an element.
            QString nodoText = e1.text();
            /// Pasamos el XML a texto para poder procesarlo como tal.
            QString result;
            QTextStream stream ( &result );
            ventana.save(stream,5);
            /// Para cada elemento de la lista de tickets intentamos sincronizar el ticket. (Basandonos en el id y en el nombre de mesa).
            /// Si ho ha habido sincronizacion con ningun elemento
            /// Creamos un ticket nuevo y lo sincronizamos.
            BlDbRecord *linea1 = agregarLinea();
            linea1->syncXML(result);
            m_lineaActual = linea1;
        } // end if
    } // end while
    return 1;
    blDebug ( "BtTicket::syncXML", 0 );
}





/// ================================ VAMOS A TRATAR DE HACER LA IMPRESION SENCILLA CON PLANTILLAS ============================

/// Sustituye valores en el texto pasado como variables por su valor.
/// tipoEscape puede ser 0 --> Sin parseo
///    1 --> ParseoXML
///    2 --> ParseoPython
void BtTicket::substrVars ( QByteArray &buff, int tipoEscape )
{

    int pos = 0;

    /// Tratamos la sustitucion de variables de m_variables
    QMapIterator<QString, QString> i ( m_variables );
    while ( i.hasNext() ) {
        i.next();
        buff.replace ( ("[" + i.key() + "]").toAscii(), i.value().toAscii() );
    } // end while


    substrConf ( buff );

    pos =  0;

    /// Buscamos parametros en el query y los ponemos.
    QRegExp rx ( "\\[(\\w*)\\]" );
    while ( ( pos = rx.indexIn ( buff, pos ) ) != -1 ) {
        if ( exists ( rx.cap ( 1 ) ) ) {
                        
            switch ( tipoEscape ) {
            case 1:
                buff.replace ( pos, rx.matchedLength(), blXMLEscape ( dbValue ( rx.cap ( 1 ) ) ).toAscii() );
                break;
            case 2:
                buff.replace ( pos, rx.matchedLength(), blPythonEscape ( dbValue ( rx.cap ( 1 ) ) ).toAscii() );
                break;
            default:
                buff.replace ( pos, rx.matchedLength(), dbValue ( rx.cap ( 1 ) ).toAscii() );

            } // end switch

            pos = 0;
        } else {
            pos += rx.matchedLength();
        }
    } // end while

}


/// Busca strings del tipo [xxxx] entro del texto pasado y los sustituye
/// Por valores existentes en la base de datos.
/// Tambien busca los parametros PARAM e IFACE para tambien tratarlos.
/// Devuelve 1 Si todo esta OK o 0 Si hay algun error
/**
\param buff El texto entero sobre el que se hace el reemplazo de sentencias.
**/
int BtTicket::trataTags ( QByteArray &buff, int tipoEscape )
{
    blDebug ( "BtTicket::trataTags", 0 );

    ///Buscamos interfaces, los preguntamos y los ponemos.
    int pos = 0;
    QByteArray cadant;

    QRegExp rx70 ( "<!--\\s*IFACE\\s*SRC\\s*=\\s*\"([^\"]*)\"\\s*-->" );
    rx70.setMinimal ( TRUE );
    while ( ( pos = rx70.indexIn ( buff, pos ) ) != -1 ) {
        QString cadarchivo = rx70.cap ( 1 );

        substrConf ( cadarchivo );

        QFile fichero ( cadarchivo );
        if ( fichero.exists() ) {
            QUiLoader loader;
            fichero.open ( QFile::ReadOnly );
            QWidget *iface = loader.load ( &fichero, this );
            fichero.close();
            QDialog *diag = new QDialog ( 0 );
            diag->setModal ( true );

            /// Creamos un layout donde estara el contenido de la ventana y la ajustamos al QDialog
            /// para que sea redimensionable y aparezca el titulo de la ventana.
            QHBoxLayout *layout = new QHBoxLayout;
            layout->addWidget ( iface );
            layout->setMargin ( 5 );
            layout->setSpacing ( 5 );
            diag->setLayout ( layout );
            diag->setWindowTitle ( iface->windowTitle() );
            diag->setGeometry ( 0, 0, iface->width(), iface->height() );
            QPushButton *button = iface->findChild<QPushButton *> ( "mui_aceptar" );
            connect ( button, SIGNAL ( released ( ) ), diag, SLOT ( accept() ) );
            QPushButton *button1 = iface->findChild<QPushButton *> ( "mui_cancelar" );
            connect ( button1, SIGNAL ( released ( ) ), diag, SLOT ( reject() ) );
            int ret = diag->exec();
            if ( ret ) {

         /// Recorre los QLineEdit.
                QList<QLineEdit *> l2 = iface->findChildren<QLineEdit *>();
                QListIterator<QLineEdit *> it2 ( l2 );
                while ( it2.hasNext() ) {
                    QLineEdit * item = it2.next();
                    QString nombre = item->objectName().right ( item->objectName().size() - 4 );
                    QString valor = item->text();
                    addDbField ( nombre, BlDbField::DbVarChar, BlDbField::DbNoSave, nombre  );
                    setDbValue ( nombre, valor );
                } // end while
                
                /// Recorre los QCheckBox.
                QList<QCheckBox *> qcb = iface->findChildren<QCheckBox *>();
                QListIterator<QCheckBox *> itqcb ( qcb );

                while ( itqcb.hasNext() ) {
                    QCheckBox *item = itqcb.next();
                    QString nombre = item->objectName().right ( item->objectName().size() - 4 );
                    QString valor = "false";
                    if ( item->isChecked() ) {
                        valor = "true";
                    } else {
                  valor = "false";
          } // end if
                    addDbField ( nombre, BlDbField::DbBoolean, BlDbField::DbNoSave, nombre  );
                    setDbValue ( nombre, valor );
                } // end while
                
      QVariant exportaRML = iface->property("exportaRML");
      if (exportaRML.isValid() ) {
         QStringList props = exportaRML.toStringList();
      
         QListIterator<QString> iProps(props);
         while (iProps.hasNext()) {
            QString camp = iProps.next();
            QStringList cami = camp.split(".");
            QObject *actual=iface;
            QListIterator<QString> iCami(cami);
            
            while(iCami.hasNext() && actual) {

                QString nom = iCami.next();
                QObject *fill = actual->findChild<QObject *>("mui_"+nom);

                if (!fill) {
               fill = actual->findChild<QObject *>(nom);
                } // end if

                if (fill) {
               actual = fill;
                } else {
                
               QVariant valor = actual->property(nom.toUtf8().data());
               m_variables[camp] =valor.toString();
               
               if (valor.canConvert<QObject*>()) {
                   actual = valor.value<QObject*>();
               } else {
                   actual = NULL;
               } // end if
            
                } // end if
                
            } // end while
            
         } // end while
         
      } // end if

            } // end if
            
            delete diag;
            
            // Si se ha pulsado cancelar entonce se sale del informe.
            if ( !ret ) return 0;
            
        } else {
            blMsgInfo ( "Archivo de Interficie no existe" );
        } // end if

        buff.replace ( pos, rx70.matchedLength(), "" );
        pos = 0;
    } // end while


    /// Buscamos establecimiento de variables y los ponemos en m_variables
    pos = buff.indexOf("<!-- SETVAR");
    cadant = buff.left(pos);
    buff = buff.right(buff.length()-pos);
    QRegExp rx54 ( "<!--\\s*SETVAR\\s*NAME\\s*=\\s*\"([^\"]*)\"\\s*VALUE\\s*=\\s*\"([^\"]*)\"\\s*-->" );
    rx54.setMinimal ( TRUE );
    while ( ( pos = rx54.indexIn ( buff, 0 ) ) != -1 ) {
        QByteArray valor = rx54.cap ( 2 ).toAscii();
        substrVars ( valor, tipoEscape );
        m_variables[rx54.cap ( 1 ) ] = valor;
        buff.replace ( pos, rx54.matchedLength(), "" );
        pos = buff.indexOf("<!--");
    } // end while
    buff = cadant + buff;
    
    /// Tratamos las variables establecidas.
    substrVars ( buff, tipoEscape );


    /// Inclusion de ficheros de codigo
    pos = buff.indexOf("<!-- INCLUDE");
    cadant = buff.left(pos);
    buff = buff.right(buff.length()-pos);
    QRegExp rx19 ( "<!--\\s*INCLUDE\\s*FILE\\s*=\\s*\"([^\"]*)\"\\s*-->" );
    rx19.setMinimal ( TRUE );
    while ( ( pos = rx19.indexIn ( buff, 0 ) ) != -1 ) {
        QString ldetalle = trataIncludeFile ( rx19.cap ( 1 ), tipoEscape );
        buff.replace ( pos, rx19.matchedLength(), ldetalle.toAscii() );
        pos = buff.indexOf("<!--");
    } // end while
    buff = cadant + buff;

    /// Buscamos Existencia de Ficheros
    pos = buff.indexOf("<!-- EXISTS FILE");
    cadant = buff.left(pos);
    buff = buff.right(buff.length()-pos);
    QRegExp rx9 ( "<!--\\s*EXISTS\\s*FILE\\s*=\\s*\"([^\"]*)\"\\s*-->(.*)<!--\\s*END\\s*EXISTS\\s*-->" );
    rx9.setMinimal ( TRUE );
    while ( ( pos = rx9.indexIn ( buff, 0 ) ) != -1 ) {
        QString ldetalle = trataExists ( rx9.cap ( 1 ), rx9.cap ( 2 ).toAscii() );
        buff.replace ( pos, rx9.matchedLength(), ldetalle.toAscii() );
        pos = buff.indexOf("<!--");
    } // end while
    buff = cadant + buff;
    
    /// Buscamos Query's en condicional
    pos = buff.indexOf("<!-- IF QUERY");
    cadant = buff.left(pos);
    buff = buff.right(buff.length()-pos);
    QRegExp rx4 ( "<!--\\s*IF\\s*QUERY\\s*=\\s*\"([^\"]*)\"\\s*-->(.*)<!--\\s*END\\s*IF\\s*QUERY\\s*-->" );
    rx4.setMinimal ( TRUE );
    while ( ( pos = rx4.indexIn ( buff, 0 ) ) != -1 ) {
        QString ldetalle = trataIfQuery ( rx4.cap ( 1 ), rx4.cap ( 2 ).toAscii() );
        buff.replace ( pos, rx4.matchedLength(), ldetalle.toAscii() );
        pos = buff.indexOf("<!--");
    } // end while
    buff = cadant + buff;
    
    
    /// Buscamos Query's por tratar
    pos = buff.indexOf("<!-- QUERY");
    cadant = buff.left(pos);
    buff = buff.right(buff.length()-pos);
    QRegExp rx1 ( "<!--\\s*QUERY\\s*=\\s*\"([^\"]*)\"\\s*-->(.*)<!--\\s*END\\s*QUERY\\s*-->" );
    rx1.setMinimal ( TRUE );
    while ( ( pos = rx1.indexIn ( buff, 0 ) ) != -1 ) {
        QByteArray ldetalle = trataQuery ( rx1.cap ( 1 ), rx1.cap ( 2 ).toAscii(), tipoEscape );
        buff.replace ( pos, rx1.matchedLength(), ldetalle );
        pos = buff.indexOf("<!--");
    } // end while
    buff = cadant + buff;
    
    /// Buscamos SubQuery's en condicional
    pos = buff.indexOf("<!-- IF SUBQUERY");
    cadant = buff.left(pos);
    buff = buff.right(buff.length()-pos);
    QRegExp rx14 ( "<!--\\s*IF\\s*SUBQUERY\\s*=\\s*\"([^\"]*)\"\\s*-->(.*)<!--\\s*END\\s*IF\\s*SUBQUERY\\s*-->" );
    rx14.setMinimal ( TRUE );
    while ( ( pos = rx14.indexIn ( buff, 0 ) ) != -1 ) {
        QString ldetalle = trataIfQuery ( rx14.cap ( 1 ), rx14.cap ( 2 ).toAscii() );
        buff.replace ( pos, rx14.matchedLength(), ldetalle.toAscii() );
        pos = buff.indexOf("<!--");
    } // end while
    buff = cadant + buff;
    
    /// Buscamos SubQuery's por tratar
    pos = buff.indexOf("<!-- SUBQUERY");
    cadant = buff.left(pos);
    buff = buff.right(buff.length()-pos);
    QRegExp rx7 ( "<!--\\s*SUBQUERY\\s*=\\s*\"([^\"]*)\"\\s*-->(.*)<!--\\s*END\\s*SUBQUERY\\s*-->" );
    rx7.setMinimal ( TRUE );
    while ( ( pos = rx7.indexIn ( buff, 0 ) ) != -1 ) {
        QString ldetalle = trataQuery ( rx7.cap ( 1 ), rx7.cap ( 2 ).toAscii(), tipoEscape );
        buff.replace ( pos, rx7.matchedLength(), ldetalle.toAscii() );
        pos = buff.indexOf("<!--");
    } // end while
    buff = cadant + buff;
    
    /// Buscamos Query's en condicional
    pos = buff.indexOf("<!-- IF");
    cadant = buff.left(pos);
    buff = buff.right(buff.length()-pos);
    QRegExp rx11 ( "<!--\\s*IF\\s*=\\s*\"([^\"]*)\"\\s*-->(.*)<!--\\s*ELSE\\s*-->(.*)<!--\\s*END\\s*IF\\s*-->" );
    rx11.setMinimal ( TRUE );
    while ( ( pos = rx11.indexIn ( buff, 0 ) ) != -1 ) {
        QString ldetalle = trataIf ( rx11.cap ( 1 ), rx11.cap ( 2 ).toAscii(), rx11.cap ( 3 ).toAscii() );
        buff.replace ( pos, rx11.matchedLength(), ldetalle.toAscii() );
        pos = buff.indexOf("<!--");
    } // end while
    buff = cadant + buff;
    
    /// Cambio del CharacterPrinterMode
    pos = buff.indexOf("<!-- SETCHARACTERPRINTMODE");
    cadant = buff.left(pos);
    buff = buff.right(buff.length()-pos);
    QRegExp rx39 ( "<!--\\s*SETCHARACTERPRINTMODE\\s*\"([^\"]*)\"\\s*-->" );
    rx39.setMinimal ( TRUE );
    while ( ( pos = rx39.indexIn ( buff, 0 ) ) != -1 ) {
        QByteArray ldetalle = trataSetCharacterPrintMode ( rx39.cap ( 1 ), tipoEscape );
        buff.replace ( pos, rx39.matchedLength(), ldetalle );
	buff = cadant + buff;
        pos = buff.indexOf("<!-- SETCHARACTERPRINTMODE");
	cadant = buff.left(pos);
	buff = buff.right(buff.length()-pos);
    } // end while
    buff = cadant + buff;

    /// Cambio del CharacterPrinterMode
    pos = buff.indexOf("<!-- SETCHARACTERSPACING");
    cadant = buff.left(pos);
    buff = buff.right(buff.length()-pos);
    QRegExp rx49 ( "<!--\\s*SETCHARACTERSPACING\\s*\"([^\"]*)\"\\s*-->" );
    rx49.setMinimal ( TRUE );
    while ( ( pos = rx49.indexIn ( buff, 0 ) ) != -1 ) {
        QByteArray ldetalle = trataSetCharacterSpacing ( rx49.cap ( 1 ), tipoEscape );
        buff.replace ( pos, rx49.matchedLength(), ldetalle );
        pos = buff.indexOf("<!--");
    } // end while
    buff = cadant + buff;
    
    /// Cambio del CharacterCodeTable
    pos = buff.indexOf("<!-- SETCHARACTERCODETABLE");
    cadant = buff.left(pos);
    buff = buff.right(buff.length()-pos);
    QRegExp rx59 ( "<!--\\s*SETCHARACTERCODETABLE\\s*\"([^\"]*)\"\\s*-->" );
    rx59.setMinimal ( TRUE );
    while ( ( pos = rx59.indexIn ( buff, 0 ) ) != -1 ) {
        QByteArray ldetalle = trataSetCharacterCodeTable ( rx59.cap ( 1 ), tipoEscape );
        buff.replace ( pos, rx59.matchedLength(), ldetalle );
        pos = buff.indexOf("<!--");
    } // end while
    buff = cadant + buff;
    
    /// Cambio del CharacterCodeTable
    pos = buff.indexOf("<!-- SETUNDERLINEMODE");
    cadant = buff.left(pos);
    buff = buff.right(buff.length()-pos);
    QRegExp rx69 ( "<!--\\s*SETUNDERLINEMODE\\s*\"([^\"]*)\"\\s*-->" );
    rx69.setMinimal ( TRUE );
    while ( ( pos = rx69.indexIn ( buff, 0 ) ) != -1 ) {
        QByteArray ldetalle = trataSetUnderlineMode ( rx69.cap ( 1 ), tipoEscape );
        buff.replace ( pos, rx69.matchedLength(), ldetalle );
	buff = cadant + buff;
        pos = buff.indexOf("<!-- SETUNDERLINEMODE");
	cadant = buff.left(pos);
	buff = buff.right(buff.length()-pos);
    } // end while
    buff = cadant + buff;
    
    
    /// Cambio del CharacterSize
    pos = buff.indexOf("<!-- SETCHARACTERSIZE");
    cadant = buff.left(pos);
    buff = buff.right(buff.length()-pos);
    QRegExp rx79 ( "<!--\\s*SETCHARACTERSIZE\\s*\"([^\"]*)\"\\s*-->" );
    rx79.setMinimal ( TRUE );
    while ( ( pos = rx79.indexIn ( buff, 0 ) ) != -1 ) {
        QByteArray ldetalle = trataSetCharacterSize ( rx79.cap ( 1 ), tipoEscape );
        buff.replace ( pos, rx79.matchedLength(), ldetalle );
	buff = cadant + buff;
        pos = buff.indexOf("<!-- SETCHARACTERSIZE");
	cadant = buff.left(pos);
	buff = buff.right(buff.length()-pos);
    } // end while
    buff = cadant + buff;
    
    /// Cambio del Smoothing
    pos = buff.indexOf("<!-- SETSMOOTHING");
    cadant = buff.left(pos);
    buff = buff.right(buff.length()-pos);
    QRegExp rx89 ( "<!--\\s*SETSMOOTHING\\s*\"([^\"]*)\"\\s*-->" );
    rx89.setMinimal ( TRUE );
    while ( ( pos = rx89.indexIn ( buff, 0 ) ) != -1 ) {
        QByteArray ldetalle = trataSetCharacterSize ( rx89.cap ( 1 ), tipoEscape );
        buff.replace ( pos, rx89.matchedLength(), ldetalle );
        pos = buff.indexOf("<!--");
    } // end while
    buff = cadant + buff;

    /// Cambio del DoubleStrike
    pos = buff.indexOf("<!-- SETDOUBLESTRIKE");
    cadant = buff.left(pos);
    buff = buff.right(buff.length()-pos);
    QRegExp rx99 ( "<!--\\s*SETDOUBLESTRIKE\\s*\"([^\"]*)\"\\s*-->" );
    rx99.setMinimal ( TRUE );
    while ( ( pos = rx99.indexIn ( buff, 0 ) ) != -1 ) {
        QByteArray ldetalle = trataSetDoubleStrike ( rx99.cap ( 1 ), tipoEscape );
        buff.replace ( pos, rx99.matchedLength(), ldetalle );
        pos = buff.indexOf("<!--");
    } // end while
    buff = cadant + buff;
    
    /// Cambio del turnUpsideDown
    pos = buff.indexOf("<!-- TURNUPSIDEDOWN");
    cadant = buff.left(pos);
    buff = buff.right(buff.length()-pos);
    QRegExp rx02 ( "<!--\\s*TURNUPSIDEDOWN\\s*\"([^\"]*)\"\\s*-->" );
    rx02.setMinimal ( TRUE );
    while ( ( pos = rx02.indexIn ( buff, 0 ) ) != -1 ) {
        QByteArray ldetalle = trataTurnUpsideDown ( rx02.cap ( 1 ), tipoEscape );
        buff.replace ( pos, rx02.matchedLength(), ldetalle );
        pos = buff.indexOf("<!--");
    } // end while
    buff = cadant + buff;
    
    /// Cambio del turn90CWRotation
    pos = buff.indexOf("<!-- TURN90CWROTATION");
    cadant = buff.left(pos);
    buff = buff.right(buff.length()-pos);
    QRegExp rx12 ( "<!--\\s*TURN90CWROTATION\\s*\"([^\"]*)\"\\s*\"([^\"]*)\"\\s*-->" );
    rx12.setMinimal ( TRUE );
    while ( ( pos = rx12.indexIn ( buff, 0 ) ) != -1 ) {
        QByteArray ldetalle = trataTurn90CWRotation (rx12.cap(1), rx12.cap(2), tipoEscape );
        buff.replace ( pos, rx12.matchedLength(), ldetalle );
	buff = cadant + buff;
        pos = buff.indexOf("<!-- TURN90CWROTATION");
	cadant = buff.left(pos);
	buff = buff.right(buff.length()-pos);
    } // end while
    buff = cadant + buff;
    
    /// Cambio del turnWhiteBlack
    pos = buff.indexOf("<!-- TURNWHITEBLACK");
    cadant = buff.left(pos);
    buff = buff.right(buff.length()-pos);
    QRegExp rx22 ( "<!--\\s*TURNWHITEBLACK\\s*\"([^\"]*)\"\\s*-->" );
    rx22.setMinimal ( TRUE );
    while ( ( pos = rx22.indexIn ( buff, 0 ) ) != -1 ) {
        QByteArray ldetalle = trataTurnWhiteBlack( rx22.cap ( 1 ), tipoEscape );
        buff.replace ( pos, rx22.matchedLength(), ldetalle );
	buff = cadant + buff;
        pos = buff.indexOf("<!-- TURNWHITEBLACK");
	cadant = buff.left(pos);
	buff = buff.right(buff.length()-pos);
    } // end while
    buff = cadant + buff;
    
    /// Cambio del setColor
    pos = buff.indexOf("<!-- SETCOLOR");
    cadant = buff.left(pos);
    buff = buff.right(buff.length()-pos);
    QRegExp rx32 ( "<!--\\s*SETCOLOR\\s*\"([^\"]*)\"\\s*-->" );
    rx32.setMinimal ( TRUE );
    while ( ( pos = rx32.indexIn ( buff, 0 ) ) != -1 ) {
        QByteArray ldetalle = trataSetColor( rx32.cap ( 1 ), tipoEscape );
        buff.replace ( pos, rx32.matchedLength(), ldetalle );
        pos = buff.indexOf("<!--");
    } // end while
    buff = cadant + buff;
    
    /// Cambio del HorizontalTab
    pos = buff.indexOf("<!-- HORIZONTALTAB");
    cadant = buff.left(pos);
    buff = buff.right(buff.length()-pos);
    QRegExp rx42 ( "<!--\\s*HORIZONTALTAB\\s*-->" );
    rx42.setMinimal ( TRUE );
    while ( ( pos = rx42.indexIn ( buff, 0 ) ) != -1 ) {
        QByteArray ldetalle = trataHorizontalTab( tipoEscape );
        buff.replace ( pos, rx42.matchedLength(), ldetalle );
        pos = buff.indexOf("<!--");
    } // end while
    buff = cadant + buff;
    
    /// Cambio del setHorizontalTabPos
    pos = buff.indexOf("<!-- SETHORIZONTALTABPOS");
    cadant = buff.left(pos);
    buff = buff.right(buff.length()-pos);
    QRegExp rx52 ( "<!--\\s*SETHORIZONTALTABPOS\\s*\"([^\"]*)\"\\s*\"([^\"]*)\"\\s*-->" );
    rx52.setMinimal ( TRUE );
    while ( ( pos = rx52.indexIn ( buff, 0 ) ) != -1 ) {
        QByteArray ldetalle = trataSetHorizontalTabPos (rx52.cap(1), rx52.cap(2), tipoEscape );
        buff.replace ( pos, rx52.matchedLength(), ldetalle );
        pos = buff.indexOf("<!--");
    } // end while
    buff = cadant + buff;
    
    
    /// Cambio del setLeftMargin
    pos = buff.indexOf("<!-- SETLEFTMARGIN");
    cadant = buff.left(pos);
    buff = buff.right(buff.length()-pos);
    QRegExp rx62 ( "<!--\\s*SETLEFTMARGIN\\s*\"([^\"]*)\"\\s*-->" );
    rx62.setMinimal ( TRUE );
    while ( ( pos = rx62.indexIn ( buff, 0 ) ) != -1 ) {
        QByteArray ldetalle = trataSetLeftMargin( rx62.cap ( 1 ), tipoEscape );
        buff.replace ( pos, rx62.matchedLength(), ldetalle );
        pos = buff.indexOf("<!--");
    } // end while
    buff = cadant + buff;
    
    /// Cambio del setPrintingAreaWidth
    pos = buff.indexOf("<!-- SETPRINTINGAREAWIDTH");
    cadant = buff.left(pos);
    buff = buff.right(buff.length()-pos);
    QRegExp rx72 ( "<!--\\s*SETPRINTINGAREAWIDTH\\s*\"([^\"]*)\"\\s*-->" );
    rx72.setMinimal ( TRUE );
    while ( ( pos = rx72.indexIn ( buff, 0 ) ) != -1 ) {
        QByteArray ldetalle = trataSetPrintingAreaWidth( rx72.cap ( 1 ), tipoEscape );
        buff.replace ( pos, rx72.matchedLength(), ldetalle );
        pos = buff.indexOf("<!--");
    } // end while
    buff = cadant + buff;
    
    
    /// Cambio del setJustification
    pos = buff.indexOf("<!-- SETJUSTIFICATION");
    cadant = buff.left(pos);
    buff = buff.right(buff.length()-pos);
    QRegExp rx82 ( "<!--\\s*SETJUSTIFICATION\\s*\"([^\"]*)\"\\s*-->" );
    rx82.setMinimal ( TRUE );
    while ( ( pos = rx82.indexIn ( buff, 0 ) ) != -1 ) {
        QByteArray ldetalle = trataSetJustification( rx82.cap ( 1 ), tipoEscape );
        buff.replace ( pos, rx82.matchedLength(), ldetalle );
	buff = cadant + buff;
        pos = buff.indexOf("<!-- SETJUSTIFICATION");
	cadant = buff.left(pos);
	buff = buff.right(buff.length()-pos);
    } // end while
    buff = cadant + buff;
    
    /// Cambio del setJustification
    pos = buff.indexOf("<!-- SETHABSOLUTEPOS");
    cadant = buff.left(pos);
    buff = buff.right(buff.length()-pos);
    QRegExp rx92 ( "<!--\\s*SETHABSOLUTEPOS\\s*\"([^\"]*)\"\\s*-->" );
    rx92.setMinimal ( TRUE );
    while ( ( pos = rx92.indexIn ( buff, 0 ) ) != -1 ) {
        QByteArray ldetalle = trataSetHAbsolutePos( rx92.cap ( 1 ), tipoEscape );
        buff.replace ( pos, rx92.matchedLength(), ldetalle );
        pos = buff.indexOf("<!--");
    } // end while
    buff = cadant + buff;
    
    
    /// Cambio del setJustification
    pos = buff.indexOf("<!-- SETHRELATIVEPOS");
    cadant = buff.left(pos);
    buff = buff.right(buff.length()-pos);
    QRegExp rx03 ( "<!--\\s*SETHRELATIVEPOS\\s*\"([^\"]*)\"\\s*-->" );
    rx03.setMinimal ( TRUE );
    while ( ( pos = rx03.indexIn ( buff, 0 ) ) != -1 ) {
        QByteArray ldetalle = trataSetHRelativePos( rx03.cap ( 1 ), tipoEscape );
        buff.replace ( pos, rx03.matchedLength(), ldetalle );
        pos = buff.indexOf("<!--");
    } // end while
    buff = cadant + buff;
    
    /// Cambio del setBARCODEFORMAT
    pos = buff.indexOf("<!-- SETBARCODEFORMAT");
    cadant = buff.left(pos);
    buff = buff.right(buff.length()-pos);
    QRegExp rx13 ( "<!--\\s*SETBARCODEFORMAT\\s*\"([^\"]*)\"\\s*\"([^\"]*)\"\\s*\"([^\"]*)\"\\s*\"([^\"]*)\"\\s*-->" );
    rx13.setMinimal ( TRUE );
    while ( ( pos = rx13.indexIn ( buff, 0 ) ) != -1 ) {
        QByteArray ldetalle = trataSetBarcodeFormat( rx13.cap ( 1 ), rx13.cap ( 2 ),rx13.cap ( 3 ),rx13.cap ( 4 ),tipoEscape );
        buff.replace ( pos, rx13.matchedLength(), ldetalle );
        pos = buff.indexOf("<!--");
    } // end while
    buff = cadant + buff;
    
    /// Cambio del PRINTBARCODE
    pos = buff.indexOf("<!-- PRINTBARCODE");
    cadant = buff.left(pos);
    buff = buff.right(buff.length()-pos);
    QRegExp rx23 ( "<!--\\s*PRINTBARCODE\\s*\"([^\"]*)\"\\s*\"([^\"]*)\"\\s*\"([^\"]*)\"\\s*-->" );
    rx23.setMinimal ( TRUE );
    while ( ( pos = rx23.indexIn ( buff, 0 ) ) != -1 ) {
        QByteArray ldetalle = trataPrintBarCode( rx23.cap ( 1 ), rx23.cap ( 2 ),rx23.cap ( 3 ),tipoEscape );
        buff.replace ( pos, rx23.matchedLength(), ldetalle );
        pos = buff.indexOf("<!--");
    } // end while
    buff = cadant + buff;
    
    /// Cambio del SETBARCODEHEIGHT
    pos = buff.indexOf("<!-- SETBARCODEHEIGHT");
    cadant = buff.left(pos);
    buff = buff.right(buff.length()-pos);
    QRegExp rx33 ( "<!--\\s*SETBARCODEHEIGHT\\s*\"([^\"]*)\"\\s*-->" );
    rx33.setMinimal ( TRUE );
    while ( ( pos = rx33.indexIn ( buff, 0 ) ) != -1 ) {
        QByteArray ldetalle = trataSetBarCodeHeight( rx33.cap ( 1 ), tipoEscape );
        buff.replace ( pos, rx33.matchedLength(), ldetalle );
        pos = buff.indexOf("<!--");
    } // end while
    buff = cadant + buff;
    
    /// Cambio del SETBARCODEWIDTH
    pos = buff.indexOf("<!-- SETBARCODEWIDTH");
    cadant = buff.left(pos);
    buff = buff.right(buff.length()-pos);
    QRegExp rx34 ( "<!--\\s*SETBARCODEWIDTH\\s*\"([^\"]*)\"\\s*-->" );
    rx34.setMinimal ( TRUE );
    while ( ( pos = rx34.indexIn ( buff, 0 ) ) != -1 ) {
        QByteArray ldetalle = trataSetBarCodeWidth( rx34.cap ( 1 ), tipoEscape );
        buff.replace ( pos, rx34.matchedLength(), ldetalle );
        pos = buff.indexOf("<!--");
    } // end while
    buff = cadant + buff;
    
    /// Cambio del SELECTPAGEMODE
    pos = buff.indexOf("<!-- SELECTPAGEMODE");
    cadant = buff.left(pos);
    buff = buff.right(buff.length()-pos);
    QRegExp rx35 ( "<!--\\s*SELECTPAGEMODE\\s*-->" );
    rx35.setMinimal ( TRUE );
    while ( ( pos = rx35.indexIn ( buff, 0 ) ) != -1 ) {
        QByteArray ldetalle = trataSelectPageMode( tipoEscape );
        buff.replace ( pos, rx35.matchedLength(), ldetalle );
        pos = buff.indexOf("<!--");
    } // end while
    buff = cadant + buff;
    
    /// Cambio del SETBARCODEWIDTH
    pos = buff.indexOf("<!-- SETPRINTAREA");
    cadant = buff.left(pos);
    buff = buff.right(buff.length()-pos);
    QRegExp rx36 ( "<!--\\s*SETPRINTAREA\\s*\"([^\"]*)\"\\s*\"([^\"]*)\"\\s*\"([^\"]*)\"\\s*\"([^\"]*)\"\\s*-->" );
    rx36.setMinimal ( TRUE );
    while ( ( pos = rx36.indexIn ( buff, 0 ) ) != -1 ) {
        QByteArray ldetalle = trataSetPrintArea( rx36.cap ( 1 ), rx36.cap ( 2 ), rx36.cap ( 3 ), rx36.cap ( 4 ), tipoEscape );
        buff.replace ( pos, rx36.matchedLength(), ldetalle );
        pos = buff.indexOf("<!--");
    } // end while
    buff = cadant + buff;
    
    /// Cambio del SETPRINTDIRECTION
    pos = buff.indexOf("<!-- SETPRINTDIRECTION");
    cadant = buff.left(pos);
    buff = buff.right(buff.length()-pos);
    QRegExp rx37 ( "<!--\\s*SETPRINTDIRECTION\\s*\"([^\"]*)\"\\s*-->" );
    rx37.setMinimal ( TRUE );
    while ( ( pos = rx37.indexIn ( buff, 0 ) ) != -1 ) {
        QByteArray ldetalle = trataSetPrintDirection( rx37.cap ( 1 ), tipoEscape );
        buff.replace ( pos, rx37.matchedLength(), ldetalle );
        pos = buff.indexOf("<!--");
    } // end while
    buff = cadant + buff;
    
    /// Cambio del setJustification
    pos = buff.indexOf("<!-- SETVABSOLUTEPOS");
    cadant = buff.left(pos);
    buff = buff.right(buff.length()-pos);
    QRegExp rx38 ( "<!--\\s*SETVABSOLUTEPOS\\s*\"([^\"]*)\"\\s*-->" );
    rx38.setMinimal ( TRUE );
    while ( ( pos = rx38.indexIn ( buff, 0 ) ) != -1 ) {
        QByteArray ldetalle = trataSetVAbsolutePos( rx38.cap ( 1 ), tipoEscape );
        buff.replace ( pos, rx38.matchedLength(), ldetalle );
        pos = buff.indexOf("<!--");
    } // end while
    buff = cadant + buff;
    
    /// Cambio del setJustification
    pos = buff.indexOf("<!-- SETVRELATIVEPOS");
    cadant = buff.left(pos);
    buff = buff.right(buff.length()-pos);
    QRegExp rx41 ( "<!--\\s*SETVRELATIVEPOS\\s*\"([^\"]*)\"\\s*-->" );
    rx41.setMinimal ( TRUE );
    while ( ( pos = rx41.indexIn ( buff, pos ) ) != -1 ) {
        QByteArray ldetalle = trataSetVRelativePos( rx41.cap ( 1 ), tipoEscape );
        buff.replace ( pos, rx41.matchedLength(), ldetalle );
        pos = buff.indexOf("<!--");
    } // end while
    buff = cadant + buff;
    
    /// Cambio del cutPaper
    pos = buff.indexOf("<!-- CUTPAPER");
    cadant = buff.left(pos);
    buff = buff.right(buff.length()-pos);
    QRegExp rx45 ( "<!--\\s*CUTPAPER\\s*\"([^\"]*)\"\\s*-->" );
    rx45.setMinimal ( TRUE );
    while ( ( pos = rx45.indexIn ( buff, 0 ) ) != -1 ) {
        QByteArray ldetalle = trataCutPaper( rx45.cap ( 1 ), tipoEscape );
        buff.replace ( pos, rx45.matchedLength(), ldetalle );
        pos = 0;
    } // end while
    buff = cadant + buff;

    /// Cambio del RIGHTJUSTIFIED
    pos = buff.indexOf("<!-- RIGHTJUSTIFIED");
    cadant = buff.left(pos);
    buff = buff.right(buff.length()-pos);
    QRegExp rx46 ( "<!--\\s*RIGHTJUSTIFIED\\s*\"([^\"]*)\"\\s*\"([^\"]*)\"\\s*\"([^\"]*)\"\\s*\"([^\"]*)\"\\s*-->" );
    rx46.setMinimal ( TRUE );
    while ( ( pos = rx46.indexIn ( buff, 0 ) ) != -1 ) {
        QByteArray ldetalle = trataRightJustified( rx46.cap ( 1 ),  rx46.cap ( 2 ), rx46.cap ( 3 ),  rx46.cap ( 4 ),tipoEscape );
        buff.replace ( pos, rx46.matchedLength(), ldetalle );
	buff = cadant + buff;
        pos = buff.indexOf("<!-- RIGHTJUSTIFIED");
	cadant = buff.left(pos);
	buff = buff.right(buff.length()-pos);
    } // end while
    buff = cadant + buff;
    
    /// Cambio del RIGHTJUSTIFIED
    pos = buff.indexOf("<!-- LEFTJUSTIFIED");
    cadant = buff.left(pos);
    buff = buff.right(buff.length()-pos);
    QRegExp rx47 ( "<!--\\s*LEFTJUSTIFIED\\s*\"([^\"]*)\"\\s*\"([^\"]*)\"\\s*\"([^\"]*)\"\\s*\"([^\"]*)\"\\s*-->" );
    rx47.setMinimal ( TRUE );
    while ( ( pos = rx47.indexIn ( buff, 0 ) ) != -1 ) {
        QByteArray ldetalle = trataLeftJustified( rx47.cap ( 1 ),  rx47.cap ( 2 ), rx47.cap ( 3 ),  rx47.cap ( 4 ),tipoEscape );
        buff.replace ( pos, rx47.matchedLength(), ldetalle );
	buff = cadant + buff;
        pos = buff.indexOf("<!-- LEFTJUSTIFIED");
	cadant = buff.left(pos);
	buff = buff.right(buff.length()-pos);
    } // end while
    buff = cadant + buff;
    
    /// Cambio del cutPaperAndFeed
    pos = buff.indexOf("<!-- CUTPAPERANDFEED");
    cadant = buff.left(pos);
    buff = buff.right(buff.length()-pos);
    QRegExp rx44 ( "<!--\\s*CUTPAPERANDFEED\\s*\"([^\"]*)\"\\s*\"([^\"]*)\"\\s*-->" );
    rx44.setMinimal ( TRUE );
    while ( ( pos = rx44.indexIn ( buff, 0 ) ) != -1 ) {
        QByteArray ldetalle = trataCutPaperAndFeed( rx44.cap ( 1 ),  rx44.cap ( 2 ),tipoEscape );
        buff.replace ( pos, rx44.matchedLength(), ldetalle );
        pos = buff.indexOf("<!--");
    } // end while
    buff = cadant + buff;

    /// Inclusion de imagenes
    pos = buff.indexOf("<!-- IMG");
    cadant = buff.left(pos);
    buff = buff.right(buff.length()-pos);
    QRegExp rx29 ( "<!--\\s*IMG\\s*SRC\\s*=\\s*\"([^\"]*)\"\\s*-->" );
    rx29.setMinimal ( TRUE );
    while ( ( pos = rx29.indexIn ( buff, 0 ) ) != -1 ) {
        QByteArray ldetalle = trataIncludeImg ( rx29.cap ( 1 ), tipoEscape );
        buff.replace ( pos, rx29.matchedLength(), ldetalle );
	buff = cadant + buff;
        pos = buff.indexOf("<!-- IMG");
	cadant = buff.left(pos);
	buff = buff.right(buff.length()-pos);
    } // end while
    buff = cadant + buff;


    blDebug ( "END BtTicket::trataTags", 0 );
    return 1;
}


/// Trata las lineas de detalle encontradas dentro de los tags <!--LINEAS DETALLE-->
/**
\param det Texto de entrada para ser tratado por iteracion.
\return Si el query tiene elementos lo devuelve el parametro. En caso contrario no devuelve nada.
**/
QByteArray BtTicket::trataIfQuery ( const QString &query, const QByteArray &datos )
{
    blDebug ( "BtTicket::trataIfQuery", 0 );
    QByteArray result = "";
    QByteArray query1 = query.toAscii();

    /// Buscamos parametros en el query y los ponemos.
    substrVars ( query1 );

    /// Cargamos el query y lo recorremos
    BlDbRecordSet *cur = mainCompany() ->loadQuery ( QString(query1) );
    if ( !cur ) return "";
    if ( !cur->eof() ) {
        result = datos;
    } // end while
    delete cur;
    blDebug ( "END BtTicket::trataIfQuery", 0 );
    return result;
}


/// Trata las lineas de detalle encontradas dentro de los tags <!--LINEAS DETALLE-->
/**
\param det Texto de entrada para ser tratado por iteracion.
\return Si el query tiene elementos lo devuelve el parametro. En caso contrario no devuelve nada.
**/
QByteArray BtTicket::trataIf ( const QString &query, const QByteArray &datos, const QByteArray &datos1 )
{
    blDebug ( "BtTicket::trataIfQuery", 0 );
    QByteArray result = "";
    QByteArray query1 = query.toAscii();

    /// Buscamos parametros en el query y los ponemos.
    substrVars ( query1 );

    QString query2 = "SELECT (" + QString(query1) + ") AS res";
    /// Cargamos el query y lo recorremos
    BlDbRecordSet *cur = mainCompany() ->loadQuery ( query2 );
    if ( !cur ) return "";
    if ( !cur->eof() ) {
        if ( cur->valor ( "res" ) == "t" ) {
            result = datos;
        } else {
            result = datos1;
        } // end if
    } // end while
    delete cur;
    blDebug ( "END BtTicket::trataIf", 0 );
    return result;
}


/// Trata las lineas de detalle encontradas dentro de los tags <!--LINEAS DETALLE-->
/**
\param det Texto de entrada para ser tratado por iteracion.
\return
**/
QByteArray BtTicket::trataIncludeFile ( const QString &file, int tipoEscape )
{
    blDebug ( "BtTicket::trataIncludeFile", 0 );
    QByteArray read = "";
    QFile arch ( file );
    if ( arch.open ( QIODevice::ReadOnly ) ) {
       read = arch.readAll();
        arch.close();
    } // end if
    /// Buscamos parametros en el query y los ponemos.
    substrVars ( read, tipoEscape );


    blDebug ( "END BtTicket::trataIncludeFile", 0 );
    return read;

}


/// Trata las inclusiones de imagenes
/**
\param det Texto de entrada para ser tratado por iteracion.
\return
**/
QByteArray BtTicket::trataIncludeImg ( const QString &file, int tipoEscape )
{
    blDebug ( "BtTicket::trataIncludeImg", 0 );
    BtEscPrinter pr;
    pr.clearBuffer();
    pr.printImage(file);
    blDebug ( "END BtTicket::trataIncludeImg", 0 );
    return pr.buffer();

}

/// Trata el setcharacterprintermode
/**
\param det Texto de entrada para ser tratado por iteracion.
\return
**/
QByteArray BtTicket::trataSetCharacterPrintMode ( const QString &param, int tipoEscape )
{

    int modo=0;
    blDebug ( "BtTicket::trataSetCharacterPrintMode", 0 );
    BtEscPrinter pr;
    pr.clearBuffer();
    if (param.contains("CHARACTER_FONTA_SELECTED"))
        modo |= CHARACTER_FONTA_SELECTED;
    if (param.contains("CHARACTER_FONTB_SELECTED"))
        modo |= CHARACTER_FONTB_SELECTED;
    if (param.contains("EMPHASIZED_MODE"))
        modo |= EMPHASIZED_MODE;
    if (param.contains("DOUBLE_HEIGHT"))
        modo |= DOUBLE_HEIGHT;
    if (param.contains("DOUBLE_WIDTH"))
        modo |= DOUBLE_WIDTH;
    if (param.contains("UNDERLINE_MODE"))
        modo |= UNDERLINE_MODE;

    pr.setCharacterPrintMode(modo);
    blDebug ( "END BtTicket::trataSetCharacterPrintMode", 0 );
    return pr.buffer();

}

/// Trata el setcharacterprintermode
/**
\param det Texto de entrada para ser tratado por iteracion.
\return
**/
QByteArray BtTicket::trataSetCharacterSpacing ( const QString &param, int tipoEscape )
{

    int modo=0;
    blDebug ( "BtTicket::trataSetCharacterSpacing", 0 );
    BtEscPrinter pr;
    pr.clearBuffer();

    pr.setCharacterSpacing(param.toInt());
    blDebug ( "END BtTicket::trataSetCharacterSpacing", 0 );
    return pr.buffer();

}



/// Trata el setcharacterprintermode
/**
\param det Texto de entrada para ser tratado por iteracion.
\return
**/
QByteArray BtTicket::trataSetCharacterCodeTable ( const QString &param, int tipoEscape )
{
    blDebug ( "BtTicket::trataSetCharacterCodeTable", 0 );
    characterCodeTable codetable=page0;
    BtEscPrinter pr;
    pr.clearBuffer();
    if (param.contains("page0"))
        codetable = page0;
    if (param.contains("page1"))
        codetable = page1;
    if (param.contains("page2"))
        codetable = page2;
    if (param.contains("page3"))
        codetable = page3;
    if (param.contains("page4"))
        codetable = page4;
    if (param.contains("page5"))
        codetable = page5;

    pr.setCharacterCodeTable(codetable);
    blDebug ( "END BtTicket::trataSetCharacterCodeTable", 0 );
    return pr.buffer();

}

/// Trata el setcharacterprintermode
/**
\param det Texto de entrada para ser tratado por iteracion.
\return
**/
QByteArray BtTicket::trataSetUnderlineMode ( const QString &param, int tipoEscape )
{

    bool modo=FALSE;
    blDebug ( "BtTicket::trataSetUnderlineMode", 0 );
    BtEscPrinter pr;
    pr.clearBuffer();
    if (param.contains("TRUE"))
        modo = TRUE;
    if (param.contains("true"))
        modo = TRUE;
    if (param.contains("1"))
        modo = TRUE;

    pr.setUnderlineMode(modo);
    blDebug ( "END BtTicket::trataSetUnderlineMode", 0 );
    return pr.buffer();

}

/// Trata el SetCharacterSize
/**
\param det Texto de entrada para ser tratado por iteracion.
\return
**/
QByteArray BtTicket::trataSetCharacterSize ( const QString &param, int tipoEscape )
{

    int modo=0;
    blDebug ( "BtTicket::trataSetCharacterSpacing", 0 );
    BtEscPrinter pr;
    pr.clearBuffer();

    pr.setCharacterSize(param.toInt());
    blDebug ( "END BtTicket::trataSetCharacterSpacing", 0 );
    return pr.buffer();

}

/// Trata el setsmoothing
/**
\param det Texto de entrada para ser tratado por iteracion.
\return
**/
QByteArray BtTicket::trataSetSmoothing ( const QString &param, int tipoEscape )
{

    bool modo=FALSE;
    blDebug ( "BtTicket::trataSetSmoothing", 0 );
    BtEscPrinter pr;
    pr.clearBuffer();
    if (param.contains("TRUE"))
        modo = TRUE;
    if (param.contains("true"))
        modo = TRUE;
    if (param.contains("1"))
        modo = TRUE;

    pr.setSmoothing(modo);
    blDebug ( "END BtTicket::trataSetSmoothing", 0 );
    return pr.buffer();

}

/// Trata el trataSetDoubleStrike
/**
\param det Texto de entrada para ser tratado por iteracion.
\return
**/
QByteArray BtTicket::trataSetDoubleStrike ( const QString &param, int tipoEscape )
{

    bool modo=FALSE;
    blDebug ( "BtTicket::trataSetDoubleStrike", 0 );
    BtEscPrinter pr;
    pr.clearBuffer();
    if (param.contains("TRUE"))
        modo = TRUE;
    if (param.contains("true"))
        modo = TRUE;
    if (param.contains("1"))
        modo = TRUE;

    pr.setDoubleStrike(modo);
    blDebug ( "END BtTicket::trataSetDoubleStrike", 0 );
    return pr.buffer();

}

/// Trata el trataTurnUpsideDown
/**
\param det Texto de entrada para ser tratado por iteracion.
\return
**/
QByteArray BtTicket::trataTurnUpsideDown ( const QString &param, int tipoEscape )
{

    bool modo=FALSE;
    blDebug ( "BtTicket::trataTurnUpsideDown", 0 );
    BtEscPrinter pr;
    pr.clearBuffer();
    if (param.contains("TRUE"))
        modo = TRUE;
    if (param.contains("true"))
        modo = TRUE;
    if (param.contains("1"))
        modo = TRUE;

    pr.turnUpsideDown(modo);
    blDebug ( "END BtTicket::trataTurnUpsideDown", 0 );
    return pr.buffer();

}


/// Trata el trataTurn90CWRotation
/**
\param det Texto de entrada para ser tratado por iteracion.
\return
**/
QByteArray BtTicket::trataTurn90CWRotation ( const QString &param, const QString &param1, int tipoEscape )
{

    bool modo=FALSE;
    bool extra = FALSE;
    blDebug ( "BtTicket::trataTurn90CWRotation", 0 );
    BtEscPrinter pr;
    pr.clearBuffer();
    if (param.contains("TRUE"))
        modo = TRUE;
    if (param.contains("true"))
        modo = TRUE;
    if (param.contains("1"))
        modo = TRUE;

    if (param1.contains("TRUE") || param1.contains("t") || param1.contains("1"))
        extra = TRUE;

    pr.turn90CWRotation(modo,extra);
    blDebug ( "END BtTicket::trataTurn90CWRotation", 0 );
    return pr.buffer();

}


/// Trata el trataTurnWhiteBlack
/**
\param det Texto de entrada para ser tratado por iteracion.
\return
**/
QByteArray BtTicket::trataTurnWhiteBlack ( const QString &param, int tipoEscape )
{

    bool modo=FALSE;
    blDebug ( "BtTicket::trataTurnWhiteBlack", 0 );
    BtEscPrinter pr;
    pr.clearBuffer();
    if (param.contains("TRUE"))
        modo = TRUE;
    if (param.contains("true"))
        modo = TRUE;
    if (param.contains("1"))
        modo = TRUE;

    pr.turnWhiteBlack(modo);
    blDebug ( "END BtTicket::trataTurnWhiteBlack", 0 );
    return pr.buffer();

}

/// Trata el trataSetColor
/**
\param det Texto de entrada para ser tratado por iteracion.
\return
**/
QByteArray BtTicket::trataSetColor ( const QString &param, int tipoEscape )
{
    blDebug ( "BtTicket::trataSetColor", 0 );
    printColor color=black;
    BtEscPrinter pr;
    pr.clearBuffer();
    if (param.contains("BLACK"))
        color = black;
    if (param.contains("RED"))
        color = red;

    pr.setColor(color);
    blDebug ( "END BtTicket::trataSetColor", 0 );
    return pr.buffer();

}

/// Trata el trataSetColor
/**
\param det Texto de entrada para ser tratado por iteracion.
\return
**/
QByteArray BtTicket::trataHorizontalTab ( int tipoEscape )
{
    blDebug ( "BtTicket::trataHorizontalTab", 0 );
    BtEscPrinter pr;
    pr.clearBuffer();
    pr.horizontalTab();
    blDebug ( "END BtTicket::trataHorizontalTab", 0 );
    return pr.buffer();

}

/// Trata el trataTurn90CWRotation
/**
\param det Texto de entrada para ser tratado por iteracion.
\return
**/
QByteArray BtTicket::trataSetHorizontalTabPos ( const QString &param, const QString &param1, int tipoEscape )
{

    blDebug ( "BtTicket::trataSetHorizontalTabPos", 0 );
    BtEscPrinter pr;
    pr.clearBuffer();

    pr.setHorizontalTabPos(param.toInt(),param1.toLatin1().data());
    blDebug ( "END BtTicket::trataSetHorizontalTabPos", 0 );
    return pr.buffer();

}


/// Trata el trataSetLeftMargin
/**
\param det Texto de entrada para ser tratado por iteracion.
\return
**/
QByteArray BtTicket::trataSetLeftMargin ( const QString &param, int tipoEscape )
{

    int modo=0;
    blDebug ( "BtTicket::trataSetLeftMargin", 0 );
    BtEscPrinter pr;
    pr.clearBuffer();

    pr.setLeftMargin(param.toInt());
    blDebug ( "END BtTicket::trataSetLeftMargin", 0 );
    return pr.buffer();

}

/// Trata el trataSetPrintingAreaWidth
/**
\param det Texto de entrada para ser tratado por iteracion.
\return
**/
QByteArray BtTicket::trataSetPrintingAreaWidth ( const QString &param, int tipoEscape )
{

    int modo=0;
    blDebug ( "BtTicket::trataSetPrintingAreaWidth", 0 );
    BtEscPrinter pr;
    pr.clearBuffer();

    pr.setPrintingAreaWidth(param.toInt());
    blDebug ( "END BtTicket::trataSetPrintingAreaWidth", 0 );
    return pr.buffer();

}


/// Trata el trataSetJustification
/**
\param det Texto de entrada para ser tratado por iteracion.
\return
**/
QByteArray BtTicket::trataSetJustification ( const QString &param, int tipoEscape )
{

    BtEscPrinter::justification modo = BtEscPrinter::left ;
    blDebug ( "BtTicket::trataSetJustification", 0 );
    BtEscPrinter pr;
    pr.clearBuffer();
    if (param.contains("LEFT"))
        modo = BtEscPrinter::left;
    if (param.contains("CENTER"))
        modo = BtEscPrinter::center;
    if (param.contains("RIGHT"))
        modo = BtEscPrinter::right;

    pr.setJustification(modo);
    blDebug ( "END BtTicket::trataSetJustification", 0 );
    return pr.buffer();

}

/// Trata el trataSetHAbsolutePos
/**
\param det Texto de entrada para ser tratado por iteracion.
\return
**/
QByteArray BtTicket::trataSetHAbsolutePos ( const QString &param, int tipoEscape )
{

    int modo=0;
    blDebug ( "BtTicket::trataSetHAbsolutePos", 0 );
    BtEscPrinter pr;
    pr.clearBuffer();

    pr.setHAbsolutePos(param.toInt());
    blDebug ( "END BtTicket::trataSetHAbsolutePos", 0 );
    return pr.buffer();

}

/// Trata el trataSetHRelativePos
/**
\param det Texto de entrada para ser tratado por iteracion.
\return
**/
QByteArray BtTicket::trataSetHRelativePos ( const QString &param, int tipoEscape )
{

    int modo=0;
    blDebug ( "BtTicket::trataSetHRelativePos", 0 );
    BtEscPrinter pr;
    pr.clearBuffer();

    pr.setHRelativePos(param.toInt());
    blDebug ( "END BtTicket::trataSetHRelativePos", 0 );
    return pr.buffer();

}


/// Trata el trataSetBarcodeFormat
/**
\param det Texto de entrada para ser tratado por iteracion.
\return
**/
QByteArray BtTicket::trataSetBarcodeFormat ( const QString &param, const QString &param1,const QString &param2,const QString &param3,int tipoEscape )
{

    barCodeTextPos pos = notPrinted;
    printerFont    font = fontA;
    blDebug ( "BtTicket::trataSetHRelativePos", 0 );
    BtEscPrinter pr;
    pr.clearBuffer();

    if (param2.contains("NOTPRINTED"))
        pos = notPrinted;
    if (param2.contains("ABOVE"))
        pos = above;
    if (param2.contains("BELOW"))
        pos = below;
    if (param2.contains("BOTH"))
        pos = both;

    if (param3.contains("FONTB"))
        font = fontB;

    pr.setBarcodeFormat(param.toInt(), param1.toInt(), pos, font);
    blDebug ( "END BtTicket::trataSetHRelativePos", 0 );
    return pr.buffer();

}



/// Trata el trataPrintBarCode
/**
\param det Texto de entrada para ser tratado por iteracion.
\return
**/
QByteArray BtTicket::trataPrintBarCode ( const QString &param, const QString &param1,const QString &param2,int tipoEscape )
{

    barcodeSystem system = upca;
    blDebug ( "BtTicket::trataPrintBarCode", 0 );
    BtEscPrinter pr;
    pr.clearBuffer();

    if (param.contains("UPCA"))
        system = upca;
    if (param.contains("UPCE"))
        system = upce;
    if (param.contains("JAN13"))
        system = jan13;
    if (param.contains("JAN8"))
        system = jan8;
    if (param.contains("CODE39"))
        system = code39;
    if (param.contains("ITF"))
        system = itf;
    if (param.contains("CODABAR"))
        system = codabar;
    if (param.contains("CODE93"))
        system = code93;
    if (param.contains("CODE128"))
        system = code128;


    pr.printBarCode(system, param1.toInt(), param2.toAscii().data());
    blDebug ( "END BtTicket::trataPrintBarCode", 0 );
    return pr.buffer();

}


/// Trata el trataPrintBarCode
/**
\param det Texto de entrada para ser tratado por iteracion.
\return
**/
QByteArray BtTicket::trataRightJustified ( const QString &param, const QString &param1,const QString &param2,const QString &param3,int tipoEscape )
{

    blDebug ( "BtTicket::trataRightJustified", 0 );
    bool truncate = FALSE;
    if (param3.contains("TRUE"))
        truncate = TRUE;
    if (param3.contains("true"))
        truncate = TRUE;
    if (param3.contains("1"))
        truncate = TRUE;

    blDebug ( "END BtTicket::trataRightJustified", 0 );
    return param.rightJustified(param1.toInt(), param2.at(0), truncate).toAscii();
}

/// Trata el trataLeftJustified
/**
\param det Texto de entrada para ser tratado por iteracion.
\return
**/
QByteArray BtTicket::trataLeftJustified ( const QString &param, const QString &param1,const QString &param2,const QString &param3,int tipoEscape )
{

    blDebug ( "BtTicket::trataLeftJustified", 0 );
    bool truncate = FALSE;
    if (param3.contains("TRUE"))
        truncate = TRUE;
    if (param3.contains("true"))
        truncate = TRUE;
    if (param3.contains("1"))
        truncate = TRUE;

    blDebug ( "END BtTicket::trataLeftJustified", 0 );
    return param.leftJustified(param1.toInt(), param2.at(0), truncate).toAscii();
}


/// Trata el setBarCodeWidth
/**
\param det Texto de entrada para ser tratado por iteracion.
\return
**/
QByteArray BtTicket::trataSetBarCodeWidth ( const QString &param, int tipoEscape )
{

    int modo=0;
    blDebug ( "BtTicket::trataSetBarCodeWidth", 0 );
    BtEscPrinter pr;
    pr.clearBuffer();

    pr.setBarCodeWidth(param.toInt());
    blDebug ( "END BtTicket::trataSetBarCodeWidth", 0 );
    return pr.buffer();

}

/// Trata el setBarCodeHeight
/**
\param det Texto de entrada para ser tratado por iteracion.
\return
**/
QByteArray BtTicket::trataSetBarCodeHeight ( const QString &param, int tipoEscape )
{

    int modo=0;
    blDebug ( "BtTicket::trataSetBarCodeHeight", 0 );
    BtEscPrinter pr;
    pr.clearBuffer();

    pr.setBarCodeHeight(param.toInt());
    blDebug ( "END BtTicket::trataSetBarCodeHeight", 0 );
    return pr.buffer();

}

/// Trata el trataSelectPageMode
/**
\param det Texto de entrada para ser tratado por iteracion.
\return
**/
QByteArray BtTicket::trataSelectPageMode ( int tipoEscape )
{

    int modo=0;
    blDebug ( "BtTicket::trataSelectPageMode", 0 );
    BtEscPrinter pr;
    pr.clearBuffer();

    pr.selectPageMode();
    blDebug ( "END BtTicket::trataSelectPageMode", 0 );
    return pr.buffer();

}


/// Trata el trataSetPrintDirection
/**
\param det Texto de entrada para ser tratado por iteracion.
\return
**/
QByteArray BtTicket::trataSetPrintDirection ( const QString &param, int tipoEscape )
{

    printDirection direc = leftToRight;
    blDebug ( "BtTicket::trataSetPrintDirection", 0 );
    BtEscPrinter pr;
    pr.clearBuffer();

    if (param.contains("LEFTTORIGHT"))
        direc = leftToRight;
    if (param.contains("BOTTOMTOTOP"))
        direc = bottomTotop;
    if (param.contains("RIGHTTOLEFT"))
        direc = rightToLeft;
    if (param.contains("TOPTOBOTTOM"))
        direc = topToBottom;


    pr.setPrintDirection( direc);
    blDebug ( "END BtTicket::trataSetPrintDirection", 0 );
    return pr.buffer();

}




/// Trata el trataSetPrintArea
/**
\param det Texto de entrada para ser tratado por iteracion.
\return
**/
QByteArray BtTicket::trataSetPrintArea ( const QString &param,  const QString &param1,  const QString &param2,  const QString &param3, int tipoEscape )
{

    int modo=0;
    blDebug ( "BtTicket::trataSetPrintArea", 0 );
    BtEscPrinter pr;
    pr.clearBuffer();

    pr.setPrintArea(param.toInt(), param1.toInt(), param2.toInt(), param3.toInt());
    blDebug ( "END BtTicket::trataSetPrintArea", 0 );
    return pr.buffer();

}


/// Trata el trataSetVAbsolutePos
/**
\param det Texto de entrada para ser tratado por iteracion.
\return
**/
QByteArray BtTicket::trataSetVAbsolutePos ( const QString &param, int tipoEscape )
{

    int modo=0;
    blDebug ( "BtTicket::trataSetVAbsolutePos", 0 );
    BtEscPrinter pr;
    pr.clearBuffer();

    pr.setVAbsolutePos(param.toInt());
    blDebug ( "END BtTicket::trataSetVAbsolutePos", 0 );
    return pr.buffer();

}

/// Trata el trataSetVRelativePos
/**
\param det Texto de entrada para ser tratado por iteracion.
\return
**/
QByteArray BtTicket::trataSetVRelativePos ( const QString &param, int tipoEscape )
{

    int modo=0;
    blDebug ( "BtTicket::trataSetVRelativePos", 0 );
    BtEscPrinter pr;
    pr.clearBuffer();

    pr.setVRelativePos(param.toInt());
    blDebug ( "END BtTicket::trataSetVRelativePos", 0 );
    return pr.buffer();

}

/// Trata el trataCutPaper
/**
\param det Texto de entrada para ser tratado por iteracion.
\return
**/
QByteArray BtTicket::trataCutPaper ( const QString &param, int tipoEscape )
{

    bool modo=FALSE;
    blDebug ( "BtTicket::trataCutPaper", 0 );
    BtEscPrinter pr;
    pr.clearBuffer();
    if (param.contains("TRUE"))
        modo = TRUE;
    if (param.contains("true"))
        modo = TRUE;
    if (param.contains("1"))
        modo = TRUE;

    pr.cutPaper(modo);
    blDebug ( "END BtTicket::trataCutPaper", 0 );
    return pr.buffer();

}



/// Trata el trataCutPaperAndFeed
/**
\param det Texto de entrada para ser tratado por iteracion.
\return
**/
QByteArray BtTicket::trataCutPaperAndFeed ( const QString &param, const QString &param1, int tipoEscape )
{

    bool modo=FALSE;
    blDebug ( "BtTicket::trataCutPaperAndFeed", 0 );
    BtEscPrinter pr;
    pr.clearBuffer();
    if (param.contains("TRUE"))
        modo = TRUE;
    if (param.contains("true"))
        modo = TRUE;
    if (param.contains("1"))
        modo = TRUE;

    pr.cutPaperAndFeed(modo, param1.toInt());
    blDebug ( "END BtTicket::trataCutPaperAndFeed", 0 );
    return pr.buffer();

}


/// Trata las lineas de detalle encontradas dentro de los tags <!--LINEAS DETALLE-->
/**
\param det Texto de entrada para ser tratado por iteracion.
\return
**/
QByteArray BtTicket::trataQuery ( const QString &query, const QByteArray &datos, int tipoEscape )
{
    blDebug ( "BtTicket::trataQuery", 0 );
    QByteArray result = "";
    QByteArray query1 = query.toAscii();

    /// Buscamos parametros en el query y los ponemos.
    substrVars ( query1, tipoEscape );

    /// Cargamos el query y lo recorremos
    result = trataCursor ( mainCompany() ->loadQuery ( query1 ), datos, tipoEscape );
    blDebug ( "END BtTicket::trataQuery", 0 );
    return result;

}

QByteArray BtTicket::trataCursor ( BlDbRecordSet *cur, const QByteArray &datos, int tipoEscape )
{
    blDebug ( "BtTicket::trataCursor", 0 );
    QByteArray result = "";
    
    if ( !cur ) {
        blDebug ( "END BtTicket::trataCursor", 0 , "cur==NULL" );
        return "";
    };
    while ( !cur->eof() ) {
        QByteArray salidatemp = datos;

        /// Buscamos cadenas perdidas adicionales que puedan quedar por poner.
        //blDebug("salidatemp =",0,salidatemp);
        QRegExp rx ( "\\[(\\w*)\\]" );
        int pos =  0;
        while ( ( pos = rx.indexIn ( salidatemp, pos ) ) != -1 ) {
            //blDebug("substituïm ",0,rx.cap(1));
            if ( cur->numcampo ( rx.cap ( 1 ) ) != -1 ) {
                switch ( tipoEscape ) {
                case 1:
                    salidatemp.replace ( pos, rx.matchedLength(), blXMLEscape ( cur->valor ( rx.cap ( 1 ), -1, TRUE ) ).toAscii()  );
                    break;
                case 2:
                    salidatemp.replace ( pos, rx.matchedLength(), blPythonEscape ( cur->valor ( rx.cap ( 1 ), -1, TRUE ) ).toAscii()  );
                    break;
                default:
                    salidatemp.replace ( pos, rx.matchedLength(), cur->valor ( rx.cap ( 1 ), -1, TRUE ).toAscii() );
                    break;
                } // emd switch
                pos = 0;
            } else {
                pos += rx.matchedLength();
            }
        } // end while

        result += salidatemp;
        cur->nextRecord();
    } // end while
    delete cur;
    blDebug ( "END BtTicket::trataCursor", 0 );
    return result;
}


/// Trata las lineas de detalle encontradas dentro de los tags <!--LINEAS DETALLE-->
/**
\param det Texto de entrada para ser tratado por iteracion.
\return Si el query tiene elementos lo devuelve el parametro. En caso contrario no devuelve nada.
**/
QByteArray BtTicket::trataExists ( const QString &query, const QByteArray &datos )
{
    blDebug ( "BtTicket::trataExists", 0 );

    QByteArray result = "";
    QByteArray query1 = query.toAscii();

    /// Buscamos parametros en el query y los ponemos.
    substrVars ( query1 );

    QFile file ( query1 );
    if ( file.exists() )
        result = datos;
    blDebug ( "END BtTicket::trataExists", 0 );

    return result;
}


int BtTicket::generaRML ( void )
{
    blDebug ( "BtTicket::generaRML", 0 );
    int err = BlDbRecord::generaRML();
    blDebug ( "END BtTicket::generaRML", 0 );
    return err;
}

///
/**
**/
int BtTicket::generaRML ( const QString &arch )
{
    blDebug ( "BtTicket::generaRML", 0, arch );

    /// Vaciamos las variables de RML
    m_variables.clear();

    /// Ponemos en m_variables CONF_DBNAME y CONF_DBUSER
    m_variables["CONF_DBNAME"] = mainCompany()->dbName();
    m_variables["CONF_DBUSER"] = mainCompany()->currentUser();

    /// Disparamos los plugins
    int res = g_plugins->lanza ( "BtTicket_generaRML", this );
    if ( res != 0 ) {
        return 1;
    } // end if

    res = BlDbRecord::generaRML ( arch );

    blDebug ( "END BtTicket::generaRML", 0 );
    return res;
}





