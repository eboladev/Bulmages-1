/***************************************************************************
 *   Copyright (C) 2005 by Tomeu Borras Riera                              *
 *   tborras@conetxia.com                                                  *
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

#include <fstream>

#include <QMessageBox>
#include <QWidget>
#include <QObject>
#include <QComboBox>
#include <QToolButton>
#include <QLayout>
#include <QTextStream>

#include "facturapview.h"
#include "bfcompany.h"
#include "listlinfacturapview.h"
#include "providerslist.h"
#include "blfunctions.h"
#include "albaranproveedorview.h"


/** Inicializa todos los componentes de la clase.
    mete la ventana en el workSpace. */
/**
\param comp
\param parent
**/
FacturaProveedorView::FacturaProveedorView ( BfCompany *comp, QWidget *parent )
        : BfForm ( comp, parent )
{
    BL_FUNC_DEBUG
    setAttribute ( Qt::WA_DeleteOnClose );
    try {
        /// Usurpamos la identidad de mlist y ponemos nuestro propio widget con sus cosillas.
        setupUi ( this );

        setTitleName ( _ ( "Factura Proveedor" ) );
        setDbTableName ( "facturap" );
        setDbFieldId ( "idfacturap" );
        addDbField ( "idfacturap", BlDbField::DbInt, BlDbField::DbPrimaryKey, _ ( "Id facturap" ) );
        addDbField ( "idproveedor", BlDbField::DbInt, BlDbField::DbNotNull, _ ( "Id proveedor" ) );
        addDbField ( "numfacturap", BlDbField::DbVarChar, BlDbField::DbNothing, _ ( "Numero" ) );
        addDbField ( "ffacturap", BlDbField::DbDate, BlDbField::DbNothing, _ ( "Fecha" ) );
        addDbField ( "procesadafacturap", BlDbField::DbBoolean, BlDbField::DbNothing, _ ( "Procesada facturap" ) );
        addDbField ( "comentfacturap", BlDbField::DbVarChar, BlDbField::DbNothing, _ ( "Comentario facturap" ) );
        addDbField ( "reffacturap", BlDbField::DbVarChar, BlDbField::DbNothing, _ ( "Referencia facturap" ) );
        addDbField ( "descfacturap", BlDbField::DbVarChar, BlDbField::DbNothing, _ ( "Descripcion facturap" ) );
        addDbField ( "idtrabajador", BlDbField::DbInt, BlDbField::DbNothing, _ ( "Id trabajador" ) );
        addDbField ( "idforma_pago", BlDbField::DbInt, BlDbField::DbNothing, _ ( "Id forma de pago" ) );

        /// Disparamos los plugins.
        int res = g_plugins->run ( "FacturaProveedorView_FacturaProveedorView", this );
        if ( res != 0 ) {
            return;
        } // end if

        subform2->setMainCompany ( comp );
        m_descuentos->setMainCompany ( comp );
        mui_idforma_pago->setMainCompany ( comp );
        mui_idforma_pago->setId ( "" );
        mui_idproveedor->setMainCompany ( comp );
        mui_idproveedor->setId ( "" );
        mui_reffacturap->setMainCompany ( comp );

        /// Establecemos los parametros de busqueda del Cliente
        mui_idproveedor->setLabel ( _ ( "Proveedor:" ) );
        mui_idproveedor->setTableName ( "proveedor" );
        mui_idproveedor->m_valores["cifproveedor"] = "";
        mui_idproveedor->m_valores["nomproveedor"] = "";

        setListaLineas ( subform2 );
        setListaDescuentos ( m_descuentos );

        m_totalBases->setReadOnly ( TRUE );
        m_totalBases->setAlignment ( Qt::AlignRight );
        m_totalTaxes->setReadOnly ( TRUE );
        m_totalTaxes->setAlignment ( Qt::AlignRight );
        m_totalDiscounts->setReadOnly ( TRUE );
        m_totalDiscounts->setAlignment ( Qt::AlignRight );
        m_totalfacturap->setReadOnly ( TRUE );
        m_totalfacturap->setAlignment ( Qt::AlignRight );
        insertWindow ( windowTitle(), this, FALSE );
	blScript(this);
    } catch ( ... ) {
        blMsgInfo ( _ ( "Error al crear la factura proveedor" ), this );
    } // end try
    
}


/** Al destruir una factura de proveedor se hace un refresco del listado
    de facturas de proveedor. */
/**
**/
FacturaProveedorView::~FacturaProveedorView()
{
    BL_FUNC_DEBUG
    

}

///
/**
**/
void FacturaProveedorView::on_m_descuentos_editFinish ( int, int )
{
    BL_FUNC_DEBUG
    calculaypintatotales();
    
}


