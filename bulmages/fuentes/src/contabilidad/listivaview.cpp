/***************************************************************************
                          listivaview.cpp  -  description
                             -------------------
    begin                : Thu Jan 30 2003
    copyright            : (C) 2003 by Tomeu Borrás Riera
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
#include "listivaview.h"
#include "regivaprintview.h"
#include "ivaview.h"
#include "modelo300.h"
#include "empresa.h"
#include "calendario.h"

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


Mod300ps *modelo;
listivaview::listivaview(empresa * emp, QString, QWidget *parent, const char *name ) : listivadlg(parent,name) {
    empresaactual = emp;
    conexionbase = emp->bdempresa();
    finicial->setText(normalizafecha("01/01").toString("dd/MM/yyyy"));
    ffinal->setText(normalizafecha("31/12").toString("dd/MM/yyyy"));
    modelo=new Mod300ps(this->parentWidget());
}// end listivaview


listivaview::~listivaview() {}// end ~lisivaview


/********************************************************************
 * Al hacer doble click sobre la tabla de ivas se accede al asiento *
 * que tiene dicha entrada                                          *
 ********************************************************************/
void listivaview::doble_click_soportado(int a, int, int, const QPoint &punto) {
    int idasiento;
    idasiento = atoi(tablasoportado->text(a,S_COL_IDASIENTO).ascii());
    introapunts->flashAsiento(idasiento);
    introapunts->show();
    introapunts->setFocus();
    done(1);
    punto.isNull();
}// end doble_click_soportado


/********************************************************************
 * Al hacer doble click sobre la tabla de ivas se accede al asiento *
 * que tiene dicha entrada                                          *
 ********************************************************************/
void listivaview::doble_click_repercutido(int a, int , int , const QPoint &) {
    int idasiento;
    idasiento = atoi(tablarepercutido->text(a,R_COL_IDASIENTO).ascii());
    introapunts->flashAsiento(idasiento);
    introapunts->show();
    introapunts->setFocus();
    done(1);
}// end doble_click_repercutido


void listivaview::boton_print() {
    regivaprintview *print = new regivaprintview(empresaactual,0,0);
    print->inicializa1(finicial->text(), ffinal->text());
    print->exec();
    delete print;
}// end boton_imprimir


void listivaview::boton_reload() {
    inicializa(introapunts);
}// end boton_reload


