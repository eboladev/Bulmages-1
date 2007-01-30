/***************************************************************************
 *   Copyright (C) 2004 by J. M. Estopa Rey                                *
 *   pepma@telefonica.net                                                  *
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
#include <QFile>
#include <QLineEdit>
#include <QCheckBox>
#include <QComboBox>
#include <QTextStream>
#include <QMenu>
#include <QFileDialog>

#include "articulolist.h"
#include "pgimportfiles.h"
#include "company.h"
#include "articuloview.h"
#include "busquedafamilia.h"
#include "busquedatipoarticulo.h"
#include "funcaux.h"


/** Constructor de la ventana de listado de articulos
    Inicializa todos los componentes, propaga el puntero a m_company
    Configura la pantalla para que haga una cosa u otra en funcion de m_modo.
    Si estamos en modo edicion mete la ventana en el workSpace. En caso contrario
    la presenta como un 'popup'.
    Hace la presentacion inicial.
*/
ArticuloList::ArticuloList(company *comp, QWidget *parent, Qt::WFlags flag, edmode editmodo)
        : Ficha(parent, flag), pgimportfiles(comp) {
    _depura("ArticuloList::INIT_ArticuloList()\n", 0);
    setupUi(this);
    m_companyact = comp;
    m_tipoarticulo->setcompany(comp);
    m_familia->setcompany(comp);
    mui_list->setcompany(comp);
    m_usadoarticulo->setCheckState(Qt::Unchecked);

    m_modo = editmodo;
    if (m_modo == EditMode) {
        m_companyact->meteWindow(windowTitle(), this);
    } else {
        setWindowTitle(tr("Selector de articulos"));
        mui_editar->setHidden(TRUE);
        mui_crear->setHidden(TRUE);
        mui_borrar->setHidden(TRUE);
        mui_exportar->setHidden(TRUE);
        mui_importar->setHidden(TRUE);
        mui_imprimir->setHidden(TRUE);
    } // end if
    presenta();
    hideBusqueda();
    _depura("ArticuloList::END_ArticuloList()\n", 0);
}


/** Hace la carga del subformulario para presentar el listado.
*/
void ArticuloList::presenta() {
    _depura("ArticuloList::INIT_presenta()\n", 0);
    mui_list->cargar(formaQuery());
    _depura("ArticuloList::END_presenta()\n", 0);
}

/** Se encarga de la accion preseleccionada al hacer doble click o al darle
    al boton de editar. 
    Si estamos en modo seleccion actualiza el valor de los campos de seleccion y
    cierra la ventana.
    Si estamos en modo edicion abre una instancia de ArticuloView y lo carga con el
    valor seleccionado.
*/
/// \TODO: este metodo deberia ser editar
void ArticuloList::editArticle(int row) {
    _depura("ArticuloList::editArticle()\n", 0);
    mdb_idarticulo = mui_list->DBvalue("idarticulo", row);
    mdb_nomarticulo = mui_list->DBvalue("nomarticulo", row);
    mdb_codigocompletoarticulo = mui_list->DBvalue("codigocompletoarticulo", row);
    if (m_modo == EditMode) {
        ArticuloView *art = m_companyact->newArticuloView();
        m_companyact->m_pWorkspace->addWindow(art);
        /// Si la carga no va bien entonces terminamos.
        if (art->cargar(mdb_idarticulo)) {
            delete art;
            return;
        } // end if
        art->hide();
        art->show();
    } else {
        close();
        emit(selected(mdb_idarticulo));
    } // end if
    _depura("ArticuloList::END_editArticle()\n", 0);
}


/** SLOT que responde al boton de editar articulo
    Comprueba que haya un elemento seleccionado y llama a editArticle.
*/
void ArticuloList::on_mui_editar_clicked() {
    _depura("ArticuloList::INIT_s_editArticle()\n", 0);
    int a = mui_list->currentRow();
    if (a < 0) {
        mensajeInfo(tr("Tiene que seleccionar un articulo"));
        return;
    } // end if
    editArticle(a);
    _depura("ArticuloList::END_s_editArticle()\n", 0);
}


/** No requiere de ninguna accion adicional */
ArticuloList::~ArticuloList() {
    _depura("ArticuloList::INIT_destructor()\n", 0);
    _depura("ArticuloList::END_destructor()\n", 0);
}


