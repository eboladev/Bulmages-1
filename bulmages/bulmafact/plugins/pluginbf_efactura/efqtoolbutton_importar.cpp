/***************************************************************************
 *   Copyright (C) 2006 by Arturo Martin Llado                             *
 *   amartin@conetxia.com                                                  *
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

/// Necesarios para importacion de efactura
#include <QtXml/QDomDocument>
#include <QtXml/QDomNodeList>
#include <QtXml/QDomNode>
#include <QtCore/QString>
#include <QtWidgets/QFileDialog>
#include <QtCore/QMap>
#include <QtCore/QList>

#include "efqtoolbutton_importar.h"
#include "facturasplist.h"
#include "blfunctions.h"
#include "facturapview.h"
#include "blfixed.h"
#include "listlinfacturapview.h"
#include "bfcompany.h"
#include "listdescfacturaprovview.h"
#include "bldb.h"


///
/**
\param faclistado
\param parent
**/
EFQToolButtonImportar::EFQToolButtonImportar ( FacturasProveedorList *faclistado, QWidget *parent ) : QToolButton ( parent ), BlMainCompanyPointer()
{
    BL_FUNC_DEBUG
    m_faclistado = faclistado;
    setMainCompany ( faclistado->mainCompany() );
    connect ( this, SIGNAL ( clicked() ), this, SLOT ( click() ) );
    
}


///
/**
**/
EFQToolButtonImportar::~EFQToolButtonImportar()
{
    BL_FUNC_DEBUG
    
}


/// Dado un nombre de etiqueta se retorna el texto/valor que contiene (<etiqueta>valor</etiqueta>).
/**
\param nombre
\param doc
\return
**/
QString EFQToolButtonImportar::obten_valor_nodo ( QString nombre, QDomDocument *doc )
{
    BL_FUNC_DEBUG
    QDomNodeList lista_nodos = doc->elementsByTagName ( nombre );
    QDomNode nodo = lista_nodos.item ( 0 );

    if ( nodo.isNull() ) {
	BlDebug::blDebug ( Q_FUNC_INFO, 0, _("No hay un nodo con ese nombre.") );
    } // end if

    

    return nodo.toElement().text();
}


/// Esta funcion obtiene los descuentos y los mete dentro de una QList, la cual se
/// recibe como parametro y devuelve un QString con el valor total de los descuentos.
/**
\param doc
\param lista_descuentos
\return
**/
QString EFQToolButtonImportar::obten_descuento_factura ( QDomDocument *doc, QList< QMap<QString, QString> > &lista_descuentos )
{
    BL_FUNC_DEBUG
    // Obtenemos el nodo padre
    QDomNodeList lista_padre = doc->elementsByTagName ( "Invoice" );
    QDomNode padre = lista_padre.item ( 0 );
    QMap<QString, QString> mapa_descuento;

    QDomNode nodo1, nodo2;

    BlFixed total_descuento = "0.00";

    /// Nos movemos entre los nodos hijos cac:AllowanceCharge
    /// Ojo, no confundir con los elementos cac:AllowanceCharge que estan
    /// dentro del elemento cac:InvoiceLine. Por eso se hace este tipo de
    /// recorrido. Cogemos los nodos cac:AllowanceCharge que son hijos
    /// del nodo raiz, no todos los nodos cac:AllowanceCharge.
    nodo1 = padre.firstChildElement ( "cac:AllowanceCharge" );

    while ( !nodo1.isNull() ) {

        /// Nos situamos sobre el elemento que contiene el porcentaje de descuento sobre el PVP
        nodo2 = nodo1.firstChildElement ( "cbc:MultiplierFactorNumeric" );

        mapa_descuento["conceptdfactura"] = "DESCUENTO";
        mapa_descuento["proporciondfactura"] = nodo2.toElement().text();

        /// Nos situamos sobre el elemento que contiene la cantidad
        nodo2 = nodo2.nextSiblingElement ( "cbc:Amount" );

        /// Acumulamos...
        total_descuento = total_descuento + BlFixed ( nodo2.toElement().text() );

        /// Guardamos la informacion de este descuento en la lista de descuentos
        lista_descuentos.append ( mapa_descuento );

        /// Siguiente descuento, si existe
        nodo1 = nodo1.nextSiblingElement ( "cac:AllowanceCharge" );
    }

    

    return total_descuento.toQString();
}


