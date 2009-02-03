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

#include <plugins.h>

#include "listivaview.h"
#include "registroivaview.h"
#include "empresa.h"
#include "funcaux.h"


///
/**
\param comp
\param parent
\return
**/
RegistroIvaView::RegistroIvaView ( Empresa *comp, QWidget *parent )
        :  RegistroIva ( comp, parent )
{
    setAttribute ( Qt::WA_DeleteOnClose );
    setupUi ( this );
    _depura ( "Inicializacion de RegistroIvaView", 0 );
    /// Disparamos los plugins con presupuesto_imprimirPresupuesto
    int res = g_plugins->lanza ( "RegistroIvaView_RegistroIvaView", this );
    if ( res != 0 )
        return;

    /// Usurpamos la identidad de mlist y ponemos nuestro propio widget con sus cosillas.
    m_contrapartida->setEmpresaBase ( comp );
    setLineas ( mui_listIva );
    mui_listIva->setEmpresaBase ( comp );
    mui_listPrevCobro->setEmpresaBase ( comp );
    /// Preparamos la lista de cobros y pagos.
    m_cursorFPago = NULL;
    cargarComboFPago ( "NULL" );

    mui_listIva->setDBTableName ( "iva" );
    mui_listIva->setDBCampoId ( "idiva" );
    mui_listIva->addSHeader ( "idiva", DBCampo::DBint, DBCampo::DBPrimaryKey, SHeader::DBNoWrite, _( "idiva" ) );
    mui_listIva->addSHeader ( "idtipoiva", DBCampo::DBint, DBCampo::DBNotNull, SHeader::DBNoWrite, _( "idtipoiva" ) );
    mui_listIva->addSHeader ( "idcuenta", DBCampo::DBint, DBCampo::DBNoSave, SHeader::DBNoWrite, _( "idcuenta" ) );
    mui_listIva->addSHeader ( "codigo", DBCampo::DBvarchar, DBCampo::DBNoSave, SHeader::DBNoWrite | SHeader::DBNoView, _( "codigo" ) );
    mui_listIva->addSHeader ( "tipocuenta", DBCampo::DBvarchar, DBCampo::DBNoSave, SHeader::DBNoWrite | SHeader::DBNoView, _( "tipocuenta" ) );
    mui_listIva->addSHeader ( "nombretipoiva", DBCampo::DBvarchar, DBCampo::DBNoSave, SHeader::DBNoWrite, _( "nombretipoiva" ) );
    mui_listIva->addSHeader ( "idregistroiva", DBCampo::DBint, DBCampo::DBNothing, SHeader::DBNone, _( "idregistroiva" ) );
    mui_listIva->addSHeader ( "baseiva", DBCampo::DBnumeric, DBCampo::DBNothing, SHeader::DBNone, _( "baseiva" ) );
    mui_listIva->addSHeader ( "ivaiva", DBCampo::DBnumeric, DBCampo::DBNothing, SHeader::DBNone, _( "ivaiva" ) );
    mui_listIva->setinsercion ( FALSE );

    mui_listPrevCobro->setDBTableName ( "prevcobro" );
    mui_listPrevCobro->setDBCampoId ( "idprevcobro" );
    mui_listPrevCobro->addSHeader ( "idprevcobro", DBCampo::DBint, DBCampo::DBPrimaryKey, SHeader::DBNoWrite, _( "idprevcobro" ) );
    mui_listPrevCobro->addSHeader ( "fprevistaprevcobro", DBCampo::DBdate, DBCampo::DBNotNull, SHeader::DBNone, _( "fprevistaprevcobro" ) );
    mui_listPrevCobro->addSHeader ( "fcobroprevcobro", DBCampo::DBdate, DBCampo::DBNoSave, SHeader::DBNone, _( "fcobroprevcobro" ) );
    mui_listPrevCobro->addSHeader ( "idfpago", DBCampo::DBint, DBCampo::DBNoSave, SHeader::DBNoWrite | SHeader::DBNoView, _( "idfpago" ) );
    mui_listPrevCobro->addSHeader ( "idcuenta", DBCampo::DBint, DBCampo::DBNoSave, SHeader::DBNoWrite, _( "idcuenta" ) );
    mui_listPrevCobro->addSHeader ( "idasiento", DBCampo::DBint, DBCampo::DBNothing, SHeader::DBNone, _( "idasiento" ) );
    mui_listPrevCobro->addSHeader ( "cantidadprevistaprevcobro", DBCampo::DBnumeric, DBCampo::DBNothing, SHeader::DBNone, _( "cantidadprevistaprevcobro" ) );
    mui_listPrevCobro->addSHeader ( "cantidadprevcobro", DBCampo::DBnumeric, DBCampo::DBNothing, SHeader::DBNone, _( "cantidadprevcobro" ) );
    mui_listPrevCobro->addSHeader ( "idregistroiva", DBCampo::DBint, DBCampo::DBNothing, SHeader::DBNone, _( "idregistroiva" ) );
    mui_listPrevCobro->addSHeader ( "tipoprevcobro", DBCampo::DBint, DBCampo::DBNothing, SHeader::DBNone, _( "tipoprevcobro" ) );
    mui_listPrevCobro->addSHeader ( "docprevcobro", DBCampo::DBvarchar, DBCampo::DBNothing, SHeader::DBNone, _( "docprevcobro" ) );
    mui_listPrevCobro->addSHeader ( "codigo", DBCampo::DBvarchar, DBCampo::DBNoSave, SHeader::DBNone, _( "codigo" ) );
    mui_listIva->addSHeader ( "tipocuenta", DBCampo::DBvarchar, DBCampo::DBNoSave, SHeader::DBNoWrite | SHeader::DBNoView, _( "tipocuenta" ) );
    mui_listPrevCobro->addSHeader ( "descripcion", DBCampo::DBnumeric, DBCampo::DBNoSave, SHeader::DBNone, _( "descripcion" ) );
    mui_listPrevCobro->addSHeader ( "idctacliente", DBCampo::DBnumeric, DBCampo::DBNothing, SHeader::DBNone, _( "idctacliente" ) );
    mui_listPrevCobro->addSHeader ( "codigoctacliente", DBCampo::DBnumeric, DBCampo::DBNoSave, SHeader::DBNone, _( "codigoctacliente" ) );
    mui_listPrevCobro->addSHeader ( "nomctacliente", DBCampo::DBnumeric, DBCampo::DBNoSave, SHeader::DBNone, _( "nomctacliente" ) );
    mui_listPrevCobro->addSHeader ( "tipoctacliente", DBCampo::DBnumeric, DBCampo::DBNoSave, SHeader::DBNone, _( "tipoctacliente" ) );
    mui_listPrevCobro->addSHeader ( "tipocuenta", DBCampo::DBnumeric, DBCampo::DBNoSave, SHeader::DBNone, _( "tipocuenta" ) );

    mui_listPrevCobro->setinsercion ( TRUE );
    empresaBase() ->meteWindow ( windowTitle(), this );
    g_plugins->lanza ( "RegistroIvaView_RegistroIvaView_Post", this );
    _depura ( "Fin de la inicializacion de RegistroIvaView", 0 );
}


