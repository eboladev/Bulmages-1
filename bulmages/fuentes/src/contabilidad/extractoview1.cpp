/***************************************************************************
                          extractoview11.cpp  -  description
                             -------------------
    begin                : Thu Jan 9 2003
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
#include "extractoview1.h"
#include "extractoprintview.h"
#include "libromayorprint.h"
#include "calendario.h"
#include "empresa.h"


#define PUNTEO          0
#define FECHA           1
#define ORDEN_ASIENTO   2
#define CONCEPTO        3
#define CONTRAPARTIDA   4
#define DEBE            5
#define HABER           6
#define SALDO           7
#define C_COSTE         8
#define CANAL           9
#define IDAPUNTE       10
#define NUM_ASIENTO     11


extractoview1::extractoview1(empresa * emp,QWidget *parent, const char *name, int flags ) : extractodlg1(parent,name,flags) {
   fprintf(stderr,"Constructor de extractoview1\n");
   empresaactual = emp;
   conexionbase = empresaactual->bdempresa();
   numdigitos = empresaactual->numdigitosempresa();
  
   
   delete listado1;
   listado = new QTable1(this,"listado");
   listado->setNumRows( 0 );
   listado->setNumCols( 0 );
   listado->setSelectionMode( QTable::SingleRow );
   listado->setSorting( TRUE );
   listado->setSelectionMode( QTable::SingleRow );
   extractodlg1Layout->addWidget( listado, 2, 0 );
   listado->setColumnMovingEnabled( TRUE );  
   listado->setNumCols(12);


   listado->horizontalHeader()->setLabel(PUNTEO, tr("P"));
   listado->horizontalHeader()->setLabel( FECHA, tr( "Fecha" ) );
   listado->horizontalHeader()->setLabel( NUM_ASIENTO, tr( "N.Asiento." ) );
   listado->horizontalHeader()->setLabel( CONCEPTO, tr( "Concepto" ) );
   listado->horizontalHeader()->setLabel( DEBE, tr( "Debe" ) );
   listado->horizontalHeader()->setLabel( HABER, tr( "Haber" ) );
   listado->horizontalHeader()->setLabel( SALDO, tr( "Saldo" ) );
   listado->horizontalHeader()->setLabel( C_COSTE, tr( "C. Coste" ) );
   listado->horizontalHeader()->setLabel( CANAL, tr( "Canal" ) );
   listado->horizontalHeader()->setLabel( CONTRAPARTIDA, tr("Contrapartida"));
   listado->horizontalHeader()->setLabel( ORDEN_ASIENTO, tr("Orden Asiento") );

   listado->setColumnWidth(PUNTEO,25);
   listado->setColumnWidth(FECHA,75);
   listado->setColumnWidth(NUM_ASIENTO,75);
   listado->setColumnWidth(ORDEN_ASIENTO, 50);
   listado->setColumnWidth(CONCEPTO,300);
   listado->setColumnWidth(DEBE,75);
   listado->setColumnWidth(HABER,75);
   listado->setColumnWidth(SALDO,100);
   listado->setColumnWidth(C_COSTE,75);
   listado->setColumnWidth(CANAL,75);
   listado->setColumnWidth(CONTRAPARTIDA,125);

   listado->hideColumn(IDAPUNTE);
   listado->hideColumn(NUM_ASIENTO);
    // Dependiendo de la configuración mostramos o no mostramos la contrapartida, los canales y los centros de coste
    if (confpr->valor(CONF_CONTRAPARTIDA_EXTRACTO) != "YES")
       listado->hideColumn(CONTRAPARTIDA);
    if (confpr->valor(CONF_CANAL_EXTRACTO) != "YES")
       listado->hideColumn(CANAL);
    if (confpr->valor(CONF_CCOSTE_EXTRACTO) != "YES")
       listado->hideColumn(C_COSTE);



   // Hacemos editables o no los campos segun nos convenga. El unico que se edita de momento es el campo de punteo.
   listado->setColumnReadOnly(PUNTEO,FALSE);
   listado->setColumnReadOnly(FECHA,TRUE);
   listado->setColumnReadOnly(NUM_ASIENTO,TRUE);
   listado->setColumnReadOnly(CONCEPTO,TRUE);
   listado->setColumnReadOnly(DEBE,TRUE);
   listado->setColumnReadOnly(HABER,TRUE);
   listado->setColumnReadOnly(SALDO,TRUE);
   listado->setColumnReadOnly(C_COSTE,TRUE);
   listado->setColumnReadOnly(CANAL,TRUE);
   listado->setColumnReadOnly(CONTRAPARTIDA, TRUE);

   
    // Establecemos el color de fondo del extracto. El valor lo tiene la clase configuracion que es global.
    listado->setPaletteBackgroundColor(confpr->valor(CONF_BG_EXTRACTO).c_str());
   
//   listado->setReadOnly(FALSE);

   // Hacemos la conexion de señales, para la petición de menu contextual y para el punteo.
   connect( listado, SIGNAL( contextMenuRequested(int,int,const QPoint&) ), this, SLOT( contextmenu(int,int,const QPoint&) ) );
   connect( listado, SIGNAL( valueChanged(int,int) ), this, SLOT( apuntecambiadogrid(int,int) ) );

   
   // Iniciamos los componentes de la fecha para que al principio aparezcan
   // Como el año inicial.
   QString cadena;
   cadena.sprintf("%2.2d/%2.2d/%4.4d",1, 1, QDate::currentDate().year());
   fechainicial1->setText(cadena);
   cadena.sprintf("%2.2d/%2.2d/%4.4d",31, 12, QDate::currentDate().year());
   fechafinal1->setText(cadena);
   cursorcta=NULL;

   // Inicializamos la ventana de filtrado
   filt = new filtrarextractosview(empresaactual,0,0);
   fprintf(stderr,"fin del constructor de extractoview1\n");
}// end estractoview1



extractoview1::~extractoview1(){
  delete cursorcta;
  delete filt;
}// end ~extractoview1


void extractoview1::inicializa2(intapunts3view *inta, diarioview1 *diar, balanceview *balan) {
  introapunts = inta;
  diario = diar;
  balance = balan;
}// end inicializa2


void extractoview1::cargacostes() {
   // Hacemos la carga de los centros de coste. Rellenamos el combobox correspondiente.
   filt->cargacostes();
}// end cargacostes


/************************************************************************************
Esta función carga el cursor de cuentas que forman el todo por el todo.
También sera la encargada de recoger la información de filtración para que
todo sea correcto.
*************************************************************************************/
void extractoview1::accept() {
  QString codinicial= codigoinicial->text();
  QString codfinal = codigofinal->text();
  QString query;
  int idc_coste = 0;
 
 // Como el filtrado de centros de coste ya no se hace asi, esta linea
 // Va a cambiar.  
//  idc_coste = filt->ccostes[filt->combocoste->currentItem()];
  
  if (idc_coste != 0) {
    query.sprintf("SELECT * FROM cuenta WHERE idcuenta IN (SELECT idcuenta FROM apunte WHERE idc_coste=%d) AND codigo >='%s' AND codigo <= '%s' ORDER BY codigo",idc_coste,codinicial.ascii(), codfinal.ascii());
  } else {
    query.sprintf("SELECT * FROM cuenta WHERE idcuenta IN (SELECT idcuenta FROM apunte) AND codigo >='%s' AND codigo <= '%s' ORDER BY codigo",codinicial.ascii(), codfinal.ascii());
  }// end if
  if (cursorcta != NULL) {
    delete cursorcta;
  }// end if
  conexionbase->begin();
  cursorcta = conexionbase->cargacursor(query,"cursorcuenta");
  conexionbase->commit();
  presentar();
}// end extractoview1


