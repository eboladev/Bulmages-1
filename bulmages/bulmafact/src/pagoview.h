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

#ifndef PAGOVIEW_H
#define PAGOVIEW_H

#include <QLineEdit>
#include <Q3TextEdit>
#include <QLabel>
#include <QCheckBox>

// Presupuestos.
#include "ui_pagobase.h"
#include "postgresiface2.h"
#include "pago.h"
#include "busquedaproveedor.h"
#include "busquedafecha.h"
#include "dialogchanges.h"


class company;

class PagoView : public QDialog, private Ui_PagoBase, public Pago, public dialogChanges
{
	Q_OBJECT

public:
	PagoView(company *, QWidget *, const char *);
	~PagoView();
	void manageArticle(int);  
	void closeEvent(QCloseEvent *);
	void pintafechapago(QString id)
	{
		mui_fechapago->setText(id);
	};
	void pintacomentpago(QString id)
	{
		mui_comentpago->setText(id);
	};
	void pintaidproveedor(QString id)
	{
		mui_proveedor->setidproveedor(id);
	};
	void pintarefpago(QString id)
	{
		mui_refpago->setText(id);
	};
	void pintacantpago(QString id)
	{
		mui_cantpago->setText(id);
	};
	void pintaprevisionpago(QString id)
	{
		if (id == "t" || id == "TRUE")
		{
			mui_previsionpago->setChecked(TRUE);
		} else {
			mui_previsionpago->setChecked(FALSE);
		}
	};

public slots:
	virtual void on_mui_guardar_clicked() {guardaPago();};

	virtual void cargar(QString id)
	{
		Pago::cargar(id);
		setCaption("Pago " + DBvalue("refpago"));
    		dialogChanges_cargaInicial();
	};
	virtual void on_mui_borrar_clicked();

	virtual void  on_mui_comentpago_textChanged(const QString &str) {
		setcomentpago(str);
	};

	virtual void on_mui_refpago_textChanged(const QString &str)	{
		setrefpago(str);
	};
	virtual void on_mui_cantpago_textChanged(const QString &str) {
		setcantpago(str);
	};
	virtual void on_mui_previsionpago_stateChanged(int i)
	{
		if (i)
		{
			setprevisionpago("TRUE");
		} else {
			setprevisionpago("FALSE");
		}
	}
	virtual void on_mui_proveedor_valueChanged(QString id)
	{
		setidproveedor(id);
	};
	virtual void on_mui_fechapago_valueChanged(QString id)
	{
		setfechapago(id);
	};
};

#endif
