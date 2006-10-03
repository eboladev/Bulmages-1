/***************************************************************************
 *   Copyright (C) 2004 by Alvaro de Miguel                                *
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
#include <QMessageBox>
#include <QComboBox>
#include <QLabel>
#include <QPixmap>
#include <QCheckBox>
#include <QFileDialog>
#include <QCloseEvent>

#include "articuloview.h"
#include "company.h"
#include "familiasview.h"
#include "tiposarticuloview.h"
#include "busquedafamilia.h"
#include "busquedatipoarticulo.h"
#include "comparticulolistview.h"
#include "comparticulolist.h"
#include "funcaux.h"
#include "plugins.h"


ArticuloView::ArticuloView(company *comp, QWidget *parent)
        : QWidget(parent), dialogChanges(this), Articulo(comp) {
    _depura("ArticuloView::ArticuloView", 0);
    setAttribute(Qt::WA_DeleteOnClose);
    try {
        m_companyact = comp;
        setupUi(this);

        /// Disparamos los plugins.
        int res = g_plugins->lanza("ArticuloView_ArticuloView", this);
        if (res != 0)
            return;

        m_familia->setcompany(comp);
        m_tipoarticulo->setcompany(comp);
        m_componentes->setcompany(comp);
        m_archivoimagen = "";
        cargarcomboiva("0");
        m_componentes->cargar("0");

        m_imagen->setPixmap(QPixmap("/usr/share/bulmages/logopeq.png"));
        m_companyact->meteWindow(tr("Edicion del articulo"), this, FALSE);
        dialogChanges_cargaInicial();
    } catch (...) {
        mensajeInfo(tr("Error al crear el articulo"));
    } // end try
    _depura("END ArticuloView::ArticuloView", 0);
}


ArticuloView::~ArticuloView() {
    _depura("ArticuloView::INIT_destructor()\n", 0);
    m_companyact->sacaWindow(this);
    _depura("ArticuloView::END_destructor()\n", 0);
}


void ArticuloView::pintar() {
    _depura("ArticuloView::pintar", 0);

    m_familia->setidfamilia(DBvalue("idfamilia"));
    m_tipoarticulo->setidtipo_articulo(DBvalue("idtipo_articulo"));
    m_codigoarticulo->setText(DBvalue("codarticulo"));
    m_nombrearticulo->setText(DBvalue("nomarticulo"));
    m_obserarticulo->setPlainText(DBvalue("obserarticulo"));
    m_pvparticulo->setText(DBvalue("pvparticulo"));
    m_abrevarticulo->setText(DBvalue("abrevarticulo"));

    m_codigocompletoarticulo->setText(DBvalue("codigocompletoarticulo"));
    /// Pintamos el stockable y el presentable.
    if (DBvalue("presentablearticulo") == "t") {
        m_presentablearticulo->setChecked(TRUE);
    } else {
        m_presentablearticulo->setChecked(FALSE);
    } // end if
    if (DBvalue("controlstockarticulo") == "t") {
        m_controlstockarticulo->setChecked(TRUE);
    } else {
        m_controlstockarticulo->setChecked(FALSE);
    } // end if

    m_imagen->setPixmap(QPixmap(confpr->valor(CONF_DIR_IMG_ARTICLES) + m_codigocompletoarticulo->text() + ".jpg"));

    setWindowTitle(tr("Articulo") + " " + m_codigocompletoarticulo->text());
    _depura("END ArticuloView::pintar", 1);
}

/// Esta funcion carga un articulo de la base de datos y lo presenta.
/// Si el parametro pasado no es un identificador valido entonces se pone
/// la ventana de edicion en modo de insercion.
int ArticuloView::cargar(QString idarticulo) {
    _depura("ArticuloView::cargar()\n", 0);
    try {
        setDBvalue("idarticulo", idarticulo);

        /// Disparamos los plugins.
        int res = g_plugins->lanza("ArticuloView_cargar", this);
        if (res != 0)
            return res;

        QString ivaType = "";
        Articulo::cargar(idarticulo);
        ivaType = DBvalue("idtipo_iva");

        int ret = cargarcomboiva(ivaType);
        if (ret)
            throw -1;

        /// Cambiamos el titulo de la ventana para que aparezca el codigo del articulo.
        setWindowTitle(tr("Articulo") + " " + DBvalue("codigocompletoarticulo"));
        ret = m_companyact->meteWindow(windowTitle(), this);
        if (ret)
            throw -1;
        m_componentes->cargar(DBvalue("idarticulo"));

    } catch (...) {
        mensajeInfo(tr("Error en la carga del articulo"));
        return -1;
    } // end try
    pintar();
    dialogChanges_cargaInicial();
    _depura("END ArticuloView::cargar()\n", 0);
    return 0;
}


/// Hace la carga del combo-box de tipos de iva para el articulo.
int ArticuloView::cargarcomboiva(QString idIva) {
    _depura("ArticuloView::INIT_cargarcomboiva()\n", 0);

    m_cursorcombo = NULL;
    if (m_cursorcombo != NULL)
        delete m_cursorcombo;
    m_combotipo_iva->clear();
    m_cursorcombo = m_companyact->cargacursor("SELECT * FROM tipo_iva");
    if (m_cursorcombo->error()) {
        delete m_cursorcombo;
        return -1;
    } // end if
    int i = 0;
    int i1 = 0;
    while (!m_cursorcombo->eof()) {
        i++;
        if (idIva == m_cursorcombo->valor("idtipo_iva")) {
            i1 = i;
        }
        m_combotipo_iva->addItem(m_cursorcombo->valor("desctipo_iva"));
        m_cursorcombo->siguienteregistro();
    } // end while
    if (i1 != 0) {
        m_combotipo_iva->setCurrentIndex(i1 - 1);
    } // end if

    _depura("ArticuloView::END_cargarcomboiva()\n", 0);
    return 0;
}


/// Esta funcion se ejecuta cuando se ha pulsado sobre el boton de borrar.
void ArticuloView::on_mui_borrar_clicked() {
    _depura("ArticuloView::INIT_boton_borrar()\n", 0);

    if (DBvalue("idarticulo") != "") {
        if (QMessageBox::question(this,
                                  tr("Borrar articulo"),
                                  tr("Esta a punto de borrar un articulo. Desea continuar?"),
                                  tr("&Si"), tr("&No"), 0, 1, 0) == 0) {
            borrar();
            dialogChanges_cargaInicial();
        } // end if
    } // end if
    _depura("ArticuloView::END_boton_borrar()\n", 0);
}


void ArticuloView::on_m_codigocompletoarticulo_editingFinished() {
    _depura("ArticuloView::INIT_s_findArticulo()\n", 0);

    QString SQlQuery = "SELECT * FROM articulo WHERE codigocompletoarticulo = '" + m_codigocompletoarticulo->text() + "'";
    cursor2 *cur = m_companyact->cargacursor(SQlQuery);
    if (!cur->eof()) {
        cargar(cur->valor("idarticulo"));
    } // end if
    delete cur;

    _depura("ArticuloView::END_s_findArticulo()\n", 0);
}


/// Metodo de guardar la ficha. Guarda todos los componentes de la ficha.
/// Si todo ha ido bien devuelve 0
/// Si hay algun error debe ser tratado con el manejo de excepciones catch.
int ArticuloView::guardar() {
    try {
        _depura("ArticuloView::guardar()\n", 0);
        setDBvalue("presentablearticulo",  m_presentablearticulo->isChecked() ? "TRUE" : "FALSE");
        setDBvalue("controlstockarticulo", m_controlstockarticulo->isChecked() ? "TRUE" : "FALSE");
        setDBvalue("idtipo_articulo", m_tipoarticulo->idtipo_articulo());
        setDBvalue("codarticulo", m_codigoarticulo->text());
        setDBvalue("nomarticulo", m_nombrearticulo->text());
        setDBvalue("idfamilia", m_familia->idfamilia());
        setDBvalue("obserarticulo", m_obserarticulo->toPlainText());
        setDBvalue("abrevarticulo", m_abrevarticulo->text());
        setDBvalue("pvparticulo", m_pvparticulo->text());
        setDBvalue("idtipo_iva", m_cursorcombo->valor("idtipo_iva", m_combotipo_iva->currentIndex()));

        if (Articulo::guardar() != 0)
            throw -1;

        /// Guardamos la imagen, si es que existe.
        if (m_archivoimagen != "") {
            cursor2 *cur1 = m_companyact->cargacursor("SELECT codigocompletoarticulo FROM articulo WHERE idarticulo = " + DBvalue("idarticulo"));
            QString cadena = "cp " + m_archivoimagen + " " + confpr->valor(CONF_DIR_IMG_ARTICLES) + cur1->valor("codigocompletoarticulo") + ".jpg";
            delete cur1;
            system(cadena.toAscii().constData());
        } // end if

        /// Guardamos la lista de componentes.
        m_componentes->setColumnValue("idarticulo", DBvalue("idarticulo"));
        if (m_componentes->guardar() != 0)
            throw -1;

        /// Disparamos los plugins
        int res = g_plugins->lanza("ArticuloView_guardar_post", this);
        if (res != 0)
            return res;

        dialogChanges_cargaInicial();

        _depura("END ArticuloView::guardar()\n", 0);
        return 0;
    } catch (...) {
        mensajeInfo("Hubo un error al guardar el articulo");
        throw -1;
    } // end try
}


int ArticuloView::borrar() {
    _depura ("ArticuloView::borrar", 0);
    try {
        _depura("ArticuloView::borrar", 0);
        m_companyact->begin();
        /// Disparamos los plugins
        int res = g_plugins->lanza("ArticuloView_borrar", this);
        if (res != 0)
            return res;
        m_componentes->borrar();
        Articulo::borrar();
        m_companyact->commit();
        close();
        _depura("END ArticuloView::borrar", 0);
        return 0;
    } catch (...) {
        mensajeInfo("Error en el borrado del articulo");
        m_companyact->rollback();
        return 0;
    }
}


void ArticuloView::on_mui_cambiarimagen_clicked() {
    _depura("ArticuloView::INIT_s_cambiarimagen()\n", 0);
    m_archivoimagen = QFileDialog::getOpenFileName(
                          this,
                          tr("Abrir ventana de archivo"),
                          "",
                          tr("Imagenes (*.jpg)"));

    m_imagen->setPixmap(QPixmap(m_archivoimagen));
    _depura("ArticuloView::END_s_cambiarimagen()\n", 0);
}


void ArticuloView::closeEvent( QCloseEvent *e) {
    _depura("ArticuloView::closeEvent", 0);
    if (dialogChanges_hayCambios()) {
        int val = QMessageBox::warning(this,
                                       tr("Guardar articulo"),
                                       tr("Desea guardar los cambios?"),
                                       tr("&Si"), tr("&No"), tr("&Cancelar"), 0, 2);
        if (val == 0)
            try {
                guardar();
            } catch (...) {
                e->ignore();
            } // end catch
        if (val == 2)
            e->ignore();
    } // end if
    _depura("END ArticuloView::closeEvent", 0);
}


void ArticuloView::on_mui_aceptar_clicked() {
    try {
        guardar();
        close();
    } catch (...) {}
}

