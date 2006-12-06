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
#include "funcaux.h"
#include "plugins.h"


/** Se encarga de la inicializacion de todos los componentes de la ventana de Articulo.
    Inicializa la gestion de cambios para que se considere que no hay camibos realizados en la ventana.
    Mete la ventana en el worSpace.
*/
ArticuloView::ArticuloView(company *comp, QWidget *parent)
        : Ficha(parent), Articulo(comp) {
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
        m_companyact->meteWindow(windowTitle(), this, FALSE);
        dialogChanges_cargaInicial();
    } catch (...) {
        mensajeInfo(tr("Error al crear el articulo"));
    } // end try
    _depura("END ArticuloView::ArticuloView", 0);
}

/** No requiere de ninguna actuacion especial. */
ArticuloView::~ArticuloView() {
    _depura("ArticuloView::~ArticuloView", 0);
    _depura("END ArticuloView::~ArticuloView", 0);
}


/** Saca la ventana del workSpace.
    Este metodo es invocado desde la clase Ficha.
*/
int ArticuloView::sacaWindow() {
    _depura("ArticuloView::sacaWindow", 0);
    m_companyact->sacaWindow(this);
    _depura("END ArticuloView::sacaWindow", 0);
    return 0;
}


/** Se encarga de presentar la ventana con los datos cargados en la clase DBRecord.
    Tambien cambia el titulo de la ventana para que contenga la informacion correcta.
*/
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
    _depura("END ArticuloView::pintar", 0);
}


/// Esta funci&oacute;n carga un art&iacute;culo de la base de datos y lo presenta.
/// Si el par&aacute;metro pasado no es un identificador v&aacute;lido entonces se pone
/// la ventana de edici&oacute;n en modo de inserci&oacute;n.
int ArticuloView::cargar(QString idarticulo) {
    _depura("ArticuloView::cargar()\n", 0);
    try {
        setDBvalue("idarticulo", idarticulo);

        /// Disparamos los plugins.
        int res = g_plugins->lanza("ArticuloView_cargar", this);
        if (res != 0) {
            return res;
        } // end if
        QString ivaType = "";
        Articulo::cargar(idarticulo);
        ivaType = DBvalue("idtipo_iva");

        int ret = cargarcomboiva(ivaType);
        if (ret) {
            throw -1;
        } // end if

	/// Cargamos los componentes.
        m_componentes->cargar(DBvalue("idarticulo"));
	
        /// Pintamos para que la visualizacion sea correcta con la carga.
	pintar();
//        setWindowTitle(tr("Articulo") + " " + DBvalue("codigocompletoarticulo"));

	/// Metemosl a ventana en el workSpace para que corrija el titulo.
        ret = m_companyact->meteWindow(windowTitle(), this);
        if (ret) {
            throw -1;
        } // end if

	/// Iniciamos el control de cambios para que se considere que no hay cambios realizados.
        dialogChanges_cargaInicial();


    } catch (...) {
        mensajeInfo(tr("Error en la carga del articulo"));
        return -1;
    } // end try
    _depura("END ArticuloView::cargar()\n", 0);
    return 0;
}


/// Hace la carga del combo-box de tipos de IVA para el art&iacute;culo.
/// \TODO: Debe crearse un Widget para tipos de IVA.
int ArticuloView::cargarcomboiva(QString idIva) {
    _depura("ArticuloView::cargarcomboiva", 0);

    m_cursorcombo = NULL;
    if (m_cursorcombo != NULL) {
        delete m_cursorcombo;
    } // end if
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
        } // end if
        m_combotipo_iva->addItem(m_cursorcombo->valor("desctipo_iva"));
        m_cursorcombo->siguienteregistro();
    } // end while
    if (i1 != 0) {
        m_combotipo_iva->setCurrentIndex(i1 - 1);
    } // end if

    _depura("END ArticuloView::cargarcomboiva", 0);
    return 0;
}


