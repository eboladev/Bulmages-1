/***************************************************************************
 *   Copyright (C) 2007 by Tomeu Borras Riera                              *
 *   tborras@conetxia.com                                                  *
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

#include "blmaincompany.h"
#include "ticket.h"
#include "plugins.h"
#include "escprinter.h"


/// Una factura puede tener multiples bases imponibles. Por eso definimos el tipo base
/// como un QMap.
typedef QMap<QString, BlFixed> base;

Ticket::Ticket ( BlMainCompany *emp, QWidget *parent ) : BlWidget ( emp, parent ), BlDbRecord ( emp )
{
    _depura ( "Ticket::Ticket", 0 );
    /// Inicializamos los parametros del ticket para la base de datos.
    setDBTableName ( "albaran" );
    setDBCampoId ( "idalbaran" );
    addDBCampo ( "descalbaran", BlDbField::DBvarchar, BlDbField::DBNothing, _( "Descripcion" ) );
    addDBCampo ( "nomticket", BlDbField::DBvarchar, BlDbField::DBNoSave, _( "Nombre" ) );
    addDBCampo ( "idalbaran", BlDbField::DBint, BlDbField::DBPrimaryKey, _( "Id albaran" ) );
    addDBCampo ( "fechaalbaran", BlDbField::DBdate, BlDbField::DBNothing, _( "Id fechaalbaran" ) );
    addDBCampo ( "horaalbaran", BlDbField::DBvarchar, BlDbField::DBNoSave, _( "Hora" ) );
    addDBCampo ( "idtrabajador", BlDbField::DBint, BlDbField::DBNotNull, _( "Trabajador" ) );
    addDBCampo ( "idcliente", BlDbField::DBint, BlDbField::DBNotNull, _( "Cliente" ) );
    addDBCampo ( "idalmacen", BlDbField::DBint, BlDbField::DBNotNull, _( "Almacen" ) );
    addDBCampo ( "numalbaran", BlDbField::DBint, BlDbField::DBNothing, _( "Num" ) );
    addDBCampo ( "refalbaran", BlDbField::DBvarchar, BlDbField::DBNothing, _( "Referencia" ) );
    addDBCampo ( "ticketalbaran", BlDbField::DBboolean, BlDbField::DBNothing, _( "Ticket" ) );
    addDBCampo ( "idforma_pago", BlDbField::DBint, BlDbField::DBNothing, _( "Id forma de pago" ) );

    setDBvalue ( "ticketalbaran", "TRUE" );
    setDBvalue ( "idalmacen", confpr->valor ( CONF_IDALMACEN_DEFECTO ) );
    setDBvalue ( "idcliente", confpr->valor ( CONF_IDCLIENTE_DEFECTO ) );
    setDBvalue ( "idtrabajador", confpr->valor ( CONF_IDTRABAJADOR_DEFECTO ) );
    setDBvalue ( "descalbaran", "Ticket de venta" );
    setDBvalue ( "idforma_pago", confpr->valor ( CONF_IDFORMA_PAGO_CONTADO ) );

    m_lineaActual = NULL;
    m_listaLineas = new QList<BlDbRecord *>;

    g_plugins->lanza ( "Ticket_Ticket_Post", this );

    _depura ( "END Ticket::Ticket", 0 );
}

Ticket::~Ticket()
{
    _depura ( "Ticket::~Ticket", 0 );
    _depura ( "END Ticket::~Ticket", 0 );
}

BlDbRecord * Ticket::agregarLinea()
{
    _depura ( "Ticket::agregarLinea", 0 );

    /// Creamos un nuevo BlDbRecord y lo inicializamos.
    BlDbRecord * item = new BlDbRecord ( empresaBase() );
    item->setDBTableName ( "lalbaran" );
    item->setDBCampoId ( "numlalbaran" );
    item->addDBCampo ( "idalbaran", BlDbField::DBint, BlDbField::DBNotNull, _( "Id Albaran" ) );
    item->addDBCampo ( "numlalbaran", BlDbField::DBint, BlDbField::DBPrimaryKey, _( "Id lalbaran" ) );
    item->addDBCampo ( "cantlalbaran", BlDbField::DBnumeric, BlDbField::DBNotNull, _( "Cantidad" ) );
    item->addDBCampo ( "pvplalbaran", BlDbField::DBnumeric, BlDbField::DBNotNull, _( "Precio" ) );
    item->addDBCampo ( "ivalalbaran", BlDbField::DBnumeric, BlDbField::DBNotNull, _( "IVA" ) );
    item->addDBCampo ( "descuentolalbaran", BlDbField::DBnumeric, BlDbField::DBNothing, _( "Descuento" ) );
    item->addDBCampo ( "ordenlalbaran", BlDbField::DBnumeric, BlDbField::DBNotNull, _( "Orden" ) );
    item->addDBCampo ( "reqeqlalbaran", BlDbField::DBnumeric, BlDbField::DBNothing, _( "Req. eq." ) );
//    item->addDBCampo ( "lotelalbaran", BlDbField::DBnumeric, BlDbField::DBNotNull, _( "Lote" ) );
    item->addDBCampo ( "idarticulo", BlDbField::DBint, BlDbField::DBNotNull, _( "Id articulo" ) );
    item->addDBCampo ( "codigocompletoarticulo", BlDbField::DBvarchar, BlDbField::DBNoSave, _( "Codigo articulo" ) );
    item->addDBCampo ( "nomarticulo", BlDbField::DBvarchar, BlDbField::DBNoSave, _( "Nombre articulo" ) );
    item->addDBCampo ( "desclalbaran", BlDbField::DBvarchar, BlDbField::DBNothing, _( "Nombre articulo" ) );


    item->setDBvalue ( "descuentolalbaran", "0" );
    /// Agregamos el BlDbRecord a la lista de lineas de ticket.
    m_listaLineas->append ( item );


//    g_plugins->lanza("Ticket_agregarLinea_Post", this);
    g_plugins->lanza ( "Ticket_agregarLinea_Post", this, ( void ** ) & item );

    _depura ( "END Ticket::agregarLinea", 0 );
    return item;
}

void Ticket::pintar()
{
    _depura ( "Ticket::pintar", 0, "Metodo para reimplementar en clases derivadas" );
    /// Disparamos los plugins.
    int res = g_plugins->lanza ( "Ticket_pintar", this );
    if ( res != 0 ) {
        return;
    } // end if
    _depura ( "END Ticket::pintar", 0 );
}

QList<BlDbRecord *> *Ticket::listaLineas()
{
    return m_listaLineas;
}


BlDbRecord *Ticket::insertarArticulo ( QString idArticulo, BlFixed cantidad, bool nuevaLinea )
{
    _depura ( "Ticket::insertarArticulo", 0 );
    /// Buscamos si ya hay una linea con el articulo que buscamos
    m_lineaActual = NULL;
    BlDbRecord *item;

    for ( int i = 0; i < listaLineas() ->size(); ++i ) {
        item = listaLineas() ->at ( i );
        if ( item->DBvalue ( "idarticulo" ) == idArticulo )
            m_lineaActual = item;
    }// end for

    if ( m_lineaActual && nuevaLinea == FALSE ) {
        /// Ya hay una linea con este articulo (es un agregado)
        BlFixed cantidadib ( m_lineaActual->DBvalue ( "cantlalbaran" ) );
        BlFixed cant1 = cantidadib + cantidad;
        m_lineaActual->setDBvalue ( "cantlalbaran", cant1.toQString() );
    } else {
        /// No hay ningun item con este articulo (es una insercion)
        m_lineaActual = agregarLinea();
        m_lineaActual->setDBvalue ( "idarticulo", idArticulo );
        m_lineaActual->setDBvalue ( "cantlalbaran", cantidad.toQString() );

        /// Buscamos los parametros en la base de datos.
        QString query = "SELECT * FROM articulo WHERE idarticulo = " + idArticulo;
        BlDbRecordSet *cur = empresaBase() ->cargacursor ( query );
        if ( !cur->eof() ) {
            m_lineaActual->setDBvalue ( "pvplalbaran", cur->valor ( "pvparticulo" ) );
            m_lineaActual->setDBvalue ( "codigocompletoarticulo", cur->valor ( "codigocompletoarticulo" ) );
            m_lineaActual->setDBvalue ( "nomarticulo", cur->valor ( "nomarticulo" ) );
            m_lineaActual->setDBvalue ( "desclalbaran", cur->valor ( "nomarticulo" ) );

            if ( cur->valor ( "idtipo_iva" ) != "" ) {
                QString query2 = "SELECT * FROM tasa_iva WHERE idtipo_iva = " + cur->valor ( "idtipo_iva" ) + " ORDER BY fechatasa_iva LIMIT 1";
                BlDbRecordSet *cur1 = empresaBase() ->cargacursor ( query2 );
                if ( !cur1->eof() )
                    m_lineaActual->setDBvalue ( "ivalalbaran", cur1->valor ( "porcentasa_iva" ) );
                delete cur1;
            } // end if
        } // end if
        delete cur;
    } // end if


    /// Disparamos los plugins.
    g_plugins->lanza ( "Ticket_insertarArticulo_Post", this );


    /// Pintamos el ticket ya que se ha modificado.
    pintar();

    _depura ( "END Ticket::insertarArticulo", 0 );
    return m_lineaActual;
}


void  Ticket::borrarArticulo ( BlDbRecord *linea, BlFixed cantidad )
{
    /// Comprueba que haya un articulo seleccionado.
    if ( m_lineaActual == NULL ) {
        return;
    }

    m_lineaActual->borrar();

}


void  Ticket::vaciarTicket()
{}

void  Ticket::subirPosArticulo ( BlDbRecord *linea, int filas )
{}

void  Ticket::bajarPosArticulo ( BlDbRecord *linea, int filas )
{}

void  Ticket::inicioPosTicket ( BlDbRecord * )
{}

void  Ticket::finPosTicket ( BlDbRecord * )
{}


BlDbRecord * Ticket::lineaTicket ( int posicion )
{
    return NULL;
}


BlDbRecord *Ticket::lineaActTicket()
{
    return m_lineaActual;
}

void Ticket::setLineaActual ( BlDbRecord *rec )
{
    m_lineaActual = rec;
}


void  Ticket::setDescuentoGlobal ( BlFixed descuento )
{}

void Ticket::abrircajon()
{

    QString filestr="/dev/null";
    if (confpr->valor ( CONF_CASHBOX_FILE ) != "") {
	filestr = confpr->valor(CONF_CASHBOX_FILE);
    } // end if
    QFile file ( filestr );
    if ( !file.open ( QIODevice::WriteOnly | QIODevice::Unbuffered ) ) {
        _depura ( "Error en la Impresion de ticket", 2 );
    } // end if

    QStringList secuencia = confpr->valor (CONF_CASHBOX_OPEN_CODE).split(",");

    /// El comando de apertura de cajon
    for (int i = 0; i < secuencia.size(); ++i) {
	    QString cad = QChar(secuencia.at(i).toInt());
	    file.write ( cad.toAscii(), 1 );
    } // end for
//	file.write("hola mundo\n");
    file.close();
}


/*
void  Ticket::imprimir()
{
    base basesimp;
    base basesimpreqeq;

    QFile file ( confpr->valor ( CONF_TICKET_PRINTER_FILE ) );
    if ( !file.open ( QIODevice::WriteOnly | QIODevice::Unbuffered ) ) {
        _depura ( "Error en la Impresion de ticket", 2 );
    } // end if
    BlDbRecordSet *cur = empresaBase() ->cargacursor ( "SELECT * FROM configuracion WHERE nombre='NombreEmpresa'" );
    if ( !cur->eof() ) {
        file.write ( cur->valor ( "valor" ).toAscii() );
        file.write ( "\n", 1 );
    } // end if
    delete cur;
    cur = empresaBase() ->cargacursor ( "SELECT * FROM configuracion WHERE nombre='DireccionCompleta'" );
    if ( !cur->eof() ) {
        file.write ( cur->valor ( "valor" ).toAscii() );
        file.write ( "\n", 1 );
    } // end if
    ///file.write ( QString ( "C/LAS POZAS 181, LOCAL 43\n" ).toAscii() );
    delete cur;
    /// file.write ( QString ( "ALIMENTACION ECOLOGICA. HERBOLARIO\n" ).toAscii() );
    cur = empresaBase() ->cargacursor ( "SELECT * FROM configuracion WHERE nombre='CodPostal'" );
    if ( !cur->eof() ) {
        file.write ( cur->valor ( "valor" ).toAscii() );
    } // end if
    delete cur;

    file.write ( QString ( " " ).toAscii() );
    cur = empresaBase() ->cargacursor ( "SELECT * FROM configuracion WHERE nombre='Ciudad'" );
    if ( !cur->eof() ) {
        file.write ( cur->valor ( "valor" ).toAscii() );
        file.write ( QString ( " " ).toAscii() );
    } // end if
    delete cur;


    cur = empresaBase() ->cargacursor ( "SELECT * FROM configuracion WHERE nombre='Provincia'" );
    if ( !cur->eof() ) {
        file.write ( QString ( "(" ).toAscii() );
        file.write ( cur->valor ( "valor" ).toAscii() );
        file.write ( QString ( ")" ).toAscii() );
        file.write ( "\n", 1 );
    } // end if
    delete cur;

    /// Imprimimos el numero de Ticket
    file.write ( QString ( "Ticket: " ).toAscii() );
    file.write ( DBvalue ( "numalbaran" ).toAscii() );
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
    cur = empresaBase() ->cargacursor ( "SELECT * FROM trabajador WHERE idtrabajador=" + DBvalue ( "idtrabajador" ) );
    if ( !cur->eof() ) {
        file.write ( DBvalue ( "idtrabajador" ).toAscii() );
        file.write ( " " );
        file.write ( cur->valor ( "nomtrabajador" ).toAscii() );
        file.write ( "\n", 1 );
    } // end if
    delete cur;

    /// Imprimimos el cliente
    cur = empresaBase() ->cargacursor ( "SELECT * FROM cliente WHERE idcliente=" + DBvalue ( "idcliente" ) );
    if ( !cur->eof() ) {
        file.write ( QString ( "Cliente: " ).toAscii() );
        file.write ( cur->valor ( "cifcliente" ).toAscii() );
        file.write ( " ", 1 );
        file.write ( cur->valor ( "nomcliente" ).toAscii() );
        file.write ( "\n", 1 );
    } // end if
    delete cur;

    /// Imprimimos el almacen
    cur = empresaBase() ->cargacursor ( "SELECT * FROM almacen WHERE idalmacen=" + DBvalue ( "idalmacen" ) );
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

    cur = empresaBase() ->cargacursor ( "SELECT * FROM configuracion WHERE nombre = 'IRPF'" );
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
        BlFixed cant ( linea->DBvalue ( "cantlalbaran" ) );
        BlFixed pvpund ( linea->DBvalue ( "pvplalbaran" ) );
        BlFixed desc1 ( linea->DBvalue ( "descuentolalbaran" ) );
        BlFixed cantpvp = cant * pvpund;
        BlFixed base = cantpvp - cantpvp * desc1 / 100;
        BlFixed iva = ( linea->DBvalue ( "ivalalbaran" ) );
        BlFixed percentiva = ( iva / 100 );
        descuentolinea = descuentolinea + ( cantpvp * desc1 / 100 );
        BlFixed subtotreq = ( base +  linea->DBvalue ( "reqeqlalbaran" ) );
        BlFixed subtotaliva = ( subtotreq * percentiva ) + subtotreq;
        BlFixed totalunidad = pvpund + ( ( pvpund - ( pvpund * desc1 / 100 ) ) * percentiva );
        basesimp[linea->DBvalue ( "ivalalbaran" ) ] = basesimp[linea->DBvalue ( "ivalalbaran" ) ] + base;
        basesimpreqeq[linea->DBvalue ( "reqeqlalbaran" ) ] = basesimpreqeq[linea->DBvalue ( "reqeqlalbaran" ) ] + base;
        /// Hacemos la impresion
        QString str =  linea->DBvalue ( "desclalbaran" ).leftJustified ( 22, ' ', TRUE ) + linea->DBvalue ( "cantlalbaran" ).rightJustified ( 1, ' ' ) + QString ( " " ) + totalunidad.toQString().rightJustified ( 2, ' ' )  + subtotaliva.toQString().rightJustified ( 8, ' ' );
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
    g_plugins->lanza("Ticket_total", &stotal);

    str = "* IVA INCLUIDO *";
    file.write ( str.rightJustified ( 20, ' ' ).toAscii() );
    file.write ( "\n", 1 );

    /// Imprimimos el trabajador

    file.write ( QString ( "LE ATENDIO: " ).toAscii() );
    cur = empresaBase() ->cargacursor ( "SELECT * FROM trabajador WHERE idtrabajador=" + DBvalue ( "idtrabajador" ) );
    if ( !cur->eof() ) {
        file.write ( DBvalue ( "idtrabajador" ).toAscii() );
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
    file.write ( DBvalue ( "idalbaran" ).toAscii() );
    file.write ( " ", 1 );
    file.write ( DBvalue ( "numalbaran" ).toAscii() );
    file.write ( " ", 1 );
    file.write ( DBvalue ( "refalbaran" ).toAscii() );
    file.write ( "\x00", 1 );
    file.write ( "\n", 1 );
    file.write ( QString ( "TELF. " ).toAscii() );
    cur = empresaBase() ->cargacursor ( "SELECT * FROM configuracion WHERE nombre='telefono'" );
    if ( !cur->eof() ) {
        file.write ( cur->valor ( "valor" ).toAscii() );
        file.write ( QString ( " " ).toAscii() );
    } // end if
    delete cur;

    file.write ( "\n", 1 );

    cur = empresaBase() ->cargacursor ( "SELECT * FROM configuracion WHERE nombre='web'" );
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


void Ticket::imprimir()
{

    guardar();

    /// Disparamos los plugins.
    int res = g_plugins->lanza ( "Ticket_imprimir", this );
    if ( res != 0 ) {
        return;
    } // end if
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

    BlDbRecordSet *cur = empresaBase() ->cargacursor ( "SELECT * FROM configuracion WHERE nombre='NombreEmpresa'" );
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

    trabajador.id = DBvalue ( "idtrabajador" );
    cur = empresaBase() ->cargacursor ( "SELECT * FROM trabajador WHERE idtrabajador=" + DBvalue ( "idtrabajador" ) );
    if ( !cur->eof() )
        trabajador.nombre = cur->valor ( "nomtrabajador" );
    delete cur;

    cur = empresaBase() ->cargacursor ( "SELECT * FROM cliente WHERE idcliente=" + DBvalue ( "idcliente" ) );
    if ( !cur->eof() ) {
        cliente.cif = cur->valor ( "cifcliente" ).toAscii();
        cliente.nombre = cur->valor ( "nomcliente" ).toAscii();
    } // end if
    delete cur;

    cur = empresaBase() ->cargacursor ( "SELECT * FROM almacen WHERE idalmacen=" + DBvalue ( "idalmacen" ) );
    if ( !cur->eof() )
        almacen.nombre = cur->valor ( "nomalmacen" ).toAscii() ;
    delete cur;

    BlDbRecord *linea;
    if ( listaLineas() ->size() )
        total.iva = BlFixed ( listaLineas()->at ( 0 )->DBvalue ( "ivalalbaran" ) );
    for ( int i = 0; i < listaLineas() ->size(); ++i ) {
        linea = listaLineas() ->at ( i );
        BlFixed cantidad = BlFixed ( linea->DBvalue ( "cantlalbaran" ) );
        total.baseImponible = total.baseImponible + cantidad * BlFixed ( linea->DBvalue ( "pvplalbaran" ) );
    } // end for
    total.totalIva = total.baseImponible + total.baseImponible * total.iva / BlFixed ( "100" );

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

    pr.turnWhiteBlack ( 1 );
    pr.printText ( " Uds. PRODUCTO � � � � � � �P.U. �IMPORTE \n" );

    pr.turnWhiteBlack ( 0 );
    pr.setCharacterPrintMode ( CHARACTER_FONTB_SELECTED );
    pr.setCharacterSize ( CHAR_WIDTH_1 | CHAR_HEIGHT_1 );

    for ( int i = 0; i < listaLineas() ->size(); ++i ) {
        if ( i == listaLineas()->size() - 1 )
            pr.setUnderlineMode ( 1 );
        linea = listaLineas() ->at ( i );
        BlFixed iva = BlFixed ( linea->DBvalue ( "ivalalbaran" ) );
        BlFixed pvp = BlFixed ( linea->DBvalue ( "pvplalbaran" ) );
        pvp = pvp + pvp * iva / BlFixed ( "100" );
        BlFixed pvptotal = BlFixed ( linea->DBvalue ( "cantlalbaran" ) ) * pvp;
        pr.printText ( linea->DBvalue ( "cantlalbaran" ).rightJustified ( 5, ' ', TRUE ) + " �" );
        pr.printText ( linea->DBvalue ( "desclalbaran" ).leftJustified ( 27, ' ', true ) + " " );
        QString pvpstr = pvp.toQString();
        QString pvptotalstr = pvptotal.toQString();
        pr.printText ( QString ( pvpstr + "�" ).rightJustified ( 10, ' ', TRUE ) + " " );
        pr.printText ( QString ( pvptotalstr + "�" ).rightJustified ( 10, ' ', TRUE ) );
        pr.printText ( "\n" );
    }
    pr.setUnderlineMode ( 0 );
    pr.setJustification ( right );
    pr.setCharacterPrintMode ( CHARACTER_FONTA_SELECTED );
    pr.printText ( "Base Imponible: " + total.baseImponible.toQString() + "�\n" );
    pr.printText ( "IVA " + total.iva.toQString() + "%:" + ( total.totalIva - total.baseImponible ).toQString() + "�\n" );
    pr.setCharacterPrintMode ( CHARACTER_FONTA_SELECTED | EMPHASIZED_MODE | DOUBLE_HEIGHT | DOUBLE_WIDTH );
    pr.printText ( "TOTAL: " + total.totalIva.toQString() + "�\n" );
    pr.printText ( "\n\n" );
    pr.setJustification ( left );
    pr.setCharacterPrintMode ( CHARACTER_FONTA_SELECTED );
    pr.printText ( "Le ha atendido " + trabajador.nombre + "\n" );
    pr.printText ( "\n" );

/*
   pr.printText("Plazo maximo para cambio 15 dias, \n");
   pr.printText(" unicamente con ticket de compra. \n");
   pr.printText("\n");
*/

    pr.printText ( "Tel. " + empresa.telefono + "\n" );
    pr.printText ( "\n" );

    pr.setJustification ( center );
    pr.setColor ( red );
    pr.printText ( "*** GRACIAS POR SU VISITA ***\n" );


    QByteArray qba = DBvalue ( "refalbaran" ).toAscii();
    char* barcode = qba.data();
    pr.setJustification ( center );
    pr.setBarcodeFormat ( 2, 50, both, fontB );
    pr.printBarCode ( code39, qba.size(), barcode );
    pr.cutPaperAndFeed ( TRUE, 10 );
    pr.print();
}

