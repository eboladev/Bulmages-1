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

#include <QtWidgets/QWidget>
#include <QtGui/QCloseEvent>
#include <QtCore/QFile>
#include <QtXml/QDomDocument>
#include <QtXml/QDomNode>
#include <QtCore/QTextStream>
#include <QtWidgets/QHBoxLayout>
#include <QtCore/QObject>
#include <QtWidgets/QPushButton>


#include <QtWidgets/QCheckBox>
#include <QtWidgets/QInputDialog>
#include "bluiloader.h"




#include "blmaincompany.h"
#include "btticket.h"
#include "blplugins.h"
#include "blescprinter.h"

/// Una factura puede tener multiples bases imponibles. Por eso definimos el tipo base
/// como un QMap.
typedef QMap<QString, BlFixed> base;

BtTicket::BtTicket ( BlMainCompany *emp, QWidget *parent ) : BlWidget ( emp, parent ), BlDbRecord ( emp )
{
    BL_FUNC_DEBUG
    
    m_nextLineIsInsert = false;
    
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
    // Control de cambios realizados en los tickets
    addDbField ( "cambiosalbaran", BlDbField::DbVarChar, BlDbField::DbNothing, _( "Cambios" ) );
    // Control de cambios realizados en los tickets tras guardar
    addDbField ( "cambiospostalbaran", BlDbField::DbVarChar, BlDbField::DbNothing, _( "Cambios tras guardar" ) );

    setDbValue ( "ticketalbaran", "true" );
    setDbValue ( "idalmacen", g_confpr->value( CONF_IDALMACEN_DEFECTO ) );
    setDbValue ( "idcliente", g_confpr->value( CONF_IDCLIENTE_DEFECTO ) );
    setDbValue ( "idtrabajador", g_confpr->value( CONF_IDTRABAJADOR_DEFECTO ) );
    setDbValue ( "descalbaran", "Ticket de venta" );
    setDbValue ( "idforma_pago", g_confpr->value( CONF_IDFORMA_PAGO_CONTADO ) );

    m_lineaActual = NULL;
    m_listaLineas = new QList<BlDbRecord *>;
    
    m_nomTicketDefecto = _("Ticket actual");

    g_plugins->run ( "BtTicket_BtTicket_Post", this );

    start();
    
    
}

BtTicket::~BtTicket()
{
    BL_FUNC_DEBUG
    g_plugins->run ( "Des_BtTicket_BtTicket_Post", this );
    
}


void BtTicket::setNextLineIsInsert(bool nextLineIsInsert)
{
    BL_FUNC_DEBUG
    m_nextLineIsInsert = nextLineIsInsert;
}


bool BtTicket::nextLineIsInsert()
{
    BL_FUNC_DEBUG
    return m_nextLineIsInsert;
}


QString BtTicket::nomTicketDefecto()
{
    BL_FUNC_DEBUG
    return m_nomTicketDefecto;
    
}


void BtTicket::agregarLog(const QString &log) {
    BL_FUNC_DEBUG
    if (dbValue("idalbaran") != "") {
      setDbValue("cambiospostalbaran", dbValue("cambiospostalbaran") + log + "\n");
    } else {
      setDbValue("cambiosalbaran", dbValue("cambiosalbaran") + log + "\n");
    } // end if
}


BlDbRecord * BtTicket::agregarLinea()
{
    BL_FUNC_DEBUG

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

    BlDebug::blDebug ( "Hacemos el append", 0 );

    /// Agregamos el BlDbRecord a la lista de lineas de ticket.
    m_listaLineas->append ( item );

    g_plugParams = (void *) item;
    g_plugins->run("BtTicket_agregarLinea_Post", this);

    
    return item;
}

void BtTicket::pintar()
{
    BL_FUNC_DEBUG
    BlDebug::blDebug ( "BtTicket::pintar", 0, "Metodo para reimplementar en clases derivadas" );
    /// Disparamos los plugins.
    int res = g_plugins->run ( "BtTicket_pintar", this );
    if ( res != 0 ) {
        return;
    } // end if
    
}

QList<BlDbRecord *> *BtTicket::listaLineas()
{
    BL_FUNC_DEBUG
    return m_listaLineas;
}


