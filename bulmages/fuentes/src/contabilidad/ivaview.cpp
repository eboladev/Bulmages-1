/***************************************************************************
                          ivaview.cpp  -  description
                             -------------------
    begin                : Tue Jan 28 2003
    copyright            : (C) 2003 by Tomeu Borrás Riera & Josep Burción
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

 /*
-- La tabla prevcobro es prevision de cobros / pagos para facturas.
-- Esta tabla proporciona las formas de pago de cada factura que se pasa a un cliente o que recibimos de un proveedor.

-- tipoprevcobro indica si es un cobro (true) o un pago (false).
-- fpagoprevcobro es un identificador a la otra tabla de Formas de Pago.
-- idcuenta La cuenta sobre la que se hará el cobro / pago.
-- idasiento Asiento de cobro (Si está hecho).
-- cantidadprevistaprevcobro Cantidad Prevista del cobro.
-- cantidadprevcobro         Cantidad Cobrada.
-- fprevistaprevcobro        Fecha en que se prevee el cobro / pago.
-- fcobroprevcobro           Fecha en que se ha realizado el cobro / pago.
-- idregistroiva             Registro de IVA con el que se corresponde, o factura con la que se corresponde.
-- tipoprevcobro             Indica si es un cobro o un pago.


CREATE TABLE prevcobro (
    idprevcobro serial PRIMARY KEY,
    fprevistaprevcobro date,
    fcobroprevcobro date,
    fpagoprevcobro integer,
    idcuenta integer REFERENCES cuenta(idcuenta),
    idasiento integer REFERENCES asiento(idasiento),
    cantidadprevistaprevcobro double,
    cantidadprevcobro         double,
    idregistroiva NOT NULL REFERENCES registroiva(idregistroiva),
    tipoprevcobro boolean
);

 */
 
 
 
 
#include "ivaview.h"
#include "listcuentasview1.h"
#include "empresa.h"
#include "calendario.h"

#define IVA_SOPORTADO    1
#define IVA_REPERCUTIDO  2

#define COL_PREV_IDPREVCOBRO 0
#define COL_PREV_FPREVISTAPREVCOBRO 1
#define COL_PREV_FCOBROPREVCOBRO     2
#define COL_PREV_IDCUENTA  3
#define COL_PREV_CODCUENTA 4
#define COL_PREV_CANTIDADPREVISTAPREVCOBRO 5
#define COL_PREV_CANTIDADPREVCOBRO 6
#define COL_PREV_FPAGOPREVCOBRO 7
#define COL_PREV_IDREGISTROIVA 8




