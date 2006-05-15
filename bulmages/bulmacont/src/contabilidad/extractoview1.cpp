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
/** \file extractiview1.cpp
  * Este archivo contiene la implementación de la clase extractoview1 que saca el extracto por pantalla de
  * una o varias cuentas determinadas. Esta clase es una de las tres principales junto a \ref introapuntes1 y 
  * \ref diarioview1
  * \author Tomeu Borrás Riera
  */

#include <qwidget.h>
#include <qlineedit.h>
#include <q3datetimeedit.h>
#include <qwidget.h>
#include <q3table.h>
#include <qtoolbutton.h>
#include <qlayout.h>
#include <q3popupmenu.h>

#include "extractoview1.h"
#include "extractoprintview.h"
#include "libromayorprint.h"
#include "calendario.h"
#include "empresa.h"
#include <q3filedialog.h>
#include <qcheckbox.h>
#include "asiento1view.h"
#include "balanceview.h"
#include "diarioview.h"
#include "listcuentasview1.h"

#include "busquedafecha.h"
#include "busquedacuenta.h"

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


extractoview1::extractoview1(empresa * emp,QWidget *parent, const char *name, int  ) : extractodlg1(parent,name) {
    fprintf(stderr,"Constructor de extractoview1\n");
    companyact = emp;
    numdigitos = companyact->numdigitosempresa();
    m_codigoinicial->setempresa(emp);
    m_codigofinal->setempresa(emp);
    m_codigoinicial->hideNombre();
    m_codigofinal->hideNombre();

    listado->setNumRows( 0 );
    listado->setNumCols( 0 );
    listado->setSelectionMode( Q3Table::SingleRow );
    listado->setSorting( TRUE );
    listado->setSelectionMode( Q3Table::SingleRow );
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
    listado->setPaletteBackgroundColor(confpr->valor(CONF_BG_EXTRACTO).ascii());

    // Hacemos la conexion de señales, para la petición de menu contextual y para el punteo.
    connect( listado, SIGNAL( contextMenuRequested(int,int,const QPoint&) ), this, SLOT( contextmenu(int,int,const QPoint&) ) );
    connect( listado, SIGNAL( valueChanged(int,int) ), this, SLOT( apuntecambiadogrid(int,int) ) );


    // Iniciamos los componentes de la fecha para que al principio aparezcan
    // Como el año inicial.
    QString cadena;
    cadena.sprintf("%2.2d/%2.2d/%4.4d",1, 1, QDate::currentDate().year());
    m_fechainicial1->setText(cadena);
    cadena.sprintf("%2.2d/%2.2d/%4.4d",31, 12, QDate::currentDate().year());
    m_fechafinal1->setText(cadena);
    cursorcta=NULL;

    // Inicializamos la ventana de filtrado
    filt = new filtrarextractosview(companyact,0,0);
    fprintf(stderr,"fin del constructor de extractoview1\n");
}// end estractoview1



extractoview1::~extractoview1() {
    delete cursorcta;
    delete filt;
}// end ~extractoview1


/** \brief Esta función carga el cursor de cuentas que forman el todo por el todo.
  * También sera la encargada de recoger la información de filtración para que
  * todo sea correcto.
  */
void extractoview1::accept() {
    fprintf(stderr,"accept inicializado\n");
    QString codinicial= m_codigoinicial->codigocuenta();
    QString codfinal = m_codigofinal->codigocuenta();
    QString query;
    /// Si los datos de codigo inicial y final estan vacios los ponemos nosotros.
    if (codinicial == "")
        codinicial ="0";
    if (codfinal == "")
        codfinal = "9999999";
    query = "SELECT * FROM cuenta WHERE idcuenta IN (SELECT idcuenta FROM apunte) AND codigo >='"+codinicial+"' AND codigo <= '"+codfinal+"'  ORDER BY codigo";

    if (cursorcta != NULL) {
        delete cursorcta;
    }// end if
    cursorcta = companyact->cargacursor(query,"cursorcuenta");
    presentar();
}// end extractoview1


void extractoview1::boton_asiento() {
    if (listado->currentRow()!= -1) {
        QString text = listado->text(listado->currentRow(),NUM_ASIENTO);
        int numasiento = atoi((char *)text.ascii());
        if (numasiento != 0) {
            companyact->intapuntsempresa()->muestraasiento(numasiento);
        }// end if
    }// end if
    companyact->muestraapuntes1();
}// end if


