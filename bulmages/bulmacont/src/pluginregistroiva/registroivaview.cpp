/***************************************************************************
 *   Copyright (C) 2004 by Tomeu Borr� Riera                              *
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

#include "listivaview.h"
#include "registroivaview.h"
#include "empresa.h"
#include <plugins.h>


#include <QMessageBox>

#include <Q3Table>
#include <QWidget>
#include <QObject>
#include <QComboBox>
#include <Q3PopupMenu>
#include <QToolButton>
#include <QFile>
#include <QTextStream>
#include <QLayout>
#include <QMessageBox>
#include <QCloseEvent>
#include <fstream>
using namespace std;

#include "funcaux.h"
#define coma "'"

RegistroIvaView::RegistroIvaView( empresa *comp , QWidget *parent) : Ficha(parent, Qt::WDestructiveClose) , RegistroIva(comp)  {
    setupUi(this);
    _depura("Inicializacion de RegistroIvaView", 0);
    /// Disparamos los plugins con presupuesto_imprimirPresupuesto
    int res = g_plugins->lanza("RegistroIvaView_RegistroIvaView", this);
    if (res != 0)
        return;

    /// Usurpamos la identidad de mlist y ponemos nuestro propio widget con sus cosillas.
    m_listprevcobro->setcompany(comp);
    m_contrapartida->setempresa(comp);

    setLineas( mui_listIva);
    mui_listIva->setcompany(comp);

    mui_listPrevCobro->setcompany(comp);


    setListLinPrevCobro(m_listprevcobro);

    /// Preparamos la lista de cobros y pagos
    m_listprevcobro->presentacionFactura();
    m_cursorFPago = NULL;
    cargarComboFPago("NULL");

    ////////////////////////////////////////////////////////
    mui_listIva->setDBTableName("iva");
    mui_listIva->setDBCampoId("idiva");
    mui_listIva->addSHeader("idiva", DBCampo::DBint, DBCampo::DBPrimaryKey, SHeader::DBNoWrite , tr("idiva"));
    mui_listIva->addSHeader("idtipoiva", DBCampo::DBint, DBCampo::DBNotNull, SHeader::DBNoWrite , tr("idtipoiva"));
    mui_listIva->addSHeader("idcuenta", DBCampo::DBint, DBCampo::DBNoSave, SHeader::DBNoWrite , tr("idcuenta"));
    mui_listIva->addSHeader("codigo", DBCampo::DBvarchar, DBCampo::DBNoSave, SHeader::DBNoWrite | SHeader::DBNoView, tr("codigo"));
    mui_listIva->addSHeader("nombretipoiva", DBCampo::DBvarchar, DBCampo::DBNoSave, SHeader::DBNoWrite , tr("nombretipoiva"));
    mui_listIva->addSHeader("idregistroiva", DBCampo::DBint, DBCampo::DBNothing, SHeader::DBNone , tr("idregistroiva"));
    mui_listIva->addSHeader("baseiva", DBCampo::DBnumeric, DBCampo::DBNothing, SHeader::DBNone , tr("baseiva"));
    mui_listIva->addSHeader("ivaiva", DBCampo::DBnumeric, DBCampo::DBNothing, SHeader::DBNone , tr("ivaiva"));
    mui_listIva->setinsercion(FALSE);

    ////////////////////////////////////////////////////////
    mui_listPrevCobro->setDBTableName("prevcobro");
    mui_listPrevCobro->setDBCampoId("idprevcobro");
    mui_listPrevCobro->addSHeader("idprevcobro", DBCampo::DBint, DBCampo::DBPrimaryKey, SHeader::DBNoWrite , tr("idiva"));
    mui_listPrevCobro->addSHeader("fprevistaprevcobro", DBCampo::DBint, DBCampo::DBNotNull, SHeader::DBNoWrite , tr("idtipoiva"));
    mui_listPrevCobro->addSHeader("fcobroprevcobro", DBCampo::DBint, DBCampo::DBNoSave, SHeader::DBNoWrite , tr("idcuenta"));
    mui_listPrevCobro->addSHeader("idfpago", DBCampo::DBvarchar, DBCampo::DBNoSave, SHeader::DBNoWrite | SHeader::DBNoView, tr("codigo"));
    mui_listPrevCobro->addSHeader("idcuenta", DBCampo::DBvarchar, DBCampo::DBNoSave, SHeader::DBNoWrite , tr("nombretipoiva"));
    mui_listPrevCobro->addSHeader("idasiento", DBCampo::DBint, DBCampo::DBNothing, SHeader::DBNone , tr("idregistroiva"));
    mui_listPrevCobro->addSHeader("cantidadprevistaprevcobro", DBCampo::DBnumeric, DBCampo::DBNothing, SHeader::DBNone , tr("baseiva"));
    mui_listPrevCobro->addSHeader("cantidadprevcobro", DBCampo::DBnumeric, DBCampo::DBNothing, SHeader::DBNone , tr("ivaiva"));
    mui_listPrevCobro->addSHeader("idregistroiva", DBCampo::DBnumeric, DBCampo::DBNothing, SHeader::DBNone , tr("ivaiva"));
    mui_listPrevCobro->addSHeader("tipoprevcobro", DBCampo::DBnumeric, DBCampo::DBNothing, SHeader::DBNone , tr("ivaiva"));
    mui_listPrevCobro->addSHeader("docprevcobro", DBCampo::DBnumeric, DBCampo::DBNothing, SHeader::DBNone , tr("ivaiva"));
    mui_listPrevCobro->addSHeader("codigo", DBCampo::DBnumeric, DBCampo::DBNothing, SHeader::DBNone , tr("ivaiva"));
    mui_listPrevCobro->addSHeader("descripcion", DBCampo::DBnumeric, DBCampo::DBNothing, SHeader::DBNone , tr("ivaiva"));
    mui_listPrevCobro->addSHeader("idctacliente", DBCampo::DBnumeric, DBCampo::DBNothing, SHeader::DBNone , tr("ivaiva"));
    mui_listPrevCobro->addSHeader("codigoctacliente", DBCampo::DBnumeric, DBCampo::DBNothing, SHeader::DBNone , tr("ivaiva"));
    mui_listPrevCobro->addSHeader("nomctacliente", DBCampo::DBnumeric, DBCampo::DBNothing, SHeader::DBNone , tr("ivaiva"));
    mui_listPrevCobro->setinsercion(TRUE);

    m_companyact->meteWindow(windowTitle(), this);

    g_plugins->lanza("RegistroIvaView_RegistroIvaView_Post", this);
    _depura("Fin de la inicializacion de RegistroIvaView", 0);
}


RegistroIvaView::~RegistroIvaView() {
    _depura("RegistroIvaView::~RegistroIvaView", 0);
    m_companyact->sacaWindow(this);
    _depura("END RegistroIvaView::~RegistroIvaView", 0);
}


/**
  * \brief Carga el combo de las formas de pago
  * 
  Esta funciÃ³n se encarga de cargar la tabla de formas de pago en el combo box correspondiente.
  Usa un cursor de clase (m_cursorFPago) que es construido al usar esta funciÃ³n y destruido en el destructor de clase.
  Esta funciÃ³n se llama con la inicializaciÃ³n de clase y cuando se quieren cargar datos.
*/
void RegistroIvaView::cargarComboFPago(QString idfpago) {
    _depura("RegistroIvaView::cargarComboFPago", 0);
    if (m_cursorFPago != NULL)
        delete m_cursorFPago;
    m_cursorFPago = m_companyact->cargacursor("SELECT * FROM fpago");
    int i = 0;
    int i1 = 0;
    while (!m_cursorFPago->eof()) {
        i ++;
        if (m_cursorFPago->valor("idfpago") == idfpago) {
            i1 = i;
        }// end if
        m_fPago->insertItem(m_cursorFPago->valor("nomfpago"));
        m_cursorFPago->siguienteregistro();
    }// end while
    if (i1 != 0 ) {
        m_fPago->setCurrentItem(i1-1);
    }// end if
    _depura("RegistroIvaView::cargarComboFPago", 0);
}


