/***************************************************************************
 *   Copyright (C) 2004 by Tomeu Borras Riera                              *
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

#include <QMessageBox>
#include <QWidget>
#include <QObject>
#include <QComboBox>
#include <QToolButton>
#include <QFile>
#include <QTextStream>
#include <QLayout>
#include <QMessageBox>
#include <QCloseEvent>

#include "blfunctions.h"
#include "listlinpresupuestoview.h"
#include "presupuestoview.h"
#include "bfcompany.h"
#include "clientslist.h"
#include "blconfiguration.h"
#include "bfbuscarformapago.h"
#include "bfinformereferencia.h"
#include "blplugins.h"


///
/**
\param comp
\param parent
\return
**/
PresupuestoView::PresupuestoView ( BfCompany *comp, QWidget *parent )
        : BfForm ( comp, parent )
{
    BL_FUNC_DEBUG
    setAttribute ( Qt::WA_DeleteOnClose );
    try {
        setupUi ( this );

        setTitleName ( _ ( "Presupuesto" ) );
        setDbTableName ( "presupuesto" );
        setDbFieldId ( "idpresupuesto" );
        addDbField ( "idpresupuesto", BlDbField::DbInt, BlDbField::DbPrimaryKey, _ ( "ID presupuesto" ) );
        addDbField ( "idcliente", BlDbField::DbInt, BlDbField::DbNotNull, _ ( "Cliente" ) );
        addDbField ( "idalmacen", BlDbField::DbInt, BlDbField::DbNotNull, _ ( "Almacen" ) );
        addDbField ( "numpresupuesto", BlDbField::DbInt, BlDbField::DbNothing, _ ( "Numero de presupuesto" ) );
        addDbField ( "fpresupuesto", BlDbField::DbDate, BlDbField::DbNothing, _ ( "Fecha de creacion" ) );
        addDbField ( "vencpresupuesto", BlDbField::DbDate, BlDbField::DbNothing, _ ( "Fecha de vencimiento" ) );
        addDbField ( "contactpresupuesto", BlDbField::DbVarChar, BlDbField::DbNothing, _ ( "Persona de contacto" ) );
        addDbField ( "telpresupuesto", BlDbField::DbVarChar, BlDbField::DbNothing, _ ( "Numero de telefono" ) );
        addDbField ( "comentpresupuesto", BlDbField::DbVarChar, BlDbField::DbNothing, _ ( "Comentarios" ) );
        addDbField ( "procesadopresupuesto", BlDbField::DbBoolean, BlDbField::DbNothing, _ ( "Procesado" ) );
        addDbField ( "descpresupuesto", BlDbField::DbVarChar, BlDbField::DbNothing, _ ( "% Descuento" ) );
        addDbField ( "refpresupuesto", BlDbField::DbVarChar, BlDbField::DbNothing, _ ( "Referencia" ) );
        addDbField ( "idforma_pago", BlDbField::DbInt, BlDbField::DbNotNull, _ ( "Forma de pago" ) );
        addDbField ( "idtrabajador", BlDbField::DbInt, BlDbField::DbNothing, _ ( "Trabajador" ) );

        /// Disparamos los plugins.
        int res = g_plugins->run ( "PresupuestoView_PresupuestoView", this );
        if ( res != 0 ) {
            return;
        } // end if

        /// Usurpamos la identidad de mlist y ponemos nuestro propio widget con sus cosillas.
        subform2->setMainCompany ( comp );
        m_descuentos->setMainCompany ( comp );
        mui_idcliente->setMainCompany ( comp );
        mui_idforma_pago->setMainCompany ( comp );
        mui_idalmacen->setMainCompany ( comp );
        mui_idtrabajador->setMainCompany ( comp );
        mui_refpresupuesto->setMainCompany ( comp );

        setListaLineas ( subform2 );
        setListaDescuentos ( m_descuentos );

        /// Inicializamos para que no se queden sin ser pintada.
        mui_idforma_pago->setFieldValue ( "0" );
        mui_idalmacen->setFieldValue ( "0" );
        mui_idtrabajador->setFieldValue ( "0" );
        /// Establecemos los parametros de busqueda del Cliente
        mui_idcliente->setLabel ( _ ( "Cliente:" ) );
        mui_idcliente->setTableName ( "cliente" );
        mui_idcliente->m_valores["cifcliente"] = "";
        mui_idcliente->m_valores["nomcliente"] = "";

        insertWindow ( windowTitle(), this, FALSE );
        /// Disparamos los plugins por flanco descendente.
        g_plugins->run ( "PresupuestoView_PresupuestoView_Post", this );
	blScript(this);
    } catch ( ... ) {
        blMsgInfo ( _ ( "Error al crear el presupuesto" ), this );
    } // end try
    
}