// ===========================

void Ticket::subir()
{
    if ( listaLineas()->count() > 0 ) {
        int i = listaLineas() ->indexOf ( lineaActTicket() );
        if ( i > 0 ) i--;
        setLineaActual ( listaLineas() ->at ( i ) );
        pintar();
    } // end if
}

void Ticket::bajar()
{
    if ( listaLineas()->count() > 0 ) {
        int i = listaLineas() ->indexOf ( lineaActTicket() );
        if ( i < listaLineas() ->size() - 1 ) i++;
        setLineaActual ( listaLineas() ->at ( i ) );
        pintar();
    } // end if
}


void Ticket::agregarCantidad ( QString cantidad )
{
    BlFixed cant ( cantidad );
    /// Comprueba la existencia de la linea de ticket.
    if ( m_lineaActual == NULL ) {
        mensajeAviso ( "No existe linea" );
        return;
    } // end if
    BlFixed cantorig ( m_lineaActual->DBvalue ( "cantlalbaran" ) );
    BlFixed suma = cant + cantorig;
    if ( suma == BlFixed ( "0.00" ) ) {
        borrarLinea ( m_lineaActual );
        //listaLineas() ->removeAt ( listaLineas() ->indexOf ( m_lineaActual ));
        //m_lineaActual = listaLineas() ->at ( 0 );
    } else {
        m_lineaActual->setDBvalue ( "cantlalbaran", suma.toQString() );
    } // end if
    pintar();
    /*
        BlFixed cant ( cantidad );
        /// Comprueba la existencia de la linea de ticket.
        if ( m_lineaActual == NULL ) {
            mensajeAviso ( "No existe linea" );
            return;
        } // end if
        BlFixed cantorig ( m_lineaActual->DBvalue ( "cantlalbaran" ) );
        BlFixed suma = cant + cantorig;
        if ( suma == BlFixed ( "0.00" ) ) {
            listaLineas() ->removeAt ( listaLineas() ->indexOf ( m_lineaActual ) );
            m_lineaActual = listaLineas() ->at ( 0 );
        } else {
            m_lineaActual->setDBvalue ( "cantlalbaran", suma.toQString() );
        } // end if
        pintar();
    */
}

