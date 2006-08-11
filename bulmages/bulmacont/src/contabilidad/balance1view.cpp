/***************************************************************************
                          BalanceTreeView.cpp  -  description
                             -------------------
    begin                : lun jun 23 2003
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

#include "balance1view.h"
#include "qlistviewitem1.h"
//Added by qt3to4:
#include <QPixmap>
#include <Q3PopupMenu>
#include "listcuentasview1.h"
#include "balanceprintview.h"
#include "empresa.h"
#include "calendario.h"

// Incluimos las imagenes que catalogan los tipos de cuentas.
#include "images/cactivo.xpm"
#include "images/cpasivo.xpm"
#include "images/cneto.xpm"
#include "images/cingresos.xpm"
#include "images/cgastos.xpm"

#include <busquedafecha.h>
#include "busquedacuenta.h"
#include "asiento1view.h"


#define CUENTA           m_ccuenta
#define DENOMINACION     m_cdenominacion
#define SALDO_ANT        m_csaldo_ant
#define DEBE            m_cdebe
#define HABER           m_chaber
#define SALDO           m_csaldo
#define DEBEEJ		m_cdebeej
#define HABEREJ		m_chaberej
#define SALDOEJ		m_csaldoej
#define IDCUENTA        m_cidcuenta
#define NIVEL           m_cnivel
#define PADRE           m_cpadre


BalanceTreeView::BalanceTreeView(empresa *emp, QWidget *parent, const char *name, int  ) : QWidget (parent,name) {
    setupUi(this);
    _depura("BalanceTreeView::BalanceTreeView\n", 0);
    m_companyact = emp;
    numdigitos = m_companyact->numdigitosempresa();

    m_codigoinicial->setempresa(emp);
    m_codigofinal->setempresa(emp);

    m_codigoinicial->hideNombre();
    m_codigofinal->hideNombre();
    /// Hacemos la carga de los centros de coste. Rellenamos el combobox correspondiente.

    // cargacostes();
    mui_combocoste->setcompany(emp);
    mui_combocoste->setidc_coste("0");


    listado->clear();

    CUENTA = listado->addColumn("código cuenta",150);
    DENOMINACION = listado->addColumn("nombre cuenta",350);
    SALDO_ANT = listado->addColumn("Saldo Anterior",90);
    DEBE = listado->addColumn("Debe",90);
    HABER = listado->addColumn("Haber",90);
    SALDO = listado->addColumn("Saldo Periodo",90);
    DEBEEJ = listado->addColumn("Debe Ejercicio",90);
    HABEREJ =  listado->addColumn("Haber Ejercicio", 90);
    SALDOEJ = listado->addColumn("Saldo Ejercicio",90);
    NIVEL = listado->addColumn("nivel",0);
    IDCUENTA = listado->addColumn("idcuenta",0);
    PADRE = listado->addColumn("padre",0);

    listado->setColumnAlignment(CUENTA,Qt::AlignLeft);
    listado->setColumnAlignment(DENOMINACION,Qt::AlignLeft);
    listado->setColumnAlignment(SALDO_ANT,Qt::AlignRight);
    listado->setColumnAlignment(DEBE,Qt::AlignRight);
    listado->setColumnAlignment(HABER,Qt::AlignRight);
    listado->setColumnAlignment(SALDO,Qt::AlignRight);
    listado->setColumnAlignment(DEBEEJ,Qt::AlignRight);
    listado->setColumnAlignment(HABEREJ, Qt::AlignRight);
    listado->setColumnAlignment(SALDOEJ,Qt::AlignRight);
    listado->setColumnAlignment(IDCUENTA,Qt::AlignRight);
    listado->setColumnAlignment(NIVEL,Qt::AlignRight);
    listado->setColumnAlignment(PADRE,Qt::AlignRight);
    //	 listado->setPaletteBackgroundColor("#FFFFFF");
    // Establecemos el color de fondo del extracto. El valor lo tiene la clase configuración que es global.
    listado->setPaletteBackgroundColor(confpr->valor(CONF_BG_BALANCETREE).ascii());

    // Inicializamos la tabla de nivel
    combonivel->insertItem("2",0);
    combonivel->insertItem("3",1);
    combonivel->insertItem("4",2);
    combonivel->insertItem("5",3);
    combonivel->insertItem("6",4);
    combonivel->insertItem("7",5);


    connect( listado, SIGNAL( contextMenuRequested( Q3ListViewItem *, const QPoint &, int) ), this, SLOT( contextmenu(Q3ListViewItem *, const QPoint &, int) ) );

    /// Iniciamos los componentes de la fecha para que al principio aparezcan
    /// Como el año inicial.
    QString cadena;
    cadena.sprintf("%2.2d/%2.2d/%4.4d",1, 1, QDate::currentDate().year());
    m_fechainicial1->setText(cadena);
    cadena.sprintf("%2.2d/%2.2d/%4.4d",31, 12, QDate::currentDate().year());
    m_fechafinal1->setText(cadena);


    m_companyact->meteWindow(caption(), this);
    _depura("END BalanceTreeView::BalanceTreeView\n", 0);
}


BalanceTreeView::~BalanceTreeView() {
    _depura("BalanceTreeView::~BalanceTreeView\n", 0);
    m_companyact->sacaWindow(this);
    _depura("END BalanceTreeView::~BalanceTreeView\n", 0);
}


/**********************************************************************************
 Esta función inicializa la ventana de extracto con los mismos datos
 que la ventana de balance, cuentas, fechas y centros de coste
 y pone la ventan de estracto como la ventana principal.
 De esta forma cuando se pulsa sobre el boton extracto estando en la de balance
 se cambia a la ventana de extracto de la forma correcta.
 **********************************************************************************/
