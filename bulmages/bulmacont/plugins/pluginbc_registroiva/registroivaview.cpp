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
#include <fstream>

#include <blplugins.h>

#include "listivaview.h"
#include "registroivaview.h"
#include "bccompany.h"
#include "blfunctions.h"


///
/**
\param comp
\param parent
\return
**/
RegistroIvaView::RegistroIvaView ( BcCompany *comp, QWidget *parent )
        :  RegistroIva ( comp, parent )
{
    BL_FUNC_DEBUG
    setAttribute ( Qt::WA_DeleteOnClose );
    setupUi ( this );
    /// Disparamos los plugins con presupuesto_imprimirPresupuesto
    int res = g_plugins->run ( "RegistroIvaView_RegistroIvaView", this );
    if ( res != 0 )
        return;

    /// Usurpamos la identidad de mlist y ponemos nuestro propio widget con sus cosillas.
    m_contrapartida->setMainCompany ( comp );
    m_contrapartida->setLabel ( _ ( "Cuenta Cliente/Proveedor:" ) );
    m_contrapartida->setTableName ( "cuenta" );
    m_contrapartida->setFieldId("idcuenta");
    m_contrapartida->m_valores["descripcion"] = "";
    m_contrapartida->m_valores["codigo"] = "";

    setLineas ( mui_listIva );
    mui_listIva->setMainCompany ( comp );
    mui_listPrevCobro->setMainCompany ( comp );
    /// Preparamos la lista de cobros y pagos.
    m_cursorFPago = NULL;
    cargarComboFPago ( "NULL" );

    mui_listIva->setDbTableName ( "iva" );
    mui_listIva->setDbFieldId ( "idiva" );
    mui_listIva->addSubFormHeader ( "idiva", BlDbField::DbInt, BlDbField::DbPrimaryKey, BlSubFormHeader::DbNoWrite, _ ( "idiva" ) );
    mui_listIva->addSubFormHeader ( "idtipoiva", BlDbField::DbInt, BlDbField::DbNotNull, BlSubFormHeader::DbNoWrite, _ ( "idtipoiva" ) );
    mui_listIva->addSubFormHeader ( "idcuenta", BlDbField::DbInt, BlDbField::DbNoSave, BlSubFormHeader::DbNoWrite, _ ( "idcuenta" ) );
    mui_listIva->addSubFormHeader ( "codigo", BlDbField::DbVarChar, BlDbField::DbNoSave, BlSubFormHeader::DbNoWrite | BlSubFormHeader::DbHideView, _ ( "codigo" ) );
    mui_listIva->addSubFormHeader ( "tipocuenta", BlDbField::DbVarChar, BlDbField::DbNoSave, BlSubFormHeader::DbNoWrite | BlSubFormHeader::DbHideView, _ ( "tipocuenta" ) );
    mui_listIva->addSubFormHeader ( "nombretipoiva", BlDbField::DbVarChar, BlDbField::DbNoSave, BlSubFormHeader::DbNoWrite, _ ( "nombretipoiva" ) );
    mui_listIva->addSubFormHeader ( "idregistroiva", BlDbField::DbInt, BlDbField::DbNothing, BlSubFormHeader::DbNone, _ ( "idregistroiva" ) );
    mui_listIva->addSubFormHeader ( "baseiva", BlDbField::DbNumeric, BlDbField::DbNothing, BlSubFormHeader::DbNone, _ ( "baseiva" ) );
    mui_listIva->addSubFormHeader ( "ivaiva", BlDbField::DbNumeric, BlDbField::DbNothing, BlSubFormHeader::DbNone, _ ( "ivaiva" ) );
    mui_listIva->setInsert ( FALSE );

    mui_listPrevCobro->setDbTableName ( "prevcobro" );
    mui_listPrevCobro->setDbFieldId ( "idprevcobro" );
    mui_listPrevCobro->addSubFormHeader ( "idprevcobro", BlDbField::DbInt, BlDbField::DbPrimaryKey, BlSubFormHeader::DbNoWrite, _ ( "idprevcobro" ) );
    mui_listPrevCobro->addSubFormHeader ( "fprevistaprevcobro", BlDbField::DbDate, BlDbField::DbNotNull, BlSubFormHeader::DbNone, _ ( "fprevistaprevcobro" ) );
    mui_listPrevCobro->addSubFormHeader ( "fcobroprevcobro", BlDbField::DbDate, BlDbField::DbNoSave, BlSubFormHeader::DbNone, _ ( "fcobroprevcobro" ) );
    mui_listPrevCobro->addSubFormHeader ( "idfpago", BlDbField::DbInt, BlDbField::DbNoSave, BlSubFormHeader::DbNoWrite | BlSubFormHeader::DbHideView, _ ( "idfpago" ) );
    mui_listPrevCobro->addSubFormHeader ( "idcuenta", BlDbField::DbInt, BlDbField::DbNoSave, BlSubFormHeader::DbNoWrite, _ ( "idcuenta" ) );
    mui_listPrevCobro->addSubFormHeader ( "idasiento", BlDbField::DbInt, BlDbField::DbNothing, BlSubFormHeader::DbNone, _ ( "idasiento" ) );
    mui_listPrevCobro->addSubFormHeader ( "cantidadprevistaprevcobro", BlDbField::DbNumeric, BlDbField::DbNothing, BlSubFormHeader::DbNone, _ ( "cantidadprevistaprevcobro" ) );
    mui_listPrevCobro->addSubFormHeader ( "cantidadprevcobro", BlDbField::DbNumeric, BlDbField::DbNothing, BlSubFormHeader::DbNone, _ ( "cantidadprevcobro" ) );
    mui_listPrevCobro->addSubFormHeader ( "idregistroiva", BlDbField::DbInt, BlDbField::DbNothing, BlSubFormHeader::DbNone, _ ( "idregistroiva" ) );
    mui_listPrevCobro->addSubFormHeader ( "tipoprevcobro", BlDbField::DbInt, BlDbField::DbNothing, BlSubFormHeader::DbNone, _ ( "tipoprevcobro" ) );
    mui_listPrevCobro->addSubFormHeader ( "docprevcobro", BlDbField::DbVarChar, BlDbField::DbNothing, BlSubFormHeader::DbNone, _ ( "docprevcobro" ) );
    mui_listPrevCobro->addSubFormHeader ( "codigo", BlDbField::DbVarChar, BlDbField::DbNoSave, BlSubFormHeader::DbNone, _ ( "codigo" ) );
    mui_listIva->addSubFormHeader ( "tipocuenta", BlDbField::DbVarChar, BlDbField::DbNoSave, BlSubFormHeader::DbNoWrite | BlSubFormHeader::DbHideView, _ ( "tipocuenta" ) );
    mui_listPrevCobro->addSubFormHeader ( "descripcion", BlDbField::DbNumeric, BlDbField::DbNoSave, BlSubFormHeader::DbNone, _ ( "descripcion" ) );
    mui_listPrevCobro->addSubFormHeader ( "idctacliente", BlDbField::DbNumeric, BlDbField::DbNothing, BlSubFormHeader::DbNone, _ ( "idctacliente" ) );
    mui_listPrevCobro->addSubFormHeader ( "codigoctacliente", BlDbField::DbNumeric, BlDbField::DbNoSave, BlSubFormHeader::DbNone, _ ( "codigoctacliente" ) );
    mui_listPrevCobro->addSubFormHeader ( "nomctacliente", BlDbField::DbNumeric, BlDbField::DbNoSave, BlSubFormHeader::DbNone, _ ( "nomctacliente" ) );
    mui_listPrevCobro->addSubFormHeader ( "tipoctacliente", BlDbField::DbNumeric, BlDbField::DbNoSave, BlSubFormHeader::DbNone, _ ( "tipoctacliente" ) );
    mui_listPrevCobro->addSubFormHeader ( "tipocuenta", BlDbField::DbNumeric, BlDbField::DbNoSave, BlSubFormHeader::DbNone, _ ( "tipocuenta" ) );

    mui_listPrevCobro->setInsert ( TRUE );
    mainCompany() ->insertWindow ( windowTitle(), this );
    g_plugins->run ( "RegistroIvaView_RegistroIvaView_Post", this );
    
}


