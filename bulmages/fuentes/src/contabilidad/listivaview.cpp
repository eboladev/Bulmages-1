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

Mod300ps *modelo;

listivaview::listivaview(empresa * emp, QString ejerActual, QWidget *parent, const char *name ) : listivadlg(parent,name) {
   empresaactual = emp;
   conexionbase = emp->bdempresa();
  //QDate fecha = QDate::currentDate();
  //QString buffer;
  //buffer.sprintf("01/01/%d",fecha.year());
  //finicial->setText(buffer);
  //buffer.sprintf("31/12/%d",fecha.year());
  //ffinal->setText(buffer);
  finicial->setText(normalizafecha("01/01/"+ejerActual).toString("dd/MM/yyyy"));
  ffinal->setText(normalizafecha("31/12/"+ejerActual).toString("dd/MM/yyyy"));
  modelo=new Mod300ps(this->parentWidget());

}// end listivaview


listivaview::~listivaview(){
}// end ~lisivaview


/********************************************************************
 * Al hacer doble click sobre la tabla de ivas se accede al asiento *
 * que tiene dicha entrada                                          *
 ********************************************************************/
void listivaview::doble_click_soportado(int a, int b, int c, const QPoint &punto) {
  int idasiento;
  idasiento = atoi(tablasoportado->text(a,S_COL_IDASIENTO).ascii());
  //introapunts->muestraasiento(idasiento);
  introapunts->flashAsiento(idasiento);
  introapunts->show();
  introapunts->setFocus();
  done(1);
  // Para quitar el warnings
  b=c=0;
  punto.isNull();
}// end doble_click_soportado


/********************************************************************
 * Al hacer doble click sobre la tabla de ivas se accede al asiento *
 * que tiene dicha entrada                                          *
 ********************************************************************/
void listivaview::doble_click_repercutido(int a, int b, int c, const QPoint &punto) {
  int idasiento;
  idasiento = atoi(tablarepercutido->text(a,R_COL_IDASIENTO).ascii());
  //introapunts->muestraasiento(idasiento);
  introapunts->flashAsiento(idasiento);
  introapunts->show();
  introapunts->setFocus();
  done(1);
  
  //Para quitar el warnings
  b=c=0;
  punto.isNull();
}// end doble_click_repercutido