ivaview::ivaview(empresa *emp,QWidget *parent, const char *name ) : ivadlg(parent,name) {
   empresaactual = emp;
   conexionbase = emp->bdempresa();
   idborrador=0;
   idregistroiva =0;
  
   m_listPrevision->setNumCols(9);
   m_listPrevision->horizontalHeader()->setLabel( COL_PREV_IDPREVCOBRO, tr( "COL_PREV_IDPREVCOBRO") );
   m_listPrevision->horizontalHeader()->setLabel( COL_PREV_FPREVISTAPREVCOBRO, tr( "COL_PREV_FPREVISTAPREVCOBRO") );
   m_listPrevision->horizontalHeader()->setLabel( COL_PREV_FCOBROPREVCOBRO, tr( "COL_PREV_FCOBROPREVCOBRO") );
   m_listPrevision->horizontalHeader()->setLabel( COL_PREV_IDCUENTA, tr( "COL_PREV_IDCUENTA") );
   m_listPrevision->horizontalHeader()->setLabel( COL_PREV_CODCUENTA, tr( "COL_PREV_CODCUENTA") );
   m_listPrevision->horizontalHeader()->setLabel( COL_PREV_CANTIDADPREVISTAPREVCOBRO, tr( "COL_PREV_CANTIDADPREVISTAPREVCOBRO") );
   m_listPrevision->horizontalHeader()->setLabel( COL_PREV_CANTIDADPREVCOBRO, tr( "COL_PREV_CANTIDADPREVCOBRO") );
   m_listPrevision->horizontalHeader()->setLabel( COL_PREV_FPAGOPREVCOBRO, tr( "COL_PREV_FPAGOPREVCOBRO") );
   m_listPrevision->horizontalHeader()->setLabel( COL_PREV_IDREGISTROIVA, tr( "COL_PREV_IDREGISTROIVA") );
   m_listPrevision->setColumnWidth(COL_PREV_IDPREVCOBRO,100);
   m_listPrevision->setColumnWidth(COL_PREV_FPREVISTAPREVCOBRO,75);
   m_listPrevision->setColumnWidth(COL_PREV_FCOBROPREVCOBRO,75);
   m_listPrevision->setColumnWidth(COL_PREV_IDCUENTA,75);
   m_listPrevision->setColumnWidth(COL_PREV_CODCUENTA,75);
   m_listPrevision->setColumnWidth(COL_PREV_CANTIDADPREVISTAPREVCOBRO,75);
   m_listPrevision->setColumnWidth(COL_PREV_CANTIDADPREVCOBRO,75);
   m_listPrevision->setColumnWidth(COL_PREV_FPAGOPREVCOBRO,75);
   m_listPrevision->setColumnWidth(COL_PREV_IDREGISTROIVA,75);
   
   m_listPrevision->setNumRows(50);
}// end ivaview


ivaview::~ivaview(){
}// end ~ivaview


int ivaview::inicializa(int sop_rep=0) {
  setSoportadoRepercutido(sop_rep);
  return(0);
}// end inicializa


// Esquema:
// Si se trata de una modificacion modificamos.
// Si se trata de una insercion insertamos.
void ivaview::accept() {
  QString query;
  float baseimponible1=atof(baseimponible->text().ascii());
  float iva1 = atof(iva->text().ascii());
  QString factura1= factura->text();
  
  QString cif1 = cif->text();
  conexionbase->begin();
  cursor2 *cursorcuenta =conexionbase->cargacuenta(0,contrapartida->text());
  conexionbase->commit();
  if (!cursorcuenta->eof()) {
    int idcuenta= atoi(cursorcuenta->valor("idcuenta").ascii());
    if (idregistroiva !=0) {
          // Se trata de una inserción, ya que no hay registros de iva introducidos.
         query.sprintf("UPDATE registroiva set idborrador=%d, baseimp=%2.2f, iva=%2.0f, contrapartida=%d, factura='%s', numorden='%s', cif='%s' WHERE idregistroiva=%d", idborrador, baseimponible1, iva1, idcuenta, factura1.ascii(), numorden->text().ascii(), cif1.ascii(), idregistroiva);
    } else {
          // Se trata de una modificacion, ya que hay registros de iva introducidos.
         query.sprintf("INSERT INTO registroiva (idborrador, baseimp, iva, contrapartida, factura, numorden, cif) VALUES (%d,%2.2f, %2.0f, %d, '%s', '%s', '%s')", idborrador, baseimponible1, iva1, idcuenta, factura1.ascii(), numorden->text().ascii(), cif1.ascii());
    }// end if
    fprintf(stderr,"%s\n",query.ascii());
    conexionbase->begin();
    conexionbase->ejecuta(query);
    conexionbase->commit();
  }// end if
  delete cursorcuenta;
  done(1);
}// end accept


/***************************************************************************
 * Se ha pulsado sobre el boton de borrar en ivaview por lo que borraremos *
 * el registro actual.                                                     *
 ***************************************************************************/
void ivaview::boton_borrar() {
  QString query;
  if (idregistroiva != 0) {
    query.sprintf("DELETE FROM registroiva WHERE idregistroiva=%d",idregistroiva);
    conexionbase->begin();
    conexionbase->ejecuta(query);
    conexionbase->commit();
  }// end if
  done(1);
}// end boton_borrar


