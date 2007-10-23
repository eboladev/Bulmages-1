/***************************************************************************
 *   Copyright (C) 2003 by Tomeu Borras Riera                              *
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

#include "ccosteview.h"
#include "empresa.h"
#include "selectccosteview.h"
#include "subform2bc.h"

#define COL_NOM_COSTE  0
#define COL_DESC_COSTE 1
#define COL_IDC_COSTE  2


/// Esta clase se encarga de presentar los centros de coste, la ventana, y
/// de controlar la inserci&oacute;n de nuevos centros de coste, borrarlos, etc.
/**
\param emp
\param parent
**/
ccosteview::ccosteview(Empresa  *emp, QWidget *parent)
        : FichaBc(emp, parent) {
    _depura("ccosteview::ccosteview", 0);
    setAttribute(Qt::WA_DeleteOnClose);
    setupUi(this);

    setTitleName(tr("Centro de Coste"));
    /// Etablecemos cual va a ser la tabla para obtener los permisos
    setDBTableName("c_coste");
    setDBCampoId("idc_coste");
    addDBCampo("idc_coste", DBCampo::DBint, DBCampo::DBPrimaryKey, tr("Id. Centro Coste"));
    addDBCampo("nombre", DBCampo::DBvarchar, DBCampo::DBNotNull, tr("Nombre"));
    addDBCampo("descripcion", DBCampo::DBvarchar, DBCampo::DBNotNull, tr("Descripcion"));
//    addDBCampo("codigo", DBCampo::DBvarchar, DBCampo::DBNotNull, tr("Codigo"));
//    addDBCampo("imputacion", DBCampo::DBdate, DBCampo::DBNothing, QApplication::translate("AlbaranCliente", "Fecha albaran"));



    /// Inicializamos el subformulario de centros distribuidos
    mui_cdistribuidos->setEmpresaBase(emp);
    mui_cdistribuidos->setDBTableName("c_costedist");
    mui_cdistribuidos->setFileConfig("ccostedist");
    mui_cdistribuidos->setDBCampoId("idc_costedist");
    mui_cdistribuidos->addSHeader("nomc_coste", DBCampo::DBvarchar, DBCampo::DBNoSave, SHeader::DBNone, tr("Nombre Destinatario"));
    mui_cdistribuidos->addSHeader("porcentc_costedist", DBCampo::DBnumeric, DBCampo::DBNotNull, SHeader::DBNone, tr("Porcentaje"));
    mui_cdistribuidos->addSHeader("idc_costedist", DBCampo::DBint, DBCampo::DBPrimaryKey, SHeader::DBNoWrite | SHeader::DBNoView, tr("Id"));
    mui_cdistribuidos->addSHeader("idc_coste", DBCampo::DBint, DBCampo::DBNothing, SHeader::DBNoWrite | SHeader::DBNoView, tr("Destinatario"));
    mui_cdistribuidos->addSHeader("iddestc_coste", DBCampo::DBint, DBCampo::DBNotNull, SHeader::DBNoWrite | SHeader::DBNoView, tr("Origen"));
    mui_cdistribuidos->setinsercion(TRUE);
    mui_cdistribuidos->setOrdenEnabled(FALSE);
    idc_coste = 0;


    /// Disparamos los plugins.
    int res = g_plugins->lanza("ccosteview_ccosteview", this);
    if (res != 0) {
        return;
    } // end if


    mui_list->setColumnCount(3);
    QStringList headers;
    headers << tr("Nombre") << tr("Descripcion") << tr("Id centro de coste");
    mui_list->setColumnWidth(0, 200);
    mui_list->setHeaderLabels(headers);
    mui_list->setColumnHidden(COL_IDC_COSTE, TRUE);
    dialogChanges_setQObjectExcluido(mui_list);
    dialogChanges_cargaInicial();
    meteWindow(windowTitle(), this);
    repintar();
    _depura("END ccosteview::ccosteview", 0);
}


///
/**
**/
ccosteview::~ccosteview() {
    _depura("ccosteview::~ccosteview", 0);
    empresaBase()->sacaWindow(this);
    _depura("END ccosteview::~ccosteview", 0);
}


///
/**
**/
void ccosteview::repintar() {
    _depura("ccosteview::pintar", 0);
    QTreeWidgetItem *it;
    QMap <int, QTreeWidgetItem *> Lista;
    int padre;
    int idc_coste1 = 0;
    cursor2 *cursoraux1, *cursoraux2;

    /// Vaciamos el &aacute;rbol.
    mui_list->clear();

    /// Cargamos las cuentas de primer nivel.
    cursoraux1 = empresaBase()->cargacursor("SELECT * FROM c_coste WHERE padre ISNULL ORDER BY idc_coste");
    while (!cursoraux1->eof()) {
        padre = atoi( cursoraux1->valor("padre").toAscii());
        idc_coste1 = atoi( cursoraux1->valor("idc_coste").toAscii());
        it = new QTreeWidgetItem(mui_list);
        Lista[idc_coste1] = it;
        it->setText(COL_IDC_COSTE, cursoraux1->valor("idc_coste"));
        it->setText(COL_DESC_COSTE,cursoraux1->valor("descripcion"));
        it->setText(COL_NOM_COSTE, cursoraux1->valor("nombre"));
        mui_list->expandItem(it);
        cursoraux1->siguienteregistro();
    } // end while
    delete cursoraux1;

    /// Cargamos las cuentas hijas
    cursoraux2= empresaBase()->cargacursor("SELECT * FROM c_coste WHERE padre IS NOT NULL ORDER BY idc_coste");
    while (!cursoraux2->eof()) {
        padre = cursoraux2->valor("padre").toInt();
        idc_coste1 = cursoraux2->valor("idc_coste").toInt();
        if (Lista[padre]) {
            it = new QTreeWidgetItem(Lista[padre]);
            Lista[idc_coste1] = it;
            it->setText(COL_IDC_COSTE, cursoraux2->valor("idc_coste"));
            it->setText(COL_DESC_COSTE, cursoraux2->valor("descripcion"));
            it->setText(COL_NOM_COSTE, cursoraux2->valor("nombre"));
            mui_list->expandItem(it);
        } else {
            _depura(tr("Error en la carga de centros de coste"), 2);
        } // end if
        cursoraux2->siguienteregistro();
    } // end while
    delete cursoraux2;

//    if (idc_coste != 0) {
//        mostrarplantilla();
//    } // end if

    /// Ya que se han producido cambios en los centros de coste
    /// Se inicializa el selector de centros de coste.
    SelectCCosteView *scoste = empresaBase()->getselccostes();
    scoste->cargacostes();
    _depura("END ccosteview::pintar", 0);
}