/** SLOT que responde a la pulsacion del boton borrar
    Verifica que queremos borrar el articulo y lo borra tras preguntar.
*/
/// \TODO: Deberia crear una instancia del articulo e invocar a su metodo de borrado
/// ya que si hay algun plugin puede que no se vea afectado por este metodo.
void ArticuloList::on_mui_borrar_clicked() {
    _depura("ArticuloList::on_mui_borrar_clicked", 0);
    int a = mui_list->currentRow();
    if (a < 0) {
        mensajeInfo(tr("Tiene que seleccionar un articulo"));
        return;
    } // end if
    try {
        QString idarticulo = mui_list->DBvalue("idarticulo");
        if (QMessageBox::Yes == QMessageBox::question(this,
                tr("Borrar articulo"),
                tr("Esta a punto de borrar un articulo. Estos datos pueden dar problemas."),
                QMessageBox::Yes, QMessageBox::No)) {
            QString SQLQuery = "DELETE FROM articulo WHERE idarticulo = " + idarticulo;
            int error = m_companyact->ejecuta(SQLQuery);
            if (error)
                throw -1;
            presenta();
        } // end if
        _depura("END ArticuloList::on_mui_borrar_clicked", 0);
    } catch (...) {
        mensajeInfo(tr("Error al borrar el articulo"));
    } // end try
}


/** Metodo auxiliar que forma el query de presentacion a partir de los datos
    de filtrado.
    Es usado por presentar
*/
QString ArticuloList::formaQuery() {
    _depura("ArticuloList::formaQuery", 0);
    QString query = "";
    query += "SELECT * FROM articulo NATURAL LEFT JOIN tipo_iva NATURAL LEFT JOIN tipo_articulo WHERE 1 = 1 ";
    if (m_presentablearticulo->isChecked())
        query += " AND presentablearticulo ";
    if (m_usadoarticulo->isChecked())
        query += " AND idarticulo IN (SELECT DISTINCT idarticulo FROM lpresupuesto"
                 " UNION SELECT DISTINCT idarticulo FROM lpedidocliente"
                 " UNION SELECT DISTINCT idarticulo FROM lalbaran"
                 " UNION SELECT DISTINCT idarticulo FROM lfactura"
                 " UNION SELECT DISTINCT idarticulo FROM lpedidoproveedor"
                 " UNION SELECT DISTINCT idarticulo FROM lalbaranp"
                 " UNION SELECT DISTINCT idarticulo FROM lfacturap"
                 ") ";
    if (m_filtro->text() != "")
        query += " AND lower(nomarticulo) LIKE lower('%" + m_filtro->text() + "%') ";
    if (m_familia->idfamilia() != "" ) {
        query += " AND idfamilia IN (SELECT idfamilia FROM familia WHERE codigocompletofamilia LIKE '" + m_familia->codigocompletofamilia() + "%')";
    } // end if
    if (m_tipoarticulo->idtipo_articulo() != "") {
        query += " AND idtipo_articulo = " + m_tipoarticulo->idtipo_articulo();
    } // end if
    query +=" ORDER BY codigocompletoarticulo";
    return (query);
    _depura("ArticuloList::END_formaQuery()\n", 0);
}


/** Metodo auxiliar para la creacion del catalogo de articulos
*/
QString ArticuloList::detalleArticulos() {
    _depura("ArticuloList::detalleArticulos", 0);
    QString texto = "";
    cursor2 *cur = m_companyact->cargacursor(formaQuery());
    while (!cur->eof()) {
        texto += "<blockTable style=\"tabla1\">\n";
        texto += "<tr>\n";
        texto += "<td><h1>" + XMLProtect(cur->valor("nomarticulo")) + "</h1>";
        texto += "<para>" + XMLProtect(cur->valor("obserarticulo")) + "</para></td></tr><tr>\n";
        QString file = confpr->valor(CONF_DIR_IMG_ARTICLES) + XMLProtect(cur->valor("codigocompletoarticulo")) + ".jpg";
        QFile f(file);
        if (f.exists()) {
            texto += "<td><!-- illustration x=\"0\" y=\"0\" height=\"5cm\" -->\n"
                     "<image file=\"" + confpr->valor(CONF_DIR_IMG_ARTICLES) +
                     XMLProtect(cur->valor("codigocompletoarticulo")) +
                     ".jpg\" x=\"0\" y=\"0\" height=\"5cm\"/>\n"
                     "<!-- /illustration --></td>\n";
        } else {
            texto += "<td></td>\n";
        }
        texto += "</tr>\n";
        texto += "</blockTable>";
        cur->siguienteregistro();
    } // end while
    delete cur;
    _depura("END ArticuloList::detalleArticulos()\n", 0);
    return texto;
}

