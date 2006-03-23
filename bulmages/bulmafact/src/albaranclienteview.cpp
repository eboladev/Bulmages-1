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

#include <QLineEdit>
#include <Q3TextEdit>
#include <QMessageBox>
#include <QLabel>
#include <Q3Table>
#include <QWidget>
#include <QObject>
#include <QComboBox>
#include <Q3PopupMenu>
#include <QToolButton>
#include <QCloseEvent>

#include "albaranclienteview.h"
#include "company.h"
#include "division.h"
#include "clientslist.h"
#include "articulolist.h"
#include "configuracion.h"
#include "presupuestolist.h"
#include "presupuestoview.h"
#include "pedidosclientelist.h"
#include "pedidoclienteview.h"
#include "facturaview.h"
#include "informereferencia.h"
#include "funcaux.h"
#include "postgresiface2.h"
#include "listlinalbaranclienteview.h"
#include "listdescalbaranclienteview.h"
#include "facturaslist.h"


AlbaranClienteView::AlbaranClienteView(company *comp, QWidget *parent, const char *name)
	: AlbaranClienteBase(parent, name, Qt::WDestructiveClose), AlbaranCliente(comp),
	dialogChanges(this)
{
	subform2->setcompany(comp);
	m_descuentos->setcompany(comp);
	m_almacen->setcompany(comp);
	m_forma_pago->setcompany(comp);
	m_cliente->setcompany(comp);
	m_trabajador->setcompany(comp);
	setListLinAlbaranCliente(subform2);
	setListDescuentoAlbaranCliente(m_descuentos);
	comp->meteWindow(caption(),this);
	dialogChanges_cargaInicial();
	_depura("Fin de la inicializacion de AlbaranClienteView\n");
};


AlbaranClienteView::~AlbaranClienteView()
{
	companyact->refreshAlbaranesCliente();
	companyact->sacaWindow(this);
};


void AlbaranClienteView::pintatotales(Fixed iva, Fixed base, Fixed total, Fixed desc)
{
	m_totalBases->setText(base.toQString());
	m_totalTaxes->setText(iva.toQString());
	m_totalalbaran->setText(total.toQString());
	m_totalDiscounts->setText(desc.toQString());
};


void AlbaranClienteView::s_verpresupuesto()
{
	QString SQLQuery= "SELECT * FROM presupuesto WHERE refpresupuesto='" +
			DBvalue("refalbaran") + "'";
	cursor2 *cur = companyact->cargacursor(SQLQuery);

	if (cur->numregistros() > 1)
	{
		PresupuestoList *list = new PresupuestoList(companyact, NULL,
				theApp->translate("Edicion de Presupuestos", "company"));
		list->modoseleccion();
		list->show();

		while (!list->isHidden())
		{
			theApp->processEvents();
		};

		this->setEnabled(true);

		if (list->idpresupuesto() != QString(""))
		{
			PresupuestoView *bud = companyact->newBudget();
			bud->chargeBudget(list->idpresupuesto());
			bud->show();
		};
	} else if (!cur->eof()) {
		PresupuestoView *bud = companyact->newBudget();
		bud->chargeBudget(cur->valor("idpresupuesto"));
		bud->show();
	};

	delete cur;
};


void AlbaranClienteView::s_verpedidocliente() {
	QString SQLQuery = "SELECT * FROM pedidocliente WHERE refpedidocliente='" + DBvalue("refalbaran") + "'";
	cursor2 *cur = companyact->cargacursor(SQLQuery);

	if (cur->numregistros() > 1)
	{
		PedidosClienteList *list = new PedidosClienteList(companyact, NULL,
				theApp->translate("Edicion de Presupuestos", "company"));
		list->modoseleccion();
		list->show();

		while (!list->isHidden())
		{
			theApp->processEvents();
		};

		this->setEnabled(true);
		if (list->idpedidocliente() != "")
		{
			// && list->idpedidocliente() !=NULL
			PedidoClienteView *bud = new PedidoClienteView(companyact, NULL,
				theApp->translate("Edicion de Presupuestos", "company"));
			companyact->m_pWorkspace->addWindow(bud);
			bud->cargar(list->idpedidocliente());
			bud->show();
		};
	} else if (!cur->eof()) {
		PedidoClienteView *bud = new PedidoClienteView(companyact, NULL,
			theApp->translate("Edicion de Presupuestos", "company"));
		companyact->m_pWorkspace->addWindow(bud);
		bud->cargar(cur->valor("idpedidocliente"));
		bud->show();
	};

	delete cur;
};