BlDbRecord *BtTicket::insertarArticulo ( QString idArticulo, BlFixed cantidad, bool nuevaLinea )
{
    BL_FUNC_DEBUG

    if (m_nextLineIsInsert) {
	nuevaLinea = true;
    } // end if

    /// Buscamos si ya hay una linea con el articulo que buscamos
    m_lineaActual = NULL;
    BlDbRecord *item;

    for ( int i = 0; i < listaLineas() ->size(); ++i ) {
        item = listaLineas() ->at ( i );
        if ( item->dbValue ( "idarticulo" ) == idArticulo )
            m_lineaActual = item;
    }// end for

    if ( m_lineaActual && nuevaLinea == false ) {
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
        QString query = "SELECT pvparticulo, codigocompletoarticulo, nomarticulo, porcentasa_iva FROM articulo LEFT JOIN (SELECT idtipo_iva, porcentasa_iva, fechatasa_iva FROM tasa_iva ) AS t1 ON articulo.idtipo_iva = t1.idtipo_iva WHERE idarticulo = " + idArticulo + " ORDER BY t1.fechatasa_iva DESC LIMIT 1";
        BlDbRecordSet *cur = mainCompany() ->loadQuery ( query );

        if ( !cur->eof() ) {
            m_lineaActual->setDbValue ( "pvplalbaran", cur->value( "pvparticulo" ) );
            m_lineaActual->setDbValue ( "codigocompletoarticulo", cur->value( "codigocompletoarticulo" ) );
            m_lineaActual->setDbValue ( "nomarticulo", cur->value( "nomarticulo" ) );
            m_lineaActual->setDbValue ( "desclalbaran", cur->value( "nomarticulo" ) );
            m_lineaActual->setDbValue ( "ivalalbaran", cur->value( "porcentasa_iva") );

        } // end if
        delete cur;
    } // end if

    /// Registramos el cambio en el control de logs.
    agregarLog("INSERCION ARTICULO " + m_lineaActual->dbValue("nomarticulo") + " Cantidad:" + cantidad.toQString('.'));
    

    /// Disparamos los plugins.
    g_plugins->run ( "BtTicket_insertarArticulo_Post", this );

    /// Pintamos el ticket ya que se ha modificado.
    pintar();

    
    return m_lineaActual;
}


void  BtTicket::borrarArticulo ( BlDbRecord *linea, BlFixed cantidad )
{
    BL_FUNC_DEBUG
    /// Comprueba que haya un articulo seleccionado.
    if ( m_lineaActual == NULL ) {
        return;
    } // end if

    /// Disparamos los plugins.
    g_plugins->run ( "BtTicket_borrarArticulo", this );

    /// Registramos el cambio en el control de logs.
    agregarLog("BORRAR ARTICULO "+m_lineaActual->dbValue("nomarticulo")+"  Cantidad:"+cantidad.toQString('.'));


    m_lineaActual->remove();
}

void  BtTicket::vaciarBtTicket()
{
    BL_FUNC_DEBUG
}

void  BtTicket::subirPosArticulo ( BlDbRecord *linea, int filas )
{
    BL_FUNC_DEBUG
}

void  BtTicket::bajarPosArticulo ( BlDbRecord *linea, int filas )
{
    BL_FUNC_DEBUG
}

void  BtTicket::inicioPosBtTicket ( BlDbRecord * )
{
    BL_FUNC_DEBUG
}

void  BtTicket::finPosBtTicket ( BlDbRecord * )
{
    BL_FUNC_DEBUG
}

BlDbRecord * BtTicket::lineaBtTicket ( int posicion )
{
    BL_FUNC_DEBUG
    return NULL;
}

BlDbRecord *BtTicket::lineaActBtTicket()
{
    BL_FUNC_DEBUG
    return m_lineaActual;
}

void BtTicket::setLineaActual ( BlDbRecord *rec )
{
    BL_FUNC_DEBUG
    m_lineaActual = rec;
}

void  BtTicket::setDescuentoGlobal ( BlFixed descuento )
{
    BL_FUNC_DEBUG
}

