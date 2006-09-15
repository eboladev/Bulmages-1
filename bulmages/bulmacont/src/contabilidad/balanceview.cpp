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
#include "arbol.h"
#include "postgresiface2.h"

#include <QWidget>
#include <QLineEdit>
#include <q3datetimeedit.h>
#include <q3filedialog.h>
#include <QToolButton>
#include <QPixmap>
#include <Q3PopupMenu>
#include <QLocale>
#include <QFont>

#include "balanceprintview.h"
#include "qtable1.h"
#include "busquedafecha.h"
#include "busquedacuenta.h"

#define CUENTA          0
#define DENOMINACION    1
#define SALDO_ANT       2
#define DEBE            3
#define HABER           4
#define SALDO           5
#define DEBEEJ          6
#define HABEREJ         7
#define SALDOEJ         8



/** \brief Constructor de clasee
  * \bug No es necesario borrar la tabla de designer para que esto funcione.
  * Se prepara el combobox de niveles a mostrar y se ponen las fechas de balance.
  */
balanceview::balanceview(empresa *emp, QWidget *parent, const char *, int  ) : QWidget(parent) {
    setupUi(this);
    companyact = emp;

    //mui_list->setcompany(emp);

    numdigitos = companyact->numdigitosempresa();
    m_codigoinicial->setempresa(emp);
    m_codigofinal->setempresa(emp);

    // Inicializamos la tabla de nivel
    combonivel->insertItem("2",0);
    combonivel->insertItem("3",1);
    combonivel->insertItem("4",2);
    combonivel->insertItem("5",3);
    combonivel->insertItem("6",4);
    combonivel->insertItem("7",5);

    // Iniciamos los componentes de la fecha para que al principio aparezcan
    // Como el a� inicial.
    QString cadena;
    cadena.sprintf("%2.2d/%2.2d/%4.4d",1, 1, QDate::currentDate().year());
    m_fechainicial1->setText(cadena);
    cadena.sprintf("%2.2d/%2.2d/%4.4d",31, 12, QDate::currentDate().year());
    m_fechafinal1->setText(cadena);
    // Hacemos la carga de los centros de coste. Rellenamos el combobox correspondiente.
    cargacostes();
    
    // Activamos las se�les
    connect(mui_actualizar, SIGNAL(clicked()), this, SLOT(accept()));

    companyact->meteWindow( caption(), this);
}

balanceview::~balanceview() {
	_depura("balanceview::~balanceview", 0);
	companyact->sacaWindow(this);
	_depura("END balanceview::~balanceview", 0);
}



/** \brief Esta funcion se encarga de cargar el combobox de centros de coste para que se pueda seleccionar uno
  * \bug Debe eliminarse el array ccoste.
  */
void balanceview::cargacostes() {
    // Hacemos la carga de los centros de coste. Rellenamos el combobox correspondiente.
    combocoste->clear();
    QString query="SELECT * FROM c_coste ORDER BY nombre";
    companyact->begin();
    cursor2 *cursorcoste = companyact->cargacursor(query,"costes");
    companyact->commit();
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




/** \brief Se encarga  de inicializar la clase con los parametros que se le han pasado.
  * Esta funcion sirve para que desde fuera se pueda preparar a la clase para presentar un balance predeterminado.
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
    while (ccostes[i]!=idc_coste && i<100)
        i++;
    if (i<100) {
        combocoste->setCurrentItem(i);
    }

    /**************************************************/
}// end inicializa1


/** \brief Se encarga de hacer la consulta en la base de datos y mostrar el resultado.
    \bug  Hay que eliminar el uso de double y usar un sistema de punto fijo.
  * Crea una tabla auxiliar de balance y hace en ella todos los calculos necesarios para concretar los resultados
  */
void balanceview::presentar() {
    QString finicial = m_fechainicial1->text();
    QString ffinal = m_fechafinal1->text();
    QString cinicial = m_codigoinicial->codigocuenta().left(2);
    QString cfinal = m_codigofinal->codigocuenta();
    int nivel = combonivel->currentText().toInt();
    bool jerarquico = checksuperiores->isChecked();
    // Extraemos el centro de coste
    int idc_coste = ccostes[combocoste->currentItem()];
    /*
    QString querycoste= "";
    if(idc_coste > 0)
        querycoste = " AND idc_coste = "+QString::number(idc_coste)+" ";
    */
    
    /// A partir de ahora ya no hay tablas temporales ni accesos a disco que merman la ejecucion del programa
    /// Se genera un Arbol dinamico en la memoria RAM que contendra todas y cada una de las cuentas del PGC con sus saldos
    if(sumasysaldosButton->isChecked())
	presentarSyS(finicial, ffinal, cinicial, cfinal, nivel, idc_coste, jerarquico);	// Balance de Sumas y Saldos
    /*
    else if(abreviadoButton->isChecked())
	presentarAbreviado(); // Balance de situacion Abreviado
    else
	presentarResultados(); // Cuenta de perdidas y ganancias
    */
}// end presentar