void extractoview1::boton_asiento() {
  if (listado->currentRow()!= -1) {
    QString text = listado->text(listado->currentRow(),NUM_ASIENTO);
    int numasiento = atoi((char *)text.ascii());
    if (numasiento != 0) {
      introapunts->muestraasiento(numasiento);
    }// end if
  }// end if
  introapunts->show();
  introapunts->setFocus();
}// end if


// Si el parametro pasado es un:
// 0 -> del dia actual
// 1 -> del mes actual
// 2 -> del año actual
void extractoview1::boton_diario1(int tipo) {
    QDate fecha1, fecha2, fechaact, fechaact1;
    if(!fechainicial1->text().isEmpty()) {
        fechaact = normalizafecha(fechainicial1->text());
        fechaact1 = normalizafecha(fechafinal1->text());
        switch(tipo) {
            case 0:
                fecha1.setYMD(fechaact.year(), fechaact.month(),fechaact.day());
                fecha2.setYMD(fechaact1.year(), fechaact1.month(), fechaact1.day());
            break;
            case 1:
                fecha1.setYMD(fechaact.year(), fechaact.month(),1);
                fecha2.setYMD(fechaact.year(), fechaact.month(), fechaact.daysInMonth());
            break;
            case 2:
                fecha1.setYMD(fechaact.year(), 1,1);
                fecha2.setYMD(fechaact.year(), 12, 31);
            break;
        }// end switch
    diario->inicializa1((char *) fecha1.toString("dd/MM/yyyy").ascii(),(char *) fecha2.toString("dd/MM/yyyy").ascii(), 0);
    }// end if
   diario->accept();
   diario->show();
   diario->setFocus();
}// end boton_diario1