// Si el parametro pasado es un:
// 0 -> del dia actual
// 1 -> del mes actual
// 2 -> del año actual
void BalanceTreeView::boton_extracto1(int tipo) {
    _depura("BalanceTreeView::boton_extracto1", 0);
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
        m_companyact->extractoempresa()->inicializa1( listado->currentItem()->text(CUENTA),  listado->currentItem()->text(CUENTA), fecha1.toString("dd/MM/yyyy"),fecha2.toString("dd/MM/yyyy"),  mui_combocoste->idc_coste().toInt());
    }// end if
    m_companyact->extractoempresa()->accept();
    m_companyact->extractoempresa()->show();
    m_companyact->extractoempresa()->setFocus();
    _depura("END BalanceTreeView::boton_extracto1", 0);
}




// Si el parametro pasado es un:
// 0 -> del dia actual
// 1 -> del mes actual
// 2 -> del año actual
void BalanceTreeView::boton_diario1(int tipo) {
    _depura("BalanceTreeView::boton_diario1", 0);
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
        m_companyact->diarioempresa()->inicializa1( fecha1.toString("dd/MM/yyyy"), fecha2.toString("dd/MM/yyyy"), 0);
    }// end if
    m_companyact->diarioempresa()->accept();
    m_companyact->diarioempresa()->show();
    m_companyact->diarioempresa()->setFocus();
    _depura("END BalanceTreeView::boton_diario1", 0);
}



void BalanceTreeView::boton_asiento() {
    _depura("BalanceTreeView::boton_asiento", 0);
    m_companyact->intapuntsempresa()->show();
    m_companyact->intapuntsempresa()->setFocus();
    _depura("END BalanceTreeView::boton_asiento", 0);
}


/****************************************************************************
 
*****************************************************************************/
void BalanceTreeView::inicializa1(QString codinicial, QString codfinal, QString fecha1, QString fecha2, int idc_coste) {
    _depura("BalanceTreeView::inicializa1", 0);
    m_codigoinicial->setText(codinicial);
    m_codigofinal->setText(codfinal);
    m_fechainicial1->setText(normalizafecha(fecha1).toString("dd/MM/yyyy"));
    m_fechafinal1->setText(normalizafecha(fecha2).toString("dd/MM/yyyy"));
    /// Establecemos el centro de coste correspondiente.
    mui_combocoste->setidc_coste(QString::number(idc_coste));
    _depura("END BalanceTreeView::inicializa1\n", 0);
}