/** SLOT que se invoca al pulsar sobre el bot&oacute;n de borrar.
    Pregunta si se desea borrar y en caso afirmativo hace el borrado.
    Una vez acabo resetea el control de cambios para que se considere que no hay cambios.
*/
void ArticuloView::on_mui_borrar_clicked() {
    _depura("ArticuloView::on_mui_borrar_clicked", 0);

    if (DBvalue("idarticulo") != "") {
        if (QMessageBox::question(this,
                                  tr("Borrar articulo"),
                                  tr("Esta a punto de borrar un articulo. Desea continuar?"),
                                  tr("&Si"), tr("&No"), 0, 1, 0) == 0) {
            borrar();
            dialogChanges_cargaInicial();
        } // end if
    } // end if
    _depura("END ArticuloView::on_mui_borrar_clicked", 0);
}

/** SLOT que responde a la finalizacion de edicion del codigocompleto del articulo.
    En cuyo caso lo que se hace es buscar un articulo que tenga dicho codigo y cargar su 
    ficha.
*/
void ArticuloView::on_m_codigocompletoarticulo_editingFinished() {
    _depura("ArticuloView::on_m_codigocompletoarticulo_editingFinished", 0);

    QString SQlQuery = "SELECT * FROM articulo WHERE codigocompletoarticulo = '" + m_codigocompletoarticulo->text() + "'";
    cursor2 *cur = m_companyact->cargacursor(SQlQuery);
    if (!cur->eof()) {
        cargar(cur->valor("idarticulo"));
    } // end if
    delete cur;

    _depura("END ArticuloView::on_m_codigocompletoarticulo_editingFinished", 0);
}


/** Metodo de guardar la ficha. Guarda todos los componentes de la ficha.
    Si todo ha ido bien devuelve 0.
    Si hay alg&uacute;n error debe ser tratado con el manejo de excepciones catch.
    Si se produce algun error devuelve una excepcion -1.
    Si se ha cambiado la imagen la almacena en el directorio correspondiente.
*/
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

        if (Articulo::guardar() != 0) {
            throw -1;
        } // end if
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
        if (res != 0) {
            return res;
        } // end if
        dialogChanges_cargaInicial();

        _depura("END ArticuloView::guardar()\n", 0);
        return 0;
    } catch (...) {
        mensajeInfo("Hubo un error al guardar el articulo");
        throw -1;
    } // end try
}


/** Metodo de borrar un articulo. 
    Hace las comprobaciones necesarias para el guardado.
    Crea una transaccion de borrado para borrar primero los componentes y luego la ficha.
    Tambien lanza los plugins por si hay componentes adicionales en el borrado.
    Si todo ha ido bien cierra la ventana.
    NOTA: Este metodo no hace la pregunta de desea borrar los cambios ya que
    a veces interesa poder borrar sin preguntar. Por eso la preguna la hace
    on_mui_borrar_clicked().
*/
int ArticuloView::borrar() {
    _depura ("ArticuloView::borrar", 0);
    try {
        _depura("ArticuloView::borrar", 0);
        m_companyact->begin();
        /// Disparamos los plugins
        int res = g_plugins->lanza("ArticuloView_borrar", this);
        if (res != 0) {
            return res;
        } // end if
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


/** SLOT que responde a la pulsacion del boton de cambio de imagen.
    Abre la imagen y la almacenta al mismo tiempo que la presenta.
    Es el metodo de guardado quien determina como almacenarla.
*/
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


/** Este metodo responde a la pulsacion del boton aceptar.
*/
/// \TODO: Deberia estar implementado en la clase Ficha y ser eliminado de aqui.
void ArticuloView::on_mui_aceptar_clicked() {
    _depura("ArticuloView::on_mui_aceptar_clicked", 0);
    try {
        guardar();
        close();
    } catch (...) {}
    _depura("END ArticuloView::on_mui_aceptar_clicked", 0);
}

