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
//Added by qt3to4:
#include <Q3PopupMenu>
#include "asiento1view.h"

//Tabla Soportado
#define  S_COL_FECHA 0
#define  S_COL_ORDEN 1
#define  S_COL_CONTRAPARTIDA 2
#define  S_COL_DESCRIPCION 3
#define  S_COL_BASEIMP 4
#define  S_COL_PORCENT_IVA 5
#define  S_COL_TOTAL 6
#define  S_COL_FACTURA 7
#define  S_COL_CIF 8
#define  S_COL_NUMASIENTO 9
#define  S_COL_CUENTA_IVA 10
#define  S_COL_IDASIENTO 11
#define  S_COL_IDBORRADOR 12


//Tabla Repercutido
#define  R_COL_FECHA 0
#define  R_COL_FACTURA 1
#define  R_COL_CONTRAPARTIDA 2
#define  R_COL_DESCRIPCION 3
#define  R_COL_BASEIMP 4
#define  R_COL_PORCENT_IVA 5
#define  R_COL_TOTAL 6
#define  R_COL_CIF 7
#define  R_COL_NUMASIENTO 8
#define  R_COL_CUENTA_IVA 9
#define  R_COL_IDASIENTO 10
#define  R_COL_IDBORRADOR 11


// Tablas de RESUMEN
#define RES_NOMBRETIPOIVASOPORTADO 0
#define RES_IVASOPORTADO 1
#define RES_BASESOPORTADO 2

#define RES_NOMBRETIPOIVAREPERCUTIDO 0
#define RES_IVAREPERCUTIDO 1
#define RES_BASEREPERCUTIDO 2


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
    _depura("ListRegistroIvaView::on_mui_tablasoportado_itemDoubleClicked", 2);
    int idasiento;
    idasiento = mui_tablasoportado->DBvalue("idasiento").toInt();
    m_companyact->intapuntsempresa()->muestraasiento(idasiento);
    m_companyact->intapuntsempresa()->show();
    m_companyact->intapuntsempresa()->setFocus();
    _depura("END ListRegistroIvaView::on_mui_tablasoportado_itemDoubleClicked", 2);
}

/**
 * Al hacer doble click sobre la tabla de ivas se accede al asiento
 * que tiene dicha entrada
 */