///
/**
**/
RegistroIvaView::~RegistroIvaView()
{
    BL_FUNC_DEBUG
    
}


/// Carga el combo de las formas de pago.
/// Esta funci&oacute;n se encarga de cargar la tabla de formas de pago en el combobox
/// correspondiente.
/// Usa un cursor de clase (m_cursorFPago) que es construido al usar esta funci&oacute;
/// y destruido en el destructor de clase.
/// Esta funci&oacute;n se llama con la inicializaci&oacute;n de clase y cuando se quieren
/// cargar datos.
/**
\param idfpago
**/
void RegistroIvaView::cargarComboFPago ( QString idfpago )
{
    BL_FUNC_DEBUG
    if ( m_cursorFPago != NULL )
        delete m_cursorFPago;
    m_cursorFPago = mainCompany() ->loadQuery ( "SELECT * FROM fpago" );
    int i = 0;
    int i1 = 0;
    while ( !m_cursorFPago->eof() ) {
        i ++;
        if ( m_cursorFPago->value( "idfpago" ) == idfpago ) {
            i1 = i;
        } // end if
        m_fPago->addItem ( m_cursorFPago->value( "nomfpago" ) );
        m_cursorFPago->nextRecord();
    } // end while
    if ( i1 != 0 ) {
        m_fPago->setCurrentIndex ( i1 - 1 );
    } // end if
    
}

/*
void RegistroIvaView::on_mui_borrar_clicked() {
    BL_FUNC_DEBUG
    if (QMessageBox::warning(this,
                             _("BulmaCont - Registro Factura"),
                             _("Desea borrar este registro"),
                             _("&Si"),
                             _("&No")) == 0) {
        mui_listIva->remove();
        remove();
    } // end if
}
*/