///
/**
**/
RegistroIvaView::~RegistroIvaView()
{
    _depura ( "RegistroIvaView::~RegistroIvaView", 0 );
    _depura ( "END RegistroIvaView::~RegistroIvaView", 0 );
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
    _depura ( "RegistroIvaView::cargarComboFPago", 0 );
    if ( m_cursorFPago != NULL )
        delete m_cursorFPago;
    m_cursorFPago = empresaBase() ->cargacursor ( "SELECT * FROM fpago" );
    int i = 0;
    int i1 = 0;
    while ( !m_cursorFPago->eof() ) {
        i ++;
        if ( m_cursorFPago->valor ( "idfpago" ) == idfpago ) {
            i1 = i;
        } // end if
        m_fPago->addItem ( m_cursorFPago->valor ( "nomfpago" ) );
        m_cursorFPago->siguienteregistro();
    } // end while
    if ( i1 != 0 ) {
        m_fPago->setCurrentIndex ( i1 - 1 );
    } // end if
    _depura ( "RegistroIvaView::cargarComboFPago", 0 );
}

/*
void RegistroIvaView::on_mui_borrar_clicked() {
    _depura("RegistroIvaView::on_mui_borrar_clicked",0);
    if (QMessageBox::warning(this,
                             _("BulmaCont - Registro Factura"),
                             _("Desea borrar este registro"),
                             _("&Si"),
                             _("&No")) == 0) {
        mui_listIva->borrar();
        borrar();
    } // end if
    _depura("END RegistroIvaView::on_mui_borrar_clicked", 0);
}
*/


