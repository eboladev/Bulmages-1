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

#include "ivaview.h"
#include "listcuentasview1.h"

#define IVA_SOPORTADO    1
#define IVA_REPERCUTIDO  2


ivaview::ivaview(QWidget *parent, const char *name ) : ivadlg(parent,name) {
  idborrador=0;
  idregistroiva =0;
}// end ivaview


ivaview::~ivaview(){
}// end ~ivaview


int ivaview::inicializa(postgresiface2 *conn, int sop_rep=0) {
  conexionbase = conn;
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
  fprintf(stderr,"Boton de Borrar Pulsado \n");
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
    //fprintf(stderr,"%s\n",query.ascii());
    conexionbase->begin();
    cursor2 * cursorapunte = conexionbase->cargacursor(query,"cursorapunte");
    conexionbase->commit();
    if (!cursorapunte->eof()) {
      //fprintf(stderr,"Asiento %s\n",cursorapunte->valor("idasiento").ascii());
      numasiento->setText(cursorapunte->valor("ordenasiento")); 
      cuentaiva->setText(cursorapunte->valor("codigo"));
    }// end if
    if (cuentaiva->text().left(3) == "600") {
      cuentaiva->setText("472");
      iva->setText("0");
      importeiva->setText("0.00");
    }
    if (cuentaiva->text().left(3) == "700") {
      cuentaiva->setText("477");
      iva->setText("0");
      importeiva->setText("0.00");
    }
    
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
                 }
              }
          }
          delete recordset;//Fin proposicion numeros factura y orden.
//* END JOSEP BURCION 
        }// end if
        delete cursorcontrapartida;
      }// end if
      delete cursorapunte;
  }// end if
  delete cursoriva;
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
