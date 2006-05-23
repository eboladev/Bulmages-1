/***************************************************************************
                          DiarioView.cpp  -  description
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
#include "diarioview.h"
#include "funcaux.h"
#include "diarioprintview.h"
#include "diarioprint.h"
#include "configuracion.h"
#include "calendario.h"

#include "filtrardiarioview.h"
#include "asiento1view.h"
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

DiarioView::DiarioView(empresa *emp, QWidget *parent, const char *name, int  ) : QWidget(parent,name) {
	setupUi(this);

    m_companyact = emp;
	mui_list->setcompany( emp);





    // Iniciamos los componentes de la fecha para que al principio aparezcan
    // Como el año inicial.
    char cadena[10];
    sprintf(cadena,"%2.2d/%2.2d/%4.4d",1, 1, QDate::currentDate().year());
    m_fechainicial1->setText(cadena);
    sprintf(cadena,"%2.2d/%2.2d/%4.4d",31, 12, QDate::currentDate().year());
    m_fechafinal1->setText(cadena);

    //Inicializamos el filtro
    filt = new filtrardiarioview(m_companyact,0,0);
}// end DiarioView

DiarioView::~DiarioView() {
    delete filt;
}// end ~DiarioView


void DiarioView::inicializa1(QString finicial, QString ffinal, int ) {
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
void DiarioView::boton_imprimir() {
    DiarioPrintView *print = new DiarioPrintView(m_companyact,0,0);
    print->setFiltro(filt);
    print->inicializa1(m_fechainicial1->text(), m_fechafinal1->text());
    print->exec();
}// end boton_imprimir


/**************************************************************
 * Se ha pulsado sobre el botón guardar del formulario
 **************************************************************/
void DiarioView::boton_guardar() {
    QString fn = Q3FileDialog::getSaveFileName(confpr->valor(CONF_DIR_USER), "Diarios (*.txt)", 0,"Guardar Libro Diario","Elige el nombre de archivo");
    if (!fn.isEmpty()) {
        // Si se ha proporcionado un nombre de archivo valido
        // invocamos la clase diarioprint y hacemos que guarde el archivo.
        diarioprint diariop(m_companyact);
        QString finicial = m_fechainicial1->text();
        QString ffinal = m_fechafinal1->text();
        diariop.inicializa(m_companyact);
        diariop.inicializa1((char *) finicial.ascii(), (char *)ffinal.ascii());
        diariop.inicializa2((char *) fn.ascii());
        diariop.accept();
    }// end if
}// end boton_guardar


/**************************************************************
 * Se ha pulsado sobre el boton aceptar del formulario
 **************************************************************/
void DiarioView::accept() {
    presentar();
}// end diarioview


/**
  * Hacemos la carga de mui_list
  * Hacemos la consulta a la base de datos y presentamos el listado.
  */
void DiarioView::presenta() {
    _depura("DiarioView::presenta",0);
    cursor2 * cur= m_companyact->cargacursor("SELECT * FROM borrador LEFT JOIN cuenta ON borrador.idcuenta = cuenta.idcuenta");
    mui_list->cargar(cur);
    delete cur;

     cur = m_companyact->cargacursor("SELECT sum(debe) as totaldebe, sum(haber) as totalhaber from borrador");
     if(! cur->eof() ) {
	totaldebe->setText(cur->valor("totaldebe"));
	totalhaber->setText(cur->valor("totalhaber"));
     }// end if
     delete cur;
    _depura("END DiarioView::presenta",0);
}

void DiarioView::presentar() {
    presenta();
}




void DiarioView::boton_filtrar() {
    filt->exec();
    accept();
}// end boton_filtrar