/// Se encarga de generar una factura a partir de un albaran.
void AlbaranClienteView::generarFactura() {
	/// Comprobamos que existe el elemento, y en caso afirmativo lo mostramos
	/// y salimos de la funcion.
	QString SQLQuery = "SELECT * FROM factura WHERE reffactura='" +
			DBvalue("refalbaran") + "'";
	cursor2 *cur = companyact->cargacursor(SQLQuery);

	if (!cur->eof())
	{
		FacturaView *bud = companyact->newFacturaView();
		companyact->m_pWorkspace->addWindow(bud);
		bud->cargar(cur->valor("idfactura"));
		bud->show();
		return;
	};

	delete cur;

	/// Informamos de que no existe el pedido y a ver si lo queremos realizar.
	/// Si no salimos de la funcion.
	if (QMessageBox::question(this, tr("Factura Inexistente"),
                tr("No existe una factura asociada a este albaran. Desea Crearla ?"),
                tr("&Yes"), tr("&No"), QString::null, 0, 1))
	{
		return;
	};

	/// Creamos la factura.
	FacturaView *bud = companyact->newFacturaView();
	companyact->m_pWorkspace->addWindow(bud);
	bud->vaciaFactura();
	bud->setcomentfactura(DBvalue("comentalbaran"));
	bud->setidforma_pago(DBvalue("idforma_pago"));
	bud->setreffactura(DBvalue("refalbaran"));
	bud->setidcliente(DBvalue("idcliente"));
	bud->setidalmacen(DBvalue("idalmacen"));
	QString l;
	LinAlbaranCliente *linea;
	uint i = 0;

	for (linea = listalineas->m_lista.first(); linea; linea = listalineas->m_lista.next())
	{
		bud->getlistalineas()->nuevalinea(linea->desclalbaran(), linea->cantlalbaran(),
			linea->pvplalbaran(),linea->descontlalbaran(), linea->idarticulo(),
			linea->codigocompletoarticulo(), linea->nomarticulo(),
			linea->ivalalbaran());
		i++;
	};

	bud->pintaFactura();
	bud->show();
};


/// Se encarga de agregar un albaran a una factura.
void AlbaranClienteView::agregarFactura()
{
	/// Pedimos la factura a la que agregar.
	_depura("Busqueda de una factura.", 0);
	FacturasList *fac = new FacturasList(companyact, NULL,
			tr("Seleccione factura", "company"));
	fac->modoseleccion();
	/// Esto es convertir un QWidget en un sistema modal de dialogo.
	this->setEnabled(false);
	fac->show();

	while(!fac->isHidden())
	{
		theApp->processEvents();
	};

	this->setEnabled(true);
	QString idfactura = fac->idfactura();
	delete fac;
	/// Si no hay idfactura es que hemos abortado y por tanto cancelamos la operacion.
	if (idfactura == "")
	{
		return;
	};

	/// Creamos la factura.
	FacturaView *bud = companyact->newFacturaView();
	bud->cargar(idfactura);
	companyact->m_pWorkspace->addWindow(bud);
	/// EN TEORIA SE DEBARIA COMPROBAR QUE LA FACTURA ES DEL MISMO CLIENTE,
	/// pero por ahora pasamos de hacerlo.
	QString l;
	LinAlbaranCliente *linea;
	uint i = 0;

	for (linea = listalineas->m_lista.first(); linea; linea = listalineas->m_lista.next())
	{
		bud->getlistalineas()->nuevalinea(linea->desclalbaran(), linea->cantlalbaran(),
			linea->pvplalbaran(),linea->descontlalbaran(), linea->idarticulo(),
			linea->codigocompletoarticulo(), linea->nomarticulo(),
			linea->ivalalbaran());
		i++;
	};

	bud->pintaFactura();
	bud->show();
};


int AlbaranClienteView::cargar(QString id)
{
	AlbaranCliente::cargar(id);
	setCaption("Albaran Cliente  " + DBvalue("refalbaran"));

	if (companyact->meteWindow(caption(), this))
	{
		return -1;
	};

	dialogChanges_cargaInicial();
	return 0;
};


void AlbaranClienteView::s_informeReferencia()
{
	InformeReferencia *inf = new InformeReferencia(companyact);
	inf->setreferencia(DBvalue("refalbaran"));
	inf->generarinforme();
	delete inf;
};


void AlbaranClienteView::closeEvent(QCloseEvent *e)
{
	_depura("closeEvent", 0);
	if (dialogChanges_hayCambios())
	{
		int val = QMessageBox::warning(this, "Guardar Albaran",
				"Desea guardar los cambios.", "Si", "No", "Cancelar", 0, 2);
		if (val == 0)
		{
			guardaAlbaranCliente();
		}

		if (val == 2)
		{
			e->ignore();
		}
	};
};