void BalanceTreeView::presentar() {
    _depura("BalanceTreeView::presentar", 0);
    QListViewItem1 * it;

    int j,num1;
    double tsaldoant=0, tdebe=0, thaber=0, tsaldo=0;
    QString query;
    cursor2 *cursorapt;
    QString finicial = m_fechainicial1->text();
    QString ffinal = m_fechafinal1->text();
    QString cinicial = m_codigoinicial->codigocuenta();
    QString cfinal = m_codigofinal->codigocuenta();

    QString ejercicio = ffinal.right(4);

    /// Hacemos la consulta de los apuntes a listar en la base de datos.
    int idc_coste;
    idc_coste = mui_combocoste->idc_coste().toInt();

    // La consulta es compleja, requiere la creación de una tabla temporal y de cierta mandanga por lo que puede
    // Causar problemas con el motor de base de datos.
    query= "CREATE TEMPORARY TABLE balancetemp AS SELECT cuenta.idcuenta, codigo, nivel(codigo) AS nivel, cuenta.descripcion, padre, tipocuenta ,debe, haber, tdebe, thaber,(tdebe-thaber) AS tsaldo, (debe-haber) AS saldo, adebe, ahaber, (adebe-ahaber) AS asaldo, ejdebe, ejhaber, (ejdebe-ejhaber) AS ejsaldo FROM cuenta";
    query += " LEFT JOIN (SELECT idcuenta, sum(debe) AS tdebe, sum(haber) AS thaber FROM apunte WHERE fecha >= '"+finicial+"' AND fecha<= '"+ffinal+"' GROUP BY idcuenta) AS t1 ON t1.idcuenta = cuenta.idcuenta";
    query += " LEFT JOIN (SELECT idcuenta, sum(debe) AS adebe, sum(haber) AS ahaber FROM apunte WHERE fecha < '"+finicial+"' GROUP BY idcuenta) AS t2 ON t2.idcuenta = cuenta.idcuenta";
    query += " LEFT JOIN (SELECT idcuenta, sum(debe) AS ejdebe, sum(haber) AS ejhaber FROM apunte WHERE EXTRACT (YEAR FROM fecha) = '"+ejercicio+"' GROUP BY idcuenta) AS t3 ON t3.idcuenta = cuenta.idcuenta";

    m_companyact->begin();
    m_companyact->ejecuta(query);
    query.sprintf("UPDATE balancetemp SET padre=0 WHERE padre ISNULL");

    m_companyact->ejecuta(query);
    query.sprintf("DELETE FROM balancetemp WHERE debe=0 AND haber =0");
    m_companyact->ejecuta(query);
    // Vamos a implementar el tema del código
    if (cinicial != "") {
        query.sprintf("DELETE FROM balancetemp WHERE codigo < '%s'",cinicial.ascii());
        m_companyact->ejecuta(query);
    }// end if
    if (cfinal != "") {
        query.sprintf("DELETE FROM balancetemp WHERE codigo > '%s'",cfinal.ascii());
        m_companyact->ejecuta(query);
    }// end if


    // Para evitar problemas con los nulls hacemos algunos updates
    query.sprintf("UPDATE balancetemp SET tsaldo=0 WHERE tsaldo ISNULL");
    m_companyact->ejecuta(query);
    query.sprintf("UPDATE balancetemp SET tdebe=0 WHERE tdebe ISNULL");
    m_companyact->ejecuta(query);
    query.sprintf("UPDATE balancetemp SET thaber=0 WHERE thaber ISNULL");
    m_companyact->ejecuta(query);
    query.sprintf("UPDATE balancetemp SET asaldo=0 WHERE asaldo ISNULL");
    m_companyact->ejecuta(query);
    query.sprintf("UPDATE balancetemp SET ejsaldo=0 WHERE ejsaldo ISNULL");
    m_companyact->ejecuta(query);
    query.sprintf("UPDATE balancetemp SET ejdebe=0 WHERE ejdebe ISNULL");
    m_companyact->ejecuta(query);
    query.sprintf("UPDATE balancetemp SET ejhaber=0 WHERE ejhaber ISNULL");
    m_companyact->ejecuta(query);
    query.sprintf( "SELECT idcuenta FROM balancetemp ORDER BY padre DESC");
    cursorapt = m_companyact->cargacursor(query,"Balance1view");
    while (!cursorapt->eof())  {
        query.sprintf("SELECT * FROM balancetemp WHERE idcuenta=%s",cursorapt->valor("idcuenta").ascii());
        cursor2 *mycur = m_companyact->cargacursor(query,"cursorrefresco");
        if (!mycur->eof()) {
            query.sprintf("UPDATE balancetemp SET tsaldo = tsaldo + (%2.2f), tdebe = tdebe + (%2.2f), thaber = thaber +(%2.2f), asaldo= asaldo+(%2.2f), ejdebe = ejdebe + (%2.2f), ejhaber = ejhaber +(%2.2f), ejsaldo = ejsaldo + (%2.2f) WHERE idcuenta = %d",atof(mycur->valor("tsaldo").ascii()), atof(mycur->valor("tdebe").ascii()), atof(mycur->valor("thaber").ascii()),atof(mycur->valor("asaldo").ascii()),atof(mycur->valor("ejdebe").ascii()), atof(mycur->valor("ejhaber").ascii()),
                          atof(mycur->valor("ejsaldo").ascii()),  atoi(mycur->valor("padre").ascii()));
            m_companyact->ejecuta(query);
        }// end if
        delete mycur;
        cursorapt->siguienteregistro();
    }// end while
    delete cursorapt;


    query.sprintf("SELECT * FROM balancetemp WHERE debe <> 0  OR haber <> 0 ORDER BY padre");
    cursor2 *cursorapt1 = m_companyact->cargacursor(query,"mycursor");
    // Calculamos cuantos registros van a crearse y dimensionamos la tabla.
    num1 = cursorapt1->numregistros();
    //      listado->setNumRows(num1);
    j=0;
    listado->clear();
    while (!cursorapt1->eof()) {
        QString padre1 = cursorapt1->valor("padre");
        fprintf(stderr,"buscamos el item: %s\n", padre1.ascii());
        Q3ListViewItem *padre = listado->findItem(padre1, IDCUENTA, Qt::CaseSensitive);

        // Si hemos encontrado el padre de la lista lo ponemos, si no lo hemos encontrado no lo ponemos.
        if (padre != 0) {
            it = new QListViewItem1(padre);
        } else {
            it = new QListViewItem1(listado);
        }// end if
        it->setTipo(atoi(cursorapt1->valor("nivel").ascii()));

        // Acumulamos los totales para al final poder escribirlos
        tsaldoant += atof(cursorapt1->valor("asaldo").ascii());
        tsaldo += atof(cursorapt1->valor("tsaldo").ascii());
        tdebe += atof(cursorapt1->valor("tdebe").ascii());
        thaber += atof(cursorapt1->valor("thaber").ascii());

        it->setText(CUENTA, cursorapt1->valor("codigo"));
        if (cursorapt1->valor("tipocuenta") == "1")
            it->setPixmap(CUENTA, QPixmap(cactivo));
        else if (cursorapt1->valor("tipocuenta") == "2")
            it->setPixmap(CUENTA, QPixmap(cpasivo));
        else if (cursorapt1->valor("tipocuenta") == "3")
            it->setPixmap(CUENTA, QPixmap(cneto));
        else if (cursorapt1->valor("tipocuenta") == "4")
            it->setPixmap(CUENTA, QPixmap(cingresos));
        else if (cursorapt1->valor("tipocuenta") == "5")
            it->setPixmap(CUENTA, QPixmap(cgastos));

        it->setText(DENOMINACION, cursorapt1->valor("descripcion"));
        it->setText(SALDO_ANT,QString::number(atof(cursorapt1->valor("asaldo")),'f',2));
        it->setText(DEBE,QString::number(atof(cursorapt1->valor("tdebe")),'f',2));
        it->setText(HABER,QString::number(atof(cursorapt1->valor("thaber")),'f',2));
        it->setText(SALDO,QString::number(atof(cursorapt1->valor("tsaldo")),'f',2));
        it->setText(DEBEEJ,QString::number(atof(cursorapt1->valor("ejdebe")),'f',2));
        it->setText(HABEREJ,QString::number(atof(cursorapt1->valor("ejhaber")),'f',2));
        it->setText(SALDOEJ,QString::number(atof(cursorapt1->valor("ejsaldo")),'f',2));
        it->setText(NIVEL, cursorapt1->valor("nivel"));
        it->setText(IDCUENTA, cursorapt1->valor("idcuenta"));
        it->setText(PADRE, cursorapt1->valor("padre"));
        it->setOpen(true);
        cursorapt1->siguienteregistro();
        j++;
    }// end while

    // Vaciamos el cursor de la base de datos.
    delete cursorapt1;

    // Eliminamos la tabla temporal y cerramos la transacción.
    query.sprintf("DROP TABLE balancetemp");
    m_companyact->ejecuta(query);
    m_companyact->commit();

    // Hacemos la actualizacion de los saldos totales
    totalsaldoant->setText(QString::number(tsaldoant,'f',2));
    totaldebe->setText(QString::number(tdebe,'f',2));
    totalhaber->setText(QString::number(thaber,'f',2));
    totalsaldo->setText(QString::number(tsaldo,'f',2));

    // Activamos la parte de nivel para que se filtre el listado
    nivelactivated(combonivel->currentItem());
    _depura("END BalanceTreeView::presentar", 0);
}



