/***************************************************************************
 *   Copyright (C) 2005 by Alvaro de Miguel                                *
 *   alvaro.demiguel@gmail.com                                             *
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

#include <QCloseEvent>
#include <QComboBox>
#include <QLabel>
#include <QLineEdit>
#include <QMessageBox>
#include <QObject>
#include <QToolButton>
#include <QCheckBox>

#include "bldialogchanges.h"
#include "blfixed.h"
#include "bldatesearch.h"
#include "bfbuscarformapago.h"
#include "bfbuscaralmacen.h"
#include "bfbuscartrabajador.h"
#include "facturaivaincclienteview.h"
#include "clientslist.h"
#include "bfcompany.h"
#include "blconfiguration.h"
#include "blfunctions.h"
#include "bfinformereferencia.h"
#include "bfclientefacturaivaincdescuentosubform.h"
#include "bfclientefacturaivaincsubform.h"
#include "blpostgresqlclient.h"
#include "blplugins.h"


/** Constructor de la clase corresponde a la parte visual de la ficha de cliente.
    Inicializa la ventana y todos sus componentes.
    Mete la ventana en el WorkSpace.
*/
/**
\param comp
\param parent
\return
**/
FacturaIVAIncClienteView::FacturaIVAIncClienteView ( BfCompany *comp, QWidget *parent )
        : BfForm ( comp, parent )
{
    BL_FUNC_DEBUG
    setAttribute ( Qt::WA_DeleteOnClose );
    try {
        setupUi ( this );

        setTitleName ( _ ( "Factura (IVA Inc)" ) );
        setDbTableName ( "factura" );
        setDbFieldId ( "idfactura" );
        addDbField ( "idfactura", BlDbField::DbInt, BlDbField::DbPrimaryKey, _ ( "Id factura" ) );
        addDbField ( "idcliente", BlDbField::DbInt, BlDbField::DbNotNull, _ ( "Cliente" ) );
        addDbField ( "idalmacen", BlDbField::DbInt, BlDbField::DbNotNull, _ ( "Almacen" ) );
        addDbField ( "numfactura", BlDbField::DbInt, BlDbField::DbNothing, _ ( "Numero factura" ) );
        addDbField ( "ticketfactura", BlDbField::DbBoolean, BlDbField::DbNothing, _ ( "Factura de ticket" ) );
        addDbField ( "ffactura", BlDbField::DbDate, BlDbField::DbNothing, _ ( "Ffactura" ) );
        addDbField ( "procesadafactura", BlDbField::DbBoolean, BlDbField::DbNothing, _ ( "Procesada factura" ) );
        addDbField ( "codigoserie_factura", BlDbField::DbVarChar, BlDbField::DbNotNull, _ ( "Serie" ) );
        addDbField ( "comentfactura", BlDbField::DbVarChar, BlDbField::DbNothing, _ ( "Comentario" ) );
        addDbField ( "reffactura", BlDbField::DbVarChar, BlDbField::DbNothing, _ ( "Referencia" ) );
        addDbField ( "descfactura", BlDbField::DbVarChar, BlDbField::DbNothing, _ ( "Descripcion" ) );
        addDbField ( "idtrabajador", BlDbField::DbInt, BlDbField::DbNothing, _ ( "Trabajador" ) );
        addDbField ( "idforma_pago", BlDbField::DbInt, BlDbField::DbNothing, _ ( "Forma de pago" ) );

	setDbValue("ticketfactura", "TRUE");
	
        /// Disparamos los plugins.
        int res = g_plugins->run ( "FacturaIVAIncClienteView_FacturaIVAIncClienteView", this );
        if ( res != 0 )
            return;

        subform2->setMainCompany ( comp );
        m_descuentos->setMainCompany ( comp );
        mui_idalmacen->setMainCompany ( comp );
        mui_idforma_pago->setMainCompany ( comp );
        mui_idcliente->setMainCompany ( comp );
        mui_idtrabajador->setMainCompany ( comp );
        mui_reffactura->setMainCompany ( comp );
        mui_codigoserie_factura->setMainCompany ( comp );
	
        setListaLineas ( subform2 );
        setListaDescuentos ( m_descuentos );


        /// Establecemos los parametros de busqueda del Cliente
        mui_idcliente->setLabel ( _ ( "Cliente:" ) );
        mui_idcliente->setTableName ( "cliente" );
        mui_idcliente->m_valores["cifcliente"] = "";
        mui_idcliente->m_valores["nomcliente"] = "";

        /// Inicializamos para que no se queden sin ser pintada.
        mui_idforma_pago->setFieldValue ( "0" );
        mui_idalmacen->setFieldValue ( "0" );
        mui_codigoserie_factura->setId ( "" );
        mui_idtrabajador->setFieldValue ( "0" );

	setTemplateName("facturaIVAINC");
	
        insertWindow ( windowTitle(), this, FALSE );
        /// Disparamos los plugins por flanco descendente.
        g_plugins->run ( "FacturaIVAIncClienteView_FacturaIVAIncClienteView_Post", this );
	blScript(this);
    } catch ( ... ) {
        blMsgInfo ( _ ( "Error al crear el factura a cliente" ), this );
    } // end try
    
}


