/***************************************************************************
                          ListRegistroIvaView.cpp  -  description
                             -------------------
    begin                : Thu Jan 30 2003
    copyright            : (C) 2003 by Tomeu Borr� Riera
    email                : tborras@conetxia.com
 ***************************************************************************/
/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/
#include "listregistroivaview.h"
#include "regivaprintview.h"
#include "registroivaview.h"
#include "empresa.h"
#include "calendario.h"
#include <Q3PopupMenu>
#include "asiento1view.h"



ListRegistroIvaView::ListRegistroIvaView(empresa * emp, QString, QWidget *parent, const char *name ) : QWidget(parent, Qt::WDestructiveClose) {
    _depura("ListRegistroIvaView::ListRegistroIvaView", 0);
    setupUi(this);
    m_companyact = emp;
    finicial->setText(normalizafecha("01/01").toString("dd/MM/yyyy"));
    ffinal->setText(normalizafecha("31/12").toString("dd/MM/yyyy"));
    emp->meteWindow(windowTitle(), this);

    /// ============================================
    /// DEFINICIONES PARA LA TABLA DE IVAAboutView
    mui_tablasoportado->setDBTableName("registroiva");
    mui_tablasoportado->setDBCampoId("idregistroiva");
    mui_tablasoportado->addSHeader("idregistroiva", DBCampo::DBvarchar, DBCampo::DBNotNull, SHeader::DBNoWrite , tr("Id"));
    mui_tablasoportado->addSHeader("contrapartida", DBCampo::DBvarchar, DBCampo::DBNothing, SHeader::DBNoWrite , tr("Concepto Contable"));
    mui_tablasoportado->addSHeader("baseimp", DBCampo::DBvarchar, DBCampo::DBNotNull, SHeader::DBNoWrite | SHeader::DBNoView, tr("idcuenta"));
    mui_tablasoportado->addSHeader("iva", DBCampo::DBvarchar, DBCampo::DBNoSave, SHeader::DBNoWrite , tr("codigo"));
    mui_tablasoportado->addSHeader("ffactura", DBCampo::DBvarchar, DBCampo::DBNoSave, SHeader::DBNoWrite , tr("tipocuenta"));
    mui_tablasoportado->addSHeader("femisionregistroiva", DBCampo::DBvarchar, DBCampo::DBNoSave, SHeader::DBNoWrite , tr("descripcioncuenta"));
    mui_tablasoportado->addSHeader("factura", DBCampo::DBvarchar, DBCampo::DBNothing, SHeader::DBNoWrite , tr("descripcion"));
    mui_tablasoportado->addSHeader("serieregistroiva", DBCampo::DBnumeric, DBCampo::DBNothing, SHeader::DBNoWrite , tr("Debe"));
    mui_tablasoportado->addSHeader("idborrador", DBCampo::DBnumeric, DBCampo::DBNothing, SHeader::DBNoWrite , tr("Haber"));
    mui_tablasoportado->addSHeader("incregistro", DBCampo::DBvarchar, DBCampo::DBNoSave, SHeader::DBNoWrite, tr("Contrapartida"));
    mui_tablasoportado->addSHeader("regularizacion", DBCampo::DBvarchar, DBCampo::DBNoSave, SHeader::DBNoWrite, tr("Comentario"));
    mui_tablasoportado->addSHeader("plan349", DBCampo::DBvarchar, DBCampo::DBNoSave, SHeader::DBNoWrite, tr("ID Canal"));
    mui_tablasoportado->addSHeader("numorden", DBCampo::DBvarchar, DBCampo::DBNoSave, SHeader::DBNoWrite, tr("Conciliacion"));
    mui_tablasoportado->addSHeader("cif", DBCampo::DBvarchar, DBCampo::DBNoSave, SHeader::DBNoWrite, tr("idc_coste"));
    mui_tablasoportado->addSHeader("idfpago", DBCampo::DBvarchar, DBCampo::DBNoSave, SHeader::DBNoWrite, tr("idapunte"));
    mui_tablasoportado->addSHeader("factemitida", DBCampo::DBvarchar, DBCampo::DBNoSave, SHeader::DBNoWrite, tr("idtipoiva"));
    mui_tablasoportado->addSHeader("rectificaaregistroiva", DBCampo::DBvarchar, DBCampo::DBNoSave, SHeader::DBNoWrite, tr("orden"));
    mui_tablasoportado->addSHeader("idasiento", DBCampo::DBvarchar, DBCampo::DBNoSave, SHeader::DBNoWrite, tr("idasiento"));
    mui_tablasoportado->setinsercion(FALSE);


    mui_tablarepercutido->setDBTableName("registroiva");
    mui_tablarepercutido->setDBCampoId("idregistroiva");
    mui_tablarepercutido->addSHeader("idregistroiva", DBCampo::DBvarchar, DBCampo::DBNotNull, SHeader::DBNoWrite , tr("Id"));
    mui_tablarepercutido->addSHeader("contrapartida", DBCampo::DBvarchar, DBCampo::DBNothing, SHeader::DBNoWrite , tr("Concepto Contable"));
    mui_tablarepercutido->addSHeader("baseimp", DBCampo::DBvarchar, DBCampo::DBNotNull, SHeader::DBNoWrite | SHeader::DBNoView, tr("idcuenta"));
    mui_tablarepercutido->addSHeader("iva", DBCampo::DBvarchar, DBCampo::DBNoSave, SHeader::DBNone , tr("codigo"));
    mui_tablarepercutido->addSHeader("ffactura", DBCampo::DBvarchar, DBCampo::DBNoSave, SHeader::DBNone , tr("tipocuenta"));
    mui_tablarepercutido->addSHeader("femisionregistroiva", DBCampo::DBvarchar, DBCampo::DBNoSave, SHeader::DBNone , tr("descripcioncuenta"));
    mui_tablarepercutido->addSHeader("factura", DBCampo::DBvarchar, DBCampo::DBNothing, SHeader::DBNone , tr("descripcion"));
    mui_tablarepercutido->addSHeader("serieregistroiva", DBCampo::DBnumeric, DBCampo::DBNothing, SHeader::DBNone , tr("Debe"));
    mui_tablarepercutido->addSHeader("idborrador", DBCampo::DBnumeric, DBCampo::DBNothing, SHeader::DBNone , tr("Haber"));
    mui_tablarepercutido->addSHeader("incregistro", DBCampo::DBvarchar, DBCampo::DBNoSave, SHeader::DBNone , tr("Contrapartida"));
    mui_tablarepercutido->addSHeader("regularizacion", DBCampo::DBvarchar, DBCampo::DBNoSave, SHeader::DBNone , tr("Comentario"));
    mui_tablarepercutido->addSHeader("plan349", DBCampo::DBvarchar, DBCampo::DBNoSave, SHeader::DBNone , tr("ID Canal"));
    mui_tablarepercutido->addSHeader("numorden", DBCampo::DBvarchar, DBCampo::DBNoSave, SHeader::DBNone , tr("Conciliacion"));
    mui_tablarepercutido->addSHeader("cif", DBCampo::DBvarchar, DBCampo::DBNoSave, SHeader::DBNone , tr("idc_coste"));
    mui_tablarepercutido->addSHeader("idfpago", DBCampo::DBvarchar, DBCampo::DBNoSave, SHeader::DBNone , tr("idapunte"));
    mui_tablarepercutido->addSHeader("factemitida", DBCampo::DBvarchar, DBCampo::DBNoSave, SHeader::DBNone , tr("idtipoiva"));
    mui_tablarepercutido->addSHeader("rectificaaregistroiva", DBCampo::DBvarchar, DBCampo::DBNoSave, SHeader::DBNone , tr("orden"));
    mui_tablarepercutido->addSHeader("idasiento", DBCampo::DBvarchar, DBCampo::DBNoSave, SHeader::DBNoWrite, tr("idasiento"));
    mui_tablarepercutido->setinsercion(FALSE);


    mui_totalRepercutido->setDBTableName("");
    mui_totalRepercutido->setDBCampoId("");
    mui_totalRepercutido->addSHeader("nombretipoiva", DBCampo::DBvarchar, DBCampo::DBNotNull, SHeader::DBNoWrite , tr("nombretipoiva"));
    mui_totalRepercutido->addSHeader("tivaiva", DBCampo::DBvarchar, DBCampo::DBNothing, SHeader::DBNoWrite , tr("tivaiva"));
    mui_totalRepercutido->addSHeader("tbaseiva", DBCampo::DBvarchar, DBCampo::DBNothing, SHeader::DBNoWrite , tr("tbaseiva"));
    mui_totalRepercutido->setinsercion(FALSE);


    mui_totalSoportado->setDBTableName("");
    mui_totalSoportado->setDBCampoId("");
    mui_totalSoportado->addSHeader("nombretipoiva", DBCampo::DBvarchar, DBCampo::DBNotNull, SHeader::DBNoWrite , tr("nombretipoiva"));
    mui_totalSoportado->addSHeader("tivaiva", DBCampo::DBvarchar, DBCampo::DBNothing, SHeader::DBNoWrite , tr("tivaiva"));
    mui_totalSoportado->addSHeader("tbaseiva", DBCampo::DBvarchar, DBCampo::DBNothing, SHeader::DBNoWrite , tr("tbaseiva"));
    mui_totalSoportado->setinsercion(FALSE);

    /// END DEFINICIONES PARA LA TABLA DE IVAAboutViewAboutViewAboutViewAboutView
    /// =============================================
    _depura("END ListRegistroIvaView::ListRegistroIvaView", 0);
}