///
/**
\param id
\return
**/
int RegistroIvaView::load ( QString id )
{
    int error = 0;
    error = RegistroIva::load ( id );
    if ( error ) {
        return -1;
    } // end if
    mui_listIva->load ( "SELECT * FROM  tipoiva LEFT JOIN (SELECT * FROM iva WHERE idregistroiva=" + id + " ) AS t1 ON t1.idtipoiva = tipoiva.idtipoiva LEFT JOIN cuenta on tipoiva.idcuenta = cuenta.idcuenta  ORDER BY codigo" );

    mui_listPrevCobro->load ( "SELECT * FROM prevcobro "
                                " LEFT JOIN cuenta ON cuenta.idcuenta = prevcobro.idcuenta "
                                " LEFT JOIN (SELECT idcuenta AS idctacliente, codigo AS codigoctacliente, descripcion AS nomctacliente, tipocuenta AS tipoctacliente FROM cuenta) AS T1 ON t1.idctacliente = prevcobro.idctacliente "
                                " WHERE idregistroiva = " + id + " ORDER BY fcobroprevcobro " );

    setWindowTitle ( _ ( "Registro factura" ) + " " + factura() );
    dialogChanges_readValues();
    return 0;
}


///
/**
\return
**/
int RegistroIvaView::save()
{
    BL_FUNC_DEBUG
    try {
        mainCompany() ->begin();
        setcontrapartida ( m_contrapartida->id() );
        setbaseimp ( m_baseImponible->text() );
        setiva ( m_importeiva->text() );
        setffactura ( m_ffactura->text() );
        setfactura ( m_factura->text() );
        setincregistro ( m_incregistroIVA->isChecked() ? "t" : "f" );
        setnumorden ( m_numorden->text() );
        setcif ( m_cif->text() );
        setfactemitida ( m_factEmitida->isChecked() ? "t" : "f" );
        setfemisionregistroiva ( m_femisionregistroiva->text() );
        setserieregistroiva ( m_serieregistroiva->text() );
        RegistroIva::save();
        mui_listIva->setColumnValue ( "idregistroiva", dbValue ( "idregistroiva" ) );
        mui_listIva->save();
        mui_listPrevCobro->setColumnValue ( "idregistroiva", dbValue ( "idregistroiva" ) );
        mui_listPrevCobro->save();
        mainCompany() ->commit();
        dialogChanges_readValues();
    } catch ( ... ) {
        blMsgInfo ( "Error al guardar el Registro de IVA" );
        mainCompany() ->rollback();
    } // end try
    
    return 0;
}



/// SLOT que se activa al pulsar sobre el bot&oacutye;n de generar previsiones.
/** 1.- Vacia la lista de Prevision de Cobros
    2.- Calcula la fecha inicial a partir de la fecha de factura y la forma de pago.
    3.- Itera para cada plazo en la forma de pago calculando el nuevo plazo.
    Falta usar las cuentas de servicio para saber si es cobro o pago. */
/**
**/
void RegistroIvaView::on_mui_generarPrevisiones_clicked()
{
    BL_FUNC_DEBUG
    QString snumpagos = m_cursorFPago->value( "nplazosfpago", m_fPago->currentIndex() );
    QString splazoprimerpago = m_cursorFPago->value( "plazoprimerpagofpago", m_fPago->currentIndex() );
    QString splazoentrerecibo = m_cursorFPago->value( "plazoentrerecibofpago", m_fPago->currentIndex() );
    BlFixed totalfactura = BlFixed ( baseimp() ) + BlFixed ( iva() );
    int plazoentrerecibo = splazoentrerecibo.toInt();
    int plazoprimerpago = splazoprimerpago.toInt();
    int numpagos = snumpagos.toInt();
    BlFixed totalplazo = totalfactura / numpagos;
    QString tipocobro;
    /// Vaciamos la lista de previsi&oacute;n para que no haga cosas raras.
    QDate ffactura = blNormalizeDate ( m_ffactura->text() );
    QDate fpcobro = ffactura.addDays ( plazoprimerpago );
    for ( int i = 0; i < numpagos; i++ ) {
        /// Hay que saber si es un cobro o un pago.
        if ( contrapartida().left ( 2 ) == "43" ) { /// Si es un cliente es un cobro, si es un proveedor es un pago.
            tipocobro = "t";
        } else {
            tipocobro = "f";
        } // end if
	BlDebug::blDebug ( Q_FUNC_INFO, 0, _("Cogemos el registro.") );
        BlDbSubFormRecord *rec = mui_listPrevCobro->lineaat ( mui_listPrevCobro->rowCount() - 1 );
        rec->setDbValue ( "fprevistaprevcobro", fpcobro.toString ( "dd/MM/yyyy" ) );
        rec->setDbValue ( "cantidadprevistaprevcobro", totalplazo.toQString() );
        rec->setDbValue ( "tipoprevcobro", tipocobro );
        rec->setDbValue ( "codigoctacliente", m_contrapartida->fieldValue("codigo") );
        rec->setDbValue ( "idctacliente", m_contrapartida->id() );
        rec->setDbValue ( "nomctacliente", m_contrapartida->fieldValue("descripcion") );
        fpcobro = fpcobro.addDays ( plazoentrerecibo );
        mui_listPrevCobro->newRecord();
    } // end for
    
}
