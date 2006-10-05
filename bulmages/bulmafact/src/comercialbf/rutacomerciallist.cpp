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

#include "rutacomerciallist.h"
#include "rutacomercialincview.h"
#include "company.h"
#include "funcaux.h"
#include "configuracion.h"


RutaComercialList::RutaComercialList(QWidget *parent)
        : QWidget(parent) {
    setAttribute(Qt::WA_DeleteOnClose);
    setupUi(this);
    m_companyact = NULL;
    m_modo = 0;
    m_idpresupuesto = "";
    meteWindow(windowTitle(), this);
    hideBusqueda();
}


RutaComercialList::RutaComercialList(company *comp, QWidget *parent)
        : QWidget(parent) {
    setAttribute(Qt::WA_DeleteOnClose);
    setupUi(this);
    m_companyact = comp;
    m_cliente->setcompany(comp);
    presenta();
    m_modo = 0;
    m_idpresupuesto = "";
    hideBusqueda();
    meteWindow(windowTitle(), this);
}


RutaComercialList::~RutaComercialList() {
    _depura("RutaComercialList::~RutaComercialList", 0);
    m_companyact->sacaWindow(this);
}


void RutaComercialList::presenta() {
    _depura("RutaComercialList::presenta()\n", 0);
    QString SQLQuery = "SELECT * FROM (SELECT * FROM rutacomercial NATURAL LEFT JOIN incidenciacomercial UNION SELECT * FROM rutacomercial NATURAL RIGHT JOIN incidenciacomercial WHERE incidenciacomercial.idrutacomercial IS NULL) AS t1 NATURAL LEFT JOIN trabajador LEFT JOIN (SELECT * FROM cliente NATURAL LEFT JOIN zonacomercial) AS t2 ON t1.idcliente = t2.idcliente WHERE 1 = 1 " + generaFiltro();
    cursor2 *cur = m_companyact->cargacursor(SQLQuery);
    mui_list->cargar(cur);
    delete cur;
    _depura("end RutaComercialList::presenta()\n", 0);
}


QString RutaComercialList::generaFiltro() {
    /// Tratamiento de los filtros.
    QString filtro = "";
    if (m_cliente->idcliente() != "") {
        filtro += " AND t1.idcliente = " + m_cliente->idcliente();
    } // end if
    if (m_fechain->text() != "")
        filtro += " AND t1.fechaincidenciacomercial >= '" + m_fechain->text() + "' ";
    if (m_fechafin->text() != "")
        filtro += " AND t1.fechaincidenciacomercial <= '" + m_fechafin->text() + "' ";
    return (filtro);
}


void RutaComercialList::editar(int row) {
    _depura("RutaComercialList::editar", 0);
    QString idrutacomercial = mui_list->DBvalue("idrutacomercial", row);
    QString idincidenciacomercial = mui_list->DBvalue("idincidenciacomercial", row);
    RutaComercialIncView *rut = new RutaComercialIncView(m_companyact, NULL);
    if (rut->cargar(idrutacomercial, idincidenciacomercial))
        return;
    m_companyact->m_pWorkspace->addWindow(rut);
    rut->show();
    _depura("END RutaComercialList::editar", 0);
}


void RutaComercialList::on_mui_editar_clicked() {
    int a = mui_list->currentRow();
    if (a >= 0)
        editar(a);
    else
        _depura("Debe seleccionar una linea", 2);
}


void RutaComercialList::on_mui_crear_clicked() {
    _depura("RutaComercialList::on_mui_crear_clicked", 0);
    RutaComercialIncView *rut = new RutaComercialIncView(m_companyact, NULL);
    m_companyact->m_pWorkspace->addWindow(rut);
    rut->show();
    _depura("END RutaComercialList::on_mui_crear_clicked", 0);
}


void RutaComercialList::imprimir() {
    QString archivo = confpr->valor(CONF_DIR_OPENREPORTS) + "rutascomerciales.rml";
    QString archivod = confpr->valor(CONF_DIR_USER) + "rutascomerciales.rml";
    QString archivologo = confpr->valor(CONF_DIR_OPENREPORTS) + "logo.jpg";
    /// Copiamos el archivo.
#ifdef WINDOWS
    archivo = "copy " + archivo + " " + archivod;
#else

    archivo = "cp " + archivo + " " + archivod;
#endif
    system(archivo.toAscii());
    /// Copiamos el logo.
#ifdef WINDOWS
    archivologo = "copy " + archivologo + " " + confpr->valor(CONF_DIR_USER) + "logo.jpg";
#else
    archivologo = "cp " + archivologo + " " + confpr->valor(CONF_DIR_USER) + "logo.jpg";
#endif

    system(archivologo.toAscii());

    QFile file;
    file.setFileName(archivod);
    file.open(QIODevice::ReadOnly);
    QTextStream stream(&file);
    QString buff = stream.readAll();
    file.close();
    QString fitxersortidatxt;
    fitxersortidatxt = "<blockTable style=\"tabla\" repeatRows=\"1\">";
    fitxersortidatxt += mui_list->imprimir();
    fitxersortidatxt += "</blockTable>";

    buff.replace("[story]", fitxersortidatxt);

    if (file.open(QIODevice::WriteOnly)) {
        QTextStream stream(&file);
        stream << buff;
        file.close();
    } // end if
    /// Crea el pdf y lo muestra.
    invocaPDF("rutascomerciales");
}