void RegistroIvaView::on_mui_borrar_clicked() {
    _depura("RegistroIvaView::on_mui_borrar_clicked",0);
    if (QMessageBox::warning( this, "BulmaCont - Registro Factura", "Desea borrar este registro", "Si", "No") == 0) {
        mui_listIva->borrar();
        borrar();
    }// end if
    _depura("END RegistroIvaView::on_mui_borrar_clicked",0);
}


int RegistroIvaView::cargar(QString id) {
    int error = 0;
    error = RegistroIva::cargar(id);
    if (error)
        return -1;

    mui_listIva->cargar("SELECT * FROM  tipoiva LEFT JOIN (SELECT * FROM iva WHERE idregistroiva="+id+" ) AS t1 ON t1.idtipoiva = tipoiva.idtipoiva LEFT JOIN cuenta on tipoiva.idcuenta = cuenta.idcuenta  ORDER BY codigo");

     mui_listPrevCobro->cargar("SELECT * FROM prevcobro "
                   " LEFT JOIN cuenta ON cuenta.idcuenta=prevcobro.idcuenta "
                   " LEFT JOIN (SELECT idcuenta AS idctacliente, codigo AS codigoctacliente, descripcion AS nomctacliente FROM cuenta) AS T1 ON t1.idctacliente = prevcobro.idctacliente "
                   " WHERE idregistroiva= "+id+" ORDER BY fcobroprevcobro ");

    setCaption("Registro FActura "+factura());
    dialogChanges_cargaInicial();
    return 0;
}


