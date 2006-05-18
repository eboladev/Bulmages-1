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

#include <Q3Table>
#include <QMessageBox>
#include <QFile>
#include <QLineEdit>
#include <QCheckBox>
#include <QComboBox>
#include <QTextStream>
#include <QMenu>
#include <Q3FileDialog>

#include "articulolist.h"
#include "pgimportfiles.h"
#include "company.h"
#include "articuloview.h"
#include "busquedafamilia.h"
#include "busquedatipoarticulo.h"
#include "qtable1.h"
#include "funcaux.h"


ArticuloList::ArticuloList(company *comp, QWidget *parent, const char *name, Qt::WFlags flag, edmode editmodo)  : QWidget(parent, name, flag)  , pgimportfiles(comp) {
    _depura("ArticuloList::INIT_ArticuloList()\n",0);
    setupUi(this);
    m_companyact = comp;
    m_tipoarticulo->setcompany(comp);
    m_familia->setcompany(comp);
    mui_list->setcompany(comp);
    presenta();
    m_modo=editmodo;
    if (m_modo == EditMode)
        comp->meteWindow(tr("Articulos"), this);
    hideBusqueda();
    _depura("ArticuloList::END_ArticuloList()\n", 0);
}


void ArticuloList::presenta() {
    _depura("ArticuloList::INIT_presenta()\n", 0);
    cursor2 * cur= m_companyact->cargacursor(formaQuery());
    mui_list->cargar(cur);
    delete cur;
    _depura("ArticuloList::END_presenta()\n", 0);
}


void ArticuloList::editArticle(int  row) {
    _depura("ArticuloList::INIT_editArticle()\n", 0);
    mdb_idarticulo = mui_list->DBvalue("idarticulo", row);
    mdb_nomarticulo = mui_list->DBvalue("nomarticulo", row);
    mdb_codigocompletoarticulo = mui_list->DBvalue("codigocompletoarticulo",row);
    if (m_modo == EditMode ) {
        ArticuloView *art = m_companyact->newArticuloView();
        m_companyact->m_pWorkspace->addWindow(art);
        /// Si la carga no va bien entonces terminamos.
        if (art->cargar(mdb_idarticulo))
            return;
        art->hide();
        art->show();
    } else {
        close();
        emit(selected(mdb_idarticulo));
    } // end if
    _depura("ArticuloList::END_editArticle()\n", 0);
}


void ArticuloList::on_mui_editar_clicked() {
    _depura("ArticuloList::INIT_s_editArticle()\n", 0);
    int a = mui_list->currentRow();
    if (a < 0) {
        _depura("Debe seleccionar una linea", 2);
        return;
    } // end if
    editArticle(a);
    _depura("ArticuloList::END_s_editArticle()\n", 0);
}


ArticuloList::~ArticuloList() {
    _depura("ArticuloList::INIT_destructor()\n", 0);
    if(m_modo == EditMode)
        m_companyact->sacaWindow(this);
    _depura("ArticuloList::END_destructor()\n", 0);
}


void ArticuloList::on_mui_borrar_clicked() {
    _depura("ArticuloList::INIT_removeArticle()\n", 0);
    if ( QMessageBox::Yes == QMessageBox::question(this, tr("Borrar articulo"),
            tr("Esta a punto de borrar un articulo. Estos datos pueden dar problemas."),
            QMessageBox::Yes, QMessageBox::No)) {
        QString SQLQuery = "DELETE FROM articulo WHERE idarticulo=" + mui_list->DBvalue("idarticulo");
        m_companyact->begin();
        int error = m_companyact->ejecuta(SQLQuery);
        if (error) {
            m_companyact->rollback();
            return;
        } // end if
        m_companyact->commit();
        presenta();
    } // end if
    _depura("ArticuloList::END_removeArticle()\n", 0);
}