ListRegistroIvaView::~ListRegistroIvaView() {
    _depura("ListRegistroIvaView::~ListRegistroIvaView", 0);
    m_companyact->sacaWindow(this);
    _depura("END ListRegistroIvaView::~ListRegistroIvaView", 0);
}

/**
 * Al hacer doble click sobre la tabla de ivas se accede al asiento 
 * que tiene dicha entrada
 */
void ListRegistroIvaView::on_mui_tablasoportado_cellDoubleClicked(int, int) {
    _depura("ListRegistroIvaView::on_mui_tablasoportado_itemDoubleClicked", 0);
    int idasiento;
    idasiento = mui_tablasoportado->DBvalue("idasiento").toInt();
    m_companyact->intapuntsempresa()->muestraasiento(idasiento);
    m_companyact->intapuntsempresa()->show();
    m_companyact->intapuntsempresa()->setFocus();
    _depura("END ListRegistroIvaView::on_mui_tablasoportado_itemDoubleClicked", 0);
}

/**
 * Al hacer doble click sobre la tabla de ivas se accede al asiento
 * que tiene dicha entrada
 */
void ListRegistroIvaView::on_mui_tablarepercutido_cellDoubleClicked(int, int) {
    _depura("ListRegistroIvaView::on_mui_tablarepercutido_itemDoubleClicked", 0);
    int idasiento;
    idasiento = mui_tablarepercutido->DBvalue("idasiento").toInt();
    m_companyact->intapuntsempresa()->muestraasiento(idasiento);
    m_companyact->intapuntsempresa()->show();
    m_companyact->intapuntsempresa()->setFocus();
    _depura("END ListRegistroIvaView::on_mui_tablarepercutido_itemDoubleClicked", 0);
}