void ListRegistroIvaView::on_mui_tablarepercutido_cellDoubleClicked(int, int) {
    _depura("ListRegistroIvaView::on_mui_tablarepercutido_itemDoubleClicked", 2);
    int idasiento;
    idasiento = mui_tablarepercutido->DBvalue("idasiento").toInt();
    m_companyact->intapuntsempresa()->muestraasiento(idasiento);
    m_companyact->intapuntsempresa()->show();
    m_companyact->intapuntsempresa()->setFocus();
    _depura("END ListRegistroIvaView::on_mui_tablarepercutido_itemDoubleClicked", 2);
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

    /// Inicializamos las tablas de RESUMEN
    /// Y empezamos a trabajar con las tablas de resumen.
    m_listSoportado->setNumCols(3);
    m_listSoportado->horizontalHeader()->setLabel( RES_NOMBRETIPOIVASOPORTADO, tr("NOMBRETIPOIVA"));
    m_listSoportado->horizontalHeader()->setLabel( RES_IVASOPORTADO, tr("IVASOPORTADO"));
    m_listSoportado->horizontalHeader()->setLabel( RES_BASESOPORTADO, tr("BASESOPORTADO"));

    m_listRepercutido->setNumCols(3);
    m_listRepercutido->horizontalHeader()->setLabel( RES_NOMBRETIPOIVAREPERCUTIDO, tr("NOMBRETIPOIVA"));
    m_listRepercutido->horizontalHeader()->setLabel( RES_IVAREPERCUTIDO, tr("IVASOPORTADO"));
    m_listRepercutido->horizontalHeader()->setLabel( RES_BASEREPERCUTIDO, tr("BASESOPORTADO"));

    QString SQLQuery = "SELECT * FROM cuenta, tipoiva LEFT JOIN (SELECT idtipoiva, SUM(baseiva) AS tbaseiva, sum(ivaiva) AS tivaiva FROM iva  WHERE iva.idregistroiva IN (SELECT idregistroiva FROM registroiva WHERE ffactura >='"+finicial->text()+"' AND ffactura <='"+ffinal->text()+"' AND factemitida) GROUP BY idtipoiva) AS dd ON dd.idtipoiva=tipoiva.idtipoiva WHERE tipoiva.idcuenta = cuenta.idcuenta";
    cursor2 *cur = m_companyact->cargacursor(SQLQuery, "elcursor");
    m_listSoportado->setNumRows(cur->numregistros());
    int j =0;
    while (! cur->eof() ) {	
        m_listSoportado->setText(j, RES_NOMBRETIPOIVASOPORTADO,cur->valor("nombretipoiva"));
        m_listSoportado->setText(j, RES_IVASOPORTADO,cur->valor("tivaiva"));
        m_listSoportado->setText(j, RES_BASESOPORTADO, cur->valor("tbaseiva"));
        cur->siguienteregistro();
        j++;
    }// end while
    delete cur;

    SQLQuery = "SELECT * FROM cuenta, tipoiva  LEFT JOIN (SELECT idtipoiva, SUM(baseiva) AS tbaseiva, SUM(ivaiva) AS tivaiva FROM iva WHERE iva.idregistroiva IN (SELECT idregistroiva FROM registroiva WHERE ffactura >='"+finicial->text()+"' AND ffactura <='"+ffinal->text()+"' AND NOT factemitida) GROUP BY idtipoiva) AS dd ON dd.idtipoiva=tipoiva.idtipoiva WHERE tipoiva.idcuenta = cuenta.idcuenta";
    cur = m_companyact->cargacursor(SQLQuery, "elcursor");
    m_listRepercutido->setNumRows(cur->numregistros());
    j =0;
    while (! cur->eof() ) {
        m_listRepercutido->setText(j, RES_NOMBRETIPOIVAREPERCUTIDO,cur->valor("nombretipoiva"));
        m_listRepercutido->setText(j, RES_IVAREPERCUTIDO,cur->valor("tivaiva"));
        m_listRepercutido->setText(j, RES_BASEREPERCUTIDO, cur->valor("tbaseiva"));
        cur->siguienteregistro();
        j++;
    }// end while
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
    
    // Vamos a cargar la lista de tablasoportado (COMPRAS)
    tablasoportado->setNumCols(13);
    tablasoportado->horizontalHeader()->setLabel( S_COL_FECHA, tr( "FECHA" ) );
    tablasoportado->horizontalHeader()->setLabel( S_COL_CONTRAPARTIDA, tr( "CONTRAPARTIDA" ) );
    tablasoportado->horizontalHeader()->setLabel( S_COL_DESCRIPCION, tr( "DESCRIPCION" ) );
    tablasoportado->horizontalHeader()->setLabel( S_COL_BASEIMP, tr( "BASE IMPONIBLE" ) );
    tablasoportado->horizontalHeader()->setLabel( S_COL_PORCENT_IVA, tr( "PORCENTAJE IVA" ) );
    tablasoportado->horizontalHeader()->setLabel( S_COL_TOTAL, tr( "TOTAL" ) );
    tablasoportado->horizontalHeader()->setLabel( S_COL_FACTURA, tr( "FACTURA PROVEEDOR" ) );
    tablasoportado->horizontalHeader()->setLabel( S_COL_ORDEN, tr( "NUM ORDEN" ) );
    tablasoportado->horizontalHeader()->setLabel( S_COL_CIF, tr( "CIF" ) );
    tablasoportado->horizontalHeader()->setLabel( S_COL_NUMASIENTO, tr( "NUM ASIENTO" ) );
    tablasoportado->horizontalHeader()->setLabel( S_COL_IDBORRADOR, tr("ID BORRADOR") );
    tablasoportado->horizontalHeader()->setLabel( S_COL_IDASIENTO, tr( "ID ASIENTO" ) );
    tablasoportado->horizontalHeader()->setLabel( S_COL_CUENTA_IVA, tr( "CUENTA IVA" ) );

    tablasoportado->hideColumn(S_COL_CUENTA_IVA);
    tablasoportado->hideColumn(S_COL_IDASIENTO);
    tablasoportado->hideColumn(S_COL_IDBORRADOR);
    tablasoportado->hideColumn(S_COL_NUMASIENTO);
    tablasoportado->hideColumn(S_COL_CONTRAPARTIDA);
    tablasoportado->hideColumn(S_COL_PORCENT_IVA);



    query.sprintf("SELECT *, (registroiva.baseimp + registroiva.iva) AS totalfactura FROM registroiva LEFT JOIN (SELECT  * FROM cuenta, borrador, asiento  WHERE cuenta.idcuenta = borrador.idcuenta AND asiento.idasiento = borrador.idasiento AND borrador.fecha >= '%s' AND borrador.fecha <= '%s') AS t1 ON t1.idborrador = registroiva.idborrador WHERE factemitida ",finicial->text().ascii(), ffinal->text().ascii());
    cursor2 *cursorreg = m_companyact->cargacursor(query);

    /// El nuevo proceso de carga es distinto.
    mui_tablasoportado->cargar(cursorreg);

    int i =0;
    cursor2 *cursorcontra;
    tablasoportado->setNumRows(cursorreg->numregistros());
    while (!cursorreg->eof()) {
        query.sprintf("SELECT * FROM cuenta WHERE cuenta.idcuenta=%s",cursorreg->valor("contrapartida").ascii());
        m_companyact->begin();
        cursorcontra = m_companyact->cargacursor(query,"contra");
        m_companyact->commit();
        if (!cursorcontra->eof()) {
            tablasoportado->setText(i,S_COL_CONTRAPARTIDA,cursorcontra->valor("codigo"));
            tablasoportado->setText(i,S_COL_DESCRIPCION,cursorcontra->valor("descripcion"));
        }// end if
        delete cursorcontra;
        tablasoportado->setText(i,S_COL_FECHA,cursorreg->valor("fecha").mid(0,10));
        tablasoportado->setText(i,S_COL_CUENTA_IVA,cursorreg->valor("codigo"));
        if (tablasoportado->text(i,S_COL_CUENTA_IVA).left(3)!="472")
            tablasoportado->setText(i,S_COL_CUENTA_IVA,tr("Exento"));

        tablasoportado->setText(i,S_COL_BASEIMP,cursorreg->valor("baseimp"));
        tablasoportado->setText(i,S_COL_PORCENT_IVA,cursorreg->valor("iva"));
        tablasoportado->setText(i,S_COL_TOTAL,cursorreg->valor("totalfactura"));
        tablasoportado->setText(i,S_COL_FACTURA,cursorreg->valor("factura"));
        tablasoportado->setText(i,S_COL_ORDEN,cursorreg->valor("numorden"));
        tablasoportado->setText(i,S_COL_CIF,cursorreg->valor("cif"));
        tablasoportado->setText(i,S_COL_NUMASIENTO,cursorreg->valor("ordenasiento"));
        tablasoportado->setText(i,S_COL_IDASIENTO,cursorreg->valor("idasiento"));
        tablasoportado->setText(i,S_COL_IDBORRADOR, cursorreg->valor("idborrador"));
        i++;
        cursorreg->siguienteregistro();
    }// end for
    delete cursorreg;

    // Vamos a cargar la lista de tablarepercutido (VENTAS)
    tablarepercutido->setNumCols(12);
    tablarepercutido->horizontalHeader()->setLabel( R_COL_FECHA, tr( "FECHA" ) );
    tablarepercutido->horizontalHeader()->setLabel( R_COL_CONTRAPARTIDA, tr( "CONTRAPARTIDA" ) );
    tablarepercutido->horizontalHeader()->setLabel( R_COL_DESCRIPCION, tr( "DESCRIPCION" ) );
    tablarepercutido->horizontalHeader()->setLabel( R_COL_BASEIMP, tr( "BASE IMPONIBLE" ) );
    tablarepercutido->horizontalHeader()->setLabel( R_COL_PORCENT_IVA, tr( "PORCENTAJE IVA" ) );
    tablarepercutido->horizontalHeader()->setLabel( R_COL_TOTAL, tr( "TOTAL" ) );
    tablarepercutido->horizontalHeader()->setLabel( R_COL_FACTURA, tr( "FACTURA" ) );
    tablarepercutido->horizontalHeader()->setLabel( R_COL_CIF, tr( "CIF" ) );
    tablarepercutido->horizontalHeader()->setLabel( R_COL_NUMASIENTO, tr( "NUM ASIENTO") );
    tablarepercutido->horizontalHeader()->setLabel( R_COL_IDBORRADOR, tr( "ID BORRADOR") );
    tablarepercutido->horizontalHeader()->setLabel( R_COL_IDASIENTO, tr( "ID ASIENTO" ) );
    tablarepercutido->horizontalHeader()->setLabel( R_COL_CUENTA_IVA, tr( "CUENTA IVA" ) );

    tablarepercutido->hideColumn(R_COL_CUENTA_IVA);
    tablarepercutido->hideColumn(R_COL_IDASIENTO);
    tablarepercutido->hideColumn(R_COL_IDBORRADOR);
    tablarepercutido->hideColumn(R_COL_NUMASIENTO);
    tablarepercutido->hideColumn(R_COL_CONTRAPARTIDA);
    tablarepercutido->hideColumn(R_COL_PORCENT_IVA);


    // Hacemos el c�culo de los que no pertenecen a iva soportado pq as�entran todos.
    query.sprintf("SELECT *, (registroiva.baseimp + registroiva.iva) AS totalfactura FROM registroiva LEFT JOIN (SELECT * FROM cuenta, borrador, asiento  WHERE cuenta.idcuenta = borrador.idcuenta AND asiento.idasiento = borrador.idasiento AND borrador.fecha >= '%s' AND borrador.fecha <= '%s') AS t1 ON t1.idborrador = registroiva.idborrador AND NOT factemitida", finicial->text().ascii(), ffinal->text().ascii());
    cursorreg = m_companyact->cargacursor(query,"cmquery");
    /// El nuevo proceso de carga es distinto.
    mui_tablasoportado->cargar(cursorreg);

    i =0;
    tablarepercutido->setNumRows(cursorreg->numregistros());
    while (!cursorreg->eof()) {
        query.sprintf("SELECT * FROM cuenta WHERE cuenta.idcuenta=%s",cursorreg->valor("contrapartida").ascii());
        m_companyact->begin();
        cursorcontra = m_companyact->cargacursor(query,"contra");
        m_companyact->commit();
        if (!cursorcontra->eof()) {
            tablarepercutido->setText(i,R_COL_CONTRAPARTIDA,cursorcontra->valor("codigo"));
            tablarepercutido->setText(i,R_COL_DESCRIPCION,cursorcontra->valor("descripcion"));
        }// end if
        delete cursorcontra;
        tablarepercutido->setText(i,R_COL_FECHA,cursorreg->valor("fecha").mid(0,10));
        tablarepercutido->setText(i,R_COL_CUENTA_IVA,cursorreg->valor("codigo"));
        if (tablarepercutido->text(i,R_COL_CUENTA_IVA).left(3)!="477")
            tablarepercutido->setText(i,R_COL_CUENTA_IVA,tr("Exento"));

        tablarepercutido->setText(i,R_COL_BASEIMP,cursorreg->valor("baseimp"));
        tablarepercutido->setText(i,R_COL_PORCENT_IVA,cursorreg->valor("iva"));
        tablarepercutido->setText(i,R_COL_TOTAL,cursorreg->valor("totalfactura"));
        tablarepercutido->setText(i,R_COL_FACTURA,cursorreg->valor("factura"));
        tablarepercutido->setText(i,R_COL_CIF,cursorreg->valor("cif"));
        tablarepercutido->setText(i,R_COL_NUMASIENTO,cursorreg->valor("ordenasiento"));
        tablarepercutido->setText(i,R_COL_IDASIENTO,cursorreg->valor("idasiento"));
        tablarepercutido->setText(i,R_COL_IDBORRADOR, cursorreg->valor("idborrador"));
        i++;
        cursorreg->siguienteregistro();
    }// end While
    delete cursorreg;
}