/** Impresion del catalogo de articulos
    utiliza la plantilla articulos1.rml para crear un catalogo de articulos.
    crea una copia de artiulos1.rml en el directorio ~/.bulmages
    hace la sustitucion y ampliacion en dicho archivo.
    Al final llama al metodo de bgtrml2pdf.py para generar un PDF 
    y lo muestra.
*/
void ArticuloList::on_mui_imprimirCatalogo_clicked() {
    _depura("ArticuloList::on_mui_imprimirCatalogo_clicked()\n", 0);
    QString archivo = confpr->valor(CONF_DIR_OPENREPORTS) + "articulos1.rml";
    QString archivod = confpr->valor(CONF_DIR_USER) + "articulos.rml";
    QString archivod1 = confpr->valor(CONF_DIR_USER) + "articulos1.rml";
    QString archivologo = confpr->valor(CONF_DIR_OPENREPORTS) + "logo.jpg";
    /// Copiamos el archivo.
#ifdef WINDOWS

    archivo = "copy " + archivo + " " + archivod;
#else

    archivo = "cp " + archivo + " " + archivod;
#endif

    system (archivo.toAscii().constData());
    /// Copiamos el logo.
#ifdef WINDOWS

    archivologo = "copy " + archivologo + " " + confpr->valor(CONF_DIR_USER) + "logo.jpg";
#else

    archivologo = "cp " + archivologo + " " + confpr->valor(CONF_DIR_USER) + "logo.jpg";
#endif

    system(archivologo.toAscii().constData());
    QFile file;
    file.setFileName(archivod);
    file.open(QIODevice::ReadOnly);
    QTextStream stream(&file);
    QString buff = stream.readAll();
    file.close();
    QString texto;
    /// Linea de totales del presupuesto.
    buff.replace("[detallearticulos]", detalleArticulos());
    if (file.open(QIODevice::WriteOnly)) {
        QTextStream stream(&file);
        stream << buff;
        file.close();
    } // end if

    QString iconv = "iconv -tUTF8 -c " + archivod + " > " + archivod1;
    system(iconv.toAscii().constData());
    invocaPDF("articulos1");
    _depura("END ArticuloList::on_mui_imprimirCatalogo_clicked", 0);
}


/** La impresion del listado esta completamente delegada en SubForm3.
*/
void ArticuloList::s_imprimir1() {
    _depura("ArticuloList::INIT_s_imprimir1()\n", 0);
    mui_list->imprimirPDF(tr("Listado de articulos"));
    _depura("ArticuloList::END_s_imprimir1()\n", 0);
}


/** SLOT que exporta el listado de articulos a formato XML.
*/
void ArticuloList::on_mui_exportar_clicked() {
    _depura("ArticuloList::on_mui_exportar_clicked", 0);
    QFile filexml(QFileDialog::getSaveFileName(this,
                  tr("Elija el archivo"),
                  confpr->valor(CONF_DIR_USER),
                  tr("Clientes (*.xml)")));

    if(filexml.open(QIODevice::WriteOnly)) {
        bulmafact2XML(filexml, IMPORT_ARTICULOS);
        filexml.close();
    } else {
        _depura("ERROR AL ABRIR EL ARCHIVO\n", 2);
    } // end if

    _depura("END ArticuloList::on_mui_exportar_clicked", 0);
}


