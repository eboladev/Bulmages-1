/***************************************************************************
 *   Copyright (C) 2004 by J. M. Torres Rigo                               *
 *   joanmi@bulma.net                                                      *
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

#ifndef PROVIDERSLIST_H
#define PROVIDERSLIST_H

#include <Q3Frame>

#include "pgimportfiles.h"
#include "subform2bf.h"


class company;

class ProveedorListSubform : public SubForm2Bf {
Q_OBJECT
public:
	ProveedorListSubform(QWidget *parent = 0, const char *name = 0);
	~ProveedorListSubform() {};
public slots:
virtual void cargar() {
    _depura("ProveedorListSubform::cargar\n",0);
    QString SQLQuery = "SELECT * FROM proveedor";
    cursor2 * cur= companyact()->cargacursor(SQLQuery);
	SubForm2::cargar(cur);
    delete cur;
};
virtual int cargar(cursor2 *cur) {
    _depura("ProveedorListSubform::cargar\n",0);
	SubForm2::cargar(cur);
	return 0;
};
};


#include "ui_providerslistbase.h"


class ProveedorList : public QWidget, public Ui_ProveedorListBase, public pgimportfiles
{
	Q_OBJECT

public:
	enum edmode
	{
		EditMode = 0,
		SelectMode = 1
	};

private:
	company *m_companyact;
	edmode m_modo;
	QString m_idprovider;
	QString m_cifprovider;
	QString m_nomprovider;

public:
	ProveedorList(company *, QWidget *parent = 0, const char *name = 0,
			Qt::WFlags flag = 0, edmode editmode = EditMode);
	~ProveedorList();
	void presenta();
	void modoseleccion()
	{
		m_modo = SelectMode;
	};
	void modoedicion()
	{
		m_modo = EditMode;
	};
	QString idprovider()
	{
		return m_idprovider;
	};
	QString cifprovider()
	{
		return m_cifprovider;
	};
	QString nomprovider()
	{
		return m_nomprovider;
	};
	void hideBotonera()
	{
		m_botonera->hide();
	};
	void showBotonera()
	{
		m_botonera->show();
	};
	void hideBusqueda()
	{
		m_busqueda->hide();
	};
	void showBusqueda()
	{
		m_busqueda->show();
	};
	void hideConfiguracion()
	{
		m_configuracion->hide();
	};
	void showConfiguracion()
	{
		m_configuracion->show();
	};
	/// Funciones que se encarga en guardar y cargar la configuracion del listado.
	void guardaconfig();
	void cargaconfig();

public slots:

	virtual void editar(int);
	virtual void on_mui_crear_clicked();
	virtual void s_findProvider();
	virtual void on_mui_editar_clicked();
	virtual void on_mui_borrar_clicked();
	virtual void on_mui_imprimir_clicked();
	virtual void on_mui_actualizar_clicked() {
		presenta();
	};
	virtual void on_mui_exportar_clicked();
	virtual void on_mui_importar_clicked();
	virtual void s_configurar();
	void on_mui_list_itemDoubleClicked( QTableWidgetItem *item) {
		on_mui_editar_clicked();
	};

signals:
	void selected(QString);
};






#endif