// Si el parametro pasado es un:
// 0 -> del periodo actual.
// 1 -> del mes actual mirado a partir de la fecha de inicio.
// 2 -> del año actual mirado a partir de la fecha de inicio.
void extractoview1::boton_balance1(int tipo) {
    QDate fecha1, fecha2, fechaact, fechaact1;
    if(!fechainicial1->text().isEmpty()) {
        fechaact = normalizafecha(fechainicial1->text());
        fechaact1 = normalizafecha(fechafinal1->text());
        switch(tipo) {
            case 0:
                fecha1.setYMD(fechaact.year(), fechaact.month(),fechaact.day());
                fecha2.setYMD(fechaact1.year(), fechaact1.month(), fechaact1.day());
            break;
            case 1:
                fecha1.setYMD(fechaact.year(), fechaact.month(),1);
                fecha2.setYMD(fechaact.year(), fechaact.month(), fechaact.daysInMonth());
            break;
            case 2:
                fecha1.setYMD(fechaact.year(), 1,1);
                fecha2.setYMD(fechaact.year(), 12, 31);
            break;
        }// end switch
        balance->inicializa1(codigoinicial->text(),codigofinal->text(), fecha1.toString("dd/MM/yyyy"), fecha2.toString("dd/MM/yyyy"), 0);
    }// end if
    balance->accept();
    balance->show();
    balance->setFocus();
}// end boton_balance1


/***************************************************************
 * Esta es la funcion que avanza un registro entre las cuentas *
 ***************************************************************/
void extractoview1::boton_siguiente() {
  if (!cursorcta->esultimoregistro()) {
     cursorcta->siguienteregistro();
     presentar();
  }// end if
}// end boton_siguiente


/******************************************************************
 * Esta es la funcion que retrocede un registro entre las cuentas *
 ******************************************************************/
void extractoview1::boton_anterior() {
   if (!cursorcta->esprimerregistro()) {
     cursorcta->registroanterior();
     presentar();
   }// end if
}// end boton_anterior


void extractoview1::boton_inicio() {
   cursorcta->primerregistro();
   presentar();
}// end if


void extractoview1::boton_fin() {
   cursorcta->ultimoregistro();
   presentar();
}// end if


void extractoview1::boton_imprimir() {
   ExtractoPrintView *print = new ExtractoPrintView(0,0);
   print->inicializa(conexionbase);
	print->inicializa1(fechainicial1->text(), fechafinal1->text(), codigoinicial->text(), codigofinal->text());
   print->exec();
}// end if