void ivaview::boton_buscacuenta() {
   listcuentasview1 *listcuentas = new listcuentasview1();
   listcuentas->modo=1;
   listcuentas->inicializa(conexionbase);
   listcuentas->exec();
   contrapartida->setText(listcuentas->codcuenta);
   delete listcuentas;
}// end boton_buscacuenta


void ivaview::cambiadacontrapartida() {
   fprintf(stderr,"cambiada la contrapartida\n");
}// end cambiadacontrapartida


// Esquema:
// 1.- Comprobamos que no haya ya un registro con la factura
// 2.- Si hay factura la ponemos, si no la hay sacamos los datos que podemos y los presentamos
void ivaview::inicializa1(int idapunte1) {
  QString query, cadena;
  double debe, haber, total_factura=0;
  idborrador = idapunte1;
  //Busca entradas de IVA en la tabla registroiva
  query.sprintf ( "SELECT * FROM registroiva, cuenta WHERE idborrador=%d AND registroiva.contrapartida=cuenta.idcuenta",idborrador);
  conexionbase->begin();
  cursor2 *cursoriva = conexionbase->cargacursor(query,"cursoriva");
  conexionbase->commit();
  if (!cursoriva->eof()) {
    // Se trata de un registro que ya ha sido introducido
    idregistroiva=atoi(cursoriva->valor("idregistroiva").ascii());
    contrapartida->setText(cursoriva->valor("codigo"));
    empfactura->setText(cursoriva->valor("nombreent_cuenta"));
    baseimponible->setText(cursoriva->valor("baseimp"));
    iva->setText(cursoriva->valor("iva"));
    factura->setText(cursoriva->valor("factura"));
    numorden->setText(cursoriva->valor("numorden"));
    cif->setText(cursoriva->valor("cif"));
    query.sprintf ("SELECT * from borrador, asiento, cuenta WHERE borrador.idborrador=%d AND borrador.idasiento=asiento.idasiento AND borrador.idcuenta=cuenta.idcuenta",idborrador);
    conexionbase->begin();
    cursor2 * cursorapunte = conexionbase->cargacursor(query,"cursorapunte");
    conexionbase->commit();
    if (!cursorapunte->eof()) {
      numasiento->setText(cursorapunte->valor("ordenasiento")); 
      cuentaiva->setText(cursorapunte->valor("codigo"));
    }// end if
    if (cuentaiva->text().left(3) == "600") {
      cuentaiva->setText("472");
      iva->setText("0");
      importeiva->setText("0.00");
    }// end if
    if (cuentaiva->text().left(3) == "700") {
      cuentaiva->setText("477");
      iva->setText("0");
      importeiva->setText("0.00");
    }// end if
    delete cursorapunte;
  } else {
      // Aun no se ha metido ningun registro de este estilo, debe inicializarse.
      // Hay una problematica al meterse asientos con multiples entradas de IVA, la cosa es durilla.
      query.sprintf ("SELECT * from borrador, asiento, cuenta WHERE borrador.idborrador=%d AND borrador.idasiento=asiento.idasiento AND borrador.idcuenta=cuenta.idcuenta",idborrador); //Josep Burción
      fprintf(stderr,"%s\n",query.ascii());
      conexionbase->begin();
      cursor2 * cursorapunte = conexionbase->cargacursor(query,"cursorapunte");
      conexionbase->commit();
      if (!cursorapunte->eof()) {
        fprintf(stderr,"Asiento %s\n",cursorapunte->valor("idasiento").ascii());
        numasiento->setText(cursorapunte->valor("ordenasiento"));
        cuentaiva->setText(cursorapunte->valor("codigo"));
        fprintf(stderr,"Cuenta usuario %s\n", cursorapunte->valor("conceptocontable").ascii());
        debe = atof(cursorapunte->valor("debe").ascii());
        haber = atof(cursorapunte->valor("haber").ascii());
        
        // La funcion bcontrapartidaborr (definida en la base de datos) busca contrapartidas de forma más compleja a como
        // se propuso en un principio. Ahora en un mismo apunte con multiples entradas se encuentra la contrapartida mediante proximidades en el mismo cuadre.
	// Atención a este SELECT que tiene tela,
	// Si se hace un SELECT * FROM ..... WHERE borrador.idborrador = bcontrapartidaborr(%d) el rendimiento cae en picado.
        query.sprintf("SELECT * FROM borrador,cuenta WHERE borrador.idcuenta = cuenta.idcuenta AND borrador.idborrador IN  (SELECT bcontrapartidaborr(%d))",idborrador);
        fprintf(stderr,"%s\n",query.ascii());
        conexionbase->begin();
        cursor2 *cursorcontrapartida = conexionbase->cargacursor(query,"contrapartida");
        conexionbase->commit();
        // en el caso en que exista una contrapartida, se podra calcular la contrapartida
        // Y la base imponible.
        if (!cursorcontrapartida->eof()) {
          empfactura->setText(cursorcontrapartida->valor("nombreent_cuenta"));
          contrapartida->setText(cursorcontrapartida->valor("codigo"));
          cif->setText(cursorcontrapartida->valor("cifent_cuenta"));
          total_factura= atof(cursorcontrapartida->valor("debe").ascii());
          if (total_factura == 0) {
            total_factura = atof(cursorcontrapartida->valor("haber").ascii());
          }// end if
          fprintf(stderr,"total factura %2.2f\n",total_factura);
          double iva1 = atof(cursorapunte->valor("debe").ascii());
          if (iva1 == 0) {
            iva1 = atof(cursorapunte->valor("haber").ascii());
          }// end if
          double baseimponible1 = total_factura - iva1;
          double porcentiva = total_factura/baseimponible1 -1;
          cadena.sprintf("%2.2f",baseimponible1);
          baseimponible->setText(cadena);
          cadena.sprintf("%2.0f",porcentiva*100);
          iva->setText(cadena);
          if (cuentaiva->text().left(3) == "600") {
              cuentaiva->setText("472");
              iva->setText("0");
              importeiva->setText("0.00");
              cadena.sprintf("%2.2f",total_factura);
              baseimponible->setText(cadena);
          }
          if (cuentaiva->text().left(3) == "700") {
              cuentaiva->setText("477");
              iva->setText("0");
              importeiva->setText("0.00");
              cadena.sprintf("%2.2f",total_factura);
              baseimponible->setText(cadena);
          }
          
//* JOSEP BURCION
          //Proponemos un número de factura si se trata de una venta y
          //proponemos un número de orden si se trata de una compra
          //Realmente esto se tendria que implementar con contadores en
          //la base de datos.
          cursor2 * recordset;
          //Primero comprovamos si esta factura ya tiene un apunte de iva distinto y cojemos el mismo numero de factura
          query.sprintf("SELECT factura, numorden FROM registroiva WHERE idborrador IN (SELECT idborrador FROM borrador WHERE idasiento=(SELECT idasiento FROM borrador WHERE idborrador='%i'))",idborrador);
          conexionbase->begin();
          recordset = conexionbase->cargacursor(query,"recordset");
          conexionbase->commit();
          if (!recordset->eof()) {
              if (! numorden->isEnabled()) {  //Se trata de una Venta
                  factura->setText(recordset->valor("factura"));
              } else { //Se trata de una compra
                  factura->setText(recordset->valor("factura"));
                  numorden->setText(recordset->valor("numorden"));
              }
              
          }  else {   //La factura no existe, entonces proponemos el siguiente número de factura
              if (! numorden->isEnabled()) {  //Se trata de una Venta
                  //query.sprintf( "SELECT factura FROM registroiva WHERE idborrador=(SELECT MAX(idborrador) FROM registroiva WHERE numorden='-1')");
                  query.sprintf("SELECT MAX(to_number(factura,'99999')) AS factura FROM registroiva WHERE numorden=''");
                  conexionbase->begin();
                  recordset = conexionbase->cargacursor(query,"recordset");
                  conexionbase->commit();
                  if (!recordset->eof()) {
                      int numfact = 1 + atoi(recordset->valor("factura").ascii());
                      cadena.sprintf("%i",numfact);
                      factura->setText(cadena);
                  }
              }
              else { //Se trata de una compra
                  //query.sprintf( "SELECT numorden FROM registroiva WHERE idborrador=(SELECT MAX(idborrador) FROM registroiva WHERE numorden<>'-1')");
                  query.sprintf("SELECT MAX(to_number(numorden,'99999')) AS numorden FROM registroiva WHERE numorden<>''");
                  conexionbase->begin();
                  recordset = conexionbase->cargacursor(query,"recordset");
                  conexionbase->commit();
                  if (!recordset->eof()) {
                      int numord = 1 + atoi(recordset->valor("numorden").ascii());
                      cadena.sprintf("%i",numord);
                      numorden->setText(cadena);
                 }// end if
              }// end if
          }// end if
          delete recordset;//Fin proposicion numeros factura y orden.
//* END JOSEP BURCION 
        }// end if
        delete cursorcontrapartida;
      }// end if
      delete cursorapunte;
  }// end if
  delete cursoriva;
  
  
  // Cargamos las formas de pago correspondientes y las ponemos como toca.
  QString SQLQuery;
  SQLQuery.sprintf("SELECT * FROM prevcobro LEFT JOIN cuenta ON prevcobro.idcuenta = cuenta.idcuenta WHERE idregistroiva=%d", idregistroiva);
  conexionbase->begin();
  cursor2 *curprevcobros = conexionbase->cargacursor(SQLQuery, "queryprevcobros");
  conexionbase->commit();
  int i =0;
  while ( ! curprevcobros->eof()) {  
      m_listPrevision->setText(i, COL_PREV_IDPREVCOBRO,curprevcobros->valor("idprevcobro"));
      m_listPrevision->setText(i, COL_PREV_FPREVISTAPREVCOBRO,curprevcobros->valor("fprevistaprevcobro"));
      m_listPrevision->setText(i, COL_PREV_FCOBROPREVCOBRO,curprevcobros->valor("fcobroprevcobro"));
      m_listPrevision->setText(i, COL_PREV_IDCUENTA,curprevcobros->valor("idcuenta"));
      m_listPrevision->setText(i, COL_PREV_CODCUENTA,curprevcobros->valor("codigo"));
      m_listPrevision->setText(i, COL_PREV_CANTIDADPREVISTAPREVCOBRO,curprevcobros->valor("cantidadprevistaprevcobro"));
      m_listPrevision->setText(i, COL_PREV_CANTIDADPREVCOBRO,curprevcobros->valor("cantidadprevcobro"));
      m_listPrevision->setText(i, COL_PREV_FPAGOPREVCOBRO,curprevcobros->valor("fpagoprevcobro"));
      m_listPrevision->setText(i, COL_PREV_IDREGISTROIVA,curprevcobros->valor("idregistroiva"));
      i++;
      curprevcobros->siguienteregistro(); 
  }// end while
  delete curprevcobros;
  
}// end inicializa1