///
/**
**/
void FacturaProveedorView::on_subform2_editFinish ( int, int )
{
    BL_FUNC_DEBUG
    calculaypintatotales();
    
}


/** inicializar debe ser invocado cuando se crea una nueva ficha sin cargar ningun
    date de la base de datos (por ejemplo una nueva ficha).
    Sirve para inicializar los componenetes sin necesidad de query alguno */
/**
**/
void FacturaProveedorView::inicializar()
{
    BL_FUNC_DEBUG
    subform2->inicializar();
    m_descuentos->inicializar();
    pintar();
    dialogChanges_readValues();
    
}


/** Pinta los totales de la Factura de proveedor.
    Este metodo se llama desde FacturaProveedor. */
/**
\param iva
\param base
\param total
\param desc
\param irpf
\param reqeq
**/
void FacturaProveedorView::pintatotales ( BlFixed iva, BlFixed base, BlFixed total, BlFixed desc, BlFixed irpf, BlFixed reqeq )
{
    BL_FUNC_DEBUG
    m_totalBases->setText ( QString ( base.toQString() ) );
    m_totalTaxes->setText ( QString ( iva.toQString() ) );
    m_totalfacturap->setText ( QString ( total.toQString() ) );
    m_totalDiscounts->setText ( QString ( desc.toQString() ) );
    m_totalIRPF->setText ( QString ( irpf.toQString() ) );
    m_totalReqEq->setText ( QString ( reqeq.toQString() ) );
    
}



/** SLOT que responde  a la pulsacion del boton ver albaranes.
    Consulta todos los albaranes con la misma referencia.
    Los instancia y los carga. */
/**
**/
void FacturaProveedorView::on_mui_veralbaranes_clicked()
{
    BL_FUNC_DEBUG
    QString query = "SELECT * FROM albaranp WHERE refalbaranp='" + dbValue ( "reffacturap" ) + "'";
    BlDbRecordSet *cur = mainCompany() ->loadQuery ( query );
    while ( !cur->eof() ) {
	/// Como estamos en un plugin buscamos nuevas formas de creacion de objetos.
	int resur = g_plugins->run ( "SNewAlbaranProveedorView", ( BfCompany * ) mainCompany() );
	if ( !resur ) {
	    blMsgInfo (_( "no se pudo crear instancia de albaran proveedor" ));
	    return;
	} // end if
	AlbaranProveedorView * albpro = ( AlbaranProveedorView * ) g_plugParams;
        albpro->load ( cur->value( "idalbaranp" ) );
        mainCompany() ->m_pWorkspace->addSubWindow ( albpro );
        albpro->show();
        cur->nextRecord();
    } // end while
    delete cur;
    
}


///
/**
\param id
**/
void FacturaProveedorView::on_mui_idproveedor_valueChanged ( QString id )
{
    BL_FUNC_DEBUG
    subform2->setIdProveedor ( id );
    mui_idforma_pago->setIdProveedor ( id );
    
}


/** Hace el borrado de la factura.
    Primero borra las lineas de factura.
    Luego borra los descuentos de factura.
    Y por ultimo delega en BlDbRecord el borrado del registro.
    Si algo falla devuelve -1.
    Si todo va bien devuelve 0.
*/
/**
\return
**/
int FacturaProveedorView::beforeDelete()
{
    BL_FUNC_DEBUG
    m_listalineas->remove();
    m_listadescuentos->remove();
    
    return 0;
}



/** Carga un FacturaProveedor.
    Carga el BlDbRecord con DBload y luego llama a la carga de las lineas y los descuentos.
    Al finalizar hace un pintado para que la informacion se vea actualizada.
    Si algo falla genera una excepcion -1.
    Si todo va bien devuelve 0.
*/
/**
\param idfacturap
\return
**/
int FacturaProveedorView::cargarPost ( QString idfacturap )
{
    BL_FUNC_DEBUG
    m_listalineas->load ( idfacturap );
    m_listadescuentos->load ( idfacturap );

    /// Disparamos los plugins.
    g_plugins->run ( "FacturaProveedorView_cargarPost_Post", this );

    calculaypintatotales();
    
    return 0;
}

/** Hace el guardado de una Factura de Proveedor.
    guarda el registro a traves de BlDbRecord y luego actualiza el identificador de facturap en
    las lineas y en los descuentos para guardarlos.
    Si algo falla genera una excepcion -1.
    Si todo va bien devuelve 0.
*/
/**
\return
**/
int FacturaProveedorView::afterSave()
{
    BL_FUNC_DEBUG
    m_listalineas->setColumnValue ( "idfacturap", dbValue ( "idfacturap" ) );
    m_listadescuentos->setColumnValue ( "idfacturap", dbValue ( "idfacturap" ) );
    m_listalineas->save();
    m_listadescuentos->save();
    
    return 0;
}