void BalanceTreeView::nivelactivated(int nivel) {
    _depura("BalanceTreeView::nivelactivated", 0);
    int nivel1 = atoi(combonivel->text(nivel).ascii());
    nivelactivated1(nivel1, listado->firstChild());
    _depura("END BalanceTreeView::nivelactivated", 0);
}


void BalanceTreeView::nivelactivated1 (int nivel, Q3ListViewItem *ot) {
    _depura("BalanceTreeView::nivelactivated1", 0);
    if (ot) {
        if (atoi(ot->text(NIVEL).ascii()) <= nivel) {
            ot->setOpen(true);
            ot->setVisible(true);
        } else {
            ot->setOpen(false);
            ot->setVisible(false);
        }// end if
        nivelactivated1(nivel, ot->firstChild());
        nivelactivated1(nivel, ot->nextSibling());
    }// end if
    _depura("END BalanceTreeView::nivelactivated1", 0);
}


/** \brief SLOT que responde a la petici� de un men contextual para un elemento del balance.
  * @param poin Punto en el que se ha hecho la pulsaci� del rat� y que, por tanto, es donde querremos hacer aparecer el menu contextual.
  *
  * Creamos el objeto QPopupMenu con las opciones que queremos que aparezcan.
  * Lo invocamos y segn la opci� que haya elegido el usuario llamamos a la funci� que da respuesta a dicha petici�.
  */