// Si el parametro pasado es un:
// 0 -> del día actual
// 1 -> del mes actual
// 2 -> del año actual
void extractoview1::boton_diario1(int tipo) {
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
        companyact->diarioempresa()->inicializa1((char *) fecha1.toString("dd/MM/yyyy").ascii(),(char *) fecha2.toString("dd/MM/yyyy").ascii(), 0);
    }// end if
    companyact->diarioempresa()->accept();
    companyact->librodiario();
}// end boton_diario1


// Si el parametro pasado es un:
// 0 -> del periodo actual.
// 1 -> del mes actual mirado a partir de la fecha de inicio.
// 2 -> del año actual mirado a partir de la fecha de inicio.
void extractoview1::boton_balance1(int tipo) {
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
        companyact->balanceempresa()->inicializa1(m_codigoinicial->text(),m_codigofinal->text(), fecha1.toString("dd/MM/yyyy"), fecha2.toString("dd/MM/yyyy"), 0);
    }// end if
    companyact->balanceempresa()->accept();
    companyact->librobalance();
}// end boton_balance1


/***************************************************************
 * Esta es la funcion que avanza un registro entre las cuentas *
 ***************************************************************/
void extractoview1::boton_siguiente() {
    if (cursorcta != NULL) {
        if (!cursorcta->esultimoregistro()) {
            cursorcta->siguienteregistro();
            presentar();
        }// end if
    }// end if
}// end boton_siguiente


/******************************************************************
 * Esta es la funcion que retrocede un registro entre las cuentas *
 ******************************************************************/
void extractoview1::boton_anterior() {
    if (cursorcta != NULL) {
        if (!cursorcta->esprimerregistro()) {
            cursorcta->registroanterior();
            presentar();
        }// end if
    }// end if
}// end boton_anterior


void extractoview1::boton_inicio() {
    if (cursorcta != NULL) {
        cursorcta->primerregistro();
        presentar();
    }// end if
}// end if


void extractoview1::boton_fin() {
    if (cursorcta != NULL) {
        cursorcta->ultimoregistro();
        presentar();
    }// end if
}// end if


void extractoview1::boton_imprimir() {
    ExtractoPrintView *print = new ExtractoPrintView(companyact, 0,0);
    print->inicializa1(m_fechainicial1->text(), m_fechafinal1->text(), m_codigoinicial->text(), m_codigofinal->text());
    print->setFiltro(filt);
    print->exec();
}// end if