void RegistroIvaView::on_mui_guardar_clicked() {
    _depura("RegistroIvaView::guardaRegistroIva",0);
    try {
	m_companyact->begin();
        setcontrapartida(m_contrapartida->idcuenta());
        setbaseimp(m_baseImponible->text());
        setiva(m_importeiva->text());
        setffactura(m_ffactura->text());
        setfactura(m_factura->text());
        setincregistro(m_incregistroIVA->isChecked()?"t":"f");
        setnumorden(m_numorden->text());
        setcif(m_cif->text());
        setfactemitida(m_factEmitida->isChecked()?"t":"f");
        setfemisionregistroiva(m_femisionregistroiva->text());
        setserieregistroiva(m_serieregistroiva->text());

        int error = RegistroIva::guardar();
        mui_listIva->setColumnValue("idregistroiva", DBvalue("idregistroiva"));
	mui_listIva->guardar();

	m_companyact->commit();
        dialogChanges_cargaInicial();
    } catch (...) {
        mensajeInfo("Error al guardar el Registro de IVA");
	m_companyact->rollback();
    } // end try
    _depura("END RegistroIvaView::guardaRegistroIva",0);
}

/**
  * \brief SLOT que se activa al pulsar sobre el botÃ³n de generar previsiones.
  *
  1.- Vacia la lista de Prevision de Cobros
  2.- Calcula la fecha inicial a partir de la fecha de factura y la forma de pago.
  3.- Itera para cada plazo en la forma de pago calculando el nuevo plazo.
  
  // Falta usar las cuentas de servicio para saber si es cobro o pago.
  */
void RegistroIvaView::boton_generarPrevisiones() {
    QString snumpagos = m_cursorFPago->valor("nplazosfpago",m_fPago->currentItem());
    QString splazoprimerpago = m_cursorFPago->valor("plazoprimerpagofpago",m_fPago->currentItem());
    QString splazoentrerecibo = m_cursorFPago->valor("plazoentrerecibofpago",m_fPago->currentItem());
    Fixed totalfactura = Fixed(baseimp()) + Fixed(iva());
    int plazoentrerecibo = splazoentrerecibo.toInt();
    int plazoprimerpago = splazoprimerpago.toInt();
    int numpagos = snumpagos.toInt();
    Fixed totalplazo = totalfactura / numpagos;
    QString tipocobro;

    // Vaciamos la lista de prevision para que no haga cosas raras
    QDate ffactura = normalizafecha(m_ffactura->text());
    QDate fpcobro = ffactura.addDays(plazoprimerpago);
    for (int i =0; i< numpagos; i++) {
        /// Hay que saber si es un cobro o un pago
        if (contrapartida().left(2) == "43") { // Si es un cliente es un cobro, si es un proveedor es un pago.
            tipocobro = "t";
        } else {
            tipocobro = "f";
        }// end if
        m_listprevcobro->nuevalinea(
            fpcobro.toString("dd/MM/yyyy"),
            fpcobro.toString("dd/MM/yyyy")
            ,"","","",
            totalplazo.toQString(),
            totalplazo.toQString(),
            tipocobro,
            "","","",
            m_contrapartida->idcuenta(),
            m_contrapartida->codigocuenta(),
            m_contrapartida->nomcuenta()
        );
        fpcobro = fpcobro.addDays(plazoentrerecibo);
    }// end for
    m_listprevcobro->pintalistlinprevcobro();
}