void BtTicket::abrircajon()
{
    BL_FUNC_DEBUG
    QString filestr = g_confpr->value(CONF_DIR_USER) + "bulmatpv_abrircajon.txt";
    
    QFile file ( filestr );
    
    if ( !file.open ( QIODevice::WriteOnly | QIODevice::Unbuffered ) ) {
        BlDebug::blDebug ( "Error en la Impresion de ticket", 2 );
    } // end if

    QStringList secuencia = g_confpr->value(CONF_CASHBOX_OPEN_CODE).split(",");

    /// El comando de apertura de cajon
    for (int i = 0; i < secuencia.size(); ++i) {
	    QString cad = QChar(secuencia.at(i).toInt());
	    file.write ( cad.toLatin1(), 1 );
    } // end for

    file.close();


    if (!g_confpr->value( CONF_CASHBOX_FILE).isEmpty() && g_confpr->value( CONF_CASHBOX_FILE) != "/dev/null") {
        QString comando = "cat " + g_confpr->value(CONF_DIR_USER) + "bulmatpv_abrircajon.txt" + "  > " + g_confpr->value( CONF_CASHBOX_FILE );
        system ( comando.toLatin1().data() );
    } else if (g_confpr->value(CONF_CUPS_DEFAULT_PRINTER).isEmpty() || g_confpr->value(CONF_CUPS_DEFAULT_PRINTER).toLower() == "none") {
        BlDebug::blDebug("Debe establecer el parametro CONF_CUPS_DEFAULT_PRINTER o CONF_CASHBOX_FILE para abrir el cajon " , 2);
    } else {
		blRawPrint( "bulmatpv_abrircajon.txt");
    } // end if


}



void BtTicket::imprimir(bool doSave)
{

  BL_FUNC_DEBUG

    if (doSave) {
        
        if ( save() == -1) {
            BlDebug::blDebug ( "Error en la llamada a save()", 0 );
            return;
        } // end if
        
    } // end if
    
    /// Disparamos los plugins.
    int res = g_plugins->run ( "BtTicket_imprimir", this );
    if ( res != 0 ) {
        g_plugins->run("BtTicket_imprimir_Post", this);
        BlDebug::blDebug("END BtTicket::imprimir",0);
        return;
    } // end if
    
    generateRML("ticket_normal.txt");


    if (!g_confpr->value( CONF_TICKET_PRINTER_FILE).isEmpty() && g_confpr->value( CONF_TICKET_PRINTER_FILE) != "/dev/null") {
        QString comando = "cat " + g_confpr->value(CONF_DIR_USER) + "ticket_normal.txt" + "  > " + g_confpr->value( CONF_TICKET_PRINTER_FILE );
        system ( comando.toLatin1().data() );
    } else if (!g_confpr->value( CONF_CASHBOX_FILE).isEmpty() && g_confpr->value( CONF_CASHBOX_FILE) != "/dev/null") {
        QString comando = "cat " + g_confpr->value(CONF_DIR_USER) + "ticket_normal.txt" + "  > " + g_confpr->value( CONF_CASHBOX_FILE );
        system ( comando.toLatin1().data() );
    } else if (!g_confpr->value(CONF_CUPS_TICKET_PRINTER).isEmpty() && g_confpr->value(CONF_CUPS_TICKET_PRINTER).toLower() != "none") {
		blRawPrint( "ticket_normal.txt", true, g_confpr->value( CONF_TICKET_PRINTER_FILE));
    } else if (g_confpr->value(CONF_CUPS_DEFAULT_PRINTER).isEmpty() || g_confpr->value(CONF_CUPS_DEFAULT_PRINTER).toLower() == "none") {
	blMsgError(_("Debe establecer el parametro 'CONF_CUPS_DEFAULT_PRINTER' o 'CONF_CASHBOX_FILE' para abrir el cajon."));
    } else {
		blRawPrint( "ticket_normal.txt");
    } // end if    


    g_plugins->run("BtTicket_imprimir_Post", this);

}


// ===========================

void BtTicket::subir()
{
  BL_FUNC_DEBUG
    int i;

    if ( listaLineas()->count() > 0) {
       /// Si no hay ninguna linea actual cogemos la ultima linea del ticket como linea actual
      if (m_lineaActual == NULL) {
            i = listaLineas() ->size();
      } else if (!listaLineas()->contains(lineaActBtTicket())) {
	    /// Podria darse el caso de un m_lineaActual mal actualizado. Y lo protegemos aqui.
            i = listaLineas() ->size();
      } else {
            i = listaLineas() ->indexOf ( lineaActBtTicket() );
      }// end if

        if ( i > 0 ) i--;
        setLineaActual ( listaLineas() ->at ( i ) );
        pintar();
    } // end if

}

void BtTicket::bajar()
{
  BL_FUNC_DEBUG
  int i;
    if ( listaLineas()->count() > 0 ) {
      if (m_lineaActual == NULL) {
            i = 0;
      } else if (!listaLineas()->contains(lineaActBtTicket())) {
	    /// Podria darse el caso de un m_lineaActual mal actualizado. Y lo protegemos aqui.
            i = 0;
      } else {
            i = listaLineas() ->indexOf ( lineaActBtTicket() );
      } // end if
      
	if ( i < listaLineas() ->size() - 1 ) i++;
        setLineaActual ( listaLineas() ->at ( i ) );
        pintar();
    } // end if

}