void listivaview::inicializa( intapunts3view *inta) {
    introapunts = inta;
    QString query;
    QString sbaseimp, siva;
    //    float  fbaseimp, fiva;
    QString   cbaseimp, civa, ctotal;
    long int tbaseimps=0, tivas=0;
    long int tbaseimpr=0, tivar=0;

    // Inicializamos las tablas de RESUMEN
    // Y empezamos a trabajar con las tablas de resumen.
    m_listSoportado->setNumCols(3);
    m_listSoportado->horizontalHeader()->setLabel( RES_NOMBRETIPOIVASOPORTADO, tr("NOMBRETIPOIVA"));
    m_listSoportado->horizontalHeader()->setLabel( RES_IVASOPORTADO, tr("IVASOPORTADO"));
    m_listSoportado->horizontalHeader()->setLabel( RES_BASESOPORTADO, tr("BASESOPORTADO"));

    m_listRepercutido->setNumCols(3);
    m_listRepercutido->horizontalHeader()->setLabel( RES_NOMBRETIPOIVAREPERCUTIDO, tr("NOMBRETIPOIVA"));
    m_listRepercutido->horizontalHeader()->setLabel( RES_IVAREPERCUTIDO, tr("IVASOPORTADO"));
    m_listRepercutido->horizontalHeader()->setLabel( RES_BASEREPERCUTIDO, tr("BASESOPORTADO"));


    QString SQLQuery = "SELECT * FROM cuenta, tipoiva LEFT JOIN (SELECT idtipoiva, SUM(baseiva) AS tbaseiva FROM iva  WHERE iva.idregistroiva IN (SELECT idregistroiva FROM registroiva WHERE ffactura >='"+finicial->text()+"' AND ffactura <='"+ffinal->text()+"' ) GROUP BY idtipoiva) AS dd ON dd.idtipoiva=tipoiva.idtipoiva WHERE tipoiva.idcuenta = cuenta.idcuenta AND cuenta.codigo LIKE '472%'";
    conexionbase->begin();
    cursor2 *cur = conexionbase->cargacursor(SQLQuery, "elcursor");
    conexionbase->commit();
    m_listSoportado->setNumRows(cur->numregistros());
    int j =0;
    while (! cur->eof() ) {
        long int baseiva = cur->valor("tbaseiva").replace(".","").toInt();
        long int porcent = cur->valor("porcentajetipoiva").replace(".","").toInt();
        long int baseimp = baseiva *10000 / porcent;
        QString numberstr = QString::number(baseimp);
        numberstr = numberstr.left(numberstr.length()-2)+"."+numberstr.right(2);
        m_listSoportado->setText(j, RES_NOMBRETIPOIVASOPORTADO,cur->valor("nombretipoiva"));
        m_listSoportado->setText(j, RES_IVASOPORTADO,cur->valor("tbaseiva"));
        m_listSoportado->setText(j, RES_BASESOPORTADO, numberstr);
        tivas+= baseiva;
        tbaseimps+=baseimp;
        cur->siguienteregistro();
        j++;
    }// end while
    delete cur;

    SQLQuery = "SELECT * FROM cuenta, tipoiva  LEFT JOIN (SELECT idtipoiva, SUM(baseiva) AS tbaseiva FROM iva iva.idregistroiva IN (SELECT idregistroiva FROM registroiva WHERE ffactura >='"+finicial->text()+"' AND ffactura <='"+ffinal->text()+"' ) GROUP BY idtipoiva) AS dd ON dd.idtipoiva=tipoiva.idtipoiva WHERE tipoiva.idcuenta = cuenta.idcuenta AND cuenta.codigo LIKE '477%'";
    conexionbase->begin();
    cur = conexionbase->cargacursor(SQLQuery, "elcursor");
    conexionbase->commit();
    m_listRepercutido->setNumRows(cur->numregistros());
    j =0;
    while (! cur->eof() ) {
        long int baseiva = cur->valor("tbaseiva").replace(".","").toInt();
        long int porcent = cur->valor("porcentajetipoiva").replace(".","").toInt();
        long int baseimp = baseiva*10000 / porcent;
        QString numberstr = QString::number(baseimp);
        numberstr = numberstr.left(numberstr.length()-2)+"."+numberstr.right(2);
        m_listRepercutido->setText(j, RES_NOMBRETIPOIVAREPERCUTIDO,cur->valor("nombretipoiva"));
        m_listRepercutido->setText(j, RES_IVAREPERCUTIDO,cur->valor("tbaseiva"));
        m_listRepercutido->setText(j, RES_BASEREPERCUTIDO, numberstr);
        tivar+= baseiva;
        tbaseimpr+=baseimp;
        cur->siguienteregistro();
        j++;
    }// end while
    delete cur;


    QString numberstr = QString::number(tivas);
    numberstr = numberstr.left(numberstr.length()-2)+"."+numberstr.right(2);
    m_ivas->setText(numberstr);
    numberstr = QString::number(tivar);
    numberstr = numberstr.left(numberstr.length()-2)+"."+numberstr.right(2);
    m_ivar->setText(numberstr);
    
    SQLQuery = "select sum(baseimp) AS tbaseimp from registroiva WHERE ffactura >='"+finicial->text()+"' AND ffactura <='"+ffinal->text()+"' AND idregistroiva IN (SELECT idregistroiva FROM iva, cuenta, tipoiva  WHERE iva.idtipoiva=tipoiva.idtipoiva AND tipoiva.idcuenta= cuenta.idcuenta AND cuenta.codigo LIKE '472%' AND baseiva <> 0)";
    cur = conexionbase->cargacursor(SQLQuery);
/*
    numberstr = QString::number(tbaseimps);
    numberstr = numberstr.left(numberstr.length()-2)+"."+numberstr.right(2);
    m_baseimps->setText(numberstr);
 */
    m_baseimps->setText(cur->valor("tbaseimp"));
    delete cur;
    
    SQLQuery = "select sum(baseimp) AS tbaseimp from registroiva WHERE ffactura >='"+finicial->text()+"' AND ffactura <='"+ffinal->text()+"' AND idregistroiva IN (SELECT idregistroiva FROM iva, cuenta, tipoiva  WHERE iva.idtipoiva=tipoiva.idtipoiva AND tipoiva.idcuenta= cuenta.idcuenta AND cuenta.codigo LIKE '477%' AND baseiva <> 0)";    
    cur = conexionbase->cargacursor(SQLQuery);
/*    
    numberstr = QString::number(tbaseimpr);
    numberstr = numberstr.left(numberstr.length()-2)+"."+numberstr.right(2);
    m_baseimpr->setText(numberstr);
*/
    m_baseimpr->setText(cur->valor("tbaseimp"));
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



    query.sprintf("SELECT * FROM registroiva, cuenta, borrador, asiento  where cuenta.idcuenta=borrador.idcuenta AND borrador.idborrador=registroiva.idborrador AND asiento.idasiento=borrador.idasiento AND (cuenta.codigo LIKE '43%%' OR cuenta.codigo LIKE '600%%') AND borrador.fecha>='%s' AND borrador.fecha<='%s'",finicial->text().ascii(), ffinal->text().ascii());
    conexionbase->begin();
    cursor2 *cursorreg = conexionbase->cargacursor(query,"cmquery");
    conexionbase->commit();
    int i =0;
    cursor2 *cursorcontra;
    tablasoportado->setNumRows(cursorreg->numregistros());
    while (!cursorreg->eof()) {
        query.sprintf("SELECT * FROM cuenta WHERE cuenta.idcuenta=%s",cursorreg->valor("contrapartida").ascii());
        conexionbase->begin();
        cursorcontra = conexionbase->cargacursor(query,"contra");
        conexionbase->commit();
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

        long int fbaseimp = cursorreg->valor("baseimp").replace(".","").toInt();
        long int fiva = cursorreg->valor("iva").replace(".","").toInt();
        long int total = fbaseimp * fiva /100;
        QString numberstr = QString::number(total);
        numberstr = numberstr.left(numberstr.length()-2)+"."+numberstr.right(2);
        tablasoportado->setText(i,S_COL_TOTAL,numberstr);
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

    tablasoportado->hideColumn(R_COL_CUENTA_IVA);
    tablasoportado->hideColumn(R_COL_IDASIENTO);
    tablasoportado->hideColumn(R_COL_IDBORRADOR);
    tablasoportado->hideColumn(R_COL_NUMASIENTO);
    tablasoportado->hideColumn(R_COL_CONTRAPARTIDA);
    tablasoportado->hideColumn(R_COL_PORCENT_IVA);


    // Hacemos el calculo de los que no pertenecen a iva soportado pq así entran todos.
    query.sprintf("SELECT * FROM registroiva, cuenta, borrador, asiento  WHERE cuenta.idcuenta=borrador.idcuenta AND borrador.idborrador=registroiva.idborrador AND asiento.idasiento=borrador.idasiento AND (cuenta.codigo NOT LIKE '43%%' AND cuenta.codigo NOT LIKE '600%%') AND borrador.fecha>='%s' AND borrador.fecha<='%s'ORDER BY borrador.fecha",finicial->text().ascii(), ffinal->text().ascii());
    conexionbase->begin();
    cursorreg = conexionbase->cargacursor(query,"cmquery");
    conexionbase->commit();
    i =0;
    tablarepercutido->setNumRows(cursorreg->numregistros());
    while (!cursorreg->eof()) {
        query.sprintf("SELECT * FROM cuenta WHERE cuenta.idcuenta=%s",cursorreg->valor("contrapartida").ascii());
        conexionbase->begin();
        cursorcontra = conexionbase->cargacursor(query,"contra");
        conexionbase->commit();
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

        long int fbaseimp = cursorreg->valor("baseimp").replace(".","").toInt();
        long int fiva = cursorreg->valor("iva").replace(".","").toInt();
        long int total = fbaseimp * fiva /100;
        QString numberstr = QString::number(total);
        numberstr = numberstr.left(numberstr.length()-2)+"."+numberstr.right(2);

        tablarepercutido->setText(i,R_COL_TOTAL,numberstr);
        tablarepercutido->setText(i,R_COL_FACTURA,cursorreg->valor("factura"));
        tablarepercutido->setText(i,R_COL_CIF,cursorreg->valor("cif"));
        tablarepercutido->setText(i,R_COL_NUMASIENTO,cursorreg->valor("ordenasiento"));
        tablarepercutido->setText(i,R_COL_IDASIENTO,cursorreg->valor("idasiento"));
        tablarepercutido->setText(i,R_COL_IDBORRADOR, cursorreg->valor("idborrador"));
        i++;
        cursorreg->siguienteregistro();
    }// end While
    delete cursorreg;
}// end inicializa





void listivaview::menu_contextual(int row, int , const QPoint &poin) {
    // Si el asiento esta cerrado el menu a mostrar es diferente
    QPopupMenu *popup = new QPopupMenu;
    popup->insertItem(tr("Ver Asiento"), 0);
    popup->insertSeparator();
    popup->insertItem(tr("Editar Registro"),101);
    popup->insertItem(tr("Borrar Registro"),103);
    int opcion = popup->exec(poin);
    switch(opcion) {
    case 0:
        int idasiento;
        idasiento = atoi(tablasoportado->text(row,S_COL_IDASIENTO).ascii());
        //introapunts->muestraasiento(idasiento);
        introapunts->flashAsiento(idasiento);
        introapunts->show();
        introapunts->setFocus();
        done(1);
        break;
    case 101:
        int idborrador = atoi(tablasoportado->text(row,S_COL_IDBORRADOR).ascii());
        if (idborrador != 0) {
            ivaview *nuevae=new ivaview(empresaactual,0,"");
            nuevae->inicializa1(idborrador);
            nuevae->exec();
            delete nuevae;
        }// end if
        break;
    }// end switch
    delete popup;

}// end contextmenu


void listivaview::menu_contextual1(int row, int , const QPoint &poin) {
    // Si el asiento esta cerrado el menu a mostrar es diferente
    QPopupMenu *popup = new QPopupMenu;
    popup->insertItem(tr("Ver Asiento"), 0);
    popup->insertSeparator();
    popup->insertItem(tr("Editar entrada de IVA"),101);
    popup->insertItem(tr("Borrar Registro"),103);
    int opcion = popup->exec(poin);
    switch(opcion) {
    case 0:
        int idasiento;
        idasiento = atoi(tablarepercutido->text(row,R_COL_IDASIENTO).ascii());
        introapunts->flashAsiento(idasiento);
        introapunts->show();
        introapunts->setFocus();
        done(1);
        break;
    case 101:
        int idborrador = atoi(tablarepercutido->text(row,R_COL_IDBORRADOR).ascii());
        if (idborrador != 0) {
            ivaview *nuevae=new ivaview(empresaactual, 0,"");
            nuevae->inicializa1(idborrador);
            nuevae->exec();
            delete nuevae;
        }// end if
        break;
    }// end switch
    delete popup;
}// end contextmenu

/** \brief ESta funcion responde a la pulsación del boton de busqueda de fecha inicial
**/
void listivaview::boton_finicial() {
    finicial->setText("+");
}// end boton_finicial

void listivaview::finicial_textChanged( const QString & texto ) {
    if (texto=="+") {
        QList<QDate> a;
        finicial->setText("");
        calendario *cal = new calendario(0,0);
        cal->exec();
        a = cal->dn->selectedDates();
        finicial->setText(a.first()->toString("dd/MM/yyyy"));
        delete cal;
    }// end if
    if (texto=="*")
        finicial->setText(QDate::currentDate().toString("dd/MM/yyyy") );
}//fin fechaasiento1_textChanged


/** Esta función responde a la pulsación del boton de busqueda de fecha final
**/
void listivaview::boton_ffinal() {
    ffinal->setText("+");
}// end boton_ffinal


void listivaview::ffinal_textChanged( const QString & texto ) {
    if (texto=="+") {
        QList<QDate> a;
        ffinal->setText("");
        calendario *cal = new calendario(0,0);
        cal->exec();
        a = cal->dn->selectedDates();
        ffinal->setText(a.first()->toString("dd/MM/yyyy"));
        delete cal;
    }// end if
    if (texto=="*")
        ffinal->setText(QDate::currentDate().toString("dd/MM/yyyy") );
}//fin fechaasiento1_textChanged


void listivaview::finicial_lostFocus() {
    finicial->setText(normalizafecha(finicial->text()).toString("dd/MM/yyyy"));
}// end return_fechainicial

void listivaview::ffinal_lostFocus() {
    ffinal->setText(normalizafecha(ffinal->text()).toString("dd/MM/yyyy"));
}// end return_fechainicial


