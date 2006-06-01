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
#include <q3datetimeedit.h>
#include <q3filedialog.h>
#include <qtoolbutton.h>
#include <QPixmap>
#include <Q3PopupMenu>

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
balanceview::balanceview(empresa *emp, QWidget *parent, const char *name, int  ) : QWidget(parent) {
    setupUi(this);
    companyact = emp;

    mui_list->setcompany(emp);


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
    // Como el año inicial.
    QString cadena;
    cadena.sprintf("%2.2d/%2.2d/%4.4d",1, 1, QDate::currentDate().year());
    m_fechainicial1->setText(cadena);
    cadena.sprintf("%2.2d/%2.2d/%4.4d",31, 12, QDate::currentDate().year());
    m_fechafinal1->setText(cadena);
    // Hacemos la carga de los centros de coste. Rellenamos el combobox correspondiente.
    cargacostes();
}// end balanceview

balanceview::~balanceview() {}



/** \brief Esta función se encarga de cargar el combo box de centros de coste para que se pueda seleccionar uno
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
    QString querycoste= "";
    if(idc_coste > 0)
        querycoste = " AND idc_coste = "+QString::number(idc_coste)+" ";

    // La consulta es compleja, requiere la creación de una tabla temporal y de cierta mandanga por lo que puede
    // Causar problemas con el motor de base de datos.
    fprintf(stderr,"BALANCE: Empezamos a hacer la presentacion\n");
    companyact->begin();
    query= "CREATE TEMPORARY TABLE balancetemp AS SELECT cuenta.idcuenta, codigo, nivel(codigo) AS nivel, cuenta.descripcion, padre, tipocuenta ,debe, haber, tdebe, thaber,(tdebe-thaber) AS tsaldo, (debe-haber) AS saldo, adebe, ahaber, (adebe-ahaber) AS asaldo, ejdebe, ejhaber, (ejdebe-ejhaber) AS ejsaldo FROM cuenta";

    query += " LEFT JOIN (SELECT idcuenta, sum(debe) AS tdebe, sum(haber) AS thaber FROM apunte WHERE fecha >= '"+finicial+"' AND fecha<= '"+ffinal+"' "+querycoste+" GROUP BY idcuenta) AS t1 ON t1.idcuenta = cuenta.idcuenta";

    query += " LEFT JOIN (SELECT idcuenta, sum(debe) AS adebe, sum(haber) AS ahaber FROM apunte WHERE fecha < '"+finicial+"' "+querycoste+" GROUP BY idcuenta) AS t2 ON t2.idcuenta = cuenta.idcuenta";

    query += " LEFT JOIN (SELECT idcuenta, sum(debe) AS ejdebe, sum(haber) AS ejhaber FROM apunte WHERE EXTRACT (YEAR FROM fecha) = '"+ejercicio+"' "+querycoste+" GROUP BY idcuenta) AS t3 ON t3.idcuenta = cuenta.idcuenta";

    companyact->ejecuta(query);
    query.sprintf("UPDATE balancetemp SET padre=0 WHERE padre ISNULL");
    companyact->ejecuta(query);
    query.sprintf("DELETE FROM balancetemp WHERE debe=0 AND haber =0");
    companyact->ejecuta(query);

    // Vamos a implementar el tema del código
    if (cinicial != "") {
        query.sprintf("DELETE FROM balancetemp WHERE codigo < '%s'",cinicial.ascii());
        companyact->ejecuta(query);
    }// end if
    if (cfinal != "") {
        query.sprintf("DELETE FROM balancetemp WHERE codigo > '%s'",cfinal.ascii());
        companyact->ejecuta(query);
    }// end if

    // Para evitar problemas con los nulls hacemos algunos updates
    query.sprintf("UPDATE balancetemp SET tsaldo=0 WHERE tsaldo ISNULL");
    companyact->ejecuta(query);
    query.sprintf("UPDATE balancetemp SET tdebe=0 WHERE tdebe ISNULL");
    companyact->ejecuta(query);
    query.sprintf("UPDATE balancetemp SET thaber=0 WHERE thaber ISNULL");
    companyact->ejecuta(query);
    query.sprintf("UPDATE balancetemp SET asaldo=0 WHERE asaldo ISNULL");
    companyact->ejecuta(query);
    query.sprintf("UPDATE balancetemp SET ejsaldo=0 WHERE ejsaldo ISNULL");
    companyact->ejecuta(query);
    query.sprintf("UPDATE balancetemp SET ejdebe=0 WHERE ejdebe ISNULL");
    companyact->ejecuta(query);
    query.sprintf("UPDATE balancetemp SET ejhaber=0 WHERE ejhaber ISNULL");
    companyact->ejecuta(query);
    query.sprintf( "SELECT idcuenta FROM balancetemp ORDER BY padre DESC");
    cursorapt = companyact->cargacursor(query,"Balance1view");
    while (!cursorapt->eof())  {
        query.sprintf("SELECT * FROM balancetemp WHERE idcuenta=%s",cursorapt->valor("idcuenta").ascii());
        cursor2 *mycur = companyact->cargacursor(query,"cursorrefresco");
        if (!mycur->eof()) {
            query.sprintf("UPDATE balancetemp SET tsaldo = tsaldo + (%2.2f), tdebe = tdebe + (%2.2f), thaber = thaber +(%2.2f), asaldo= asaldo+(%2.2f), ejdebe = ejdebe + (%2.2f), ejhaber = ejhaber +(%2.2f), ejsaldo = ejsaldo + (%2.2f) WHERE idcuenta = %d",atof(mycur->valor("tsaldo").ascii()), atof(mycur->valor("tdebe").ascii()), atof(mycur->valor("thaber").ascii()),atof(mycur->valor("asaldo").ascii()),atof(mycur->valor("ejdebe").ascii()), atof(mycur->valor("ejhaber").ascii()),
                          atof(mycur->valor("ejsaldo").ascii()),  atoi(mycur->valor("padre").ascii()));
            companyact->ejecuta(query);
        }// end if
        delete mycur;
        cursorapt->siguienteregistro();
    }// end while
    delete cursorapt;

    companyact->commit();
    companyact->begin();
    // Borramos todo lo que no es de este nivel
    query.sprintf("DELETE FROM balancetemp where nivel(codigo)>%s",combonivel->text(combonivel->currentItem()).ascii());
    companyact->ejecuta(query);

    //Borramos todo lo que tiene un hijo en el balance
    query.sprintf("DELETE FROM balancetemp WHERE idcuenta IN (SELECT padre FROM balancetemp)");
    companyact->ejecuta(query);
    query.sprintf("SELECT * FROM balancetemp WHERE debe <> 0  OR haber <> 0 ORDER BY codigo");
    cursorapt = companyact->cargacursor(query,"mycursor");

    // Presentamos el mui list
    mui_list->cargar(cursorapt);
    cursorapt->primerregistro();

    // Calculamos cuantos registros van a crearse y dimensionamos la tabla.
    num1 = cursorapt->numregistros();

    j=0;
    while (!cursorapt->eof()) {
        // Acumulamos los totales para al final poder escribirlos
        tsaldoant += atof(cursorapt->valor("asaldo").ascii());
        tsaldo += atof(cursorapt->valor("tsaldo").ascii());
        tdebe += atof(cursorapt->valor("tdebe").ascii());
        thaber += atof(cursorapt->valor("thaber").ascii());


        // Calculamos la siguiente cuenta registro y finalizamos el bucle
        cursorapt->siguienteregistro();
        j++;
    }// end while

    // Vaciamos el cursor de la base de datos.
    delete cursorapt;

    query.sprintf("DROP TABLE balancetemp");
    companyact->ejecuta(query);
    companyact->commit();

    // Hacemos la actualizacion de los saldos totales
    totalsaldoant->setText(QString::number(tsaldoant,'f',2));
    totaldebe->setText(QString::number(tdebe,'f',2));
    totalhaber->setText(QString::number(thaber,'f',2));
    totalsaldo->setText(QString::number(tsaldo,'f',2));
}// end presentar


void balanceview::accept() {
    presentar();
}// end accept


void balanceview::nivelactivated (int) {
    presentar();
}// end nivelactivated1


/** \BRIEF SLOT que responde a la pulsación del botón de imprimir
  * Crea el objeto \ref BalancePrintView lo inicializa con los mismos valores del balance y lo ejecuta en modo Modal.
  */
void balanceview::boton_imprimir() {
    BalancePrintView * balan = new BalancePrintView(companyact);
    balan->inicializa1(m_codigoinicial->text(), m_codigofinal->text(), m_fechainicial1->text(), m_fechafinal1->text(), FALSE);
    balan->exec();
}// end boton_imprimir.


