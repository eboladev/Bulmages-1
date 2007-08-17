/***************************************************************************
 *   Copyright (C) 2006 by Tomeu Borras Riera                              *
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

#include <QMenu>
#include "listado.h"
#include "empresabase.h"



void Listado::setModoConsulta() {
    m_modo = SelectMode;
}


void Listado::setModoEdicion() {
    m_modo = EditMode;
}


bool Listado::modoEdicion() {
    return m_modo == EditMode;
}


bool Listado::modoConsulta() {
    return m_modo == SelectMode;
}


Listado::Listado(QWidget *parent, Qt::WFlags f, edmode modo) : BLWidget(parent, f) {
    _depura("Listado::Listado", 0);
    m_modo = modo;
    _depura("END Listado::Listado", 0);
}

Listado::Listado(EmpresaBase *emp, QWidget *parent, Qt::WFlags f, edmode modo) : BLWidget(emp, parent, f) {
    _depura("Listado::Listado", 0);
    m_modo = modo;
    _depura("END Listado::Listado", 0);
}


Listado::~Listado() {
    _depura("Listado::~Listado", 0, this->windowTitle());
    sacaWindow();
    _depura("END Listado::~Listado", 0);
}


int Listado::sacaWindow() {
    _depura("Listado::sacaWindow", 0);
    empresaBase()->sacaWindow(this);
    _depura("END Listado::sacaWindow", 0);
    return 0;
}

int Listado::meteWindow(QString title) {
    _depura("Listado::meteWindow", 0);
    empresaBase()->meteWindow(title, this);
    _depura("END Listado::meteWindow", 0);
    return 0;
}


/** SLOT que responde al doble click sobre un elemento del listado
    llama a editArticle para que actue correspondientemente.
*/
void Listado::on_mui_list_cellDoubleClicked(int a, int) {
    _depura("ArticuloList::on_mui_list_cellDoubleClicked", 0);
    editar(a);
    _depura("END ArticuloList::on_mui_list_cellDoubleClicked", 0);
}


void Listado::editar(int) {
    _depura("metodo para ser reimplementado en clases hijas", 2);
}


void Listado::crear() {
    _depura("metodo para ser reimplementado en clases hijas", 2);
}


void Listado::borrar() {
    _depura("metodo para ser reimplementado en clases hijas", 2);
}


void Listado::imprimir() {
    m_listado->imprimirPDF("");
}

/// Este metodo carga el subformulario con un SELECT generico. Si no quiere usarse este select
/// Debe reimplementar el metodo en las clases derivadas.
void Listado::presentar() {
    _depura("Listado::presentar", 0);
    QString query = "SELECT * FROM " + m_listado->tableName();
    m_listado->cargar(query);
    _depura("END Listado::presentar", 0);
}


void Listado::setSubForm(SubForm3 *list) {
    m_listado = list;
}


void Listado::on_m_filtro_textChanged(const QString &text) {
    if (text.size() >= 3) {
        on_mui_actualizar_clicked();
    } // end if
}

void Listado::on_m_filtro_editingFinished() {
    _depura("Listado::on_m_filtro_editFinished", 0);
    static QString valant = "";
    QLineEdit *line = findChild<QLineEdit *>("m_filtro");
    if (line->text() != valant) {
	valant = line->text();
        on_mui_actualizar_clicked();
    } // end if
    _depura("END Listado::on_m_filtro_editFinished", 0);
}

void Listado::on_mui_actualizar_clicked() {
    presentar();
}


void Listado::on_mui_imprimir_clicked() {
    imprimir();
}


void Listado::on_mui_crear_clicked() {
    crear();
}


void Listado::on_mui_borrar_clicked() {
    borrar();
}


/** SLOT que responde al boton de editar articulo
    Comprueba que haya un elemento seleccionado y llama a editArticle.
*/
void Listado::on_mui_editar_clicked() {
    _depura("ArticuloList::INIT_s_editArticle", 0);
    int a = m_listado->currentRow();
    if (a < 0) {
        mensajeInfo(tr("Tiene que seleccionar un elemento"));
        return;
    } // end if
    editar(a);
    _depura("ArticuloList::END_s_editArticle", 0);
}


void Listado::on_mui_configurar_toggled(bool checked) {
    if (checked) {
        m_listado->showConfig();
    } else {
        m_listado->hideConfig();
    } // end if
}


/** \TODO: REVISAR ESTE METODO YA QUE NO PARECE SER EL ADECUADO
    EN LA LLAMADA DE SUBMENUS
*/
void Listado::on_mui_list_customContextMenuRequested(const QPoint &p) {
    _depura("Listado::on_mui_list_customContextMenuRequested", 0);
    submenu(p);
    _depura("END Listado::on_mui_list_customContextMenuRequested", 0);
}


/** \TODO: REVISAR ESTE METODO YA QUE NO PARECE SER EL ADECUADO
    EN LA LLAMADA DE SUBMENUS
*/
void Listado::submenu(const QPoint &) {
    _depura("ArticuloList::on_mui_list_customContextMenuRequested", 0);
    int a = m_listado->currentRow();
    if (a < 0)
        return;
    QMenu *popup = new QMenu(this);
    QAction *edit = popup->addAction(tr("Editar"));
    QAction *del = popup->addAction(tr("Borrar"));
    QAction *opcion = popup->exec(QCursor::pos());
    if (opcion == del)
        on_mui_borrar_clicked();
    if (opcion == edit)
        on_mui_editar_clicked();
    delete popup;
}


void Listado::on_mui_list_toogledConfig(bool check) {
    QToolButton *botonconfigurar = findChild<QToolButton *>("mui_configurar");
    if (botonconfigurar)
        botonconfigurar->setChecked(check);
}


void Listado::hideBotonera() {
    QWidget *botonera = findChild<QWidget *>("m_botonera");
    if (botonera)
        botonera->hide();
}


void Listado::showBotonera() {
    QWidget *botonera = findChild<QWidget *>("m_botonera");
    if (botonera)
        botonera->show();
}


void Listado::hideBusqueda() {
    QWidget *busqueda = findChild<QWidget *>("m_busqueda");
    if (busqueda)
        busqueda->hide();
}


void Listado::showBusqueda() {
    QWidget *busqueda = findChild<QWidget *>("m_busqueda");
    if (busqueda)
        busqueda->show();
}

void Listado::trataPermisos(QString nomtabla) {
    _depura("Listado::trataPermisos", 0);

 	if (!empresaBase()->has_table_privilege(nomtabla, "INSERT")) {
		/// Buscamos los permisos que tiene el usuario y desactivamos botones.
		QToolButton *b = findChild<QToolButton *>("mui_crear");
		if (b) b->setDisabled(TRUE);
		QToolButton *c = findChild<QToolButton *>("mui_importar");
		if (c) c->setDisabled(TRUE);
		QToolButton *d = findChild<QToolButton *>("mui_exportar");
		if (d) d->setDisabled(TRUE);
	} // end if



	if (!empresaBase()->has_table_privilege(nomtabla, "UPDATE")) {
		/// Buscamos los permisos que tiene el usuario y desactivamos botones.
		QToolButton *b = findChild<QToolButton *>("mui_editar");
		if (b) b->setDisabled(TRUE);
		QToolButton *c = findChild<QToolButton *>("mui_borrar");
		if (c) c->setDisabled(TRUE);
		QToolButton *e = findChild<QToolButton *>("mui_importar");
		if (e) e->setDisabled(TRUE);
		QToolButton *d = findChild<QToolButton *>("mui_exportar");
		if (d) d->setDisabled(TRUE);
	} // end if


    _depura("END Listado::trataPermisos", 0);
}

