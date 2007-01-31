/***************************************************************************
 *   Copyright (C) 2003 by Tomeu Borras Riera                              *
 *   tborras@conetxia.com                                                  *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/

#include <QPixmap>
#include <QMenu>

#include <busquedafecha.h>

#include "balance1view.h"
#include "listcuentasview1.h"
#include "balanceprintview.h"
#include "empresa.h"
#include "calendario.h"
#include "busquedacuenta.h"
#include "asiento1view.h"

/// Incluimos las imagenes que catalogan los tipos de cuentas.
#include "images/cactivo.xpm"
#include "images/cpasivo.xpm"
#include "images/cneto.xpm"
#include "images/cingresos.xpm"
#include "images/cgastos.xpm"

#define CUENTA         m_ccuenta
#define DENOMINACION   m_cdenominacion
#define SALDO_ANT      m_csaldo_ant
#define DEBE           m_cdebe
#define HABER          m_chaber
#define SALDO          m_csaldo
#define DEBEEJ         m_cdebeej
#define HABEREJ        m_chaberej
#define SALDOEJ        m_csaldoej
#define IDCUENTA       m_cidcuenta
#define NIVEL          m_cnivel
#define PADRE          m_cpadre


BalanceTreeView::BalanceTreeView(empresa *emp, QWidget *parent, int)
        : Ficha(parent) {
    setupUi(this);
    _depura("BalanceTreeView::BalanceTreeView", 0);
    m_companyact = emp;
    numdigitos = m_companyact->numdigitosempresa();

    m_codigoinicial->setempresa(emp);
    m_codigofinal->setempresa(emp);

    m_codigoinicial->hideNombre();
    m_codigofinal->hideNombre();
    /// Hacemos la carga de los centros de coste. Rellenamos el combobox correspondiente.
    mui_combocoste->setcompany(emp);
    mui_combocoste->setidc_coste("0");

    listado->clear();

    listado->setColumnCount(12);
    QStringList nombrecolumnas;
    nombrecolumnas << tr("Codigo cuenta")
    << tr("Nombre de la cuenta")
    << tr("Saldo anterior")
    << tr("Debe")
    << tr("Haber")
    << tr("Saldo periodo")
    << tr("Debe ejercicio")
    << tr("Haber ejercicio")
    << tr("Saldo ejercicio")
    << tr("Nivel")
    << tr("ID cuenta")
    << tr("Padre");

    listado->setHeaderLabels(nombrecolumnas);

    CUENTA = 1;
    DENOMINACION = 2;
    SALDO_ANT = 3;
    DEBE = 4;
    HABER = 5;
    SALDO = 6;
    DEBEEJ = 7;
    HABEREJ =  8;
    SALDOEJ = 9;
    NIVEL = 10;
    IDCUENTA = 11;
    PADRE = 12;

    /// Inicializamos la tabla de nivel.
    combonivel->insertItem(0, "2");
    combonivel->insertItem(1, "3");
    combonivel->insertItem(2, "4");
    combonivel->insertItem(3, "5");
    combonivel->insertItem(4, "6");
    combonivel->insertItem(5, "7");

    connect(listado, SIGNAL(contextMenuRequested(QTreeWidgetItem *, const QPoint &, int)), this, SLOT(contextmenu(QTreeWidgetItem *, const QPoint &, int)));

    connect(combonivel, SIGNAL(activated(int)), this, SLOT(nivelactivated(int)));
    /// Iniciamos los componentes de la fecha para que al principio aparezcan
    /// como el a&ntilde;o inicial.
    QString cadena;
    cadena.sprintf("%2.2d/%2.2d/%4.4d", 1, 1, QDate::currentDate().year());
    m_fechainicial1->setText(cadena);
    cadena.sprintf("%2.2d/%2.2d/%4.4d", 31, 12, QDate::currentDate().year());
    m_fechafinal1->setText(cadena);
    m_companyact->meteWindow(windowTitle(), this);
    _depura("END BalanceTreeView::BalanceTreeView", 0);
}


BalanceTreeView::~BalanceTreeView() {
    _depura("BalanceTreeView::~BalanceTreeView", 0);
    m_companyact->sacaWindow(this);
    _depura("END BalanceTreeView::~BalanceTreeView", 0);
}


/// Esta funci&oacute;n inicializa la ventana de extracto con los mismos datos
/// que la ventana de balance, cuentas, fechas y centros de coste
/// y pone la ventana de estracto como la ventana principal.
/// De esta forma cuando se pulsa sobre el bot&oacute;n extracto estando en la de balance
/// se cambia a la ventana de extracto de la forma correcta.
/// Si el parametro pasado es un:
/// 0 -> del d&iacute;a actual.
/// 1 -> del mes actual.
/// 2 -> del a&ntilde;o actual.
void BalanceTreeView::boton_extracto1(int tipo) {
    _depura("BalanceTreeView::boton_extracto1", 0);
    QDate fecha1, fecha2, fechaact, fechaact1;
    if (!m_fechainicial1->text().isEmpty()) {
        fechaact = normalizafecha(m_fechainicial1->text());
        fechaact1 = normalizafecha(m_fechafinal1->text());
        switch (tipo) {
        case 0:
            fecha1.setYMD(fechaact.year(), fechaact.month(),fechaact.day());
            fecha2.setYMD(fechaact1.year(), fechaact1.month(), fechaact1.day());
            break;
        case 1:
            fecha1.setYMD(fechaact.year(), fechaact.month(), 1);
            fecha2.setYMD(fechaact.year(), fechaact.month(), fechaact.daysInMonth());
            break;
        case 2:
            fecha1.setYMD(fechaact.year(), 1, 1);
            fecha2.setYMD(fechaact.year(), 12, 31);
            break;
        } // end switch
        m_companyact->extractoempresa()->inicializa1(listado->currentItem()->text(CUENTA), listado->currentItem()->text(CUENTA), fecha1.toString("dd/MM/yyyy"), fecha2.toString("dd/MM/yyyy"), mui_combocoste->idc_coste().toInt());
    } // end if
    m_companyact->extractoempresa()->accept();
    m_companyact->extractoempresa()->show();
    m_companyact->extractoempresa()->setFocus();
    _depura("END BalanceTreeView::boton_extracto1", 0);
}


/// Si el par&aacute;metro pasado es un:
/// 0 -> del d&iacute;a actual.
/// 1 -> del mes actual.
/// 2 -> del a&ntilde;o actual.
void BalanceTreeView::boton_diario1(int tipo) {
    _depura("BalanceTreeView::boton_diario1", 0);
    QDate fecha1, fecha2, fechaact, fechaact1;
    if (!m_fechainicial1->text().isEmpty()) {
        fechaact = normalizafecha(m_fechainicial1->text());
        fechaact1 = normalizafecha(m_fechafinal1->text());
        switch (tipo) {
        case 0:
            fecha1.setYMD(fechaact.year(), fechaact.month(),fechaact.day());
            fecha2.setYMD(fechaact1.year(), fechaact1.month(), fechaact1.day());
            break;
        case 1:
            fecha1.setYMD(fechaact.year(), fechaact.month(), 1);
            fecha2.setYMD(fechaact.year(), fechaact.month(), fechaact.daysInMonth());
            break;
        case 2:
            fecha1.setYMD(fechaact.year(), 1, 1);
            fecha2.setYMD(fechaact.year(), 12, 31);
            break;
        } // end switch
        m_companyact->diarioempresa()->inicializa1( fecha1.toString("dd/MM/yyyy"), fecha2.toString("dd/MM/yyyy"), 0);
    } // end if
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


void BalanceTreeView::inicializa1(QString codinicial, QString codfinal, QString fecha1, QString fecha2, int idc_coste) {
    _depura("BalanceTreeView::inicializa1", 0);
    m_codigoinicial->setText(codinicial);
    m_codigofinal->setText(codfinal);
    m_fechainicial1->setText(normalizafecha(fecha1).toString("dd/MM/yyyy"));
    m_fechafinal1->setText(normalizafecha(fecha2).toString("dd/MM/yyyy"));
    /// Establecemos el centro de coste correspondiente.
    mui_combocoste->setidc_coste(QString::number(idc_coste));
    _depura("END BalanceTreeView::inicializa1", 0);
}


void BalanceTreeView::presentar() {
    _depura("BalanceTreeView::presentar", 0);
    //QListViewItem1 * it;
    QTreeWidgetItem *it;
    int num1;
    double tsaldoant = 0, tdebe = 0, thaber = 0, tsaldo = 0;
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

    /// La consulta es compleja, requiere la creaci&oacute;n de una tabla temporal y de
    /// cierta mandanga por lo que puede causar problemas con el motor de base de datos.
    query = "CREATE TEMPORARY TABLE balancetemp AS SELECT cuenta.idcuenta, codigo, nivel(codigo) AS nivel, cuenta.descripcion, padre, tipocuenta ,debe, haber, tdebe, thaber,(tdebe - thaber) AS tsaldo, (debe - haber) AS saldo, adebe, ahaber, (adebe - ahaber) AS asaldo, ejdebe, ejhaber, (ejdebe - ejhaber) AS ejsaldo FROM cuenta";
    query += " LEFT JOIN (SELECT idcuenta, sum(debe) AS tdebe, sum(haber) AS thaber FROM apunte WHERE fecha >= '" + finicial + "' AND fecha <= '" + ffinal + "' GROUP BY idcuenta) AS t1 ON t1.idcuenta = cuenta.idcuenta";
    query += " LEFT JOIN (SELECT idcuenta, sum(debe) AS adebe, sum(haber) AS ahaber FROM apunte WHERE fecha < '" + finicial + "' GROUP BY idcuenta) AS t2 ON t2.idcuenta = cuenta.idcuenta";
    query += " LEFT JOIN (SELECT idcuenta, sum(debe) AS ejdebe, sum(haber) AS ejhaber FROM apunte WHERE EXTRACT (YEAR FROM fecha) = '" + ejercicio + "' GROUP BY idcuenta) AS t3 ON t3.idcuenta = cuenta.idcuenta";

    m_companyact->begin();
    m_companyact->ejecuta(query);
    query.sprintf("UPDATE balancetemp SET padre = 0 WHERE padre ISNULL");

    m_companyact->ejecuta(query);
    query.sprintf("DELETE FROM balancetemp WHERE debe = 0 AND haber = 0");
    m_companyact->ejecuta(query);
    /// Vamos a implementar el tema del c&oacute;digo.
    if (cinicial != "") {
        query.sprintf("DELETE FROM balancetemp WHERE codigo < '%s'", cinicial.toAscii().constData());
        m_companyact->ejecuta(query);
    } // end if
    if (cfinal != "") {
        query.sprintf("DELETE FROM balancetemp WHERE codigo > '%s'", cfinal.toAscii().constData());
        m_companyact->ejecuta(query);
    } // end if

    /// Para evitar problemas con los nulls hacemos algunos updates.
    query.sprintf("UPDATE balancetemp SET tsaldo = 0 WHERE tsaldo ISNULL");
    m_companyact->ejecuta(query);
    query.sprintf("UPDATE balancetemp SET tdebe = 0 WHERE tdebe ISNULL");
    m_companyact->ejecuta(query);
    query.sprintf("UPDATE balancetemp SET thaber = 0 WHERE thaber ISNULL");
    m_companyact->ejecuta(query);
    query.sprintf("UPDATE balancetemp SET asaldo = 0 WHERE asaldo ISNULL");
    m_companyact->ejecuta(query);
    query.sprintf("UPDATE balancetemp SET ejsaldo = 0 WHERE ejsaldo ISNULL");
    m_companyact->ejecuta(query);
    query.sprintf("UPDATE balancetemp SET ejdebe = 0 WHERE ejdebe ISNULL");
    m_companyact->ejecuta(query);
    query.sprintf("UPDATE balancetemp SET ejhaber = 0 WHERE ejhaber ISNULL");
    m_companyact->ejecuta(query);

    /// Cargamos el balance temporal
    query.sprintf("SELECT idcuenta FROM balancetemp ORDER BY padre DESC");
    cursorapt = m_companyact->cargacursor(query);
    while (!cursorapt->eof()) {
        query.sprintf("SELECT * FROM balancetemp WHERE idcuenta = %s", cursorapt->valor("idcuenta").toAscii().constData());
        cursor2 *mycur = m_companyact->cargacursor(query);
        if (!mycur->eof()) {
 	    query = "UPDATE balancetemp SET tsaldo = tsaldo + "+mycur->valor("tsaldo")+", tdebe = tdebe + "+mycur->valor("tdebe")+", thaber = thaber +"+mycur->valor("thaber")+", asaldo = asaldo+"+mycur->valor("asaldo")+", ejdebe = ejdebe + "+mycur->valor("ejdebe")+", ejhaber = ejhaber + "+mycur->valor("ejhaber")+", ejsaldo = ejsaldo + "+mycur->valor("ejsaldo")+" WHERE idcuenta = "+mycur->valor("padre");
            m_companyact->ejecuta(query);
        } /// end if
        delete mycur;
        cursorapt->siguienteregistro();
    } // end while
    delete cursorapt;

    query ="SELECT * FROM balancetemp WHERE debe <> 0  OR haber <> 0 ORDER BY padre";
    cursor2 *cursorapt1 = m_companyact->cargacursor(query);
    /// Calculamos cuantos registros van a crearse y dimensionamos la tabla.
    num1 = cursorapt1->numregistros();
    listado->clear();
    while (!cursorapt1->eof()) {
        QString padre1 = cursorapt1->valor("padre");

	QTreeWidgetItem *padre = NULL;
	if (! listado->findItems(padre1, Qt::MatchCaseSensitive, IDCUENTA).isEmpty())
        	padre = listado->findItems(padre1, Qt::MatchCaseSensitive, IDCUENTA).first();

        /// Si hemos encontrado el padre de la lista lo ponemos, si no lo hemos encontrado
        /// no lo ponemos.
        if (padre != NULL) {
            it = new QTreeWidgetItem(padre);
        } else {
            it = new QTreeWidgetItem(listado);
        } // end if


	/// Establecemos los alineados del nuevo elemento creado.
        it->setTextAlignment(CUENTA, Qt::AlignLeft);
        it->setTextAlignment(DENOMINACION, Qt::AlignLeft);
        it->setTextAlignment(SALDO_ANT, Qt::AlignRight);
        it->setTextAlignment(DEBE, Qt::AlignRight);
        it->setTextAlignment(HABER, Qt::AlignRight);
        it->setTextAlignment(SALDO, Qt::AlignRight);
        it->setTextAlignment(DEBEEJ, Qt::AlignRight);
        it->setTextAlignment(HABEREJ, Qt::AlignRight);
        it->setTextAlignment(SALDOEJ, Qt::AlignRight);
        it->setTextAlignment(IDCUENTA, Qt::AlignRight);
        it->setTextAlignment(NIVEL, Qt::AlignRight);
        it->setTextAlignment(PADRE, Qt::AlignRight);

	it->treeWidget()->expandItem(it);

        /// Acumulamos los totales para al final poder escribirlos.
        tsaldoant += atof(cursorapt1->valor("asaldo").toAscii());
        tsaldo += atof(cursorapt1->valor("tsaldo").toAscii());
        tdebe += atof(cursorapt1->valor("tdebe").toAscii());
        thaber += atof(cursorapt1->valor("thaber").toAscii());

        it->setText(CUENTA, cursorapt1->valor("codigo"));
        if (cursorapt1->valor("tipocuenta") == "1") {
            //it->setPixmap(CUENTA, QPixmap(cactivo));
        } else if (cursorapt1->valor("tipocuenta") == "2") {
            //it->setPixmap(CUENTA, QPixmap(cpasivo));
        } else if (cursorapt1->valor("tipocuenta") == "3") {
            //it->setPixmap(CUENTA, QPixmap(cneto));
        } else if (cursorapt1->valor("tipocuenta") == "4") {
            //it->setPixmap(CUENTA, QPixmap(cingresos));
        } else if (cursorapt1->valor("tipocuenta") == "5") {
            //it->setPixmap(CUENTA, QPixmap(cgastos));
        } // end if

        it->setText(DENOMINACION, cursorapt1->valor("descripcion"));
        it->setText(SALDO_ANT, QString::number(cursorapt1->valor("asaldo").toDouble(), 'f', 2));
        it->setText(DEBE, QString::number(cursorapt1->valor("tdebe").toDouble(), 'f', 2));
        it->setText(HABER, QString::number(cursorapt1->valor("thaber").toDouble(), 'f', 2));
        it->setText(SALDO, QString::number(cursorapt1->valor("tsaldo").toDouble(), 'f', 2));
        it->setText(DEBEEJ, QString::number(cursorapt1->valor("ejdebe").toDouble(), 'f', 2));
        it->setText(HABEREJ, QString::number(cursorapt1->valor("ejhaber").toDouble(), 'f', 2));
        it->setText(SALDOEJ, QString::number(cursorapt1->valor("ejsaldo").toDouble(), 'f', 2));

        it->setText(NIVEL, cursorapt1->valor("nivel"));
        it->setText(IDCUENTA, cursorapt1->valor("idcuenta"));
        it->setText(PADRE, cursorapt1->valor("padre"));
        cursorapt1->siguienteregistro();
    } // end while

    /// Vaciamos el cursor de la base de datos.
    delete cursorapt1;

    /// Eliminamos la tabla temporal y cerramos la transacci&oacute;n.
    query.sprintf("DROP TABLE balancetemp");
    m_companyact->ejecuta(query);
    m_companyact->commit();

    /// Hacemos la actualizacion de los saldos totales.
    totalsaldoant->setText(QString::number(tsaldoant, 'f', 2));
    totaldebe->setText(QString::number(tdebe, 'f', 2));
    totalhaber->setText(QString::number(thaber, 'f', 2));
    totalsaldo->setText(QString::number(tsaldo, 'f', 2));

    /// Activamos la parte de nivel para que se filtre el listado.
    nivelactivated(combonivel->currentIndex());
    _depura("END BalanceTreeView::presentar", 0);
}


void BalanceTreeView::nivelactivated(int nivel) {
    _depura("BalanceTreeView::nivelactivated", 0);
    combonivel->setCurrentIndex(nivel);
    int nivel1 = combonivel->currentText().toInt();
    
    int j = 0;
    while (j < listado->topLevelItemCount()) {
    	nivelactivated1(nivel1, listado->topLevelItem(j));
        j++;
    } // end while
    _depura("END BalanceTreeView::nivelactivated", 0);
}


//void BalanceTreeView::nivelactivated1(int nivel, Q3ListViewItem *ot) {
void BalanceTreeView::nivelactivated1(int nivel, QTreeWidgetItem *ot) {
    _depura("BalanceTreeView::nivelactivated1", 0);
    if (ot) {
        if (atoi(ot->text(NIVEL).toAscii()) <= nivel) {
	    ot->treeWidget()->setItemExpanded(ot, TRUE);
            ot->treeWidget()->expandItem(ot);
        } else {
	    ot->treeWidget()->setItemExpanded(ot, FALSE);
            ot->treeWidget()->collapseItem(ot);
        } // end if
//        nivelactivated1(nivel, ot->firstChild());
//        nivelactivated1(nivel, ot->nextSibling());
    } // end if
    _depura("END BalanceTreeView::nivelactivated1", 0);
}


/// SLOT que responde a la petici&oacute;n de un men&uacute; contextual para un elemento
/// del balance.
/** @param poin Punto en el que se ha hecho la pulsaci&oacute;n del rat&oacute;n y que,
    por tanto, es donde queremos hacer aparecer el men&uacute; contextual.
    Creamos el objeto QPopupMenu con las opciones que queremos que aparezcan.
    Lo invocamos y seg&uacute;n la opci&oacute;n que haya elegido el usuario llamamos
    a la funci&oacute;n que da respuesta a dicha petici&oacute;n. */