///
/**
\param id
\return
**/
int RegistroIvaView::cargar ( QString id )
{
    int error = 0;
    error = RegistroIva::cargar ( id );
    if ( error ) {
        return -1;
    } // end if
    mui_listIva->cargar ( "SELECT * FROM  tipoiva LEFT JOIN (SELECT * FROM iva WHERE idregistroiva=" + id + " ) AS t1 ON t1.idtipoiva = tipoiva.idtipoiva LEFT JOIN cuenta on tipoiva.idcuenta = cuenta.idcuenta  ORDER BY codigo" );

    mui_listPrevCobro->cargar ( "SELECT * FROM prevcobro "
                                " LEFT JOIN cuenta ON cuenta.idcuenta = prevcobro.idcuenta "
                                " LEFT JOIN (SELECT idcuenta AS idctacliente, codigo AS codigoctacliente, descripcion AS nomctacliente, tipocuenta AS tipoctacliente FROM cuenta) AS T1 ON t1.idctacliente = prevcobro.idctacliente "
                                " WHERE idregistroiva = " + id + " ORDER BY fcobroprevcobro " );

    setWindowTitle ( _( "Registro factura" ) + " " + factura() );
    dialogChanges_cargaInicial();
    return 0;
}


///
/**
\return
**/
int RegistroIvaView::guardar()
{
    _depura ( "RegistroIvaView::guardar", 0 );
    try {
        empresaBase() ->begin();
        setcontrapartida ( m_contrapartida->idcuenta() );
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
        RegistroIva::guardar();
        mui_listIva->setColumnValue ( "idregistroiva", DBvalue ( "idregistroiva" ) );
        mui_listIva->guardar();
        mui_listPrevCobro->setColumnValue ( "idregistroiva", DBvalue ( "idregistroiva" ) );
        mui_listPrevCobro->guardar();
        empresaBase() ->commit();
        dialogChanges_cargaInicial();
    } catch ( ... ) {
        mensajeInfo ( "Error al guardar el Registro de IVA" );
        empresaBase() ->rollback();
    } // end try
    _depura ( "END RegistroIvaView::guardar" );
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
    _depura ( "RegistroIvaView::on_mui_generarPrevisiones_clicked", 0 );
    QString snumpagos = m_cursorFPago->valor ( "nplazosfpago", m_fPago->currentIndex() );
    QString splazoprimerpago = m_cursorFPago->valor ( "plazoprimerpagofpago", m_fPago->currentIndex() );
    QString splazoentrerecibo = m_cursorFPago->valor ( "plazoentrerecibofpago", m_fPago->currentIndex() );
    Fixed totalfactura = Fixed ( baseimp() ) + Fixed ( iva() );
    int plazoentrerecibo = splazoentrerecibo.toInt();
    int plazoprimerpago = splazoprimerpago.toInt();
    int numpagos = snumpagos.toInt();
    Fixed totalplazo = totalfactura / numpagos;
    QString tipocobro;
    /// Vaciamos la lista de previsi&oacute;n para que no haga cosas raras.
    QDate ffactura = normalizafecha ( m_ffactura->text() );
    QDate fpcobro = ffactura.addDays ( plazoprimerpago );
    for ( int i = 0; i < numpagos; i++ ) {
        /// Hay que saber si es un cobro o un pago.
        if ( contrapartida().left ( 2 ) == "43" ) { /// Si es un cliente es un cobro, si es un proveedor es un pago.
            tipocobro = "t";
        } else {
            tipocobro = "f";
        } // end if
        _depura ( "Cogemos el registro", 3 );
        SDBRecord *rec = mui_listPrevCobro->lineaat ( mui_listPrevCobro->rowCount() - 1 );
        rec->setDBvalue ( "fprevistaprevcobro", fpcobro.toString ( "dd/MM/yyyy" ) );
        rec->setDBvalue ( "cantidadprevistaprevcobro", totalplazo.toQString() );
        rec->setDBvalue ( "tipoprevcobro", tipocobro );
        rec->setDBvalue ( "codigoctacliente", m_contrapartida->codigocuenta() );
        rec->setDBvalue ( "idctacliente", m_contrapartida->idcuenta() );
        rec->setDBvalue ( "nomctacliente", m_contrapartida->nomcuenta() );
        _depura ( "Pintamos", 3 );
        fpcobro = fpcobro.addDays ( plazoentrerecibo );
        mui_listPrevCobro->nuevoRegistro();
    } // end for
    _depura ( "END RegistroIvaView::on_mui_generarPrevisiones_clicked", 0 );
}