void extractoview1::boton_guardar() {
  QString fn = QFileDialog::getSaveFileName(0, tr("Diarios (*.txt)"), 0,tr("Guardar Libro Diario"),tr("Elige el nombre de archivo"));
  if (!fn.isEmpty()) {
     libromayorprint libromayor;
     QString finicial = fechainicial1->text().ascii();
     QString ffinal = fechafinal1->text().ascii();
     libromayor.inicializa(conexionbase);
     libromayor.inicializa1(codigoinicial->text(), codigofinal->text(), finicial, ffinal);
     libromayor.inicializa2((char *) fn.ascii());
     libromayor.accept();
  }// end if
}// end boton_guardar


void extractoview1::vaciar() {
   inicialdebe->setText("0");
   inicialhaber->setText("0");
   inicialsaldo->setText("0");
   totaldebe->setText("0");
   totalhaber->setText("0");
   totalsaldo->setText("0");
   listado->setNumRows(0);
}// end if


void extractoview1::ajustes() {
}// end ajustes


void extractoview1::presentar() {
   fprintf(stderr,"Presentar\n");
   int j,num1;
   float debe, haber,saldo;
   float debeinicial=0, haberinicial=0, saldoinicial=0;
   float debefinal, haberfinal, saldofinal;
   int idcuenta;
   int idasiento;
   QString finicial = fechainicial1->text();
   QString ffinal = fechafinal1->text();
   QString contra = filt->codigocontrapartida->text();

   // Preparamos el string para que aparezca una u otra cosa segun el punteo.
   QString tipopunteo;
	tipopunteo="";
   if (filt->punteotodos->isChecked()) {
      tipopunteo = "";
   } else if (filt->punteopunteado->isChecked()) {
      tipopunteo = " AND punteo=TRUE ";
   } else {
      tipopunteo = " AND punteo=FALSE ";
   }// end if

   if (contra != "") {
   	tipopunteo += " AND apunte.contrapartida = id_cuenta('"+contra+"') ";
   }// end if
   
   QString cad;
   QString cadaux;
	int idc_coste = 0;
   cursor2 *cursorapt;
   cursor2 *cursorcoste;
   cursor2 *cursorcanal;
   cursor2 *cursoraux1;

   // Vaciamos la ventana del extracto para que no haya pisamientos.
   vaciar();


   fprintf(stderr,"Vamos a presentar\n");
   if (!cursorcta->eof() && !cursorcta->bof()) {
      idcuenta = atoi(cursorcta->valor("idcuenta").ascii());
      // Escribimos el nombre de la cuenta y el código de la misma.
      codigocuenta->setText(cursorcta->valor("codigo"));
      nombrecuenta->setText(cursorcta->valor("descripcion"));
      // Hacemos la consulta de los apuntes a listar en la base de datos.
      QString query="";
      // Al igual que en el caso anterior los centros de coste han cambiado y aun no se pueden implementar.
      //idc_coste = filt->ccostes[filt->combocoste->currentItem()];    
      
      if (idc_coste != 0) {
        query.sprintf("SELECT * FROM apunte, asiento where asiento.idasiento = apunte.idasiento AND  idcuenta=%d AND apunte.fecha>='%s' AND apunte.fecha<='%s' AND idc_coste=%d  %s ORDER BY apunte.fecha, ordenasiento, orden",idcuenta, (char *) finicial.ascii(),(char *) ffinal.ascii(), idc_coste, tipopunteo.ascii());
      } else {
        query.sprintf("SELECT * FROM apunte, asiento where asiento.idasiento = apunte.idasiento AND idcuenta=%d AND apunte.fecha>='%s' AND  apunte.fecha<='%s'  %s ORDER BY apunte.fecha, ordenasiento, orden",idcuenta,(char *) finicial.ascii(),(char *) ffinal.ascii(), tipopunteo.ascii());
      }// end if
      fprintf(stderr,"%s\n",query.ascii());
      conexionbase->begin();
      cursorapt=conexionbase->cargacursor(query,"cargasaldoscuentafecha");
      conexionbase->commit();
      num1 = cursorapt->numregistros();      
      if (!cursorapt->eof()) {
         fprintf(stderr,"Existen apuntes por tanto pintamos algo\n");
//         activo = strcmp((char *) cursorcta->valor(13).c_str() , "f");

         // Cargamos los saldos iniciales.
         cursor2* cursoraux;
         conexionbase->begin();
         query.sprintf("SELECT sum(debe) AS tdebe, sum(haber) AS thaber FROM apunte WHERE idcuenta=%d AND fecha <'%s'",idcuenta, finicial.ascii());
         cursoraux = conexionbase->cargacursor(query, "saldos_iniciales");
         conexionbase->commit();
         if (!cursoraux->eof()) {
            debeinicial = atof(cursoraux->valor("tdebe").ascii());
            haberinicial = atof(cursoraux->valor("thaber").ascii());
               saldoinicial = debeinicial - haberinicial;
         }// end if
         delete cursoraux;
         
         cadaux.sprintf("%5.2f",debeinicial);
         inicialdebe->setText(cadaux);
         cadaux.sprintf("%5.2f",haberinicial);
         inicialhaber->setText(cadaux);
         cadaux.sprintf("%5.2f",saldoinicial);
         inicialsaldo->setText(cadaux);
         saldo = saldoinicial;
         debefinal = debeinicial;
         haberfinal = haberinicial;
         listado->setNumRows(num1);
         j=0;
         while (!cursorapt->eof()) {
            fprintf(stderr,"Iteramos para cada apunte de la cuenta \n");
            listado->setText(j,IDAPUNTE, cursorapt->valor("idapunte"));
            idasiento=atoi(cursorapt->valor("idasiento").ascii());
            int ordenasiento = atoi(cursorapt->valor("ordenasiento").ascii());


            // Aqui vamos a poner el tema del Punteo, por lo menos la presentación.
            QCheckTableItem *check = new QCheckTableItem(listado,0);
            listado->setItem(j,PUNTEO,check);
            if (cursorapt->valor("punteo")== "t") {
               check->setChecked(TRUE);
            }// end if


            
            debe=atof(cursorapt->valor(8).ascii());
            haber=atof(cursorapt->valor(9).ascii());
            saldo += debe - haber;
            debefinal += debe;
            haberfinal += haber;
            cad = cursorapt->valor(4);
            // Sacamos el centro de coste
            QString query;
            int ccoste = atoi(cursorapt->valor("idc_coste").ascii());
            query.sprintf( "SELECT * FROM c_coste WHERE idc_coste=%d",ccoste);
            conexionbase->begin();
            cursorcoste=conexionbase->cargacursor(query,"ccoste");
            conexionbase->commit();
            if (!cursorcoste->eof()) {
              cadaux.sprintf("%s",cursorcoste->valor("nombre").ascii());
              listado->setText(j,C_COSTE,cadaux);
            }// end if
            delete cursorcoste;

            // Sacamos el canal
            int canal = atoi(cursorapt->valor("idcanal").ascii());
            query.sprintf( "SELECT * FROM canal WHERE idcanal=%d",canal);
            conexionbase->begin();
            cursorcanal=conexionbase->cargacursor(query,"canal");
            conexionbase->commit();
            if (!cursorcanal->eof()) {
              cadaux.sprintf("%s",cursorcanal->valor("nombre").ascii());
              listado->setText(j,CANAL,cadaux);
            }// end if
            delete cursorcanal;

            // Sacamos la contrapartida
            query.sprintf("SELECT codigo FROM cuenta WHERE idcuenta=%s",cursorapt->valor("contrapartida").ascii());
            fprintf(stderr,"%s\n",query.ascii());
            conexionbase->begin();
            cursoraux1=conexionbase->cargacursor(query,"contrapartida");
            conexionbase->commit();
            if (!cursoraux1->eof()) {
              listado->setText(j,CONTRAPARTIDA,cursoraux1->valor("codigo"));
            }// end if
            delete cursoraux1;


            
            // Esta parte es la que inserta en la tabla
            listado->setText(j,FECHA,cad.mid(0,10));
            cadaux.sprintf("%d",idasiento);
            listado->setText(j,NUM_ASIENTO,cadaux);
            cadaux.sprintf("%d",ordenasiento);
            listado->setText(j,ORDEN_ASIENTO,cadaux);
            
            listado->setText(j,CONCEPTO, cursorapt->valor(5));
            cadaux.sprintf("%5.2f",debe);
            listado->setText(j,DEBE,cadaux);
            cadaux.sprintf("%5.2f",haber);
            listado->setText(j,HABER,cadaux);
            cadaux.sprintf("%5.2f",saldo);
            listado->setText(j,SALDO,cadaux);
            cursorapt->siguienteregistro();
            j++;
         }// end while
         saldofinal = debefinal - haberfinal;
         cadaux.sprintf("%5.2f",debefinal);
         totaldebe->setText(cadaux);
         cadaux.sprintf("%5.2f",haberfinal);
         totalhaber->setText(cadaux);
         cadaux.sprintf("%5.2f",saldofinal);
         totalsaldo->setText(cadaux);
      }// end if
      delete cursorapt;
   }// end if
   ajustes();
}// end presentar