void RutaComercialList::on_mui_borrar_clicked() {
    _depura("RutaComercialList::on_mui_borrar_clicked", 0);
    QString idrutacomercial = mui_list->DBvalue("idrutacomercial");
    QString idincidenciacomercial = mui_list->DBvalue("idincidenciacomercial");
    RutaComercialIncView *rut = new RutaComercialIncView(m_companyact, NULL);
    if (rut->cargar(idrutacomercial, idincidenciacomercial))
        return;
    rut->on_mui_borrar_clicked();
    _depura("END RutaComercialList::on_mui_borrar_clicked", 0);
}


/// =============================================================================
///                    SUBFORMULARIO
/// =============================================================================
RutaComercialListSubForm::RutaComercialListSubForm(QWidget *parent) : SubForm2Bf(parent) {
    setDBTableName("rutacomercial");
    setDBCampoId("idrutacomercial");
    addSHeader("cifcliente", DBCampo::DBvarchar, DBCampo::DBNoSave, SHeader::DBNone | SHeader::DBNoWrite, tr("CIF del cliente"));
    addSHeader("nomcliente", DBCampo::DBvarchar, DBCampo::DBNoSave, SHeader::DBNone | SHeader::DBNoWrite, tr("Nombre del cliente"));
    addSHeader("idzonacomercial", DBCampo::DBvarchar, DBCampo::DBNoSave, SHeader::DBNoView | SHeader::DBNoWrite, tr("ID zona comercial"));
    addSHeader("fecharutacomercial", DBCampo::DBvarchar, DBCampo::DBNoSave, SHeader::DBNone | SHeader::DBNoWrite, tr("Fecha ruta comercial"));
    addSHeader("fechaincidenciacomercial", DBCampo::DBvarchar, DBCampo::DBNoSave, SHeader::DBNone | SHeader::DBNoWrite, tr("Fecha incidencia comercial"));
    addSHeader("nomtrabajador", DBCampo::DBvarchar, DBCampo::DBNoSave, SHeader::DBNone | SHeader::DBNoWrite, tr("Nombre del trabajador"));
    addSHeader("apellidostrabajador", DBCampo::DBvarchar, DBCampo::DBNoSave, SHeader::DBNone | SHeader::DBNoWrite, tr("Apellidos del trabajador"));
    addSHeader("idcliente", DBCampo::DBvarchar, DBCampo::DBNoSave, SHeader::DBNoView | SHeader::DBNoWrite, tr("ID cliente"));
    addSHeader("idtrabajador", DBCampo::DBvarchar, DBCampo::DBNoSave, SHeader::DBNoView | SHeader::DBNoWrite, tr("ID trabajador"));
    addSHeader("idrutacomercial", DBCampo::DBvarchar, DBCampo::DBNoSave, SHeader::DBNoView | SHeader::DBNoWrite, tr("ID ruta comercial"));
    addSHeader("comentariosrutacomercial", DBCampo::DBvarchar, DBCampo::DBNoSave, SHeader::DBNone | SHeader::DBNoWrite, tr("Comentarios ruta comercial"));
    addSHeader("idincidenciacomercial", DBCampo::DBvarchar, DBCampo::DBNoSave, SHeader::DBNoView | SHeader::DBNoWrite, tr("ID incidencia comercial"));
    addSHeader("comentincidenciacomercial", DBCampo::DBvarchar, DBCampo::DBNoSave, SHeader::DBNone | SHeader::DBNoWrite, tr("Comentario de la incidencia comercial"));
    addSHeader("estadoincidenciacomercial", DBCampo::DBvarchar, DBCampo::DBNoSave, SHeader::DBNone | SHeader::DBNoWrite, tr("Estado de la incidencia comercial"));
    addSHeader("idpedidocliente", DBCampo::DBvarchar, DBCampo::DBNoSave, SHeader::DBNoView | SHeader::DBNoWrite, tr("ID pedido cliente"));
    addSHeader("idcobro", DBCampo::DBvarchar, DBCampo::DBNoSave, SHeader::DBNoView | SHeader::DBNoWrite, tr("ID cobro"));
    addSHeader("nomzonacomercial", DBCampo::DBvarchar, DBCampo::DBNoSave, SHeader::DBNone | SHeader::DBNoWrite, tr("Nombre zona comercial"));
    addSHeader("refrutacomercial", DBCampo::DBvarchar, DBCampo::DBNoSave, SHeader::DBNone | SHeader::DBNoWrite, tr("Referencia ruta"));
    addSHeader("horarutacomercial", DBCampo::DBvarchar, DBCampo::DBNoSave, SHeader::DBNone | SHeader::DBNoWrite, tr("Hora ruta"));
    addSHeader("refincidenciacomercial", DBCampo::DBvarchar, DBCampo::DBNoSave, SHeader::DBNone | SHeader::DBNoWrite, tr("Referencia de la incidencia"));
    addSHeader("horaincidenciacomercial", DBCampo::DBvarchar, DBCampo::DBNoSave, SHeader::DBNone | SHeader::DBNoWrite, tr("Hora de la incidencia"));
    setinsercion(FALSE);
};