void ListRegistroIvaView::menu_contextual(int row, int , const QPoint &poin) {
    // Si el asiento esta cerrado el menu a mostrar es diferente
    int idborrador =0;
    Q3PopupMenu *popup = new Q3PopupMenu;
    popup->insertItem(tr("Ver Asiento"), 0);
    popup->insertSeparator();
    popup->insertItem(tr("Editar Registro"),101);
    popup->insertItem(tr("Borrar Registro"),103);
    int opcion = popup->exec(poin);
    switch(opcion) {
    case 0:
        int idasiento;
        idasiento = atoi(tablasoportado->text(row,S_COL_IDASIENTO).ascii());
//        m_companyact->intapuntsempresa()->flashAsiento(idasiento);
        m_companyact->intapuntsempresa()->muestraasiento(idasiento);
        m_companyact->intapuntsempresa()->show();
        m_companyact->intapuntsempresa()->setFocus();
        close();
        break;
    case 101:
        idborrador = atoi(tablasoportado->text(row,S_COL_IDBORRADOR).ascii());
        if (idborrador != 0) {
            RegistroIvaView *nuevae=new RegistroIvaView(m_companyact,0);
            nuevae->inicializa1(idborrador);
	    m_companyact->pWorkspace()->addWindow(nuevae);
	    nuevae->show();
        }// end if
        break;
    case 103:
        idborrador = atoi(tablasoportado->text(row,S_COL_IDBORRADOR).ascii());
        if (idborrador != 0) {
            RegistroIvaView *nuevae=new RegistroIvaView(m_companyact,0);
            nuevae->inicializa1(idborrador);
            nuevae->on_mui_borrar_clicked();
            delete nuevae;
	    on_mui_actualizar_clicked();
        }// end if
        break;


    }// end switch
    delete popup;
}