QString ArticuloList::formaQuery() {
    _depura("ArticuloList::INIT_formaQuery()\n", 0);
    QString query = "";
    query += "SELECT * FROM articulo NATURAL LEFT JOIN tipo_iva NATURAL LEFT JOIN tipo_articulo WHERE 1=1 ";
    if(m_presentablearticulo->isChecked())
        query += " AND presentablearticulo ";
    if(m_usadoarticulo->isChecked())
        query += " AND idarticulo IN (SELECT DISTINCT idarticulo FROM lpresupuesto"
                 " UNION SELECT DISTINCT idarticulo FROM lpedidocliente"
                 " UNION SELECT DISTINCT idarticulo FROM lalbaran"
                 " UNION SELECT DISTINCT idarticulo FROM lfactura"
                 " UNION SELECT DISTINCT idarticulo FROM lpedidoproveedor"
                 " UNION SELECT DISTINCT idarticulo FROM lalbaranp"
                 " UNION SELECT DISTINCT idarticulo FROM lfacturap"
                 ") ";
    if(m_filtro->text() != "")
        query += " AND lower(nomarticulo) LIKE lower('%" + m_filtro->text() + "%') ";
    if(m_familia->idfamilia() != "" ) {
        query += " AND idfamilia IN (SELECT idfamilia FROM familia WHERE codigocompletofamilia LIKE '"+m_familia->codigocompletofamilia()+"%')";
    } // end if
    if (m_tipoarticulo->idtipo_articulo() != "") {
        query += " AND idtipo_articulo = "+m_tipoarticulo->idtipo_articulo();
    } // end if
    query +=" ORDER BY codigocompletoarticulo";
    return (query);
    _depura("ArticuloList::END_formaQuery()\n", 0);
}


QString ArticuloList::detalleArticulos() {
    _depura("ArticuloList::INIT_detalleArticulos()\n", 0);
    QString texto = "";
    cursor2 *cur = m_companyact->cargacursor(formaQuery());
    while(!cur->eof()) {
        texto += "<blockTable style=\"tabla1\">\n";
        texto += "<tr>\n";
        texto += "<td><h1>" + XMLProtect(cur->valor("nomarticulo")) + "</h1>";
        texto += "<para>" + XMLProtect(cur->valor("obserarticulo")) + "</para></td></tr><tr>\n";
        QString file = confpr->valor(CONF_DIR_IMG_ARTICLES) + XMLProtect(cur->valor("codigocompletoarticulo")) + ".jpg";
        QFile f(file);
        if (f.exists()) {
            texto += "	<td><illustration x=\"0\" y=\"0\" height=\"5cm\">\n"
                     "<image file=\"" + confpr->valor(CONF_DIR_IMG_ARTICLES) + 
                     XMLProtect(cur->valor("codigocompletoarticulo")) +
                     ".jpg\" x=\"0\" y=\"0\" height=\"5cm\"/>\n"
                     "</illustration></td>\n";
        } else {
            texto += "<td></td>\n";
        }
        texto += "</tr>\n";
        texto += "</blockTable>";
        cur->siguienteregistro();
    } // end while
    delete cur;
    return texto;
    _depura("ArticuloList::END_detalleArticulos()\n", 0);
}


void ArticuloList::Imprimir() {
    _depura("ArticuloList::INIT_Imprimir()\n", 0);
    QString archivo=confpr->valor(CONF_DIR_OPENREPORTS) + "articulos.rml";
    QString archivod = confpr->valor(CONF_DIR_USER) + "articulos.rml";
    QString archivologo=confpr->valor(CONF_DIR_OPENREPORTS) + "logo.jpg";

    /// Copiamos el archivo
#ifdef WINDOWS
    archivo = "copy " + archivo + " " + archivod;
#else
    archivo = "cp " + archivo + " " + archivod;
#endif

    system (archivo.ascii());

    /// Copiamos el logo
#ifdef WINDOWS
    archivologo = "copy " + archivologo + " " + confpr->valor(CONF_DIR_USER) + "logo.jpg";
#else
    archivologo = "cp " + archivologo + " " + confpr->valor(CONF_DIR_USER) + "logo.jpg";
#endif

    system (archivologo.ascii());
    QFile file;
    file.setName( archivod );
    file.open( QIODevice::ReadOnly );
    QTextStream stream(&file);
    QString buff = stream.read();
    file.close();
    QString texto;
    /// Linea de totales del presupuesto
    buff.replace("[detallearticulos]",detalleArticulos());
    if (file.open(QIODevice::WriteOnly)) {
        QTextStream stream(&file);
        stream << buff;
        file.close();
    } // end if
    invocaPDF("articulos");
    _depura("ArticuloList::END_Imprimir()\n", 0);
}