///
/**
\param it
**/
void ccosteview::on_mui_list_itemClicked(QTreeWidgetItem *it, int) {
    _depura("ccosteview::on_mui_list_itemClicked", 0);
    int previdccoste = it->text(COL_IDC_COSTE).toInt();
    if (dialogChanges_hayCambios()) {
        if (QMessageBox::warning(this,
                                 tr("Guardar centro de coste"),
                                 tr("Desea guardar los cambios?"),
                                 tr("&Guardar"), tr("&Cancelar"), 0, 0, 1) == 0) {
            on_mui_guardar_clicked();
        } // end if
    } // end if
    idc_coste = previdccoste;
    mostrarplantilla();
    _depura("END ccosteview::on_mui_list_itemClicked", 0);
}


///
/**
**/
void ccosteview::mostrarplantilla() {
    _depura("ccosteview::mostrarplantilla", 0);

    QString query;

    _depura("mostrarplantilla", 2, QString::number(idc_coste));
    FichaBc::cargar(QString::number(idc_coste));

    query = "SELECT * FROM c_costedist LEFT JOIN (SELECT idc_coste AS idcc, nombre AS nomc_coste FROM c_coste) AS t1 ON c_costedist.idc_Coste = t1.idcc WHERE iddestc_coste=" + QString::number(idc_coste);
    mui_cdistribuidos->cargar(query);


    dialogChanges_cargaInicial();
    _depura("END ccosteview::mostrarplantilla", 0);
}


///
/**
**/
int ccosteview::guardar() {
    _depura("ccosteview::guardar", 0);
    FichaBc::guardar();

    mui_cdistribuidos->setColumnValue("iddestc_coste", QString::number(idc_coste));
    mui_cdistribuidos->guardar();

    dialogChanges_cargaInicial();
    repintar();
    _depura("END ccosteview::guardar", 0);
    return 0;
}


///
/**
**/
void ccosteview::on_mui_crear_clicked() {
    _depura("ccosteview::on_mui_crear_clicked", 0);
    /// Si se ha modificado el contenido advertimos y guardamos.
    if (dialogChanges_hayCambios()) {
        if (QMessageBox::warning(this,
                                 tr("Guardar centro de coste"),
                                 tr("Desea guardar los cambios?"),
                                 QMessageBox::Ok,
                                 QMessageBox::Cancel) == QMessageBox::Ok) {
            on_mui_guardar_clicked();
        } // end if
    } // end if

    QString query;
    QTreeWidgetItem *it;
    it = mui_list->currentItem();
    if (it) {
        idc_coste = atoi(it->text(COL_IDC_COSTE).toAscii());
        query.sprintf("INSERT INTO c_coste (padre, nombre, descripcion) VALUES (%d, 'Nuevo centro de coste', 'Escriba su descripcion')", idc_coste);
        empresaBase()->begin();
        empresaBase()->ejecuta(query);
    } else {
        query.sprintf("INSERT INTO c_coste (nombre, descripcion) VALUES ('Nuevo centro de coste', 'Escriba su descripcion')");
        empresaBase()->begin();
        empresaBase()->ejecuta(query);
    } // end if
    query.sprintf("SELECT MAX(idc_coste) AS id_coste FROM c_coste");
    cursor2 *cur = empresaBase()->cargacursor(query);
    idc_coste = atoi(cur->valor("id_coste").toAscii());
    delete cur;
    empresaBase()->commit();
    repintar();
    _depura("END ccosteview::on_mui_crear_clicked", 0);
}


///
/**
**/
void ccosteview::on_mui_borrar_clicked() {
    _depura("ccosteview::on_mui_borrar_clicked", 0);
    try {
        switch (QMessageBox::warning(this,
                                     tr("Borrar centro de coste"),
                                     tr("Se va a borrar el centro de coste.\nEsta operacion puede ocasionar perdida de datos."),
                                     tr("&Borrar"), tr("&Cancelar"), 0, 0, 1)) {
        case 0: /// Retry clicked or Enter pressed.
            QString query;
            query.sprintf("DELETE FROM c_coste WHERE idc_coste = %d", idc_coste);
            empresaBase()->begin();
            empresaBase()->ejecuta(query);
            empresaBase()->commit();
            idc_coste = 0;
            repintar();
        } // end switch
    } catch (...) {
        mensajeInfo(tr("No se pudo borrar el Centro de Coste"));
        empresaBase()->rollback();
        return;
    } // end try
    _depura("END ccosteview::on_mui_borrar_clicked", 0);
}