void extractoview1::inicializa1(QString codinicial, QString codfinal, QString fecha1, QString fecha2, int idc_coste) {
   codigoinicial->setText(codinicial);
   codigofinal->setText(codfinal);
   fechainicial1->setText(normalizafecha(fecha1).toString("dd/MM/yyyy"));
   fechafinal1->setText(normalizafecha(fecha2).toString("dd/MM/yyyy"));
   filt->setccoste(idc_coste);
}// end inicializa1


void extractoview1::boton_buscacuentainicial() {
   listcuentasview1 *listcuentas = new listcuentasview1();
   listcuentas->modo=1;
   listcuentas->inicializa(conexionbase);
   listcuentas->exec();
   codigoinicial->setText(listcuentas->codcuenta);
   delete listcuentas;
}// end boton_buscacuentainicial


void extractoview1::boton_buscacuentafinal() {
   listcuentasview1 *listcuentas = new listcuentasview1();
   listcuentas->modo=1;
   listcuentas->inicializa(conexionbase);
   listcuentas->exec();
   codigofinal->setText(listcuentas->codcuenta);
   delete listcuentas;
}// end boton_buscacuentafinal


void extractoview1::return_codigoinicial() {
   QString cad = codigoinicial->text();
   if (cad != "") {
      cad = extiendecodigo(cad,numdigitos);
      conexionbase->begin();
      cursor2 *curs = conexionbase->cargacuenta(0, cad );
      conexionbase->commit();
      int num = curs->numregistros();
      if (num >0) {
         codigoinicial->setText(cursorcta->valor(1));
         codigofinal->setText(cursorcta->valor(1));
         codigofinal->selectAll();
         accept();
      } else {
        codigoinicial->selectAll();
        codigoinicial->setFocus();
      }// end if
      delete curs;
   }// end if
}// end return_codigoinicial