/// Este metodo es llamado cuando hacemos un nuevo registro, pero no hay carga desde la base de datos.
/**
**/
void PresupuestoView::inicializar()
{
    BL_FUNC_DEBUG
    subform2->inicializar();
    m_descuentos->inicializar();
    dialogChanges_readValues();
    
}


///
/**
**/
PresupuestoView::~PresupuestoView()
{
    BL_FUNC_DEBUG
    /// Disparamos los plugins.
    g_plugins->run ( "PresupuestoView_DesPresupuestoView", this );
    
}


///
/**
**/
void PresupuestoView::s_pintaTotales()
{
    BL_FUNC_DEBUG
    calculaypintatotales();
    
}



///
/**
**/
void PresupuestoView::on_m_descuentos_editFinish ( int, int )
{
    BL_FUNC_DEBUG
    calculaypintatotales();
    
}


///
/**
**/
void PresupuestoView::on_subform2_editFinish ( int, int )
{
    BL_FUNC_DEBUG
    calculaypintatotales();
    
}


///
/**
\param iva
\param base
\param total
\param desc
\param irpf
\param reqeq
**/
void PresupuestoView::pintatotales ( BlFixed iva, BlFixed base, BlFixed total, BlFixed desc, BlFixed irpf, BlFixed reqeq )
{
    BL_FUNC_DEBUG
    m_totalBases->setText ( QString ( base.toQString() ) );
    m_totalTaxes->setText ( QString ( iva.toQString() ) );
    m_totalPresupuestoCliente->setText ( QString ( total.toQString() ) );
    m_totalDiscounts->setText ( QString ( desc.toQString() ) );
    m_totalIRPF->setText ( QString ( irpf.toQString() ) );
    m_totalReqEq->setText ( QString ( reqeq.toQString() ) );
    
}



///
/**
\param id
**/
void PresupuestoView::on_mui_idcliente_valueChanged ( QString id )
{
    BL_FUNC_DEBUG
    subform2->setIdCliente ( id );
    mui_idforma_pago->setIdCliente ( id );
    
}


///
/**
\param id
**/
void PresupuestoView::on_mui_idalmacen_valueChanged ( QString id )
{
    BL_FUNC_DEBUG
    m_listalineas->setIdAlmacen ( id );
    
}


///
/**
\return
**/
int PresupuestoView::beforeDelete()
{
    BL_FUNC_DEBUG
    /// Disparamos los plugins.
    g_plugins->run ( "Presupuesto_beforeDelete", this );
    m_listalineas->remove();
    m_listadescuentos->remove();

    
    return 0;
}


/// Esta funcion carga un Presupuesto.
/**
\param idbudget
\return
**/
int PresupuestoView::cargarPost ( QString idbudget )
{
    BL_FUNC_DEBUG

    m_listalineas->load ( idbudget );
    m_listadescuentos->load ( idbudget );

    calculaypintatotales();

    
    return 0;
}


///
/**
\return
**/
int PresupuestoView::afterSave()
{
    BL_FUNC_DEBUG

    m_listalineas->setColumnValue ( "idpresupuesto", dbValue ( "idpresupuesto" ) );
    m_listadescuentos->setColumnValue ( "idpresupuesto", dbValue ( "idpresupuesto" ) );

    m_listalineas->save();
    m_listadescuentos->save();
    /// Disparamos los plugins.
    g_plugins->run ( "Presupuesto_afterSave_Post", this );

    
    return 0;
}


