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

#include "empresabase.h"
#include "ticket.h"
#include "plugins.h"

/// Una factura puede tener multiples bases imponibles. Por eso definimos el tipo base
/// como un QMap.
typedef QMap<QString, Fixed> base;

Ticket::Ticket ( EmpresaBase *emp, QWidget *parent ) : BLWidget ( emp, parent ), DBRecord ( emp )
{
    _depura ( "Ticket::Ticket", 0 );
    /// Inicializamos los parametros del ticket para la base de datos.
    setDBTableName ( "albaran" );
    setDBCampoId ( "idalbaran" );
    addDBCampo ( "descalbaran", DBCampo::DBvarchar, DBCampo::DBNothing, QApplication::translate ( "Ticket", "descripcion" ) );
    addDBCampo ( "nomticket", DBCampo::DBvarchar, DBCampo::DBNoSave, QApplication::translate ( "Ticket", "nombre" ) );
    addDBCampo ( "idalbaran", DBCampo::DBint, DBCampo::DBPrimaryKey, QApplication::translate ( "Ticket", "Id albaran" ) );
    addDBCampo ( "fechaalbaran", DBCampo::DBdate, DBCampo::DBNothing, QApplication::translate ( "Ticket", "Id fechaalbaran" ) );
    addDBCampo ( "horaalbaran", DBCampo::DBvarchar, DBCampo::DBNoSave, QApplication::translate ( "Ticket", "hora" ) );
    addDBCampo ( "idtrabajador", DBCampo::DBint, DBCampo::DBNotNull, QApplication::translate ( "Ticket", "Trabajador" ) );
    addDBCampo ( "idcliente", DBCampo::DBint, DBCampo::DBNotNull, QApplication::translate ( "Ticket", "Cliente" ) );
    addDBCampo ( "idalmacen", DBCampo::DBint, DBCampo::DBNotNull, QApplication::translate ( "Ticket", "Almacen" ) );
    addDBCampo ( "numalbaran", DBCampo::DBint, DBCampo::DBNothing, QApplication::translate ( "Ticket", "Num" ) );
    addDBCampo ( "refalbaran", DBCampo::DBvarchar, DBCampo::DBNothing, QApplication::translate ( "Ticket", "Referencia" ) );
    addDBCampo ( "ticketalbaran", DBCampo::DBboolean, DBCampo::DBNothing, QApplication::translate ( "Ticket", "Ticket" ) );
    addDBCampo ( "idforma_pago", DBCampo::DBint, DBCampo::DBNothing, QApplication::translate ( "Ticket", "Forma Pago" ) );


    setDBvalue ( "ticketalbaran", "TRUE" );
    setDBvalue ( "idalmacen", confpr->valor ( CONF_IDALMACEN_DEFECTO ) );
    setDBvalue ( "idcliente", confpr->valor ( CONF_IDCLIENTE_DEFECTO ) );
    setDBvalue ( "idtrabajador", confpr->valor ( CONF_IDTRABAJADOR_DEFECTO ) );
    setDBvalue ( "descalbaran", "Ticket de venta" );
    setDBvalue ( "idforma_pago", confpr->valor (CONF_IDFORMA_PAGO_CONTADO) );

    m_lineaActual = NULL;
    m_listaLineas = new QList<DBRecord *>;

    _depura ( "END Ticket::Ticket", 0 );
}

Ticket::~Ticket()
{
    _depura ( "Ticket::~Ticket", 0 );
    _depura ( "END Ticket::~Ticket", 0 );
}