//El usuario cambia el % de IVA
//Entonces recalculamos el importe de IVA
void ivaview::iva_changed() {
    importeiva->setText(QString::number(((baseimponible->text().toFloat()) *(iva->text().toFloat()))/100,'f',2));
}

//El usuario cambia el importe del IVA
//Entonces recalculamos la base
//Controlamos que IVA no tenga valor ZERO para evitar "DIVISION POR ZERO!!"
void ivaview::importeiva_changed() {
    if (iva->text().toFloat() != 0) {
        baseimponible->setText(QString::number((importeiva->text().toFloat() * 100) / iva->text().toFloat(),'f',2));
    }
}

//El usuario cambia la base del IVA
//Entonces recalculamos el importe
void ivaview::baseiva_lostfocus() {
    iva_changed(); //Hacemos lo mismo que en iva_changed();
}

//Cambia ligeramente el aspecto del dialogo en funcion de si visualizamos un IVA soportado o repercutido.
// 1 = Soportado (Compra)
// 2 = Repercutido (Venta)
void ivaview::setSoportadoRepercutido(int sop_rep=0) {
  if (sop_rep==1) setCaption("Iva soportado (COMPRAS)"); 
  else if (sop_rep==2) setCaption("Iva repercutido (VENTAS)");
  else setCaption("Iva soportado/repercutido");
  numorden->setEnabled(sop_rep==1);
}

