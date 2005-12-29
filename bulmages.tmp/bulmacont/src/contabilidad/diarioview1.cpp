/***************************************************************************
                          diarioview1.cpp  -  description
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

#include <qwidget.h>
#include <q3table.h>
#include <qlineedit.h>
#include <q3datetimeedit.h>
#include <q3filedialog.h>
#include <qcolor.h>
#include <q3popupmenu.h>
#include <qlayout.h>
#include <qcheckbox.h>
//Added by qt3to4:
#include <QPixmap>

#include "empresa.h"
#include "diarioview1.h"
#include "funcaux.h"
#include "diarioprintview.h"
#include "diarioprint.h"
#include "configuracion.h"
#include "calendario.h"

#include "filtrardiarioview.h"
#include "intapunts3view.h"
#include "extractoview1.h"
#include "balanceview.h"
#include "selectccosteview.h"
#include "selectcanalview.h"

// Incluimos las imagenes que catalogan los tipos de cuentas.
#include "images/cactivo.xpm"
#include "images/cpasivo.xpm"
#include "images/cneto.xpm"
#include "images/cingresos.xpm"
#include "images/cgastos.xpm"

#include "busquedafecha.h"
#include "qtable1.h"

#define COL_FECHA 0
#define COL_ORDENASIENTO 1
#define COL_CUENTA 2
#define COL_DENOMINACION 3
#define COL_DEBE 4
#define COL_HABER 5
#define COL_CONCEPTO 6
#define COL_CCOSTE 7
#define COL_CANAL 8
#define COL_CONTRAPARTIDA 9
#define COL_NUMASIENTO 10

diarioview1::diarioview1(empresa *emp, QWidget *parent, const char *name, int  ) : diariodlg1(parent,name) {
    empresaactual = emp;
    conexionbase = empresaactual->bdempresa();

    m_listado->setSorting( FALSE );
    m_listado->setSelectionMode( Q3Table::SingleRow );

    // Trabajamos con las fuentes
    QFont m_listado_font(  m_listado->font() );
    m_listado_font.setPointSize(atoi(confpr->valor(CONF_FONTSIZE_DIARIO).ascii()));
    m_listado_font.setFamily(confpr->valor(CONF_FONTFAMILY_DIARIO).ascii());
    m_listado->setFont( m_listado_font );

    m_listado->setColumnMovingEnabled( TRUE );
    m_listado->setReadOnly(TRUE);

    m_listado->setNumCols(11);
    m_listado->horizontalHeader()->setLabel( COL_FECHA, tr( "Fecha" ) );
    m_listado->horizontalHeader()->setLabel( COL_NUMASIENTO, tr( "N.Asiento." ) );
    m_listado->horizontalHeader()->setLabel( COL_CUENTA, tr( "Cuenta" ) );
    m_listado->horizontalHeader()->setLabel( COL_DENOMINACION, tr( "Denominación" ) );
    m_listado->horizontalHeader()->setLabel( COL_DEBE, tr( "Debe" ) );
    m_listado->horizontalHeader()->setLabel( COL_HABER, tr( "Haber" ) );
    m_listado->horizontalHeader()->setLabel( COL_CONCEPTO, tr( "Concepto" ) );
    m_listado->horizontalHeader()->setLabel( COL_CCOSTE, tr( "C. Coste" ) );
    m_listado->horizontalHeader()->setLabel( COL_CANAL, tr( "Canal" ) );
    m_listado->horizontalHeader()->setLabel( COL_CONTRAPARTIDA, tr( "Contrapartida" ) );
    m_listado->horizontalHeader()->setLabel( COL_ORDENASIENTO, tr( "Asiento") );


    m_listado->setColumnWidth(COL_FECHA,75);
    m_listado->setColumnWidth(COL_NUMASIENTO,50);
    m_listado->setColumnWidth(COL_ORDENASIENTO, 50);
    m_listado->setColumnWidth(COL_CUENTA,100);
    m_listado->setColumnWidth(COL_DENOMINACION,300);
    m_listado->setColumnWidth(COL_DEBE,75);
    m_listado->setColumnWidth(COL_HABER,75);
    m_listado->setColumnWidth(COL_CONCEPTO,300);
    m_listado->setColumnWidth(COL_CCOSTE,75);
    m_listado->setColumnWidth(COL_CANAL,75);
    m_listado->setColumnWidth(COL_CONTRAPARTIDA,100);

    // Establecemos el color de fondo, segun la configuración.
    m_listado->setPaletteBackgroundColor(confpr->valor(CONF_BG_DIARIO).ascii());

    m_listado->hideColumn(COL_NUMASIENTO);
    // Dependiendo de la configuración mostramos o no mostramos la contrapartida, los canales y los centros de coste
    if (confpr->valor(CONF_CONTRAPARTIDA_DIARIO) != "YES")
        m_listado->hideColumn(COL_CONTRAPARTIDA);
    if (confpr->valor(CONF_CANAL_DIARIO) != "YES")
        m_listado->hideColumn(COL_CANAL);
    if (confpr->valor(CONF_CCOSTE_DIARIO) != "YES")
        m_listado->hideColumn(COL_CCOSTE);


    connect( m_listado, SIGNAL( contextMenuRequested(int,int,const QPoint&) ), this, SLOT( contextmenu(int,int,const QPoint&) ) );


    // Iniciamos los componentes de la fecha para que al principio aparezcan
    // Como el año inicial.
    char cadena[10];
    sprintf(cadena,"%2.2d/%2.2d/%4.4d",1, 1, QDate::currentDate().year());
    m_fechainicial1->setText(cadena);
    sprintf(cadena,"%2.2d/%2.2d/%4.4d",31, 12, QDate::currentDate().year());
    m_fechafinal1->setText(cadena);

    //Inicializamos el filtro
    filt = new filtrardiarioview(empresaactual,0,0);
}// end diarioview1

diarioview1::~diarioview1() {
    delete filt;
}// end ~diarioview1

void diarioview1::inicializa2(intapunts3view *inta, extractoview1 *ext, balanceview *bal) {
    introapunts = inta;
    extracto = ext;
    balance=bal;
}// end inicializa2





void diarioview1::inicializa1(QString finicial, QString ffinal, int ) {
    QString s1, s2, s3;
    QDate fecha1aux;
    int dia, mes, ano;

    s1= finicial;
    s2=s1.mid(0,2);
    dia = atoi(s2.ascii());
    s2=s1.mid(3,2);
    mes = atoi(s2.ascii());
    s2=s1.mid(6,4);
    ano = atoi(s2.ascii());
    fecha1aux.setYMD(ano,mes,dia);
    QString cadena2;
    cadena2.sprintf("%2.2d/%2.2d/%4.4d",fecha1aux.day(), fecha1aux.month(), fecha1aux.year());
    m_fechainicial1->setText(cadena2);

    s1= ffinal;
    s2=s1.mid(0,2);
    dia = atoi(s2.ascii());
    s2=s1.mid(3,2);
    mes = atoi(s2.ascii());
    s2=s1.mid(6,4);
    ano = atoi(s2.ascii());
    fecha1aux.setYMD(ano,mes,dia);
    cadena2.sprintf("%2.2d/%2.2d/%4.4d",fecha1aux.day(), fecha1aux.month(), fecha1aux.year());
    m_fechafinal1->setText(cadena2);
}// end inicializa1


/** \brief SLOT que responde a la pulsación del botón de imprimir.
 * Muestra el formulario de impresión de diario y lo ejecuta \ref DiarioPrintView
 */