void extractoview1::return_codigofinal() {
   QString cad = codigofinal->text();
   if (cad != "") {
      cad = extiendecodigo(cad,numdigitos);
      conexionbase->begin();
      cursor2 *cursorcta = conexionbase->cargacuenta(0, cad );
      conexionbase->commit();
      int num = cursorcta->numregistros();
      if (num >0) {
         codigofinal->setText(cursorcta->valor(1));
      } else {
        codigofinal->selectAll();
        codigofinal->setFocus();
      }// end if
      delete cursorcta;
   }// end if
}// end return_codigofinal


void extractoview1::return_fechainicial() {
  fechainicial1->setText(normalizafecha(fechainicial1->text()).toString("dd/MM/yyyy"));
}// end return_fechainicial



void extractoview1::return_fechafinal() {
  fechafinal1->setText(normalizafecha(fechafinal1->text()).toString("dd/MM/yyyy"));
  accept();
}// end return_fechafinal


void extractoview1::contextmenu(int row, int col, const QPoint &poin) {
        // Si el asiento esta cerrado el menu a mostrar es diferente
        QPopupMenu *popup = new QPopupMenu;
		  popup->insertItem(tr("Ver Asiento"), 0);
		  popup->insertSeparator();
        popup->insertItem(tr("Ver Diario (Este dia)"),101);
//       popup->insertItem("Ver Diario (Esta semana)",102);
        popup->insertItem(tr("Ver Diario (Este mes)"),103);
        popup->insertItem(tr("Ver Diario (Este año)"),104);
		  popup->insertSeparator();
        popup->insertItem(tr("Ver Balance (Este dia)"),121);
        popup->insertItem(tr("Ver Balance (Este mes)"),123);
        popup->insertItem(tr("Ver Balance (Este año)"),124);
        int opcion = popup->exec(poin);
		  switch(opcion) {
		  		case 0:
					boton_asiento();
				break;
		  		case 101:
					boton_diario1(0);
				break;
				case 103:
					boton_diario1(1);
				break;
				case 104:
					boton_diario1(2);
				break;
				case 121:
					boton_balance1(0);
				break;
				case 123:
					boton_balance1(1);
				break;
				case 124:
					boton_balance1(2);
		  }// end switch
        delete popup;
        // Para evitar los warnings establecemos las variables a 0
        row=col=0;
}// end contextmenu


