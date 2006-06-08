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




RutaComercialList::RutaComercialList(QWidget *parent, const char *name, Qt::WFlags )
        : QWidget(parent, name, Qt::WDestructiveClose) {
    setupUi(this);
    m_companyact = NULL;
    m_modo=0;
    m_idpresupuesto="";
    meteWindow(caption(),this);
    hideBusqueda();
}

RutaComercialList::RutaComercialList(company *comp, QWidget *parent, const char *name, Qt::WFlags )
        : QWidget(parent, name, Qt::WDestructiveClose) {
    setupUi(this);
    m_companyact = comp;
    m_cliente->setcompany(comp);
    presenta();
    m_modo=0;
    m_idpresupuesto="";
    hideBusqueda();
    meteWindow(caption(),this);
}

RutaComercialList::~RutaComercialList() {
    _depura("RutaComercialList::~RutaComercialList",0);
    m_companyact->sacaWindow(this);
}


void RutaComercialList::presenta() {
    _depura("RutaComercialList::presenta()\n");

    QString SQLQuery = "SELECT * FROM (SELECT * FROM rutacomercial NATURAL LEFT JOIN incidenciacomercial UNION   SELECT * FROM rutacomercial NATURAL RIGHT JOIN incidenciacomercial WHERE incidenciacomercial.idrutacomercial IS NULL) AS t1 NATURAL LEFT JOIN trabajador LEFT JOIN (SELECT * FROM cliente NATURAL LEFT JOIN zonacomercial ) AS t2 ON t1.idcliente=t2.idcliente   WHERE 1=1"+generaFiltro();

    cursor2 * cur= m_companyact->cargacursor(SQLQuery);
    mui_list->cargar(cur);
    delete cur;

    _depura("end RutaComercialList::presenta()\n");
}// end presenta



QString RutaComercialList::generaFiltro() {
    /// Tratamiento de los filtros.
    QString filtro="";
    if (m_cliente->idcliente() != "") {
        filtro += " AND t1.idcliente="+m_cliente->idcliente();
    }// end if

    if (m_fechain->text() != "")
        filtro += " AND t1.fechaincidenciacomercial >= '"+m_fechain->text()+"' ";

    if (m_fechafin->text() != "")
        filtro += " AND t1.fechaincidenciacomercial <= '"+m_fechafin->text()+"' ";


    return (filtro);
}// end generaFiltro






void RutaComercialList::editar(int  row) {
    _depura("RutaComercialList::editar",0);
    QString idrutacomercial = mui_list->DBvalue("idrutacomercial",row);
    QString idincidenciacomercial = mui_list->DBvalue("idincidenciacomercial",row);

    RutaComercialIncView *rut = new RutaComercialIncView(m_companyact, NULL, "Ruta Comercial");
    if (rut->cargar(idrutacomercial, idincidenciacomercial))
        return;
    m_companyact->m_pWorkspace->addWindow(rut);
    rut->show();

    _depura("END RutaComercialList::editar",0);
}


void RutaComercialList::on_mui_editar_clicked() {
    int a = mui_list->currentRow();
    if (a >=0 )
        editar(a);
    else
        _depura("Debe seleccionar una linea",2);
}



void RutaComercialList::on_mui_crear_clicked() {
    _depura("RutaComercialList::on_mui_crear_clicked",0);

    RutaComercialIncView *rut = new RutaComercialIncView(m_companyact, NULL, "Ruta Comercial");
    m_companyact->m_pWorkspace->addWindow(rut);
    rut->show();
    _depura("END RutaComercialList::on_mui_crear_clicked",0);
}



void RutaComercialList::imprimir() {
    QString archivo=confpr->valor(CONF_DIR_OPENREPORTS)+"rutascomerciales.rml";
    QString archivod = confpr->valor(CONF_DIR_USER)+"rutascomerciales.rml";
    QString archivologo=confpr->valor(CONF_DIR_OPENREPORTS)+"logo.jpg";

    /// Copiamos el archivo
#ifdef WINDOWS
    archivo = "copy "+archivo+" "+archivod;
#else
    archivo = "cp "+archivo+" "+archivod;
#endif

    system (archivo.ascii());
    /// Copiamos el logo

#ifdef WINDOWS
    archivologo = "copy "+archivologo+" "+confpr->valor(CONF_DIR_USER)+"logo.jpg";
#else
    archivologo = "cp "+archivologo+" "+confpr->valor(CONF_DIR_USER)+"logo.jpg";
#endif

    system (archivologo.ascii());

    QFile file;
    file.setName( archivod );
    file.open( QIODevice::ReadOnly );
    QTextStream stream(&file);
    QString buff = stream.read();
    file.close();
    QString fitxersortidatxt;
    fitxersortidatxt = "<blockTable style=\"tabla\" repeatRows=\"1\">";
    fitxersortidatxt += mui_list->imprimir();
    fitxersortidatxt += "</blockTable>";

    buff.replace("[story]",fitxersortidatxt);

    if ( file.open( QIODevice::WriteOnly ) ) {
        QTextStream stream( &file );
        stream << buff;
        file.close();
    }// end if

    // Crea el pdf  y lo muestra.
    invocaPDF("rutascomerciales");

}


