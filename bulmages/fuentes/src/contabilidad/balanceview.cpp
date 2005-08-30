/***************************************************************************
                          balanceview.cpp  -  description
                             -------------------
    begin                : sáb abr 26 2003
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

#include "balanceview.h"
#include "empresa.h"
#include "calendario.h"

#include <qwidget.h>
#include <qlineedit.h>
#include <qdatetimeedit.h>
#include <qfiledialog.h>
#include <qtoolbutton.h>

#include "balanceprintview.h"


// Incluimos las imagenes que catalogan los tipos de cuentas.
#include "images/cactivo.xpm"
#include "images/cpasivo.xpm"
#include "images/cneto.xpm"
#include "images/cingresos.xpm"
#include "images/cgastos.xpm"

#include <qstring.h>

#include "qtable1.h"
#include "busquedafecha.h"
#include "busquedacuenta.h"

#define CUENTA           0
#define DENOMINACION     1
#define SALDO_ANT        2
#define DEBE            3
#define HABER           4
#define SALDO           5
#define DEBEEJ		6
#define HABEREJ		7
#define SALDOEJ		8



/** \brief Constructor de clasee
  * \bug No es necesario borrar la tabla de designer para que esto funcione.
  * En el constructor  inicializa la empresa y base de datos.
  * También se configura correctamente el objeto m_listado (que reemplaza al que está en el formulario
  * Se prepara el combobox de niveles a mostrar y se ponen las fechas de balance.
  */
balanceview::balanceview(empresa *emp, QWidget *parent, const char *name, int  ) : balancedlg(parent,name) {
   empresaactual = emp;
   conexionbase = empresaactual->bdempresa();
   numdigitos = empresaactual->numdigitosempresa();
	m_codigoinicial->setempresa(emp);
	m_codigofinal->setempresa(emp);


    QFont tapunts_font(  m_listado->font() );
    tapunts_font.setPointSize(atoi(confpr->valor(CONF_FONTSIZE_APUNTES).ascii()));
    tapunts_font.setFamily(confpr->valor(CONF_FONTFAMILY_APUNTES).ascii());
    m_listado->setFont( tapunts_font );

   fprintf(stderr,"BALANCEVIEW CONSTRUCTOR\n");
   m_listado->setNumRows( 0 );
   fprintf(stderr,"setNumCols\n");
   m_listado->setNumCols( 0 );
   m_listado->setSelectionMode( QTable::SingleRow );
   m_listado->setSorting( TRUE );
   m_listado->setSelectionMode( QTable::SingleRow );
   m_listado->setColumnMovingEnabled( TRUE );
   m_listado->setNumCols(9);
   m_listado->horizontalHeader()->setLabel( CUENTA, tr( "Cuenta" ) );
   m_listado->horizontalHeader()->setLabel( DENOMINACION, tr( "Denominacion" ) );
   m_listado->horizontalHeader()->setLabel( SALDO_ANT, tr( "Saldo Ant." ) );
   m_listado->horizontalHeader()->setLabel( DEBE, tr( "Debe Periodo" ) );
   m_listado->horizontalHeader()->setLabel( HABER, tr( "Haber Periodo" ) );
   m_listado->horizontalHeader()->setLabel( SALDO, tr( "Saldo Periodo" ) );
   m_listado->horizontalHeader()->setLabel( DEBEEJ, tr("Debe Ejercicio") );
   m_listado->horizontalHeader()->setLabel( HABEREJ, tr("Haber Ejercicio") );
   m_listado->horizontalHeader()->setLabel( SALDOEJ, tr("Saldo Ejercicio") );

   m_listado->setColumnWidth(CUENTA,75);
   m_listado->setColumnWidth(DENOMINACION,300);
   m_listado->setColumnWidth(SALDO_ANT,75);
   m_listado->setColumnWidth(DEBE,75);
   m_listado->setColumnWidth(HABER,75);
   m_listado->setColumnWidth(SALDO,100);

   // Establecemos el color de fondo del extracto. El valor lo tiene la clase configuracion que es global.
   m_listado->setPaletteBackgroundColor(confpr->valor(CONF_BG_BALANCE).ascii());
   m_listado->setReadOnly(TRUE);

   // Inicializamos la tabla de nivel
   combonivel->insertItem("2",0);
   combonivel->insertItem("3",1);
   combonivel->insertItem("4",2);
   combonivel->insertItem("5",3);
   combonivel->insertItem("6",4);
   combonivel->insertItem("7",5);

   connect( m_listado, SIGNAL( contextMenuRequested(int,int,const QPoint&) ), this, SLOT( contextmenu(int,int,const QPoint&) ) );

   // Iniciamos los componentes de la fecha para que al principio aparezcan
   // Como el año inicial.
   QString cadena;
   cadena.sprintf("%2.2d/%2.2d/%4.4d",1, 1, QDate::currentDate().year());
   m_fechainicial1->setText(cadena);
   cadena.sprintf("%2.2d/%2.2d/%4.4d",31, 12, QDate::currentDate().year());
   m_fechafinal1->setText(cadena);
   // Hacemos la carga de los centros de coste. Rellenamos el combobox correspondiente.
   cargacostes();
}// end balanceview