/** Destructor de la clase.
    Indica al listado que debe actualizarse.
*/
/// \TODO: Este metodo deberia mejorarse para que indicase al listado que
/// solo debe eliminar una fila del mismo.
/**
**/
FacturaIVAIncClienteView::~FacturaIVAIncClienteView()
{
    BL_FUNC_DEBUG
    
}


/// Inicializa todos los elementos del formulario
/**
**/
void FacturaIVAIncClienteView::inicializar()
{
    BL_FUNC_DEBUG
    subform2->inicializar();
    m_descuentos->inicializar();
    dialogChanges_readValues();
    
}



/// Pinta los totales en las casillas correspondientes
/**
\param iva
\param base
\param total
\param desc
\param irpf
\param reqeq
**/
void FacturaIVAIncClienteView::pintatotales ( BlFixed iva, BlFixed base, BlFixed total, BlFixed desc, BlFixed irpf, BlFixed reqeq )
{
    BL_FUNC_DEBUG
    m_totalBases->setText ( base.toQString() );
    m_totalTaxes->setText ( iva.toQString() );
    m_totalfactura->setText ( total.toQString() );
    m_totalDiscounts->setText ( desc.toQString() );
    m_totalIRPF->setText ( QString ( irpf.toQString() ) );
    m_totalReqEq->setText ( QString ( reqeq.toQString() ) );
    
}




///
/**
\param id
**/
void FacturaIVAIncClienteView::on_mui_idcliente_valueChanged ( QString id )
{
    BL_FUNC_DEBUG
    subform2->setIdCliente ( id );
    mui_idforma_pago->setIdCliente ( id );
    
}






/// Este slot se activa cuando hay cambios en los subformularios.
/**
**/
void FacturaIVAIncClienteView::s_pintaTotales()
{
    BL_FUNC_DEBUG
    calculaypintatotales();
    
}


void FacturaIVAIncClienteView::calculaypintatotales() {
  
  if (!m_listalineas) return;

  
  BlFixed total("0.00");
  BlFixed impuestos("0.00");
  BlFixed bimp("0.00");
  BlFixed porcentt("0.00");
  BlFixed totalreq("0.00");
  BlFixed totalirpf("0.00");
  BlFixed desc("0.00");
  BlDbSubFormRecord *linea, *linea1;
    
  
  
  /// Impresion de los contenidos.
  QString l;
  BlFixed irpf ( "0" );

  /// El calculo del IRPF basandonos en la fecha.
  QString campfecha = "fecha" +  tableName();
  if (!exists(campfecha)) 
    campfecha = "f" + tableName();
  if (exists(campfecha)) {
      QString query = "SELECT tasairpf FROM irpf WHERE fechairpf <= "+QString (dbValue(campfecha)=="" ? "now()" : "'" + dbValue(campfecha) +"'::DATE") + " ORDER BY fechairpf DESC LIMIT 1";
      BlDbRecordSet *cur = mainCompany() ->loadQuery ( query);
      if ( cur ) {
	  if ( !cur->eof() ) {
	      irpf = BlFixed ( cur->value( "tasairpf" ) );
	  } // end if
	  delete cur;
      } // end if
  } // end if
    
    /// Calculamos los descuentos totales
    if ( m_listadescuentos->rowCount() ) {
        for ( int i = 0; i < m_listadescuentos->rowCount(); ++i ) {
            linea1 = m_listadescuentos->lineaat ( i );
            BlFixed propor ( linea1->dbValue ( "proporcion" + m_listadescuentos->tableName() ).toAscii().constData() );
            porcentt = porcentt + propor;
        } // end for
    } // end if
    

   
   for ( int i = 0; i < m_listalineas->rowCount(); ++i ) {
        linea = m_listalineas->lineaat ( i );
        BlFixed cant ( linea->dbValue ( "cantlfactura" ));
        BlFixed pvpund ( linea->dbValue ( "pvpivainclfactura"));
	BlFixed ivalin ( linea->dbValue ( "ivalfactura"));
	BlFixed reqeq ( linea->dbValue ( "reqeqlfactura"));
	BlFixed descl ( linea->dbValue ( "descuentolfactura"));

	descl = (descl + porcentt) / BlFixed("100");
	BlFixed totallinea = cant * pvpund * (BlFixed("1.00") - descl);
	
	BlFixed baselinea = totallinea / (BlFixed("1.00") + ivalin / BlFixed("100"));
	BlFixed ivalinea = totallinea - baselinea;
	BlFixed reqeqlinea = baselinea * (reqeq / BlFixed("100"));
	totalreq = totalreq + reqeqlinea;
	
	
	totallinea.setPrecision(2);
	total = total + totallinea + reqeqlinea;
	
	ivalinea.setPrecision(2);
	impuestos = impuestos + ivalinea;
	
	desc = desc + baselinea * descl;

		
    } // end for
    // Calculamos la base imponible
    bimp = total - impuestos;
    totalirpf = bimp * irpf / BlFixed("100");
    total = total - totalirpf;

    m_totalBases -> setText (bimp.toQString());
    m_totalTaxes->setText ( impuestos.toQString() );
    m_totalfactura->setText ( total.toQString() );
    m_totalDiscounts->setText ( desc.toQString() );
    m_totalIRPF->setText ( QString ( totalirpf.toQString() ) );
    m_totalReqEq->setText ( QString ( totalreq.toQString() ) );
    

  
}