/**
  * \brief Esta función guarda una linea de la tabla de previsiones
  *
  * La linea se guarda en la base de datos prevcobro, la tabla afectada es m_listPrevision
  * El parametro que se pasa es la columna de la tabla que queremos guardar en la base de datos.
  */
void ivaview::guardaprevpago (int numrow) {
   QString idprevpago = m_listPrevision->text(numrow, COL_PREV_IDPREVCOBRO);
   if (idprevpago != "") {       // Ya un elemento, debemos hacer un UPDATE
      QString SQLQuery = "UPDATE prevcobro SET idcuenta="+m_listPrevision->text(numrow, COL_PREV_IDCUENTA);
      SQLQuery += " WHERE idprevcobro="+idprevpago;
      conexionbase->begin();
      conexionbase->ejecuta(SQLQuery);
      conexionbase->commit();
   } else {                      // Hay que hacer un INSERT
   }// end if
}// end guardaprevpago

/**
  * \brief SLOT que captura el cambio de foco en la rejilla de Previsiones de cobros y pagos.
  * Detecta si se ha cambiado de fila y si es el caso se guarda el registro.
  */
void ivaview::tcambiaseleccion() {
   static int rowactual=0;
   if (rowactual != m_listPrevision->currentRow())
       guardaprevpago(rowactual);
   rowactual = m_listPrevision->currentRow();
}// end ivaview