void RutaComercialList::on_mui_borrar_clicked() {
    _depura("RutaComercialList::on_mui_borrar_clicked",0);

    QString idrutacomercial = mui_list->DBvalue("idrutacomercial");
    QString idincidenciacomercial = mui_list->DBvalue("idincidenciacomercial");

    RutaComercialIncView *rut = new RutaComercialIncView(m_companyact, NULL, "Ruta Comercial");
    if (rut->cargar(idrutacomercial, idincidenciacomercial))
        return;
    rut->on_mui_borrar_clicked();

    _depura("END RutaComercialList::on_mui_borrar_clicked",0);
}




/// =============================================================================
///                    SUBFORMULARIO
/// =============================================================================

RutaComercialListSubForm::RutaComercialListSubForm(QWidget *parent, const char *) : SubForm2Bf(parent) {
    setDBTableName("rutacomercial");
    setDBCampoId("idrutacomercial");
    addSHeader("cifcliente", DBCampo::DBvarchar, DBCampo::DBNoSave, SHeader::DBNone | SHeader::DBNoWrite, "cifcliente");
    addSHeader("nomcliente", DBCampo::DBvarchar, DBCampo::DBNoSave, SHeader::DBNone | SHeader::DBNoWrite, "nomcliente");
    addSHeader("idzonacomercial", DBCampo::DBvarchar, DBCampo::DBNoSave, SHeader::DBNoView | SHeader::DBNoWrite, "idzonacomercial");
    addSHeader("fecharutacomercial", DBCampo::DBvarchar, DBCampo::DBNoSave, SHeader::DBNone | SHeader::DBNoWrite, "fecharutacomercial");
    addSHeader("fechaincidenciacomercial", DBCampo::DBvarchar, DBCampo::DBNoSave, SHeader::DBNone | SHeader::DBNoWrite, "fechaincidenciacomercial");
    addSHeader("nomtrabajador", DBCampo::DBvarchar, DBCampo::DBNoSave, SHeader::DBNone | SHeader::DBNoWrite, "nomtrabajador");
    addSHeader("apellidostrabajador", DBCampo::DBvarchar, DBCampo::DBNoSave, SHeader::DBNone | SHeader::DBNoWrite, "apellidostrabajador");
    addSHeader("idcliente", DBCampo::DBvarchar, DBCampo::DBNoSave, SHeader::DBNoView | SHeader::DBNoWrite, "idcliente");
    addSHeader("idtrabajador", DBCampo::DBvarchar, DBCampo::DBNoSave, SHeader::DBNoView | SHeader::DBNoWrite, "idtrabajador");
    addSHeader("idrutacomercial", DBCampo::DBvarchar, DBCampo::DBNoSave, SHeader::DBNoView | SHeader::DBNoWrite, "idrutacomercial");
    addSHeader("comentariosrutacomercial", DBCampo::DBvarchar, DBCampo::DBNoSave, SHeader::DBNone | SHeader::DBNoWrite, "comentariosrutacomercial");
    addSHeader("idincidenciacomercial", DBCampo::DBvarchar, DBCampo::DBNoSave, SHeader::DBNoView | SHeader::DBNoWrite, "idincidenciacomercial");
    addSHeader("comentincidenciacomercial", DBCampo::DBvarchar, DBCampo::DBNoSave, SHeader::DBNone | SHeader::DBNoWrite, "comentincidenciacomercial");
    addSHeader("estadoincidenciacomercial", DBCampo::DBvarchar, DBCampo::DBNoSave, SHeader::DBNone | SHeader::DBNoWrite, "estadoincidenciacomercial");
    addSHeader("idpedidocliente", DBCampo::DBvarchar, DBCampo::DBNoSave, SHeader::DBNoView | SHeader::DBNoWrite, "idpedidocliente");
    addSHeader("idcobro", DBCampo::DBvarchar, DBCampo::DBNoSave, SHeader::DBNoView | SHeader::DBNoWrite, "idcobro");
    addSHeader("nomzonacomercial", DBCampo::DBvarchar, DBCampo::DBNoSave, SHeader::DBNone | SHeader::DBNoWrite, "nomzonacomercial");
    addSHeader("refrutacomercial", DBCampo::DBvarchar, DBCampo::DBNoSave, SHeader::DBNone | SHeader::DBNoWrite, "Ref. Ruta");
    addSHeader("horarutacomercial", DBCampo::DBvarchar, DBCampo::DBNoSave, SHeader::DBNone | SHeader::DBNoWrite, "Hora Ruta");
    addSHeader("refincidenciacomercial", DBCampo::DBvarchar, DBCampo::DBNoSave, SHeader::DBNone | SHeader::DBNoWrite, "Ref. Incidencia");
    addSHeader("horaincidenciacomercial", DBCampo::DBvarchar, DBCampo::DBNoSave, SHeader::DBNone | SHeader::DBNoWrite, "Hora Incidencia");



    setinsercion(FALSE);
};