void BtTicket::agregarCantidad ( QString cantidad )
{
    BL_FUNC_DEBUG
    BlFixed cant ( cantidad );

    /// Comprueba la existencia de la linea de ticket.
    if ( m_lineaActual == NULL ) {
        blMsgWarning ( "No existe linea" );
        return;
    } // end if
    
    /// Registramos el cambio en el control de logs.
    agregarLog("AGREGAR CANTIDAD "+m_lineaActual->dbValue("nomarticulo")+" Cantidad:" + cantidad);
    
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
    BL_FUNC_DEBUG
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
    
    /// Registramos el cambio en el control de logs.
    agregarLog("PONER CANTIDAD "+m_lineaActual->dbValue("nomarticulo")+ " Cantidad:"+cantidad);

    if ( cant == 0 ) {
        borrarLinea ( m_lineaActual );
    } else {
        m_lineaActual->setDbValue ( "cantlalbaran", cant.toQString('0', cant.precision()) );
    } // end if


    pintar();
}

void BtTicket::ponerPrecio ( QString precio )
{
    BL_FUNC_DEBUG
    BlFixed valor( precio );
    
   
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

    g_plugins->run ( "BtTicket_ponerPrecio_Post", this );
    
    /// Registramos el cambio en el control de logs.
    agregarLog("PONER PRECIO "+m_lineaActual->dbValue("nomarticulo")+" Precio:" + precio);

    pintar();
}

void BtTicket::insertarArticuloCodigo ( QString codigo )
{
    BL_FUNC_DEBUG

    QString query = "SELECT idarticulo FROM articulo WHERE codigocompletoarticulo= '" + codigo + "'";
    BlDbRecordSet *cur = mainCompany() ->loadQuery ( query );
    
    if ( !cur->eof() ) {
        insertarArticulo ( cur->value( "idarticulo" ), BlFixed ( "1" ) );
    } // end if
    
    delete cur;

    g_plugins->run ( "BtTicket_insertarArticuloCodigo_Post", this );
    
    

}


void BtTicket::insertarArticuloCodigoNL ( QString codigo )
{
  BL_FUNC_DEBUG
    
    QString query = "SELECT idarticulo FROM articulo WHERE codigocompletoarticulo= '" + codigo + "'";
    BlDbRecordSet *cur = mainCompany() ->loadQuery ( query );
    
    if ( !cur->eof() ) {
        insertarArticulo ( cur->value( "idarticulo" ), BlFixed ( "1" ), true );
    } // end if
    
    delete cur;
    
    g_plugins->run ( "BtTicket_insertarArticuloCodigoNL_Post", this );
    
}


