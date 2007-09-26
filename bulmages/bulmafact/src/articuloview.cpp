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


/** Se encarga de la inicializacion de todos los componentes de la ventana de 'Articulo'.
    Inicializa la gestion de cambios para que se considere que no hay cambios realizados
    en la ventana. Mete la ventana en el 'workSpace'.
*/
/**
\param comp
\param parent
\return
**/
ArticuloView::ArticuloView(Company *comp, QWidget *parent)
        : FichaBf(comp, parent) {
    _depura("ArticuloView::ArticuloView", 0);
    setAttribute(Qt::WA_DeleteOnClose);
    try {
        setupUi(this);

        setTitleName(tr("Articulo"));
        setDBTableName("articulo");
        setDBCampoId("idarticulo");
        addDBCampo("idarticulo", DBCampo::DBint, DBCampo::DBPrimaryKey, QApplication::translate("Articulo", "Identificador"));
        addDBCampo("codarticulo", DBCampo::DBvarchar, DBCampo::DBNotNull, QApplication::translate("Articulo", "Codigo"));
        addDBCampo("nomarticulo", DBCampo::DBvarchar, DBCampo::DBNotNull, QApplication::translate("Articulo", "Nombre del articulo"));
        addDBCampo("abrevarticulo", DBCampo::DBvarchar, DBCampo::DBNothing, QApplication::translate("Articulo", "Abreviacion"));
        addDBCampo("obserarticulo", DBCampo::DBvarchar, DBCampo::DBNothing, QApplication::translate("Articulo", "Observaciones"));
        addDBCampo("presentablearticulo", DBCampo::DBboolean, DBCampo::DBNothing, QApplication::translate("Articulo", "Incluir en presentaciones"));
        addDBCampo("controlstockarticulo", DBCampo::DBboolean, DBCampo::DBNothing, QApplication::translate("Articulo", "Incluir en control de stock"));
        addDBCampo("idtipo_articulo", DBCampo::DBint, DBCampo::DBNothing, QApplication::translate("Articulo", "Tipo de articulo"));
        addDBCampo("idtipo_iva", DBCampo::DBint, DBCampo::DBNothing, QApplication::translate("Articulo", "Tipo de I.V.A."));
        addDBCampo("codigocompletoarticulo", DBCampo::DBvarchar, DBCampo::DBNoSave, QApplication::translate("Articulo", "Codigo completo"));
        addDBCampo("idfamilia", DBCampo::DBint, DBCampo::DBNotNull, QApplication::translate("Articulo", "Familia"));
        addDBCampo("stockarticulo", DBCampo::DBint, DBCampo::DBNothing, QApplication::translate("Articulo", "Stock"));
        addDBCampo("inactivoarticulo", DBCampo::DBboolean, DBCampo::DBNothing, QApplication::translate("Articulo", "Inactivo"));
        addDBCampo("pvparticulo", DBCampo::DBnumeric, DBCampo::DBNotNull, QApplication::translate("Articulo", "P.V.P. base"));
        addDBCampo("pesoundarticulo", DBCampo::DBnumeric, DBCampo::DBNotNull, QApplication::translate("Articulo", "Peso Unidad"));
        addDBCampo("volumenundarticulo", DBCampo::DBnumeric, DBCampo::DBNotNull, QApplication::translate("Articulo", "Volumen Unidad"));

        /// Disparamos los plugins.
        int res = g_plugins->lanza("ArticuloView_ArticuloView", this);
        if (res != 0) {
            return;
        } // end if

        mui_idfamilia->setEmpresaBase(comp);
        mui_idtipoarticulo->setEmpresaBase(comp);
        mui_idtipo_iva->setEmpresaBase(comp);
        m_componentes->setEmpresaBase(comp);
        dialogChanges_setQObjectExcluido(m_componentes);
        dialogChanges_setQObjectExcluido(m_componentes->mui_list);
        m_archivoimagen = "";
        m_imagen->setPixmap(QPixmap("/usr/share/bulmages/logopeq.png"));
        meteWindow(windowTitle(), this, FALSE);
        dialogChanges_cargaInicial();
    } catch (...) {
        mensajeInfo(tr("Error al crear el articulo"));
    } // end try
    _depura("END ArticuloView::ArticuloView", 0);
}