balanceview::~balanceview(){
}




void balanceview::inicializa2(intapunts3view *inta, diarioview1 *diar, extractoview1 *extract) {
  introapunts = inta;
  diario = diar;
  extracto = extract;
}// end inicializa2


/** \brief Esta función se encarga de cargar el combo box de centros de coste para que se pueda seleccionar uno
  * \bug Debe eliminarse el array ccoste.
  */
void balanceview::cargacostes() {
   // Hacemos la carga de los centros de coste. Rellenamos el combobox correspondiente.
   combocoste->clear();
   QString query="SELECT * FROM c_coste ORDER BY nombre";
   conexionbase->begin();
   cursor2 *cursorcoste = conexionbase->cargacursor(query,"costes");
   conexionbase->commit();
   combocoste->insertItem("--",0);
   ccostes[0]=0;
   int i=1;
   while (!cursorcoste->eof()) {
      combocoste->insertItem(cursorcoste->valor(2),-1);
      ccostes[i++] = atoi(cursorcoste->valor(0));
      cursorcoste->siguienteregistro();
   }// end while
   delete cursorcoste;
}// end cargacostes



/**
  *Esta función inicializa la ventana de extracto con los mismos datos
  *que la ventana de balance, cuentas, fechas y centros de coste
  *y pone la ventan de estracto como la ventana principal.
  *De esta forma cuando se pulsa sobre el boton extracto estando en la de balance
  *se cambia a la ventana de extracto de la forma correcta.
  * Si el parametro pasado es un:
  * 0 -> del día actual
  * 1 -> del mes actual
  * 2 -> del año actual
  */
void balanceview::boton_extracto1(int tipo) {
	QDate fecha1, fecha2, fechaact, fechaact1;
	if(!m_fechainicial1->text().isEmpty()) {
        fechaact = normalizafecha(m_fechainicial1->text());
        fechaact1 =normalizafecha(m_fechafinal1->text());
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
 		  extracto->inicializa1( m_listado->text(m_listado->currentRow(), CUENTA), m_listado->text(m_listado->currentRow(), CUENTA),fecha1.toString("dd/MM/yyyy"), fecha2.toString("dd/MM/yyyy"),  ccostes[combocoste->currentItem()]);
   }// end if
   extracto->accept();
   empresaactual->libromayor();
}// end boton_extracto1



/**
  * Si el parametro pasado es un:
  * 0 -> del día actual
  * 1 -> del mes actual
  * 2 -> del año actual
  */
void balanceview::boton_diario1(int tipo) {
	QDate fecha1, fecha2, fechaact, fechaact1;
	if(!m_fechainicial1->text().isEmpty()) {
        fechaact = normalizafecha(m_fechainicial1->text());
        fechaact1 =normalizafecha(m_fechafinal1->text());
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
   	diario->inicializa1(fecha1.toString("dd/MM/yyyy"), fecha2.toString("dd/MM/yyyy"), 0);
   }// end if
   diario->accept();
   empresaactual->librodiario();
}// end boton_diario1




void balanceview::boton_asiento() {
  empresaactual->muestraapuntes1();
}// end if


/** \brief Se encarga  de inicializar la clase con los parametros que se le han pasado.
  * Esta función sirve para que desde fuera se pueda preparar a la clase para presentar un m_listado predeterminado.
  */