void extractoview1::apuntecambiadogrid(int row,int col) {
   fprintf(stderr,"Se ha cambiado algo en el extracto \n");
   QCheckTableItem *check =(QCheckTableItem *) listado->item(row,col);
   QString query;
   if (check->isChecked()) {
       query.sprintf("UPDATE APUNTE SET punteo = TRUE WHERE idapunte=%s",listado->text(row,IDAPUNTE).ascii());
   } else {
       query.sprintf("UPDATE APUNTE SET punteo = FALSE WHERE idapunte=%s",listado->text(row,IDAPUNTE).ascii());
   }// end if
   conexionbase->begin();
   conexionbase->ejecuta(query.ascii());
   conexionbase->commit();
}// end if

void extractoview1::boton_filtrar() {
   filt->exec();
   accept();
}// end boton_filtrar

void extractoview1::boton_casacion() {
   QString query;
   query.sprintf("SELECT * FROM apunte WHERE punteo=FALSE AND haber <>0  AND idcuenta=%s ORDER BY fecha",cursorcta->valor("idcuenta").ascii());
   conexionbase->begin();
   cursor2 *curshaber = conexionbase->cargacursor(query, "curshaber");
   conexionbase->commit();
   while (!curshaber->eof()) {
      query.sprintf("SELECT * FROM apunte WHERE punteo=FALSE AND debe = %s AND idcuenta = %s ORDER BY fecha", curshaber->valor("haber").ascii(), cursorcta->valor("idcuenta").ascii());
      conexionbase->begin();
      cursor2 *cursdebe = conexionbase->cargacursor(query.ascii(), "cursdebe");
      conexionbase->commit();
      if (!cursdebe->eof()) {
         query.sprintf("UPDATE apunte set punteo=TRUE WHERE idapunte=%s",curshaber->valor("idapunte").ascii());
         conexionbase->begin();
         conexionbase->ejecuta(query);
         query.sprintf("UPDATE apunte SET punteo=TRUE WHERE idapunte=%s",cursdebe->valor("idapunte").ascii());
         conexionbase->ejecuta(query);
         conexionbase->commit();
      }// end if
      delete cursdebe;
      curshaber->siguienteregistro();
   }// end while
   delete curshaber;
   presentar();
}// end boton_casacion