/** SLOT que importa los articulos desde un fichero xml
    Pide al usuario que indique cual es el fichero
    Hace la importacion a traves de XML2BulmaFact.
*/
void ArticuloList::on_mui_importar_clicked() {
    _depura("ArticuloList::INIT_s_importar()\n", 0);
    QFile filexml(QFileDialog::getOpenFileName(this,
                  tr("Elija el archivo"),
                  confpr->valor(CONF_DIR_USER),
                  tr("Clientes (*.xml)")));

    if (filexml.open(QIODevice::ReadOnly)) {
        XML2BulmaFact(filexml, IMPORT_ARTICULOS);
        filexml.close();
        presenta();
    } else {
        _depura("ERROR AL ABRIR EL ARCHIVO\n", 2);
    } // end if
    _depura("ArticuloList::END_s_importar()\n", 0);
}


/** SLOT que responde al doble click sobre un elemento del listado
    llama a editArticle para que actue correspondientemente.
*/
void ArticuloList::on_mui_list_cellDoubleClicked(int a, int) {
    _depura("ArticuloList::on_mui_list_cellDoubleClicked", 0);
    editArticle(a);
    _depura("END ArticuloList::on_mui_list_cellDoubleClicked", 0);
}

/** \TODO: REVISAR ESTE METODO YA QUE NO PARECE SER EL ADECUADO
    EN LA LLAMADA DE SUBMENUS
*/
void ArticuloList::on_mui_list_customContextMenuRequested(const QPoint &) {
    _depura("ArticuloList::on_mui_list_customContextMenuRequested", 0);
    int a = mui_list->currentRow();
    if (a < 0)
        return;
    QMenu *popup = new QMenu(this);
    QAction *edit = popup->addAction(tr("Editar articulo"));
    QAction *del = popup->addAction(tr("Borrar articulo"));
    QAction *opcion = popup->exec(QCursor::pos());
    if (opcion == del)
        on_mui_borrar_clicked();
    if (opcion == edit)
        on_mui_editar_clicked();
    delete popup;
}


/// =============================================================================
///                    SUBFORMULARIO
/// =============================================================================
/** Prepara el subformulario para que trabaje con la tabla articulo
*/
ArticuloListSubForm::ArticuloListSubForm(QWidget *parent, const char *)
        : SubForm2Bf(parent) {
    _depura("ArticuloListSubForm::ArticuloListSubForm", 0);
    setDBTableName("articulo");
    setDBCampoId("idarticulo");
    addSHeader("idarticulo", DBCampo::DBint, DBCampo::DBNotNull | DBCampo::DBPrimaryKey, SHeader::DBNoView | SHeader::DBNoWrite, tr("ID articulo"));
    addSHeader("codigocompletoarticulo", DBCampo::DBvarchar, DBCampo::DBNoSave, SHeader::DBNone | SHeader::DBNoWrite, tr("Codigo completo del articulo"));
    addSHeader("nomarticulo", DBCampo::DBvarchar, DBCampo::DBNoSave, SHeader::DBNone | SHeader::DBNoWrite, tr("Nombre del articulo"));
    addSHeader("abrevarticulo", DBCampo::DBvarchar, DBCampo::DBNoSave, SHeader::DBNone | SHeader::DBNoWrite, tr("Descripcion abreviada del articulo"));
    addSHeader("obserarticulo", DBCampo::DBvarchar, DBCampo::DBNoSave, SHeader::DBNone | SHeader::DBNoWrite, tr("Observaciones sobre el articulo"));
    addSHeader("desctipo_articulo", DBCampo::DBvarchar, DBCampo::DBNoSave, SHeader::DBNone | SHeader::DBNoWrite, tr("Descripcion del tipo de articulo"));
    addSHeader("desctipo_iva", DBCampo::DBvarchar, DBCampo::DBNoSave, SHeader::DBNone | SHeader::DBNoWrite, tr("Descripcion tipo de I.V.A."));
    addSHeader("pvparticulo", DBCampo::DBnumeric, DBCampo::DBNoSave, SHeader::DBNone | SHeader::DBNoWrite, tr("P.V.P. articulo"));
    addSHeader("stockarticulo", DBCampo::DBnumeric, DBCampo::DBNoSave, SHeader::DBNone | SHeader::DBNoWrite, tr("Disponible en stock"));
    setinsercion(FALSE);
    setDelete(FALSE);
    setSortingEnabled(TRUE);
    _depura("END ArticuloListSubForm::ArticuloListSubForm", 0);
}