void ListRegistroIvaView::boton_print() {
    regivaprintview *print = new regivaprintview(m_companyact,0,0);
    print->inicializa1(finicial->text(), ffinal->text());
    print->exec();
    delete print;
}

void ListRegistroIvaView::on_mui_actualizar_clicked() {
    inicializa();
}

/**
 * 
 * @param inta 
 */
void ListRegistroIvaView::inicializa() {
    QString query;
    QString sbaseimp, siva;
    QString   cbaseimp, civa, ctotal;

    QString SQLQuery = "SELECT * FROM cuenta, tipoiva LEFT JOIN (SELECT idtipoiva, SUM(baseiva) AS tbaseiva, sum(ivaiva) AS tivaiva FROM iva  WHERE iva.idregistroiva IN (SELECT idregistroiva FROM registroiva WHERE ffactura >='"+finicial->text()+"' AND ffactura <='"+ffinal->text()+"' AND factemitida) GROUP BY idtipoiva) AS dd ON dd.idtipoiva=tipoiva.idtipoiva WHERE tipoiva.idcuenta = cuenta.idcuenta";
    cursor2 *cur = m_companyact->cargacursor(SQLQuery);
    mui_totalRepercutido->cargar(cur);

    delete cur;

    SQLQuery = "SELECT * FROM cuenta, tipoiva  LEFT JOIN (SELECT idtipoiva, SUM(baseiva) AS tbaseiva, SUM(ivaiva) AS tivaiva FROM iva WHERE iva.idregistroiva IN (SELECT idregistroiva FROM registroiva WHERE ffactura >='"+finicial->text()+"' AND ffactura <='"+ffinal->text()+"' AND NOT factemitida) GROUP BY idtipoiva) AS dd ON dd.idtipoiva=tipoiva.idtipoiva WHERE tipoiva.idcuenta = cuenta.idcuenta";
    cur = m_companyact->cargacursor(SQLQuery);
    mui_totalSoportado->cargar(cur);
    delete cur;

    SQLQuery = "SELECT SUM(baseimp) AS tbaseimp, sum(iva) AS tbaseiva FROM registroiva WHERE factemitida AND ffactura >='"+finicial->text()+"' AND ffactura <='"+ffinal->text()+"'";
    cur = m_companyact->cargacursor(SQLQuery);
    m_baseimps->setText(cur->valor("tbaseimp"));
    m_ivas->setText(cur->valor("tbaseiva"));
    delete cur;

    SQLQuery = "SELECT SUM(baseimp) AS tbaseimp, sum(iva) AS tbaseiva FROM registroiva WHERE NOT factemitida AND ffactura >='"+finicial->text()+"' AND ffactura <='"+ffinal->text()+"'";
    cur = m_companyact->cargacursor(SQLQuery);
    m_baseimpr->setText(cur->valor("tbaseimp"));
    m_ivar->setText(cur->valor("tbaseiva"));
    delete cur;


    query.sprintf("SELECT *, (registroiva.baseimp + registroiva.iva) AS totalfactura FROM registroiva LEFT JOIN (SELECT  * FROM cuenta, borrador, asiento  WHERE cuenta.idcuenta = borrador.idcuenta AND asiento.idasiento = borrador.idasiento AND borrador.fecha >= '%s' AND borrador.fecha <= '%s') AS t1 ON t1.idborrador = registroiva.idborrador WHERE factemitida ",finicial->text().ascii(), ffinal->text().ascii());
    cursor2 *cursorreg = m_companyact->cargacursor(query);

    /// El nuevo proceso de carga es distinto.
    mui_tablasoportado->cargar(cursorreg);
    delete cursorreg;


    // Hacemos el c�culo de los que no pertenecen a iva soportado pq as�entran todos.
    query.sprintf("SELECT *, (registroiva.baseimp + registroiva.iva) AS totalfactura FROM registroiva LEFT JOIN (SELECT * FROM cuenta, borrador, asiento  WHERE cuenta.idcuenta = borrador.idcuenta AND asiento.idasiento = borrador.idasiento AND borrador.fecha >= '%s' AND borrador.fecha <= '%s') AS t1 ON t1.idborrador = registroiva.idborrador WHERE NOT factemitida", finicial->text().ascii(), ffinal->text().ascii());
    cursorreg = m_companyact->cargacursor(query );
    /// El nuevo proceso de carga es distinto.
    mui_tablarepercutido->cargar(cursorreg);
    delete cursorreg;
}