void diarioview1::boton_imprimir() {
    DiarioPrintView *print = new DiarioPrintView(empresaactual,0,0);
    print->setFiltro(filt);
    print->inicializa1(m_fechainicial1->text(), m_fechafinal1->text());
    print->exec();
}// end boton_imprimir


/**************************************************************
 * Se ha pulsado sobre el botón guardar del formulario
 **************************************************************/
void diarioview1::boton_guardar() {
    QString fn = Q3FileDialog::getSaveFileName(confpr->valor(CONF_DIR_USER), "Diarios (*.txt)", 0,"Guardar Libro Diario","Elige el nombre de archivo");
    if (!fn.isEmpty()) {
        // Si se ha proporcionado un nombre de archivo valido
        // invocamos la clase diarioprint y hacemos que guarde el archivo.
        diarioprint diariop(empresaactual);
        QString finicial = m_fechainicial1->text();
        QString ffinal = m_fechafinal1->text();
        diariop.inicializa(conexionbase);
        diariop.inicializa1((char *) finicial.ascii(), (char *)ffinal.ascii());
        diariop.inicializa2((char *) fn.ascii());
        diariop.accept();
    }// end if
}// end boton_guardar


/**************************************************************
 * Se ha pulsado sobre el boton aceptar del formulario
 **************************************************************/