void BalanceTreeView::contextmenu(const QPoint &point) {
    _depura("BalanceTreeView::contextmenu", 0);

    QMenu *menupopup = new QMenu(this);
    QAction *opt1 = menupopup->addAction(tr("Ver Diario (este dia)"));
    QAction *opt2 = menupopup->addAction(tr("Ver Diario (este mes)"));
    QAction *opt3 = menupopup->addAction(tr("Ver Diario (este anyo)"));
    menupopup->addSeparator();
    QAction *opt4 = menupopup->addAction(tr("Ver extracto (este dia)"));
    QAction *opt5 = menupopup->addAction(tr("Ver extracto (este mes)"));
    QAction *opt6 = menupopup->addAction(tr("Ver extracto (este anyo)"));
    QAction *opcion = menupopup->exec(point);

    if (opcion == opt1) {
        boton_diario1(0);
    } // end if
    if (opcion == opt2) {
        boton_diario1(1);
    } // end if
    if (opcion == opt3) {
        boton_diario1(2);
    } // end if
    if (opcion == opt4) {
        boton_extracto1(0);
    } // end if
    if (opcion == opt5) {
        boton_extracto1(1);
    } // end if
    if (opcion == opt6) {
        boton_extracto1(2);
    } // end if

    delete menupopup;
    _depura("END BalanceTreeView::contextmenu", 0);
}


/// SLOT que responde a la pulsaci&oacute;n del bot&oacute;n de imprimir.
/** Crea el objeto \ref BalancePrintView lo inicializa con los mismos valores del balance
    y lo ejecuta en modo Modal. */
void BalanceTreeView::boton_imprimir() {
    _depura("BalanceTreeView::boton_imprimir", 0);
    BalancePrintView *balan = new BalancePrintView(m_companyact);
    balan->inicializa1(m_codigoinicial->text(), m_codigofinal->text(), m_fechainicial1->text(), m_fechafinal1->text(), TRUE);
    balan->exec();
    _depura("END BalanceTreeView::boton_imprimir", 0);
}

