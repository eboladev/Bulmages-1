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
#include <QCheckBox>
#include <QFile>
#include <QCheckBox>
#include <QTextStream>

#include "almaceneslistview.h"
#include "company.h"
#include "funcaux.h"
#include "plugins.h"
#include "configuracion.h"
#include "almacenview.h"

AlmacenesListView::AlmacenesListView(QWidget *parent, Qt::WFlags flag)
        : Listado(NULL, parent, flag) {
    _depura("AlmacenesListView::AlmacenesListView(1)", 0);
    setupUi(this);
    setAttribute(Qt::WA_DeleteOnClose);
    /// Disparamos los plugins.
    int res = g_plugins->lanza("AlmacenesListView_AlmacenesListView", this);
    if (res != 0)
        return;
    m_idalmacen = "";
    setSubForm(mui_list);

    mui_list->setDBTableName("almacen");
    mui_list->setDBCampoId("idalmacen");
    mui_list->addSHeader("idalmacen", DBCampo::DBint, DBCampo::DBNotNull | DBCampo::DBPrimaryKey, SHeader::DBNoView | SHeader::DBNoWrite, tr("ID almacen"));
    mui_list->addSHeader("codalmacen", DBCampo::DBvarchar, DBCampo::DBNoSave, SHeader::DBNone | SHeader::DBNoWrite, tr("Código almacen"));
    mui_list->addSHeader("nomalmacen", DBCampo::DBvarchar, DBCampo::DBNoSave, SHeader::DBNone | SHeader::DBNoWrite, tr("Nombre almacen"));
    mui_list->setinsercion(FALSE);
    mui_list->setDelete(FALSE);
    mui_list->setSortingEnabled(TRUE);

    hideBusqueda();
    _depura("END AlmacenesListView::AlmacenesListView(1)", 0);
}


AlmacenesListView::AlmacenesListView(Company *comp, QWidget *parent, Qt::WFlags flag)
        : Listado(comp, parent, flag) {
    _depura("AlmacenesListView::AlmacenesListView(2)", 0);
    setupUi(this);
    setAttribute(Qt::WA_DeleteOnClose);
    /// Disparamos los plugins.
    int res = g_plugins->lanza("AlmacenesListView_AlmacenesListView", this);
    if (res != 0)
        return;
    mui_list->setEmpresaBase(comp);
    setSubForm(mui_list);

    mui_list->setDBTableName("almacen");
    mui_list->setDBCampoId("idalmacen");
    mui_list->addSHeader("idalmacen", DBCampo::DBint, DBCampo::DBNotNull | DBCampo::DBPrimaryKey, SHeader::DBNoView | SHeader::DBNoWrite, tr("ID almacen"));
    mui_list->addSHeader("codalmacen", DBCampo::DBvarchar, DBCampo::DBNoSave, SHeader::DBNone | SHeader::DBNoWrite, tr("Código almacen"));
    mui_list->addSHeader("nomalmacen", DBCampo::DBvarchar, DBCampo::DBNoSave, SHeader::DBNone | SHeader::DBNoWrite, tr("Nombre almacen"));
    mui_list->setinsercion(FALSE);
    mui_list->setDelete(FALSE);
    mui_list->setSortingEnabled(TRUE);

    presentar();
    m_idalmacen = "";
    empresaBase()->meteWindow(windowTitle(), this);
    hideBusqueda();
    /// Hacemos el tratamiento de los permisos que desabilita botones en caso de no haber suficientes permisos.
    trataPermisos("almacen");
    _depura("END AlmacenesListView::AlmacenesListView(2)", 0);
}





AlmacenesListView::~AlmacenesListView() {
    _depura("AlmacenesListView::~AlmacenesListView", 0);
    _depura("END AlmacenesListView::~AlmacenesListView", 0);
}





QString AlmacenesListView::idalmacen() {
    return m_idalmacen;
}



void AlmacenesListView::setEmpresaBase(Company *comp) {
    PEmpresaBase::setEmpresaBase(comp);
    mui_list->setEmpresaBase(comp);
}





void AlmacenesListView::crear() {
/*
    ((Company *)empresaBase())->s_newAlmacen();
*/
}



void AlmacenesListView::presentar() {
    _depura("AlmacenesListView::presentar", 0);

    /// Hacemos el listado y lo presentamos.
    mui_list->cargar("SELECT * FROM almacen");

    _depura("END AlmacenesListView::presentar", 0);
}


QString AlmacenesListView::generaFiltro() {
    _depura("AlmacenesListView::generaFiltro", 0);
    /// Tratamiento de los filtros.
    QString filtro = "";

    return (filtro);
    _depura("END AlmacenesListView::generaFiltro", 0);
}


void AlmacenesListView::editar(int row) {
    _depura("AlmacenesListView::editar", 0);
    try {
        m_idalmacen = mui_list->DBvalue(QString("idalmacen"), row);

        if (modoEdicion()) {
		AlmacenView *alm = new AlmacenView((Company *)empresaBase(), 0);
		empresaBase()->pWorkspace()->addWindow(alm);
		alm->show();
		alm->cargar(m_idalmacen);
        } else {
            emit(selected(m_idalmacen));
        } // end if
        _depura("END AlmacenesListView::editar", 0);
    } catch(...) {
        mensajeInfo(tr("Error al editar el almacen"));
    } // end try
}



void AlmacenesListView::imprimir() {
    _depura("AlmacenesListView::imprimir", 0);
    mui_list->imprimirPDF(tr("Almacenes"));
    _depura("END AlmacenesListView::imprimir", 0);
}


void AlmacenesListView::borrar() {
    _depura("AlmacenesListView::borrar", 0);
    int a = mui_list->currentRow();
    if (a < 0) {
        mensajeInfo(tr("Debe seleccionar una linea"));
        return;
    } // end if
    try {
        m_idalmacen = mui_list->DBvalue(QString("idpresupuesto"));
        if (modoEdicion()) {
/*
            PresupuestoView *pv = ((Company *)empresaBase())->nuevoPresupuestoView();
            if (pv->cargar(m_idalmacen))
                throw -1;
            pv->on_mui_borrar_clicked();
            pv->close();
*/
        } // end if
        presentar();
    } catch (...) {
        mensajeInfo(tr("Error al borrar el almacen"));
    } // end try
    _depura("END AlmacenesListView::borrar", 0);
}