/** Imprime una factura de proveedor
*/
/// \DEPRECATED: No se prevee que se tengan que imprimir facturas de proveedor.
/**
\return
**/
void FacturaProveedorView::imprimirFacturaProveedor()
{
    /// Hacemos el lanzamiento de plugins para este caso.
    int res = g_plugins->run ( "imprimirFacturaProveedor", this );
    if ( res )
        return;

    base basesimp;

    /// Copiamos el archivo.
    QString archivo = g_confpr->value( CONF_DIR_OPENREPORTS ) + "facturap.rml";
    QString archivod = g_confpr->value( CONF_DIR_USER ) + "facturap.rml";
    blCopyFile(archivo, archivod);
    
    /// Copiamos el logo
    QString archivologo = g_confpr->value( CONF_DIR_OPENREPORTS ) + "logo.jpg";
    QString logousuario = g_confpr->value( CONF_DIR_USER ) + "logo.jpg";
    blCopyFile(archivologo, logousuario);

    QFile file;
    file.setFileName ( archivod );
    file.open ( QIODevice::ReadOnly );
    QTextStream stream ( &file );
    QString buff = stream.readAll();
    file.close();
    QString fitxersortidatxt = "";

    /// Linea de totales del presupuesto
    QString SQLQuery = "SELECT * FROM proveedor WHERE idproveedor = " + dbValue ( "idproveedor" );
    BlDbRecordSet *cur = mainCompany() ->loadQuery ( SQLQuery );
    if ( !cur->eof() ) {
        buff.replace ( "[dirproveedor]", cur->value( "dirproveedor" ) );
        buff.replace ( "[poblproveedor]", cur->value( "poblproveedor" ) );
        buff.replace ( "[telproveedor]", cur->value( "telproveedor" ) );
        buff.replace ( "[nomproveedor]", cur->value( "nomproveedor" ) );
        buff.replace ( "[cifproveedor]", cur->value( "cifproveedor" ) );
    } // end if
    delete cur;

    buff.replace ( "[numfacturap]", dbValue ( "numfacturap" ) );
    buff.replace ( "[ffacturap]", dbValue ( "ffacturap" ) );
    buff.replace ( "[comentfacturap]", dbValue ( "comentfacturap" ) );
    buff.replace ( "[descfacturap]", dbValue ( "descfacturap" ) );
    buff.replace ( "[reffacturap]", dbValue ( "reffacturap" ) );

    /// Impresion de la tabla de contenidos.
    fitxersortidatxt += "<blockTable style=\"tablacontenido\" colWidths=\"1.75cm, 8.75cm, 1.5cm, 1.5cm, 1.5cm, 2.25cm\" repeatRows=\"1\">\n";
    fitxersortidatxt += "<tr>\n";
    fitxersortidatxt += "        <td>" + _ ( "Codigo" ) + "</td>\n";
    fitxersortidatxt += "        <td>" + _ ( "Concepto" ) + "</td>\n";
    fitxersortidatxt += "        <td>" + _ ( "Cant." ) + "</td>\n";
    fitxersortidatxt += "        <td>" + _ ( "Precio" ) + "</td>\n";
    fitxersortidatxt += "        <td>" + _ ( "Desc." ) + "</td>\n";
    fitxersortidatxt += "        <td>" + _ ( "Total" ) + "</td>\n";
    fitxersortidatxt += "</tr>\n";
    QString l;

    /// Contador que sirve para poner lineas de mas en caso de que sea preciso.
    int i = 0;

    BlDbSubFormRecord *linea;
    for ( int i = 0; i < m_listalineas->rowCount(); ++i ) {
        linea = m_listalineas->lineaat ( i );
        BlFixed base = BlFixed ( linea->dbValue ( "cantlfacturap" ).toAscii().constData() ) * BlFixed ( linea->dbValue ( "pvplfacturap" ).toAscii().constData() );
        basesimp[linea->dbValue ( "ivalfacturap" ) ] = basesimp[linea->dbValue ( "ivalfacturap" ) ] + base - base * BlFixed ( linea->dbValue ( "descuentolfacturap" ).toAscii().constData() ) / 100;

        fitxersortidatxt += "<tr>\n";
        fitxersortidatxt += "        <td>" + linea->dbValue ( "codigocompletoarticulo" ) + "</td>\n";
        fitxersortidatxt += "        <td>" + linea->dbValue ( "desclfacturap" ) + "</td>\n";
        fitxersortidatxt += "        <td>" + linea->dbValue ( "cantlfacturap" ) + "</td>\n";
        fitxersortidatxt += "        <td>" + linea->dbValue ( "pvplfacturap" ) + "</td>\n";
        fitxersortidatxt += "        <td>" + linea->dbValue ( "descuentolfacturap" ) + " %</td>\n";
        fitxersortidatxt += "        <td>" + ( base - base * BlFixed ( linea->dbValue ( "descuentolfacturap" ) ) / 100 ).toQString() + "</td>\n";
        fitxersortidatxt += "</tr>";
        i++;
    } // end for

    while ( i++ < 15 )
        fitxersortidatxt += "<tr></tr>";

    fitxersortidatxt += "</blockTable>\n";
    buff.replace ( "[story]", fitxersortidatxt );

    BlFixed basei ( "0.00" );
    base::Iterator it;
    for ( it = basesimp.begin(); it != basesimp.end(); ++it ) {
        basei = basei + it.value();
    } // end for

    /// Impresion de los descuentos.
    fitxersortidatxt = "";
    BlFixed porcentt ( "0.00" );
    BlDbSubFormRecord *linea1;
    if ( m_listadescuentos->rowCount() ) {
        fitxersortidatxt += "<blockTable style=\"tabladescuento\" colWidths=\"12cm, 2cm, 3cm\" repeatRows=\"1\">\n";
        fitxersortidatxt += "<tr>\n";
        fitxersortidatxt += "        <td>" + _ ( "Descuento" ) + "</td>\n";
        fitxersortidatxt += "        <td>" + _ ( "Porcentaje" ) + "</td>\n";
        fitxersortidatxt += "        <td>" + _ ( "Total" ) + "</td>\n";
        fitxersortidatxt += "</tr>\n";
        for ( int i = 0; i < m_listadescuentos->rowCount(); ++i ) {
            linea1 = m_listadescuentos->lineaat ( i );
            porcentt = porcentt + BlFixed ( linea1->dbValue ( "proporciondfacturap" ).toAscii().constData() );
            fitxersortidatxt += "<tr>\n";
            fitxersortidatxt += "        <td>" + linea1->dbValue ( "conceptdfacturap" ) + "</td>\n";
            fitxersortidatxt += "        <td>" + linea1->dbValue ( "proporciondfacturap" ) + " %</td>\n";
            fitxersortidatxt += "        <td>" + l.sprintf ( "-%s", ( BlFixed ( linea1->dbValue ( "proporciondfacturap" ) ) * basei / 100 ).toQString().toAscii().constData() ) + "</td>\n";
            fitxersortidatxt += "</tr>";
        } // end for
        fitxersortidatxt += "</blockTable>\n";
    } // end if
    buff.replace ( "[descuentos]", fitxersortidatxt );

    /// Impresion de los totales.
    fitxersortidatxt = "";
    QString tr1 = "";   /// Rellena el primer tr de titulares.
    QString tr2 = "";   /// Rellena el segundo tr de cantidades.
    fitxersortidatxt += "<blockTable style=\"tablatotales\">\n";

    BlFixed totbaseimp ( "0.00" );
    BlFixed parbaseimp ( "0.00" );
    for ( it = basesimp.begin(); it != basesimp.end(); ++it ) {
        if ( porcentt > 0 ) {
            parbaseimp = it.value() - it.value() * porcentt / 100;
        } else {
            parbaseimp = it.value();
        } // end if
        totbaseimp = totbaseimp + parbaseimp;
        tr1 += "        <td>" + _ ( "Base " ) + it.key() + " %</td>\n";
        tr2 += "        <td>" + l.sprintf ( "%s", parbaseimp.toQString().toAscii().constData() ) + "</td>\n";
    } // end for

    BlFixed totiva ( "0.0" );
    BlFixed pariva ( "0.0" );
    for ( it = basesimp.begin(); it != basesimp.end(); ++it ) {
        if ( porcentt > 0 ) {
            pariva = ( it.value() - it.value() * porcentt / 100 ) * BlFixed ( it.key() ) / 100;
        } else {
            pariva = it.value() * BlFixed ( it.key() ) / 100;
        } // end if
        totiva = totiva + pariva;
        tr1 += "        <td>" + _ ( "IVA " ) + it.key() + " %</td>\n";
        tr2 += "        <td>" + l.sprintf ( "%s", pariva.toQString().toAscii().constData() ) + "</td>\n";
    } // end for
    tr1 += "        <td>" + _ ( "Total " ) + "</td>\n";
    tr2 += "        <td>" + l.sprintf ( "%s", ( totiva + totbaseimp ).toQString().toAscii().constData() ) + "</td>\n";
    fitxersortidatxt += "<tr>" + tr1 + "</tr><tr>" + tr2 + "</tr></blockTable>\n";
    buff.replace ( "[totales]", fitxersortidatxt );

    if ( file.open ( QIODevice::WriteOnly ) ) {
        QTextStream stream ( &file ) ;
        stream << buff;
        file.close();
    } // end if
    blCreateAndLoadPDF ( "facturap" );
}