/** No requiere de ninguna actuacion especial. */
/**
**/
ArticuloView::~ArticuloView() {
    _depura("ArticuloView::~ArticuloView", 0);
    _depura("END ArticuloView::~ArticuloView", 0);
}


/** Se encarga de presentar la ventana con los datos cargados en la clase DBRecord.
    Tambien cambia el titulo de la ventana para que contenga la informacion correcta.
*/
/**
**/
void ArticuloView::pintarPost() {
    _depura("ArticuloView::pintar", 0);
    m_imagen->setPixmap(QPixmap(confpr->valor(CONF_DIR_IMG_ARTICLES) + mui_codigocompletoarticulo->text() + ".jpg"));
    _depura("END ArticuloView::pintar", 0);
}


/// Esta funci&oacute;n carga un art&iacute;culo de la base de datos y lo presenta.
/// Si el par&aacute;metro pasado no es un identificador v&aacute;lido entonces se pone
/// la ventana de edici&oacute;n en modo de inserci&oacute;n.
/**
\param idarticulo
\return
**/
int ArticuloView::cargarPost(QString idarticulo) {
    _depura("ArticuloView::cargar", 0);

    /// Disparamos los plugins.
    int res = g_plugins->lanza("ArticuloView_cargar", this);
    if (res != 0) {
        return 0;
    } // end if

    /// Cargamos los componentes.
    m_componentes->cargar(idarticulo);

    _depura("END ArticuloView::cargar", 0);
    return 0;
}


/** SLOT que responde a la finalizacion de edicion del codigocompleto del articulo.
    En cuyo caso lo que se hace es buscar un articulo que tenga dicho codigo y cargar su
    ficha.
*/
/**
\return
**/
void ArticuloView::on_mui_codigocompletoarticulo_editingFinished() {
    _depura("ArticuloView::on_m_codigocompletoarticulo_editingFinished", 0);

    if (mui_codigocompletoarticulo->text() == DBvalue("codigocompletoarticulo")) {
        return;
    } // end if
    QString SQlQuery = "SELECT * FROM articulo WHERE codigocompletoarticulo = '" + mui_codigocompletoarticulo->text() + "'";
    cursor2 *cur = empresaBase()->cargacursor(SQlQuery);
    if (!cur) return;
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
/**
\return
**/
int ArticuloView::guardarPost() {
    _depura("ArticuloView::guardarPost", 0);
    /// Guardamos la imagen, si es que existe.
    if (m_archivoimagen != "") {
        cursor2 *cur1 = empresaBase()->cargacursor("SELECT codigocompletoarticulo FROM articulo WHERE idarticulo = " + DBvalue("idarticulo"));
        if (!cur1) throw -1;
        QString cadena = "cp " + m_archivoimagen + " " + confpr->valor(CONF_DIR_IMG_ARTICLES) + cur1->valor("codigocompletoarticulo") + ".jpg";
        delete cur1;
        system(cadena.toAscii().constData());
    } // end if

    /// Guardamos la lista de componentes.
    m_componentes->setColumnValue("idarticulo", DBvalue("idarticulo"));
    if (m_componentes->guardar() != 0) {
        throw -1;
    } // end if
    /// Disparamos los plugins
    g_plugins->lanza("ArticuloView_guardar_post", this);

    _depura("END ArticuloView::guardarPost", 0);
    return 0;

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
/**
\return
**/
int ArticuloView::borrarPre() {
    _depura ("ArticuloView::borrar", 0);
    m_componentes->borrar();
    _depura("END ArticuloView::borrar", 0);
    return 0;
}


/** SLOT que responde a la pulsacion del boton de cambio de imagen.
    Abre la imagen y la almacenta al mismo tiempo que la presenta.
    Es el metodo de guardado quien determina como almacenarla.
*/
/**
**/
void ArticuloView::on_mui_cambiarimagen_clicked() {
    _depura("ArticuloView::INIT_s_cambiarimagen()", 0);
    m_archivoimagen = QFileDialog::getOpenFileName(
                          this,
                          tr("Abrir ventana de archivo"),
                          "",
                          tr("Imagenes (*.jpg)"));

    m_imagen->setPixmap(QPixmap(m_archivoimagen));
    _depura("ArticuloView::END_s_cambiarimagen()", 0);
}