void Ticket::ponerCantidad ( QString cantidad )
{
    BlFixed cant ( cantidad );
    /// Comprueba la existencia de la linea de ticket.
    if ( m_lineaActual == NULL ) {
        mensajeAviso ( "No existe linea" );
        return;
    } // end if

    if ( cant == 0 ) {
        borrarLinea ( m_lineaActual );
        //listaLineas() ->removeAt ( listaLineas() ->indexOf ( m_lineaActual ));
        //m_lineaActual = listaLineas() ->at ( 0 );
    } else {
        m_lineaActual->setDBvalue ( "cantlalbaran", cant.toQString() );
    } // end if
    pintar();
    /*
        BlFixed cant ( cantidad );
        /// Comprueba la existencia de la linea de ticket.
        if ( m_lineaActual == NULL ) {
            mensajeAviso ( "No existe linea" );
            return;
        } // end if

        if ( cant == 0 ) {
            listaLineas() ->removeAt ( listaLineas() ->indexOf ( m_lineaActual ) );
            m_lineaActual = listaLineas() ->at ( 0 );
        } else {
            m_lineaActual->setDBvalue ( "cantlalbaran", cant.toQString() );
        } // end if
        pintar();
    */
}

void Ticket::ponerPrecio ( QString precio )
{
    BlFixed valor ( precio );
    /// Comprueba la existencia de la linea de ticket.
    if ( m_lineaActual == NULL ) {
        mensajeAviso ( "No existe linea" );
        return;
    } // end if
    m_lineaActual->setDBvalue ( "pvplalbaran", valor.toQString() );

    g_plugins->lanza ( "Ticket_ponerPrecio_Post", this );

    pintar();
}

