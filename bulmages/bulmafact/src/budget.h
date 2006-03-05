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

#ifndef BUDGET_H
#define BUDGET_H

// PRESUPUESTOS.

#include <QLineEdit>
#include <Q3TextEdit>
#include <QLabel>
#include <QCheckBox>

#include "budgetbase.h"
#include "postgresiface2.h"
#include "presupuesto.h"
#include "listlinpresupuestoview.h"
#include "listdescpresupuestoview.h"
#include "busquedacliente.h"
#include "busquedaformapago.h"
#include "busquedafecha.h"
#include "busquedaalmacen.h"
#include "busquedatrabajador.h"
#include "fixed.h"
#include "dialogchanges.h"


class company;

class Budget : public BudgetBase , public presupuesto, public dialogChanges
{
	Q_OBJECT

private:
	cursor2 *m_cursorcombo;

public:
	Budget(company *, QWidget *, const char *);
	~Budget();
	void pintatotales(Fixed iva, Fixed base, Fixed total, Fixed desc);
	void inicialize();
	void manageArticle(int);
	void pintaNumPresupuesto(QString id)
	{
		m_numpresupuesto->setText(id);
	};
	void pintaFPresupuesto(QString id)
	{
		m_fpresupuesto->setText(id);
	};
	void pintaVencPresupuesto(QString id)
	{
		m_vencpresupuesto->setText(id);
	};
	void pintaContractPresupuesto(QString id)
	{
		m_contactpresupuesto->setText(id);
	};
	void pintaTelPresupuesto(QString id)
	{
		m_telpresupuesto->setText(id);
	};
	void pintaComentPresupuesto(QString id)
	{
		m_comentpresupuesto->setText(id);
	};
	void pintaidcliente(QString id)
	{
		m_cliente->setidcliente(id);
	};
	void pintarefpresupuesto(QString id)
	{
		m_refpresupuesto->setText(id);
	};
	void pintaidforma_pago(QString id)
	{
		m_forma_pago->setidforma_pago(id);
	};
	void pintaidalmacen(QString id)
	{
		m_almacen->setidalmacen(id);
	};
	void pintaidtrabajador(QString id)
	{
		m_trabajador->setidtrabajador(id);
	};
	void pintaprocesadopresupuesto(QString id)
	{
		if (id == "t" || id == "TRUE")
		{
			m_procesadopresupuesto->setChecked(TRUE);
		} else {
			m_procesadopresupuesto->setChecked(FALSE);
		}
	};
	void pintadescpresupuesto(QString id)
	{
		m_descpresupuesto->setText(id);
	};

private:
	void generarPedidoCliente();
	virtual void closeEvent(QCloseEvent *);

public slots:
	virtual void s_informeReferencia();
	virtual int chargeBudget(QString id);
	virtual void s_removeBudget();
	virtual void s_printBudget();
	virtual void s_saveBudget()
	{
		guardapresupuesto();
	};
	virtual void s_removeBudgetLine()
	{
		subform2->borralinpresupuestoact();
	};
	virtual void s_comentariotextChanged()
	{
		setComentPresupuesto(m_comentpresupuesto->text());
	};
	virtual void s_contactotextChanged(const QString &str)
	{
		setContractPresupuesto(str);
	};
	virtual void s_telpresupuestotextChanged(const QString &str)
	{
		setTelPresupuesto(str);
	};
	virtual void s_refpresupuestotextChanged(const QString &str)
	{
		setrefpresupuesto(str);
	};
	virtual void s_numpresupuestotextChanged(const QString &str)
	{
		setNumPresupuesto(str);
	};
	virtual void s_procesadopresupuestostateChanged(int i)
	{
		if (i)
		{
			setprocesadopresupuesto("TRUE");
		} else {
			setprocesadopresupuesto("FALSE");
		}
	};
	virtual void s_descpresupuestotextChanged(const QString &str)
	{
		setdescpresupuesto(str);
	};

	/// Este slot se activa cuando hay cambios en los subformularios.
	virtual void s_pintaTotales()
	{
		calculaypintatotales();
	};
	virtual void s_formapagovalueChanged(QString val)
	{
		setidforma_pago(val);
	};
	virtual void s_trabajadorvalueChanged(QString val)
	{
		setidtrabajador(val);
	};
	virtual void s_almacenvalueChanged(QString val)
	{
		setidalmacen(val);
	};
	virtual void s_clientevalueChanged(QString id)
	{
		setidcliente(id);
	};
	virtual void s_fpresupuestovalueChanged(QString id)
	{
		setFPresupuesto(id);
	};
	virtual void s_vencpresupuestovalueChanged(QString id)
	{
		setVencPresupuesto(id);
	};
	virtual void s_realizarPedidoCliente()
	{
		generarPedidoCliente();
	};
};

#endif