void ListRegistroIvaView::menu_contextual1(int row, int , const QPoint &poin) {
    // Si el asiento esta cerrado el menu a mostrar es diferente
    int idborrador=0;
    Q3PopupMenu *popup = new Q3PopupMenu;
    popup->insertItem(tr("Ver Asiento"), 0);
    popup->insertSeparator();
    popup->insertItem(tr("Editar entrada de IVA"),101);
    popup->insertItem(tr("Borrar Registro"),103);
    int opcion = popup->exec(poin);
    switch(opcion) {
    case 0:
        int idasiento;
        idasiento = atoi(tablarepercutido->text(row,R_COL_IDASIENTO).ascii());
        m_companyact->intapuntsempresa()->muestraasiento(idasiento);
        m_companyact->intapuntsempresa()->show();
        m_companyact->intapuntsempresa()->setFocus();
        close();
        break;
    case 101:
        idborrador = atoi(tablarepercutido->text(row,R_COL_IDBORRADOR).ascii());
        if (idborrador != 0) {
            RegistroIvaView *nuevae=new RegistroIvaView(m_companyact, 0);
            nuevae->inicializa1(idborrador);
	    m_companyact->pWorkspace()->addWindow(nuevae);
	    nuevae->show();
        }// end if
        break;
    case 103:
        idborrador = atoi(tablarepercutido->text(row,R_COL_IDBORRADOR).ascii());
        if (idborrador != 0) {
            RegistroIvaView *nuevae=new RegistroIvaView(m_companyact, 0);
            nuevae->inicializa1(idborrador);
            nuevae->on_mui_borrar_clicked();
            delete nuevae;
	    on_mui_actualizar_clicked();
        }// end if
        break;
    }// end switch
    delete popup;
}