void extractoview1::boton_guardar() {
    QString fn = Q3FileDialog::getSaveFileName(confpr->valor(CONF_DIR_USER), tr("Diarios (*.txt)"), 0,tr("Guardar Libro Diario"),tr("Elige el nombre de archivo"));
    if (!fn.isEmpty()) {
        libromayorprint libromayor;
        QString finicial = m_fechainicial1->text().ascii();
        QString ffinal = m_fechafinal1->text().ascii();
        libromayor.inicializa(companyact);
        libromayor.inicializa1(m_codigoinicial->text(), m_codigofinal->text(), finicial, ffinal);
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


void extractoview1::ajustes() {}// end ajustes


/** \brief Esta función se encarga de montar el query que va a hacer la consulta en la base de datos
  */
void extractoview1::presentar() {
    fprintf(stderr,"Presentar\n");
    int j,num1;
    float debe, haber,saldo;
    float debeinicial=0, haberinicial=0, saldoinicial=0;
    float debefinal, haberfinal, saldofinal;
    int idcuenta;
    int idasiento;
    QString finicial = m_fechainicial1->text();
    QString ffinal = m_fechafinal1->text();
    QString contra = filt->codigocontrapartida();

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
        selectcanalview *scanal=companyact->getselcanales();
        selectccosteview *scoste=companyact->getselccostes();
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

        query="SELECT * FROM "+tabla+", asiento where asiento.idasiento = "+tabla+".idasiento AND  idcuenta="+QString::number(idcuenta)+" AND "+tabla+".fecha>='"+finicial+"' AND "+tabla+".fecha<='"+ffinal+"' "+ccostes+" "+ccanales+" "+tipopunteo+" ORDER BY "+tabla+".fecha, ordenasiento, orden";

        fprintf(stderr,"%s\n",query.ascii());

        cursorapt=companyact->cargacursor(query,"cargasaldoscuentafecha");

        num1 = cursorapt->numregistros();
        if (!cursorapt->eof()) {
            fprintf(stderr,"Existen apuntes por tanto pintamos algo\n");

            // Cargamos los saldos iniciales.
            cursor2* cursoraux;

            query.sprintf("SELECT sum(debe) AS tdebe, sum(haber) AS thaber FROM apunte WHERE idcuenta=%d AND fecha <'%s'",idcuenta, finicial.ascii());
            cursoraux = companyact->cargacursor(query, "saldos_iniciales");

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


                // Aquí vamos a poner el tema del Punteo, por lo menos la presentación.
                Q3CheckTableItem *check = new Q3CheckTableItem(listado,0);
                listado->setItem(j,PUNTEO,check);
                if (cursorapt->valor("punteo")== "t") {
                    check->setChecked(TRUE);
                }// end if



                debe=atof(cursorapt->valor("debe").ascii());
                haber=atof(cursorapt->valor("haber").ascii());
                saldo += debe - haber;
                debefinal += debe;
                haberfinal += haber;
                cad = cursorapt->valor("fecha");
                // Sacamos el centro de coste
                QString query;
                int ccoste = atoi(cursorapt->valor("idc_coste").ascii());
                query.sprintf( "SELECT * FROM c_coste WHERE idc_coste=%d",ccoste);

                cursorcoste=companyact->cargacursor(query,"ccoste");

                if (!cursorcoste->eof()) {
                    cadaux.sprintf("%s",cursorcoste->valor("nombre").ascii());
                    listado->setText(j,C_COSTE,cadaux);
                }// end if
                delete cursorcoste;

                // Sacamos el canal
                int canal = atoi(cursorapt->valor("idcanal").ascii());
                query.sprintf( "SELECT * FROM canal WHERE idcanal=%d",canal);

                cursorcanal=companyact->cargacursor(query,"canal");

                if (!cursorcanal->eof()) {
                    cadaux.sprintf("%s",cursorcanal->valor("nombre").ascii());
                    listado->setText(j,CANAL,cadaux);
                }// end if
                delete cursorcanal;

                // Sacamos la contrapartida Solo en el caso de que no sea a partir de borrador
                if (!filt->m_asAbiertos->isChecked()) {
                    query.sprintf("SELECT codigo FROM cuenta WHERE idcuenta=%s",cursorapt->valor("contrapartida").ascii());
                    fprintf(stderr,"%s\n",query.ascii());

                    cursoraux1=companyact->cargacursor(query,"contrapartida");

                    if (!cursoraux1->eof()) {
                        listado->setText(j,CONTRAPARTIDA,cursoraux1->valor("codigo"));
                    }// end if
                    delete cursoraux1;
                }// end if



                // Esta parte es la que inserta en la tabla
                listado->setText(j,FECHA,cad.mid(0,10));
                cadaux.sprintf("%d",idasiento);
                listado->setText(j,NUM_ASIENTO,cadaux);
                cadaux.sprintf("%d",ordenasiento);
                listado->setText(j,ORDEN_ASIENTO,cadaux);

                listado->setText(j,CONCEPTO, cursorapt->valor("conceptocontable"));
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



void extractoview1::inicializa1(QString codinicial, QString codfinal, QString fecha1, QString fecha2, int ) {
    m_codigoinicial->setText(codinicial);
    m_codigofinal->setText(codfinal);
    m_fechainicial1->setText(normalizafecha(fecha1).toString("dd/MM/yyyy"));
    m_fechafinal1->setText(normalizafecha(fecha2).toString("dd/MM/yyyy"));
}// end inicializa1



void extractoview1::contextmenu(int row, int col, const QPoint &poin) {
    // Si el asiento esta cerrado el menu a mostrar es diferente
    Q3PopupMenu *popup = new Q3PopupMenu;
    popup->insertItem(tr("Ver Asiento"), 0);
    popup->insertSeparator();
    popup->insertItem(tr("Ver Diario (Este día)"),101);
    popup->insertItem(tr("Ver Diario (Este mes)"),103);
    popup->insertItem(tr("Ver Diario (Este año)"),104);
    popup->insertSeparator();
    popup->insertItem(tr("Ver Balance (Este día)"),121);
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
    Q3CheckTableItem *check =(Q3CheckTableItem *) listado->item(row,col);
    QString query;
    if (check->isChecked()) {
        query.sprintf("UPDATE APUNTE SET punteo = TRUE WHERE idapunte=%s",listado->text(row,IDAPUNTE).ascii());
    } else {
        query.sprintf("UPDATE APUNTE SET punteo = FALSE WHERE idapunte=%s",listado->text(row,IDAPUNTE).ascii());
    }// end if
    companyact->begin();
    companyact->ejecuta(query.ascii());
    companyact->commit();
}// end if

void extractoview1::boton_filtrar() {
    filt->exec();
    accept();
}// end boton_filtrar

void extractoview1::boton_casacion() {
    QString query;
    query.sprintf("SELECT * FROM apunte WHERE punteo=FALSE AND haber <>0  AND idcuenta=%s ORDER BY fecha",cursorcta->valor("idcuenta").ascii());
    companyact->begin();
    cursor2 *curshaber = companyact->cargacursor(query, "curshaber");
    companyact->commit();
    while (!curshaber->eof()) {
        query.sprintf("SELECT * FROM apunte WHERE punteo=FALSE AND debe = %s AND idcuenta = %s ORDER BY fecha", curshaber->valor("haber").ascii(), cursorcta->valor("idcuenta").ascii());
        companyact->begin();
        cursor2 *cursdebe = companyact->cargacursor(query.ascii(), "cursdebe");
        companyact->commit();
        if (!cursdebe->eof()) {
            query.sprintf("UPDATE apunte set punteo=TRUE WHERE idapunte=%s",curshaber->valor("idapunte").ascii());
            companyact->begin();
            companyact->ejecuta(query);
            query.sprintf("UPDATE apunte SET punteo=TRUE WHERE idapunte=%s",cursdebe->valor("idapunte").ascii());
            companyact->ejecuta(query);
            companyact->commit();
        }// end if
        delete cursdebe;
        curshaber->siguienteregistro();
    }// end while
    delete curshaber;
    presentar();
}// end boton_casacion

void extractoview1::boton_guardarpunteo() {
    QString fn = Q3FileDialog::getSaveFileName(confpr->valor(CONF_DIR_USER), tr("Punteos (*.pto)"), 0,tr("Guardar Punteo"),tr("Elige el nombre de archivo"));
    if (!fn.isEmpty()) {
        FILE *mifile;
        mifile = fopen((char *) fn.ascii(),"wt");
        if (mifile != NULL) {
            QString query;
            query = "SELECT * FROM apunte WHERE punteo=TRUE";
            cursor2 *cursp = companyact->cargacursor(query,"punteos");
            while (!cursp->eof()) {
                fprintf(mifile,"%s\n", cursp->valor("idapunte").ascii());
                cursp->siguienteregistro();
            }// end while
            delete cursp;
            fclose(mifile);
        }// end if
    }// end if
}// end boton_guardarpunteo



/** \brief Esta función borra todo el punteo de un extracto y lo pone a cero
  * Esta función se activa justo cuando se pulsa sobre el botón de resetear el punteo.
  * Por supuesto cuando se pulsa dicho boton se borra el punteo.
  */
void extractoview1::boton_borrapunteo() {
    int valor = QMessageBox::warning( 0, tr("Borrar Punteo"), "Se dispone a borrar el punteo. Este cambio es irrecuperable si no ha guardado su el punte. Desea continuar?", QMessageBox::Yes, QMessageBox::No);
    if (valor == QMessageBox::Yes) {
        companyact->begin();
        companyact->ejecuta("UPDATE apunte SET punteo=FALSE");
        companyact->commit();
        presentar();
    }// end if
}// end boton_cargarpunteo


/** \brief Carga los punteos desde un fichero de texto que se solicita mediante un QFileDialog
  * El fichero de punteos requiere que no se hayan modificado los identificadores de borrador.
  * Para ello es preciso que no se hayan abierto y cerrado los asientos correspondientes ya que en dicho caso
  * La carga del punteo no funciona correctamente.
  */
void extractoview1::boton_cargarpunteos() {
    QString fn = Q3FileDialog::getOpenFileName(confpr->valor(CONF_DIR_USER), tr("Punteos (*.pto)"), 0,tr("Cargar Punteo"),tr("Elige el nombre de archivo"));
    if (!fn.isEmpty()) {
        ifstream filestr((char *) fn.ascii());
        string a;
        companyact->begin();
        companyact->ejecuta("UPDATE apunte SET punteo=FALSE");
        companyact->commit();
        while (filestr.good()) {
            filestr >> a;
            QString query;
            query.sprintf("UPDATE apunte SET punteo=TRUE WHERE idapunte=%s",a.c_str());
            companyact->begin();
            companyact->ejecuta(query);
            companyact->commit();
        }// end while
        filestr.close();
    }// end if
    presentar();
}// end boton_cargarpunteo