void extractoview1::boton_guardarpunteo() {
  QString fn = QFileDialog::getSaveFileName(0, tr("Punteos (*.pto)"), 0,tr("Guardar Punteo"),tr("Elige el nombre de archivo"));
  if (!fn.isEmpty()) {
    FILE *mifile;
    mifile = fopen((char *) fn.ascii(),"wt");
    if (mifile != NULL) {
     
        QString query;
        query = "SELECT * FROM apunte WHERE punteo=TRUE";
        conexionbase->begin();
        cursor2 *cursp = conexionbase->cargacursor(query,"punteos");
        conexionbase->commit();
        while (!cursp->eof()) {
           fprintf(mifile,"%s\n", cursp->valor("idapunte").ascii());
           cursp->siguienteregistro();
        }// end while
        delete cursp;
        fclose(mifile);
    }// end if
  }// end if   
}// end boton_guardarpunteo

// Esta función se activa justo cuando se pulsa sobre el botón de resetear el punteo.
// Por supuesto cuando se pulsa dicho boton se borra el punteo.
void extractoview1::boton_borrapunteo() {
   int valor = QMessageBox::warning( 0, tr("Borrar Punteo"), "Se dispone a borrar el punteo. Este cambio es irrecuperable si no ha guardado su el punte. ¿Desea continuar?", QMessageBox::Yes, QMessageBox::No);
   if (valor == QMessageBox::Yes) {
      conexionbase->begin();
      conexionbase->ejecuta("UPDATE apunte SET punteo=FALSE");
      conexionbase->commit();
      presentar();
   }// end if
}// end boton_cargarpunteo



void extractoview1::boton_cargarpunteos() {
   QString fn = QFileDialog::getOpenFileName(0, tr("Punteos (*.pto)"), 0,tr("Cargar Punteo"),tr("Elige el nombre de archivo"));
   if (!fn.isEmpty()) {
      ifstream filestr((char *) fn.ascii());
      string a;
      conexionbase->begin();
      conexionbase->ejecuta("UPDATE apunte SET punteo=FALSE");
      conexionbase->commit();
      while (filestr.good()) {
         filestr >> a;
         QString query;
         query.sprintf("UPDATE apunte SET punteo=TRUE WHERE idapunte=%s",a.c_str());
         conexionbase->begin();
         conexionbase->ejecuta(query);
         conexionbase->commit();
      }// end while
      filestr.close();
   }// end if
   presentar();
}// end boton_cargarpunteo


void extractoview1::codigo_textChanged(const QString &texto) {
    QLineEdit *codigo = (QLineEdit *) sender();
    if (texto == "+") {
        // Hacemos aparecer la ventana de cuentas
        listcuentasview1 *listcuentas = new listcuentasview1();
        listcuentas->modo=1;
        listcuentas->inicializa(conexionbase);
        listcuentas->exec();
        codigo->setText(listcuentas->codcuenta);
        delete listcuentas;
    }// end if
}// end codigo_textChanged


// Cuando se ha pulsado una tecla sobre la fecha del extracto
// Se evalua si la pulsación es un código de control o es un digitos
// Para la introducción de fechas.
void extractoview1::fecha_textChanged(const QString &texto) {
	QLineEdit *fecha = (QLineEdit *) sender();
    if (texto=="+") {
        QList<QDate> a;
        fecha->setText("");
        calendario *cal = new calendario(0,0);
        cal->exec();
        a = cal->dn->selectedDates();
        fecha->setText(a.first()->toString("dd/MM/yyyy"));
        delete cal;
    }// end if
    if (texto=="*")
        fecha->setText(QDate::currentDate().toString("dd/MM/yyyy") );
}// end fecha_textChanged

// cuando pulsamos el boton de la fecha
// es como si escribiesemos un + en el texto
void extractoview1::boton_fechainicial() {
	fechainicial1->setText("+");
   fechainicial1->selectAll();
   fechainicial1->setFocus();
}// end boton_fechainicial

// cuando pulsamos el boton de la fecha
// es como si escribiesemos un + en el texto
void extractoview1::boton_fechafinal() {
	fechafinal1->setText("+");
   fechafinal1->selectAll();
   fechafinal1->setFocus();
}// end boton_fechainicial