/**
  * \brief SLOT que captura el cambio de foco en la rejilla de Previsiones de cobros y pagos.
  * Detecta si se ha cambiado de fila y si es el caso se guarda el registro.
  */
void ivaview::cambiadogrid(int row, int col) {
   QString fecha;
   switch (col) {
      case COL_PREV_FPREVISTAPREVCOBRO:
         fecha = normalizafecha(m_listPrevision->text(row,col)).toString("dd/MM/yyyy");
         m_listPrevision->setText(row, col, fecha);
      break;
   }// end switch
}// end apuntecambiadogrid

/**
  * \brief SLOT que captura la pulsación de determinadas telcas especiales para la aplicacion
  * Se emite cuando se han pulsado determinadas teclas en la edición de la tabla de cobros/pagos.
  * Responde a la pulsación del '+' y del '*'
  */
void ivaview::pulsadomas(int row, int col, int caracter) { 
   QString query, fecha;
//   QPopupMenu *menucanal = new QPopupMenu( this );
//   QPopupMenu *menucoste = new QPopupMenu( this );
//   int opcion;
   switch (caracter) {
      case '+':
         switch(col) {
            case COL_PREV_FPREVISTAPREVCOBRO:
               int dia, mes, ano;
               QList<QDate> a;
               QString cadena;
               calendario *cal = new calendario(0,0);
               cal->exec();
               a = cal->dn->selectedDates();
               dia = a.first()->day();
               mes = a.first()->month();
               ano = a.first()->year();
               cadena.sprintf( "%2.2d/%2.2d/%d",dia, mes, ano);
               fprintf(stderr,"Se ha pulsado:%s\n", cadena.ascii());
               m_listPrevision->setText(row, COL_PREV_FPREVISTAPREVCOBRO, cadena);
               delete cal;
            break;
         }// end switch
      case '*':
         fprintf(stderr,"Se ha pulsado el *\n");
         //duplicar(col);
      break;
      case 4100:
         fprintf(stderr,"Se ha pulsado el enter\n");
         switch (col) {
            case COL_PREV_FPREVISTAPREVCOBRO:
               fecha = normalizafecha(m_listPrevision->text(row,col)).toString("dd/MM/yyyy");
               m_listPrevision->setText(row, col, fecha);
               m_listPrevision->setCurrentCell(row, COL_PREV_FCOBROPREVCOBRO);
            break;
/*
            case COL_SUBCUENTA:
                tapunts->setCurrentCell(row, COL_DEBE);
                break;
            case COL_CONTRAPARTIDA:
                tapunts->setCurrentCell(row, COL_DEBE);
                break;
            case COL_IVA:
//                Se ha verificado que es más cómodo poder cambiar la fecha o dar al enter
               if (row >0) {
                  if (tapunts->text(row-1,COL_CCOSTE) == "") 
                     tapunts->setText(row+1,COL_FECHA,normalizafecha(tapunts->text(row,COL_FECHA)).toString("dd/MM/yyyy"));
                     tapunts->setText(row+1,COL_CCOSTE, tapunts->text(row,COL_CCOSTE));
                     tapunts->setText(row+1,COL_CANAL, tapunts->text(row,COL_CANAL));
                     tapunts->setText(row+1,COL_IDCCOSTE, tapunts->text(row,COL_IDCCOSTE));
                     tapunts->setText(row+1,COL_IDCANAL, tapunts->text(row,COL_IDCANAL));
                     tapunts->setCurrentCell(row+1, COL_FECHA);               
               } else {
                     tapunts->setCurrentCell(row,COL_CCOSTE);
               }// end if
                break;
            case COL_CCOSTE:
               if (row > 0) {
                     tapunts->setText(row+1,COL_FECHA,normalizafecha(tapunts->text(row,COL_FECHA)).toString("dd/MM/yyyy"));
                     tapunts->setText(row+1,COL_CCOSTE, tapunts->text(row,COL_CCOSTE));
                     tapunts->setText(row+1,COL_CANAL, tapunts->text(row,COL_CANAL));
                     tapunts->setText(row+1,COL_IDCCOSTE, tapunts->text(row,COL_IDCCOSTE));
                     tapunts->setText(row+1,COL_IDCANAL, tapunts->text(row,COL_IDCANAL));
                     tapunts->setCurrentCell(row+1, COL_FECHA);                  
               } else {
                     tapunts->setCurrentCell(row,COL_CANAL);
               }// end if
               break;
            case COL_CANAL:
                     tapunts->setText(row+1,COL_FECHA,normalizafecha(tapunts->text(row,COL_FECHA)).toString("dd/MM/yyyy"));
                     tapunts->setText(row+1,COL_CCOSTE, tapunts->text(row,COL_CCOSTE));
                     tapunts->setText(row+1,COL_CANAL, tapunts->text(row,COL_CANAL));
                     tapunts->setText(row+1,COL_IDCCOSTE, tapunts->text(row,COL_IDCCOSTE));
                     tapunts->setText(row+1,COL_IDCANAL, tapunts->text(row,COL_IDCANAL));
                     tapunts->setCurrentCell(row+1, COL_FECHA);               
               break;
            case COL_CONCEPTO:
                tapunts->setCurrentCell(row, COL_IVA);
                break;
*/
            default:
               m_listPrevision->setCurrentCell(row, col+1);
            break;
         }// end switch
      break;
/*            
        case Qt::Key_Delete:  //Tecla Supr
            fprintf(stderr,"Se ha pulsado el Supr\n");
            borraborrador(row);
            break;
        case 4115: //Tecla Control + UP
            fprintf(stderr,"Se ha pulsado el ctrl + arriba\n");
            subirapunte(row);
            if (row > 0 )
                tapunts->setCurrentCell(row, col);
            break;
        case 4117: //Tecla Control + DOWN
            bajarapunte(row);
            tapunts->setCurrentCell(row, col);
            break;
*/
   }// end switch
}// end pulsadomas