///
/**
\return
**/
QString PresupuestoView::detalleArticulos()
{
    BL_FUNC_DEBUG
    QString texto = "";
    BlDbRecordSet *cur = mainCompany() ->loadQuery ( "SELECT * FROM lpresupuesto LEFT JOIN articulo ON lpresupuesto.idarticulo = articulo.idarticulo WHERE presentablearticulo AND idpresupuesto=" + dbValue ( "idpresupuesto" ) );
    int i = 0;
    while ( !cur->eof() ) {
        i = !i;
        if ( i ) {
            texto += "<blockTable style=\"tabladetalle1\" colWidths=\"5cm, 8cm\" rowHeights=\"5.5cm\">\n";
        } else {
            texto += "<blockTable style=\"tabladetalle2\" colWidths=\"8cm, 5cm\" rowHeights=\"5.5cm\">\n";
        } // end if
        texto += "<tr>\n";
        if ( i ) {
            texto += "<td><h1>" + cur->value( "nomarticulo" ) + "</h1>";
            texto += "<para><pre>" + cur->value( "obserarticulo" ) + "</pre></para></td>\n";
        } // end if
        QString file = g_confpr->value( CONF_DIR_IMG_ARTICLES ) + cur->value( "codigocompletoarticulo" ) + ".jpg";
        QFile f ( file );
        if ( f.exists() ) {
            texto += "    <td><illustration x=\"0\" y=\"0\" height=\"5cm\">\n"
                     "        <image file=\"" + g_confpr->value( CONF_DIR_IMG_ARTICLES ) + cur->value( "codigocompletoarticulo" ) + ".jpg\" x=\"0\" y=\"0\" height=\"5cm\"/>\n"
                     "        </illustration></td>\n";
        } else {
            texto += "<td></td>\n";
        } // end if
        if ( !i ) {
            texto += "<td><h1>" + cur->value( "nomarticulo" ) + "</h1>";
            texto += "<para><pre>" + cur->value( "obserarticulo" ) + "</pre></para></td>\n";
        } // end if
        texto += "</tr>\n";
        texto += "</blockTable>";
        cur->nextRecord();
    } // end while
    delete cur;
    
    return texto;
}




/// Se encarga de generar una factura a partir de un albar&aacute;n.
/** Primero de todo busca una factura por referencia que tenga este albaran.
    Si dicha factura existe entonces la cargamos y terminamos.
    Si no existe dicha factura el sistema avisa y permite crear una poniendo
    Todos los datos del albaran automaticamente en ella.
*/
/**
\return
**/
void PresupuestoView::on_mui_duplicar_released()
{
    BL_FUNC_DEBUG
    PresupuestoView *fpv = this ;


    PresupuestoView *bud = NULL;
    BlDbRecordSet *cur = NULL;

    try {
        /// Comprueba si disponemos de los datos m&iacute;nimos. Si no se hace esta
        /// comprobaci&oacute;n la consulta a la base de datos ser&aacute; erronea y al hacer
        /// el siguiente cur->eof() el programa fallar&aacute;.
        /// Comprobamos que existe una factura con esos datos, y en caso afirmativo lo mostramos.

        /// Creamos la factura.
        PresupuestoView *bud = new PresupuestoView ( ( BfCompany * ) fpv->mainCompany(), 0 );
        fpv->mainCompany() ->m_pWorkspace->addSubWindow ( bud );
	bud->inicializar();

        bud->setDbValue ( "comentpresupuesto", fpv->dbValue ( "comentpresupuesto" ) );
        bud->setDbValue ( "idforma_pago", fpv->dbValue ( "idforma_pago" ) );
        bud->setDbValue ( "refpresupuesto", fpv->dbValue ( "refpresupuesto" ) );
        bud->setDbValue ( "idcliente", fpv->dbValue ( "idcliente" ) );
        bud->pintar();
        bud->show();

        /// Traspasamos las lineas de factura
        QString l;
        BlDbSubFormRecord *linea, *linea1;
        for ( int i = 0; i < fpv->m_listalineas->rowCount(); ++i ) {
            linea = fpv->m_listalineas->lineaat ( i );
            if ( linea->dbValue ( "idarticulo" ) != "" ) {
                linea1 = bud->getlistalineas() ->lineaat ( bud->getlistalineas() ->rowCount() - 1 );
                bud->getlistalineas() ->newRecord();
                bud->getlistalineas() ->setProcesarCambios ( FALSE );
                linea1->setDbValue ( "desclpresupuesto", linea->dbValue ( "desclpresupuesto" ) );
                linea1->setDbValue ( "cantlpresupuesto", linea->dbValue ( "cantlpresupuesto" ) );
                linea1->setDbValue ( "pvplpresupuesto", linea->dbValue ( "pvplpresupuesto" ) );
                linea1->setDbValue ( "descuentolpresupuesto", linea->dbValue ( "descuentolpresupuesto" ) );
                linea1->setDbValue ( "idarticulo", linea->dbValue ( "idarticulo" ) );
                linea1->setDbValue ( "codigocompletoarticulo", linea->dbValue ( "codigocompletoarticulo" ) );
                linea1->setDbValue ( "nomarticulo", linea->dbValue ( "nomarticulo" ) );
                linea1->setDbValue ( "ivalpresupuesto", linea->dbValue ( "ivalpresupuesto" ) );
                linea1->setDbValue ( "reqeqlpresupuesto", linea->dbValue ( "reqeqlpresupuesto" ) );
            } // end if
        } // end for
        bud->calculaypintatotales();

    } catch ( ... ) {
        blMsgInfo ( _ ( "Error inesperado" ), this );
        if ( cur ) delete cur;
        if ( bud ) delete bud;
    } // end try

}