///
/**
**/
void FacturaIVAIncClienteView::on_m_descuentos_editFinish ( int, int )
{
    BL_FUNC_DEBUG
    calculaypintatotales();
    
}


///
/**
**/
void FacturaIVAIncClienteView::on_subform2_editFinish ( int, int )
{
    BL_FUNC_DEBUG
    calculaypintatotales();
    
}



/** Metodo de borrar un factura. Se encarga de mandar a la base de datos
    la instruccion necesaria para el borrado de un factura y controlar
    los posibles errores que se produzcan.
    Tambi&eacute;n borra todas las lineas y las lineas de descuento que se refieren
    a el factura que se pretende borrar.
*/
/// \todo: Este metodo deberia poderse delegar en BlDbRecord, o por lo menos la parte del borrado del registro.
/// \todo: Hace falta meter el metodo dentro del sistema de excepciones try catch.
/**
\return
**/
int FacturaIVAIncClienteView::beforeDelete()
{
    BL_FUNC_DEBUG
    m_listalineas->remove();
    m_listadescuentos->remove();
    
    return 0;
}



/** Este m&eacute;todo carga un FacturaIVAIncCliente. Tambi&eacute;n carga las lineas
    de albar&aacute;n y los descuentos de albar&aacute;n.
    Tras la carga tambi&eacute;n invoca un repintado del factura para que se vea
    correctamente la pantalla.
*/
/**
\param idfactura
\return
**/
int FacturaIVAIncClienteView::cargarPost ( QString idfactura )
{
    BL_FUNC_DEBUG

    m_listalineas->load ( idfactura );
    m_listadescuentos->load ( idfactura );

    /// Disparamos los plugins con presupuesto_imprimirPresupuesto.
    g_plugins->run ( "FacturaIVAIncCliente_cargarPost_Post", this );

    calculaypintatotales();
    
    return 0;
}


/** Este m&eacute;todo se encarga de hacer el guardado del albar&aacute; en la
    base de datos. Una vez guardado se guardan las lineas de albar&aacute;n y los
    descuentos de albar&aacute;n. Tras hacer el guardado se hace una carga del factura
    para recuperar los datos que haya podido escribir la base de datos automaticamente.
    Dichos datos son la referencia y el n&uacute;mero de albar&aacute;n.

    Si todo funciona bien este m&eacute;todo devuelve 0. Si se produce algun error
    se genera una excepcion -1.
*/
/**
\return
**/
int FacturaIVAIncClienteView::afterSave()
{
    BL_FUNC_DEBUG

    m_listalineas->setColumnValue ( "idfactura", dbValue ( "idfactura" ) );
    m_listalineas->save();
    m_listadescuentos->setColumnValue ( "idfactura", dbValue ( "idfactura" ) );
    m_listadescuentos->save();

    
    return 0;
}


/** Pintar una factura.
*/
/**
\param idbudget
\return
**/
void FacturaIVAIncClienteView::pintarPost ( )
{
    BL_FUNC_DEBUG

    /// Escribimos como descripcion el nombre del cliente para que aparezca en el titulo y en el dockwidget
    setDescripcion( mui_idcliente->fieldValue("nomcliente") + "\n" + mui_ffactura->text());
}


///
/**
\param id
**/
void FacturaIVAIncClienteView::on_mui_idalmacen_valueChanged ( QString id )
{
    BL_FUNC_DEBUG
    m_listalineas->setIdAlmacen ( id );
    
}