void listivaview::boton_print() {
   regivaprintview *print = new regivaprintview(0,0);
   print->inicializa(conexionbase);
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
    float  fbaseimp, fiva;
    QString   cbaseimp, civa, ctotal;
    double ivas16=0, ivas4=0, ivas7=0;
    double ivar16=0, ivar4=0, ivar7=0;
    double bases16=0, bases4=0, bases7=0, bases0=0;
    double baser16=0, baser4=0, baser7=0, baser0=0;
    
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

    //query.sprintf("SELECT * FROM registroiva, cuenta, borrador, asiento  where cuenta.idcuenta=borrador.idcuenta AND borrador.idborrador=registroiva.idborrador AND asiento.idasiento=borrador.idasiento AND cuenta.codigo LIKE '472%%' AND borrador.fecha>='%s' AND borrador.fecha<='%s'",finicial->text().ascii(), ffinal->text().ascii());
    query.sprintf("SELECT * FROM registroiva, cuenta, borrador, asiento  where cuenta.idcuenta=borrador.idcuenta AND borrador.idborrador=registroiva.idborrador AND asiento.idasiento=borrador.idasiento AND (cuenta.codigo LIKE '472%%' OR cuenta.codigo LIKE '600%%') AND borrador.fecha>='%s' AND borrador.fecha<='%s'",finicial->text().ascii(), ffinal->text().ascii());
    //Si tinguessim un camp anomenat 'ctaiva' fariem:
    //query.sprintf("SELECT * FROM registroiva, cuenta, borrador, asiento  WHERE cuenta.idcuenta=borrador.idcuenta AND borrador.idborrador=registroiva.idborrador AND asiento.idasiento=borrador.idasiento AND registroiva.ctaiva LIKE '472%%' AND borrador.fecha>='%s' AND borrador.fecha<='%s' ORDER BY to_number(registroiva.numorden,'99999')",finicial->text().ascii(), ffinal->text().ascii());
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
      if (tablasoportado->text(i,S_COL_CUENTA_IVA).left(3)!="472") tablasoportado->setText(i,S_COL_CUENTA_IVA,tr("Exento"));
      sbaseimp = cursorreg->valor("baseimp");
      fbaseimp = atof(sbaseimp.ascii());
      cbaseimp.sprintf("%2.2f",fbaseimp);
      tablasoportado->setText(i,S_COL_BASEIMP,cbaseimp);
      siva = cursorreg->valor("iva");
      fiva = atof(siva.ascii());
      switch (atoi(siva.ascii())) {
        case 16:
          ivas16 += fbaseimp*fiva/100;
          bases16 += fbaseimp;
          break;
        case 7:
          ivas7 += fbaseimp*fiva/100;
          bases7 += fbaseimp;
          break;
        case 4:
          ivas4 += fbaseimp*fiva/100;
          bases4 += fbaseimp;
          break;
        case 0:
            bases0 += fbaseimp;
      }// end switch
      civa.sprintf("%2.0f",fiva);
      tablasoportado->setText(i,S_COL_PORCENT_IVA,civa);
      ctotal.sprintf("%2.2f",fbaseimp*fiva/100);
      tablasoportado->setText(i,S_COL_TOTAL,ctotal);
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
    //query.sprintf("SELECT * FROM registroiva, cuenta, borrador, asiento  WHERE cuenta.idcuenta=borrador.idcuenta AND borrador.idborrador=registroiva.idborrador AND asiento.idasiento=borrador.idasiento AND cuenta.codigo LIKE '477%%' AND borrador.fecha>='%s' AND borrador.fecha<='%s'ORDER BY borrador.fecha",finicial->text().ascii(), ffinal->text().ascii());
    query.sprintf("SELECT * FROM registroiva, cuenta, borrador, asiento  WHERE cuenta.idcuenta=borrador.idcuenta AND borrador.idborrador=registroiva.idborrador AND asiento.idasiento=borrador.idasiento AND (cuenta.codigo LIKE '477%%' OR cuenta.codigo LIKE '700%%') AND borrador.fecha>='%s' AND borrador.fecha<='%s'ORDER BY borrador.fecha",finicial->text().ascii(), ffinal->text().ascii());     
    //Si tinguessim un camp anomenat 'ctaiva' fariem:
    //query.sprintf("SELECT * FROM registroiva, cuenta, borrador, asiento  WHERE cuenta.idcuenta=borrador.idcuenta AND borrador.idborrador=registroiva.idborrador AND asiento.idasiento=borrador.idasiento AND registroiva.ctaiva LIKE '477%%' AND borrador.fecha>='%s' AND borrador.fecha<='%s'ORDER BY borrador.fecha, to_number(registroiva.factura,'99999')",finicial->text().ascii(), ffinal->text().ascii());
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
      if (tablarepercutido->text(i,R_COL_CUENTA_IVA).left(3)!="477") tablarepercutido->setText(i,R_COL_CUENTA_IVA,tr("Exento"));
      sbaseimp = cursorreg->valor("baseimp");
      fbaseimp = atof(sbaseimp.ascii());
      cbaseimp.sprintf("%2.2f",fbaseimp);
      tablarepercutido->setText(i,R_COL_BASEIMP,cbaseimp);
      siva = cursorreg->valor("iva");
      tablarepercutido->setText(i,R_COL_PORCENT_IVA,siva);
      fiva = atof(siva.ascii());
      switch (atoi(siva.ascii())) {
        case 16:
          ivar16 += fbaseimp*fiva/100;
          baser16 += fbaseimp;
          break;
        case 7:
          ivar7 += fbaseimp*fiva/100;
          baser7 += fbaseimp;
          break;
        case 4:
          ivar4 += fbaseimp*fiva/100;
          baser4 += fbaseimp;
          break;
        case 0:
            baser0 += fbaseimp;
          
      }// end switch
      civa.sprintf("%2.0f",fiva);
      tablarepercutido->setText(i,3,civa);
      ctotal.sprintf("%2.2f",fbaseimp*fiva/100);
      tablarepercutido->setText(i,R_COL_TOTAL,ctotal);
      tablarepercutido->setText(i,R_COL_FACTURA,cursorreg->valor("factura"));
      tablarepercutido->setText(i,R_COL_CIF,cursorreg->valor("cif"));
      tablarepercutido->setText(i,R_COL_NUMASIENTO,cursorreg->valor("ordenasiento"));
      tablarepercutido->setText(i,R_COL_IDASIENTO,cursorreg->valor("idasiento"));
      tablarepercutido->setText(i,R_COL_IDBORRADOR, cursorreg->valor("idborrador"));
      i++;
      cursorreg->siguienteregistro();
    }// end While
    delete cursorreg;



    // Vamos a poner los totales.
    QString cadena;
    cadena.sprintf("%2.2f",ivas4);
    siva4->setText(cadena);
    cadena.sprintf("%2.2f",ivas7);
    siva7->setText(cadena);
    cadena.sprintf("%2.2f",ivas16);
    siva16->setText(cadena);
    cadena.sprintf("%2.2f",ivas4+ivas7+ivas16);
    sivat->setText(cadena);
        
    cadena.sprintf("%2.2f",ivar4);
    riva4->setText(cadena);
    cadena.sprintf("%2.2f",ivar7);
    riva7->setText(cadena);
    cadena.sprintf("%2.2f",ivar16);
    riva16->setText(cadena);
    cadena.sprintf("%2.2f",ivar4+ivar7+ivar16);
    rivat->setText(cadena);


    cadena.sprintf("%2.2f",bases0);
    tbasese->setText(cadena);
    cadena.sprintf("%2.2f",bases4);
    tbases4->setText(cadena);
    cadena.sprintf("%2.2f",bases7);
    tbases7->setText(cadena);
    cadena.sprintf("%2.2f",bases16);
    tbases16->setText(cadena);
    cadena.sprintf("%2.2f",bases0+bases4+bases7+bases16);
    tbases->setText(cadena);

    cadena.sprintf("%2.2f",baser0);
    tbasere->setText(cadena);
    cadena.sprintf("%2.2f",baser4);
    tbaser4->setText(cadena);
    cadena.sprintf("%2.2f",baser7);
    tbaser7->setText(cadena);
    cadena.sprintf("%2.2f",baser16);
    tbaser16->setText(cadena);
    cadena.sprintf("%2.2f",baser0+baser4+baser7+baser16);
    tbaser->setText(cadena);      
    
        
    modelo->ivas4=ivas4;
    modelo->ivas7=ivas7;
    modelo->ivas16=ivas16;
    
     modelo->ivar4=ivar4;
    modelo->ivar7=ivar7;
    modelo->ivar16=ivar16;
    
     modelo->bases0=bases0;
    modelo->bases4=bases4;
    modelo->bases7=bases7;
    modelo->bases16=bases16;
    
     modelo->baser0=baser0;
    modelo->baser4=baser4;
    modelo->baser7=baser7;
    modelo->baser16=baser16;

    
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
//               nuevae->inicializa(1); //el "1" indica IVA Soportado
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
           //introapunts->muestraasiento(idasiento);
           introapunts->flashAsiento(idasiento);
           introapunts->show();
           introapunts->setFocus();
           done(1);
           break;
        case 101:
           int idborrador = atoi(tablarepercutido->text(row,R_COL_IDBORRADOR).ascii());
           if (idborrador != 0) {
               ivaview *nuevae=new ivaview(empresaactual, 0,"");
//               nuevae->inicializa(2); //el "2" indica IVA Repercutido
               nuevae->inicializa1(idborrador);
               nuevae->exec();
               delete nuevae;
           }// end if            
        break;
    }// end switch
    delete popup;
}// end contextmenu