void Ticket::insertarArticuloCodigo ( QString codigo )
{
    _depura ( "Ticket::insertarArticuloCodigo", 0 );

    QString query = "SELECT * FROM articulo WHERE codigocompletoarticulo= '" + codigo + "'";
    BlDbRecordSet *cur = empresaBase() ->cargacursor ( query );
    if ( !cur->eof() ) {
        insertarArticulo ( cur->valor ( "idarticulo" ), BlFixed ( "1" ) );
    } // end if
    delete cur;

    g_plugins->lanza ( "Ticket_insertarArticuloCodigo_Post", this );
    _depura ( "END Ticket::insertarArticuloCodigo", 0 );

}


void Ticket::insertarArticuloCodigoNL ( QString codigo )
{
    _depura("Ticket::insertarArticuloCodigoNL",0);
    QString query = "SELECT * FROM articulo WHERE codigocompletoarticulo= '" + codigo + "'";
    BlDbRecordSet *cur = empresaBase() ->cargacursor ( query );
    if ( !cur->eof() ) {
        insertarArticulo ( cur->valor ( "idarticulo" ), BlFixed ( "1" ), TRUE );
    } // end if
    delete cur;
    g_plugins->lanza ( "Ticket_insertarArticuloCodigoNL_Post", this );
    _depura("END Ticket::insertarArticuloCodigoNL",0);
}