void BalanceTreeView::contextmenu( Q3ListViewItem *, const QPoint &poin, int) {
    _depura("BalanceTreeView::contextmenu", 0);
    Q3PopupMenu *popup;
    int opcion;
    popup = new Q3PopupMenu;
    popup->insertItem(tr("Ver Diario (Este día)"),101);
    popup->insertItem(tr("Ver Diario (Este mes)"),103);
    popup->insertItem(tr("Ver Diario (Este año)"),104);
    popup->insertSeparator();
    popup->insertItem(tr("Ver Extracto (Este día)"),111);
    popup->insertItem(tr("Ver Extracto (Este mes)"),113);
    popup->insertItem(tr("Ver Extracto (Este año)"),114);
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
    _depura("END BalanceTreeView::contextmenu", 0);
}


/** \brief SLOT que responde a la pulsación del boton de imprimir
  * Crea el objeto \ref BalancePrintView lo inicializa con los mismos valores del balance y lo ejecuta en modo Modal.
  */
void BalanceTreeView::boton_imprimir() {
    _depura("BalanceTreeView::boton_imprimir", 0);
    BalancePrintView *balan = new BalancePrintView(m_companyact);
    balan->inicializa1(m_codigoinicial->text(), m_codigofinal->text(), m_fechainicial1->text(), m_fechafinal1->text(), TRUE);
    balan->exec();
    _depura("END BalanceTreeView::boton_imprimir", 0);
}