void diarioview1::accept() {
    presentar();
}// end diarioview

// Si el parametro pasado es un:
// 0 -> del dia actual
// 1 -> del mes actual
// 2 -> del año actual
void diarioview1::boton_extracto1(int tipo) {
    QDate fecha1, fecha2, fechaact;
    if(!m_listado->text(m_listado->currentRow(),2).isEmpty()) {
        fechaact = normalizafecha(m_listado->text(m_listado->currentRow(), COL_FECHA));
        switch(tipo) {
        case 0:
            fecha1.setYMD(fechaact.year(), fechaact.month(),fechaact.day());
            fecha2.setYMD(fechaact.year(), fechaact.month(), fechaact.day());
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
        extracto->inicializa1(m_listado->text(m_listado->currentRow(),COL_CUENTA),m_listado->text(m_listado->currentRow(),COL_CUENTA), fecha1.toString("dd/MM/yyyy"), fecha2.toString("dd/MM/yyyy"), 0);
    }// end if
    extracto->accept();
    empresaactual->libromayor();
}// end boton_extracto1


// Si el parametro pasado es un:
// 0 -> del periodo actual.
// 1 -> del mes actual mirado a partir de la fecha de inicio.
// 2 -> del año actual mirado a partir de la fecha de inicio.
void diarioview1::boton_balance1(int tipo) {
    QDate fecha1, fecha2, fechaact, fechaact1;
    if(!m_fechainicial1->text().isEmpty()) {
        fechaact = normalizafecha(m_fechainicial1->text());
        fechaact1 = normalizafecha(m_fechafinal1->text());
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
        balance->inicializa1(m_listado->text(m_listado->currentRow(),COL_CUENTA), m_listado->text(m_listado->currentRow(),COL_CUENTA), fecha1.toString("dd/MM/yyyy"), fecha2.toString("dd/MM/yyyy"), 0);
    }// end if
    balance->accept();
    empresaactual->librobalance();
}// end boton_balance1


void diarioview1::boton_asiento() {
    if (m_listado->currentRow()!= -1) {
        QString text = m_listado->text(m_listado->currentRow(),COL_NUMASIENTO);
        int numasiento = atoi((char *)text.ascii());
        if (numasiento != 0) {
            introapunts->muestraasiento(numasiento);
        }// end if
    }// end if
    empresaactual->muestraapuntes1();
}// end if


void diarioview1::presentar() {
    float debe, haber;
    float totaldebe1, totalhaber1;
    int idcuenta;
    int idasiento;
    QString fecha;
    QString fechaasiento;
    QString descripcion;
    QString concepto;
    QString codigocuenta;
    QString nomcuenta;
    QString cadaux;
    cursor2 *cursorcoste;
    cursor2 *cursorcanal;
    cursor2 *cursoraux1;

    QString finicial = m_fechainicial1->text();
    QString ffinal = m_fechafinal1->text();
    QString cad;
    QString query;
    m_listado->setNumRows(0);
    totaldebe1=totalhaber1=0;
    cursor2 *cursoraux;

    // Consideraciones para centros de coste y canales
    selectcanalview *scanal=empresaactual->getselcanales();
    selectccosteview *scoste=empresaactual->getselccostes();
    QString ccostes = scoste->cadcoste();
    if (ccostes != "")
        ccostes.sprintf(" AND idc_coste IN (%s) ", ccostes.ascii());
    QString ccanales = scanal->cadcanal();
    if (ccanales != "")
        ccanales.sprintf(" AND idcanal IN (%s) ", ccanales.ascii());

    QString tabla;
    if (filt->m_asAbiertos->isChecked()) {
        tabla= "borrador";
    } else {
        tabla = "apunte";
    }// end if

    // Fin de consideraciones para centros de coste y canales

    query= "SELECT asiento.ordenasiento, "+tabla+".contrapartida, "+tabla+".fecha, asiento.fecha AS fechaasiento,cuenta.tipocuenta, cuenta.descripcion, "+tabla+".conceptocontable,"+tabla+".descripcion AS descapunte, "+tabla+".debe, "+tabla+".haber, cuenta.idcuenta, asiento.idasiento, "+tabla+".idc_coste, "+tabla+".idcanal, cuenta.codigo FROM asiento, "+tabla+", cuenta WHERE asiento.idasiento="+tabla+".idasiento AND "+tabla+".idcuenta = cuenta.idcuenta AND "+tabla+".fecha >= '"+finicial+"' AND "+tabla+".fecha <= '"+ffinal+"' "+ccostes+" "+ccanales+" ORDER BY "+tabla+".fecha, asiento.idasiento, "+tabla+".orden";

    //   conexionbase->begin();
    cursoraux = conexionbase->cargacursor(query,"cursorapuntes");
    //   conexionbase->commit();
    m_listado->setNumRows(cursoraux->numregistros());
    int i =0;
    int modo=0;
    fprintf(stderr,"Presentar\n");
    int asientoanterior = 0;
    for(;!cursoraux->eof();cursoraux->siguienteregistro()) {
        fechaasiento = cursoraux->valor("fechaasiento");
        int ordenasiento = atoi(cursoraux->valor("ordenasiento").ascii());
        idasiento = atoi(cursoraux->valor("idasiento").ascii());
        // Manejamos el sistema de colorines.
        if (idasiento != asientoanterior) {
            modo = (modo) ? 0 : 1;
            asientoanterior = idasiento;
        }// end if
        fecha = cursoraux->valor("fecha");
        descripcion = cursoraux->valor("conceptocontable");
        concepto = cursoraux->valor("descapunte");
        debe = atof(cursoraux->valor("debe").ascii());
        haber = atof(cursoraux->valor("haber").ascii());
        idcuenta = atoi(cursoraux->valor("idcuenta").ascii());
        totaldebe1+=debe;
        totalhaber1+=haber;
        codigocuenta = cursoraux->valor("codigo");
        nomcuenta = cursoraux->valor("descripcion");
        // Sacamos el centro de coste si existe.
        if (cursoraux->valor("idc_coste") != "") {
            QString query1;
            query1 = "SELECT * FROM c_coste WHERE idc_coste=" + cursoraux->valor("idc_coste");
            cursorcoste=conexionbase->cargacursor(query1,"ccoste");
            if (!cursorcoste->eof()) {
                m_listado->setItem(i,COL_CCOSTE, new QTableItem1(m_listado, Q3TableItem::OnTyping,cadaux, modo));
                m_listado->setText(i,COL_CCOSTE,cursorcoste->valor("nombre"));
            }// end if
            delete cursorcoste;
        }// end if

        // Sacamos el canal si existe
        if (cursoraux->valor("idcanal") != "") {
            QString query2 = "SELECT * FROM canal WHERE idcanal=" + cursoraux->valor("idcanal");
            cursorcanal=conexionbase->cargacursor(query2,"canal");
            if (!cursorcanal->eof()) {
                m_listado->setItem(i,COL_CANAL, new QTableItem1(m_listado, Q3TableItem::OnTyping,cadaux, modo));
                m_listado->setText(i,COL_CANAL,cursorcanal->valor("nombre"));
            }// end if
            delete cursorcanal;
        }// end if

        // Sacamos la contrapartida si existe.
        if (cursoraux->valor("contrapartida") != "") {
            QString querycontrapartida;
            querycontrapartida.sprintf("SELECT codigo FROM cuenta WHERE idcuenta=%s",cursoraux->valor("contrapartida").ascii());
            cursoraux1=conexionbase->cargacursor(querycontrapartida,"contrapartida");
            if (!cursoraux1->eof()) {
                m_listado->setItem(i,COL_CONTRAPARTIDA, new QTableItem1(m_listado, Q3TableItem::OnTyping,cadaux, modo));
                m_listado->setText(i,COL_CONTRAPARTIDA,cursoraux1->valor("codigo"));
            }// end if
            delete cursoraux1;
        }// end if

        // Insercion de m_listados.
        cadaux.sprintf("%10.10s",fecha.ascii());
        m_listado->setItem(i,0, new QTableItem1(m_listado, Q3TableItem::OnTyping,cadaux, modo));

        cadaux.sprintf("%d",idasiento);
        m_listado->setItem(i,COL_NUMASIENTO, new QTableItem1(m_listado, Q3TableItem::OnTyping,cadaux, modo));
        m_listado->setText(i,COL_NUMASIENTO,cadaux);

        cadaux.sprintf("%d",ordenasiento);
        m_listado->setItem(i,COL_ORDENASIENTO, new QTableItem1(m_listado, Q3TableItem::OnTyping,cadaux, modo));
        m_listado->setText(i,COL_ORDENASIENTO,cadaux);

        QTableItem1 *item =   new QTableItem1(m_listado, Q3TableItem::OnTyping ,cadaux, modo);
        m_listado->setItem(i,2, item);
        m_listado->setText(i,2,codigocuenta);
        // Ponemos los iconos para que la cosa parezca mas guay.
        if (cursoraux->valor("tipocuenta") == "1")
            m_listado->setPixmap(i,2, QPixmap(cactivo));
        else if (cursoraux->valor("tipocuenta") == "2")
            m_listado->setPixmap(i,2, QPixmap(cpasivo));
        else if (cursoraux->valor("tipocuenta") == "3")
            m_listado->setPixmap(i,2, QPixmap(cneto));
        else if (cursoraux->valor("tipocuenta") == "4")
            m_listado->setPixmap(i,2, QPixmap(cingresos));
        else if (cursoraux->valor("tipocuenta") == "5")
            m_listado->setPixmap(i,2, QPixmap(cgastos));

        m_listado->setItem(i,3, new QTableItem1(m_listado, Q3TableItem::OnTyping,cadaux, modo));
        m_listado->setText(i,3,nomcuenta);

        cadaux.sprintf("%5.2f",debe);
        m_listado->setItem(i,4, new QTableItem1(m_listado, Q3TableItem::OnTyping,cadaux, modo));
        m_listado->setText(i,4,cadaux);

        cadaux.sprintf("%5.2f",haber);
        m_listado->setItem(i,5, new QTableItem1(m_listado, Q3TableItem::OnTyping,cadaux, modo));
        m_listado->setText(i,5,cadaux);

        m_listado->setItem(i,6, new QTableItem1(m_listado, Q3TableItem::OnTyping,cadaux, modo));
        m_listado->setText(i,6,descripcion);
        i++;
    }// end for
    delete cursoraux;
    cadaux.sprintf("%5.2f",totaldebe1);
    totaldebe->setText(cadaux);
    cadaux.sprintf("%5.2f",totalhaber1);
    totalhaber->setText(cadaux);
}// end presentar



void diarioview1::contextmenu(int , int , const QPoint &poin) {
    Q3PopupMenu *popup;
    int opcion;
    popup = new Q3PopupMenu;
    popup->insertItem("Mostrar Apunte",0);
    popup->insertSeparator();
    popup->insertItem("Ver Extracto (Este día)",111);
    popup->insertItem("Ver Extracto (Este mes)",113);
    popup->insertItem("Ver Extracto (Este año)",114);
    popup->insertSeparator();
    popup->insertItem("Ver Balance (Este día)",121);
    popup->insertItem("Ver Balance (Este mes)",123);
    popup->insertItem("Ver Balance (Este año)",124);
    opcion = popup->exec(poin);
    switch(opcion) {
    case 0:
        boton_asiento();
        break;
    case 111:
        boton_extracto1(0);
        break;
    case 113:
        boton_extracto1(1);
        break;
    case 114:
        boton_extracto1(2);
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
}// end contextmenu

void diarioview1::boton_filtrar() {
    filt->exec();
    accept();
}// end boton_filtrar


// Cuando se ha pulsado una tecla sobre la fecha del extracto
// Se evalua si la pulsación es un código de control o es un digitos
// Para la introducción de fechas.
void diarioview1::textChanged(const QString &texto) {
    QLineEdit *fecha = (QLineEdit *) sender();
    if (texto=="+") {
        Q3PtrList<QDate> a;
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