/// Esta funcion recibe como parametro un QMap por referencia, al cual se le insertara
/// la informacion sobre la linea de factura situada en la posicion indicada por el parametro i,
/// contando desde 0 hasta numlineas-1 y de arriba a abajo del documento.
/**
\param doc
\param mapa_lfactura
\param i
**/
void EFQToolButtonImportar::obten_linea_factura ( QDomDocument *doc, QMap<QString, QString> &mapa_lfactura, int i )
{
    BL_FUNC_DEBUG
    QDomNodeList lista_lineas = doc->elementsByTagName ( "cac:InvoiceLine" );
    QDomNode padre = lista_lineas.item ( i );
    QDomNode tmp;

    // Buscamos la descripcion de esa linea de factura
    tmp = padre;
    tmp = tmp.firstChildElement ( "cac:Item" );
    tmp = tmp.firstChildElement ( "cbc:Description" );

    mapa_lfactura["desclfactura"] = tmp.toElement().text();

    // Buscamos la cantidad de elementos pertenecientes a la linea
    tmp = padre;
    tmp = tmp.firstChildElement ( "cbc:InvoicedQuantity" );

    mapa_lfactura["cantlfactura"] = tmp.toElement().text();

    // Buscamos PVP de la linea
    tmp = padre;
    tmp = tmp.firstChildElement ( "cbc:LineExtensionAmount" );

    mapa_lfactura["pvplfactura"] = tmp.toElement().text();

    // Buscamos IVA de la linea
    tmp = padre;
    tmp = tmp.firstChildElement ( "cac:TaxTotal" );
    tmp = tmp.firstChildElement ( "cbc:TotalTaxAmount" );

    mapa_lfactura["ivalfactura"] = tmp.toElement().text();

    // Buscamos descuento de la linea
    tmp = padre;
    tmp = tmp.firstChildElement ( "cac:AllowanceCharge" );
    tmp = tmp.firstChildElement ( "cbc:MultiplierFactorNumeric" );

    mapa_lfactura["descuentolfactura"] = tmp.toElement().text();

    // Buscamos ID de articulo de la linea
    tmp = padre;
    tmp = tmp.firstChildElement ( "cac:Item" );
    tmp = tmp.firstChildElement ( "cac:SellersItemIdentification" );
    tmp = tmp.firstChildElement ( "cac:ID" );

    mapa_lfactura["idarticulo"] = tmp.toElement().text();

    
}


/// Esta funcion obtiene el CIF de la empresa que emitio la factura (el campo
/// cac:ID dentro de la seccion cac:PartyIdentification del elemento cac:SellerParty).
/// Una vez obtenido este valor, nos sirve para saber si este proveedor existe o no
/// en nuestra base de datos. Si no existe se aborta el proceso de importacion y se
/// le pide al usuario que primero anyada a este nuevo proveedor en la base de datos.
/**
\param doc
\return
**/
QString EFQToolButtonImportar::obten_id_proveedor ( QDomDocument *doc )
{
    BL_FUNC_DEBUG
    // Nos situamos sobre la parte que identifica al vendedor
    QDomNodeList lista_nodos = doc->elementsByTagName ( "cac:SellerParty" );
    // Obtenemos el nodo padre de esta seccion
    QDomNode padre = lista_nodos.item ( 0 );
    // Lo recorremos hasta llegar a donde nos interesa
    QDomNode nodo = padre;;

    nodo = nodo.firstChildElement ( "cac:Party" );
    nodo = nodo.firstChildElement ( "cac:PartyIdentification" );
    nodo = nodo.firstChildElement ( "cac:ID" );

    

    return nodo.toElement().text();
}