int Ticket::cargar ( QString id )
{
    try {
        QString query = "SELECT * FROM albaran WHERE idalbaran = " + id;
        BlDbRecordSet *cur = empresaBase() ->cargacursor ( query );
        if ( cur ) {
            if ( !cur->eof() ) {
                DBload ( cur );
            }
            delete cur;
        } // end if
        cur = empresaBase() ->cargacursor ( "SELECT * FROM lalbaran LEFT JOIN articulo ON lalbaran.idarticulo = articulo.idarticulo WHERE idalbaran = " + id );
        while ( !cur->eof() ) {
            BlDbRecord *l = agregarLinea();
            l->DBload ( cur );
            cur->siguienteregistro();
        } // end while
        delete cur;
    } catch ( ... ) {
        mensajeInfo ( "Error en la carga" );
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
int Ticket::guardar()
{
    _depura ( "Ticket::guardar", 0 );

    try {

	if ( listaLineas() ->count() == 0 ) {
		mensajeAviso ( _( "El ticket esta vacio." ) );
		return -1;
	} // end if

        QString id;
        empresaBase() ->begin();
        DBsave ( id );
        BlDbRecord *item;
        for ( int i = 0; i < listaLineas() ->size(); ++i ) {
            QString id1;
            item = listaLineas() ->at ( i );
            item->setDBvalue ( "idalbaran", id );
            item->setDBvalue ( "ordenlalbaran", QString::number ( i ) );
            item->DBsave ( id1 );
        }// end for
        empresaBase() ->commit();
	setDBvalue("idalbaran", id);
        BlDbRecordSet *cur = empresaBase() ->cargacursor ( "SELECT * FROM albaran WHERE idalbaran = " + id );
	setDBvalue("refalbaran", cur->valor("refalbaran"));
	setDBvalue("numalbaran", cur->valor("numalbaran"));

//        DBload ( cur );
        delete cur;

        _depura ( "END Ticket::guardar", 0 );
        return 0;
    } catch ( ... ) {
        mensajeInfo ( "Error inesperado con la base de datos" );
        empresaBase() ->rollback();
        return -1;
    } // end try
}


void Ticket::borrarLinea ( BlDbRecord *linea )
{
    if ( linea == NULL )
        return;
    int numlinea = listaLineas()->indexOf ( linea );

    if ( linea == m_lineaActual ) {
        listaLineas() ->removeAt ( listaLineas() ->indexOf ( linea ) );
        if ( numlinea > listaLineas()->count() - 1 ) {
            m_lineaActual = listaLineas()->count() > 0 ? listaLineas()->at ( listaLineas()->count() - 1 ) : NULL;
        } else {
            m_lineaActual = listaLineas()->count() > 0 ? listaLineas() ->at ( numlinea ) : NULL;
        }
    } else {
        listaLineas() ->removeAt ( listaLineas() ->indexOf ( linea ) );
    }
}