void balanceview::inicializa1(QString codinicial, QString codfinal, QString fecha1, QString fecha2, int idc_coste) {

/********************** Josep *********************/
  fprintf(stderr,"balanceview::inicializa1\n");
  m_codigoinicial->setText(codinicial);
  m_codigofinal->setText(codfinal);
  m_fechainicial1->setText(normalizafecha(fecha1).toString("dd/MM/yyyy"));
  m_fechafinal1->setText(normalizafecha(fecha2).toString("dd/MM/yyyy"));
  // Establecemos el centro de coste correspondiente.
  int i=0;
  while (ccostes[i]!=idc_coste && i<100) i++;
  if (i<100) { combocoste->setCurrentItem(i); }

/**************************************************/
}// end inicializa1


/** \brief Se encarga de hacer la consulta en la base de datos y mostrar el resultado.
    \bug  Hay que eliminar el uso de double y usar un sistema de punto fijo.
  * Crea una tabla auxiliar de balance y hace en ella todos los calculos necesarios para concretar los resultados
  */
void balanceview::presentar() {
      int j,num1;
      double tsaldoant=0, tdebe=0, thaber=0, tsaldo=0;
      QString query;
      cursor2 *cursorapt;
      QString finicial = m_fechainicial1->text();
      QString ffinal = m_fechafinal1->text();
      QString cinicial = m_codigoinicial->codigocuenta();
      QString cfinal = m_codigofinal->codigocuenta();
      QString ejercicio = ffinal.right(4);

      // Hacemos la consulta de los apuntes a listar en la base de datos.
      int idc_coste;
      idc_coste = ccostes[combocoste->currentItem()];

      // La consulta es compleja, requiere la creación de una tabla temporal y de cierta mandanga por lo que puede
	// Causar problemas con el motor de base de datos.
	fprintf(stderr,"BALANCE: Empezamos a hacer la presentacion\n");
        conexionbase->begin();		
	query= "CREATE TEMPORARY TABLE balancetemp AS SELECT cuenta.idcuenta, codigo, nivel(codigo) AS nivel, cuenta.descripcion, padre, tipocuenta ,debe, haber, tdebe, thaber,(tdebe-thaber) AS tsaldo, (debe-haber) AS saldo, adebe, ahaber, (adebe-ahaber) AS asaldo, ejdebe, ejhaber, (ejdebe-ejhaber) AS ejsaldo FROM cuenta";
	query += " LEFT JOIN (SELECT idcuenta, sum(debe) AS tdebe, sum(haber) AS thaber FROM apunte WHERE fecha >= '"+finicial+"' AND fecha<= '"+ffinal+"' GROUP BY idcuenta) AS t1 ON t1.idcuenta = cuenta.idcuenta";
	query += " LEFT JOIN (SELECT idcuenta, sum(debe) AS adebe, sum(haber) AS ahaber FROM apunte WHERE fecha < '"+finicial+"' GROUP BY idcuenta) AS t2 ON t2.idcuenta = cuenta.idcuenta";
	query += " LEFT JOIN (SELECT idcuenta, sum(debe) AS ejdebe, sum(haber) AS ejhaber FROM apunte WHERE EXTRACT (YEAR FROM fecha) = '"+ejercicio+"' GROUP BY idcuenta) AS t3 ON t3.idcuenta = cuenta.idcuenta";

      conexionbase->ejecuta(query);
      query.sprintf("UPDATE balancetemp SET padre=0 WHERE padre ISNULL");
      conexionbase->ejecuta(query);
      query.sprintf("DELETE FROM balancetemp WHERE debe=0 AND haber =0");
      conexionbase->ejecuta(query);

      // Vamos a implementar el tema del código
      if (cinicial != "") {
         query.sprintf("DELETE FROM balancetemp WHERE codigo < '%s'",cinicial.ascii());
         conexionbase->ejecuta(query);
      }// end if
      if (cfinal != "") {
         query.sprintf("DELETE FROM balancetemp WHERE codigo > '%s'",cfinal.ascii());
         conexionbase->ejecuta(query);
      }// end if      

      // Para evitar problemas con los nulls hacemos algunos updates
      query.sprintf("UPDATE balancetemp SET tsaldo=0 WHERE tsaldo ISNULL");
      conexionbase->ejecuta(query);
      query.sprintf("UPDATE balancetemp SET tdebe=0 WHERE tdebe ISNULL");
      conexionbase->ejecuta(query);
      query.sprintf("UPDATE balancetemp SET thaber=0 WHERE thaber ISNULL");
      conexionbase->ejecuta(query);
      query.sprintf("UPDATE balancetemp SET asaldo=0 WHERE asaldo ISNULL");
      conexionbase->ejecuta(query);
      query.sprintf("UPDATE balancetemp SET ejsaldo=0 WHERE ejsaldo ISNULL");
      conexionbase->ejecuta(query);
      query.sprintf("UPDATE balancetemp SET ejdebe=0 WHERE ejdebe ISNULL");
      conexionbase->ejecuta(query);
      query.sprintf("UPDATE balancetemp SET ejhaber=0 WHERE ejhaber ISNULL");
      conexionbase->ejecuta(query);
      query.sprintf( "SELECT idcuenta FROM balancetemp ORDER BY padre DESC");
      cursorapt = conexionbase->cargacursor(query,"Balance1view");
      while (!cursorapt->eof())  {
         query.sprintf("SELECT * FROM balancetemp WHERE idcuenta=%s",cursorapt->valor("idcuenta").ascii());
         cursor2 *mycur = conexionbase->cargacursor(query,"cursorrefresco");
         if (!mycur->eof()) {
            query.sprintf("UPDATE balancetemp SET tsaldo = tsaldo + (%2.2f), tdebe = tdebe + (%2.2f), thaber = thaber +(%2.2f), asaldo= asaldo+(%2.2f), ejdebe = ejdebe + (%2.2f), ejhaber = ejhaber +(%2.2f), ejsaldo = ejsaldo + (%2.2f) WHERE idcuenta = %d",atof(mycur->valor("tsaldo").ascii()), atof(mycur->valor("tdebe").ascii()), atof(mycur->valor("thaber").ascii()),atof(mycur->valor("asaldo").ascii()),atof(mycur->valor("ejdebe").ascii()), atof(mycur->valor("ejhaber").ascii()), 
	    atof(mycur->valor("ejsaldo").ascii()),  atoi(mycur->valor("padre").ascii()));
            conexionbase->ejecuta(query);
         }// end if
         delete mycur;
         cursorapt->siguienteregistro();
      }// end while
      delete cursorapt;

      conexionbase->commit();
      conexionbase->begin();
      // Borramos todo lo que no es de este nivel
      query.sprintf("DELETE FROM balancetemp where nivel(codigo)>%s",combonivel->text(combonivel->currentItem()).ascii());
      conexionbase->ejecuta(query);
      
      //Borramos todo lo que tiene un hijo en el balance
      query.sprintf("DELETE FROM balancetemp WHERE idcuenta IN (SELECT padre FROM balancetemp)");
      conexionbase->ejecuta(query);
      query.sprintf("SELECT * FROM balancetemp WHERE debe <> 0  OR haber <> 0 ORDER BY codigo");
      cursorapt = conexionbase->cargacursor(query,"mycursor");
      
      // Calculamos cuantos registros van a crearse y dimensionamos la tabla.
      num1 = cursorapt->numregistros();
      m_listado->setNumRows(0);
      m_listado->setNumRows(num1);
      j=0;
      while (!cursorapt->eof()) {
         // Acumulamos los totales para al final poder escribirlos
         tsaldoant += atof(cursorapt->valor("asaldo").ascii());
         tsaldo += atof(cursorapt->valor("tsaldo").ascii());
         tdebe += atof(cursorapt->valor("tdebe").ascii());
         thaber += atof(cursorapt->valor("thaber").ascii());
      
         // Hacemos la insercion de los campos en la tabla.
         m_listado->setText(j,CUENTA,cursorapt->valor("codigo"));
         m_listado->setText(j,DENOMINACION,cursorapt->valor("descripcion"));
         m_listado->setText(j,SALDO_ANT,QString::number(atof(cursorapt->valor("asaldo").ascii()),'f',2));
         m_listado->setText(j,DEBE,QString::number(atof(cursorapt->valor("tdebe").ascii()),'f',2));
         m_listado->setText(j,HABER,QString::number(atof(cursorapt->valor("thaber").ascii()),'f',2));
         m_listado->setText(j,SALDO,QString::number(atof(cursorapt->valor("tsaldo").ascii()),'f',2));
         m_listado->setText(j, DEBEEJ,QString::number(atof(cursorapt->valor("ejdebe").ascii()),'f',2));
         m_listado->setText(j, HABEREJ,QString::number(atof(cursorapt->valor("ejhaber").ascii()),'f',2));
         m_listado->setText(j, SALDOEJ,QString::number(atof(cursorapt->valor("ejsaldo").ascii()),'f',2));


         // Ponemos los iconos para que la cosa parezca mas guay.
         if (cursorapt->valor("tipocuenta") == "1")
            m_listado->setPixmap(j,CUENTA, QPixmap(cactivo));
         else if (cursorapt->valor("tipocuenta") == "2")
            m_listado->setPixmap(j,CUENTA, QPixmap(cpasivo));
         else if (cursorapt->valor("tipocuenta") == "3")
            m_listado->setPixmap(j,CUENTA, QPixmap(cneto));
         else if (cursorapt->valor("tipocuenta") == "4")
            m_listado->setPixmap(j,CUENTA, QPixmap(cingresos));
         else if (cursorapt->valor("tipocuenta") == "5")
            m_listado->setPixmap(j,CUENTA, QPixmap(cgastos));      

         // Calculamos la siguiente cuenta registro y finalizamos el bucle
         cursorapt->siguienteregistro();
         j++;
      }// end while

      // Vaciamos el cursor de la base de datos.
      delete cursorapt;

      query.sprintf("DROP TABLE balancetemp");
      conexionbase->ejecuta(query);
      conexionbase->commit();
      
      // Hacemos la actualizacion de los saldos totales
      totalsaldoant->setText(QString::number(tsaldoant,'f',2));
      totaldebe->setText(QString::number(tdebe,'f',2));
      totalhaber->setText(QString::number(thaber,'f',2));
      totalsaldo->setText(QString::number(tsaldo,'f',2));
}// end presentar