DBRecord * Ticket::agregarLinea()
{
    _depura ( "Ticket::agregarLinea", 0 );

    /// Creamos un nuevo DBRecord y lo inicializamos.
    DBRecord * item = new DBRecord ( empresaBase() );
    item->setDBTableName ( "lalbaran" );
    item->setDBCampoId ( "numlalbaran" );
    item->addDBCampo ( "idalbaran", DBCampo::DBint, DBCampo::DBNotNull, QApplication::translate ( "Ticket", "Id Albaran" ) );
    item->addDBCampo ( "numlalbaran", DBCampo::DBint, DBCampo::DBPrimaryKey, QApplication::translate ( "Ticket", "Id lalbaran" ) );
    item->addDBCampo ( "cantlalbaran", DBCampo::DBnumeric, DBCampo::DBNotNull, QApplication::translate ( "Ticket", "Cantidad" ) );
    item->addDBCampo ( "pvplalbaran", DBCampo::DBnumeric, DBCampo::DBNotNull, QApplication::translate ( "Ticket", "Precio" ) );
    item->addDBCampo ( "ivalalbaran", DBCampo::DBnumeric, DBCampo::DBNotNull, QApplication::translate ( "Ticket", "IVA" ) );
    item->addDBCampo ( "descuentolalbaran", DBCampo::DBnumeric, DBCampo::DBNothing, QApplication::translate ( "Ticket", "Descuento" ) );
    item->addDBCampo ( "ordenlalbaran", DBCampo::DBnumeric, DBCampo::DBNotNull, QApplication::translate ( "Ticket", "Orden" ) );
    item->addDBCampo ( "reqeqlalbaran", DBCampo::DBnumeric, DBCampo::DBNothing, QApplication::translate ( "Ticket", "REQ EQ" ) );
//    item->addDBCampo ( "lotelalbaran", DBCampo::DBnumeric, DBCampo::DBNotNull, QApplication::translate ( "Ticket", "Lote" ) );
    item->addDBCampo ( "idarticulo", DBCampo::DBint, DBCampo::DBNotNull, QApplication::translate ( "Ticket", "Id Articulo" ) );
    item->addDBCampo ( "codigocompletoarticulo", DBCampo::DBvarchar, DBCampo::DBNoSave, QApplication::translate ( "Ticket", "Codigo Articulo" ) );
    item->addDBCampo ( "nomarticulo", DBCampo::DBvarchar, DBCampo::DBNoSave, QApplication::translate ( "Ticket", "Nombre Articulo" ) );
    item->addDBCampo ( "desclalbaran", DBCampo::DBvarchar, DBCampo::DBNothing, QApplication::translate ( "Ticket", "Nombre Articulo" ) );


    item->setDBvalue ( "descuentolalbaran", "0" );
    /// Agregamos el DBRecord a la lista de lineas de ticket.
    m_listaLineas->append ( item );

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

QList<DBRecord *> *Ticket::listaLineas()
{
    return m_listaLineas;
}


DBRecord *Ticket::insertarArticulo ( QString idArticulo, Fixed cantidad, bool nuevaLinea )
{
    _depura ( "Ticket::insertarArticulo", 0 );
    /// Buscamos si ya hay una linea con el articulo que buscamos
    m_lineaActual = NULL;
    DBRecord *item;
    for ( int i = 0; i < listaLineas() ->size(); ++i ) {
        item = listaLineas() ->at ( i );
        if ( item->DBvalue ( "idarticulo" ) == idArticulo )
            m_lineaActual = item;
    }// end for

    if ( m_lineaActual && nuevaLinea == FALSE ) {
        /// Ya hay una linea con este articulo (es un agregado)
        Fixed cantidadib ( m_lineaActual->DBvalue ( "cantlalbaran" ) );
        Fixed cant1 = cantidadib + cantidad;
        m_lineaActual->setDBvalue ( "cantlalbaran", cant1.toQString() );
    } else {
        /// No hay ningun item con este articulo (es una insercion)
        m_lineaActual = agregarLinea();
        m_lineaActual->setDBvalue ( "idarticulo", idArticulo );
        m_lineaActual->setDBvalue ( "cantlalbaran", cantidad.toQString() );

        /// Buscamos los parametros en la base de datos.
        QString query = "SELECT * FROM articulo WHERE idarticulo = " + idArticulo;
        cursor2 *cur = empresaBase() ->cargacursor ( query );
        if ( !cur->eof() ) {
            m_lineaActual->setDBvalue ( "pvplalbaran", cur->valor ( "pvparticulo" ) );
            m_lineaActual->setDBvalue ( "codigocompletoarticulo", cur->valor ( "codigocompletoarticulo" ) );
            m_lineaActual->setDBvalue ( "nomarticulo", cur->valor ( "nomarticulo" ) );
            m_lineaActual->setDBvalue ( "desclalbaran", cur->valor ( "nomarticulo" ) );

            if ( cur->valor ( "idtipo_iva" ) != "" ) {
                QString query2 = "SELECT * FROM tasa_iva WHERE idtipo_iva = " + cur->valor ( "idtipo_iva" ) + " ORDER BY fechatasa_iva LIMIT 1";
                cursor2 *cur1 = empresaBase() ->cargacursor ( query2 );
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


void  Ticket::borrarArticulo ( DBRecord *linea, Fixed cantidad )
{
    /// Comprueba que haya un articulo seleccionado.
    if ( m_lineaActual == NULL ) {
        return;
    }

    m_lineaActual->borrar();

}


void  Ticket::vaciarTicket()
{}

void  Ticket::subirPosArticulo ( DBRecord *linea, int filas )
{}

void  Ticket::bajarPosArticulo ( DBRecord *linea, int filas )
{}

void  Ticket::inicioPosTicket ( DBRecord * )
{}

void  Ticket::finPosTicket ( DBRecord * )
{}


DBRecord * Ticket::lineaTicket ( int posicion )
{
    return NULL;
}


DBRecord *Ticket::lineaActTicket()
{
    return m_lineaActual;
}

void Ticket::setLineaActual ( DBRecord *rec )
{
    m_lineaActual = rec;
}


void  Ticket::setDescuentoGlobal ( Fixed descuento )
{}

void Ticket::abrircajon()
{
    QFile file ( confpr->valor ( CONF_TICKET_PRINTER_FILE ) );
    if ( !file.open ( QIODevice::WriteOnly | QIODevice::Unbuffered ) ) {
        _depura ( "Error en la Impresion de ticket", 2 );
    } // end if

    /// El corte de papel.
    file.write ( "\x1B\x70\x0\x64\x64", 5 );
    file.close();
}

void  Ticket::imprimir()
{
    base basesimp;
    base basesimpreqeq;

    QFile file ( confpr->valor ( CONF_TICKET_PRINTER_FILE ) );
    if ( !file.open ( QIODevice::WriteOnly | QIODevice::Unbuffered ) ) {
        _depura ( "Error en la Impresion de ticket", 2 );
    } // end if
    cursor2 *cur = empresaBase() ->cargacursor ( "SELECT * FROM configuracion WHERE nombre='NombreEmpresa'" );
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
    Fixed irpf ( "0" );

    cur = empresaBase() ->cargacursor ( "SELECT * FROM configuracion WHERE nombre = 'IRPF'" );
    if ( cur ) {
        if ( !cur->eof() ) {
            irpf = Fixed ( cur->valor ( "valor" ) );
        } // end if
        delete cur;
    } // end if

    DBRecord *linea;
    Fixed descuentolinea ( "0.00" );
    for ( int i = 0; i < listaLineas() ->size(); ++i ) {
        linea = listaLineas() ->at ( i );
        Fixed cant ( linea->DBvalue ( "cantlalbaran" ) );
        Fixed pvpund ( linea->DBvalue ( "pvplalbaran" ) );
        Fixed desc1 ( linea->DBvalue ( "descuentolalbaran" ) );
        Fixed cantpvp = cant * pvpund;
        Fixed base = cantpvp - cantpvp * desc1 / 100;
        Fixed iva = ( linea->DBvalue ( "ivalalbaran" ) );
        Fixed percentiva = ( iva / 100 );
        descuentolinea = descuentolinea + ( cantpvp * desc1 / 100 );
        Fixed subtotreq = ( base +  linea->DBvalue ( "reqeqlalbaran" ) );
        Fixed subtotaliva = ( subtotreq * percentiva ) + subtotreq;
        Fixed totalunidad = pvpund + ( ( pvpund - ( pvpund * desc1 / 100 ) ) * percentiva );
        basesimp[linea->DBvalue ( "ivalalbaran" ) ] = basesimp[linea->DBvalue ( "ivalalbaran" ) ] + base;
        basesimpreqeq[linea->DBvalue ( "reqeqlalbaran" ) ] = basesimpreqeq[linea->DBvalue ( "reqeqlalbaran" ) ] + base;
        /// Hacemos la impresion
        QString str =  linea->DBvalue ( "desclalbaran" ).leftJustified ( 22, ' ', TRUE ) + linea->DBvalue ( "cantlalbaran" ).rightJustified ( 1, ' ' ) + QString ( " " ) + totalunidad.toQString().rightJustified ( 2, ' ' )  + subtotaliva.toQString().rightJustified ( 8, ' ' );
        file.write ( str.toAscii() );
        file.write ( "\n", 1 );
    } // end for


    file.write ( "\n", 1 );
    file.write ( "\n", 1 );

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
        QString str = "Base Imp" + it.key() + "% " + parbaseimp.toQString().rightJustified ( 10, ' ' );
//        file.write(str.rightJustified(42,' ').toAscii());
//        file.write ( "\n", 1);
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
        QString str = "IVA" + it.key() + "% " + pariva.toQString().rightJustified ( 10, ' ' );
//        file.write(str.rightJustified(42,' ').toAscii());
//        file.write ( "\n", 1);

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
        QString str = "R. Eq" + it.key() + "% " + parreqeq.toQString().rightJustified ( 10, ' ' );
//        file.write(str.rightJustified(42,' ').toAscii());
//        file.write ( "\n", 1);
        totreqeq = totreqeq + parreqeq;
    } // end for



    Fixed totirpf = totbaseimp * irpf / 100;

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

    Fixed total = totiva + totbaseimp + totreqeq - totirpf;
    str = "TOTAL " + total.toQString().rightJustified ( 10, ' ' );
    file.write ( str.rightJustified ( 42, ' ' ).toAscii() );
    file.write ( "\n", 1 );

    /// Lanzamiento de un plugin extraño hacia el total
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

    /// Imprimimos el dibujo final
    /*
            file.write ("\x1B\x2A\x00\xD2\x00", 5);
            for (int i=0; i <15; i++) {
                file.write ( "\x01\x02\x04\x08\x10\x20\x40\x80", 8);
                file.write ("\x40\x20\x10\x08\x04\x02", 6);
            }
    */
    /// Imprimimos espacios
    file.write ( "\n \n \n \n \n", 9 );


    /// El corte de papel.
    file.write ( "\x1D\x56\x01", 3 );
    file.close();

    /// La apertura del cajon.
    abrircajon();
}


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
   Fixed cant ( cantidad );
   /// Comprueba la existencia de la linea de ticket.
   if ( m_lineaActual == NULL ) {
	mensajeAviso ( "No existe linea" );
	return;
   } // end if
   Fixed cantorig ( m_lineaActual->DBvalue ( "cantlalbaran" ) );
   Fixed suma = cant + cantorig;
   if ( suma == Fixed ( "0.00" ) ) {
	borrarLinea(m_lineaActual);
	//listaLineas() ->removeAt ( listaLineas() ->indexOf ( m_lineaActual ));
	//m_lineaActual = listaLineas() ->at ( 0 );
   } else {
	m_lineaActual->setDBvalue ( "cantlalbaran", suma.toQString() );
   } // end if
   pintar();
/*
    Fixed cant ( cantidad );
    /// Comprueba la existencia de la linea de ticket.
    if ( m_lineaActual == NULL ) {
        mensajeAviso ( "No existe linea" );
        return;
    } // end if
    Fixed cantorig ( m_lineaActual->DBvalue ( "cantlalbaran" ) );
    Fixed suma = cant + cantorig;
    if ( suma == Fixed ( "0.00" ) ) {
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
   Fixed cant ( cantidad );
   /// Comprueba la existencia de la linea de ticket.
   if ( m_lineaActual == NULL ) {
	mensajeAviso ( "No existe linea" );
	return;
   } // end if

   if ( cant == 0 ) {
	borrarLinea(m_lineaActual);
	//listaLineas() ->removeAt ( listaLineas() ->indexOf ( m_lineaActual ));
	//m_lineaActual = listaLineas() ->at ( 0 );
   } else {
	m_lineaActual->setDBvalue ( "cantlalbaran", cant.toQString() );
   } // end if
   pintar();
/*
    Fixed cant ( cantidad );
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
    Fixed valor ( precio );
    /// Comprueba la existencia de la linea de ticket.
    if ( m_lineaActual == NULL ) {
        mensajeAviso ( "No existe linea" );
        return;
    } // end if
    m_lineaActual->setDBvalue ( "pvplalbaran", valor.toQString() );
    pintar();
}

void Ticket::insertarArticuloCodigo ( QString codigo )
{
    _depura ( "Ticket::insertarArticuloCodigo", 0 );
    QString query = "SELECT * FROM articulo WHERE codigocompletoarticulo= '" + codigo + "'";
    cursor2 *cur = empresaBase() ->cargacursor ( query );
    if ( !cur->eof() ) {
        insertarArticulo ( cur->valor ( "idarticulo" ), Fixed ( "1" ) );
    } // end if
    delete cur;

    g_plugins->lanza ( "Ticket_insertarArticulo_Post", this );
    _depura ( "Ticket::insertarArticuloCodigo", 0 );

}

void Ticket::insertarArticuloCodigoNL ( QString codigo )
{
    QString query = "SELECT * FROM articulo WHERE codigocompletoarticulo= '" + codigo + "'";
    cursor2 *cur = empresaBase() ->cargacursor ( query );
    if ( !cur->eof() ) {
        insertarArticulo ( cur->valor ( "idarticulo" ), Fixed ( "1" ), TRUE );
    } // end if
    delete cur;
    g_plugins->lanza ( "Ticket_insertarArticuloNL_Post", this );

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
        QString id;
        empresaBase() ->begin();
        DBsave ( id );
        DBRecord *item;
        for ( int i = 0; i < listaLineas() ->size(); ++i ) {
            QString id1;
            item = listaLineas() ->at ( i );
            item->setDBvalue ( "idalbaran", id );
            item->setDBvalue ( "ordenlalbaran", QString::number ( i ) );
            item->DBsave ( id1 );
        }// end for
        empresaBase() ->commit();

        cursor2 *cur = empresaBase() ->cargacursor ( "SELECT * FROM albaran WHERE idalbaran = " + id );
        DBload ( cur );
        delete cur;

        _depura ( "END Ticket::guardar", 0 );
        return 0;
    } catch ( ... ) {
        mensajeInfo ( "Error inesperado con la base de datos" );
        empresaBase() ->rollback();
        return -1;
    } // end try
}


void Ticket::borrarLinea(DBRecord* linea)
{
   if(linea == NULL)
	return;
   int numlinea = listaLineas()->indexOf(linea);

   if(linea == m_lineaActual){
	listaLineas() ->removeAt ( listaLineas() ->indexOf ( linea ) );
	if(numlinea > listaLineas()->count()-1)
	{
		m_lineaActual = listaLineas()->count() > 0?listaLineas()->at ( listaLineas()->count()-1 ):NULL;
	} else {
		m_lineaActual = listaLineas()->count() > 0?listaLineas() ->at (numlinea):NULL;
	}
   } else {
	listaLineas() ->removeAt ( listaLineas() ->indexOf ( linea ) );
   }
}