void balanceview::presentarSyS(QString finicial, QString ffinal, QString cinicial, QString cfinal, int nivel, int idc_coste, bool jerarquico) {
    QLocale spanish(QLocale::Spanish); // vamos a formatear los numeros con punto para los millares y coma para los decimales
    double tsaldoant, tdebe, thaber, tsaldo; //, debeej, haberej, saldoej;
    
    // Primero, averiguaremos la cantidad de ramas iniciales que nacen de la raiz (tantas como numero de cuentas de nivel 2) y las vamos creando
    companyact->begin();
    QString query = "SELECT *, nivel(codigo) AS nivel FROM cuenta ORDER BY codigo";
    cursor2 *ramas;
    ramas = companyact->cargacursor(query);
    Arbol *arbol;
    arbol = new Arbol;
    while(!ramas->eof()) {
	if(atoi(ramas->valor("nivel").ascii()) == 2) // Cuenta raiz
	    arbol->nuevarama(ramas); // Se crea una rama en el arbol (de las primeritas que salen de una raiz)
	ramas->siguienteregistro();
    }
    arbol->inicializa(ramas);
    delete ramas;
    
    // Ahora, usaremos el arbol para poner los datos a cada hoja (cuenta) segun los periodos que necesitemos acotar.
    // Para ello, vamos a recopilar todos los apuntes introducidos agrupados por cuenta para poder averiguar el estado contable de cada cuenta
    query = "SELECT cuenta.idcuenta, numapuntes, cuenta.codigo, saldoant, debe, haber, saldo, debeej, haberej, saldoej FROM (SELECT idcuenta, codigo FROM cuenta) AS cuenta NATURAL JOIN (SELECT idcuenta, count(idcuenta) AS numapuntes,sum(debe) AS debeej, sum(haber) AS haberej, (sum(debe)-sum(haber)) AS saldoej FROM apunte WHERE EXTRACT(year FROM fecha) = '" + ffinal.right(4) + "') GROUP BY idcuenta) AS ejercicio LEFT OUTER JOIN (SELECT idcuenta,sum(debe) AS debe, sum(haber) AS haber, (sum(debe)-sum(haber)) AS saldo FROM apunte WHERE fecha >= '" + finicial + "' AND fecha <= '" + ffinal + "' GROUP BY idcuenta) AS periodo ON periodo.idcuenta=ejercicio.idcuenta LEFT OUTER JOIN (SELECT idcuenta, (sum(debe)-sum(haber)) AS saldoant FROM apunte WHERE fecha < '" + finicial + "' GROUP BY idcuenta) AS anterior ON cuenta.idcuenta=anterior.idcuenta ORDER BY codigo";
    fprintf(stderr,"Query: %s\n", query.toAscii().data());
    ///// OJO!! falta usar el querycoste
    
    // Poblamos el arbol de hojas (cuentas)
    cursor2 *hojas;
    hojas = companyact->cargacursor(query,"Recopilacion por Periodo");
    while(!hojas->eof()) { // Para cada cuenta con apuntes introducidos hay que actualizar hojas del arbol
	arbol->actualizahojas(hojas);
	hojas->siguienteregistro();
    }// end while
    delete hojas;
    arbol->inicia();
    
    // Por fin, presentamos los valores
    // Aunque antes, prepararemos el Widget con la cabecera a mostrar
    QStringList headers, datos;
    headers << "C�igo" << "Denominaci�" << "Saldo anterior" << "Debe" << "Haber" << "Saldo" << "Debe ejercicio" << "Haber ejercicio" << "Saldo ejercicio";
    mui_list->setHeaderLabels(headers);
    mui_list->clear();
    
    QTreeWidgetItem *it; // cada hoja del arbol usara uno de estos Widgets para mostrarse
    int nivelActual;
    QMap <int, QTreeWidgetItem *> ptrList; // mantenemos una tabla con indices de niveles del arbol
    QMap <int, QTreeWidgetItem *>::const_iterator ptrIt, i; // y el iterador para controlar donde accedemos, asi como un indice adicional
    ptrList.clear();
    while (arbol->deshoja(nivel, jerarquico)) {
	QString lcuenta = arbol->hojaactual("codigo");
	QString ldenominacion = arbol->hojaactual("descripcion");
	QString lsaldoant = arbol->hojaactual("saldoant");
	QString ldebe = arbol->hojaactual("debe");
	QString lhaber = arbol->hojaactual("haber");
	QString lsaldo = arbol->hojaactual("saldo");
	QString ldebeej = arbol->hojaactual("debeej");
	QString lhaberej = arbol->hojaactual("haberej");
	QString lsaldoej = arbol->hojaactual("saldoej");
	
	if(lcuenta >= cinicial and lcuenta <= cfinal){
	    // Acumulamos los totales para al final poder escribirlos
	    tsaldoant += lsaldoant.toDouble();
	    tsaldo += lsaldo.toDouble();
	    tdebe += ldebe.toDouble();
	    thaber += lhaber.toDouble();
//	    debeej += ldebeej.toDouble();
//	    haberej += lhaberej.toDouble();
//	    saldoej += lsaldoej.toDouble();
	
	    // Las variables de las filas en formato espa�l
	    lsaldoant = spanish.toString(lsaldoant.toDouble(),'f',2);
	    ldebe = spanish.toString(ldebe.toDouble(),'f',2);
	    lhaber = spanish.toString(lhaber.toDouble(),'f',2);
	    lsaldo = spanish.toString(lsaldo.toDouble(),'f',2);
	    ldebeej = spanish.toString(ldebeej.toDouble(),'f',2);
	    lhaberej = spanish.toString(lhaberej.toDouble(),'f',2);
	    lsaldoej = spanish.toString(lsaldoej.toDouble(),'f',2);
	    
	    /// Ahora, vamos a pintar en el Widget cada linea
	    
	    // Formamos la linea
	    datos << lcuenta << ldenominacion << lsaldoant << ldebe << lhaber << lsaldo << ldebeej << lhaberej << lsaldoej;

	    // Si se van mostrar tambien las cuentas superiores, habra que jerarquizar el arbol. Sino, se pinta cada linea al mismo nivel
	    if(jerarquico) { // jerarquizando...
		nivelActual = lcuenta.length();
		if(nivelActual == 2) {
		    it = new QTreeWidgetItem(mui_list, datos);	// la hoja cuelga de la raiz principal
		} else {
		    while(ptrIt.key() >= nivelActual) 
			ptrIt--; // Ascendemos por el arbol para colgar la hoja en el lugar correcto 
		    it = new QTreeWidgetItem(ptrIt.value(), datos);
		} // end if
		ptrIt = ptrList.insert(nivelActual, it); // insertamos el widget (hoja actual) en la tabla controlada y obtenemos su puntero iterador
		mui_list->setItemExpanded(it, true);
		// Borramos el resto de niveles que cuelgan, para no seguir colgando por esa rama
		i = ptrIt+1;
		while(i!=ptrList.constEnd()) {
		    ptrList.erase(i.key());
		    ++i;
		}
	    } else { // sin jerarquizar...
		it = new QTreeWidgetItem(mui_list, datos);
	    } // end if
	    datos.clear();
	    
	    // Formateamos un poquito la informacion mostrada
	    int tamanyo = 10;
	    if(ldenominacion.length() > 40)
		tamanyo -= 1;
	    else if(ldenominacion.length() > 50)
		tamanyo -= 2;
	    it->setFont(DENOMINACION, QFont("Serif", tamanyo, -1, false));
	    for(int col=SALDO_ANT; col < it->columnCount(); col++) {
		if(col == DEBE or col == HABER or col == SALDO)
		    it->setFont(col, QFont("SansSerif", 10, QFont::DemiBold, false));
		else
		    it->setFont(col, QFont("SansSerif", 10, QFont::Normal, false));	
		it->setTextAlignment(col, Qt::AlignRight);
		if(it->text(col).left(1) == "-")
		    it->setTextColor(col, Qt::darkRed);	// para los valores negativos la fuente en rojo
		else
		    it->setTextColor(col, Qt::darkBlue);	// para los valores negativos la fuente en azul
	    } // end for	
	} // end if
    } // end while
    mui_list->resizeColumnToContents(1);
    
    // Hacemos la actualizacion de los saldos totales en formato espa�l y los ponemos en su Widget
    QString totsaldoant = spanish.toString(tsaldoant,'f',2);
    QString totdebe = spanish.toString(tdebe,'f',2);
    QString tothaber = spanish.toString(thaber,'f',2);
    QString totsaldo = spanish.toString(tsaldo,'f',2);
    //QString totdebeej = spanish.toString(debeej,'f',2);
    //QString tothaberej = spanish.toString(haberej,'f',2);
    //QString totsaldoej = spanish.toString(saldoej,'f',2);    
    totalsaldoant->setText(totsaldoant);
    totaldebe->setText(totdebe);
    totalhaber->setText(tothaber);
    totalsaldo->setText(totsaldo);
    
     // Eliminamos el arbol de la memoria y cerramos la conexion con la BD
    delete arbol;
    companyact->commit();
}

void balanceview::accept() {
    presentar();
}// end accept


void balanceview::nivelactivated (int) {
    presentar();
}// end nivelactivated1


/** \BRIEF SLOT que responde a la pulsacionn del boton de imprimir
  * Crea el objeto \ref BalancePrintView lo inicializa con los mismos valores del balance y lo ejecuta en modo Modal.
  */
void balanceview::boton_imprimir() {
    BalancePrintView * balan = new BalancePrintView(companyact);
    balan->inicializa1(m_codigoinicial->text(), m_codigofinal->text(), m_fechainicial1->text(), m_fechafinal1->text(), FALSE);
    balan->exec();
}// end boton_imprimir.