void balanceview::accept() {
  presentar();
}// end accept



/** \brief Responde a una petici� de menu contextual sobre el balance.
  * Saca un menu contextual que presenta las opciones necesarias sobre la cuenta.
  * Espera a que se seleccione una opci� o que se quite el menu contextual y llama a la funci� correspondiente
  * con la entrada de menu que se haya seleccionado.
  */
void balanceview::contextmenu(int, int col, const QPoint &poin) {
   QPopupMenu *popup;
   int opcion;
   QString query   ;
   popup = new QPopupMenu;
        popup->insertItem(tr("Ver Extracto (Este día)"),111);
        popup->insertItem(tr("Ver Extracto (Este mes)"),113);
        popup->insertItem(tr("Ver Extracto (Este año)"),114);
	
        popup->insertSeparator();
        popup->insertItem(tr("Ver Diario (Este día)"),101);
        popup->insertItem(tr("Ver Diario (Este mes)"),103);
        popup->insertItem(tr("Ver Diario (Este año)"),104);
        
        // Si estamos sobre la columna del numero de cuenta añadiremos opciones al menu.
        if (col == CUENTA) {
        		popup->insertItem(tr("Editar Cuenta"), 105);
        }// end if
   		opcion = popup->exec(poin);
		  switch(opcion) {
		  		case 101:
					boton_diario1(0);
				break;
				case 103:
					boton_diario1(1);
				break;
				case 104:
					boton_diario1(2);
				break;
				case 111:
					boton_extracto1(0);
				break;
				case 113:
					boton_extracto1(1);
				break;
				case 114:
					boton_extracto1(2);
		  }// end switch
   delete popup;
}// end contextmenu

void balanceview::nivelactivated (int) {
   presentar();
}// end nivelactivated1


/** \BRIEF SLOT que responde a la pulsación del botón de imprimir
  * Crea el objeto \ref BalancePrintView lo inicializa con los mismos valores del balance y lo ejecuta en modo Modal.
  */
void balanceview::boton_imprimir() {
   BalancePrintView * balan = new BalancePrintView(empresaactual);
   balan->inicializa1(m_codigoinicial->text(), m_codigofinal->text(), m_fechainicial1->text(), m_fechafinal1->text(), FALSE);
   balan->exec();
}// end boton_imprimir.