void ListRegistroIvaView::on_mui_tablarepercutido_pintaMenu(QMenu *menu) {
    _depura("ListRegistroIvaView::on_mui_tablarepercutido_pintaMenu", 0);
    m_verreg = menu->addAction("Editar Registro");
    m_verasiento = menu->addAction("Asiento Contable");
    menu->addSeparator();
    _depura("END ListRegistroIvaView::on_mui_tablarepercutido_pintaMenu", 0);
}

void ListRegistroIvaView::on_mui_tablasoportado_pintaMenu(QMenu *menu) {
    _depura("ListRegistroIvaView::on_mui_tablasoportado_pintaMenu", 0);
    m_verreg = menu->addAction("Editar Registro");
    m_verasiento = menu->addAction("Asiento Contable");
    menu->addSeparator();
    _depura("END ListRegistroIvaView::on_mui_tablasoportado_pintaMenu", 0);
}

void ListRegistroIvaView::on_mui_tablasoportado_trataMenu(QAction *ac) {
    _depura("ListRegistroIvaView::on_mui_tablasoportado_trataMenu", 0);
    if (m_verreg == ac) {
        QString idborrador = mui_tablasoportado->DBvalue("idborrador");
        RegistroIvaView *nuevae=new RegistroIvaView(m_companyact,0);
        nuevae->inicializa1(idborrador.toInt());
        m_companyact->pWorkspace()->addWindow(nuevae);
        nuevae->show();
    } // end if
    if (m_verasiento == ac) {
        int idasiento;
        idasiento = mui_tablasoportado->DBvalue("idasiento").toInt();
        m_companyact->intapuntsempresa()->muestraasiento(idasiento);
        m_companyact->intapuntsempresa()->show();
        m_companyact->intapuntsempresa()->setFocus();
    } // end if
    _depura("END ListRegistroIvaView::on_mui_tablasoportado_trataMenu", 0);
}

void ListRegistroIvaView::on_mui_tablarepercutido_trataMenu(QAction *ac) {
    _depura("ListRegistroIvaView::on_mui_tablarepercutido_trataMenu", 0);
    if (m_verreg == ac) {
        QString idborrador = mui_tablarepercutido->DBvalue("idborrador");
        RegistroIvaView *nuevae=new RegistroIvaView(m_companyact,0);
        nuevae->inicializa1(idborrador.toInt());
        m_companyact->pWorkspace()->addWindow(nuevae);
        nuevae->show();
    } // end if
    if (m_verasiento == ac) {
        int idasiento;
        idasiento = mui_tablarepercutido->DBvalue("idasiento").toInt();
        m_companyact->intapuntsempresa()->muestraasiento(idasiento);
        m_companyact->intapuntsempresa()->show();
        m_companyact->intapuntsempresa()->setFocus();
    } // end if
    _depura("END ListRegistroIvaView::on_mui_tablarepercutido_trataMenu", 0);
}