int BtTicket::load ( QString id )
{
  BL_FUNC_DEBUG
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
  Luego llama a plugins y a afterSave por si se quieren hacer acciones adicionales de guardado.
  Tras guardar todos los elementos hace una carga.
\return 0 Si no hay problemas. -1 Si ha habido problemas.
**/
int BtTicket::save()
{
    BL_FUNC_DEBUG

    try {

        if ( listaLineas() ->count() == 0 ) {
                blMsgWarning ( _( "El ticket esta vacio." ) );
                return -1;
        } // end if

        QString id;
        mainCompany() ->begin();
        dbSave ( id );

        BlDbRecord *item;
        
        for ( int i = 0; i < listaLineas() ->size(); ++i ) {
            QString id1;
            item = listaLineas() ->at ( i );
            int f = item->setDbValue ( "idalbaran", id );
            item->setDbValue ( "ordenlalbaran", QString::number ( i ) );
            /// TODO: Recargo de equivalencia siempre a 0 ?? Algo hay que poner.
            item->setDbValue ( "reqeqlalbaran", QString::number ( 0 ) );
            item->save();
        } // end for
        
        setDbValue("idalbaran", id);
        BlDbRecordSet *cur = mainCompany() ->loadQuery ( "SELECT * FROM albaran WHERE idalbaran = " + id );
        setDbValue("refalbaran", cur->value("refalbaran"));
        setDbValue("numalbaran", cur->value("numalbaran"));

        mainCompany() ->commit();

        delete cur;
        
        
        return 0;
        
    } catch ( ... ) {
    
        blMsgInfo ( "Error inesperado con la base de datos" );
        mainCompany() ->rollback();
        
        return -1;
        
    } // end try
}

void BtTicket::borrarLinea ( BlDbRecord *linea )
{
    BL_FUNC_DEBUG
    if ( linea == NULL )
        return;
    
    if (!listaLineas()->contains(linea) )
        return;

    g_plugParams = (void *) linea;
    g_plugins->run("BtTicket_borrarLinea", this);
       
    int numlinea = listaLineas()->indexOf ( linea );

    /// Registramos el cambio en el control de logs.
    agregarLog("BORRAR LINEA "+ linea->dbValue("nomarticulo"));

    if ( linea == m_lineaActual ) {
        listaLineas() ->removeAt ( listaLineas() ->indexOf ( linea ) );
        
        if ( numlinea > listaLineas()->count() - 1 ) {
            m_lineaActual = listaLineas()->count() > 0 ? listaLineas()->at ( listaLineas()->count() - 1 ) : NULL;
        } else {
            m_lineaActual = listaLineas()->count() > 0 ? listaLineas() ->at ( numlinea ) : NULL;
        } // end if
    } else {
        listaLineas() ->removeAt ( listaLineas() ->indexOf ( linea ) );
    } // end if
    
    linea->remove();
}


/// Hace la exportacion del campo a XML
QString BtTicket::exportXML() {
    BL_FUNC_DEBUG

    int error;
    BlDbField *campo;

    m_textoXML = "<BTTICKET>\n";
    m_textoXML += "\t<NOMTICKET>" + dbValue("nomticket") + "</NOMTICKET>\n"; 
    m_textoXML += "\t" + BlDbRecord::exportXML().replace("\t<","\t\t<").replace("\n<","\n\t<");
    m_textoXML += "\t<LISTALINEAS>\n";
    BlDbRecord *linea1;
    for ( int i = 0; i < m_listaLineas->size(); ++i ) {
        linea1 = m_listaLineas->at ( i );
        m_textoXML += "\t\t" + linea1->exportXML().replace("\t<","\t\t\t<").replace("\n<","\n\t\t<");
    } // end for
    m_textoXML += "\t</LISTALINEAS>\n";
    
    g_plugins->run ( "BtTicket_exportXML_Post", this );    
    
    m_textoXML += "</BTTICKET>\n";
    
    
    
    return m_textoXML;

}


bool BtTicket::syncXML(const QString &text, bool insertarSiempre) {
    BL_FUNC_DEBUG

    /* Para que los plugins tambien puedan tratar sus cosas ponemos el texto XML a disposicion de todos en la variable m_textoXML */
    m_textoXML = text;

    g_plugins->run ( "BtTicket_syncXML", this );
    
    QDomDocument doc ( "mydocument" );

    if ( !doc.setContent ( text ) ) {
	
        return 0;
    } // end if

    QDomElement docElem = doc.documentElement();
    QDomElement principal = docElem.firstChildElement ( "BLDBRECORD" );
    /// Comprobamos que los nombres coinciden y si no coinciden salimos pq no ha lugar a sincronizacion.
    if (!insertarSiempre) {
	QDomElement nomticket = docElem.firstChildElement ( "NOMTICKET" );
	if (nomticket.text() != dbValue("nomticket") ) {
	    BlDebug::blDebug("END BtTicket::syncXML", 0);
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
    
    g_plugins->run ( "BtTicket_syncXML_Post", this ); 
    
    return 1;
}





/// ================================ VAMOS A TRATAR DE HACER LA IMPRESION SENCILLA CON PLANTILLAS ============================

#ifdef IMPRESION_CAMBIADA






int BtTicket::generateRML ( void )
{
    BL_FUNC_DEBUG
    int err = BlDbRecord::generateRML();
    
    return err;
}

///
/**
**/
int BtTicket::generateRML ( const QString &arch )
{
    BlDebug::blDebug ( "BtTicket::generateRML", 0, arch );

    /// Vaciamos las variables de RML
    m_variables.clear();

    /// Ponemos en m_variables CONF_DBNAME y CONF_DBUSER
    m_variables["CONF_DBNAME"] = mainCompany()->dbName();
    m_variables["CONF_DBUSER"] = mainCompany()->currentUser();

    /// Disparamos los plugins
    int res = g_plugins->run ( "BtTicket_generateRML", this );
    if ( res != 0 ) {
        return 1;
    } // end if

    res = BlDbRecord::generateRML ( arch );

    
    return res;
}


#endif