void ArticuloList::s_imprimir1() {
    _depura("ArticuloList::INIT_s_imprimir1()\n", 0);
    QString archivo = confpr->valor(CONF_DIR_OPENREPORTS) + "articulos1.rml";
    QString archivod = confpr->valor(CONF_DIR_USER) + "articulos1.rml";
    QString archivologo = confpr->valor(CONF_DIR_OPENREPORTS) + "logo.jpg";

    /// Copiamos el archivo
#ifdef WINDOWS
    archivo = "copy " + archivo + " " + archivod;
#else
    archivo = "cp " + archivo + " " + archivod;
#endif

    system (archivo.ascii());

    /// Copiamos el logo
#ifdef WINDOWS
    archivologo = "copy " + archivologo + " " + confpr->valor(CONF_DIR_USER) + "logo.jpg";
#else
    archivologo = "cp " + archivologo + " " + confpr->valor(CONF_DIR_USER) + "logo.jpg";
#endif

    system (archivologo.ascii());

    QFile file;
    file.setName( archivod );
    file.open( QIODevice::ReadOnly );
    QTextStream stream(&file);
    QString buff = stream.read();
    file.close();
    QString fitxersortidatxt;
    /// Linea de totales del presupuesto
    fitxersortidatxt = "<blockTable style=\"tabla\" repeatRows=\"1\">";
    fitxersortidatxt += mui_list->imprimir();
    fitxersortidatxt += "</blockTable>";
    buff.replace("[story]", fitxersortidatxt);
    if (file.open( QIODevice::WriteOnly)) {
        QTextStream stream(&file);
        stream << buff;
        file.close();
    }

    invocaPDF("articulos1");
    _depura("ArticuloList::END_s_imprimir1()\n", 0);
}


void ArticuloList::on_mui_exportar_clicked() {
    _depura("ArticuloList::INIT_s_exportar()\n", 0);

    QFile filexml (Q3FileDialog::getSaveFileName(confpr->valor(CONF_DIR_USER), "Clientes (*.xml)", this, "select file", "Elija el Archivo"));
    if(filexml.open(QIODevice::WriteOnly)) {
        bulmafact2XML(filexml, IMPORT_ARTICULOS);
        filexml.close();
    } else {
        _depura("ERROR AL ABRIR EL ARCHIVO\n", 2);
    } // end if

    _depura("ArticuloList::END_s_exportar()\n", 0);
}


void ArticuloList::on_mui_importar_clicked() {
    _depura("ArticuloList::INIT_s_importar()\n", 0);
    QFile filexml (Q3FileDialog::getOpenFileName(confpr->valor(CONF_DIR_USER), "Clientes (*.xml)", this, "select file", "Elija el Archivo"));
    if (filexml.open(QIODevice::ReadOnly))  {
        XML2BulmaFact(filexml, IMPORT_ARTICULOS);
        filexml.close();
        presenta();
    } else {
        _depura("ERROR AL ABRIR EL ARCHIVO\n", 2);
    } // end if
    _depura("ArticuloList::END_s_importar()\n", 0);
}


void ArticuloList::on_mui_list_cellDoubleClicked(int a, int) {
    editArticle(a);
}


void ArticuloList::on_mui_list_customContextMenuRequested(const QPoint &) {
    _depura("ArticuloList::on_mui_list_customContextMenuRequested", 0);
    int a = mui_list->currentRow();
    if ( a < 0)
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
ArticuloListSubForm::ArticuloListSubForm(QWidget *parent, const char *) : SubForm2Bf(parent) {
    setDBTableName("articulo");
    setDBCampoId("idarticulo");
    addSHeader("idarticulo", DBCampo::DBint, DBCampo::DBNotNull | DBCampo::DBPrimaryKey, SHeader::DBNoView | SHeader::DBNoWrite, tr("ID articulo"));
    addSHeader("codigocompletoarticulo", DBCampo::DBvarchar, DBCampo::DBNoSave, SHeader::DBNone | SHeader::DBNoWrite, tr("Codigo completo del articulo"));
    addSHeader("nomarticulo", DBCampo::DBvarchar, DBCampo::DBNoSave, SHeader::DBNone | SHeader::DBNoWrite, tr("Nombre del articulo"));
    addSHeader("abrevarticulo", DBCampo::DBvarchar, DBCampo::DBNoSave, SHeader::DBNone | SHeader::DBNoWrite, tr("Descripcion abreviada del articulo"));
    addSHeader("obserarticulo", DBCampo::DBvarchar, DBCampo::DBNoSave, SHeader::DBNone | SHeader::DBNoWrite, tr("Observaciones sobre el articulo"));
    addSHeader("desctipo_articulo", DBCampo::DBvarchar, DBCampo::DBNoSave, SHeader::DBNone | SHeader::DBNoWrite, tr("Descripcion del tipo de articulo"));
    addSHeader("desctipo_iva", DBCampo::DBvarchar, DBCampo::DBNoSave, SHeader::DBNone | SHeader::DBNoWrite, tr("Descripcion tipo de I.V.A."));
    addSHeader("pvparticulo", DBCampo::DBvarchar, DBCampo::DBNoSave, SHeader::DBNone | SHeader::DBNoWrite, tr("P.V.P. articulo"));
    addSHeader("stockarticulo", DBCampo::DBvarchar, DBCampo::DBNoSave, SHeader::DBNone | SHeader::DBNoWrite, tr("Disponible en stock"));
    setinsercion(FALSE);
}