/// ------------------ Importa una factura desde un fichero en formato UBL 1.0 ------------------- ///
/**
\return
**/
void EFQToolButtonImportar::importa_factura_ubl()
{
    BL_FUNC_DEBUG

    QString fichero = QFileDialog::getOpenFileName (
                          this,
                          "Escoja un fichero que contenga una efactura para importarlo a la base de datos de BulmaFact",
                          "/tmp",
                          "Todos los archivos (*)"
                      );

    /// Si se le da a cancelar en el dialogo anterior
    /// cancelamos la ejecucion del resto del codigo
    if ( fichero == "" )
        return;

    /// Declaramos una variable de tipo QDomDocument que usaremos
    /// para cargar los datos del fichero en memoria
    QDomDocument doc ( "efactura" );
    BlFile file ( fichero );

    if ( !file.open ( QIODevice::ReadOnly ) )
        return;

    /// Cargamos los datos en memoria usando el contenido de 'fichero'
    if ( !doc.setContent ( &file ) ) {
        file.close();
        return;
    }

    file.close();

    /// Empezamos a obtener datos de la efactura

    QString numeroFactura = obten_valor_nodo ( "ID", &doc );
    QString fechaFactura = obten_valor_nodo ( "cbc:IssueDate", &doc );
    QString descFactura = obten_valor_nodo ( "cbc:Note", &doc );
    QString bimpFactura = obten_valor_nodo ( "cbc:LineExtensionTotalAmount", &doc );
    QString impFactura = obten_valor_nodo ( "cbc:TotalTaxAmount", &doc );
    QString totalFactura = obten_valor_nodo ( "cbc:TaxInclusiveTotalAmount", &doc );

    /// Comprobamos que el proveedor existe. Si no, abortamos y damos mensaje de error.
    QString idProveedor = obten_id_proveedor ( &doc );

    QString query = "SELECT * FROM proveedor WHERE cifproveedor = '" + idProveedor + "'";
    BlDbRecordSet *proveedor = mainCompany() ->loadQuery ( query );

    if ( proveedor->numregistros() == 0 ) {
	BlDebug::blDebug ( Q_FUNC_INFO, 0, QString(_("El proveedor con CIF '%1' no existe en la base de datos. Hay que crearlo antes de importar esta factura.")).arg(idProveedor) );
	blMsgError(QString(_("El proveedor con CIF '%1' no existe en la base de datos. Hay que crearlo antes de importar esta factura.")).arg(idProveedor));
        return;
    }

/// Mostramos la ficha con la informacion de la factura importada --------------------------------------

    FacturaProveedorView *fp = ( ( BfCompany * ) mainCompany() ) ->newFacturaProveedorView();
    mainCompany() ->m_pWorkspace->addSubWindow ( fp );
    fp->inicializar();
    fp->pintar();
    fp->show();

/// Obtenemos los descuentos ---------------------------------------------------------------------------

    QList< QMap<QString, QString> > lista_mapas_dfactura;

    QString descuentoFactura = obten_descuento_factura ( &doc, lista_mapas_dfactura );

/// Vamos a usar un QMap para ir recorriendo los descuentos de la factura y los valores
/// los iremos guardando, una vez obtenidos los que nos interesan, en una lista de QMaps.

    // Contamos los descuentos que hay

    int numdescuentos = lista_mapas_dfactura.count();

///  Estas son las claves que vamos a usar dentro del QMap mapa_dfactura

///  mapa_dfactura["conceptdfactura"]
///  mapa_dfactura["proporciondfactura"]

/// Pintamos los descuentos --------------------------------------------

    ListDescuentoFacturaProvView *descuentos = ( ListDescuentoFacturaProvView * ) fp->getlistadescuentos();
    BlDbSubFormRecord *rec = descuentos->lista() ->last();
    QMap <QString, QString> mapa_dfactura;

    for ( int i = 0; i < numdescuentos; i++ ) {
        descuentos->setInsert ( false );
        mapa_dfactura = lista_mapas_dfactura.at ( i );

        rec->setDbValue ( "conceptdfacturap", mapa_dfactura["conceptdfactura"] );
        rec->setDbValue ( "proporciondfacturap", mapa_dfactura["proporciondfactura"] );

        descuentos->setInsert ( true );
        descuentos->newRecord();

        rec = descuentos->lista() ->last();
    } // end for

/// FIN Descuentos de factura --------------------------------------------------------------------------

/// Obtenemos lineas de factura ------------------------------------------------------------------------

    // Contamos las lineas que hay

    QDomNodeList lista_lfactura = doc.elementsByTagName ( "cac:InvoiceLine" );

    lista_lfactura.item ( 0 );

    int numlineas = 0;

    for ( int i = 0; !lista_lfactura.item ( i ).isNull(); i++ )
        numlineas = i + 1;

    if ( numlineas == 0 ) {
	BlDebug::blDebug ( Q_FUNC_INFO, 0, _("Esta factura es erronea. No contiene ninguna linea asociada.") );
	blMsgError(_("Esta factura es erronea. No contiene ninguna linea asociada."));
        exit ( -1 );
    }

/// Vamos a usar un QMap para ir recorriendo las lineas de factura y los valores
/// los iremos guardando, una vez obtenidos los que nos interesan, en una lista de QMaps.

    QMap<QString, QString> mapa_lfactura;
    QList< QMap<QString, QString> > lista_mapas_lfactura;

///  Estas son las claves que vamos a usar dentro del QMap mapa_lfactura

///  mapa_lfactura["desclfactura"]
///  mapa_lfactura["cantlfactura"]
///  mapa_lfactura["pvplfactura"]
///  mapa_lfactura["ivalfactura"]
///  mapa_lfactura["descuentolfactura"]
///  mapa_lfactura["idarticulo"]

/// Mientras haya lineas, las vamos obteniendo
    for ( int i = 0; i < numlineas; i++ ) {
        obten_linea_factura ( &doc, mapa_lfactura, i );
        lista_mapas_lfactura.append ( mapa_lfactura );
    }

/// Pintamos las lineas de factura --------------------------------------------

    ListLinFacturaProveedorView *lineas = ( ListLinFacturaProveedorView * ) fp->getlistalineas();
    rec = lineas->lista() ->last();
    BlDbRecordSet *articulo = NULL;
    QString idarticulo, nomarticulo;

    for ( int i = 0; i < numlineas; i++ ) {
        lineas->setInsert ( false ); /// Hace falta que este en el bucle? preguntar a Tomeu
        mapa_lfactura = lista_mapas_lfactura.at ( i );

        /// Comprobamos que existe un articulo con ese codigo en la BD
        /// Si no es asi, mandamos una alerta al usuario y anyadimos ese
        /// articulo a la linea como articulo generico
        query = "SELECT * FROM articulo WHERE codigocompletoarticulo = '" + mapa_lfactura["idarticulo"] + "'";
        articulo = mainCompany() ->loadQuery ( query );

        /// Si no obtenemos resultados cargamos en el cursor articulo los
        /// valores que necesitamos (idarticulo y nomarticulo)
        if ( articulo->numregistros() == 0 ) {

	    BlDebug::blDebug ( Q_FUNC_INFO, 0, QString(_("El articulo con codigo completo '%1' no existe en la base de datos. Se importara como articulo generico.")).arg(mapa_lfactura["idarticulo"]) );
	    blMsgError(QString(_("El articulo con codigo completo '%1' no existe en la base de datos. Se importara como articulo generico.")).arg(mapa_lfactura["idarticulo"]));

            /// Obtenemos el codigo de articulo generico
            query = "SELECT valor FROM configuracion WHERE nombre = 'CodArticuloGenerico'";
            articulo = mainCompany() ->loadQuery ( query );

            mapa_lfactura["idarticulo"] = articulo->value( "valor" );

            /// Datos necesarios para guardar correctamente la linea de factura
            query = "SELECT idarticulo, nomarticulo FROM articulo WHERE codigocompletoarticulo = '" + articulo->value( "valor" ) + "'";
            articulo = mainCompany() ->loadQuery ( query );
        } // end if

        idarticulo  = articulo->value( "idarticulo" );
        nomarticulo = articulo->value( "nomarticulo" );

        rec->setDbValue ( "codigocompletoarticulo", mapa_lfactura["idarticulo"] );

//   rec->refresh();

        rec->setDbValue ( "idarticulo", articulo->value( "idarticulo" ) );

        rec->setDbValue ( "nomarticulo", articulo->value( "nomarticulo" ) );
        rec->setDbValue ( "desclfacturap", mapa_lfactura["desclfactura"] );
        rec->setDbValue ( "cantlfacturap", mapa_lfactura["cantlfactura"] );
        rec->setDbValue ( "pvplfacturap", mapa_lfactura["pvplfactura"] );
        rec->setDbValue ( "ivalfacturap", mapa_lfactura["ivalfactura"] );
        rec->setDbValue ( "descuentolfacturap", mapa_lfactura["descuentolfactura"] );

        lineas->setInsert ( true );
        lineas->newRecord();

        rec = lineas->lista() ->last();
    } // end for

    delete articulo;

/// FIN lineas de factura ------------------------------------------------------------------------------

/// Empezamos a pintar datos ---------------------------------------------------------------------------
    /*
     fp->pintanumfacturap(numeroFactura);
     fp->pintafechafacturap(fechaFactura);
     fp->pintadescfacturap(descFactura);
     fp->pintaidproveedor(proveedor->value("idproveedor"));
    */
    fp->pintar();

    delete proveedor;

    BlFixed bimp ( bimpFactura );
    BlFixed iva ( impFactura );

    fp->calculaypintatotales();
    fp->m_totalDiscounts->setText ( descuentoFactura );

/// Damos valores a los campos dbValue -----------------------------------------------------------------

    lineas->setMainCompany ( mainCompany() );
    descuentos->setMainCompany ( mainCompany() );

    fp->setDbValue ( "idproveedor", idProveedor );
    fp->setDbValue ( "reffacturap", "" ); /// El valor lo pone el usuario que importa la factura
    fp->setDbValue ( "numfacturap", numeroFactura );
    fp->setDbValue ( "ffacturap", fechaFactura );
    fp->setDbValue ( "descfacturap", descFactura );
    fp->setDbValue ( "comentfacturap", "" );

    /// Que seleccione la forma de pago el que esta importanto la factura.
    /// Esto lo hacemos asi porque guardamos este campo como una cadena
    /// de texto dentro de la efactura, al ser algo tan variable mejor que
    /// lo haga de nuevo el propio usuario.
    fp->setDbValue ( "idforma_pago", "" );

    fp->setDbValue ( "procesadafacturap", "" );

    
}


///
/**
**/
void EFQToolButtonImportar::click()
{
    BL_FUNC_DEBUG
    importa_factura_ubl();
    
}
