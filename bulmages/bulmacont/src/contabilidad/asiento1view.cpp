/***************************************************************************
                          intapunts3view.cpp  -  description
                             -------------------
    begin                : mar may 27 2006
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
#include <q3popupmenu.h>
#include <qlineedit.h>
#include <q3listview.h>
#include <qtoolbutton.h>
#include <qpushbutton.h>
//Added by qt3to4:
#include <QHBoxLayout>
#include <stdlib.h>
#include <qinputdialog.h>
#include <qcolor.h>
#include <qfont.h>
#include <qstringlist.h>
#include <qlayout.h>
#include <qlabel.h>

#include "asiento1view.h"
#include "busquedafecha.h"
#include "asientoview.h"

#include "listlinasiento1view.h"

#define COL_FECHA       0
#define COL_CODIGO   1
#define COL_CONCEPTO    6
#define COL_DEBE        4
#define COL_HABER       5
#define COL_NOMCUENTA   2
#define COL_CONTRAPARTIDA 3
#define COL_IDBORRADOR    7
#define COL_TIPOIVA       8
#define COL_IVA           9
#define COL_NOMBREC_COSTE       10
#define COL_NOMBRECANAL        11
#define COL_IDCUENTA     12
#define COL_IDCONTRAPARTIDA 13
#define COL_IDCANAL         14
#define COL_IDCCOSTE        15
#define COL_ORDEN	    16

/*
/// Usar esta macro es peligroso ya que el cursor puede estar vacio.
/// es preferible usar la función idAsiento() que hace la comprobación.
#define IDASIENTO cursorasientos->valor("idasiento").ascii()
#define QS_IDASIENTO cursorasientos->valor("idasiento")
#define ORDENASIENTO cursorasientos->valor("ordenasiento").ascii()
#define ROWACTUAL tapunts3->currentRow()
*/

/// Define el numero de filas que va a tener la tabla de apuntes.
#define TAPUNTS_NUM_ROWS 10000

/** \brief Constructor de la clase, inicializa los componentes
  * \param emp empresa que llama al objeto 
  * \param parent widget padre de éste
  * \param nam Nombre que recibe el widget o ventana
  */
Asiento1View::Asiento1View(empresa *emp,QWidget *parent, const char *name, int  ) : Asiento1Dlg (parent,name) ,  Asiento1(emp) , ListAsientos(emp) {
    _depura("Constructor de Asiento1View\n",0);
    companyact = emp;
    setListLinAsiento1(subform2);
    /// Hacemos la carga del listado de asientos.
    cargaasientos();
    /// Desplazamos hasta el último asiento.
    boton_fin();
    _depura("FIN del Constructor de Asiento1View\n",0);
}// end intapunts3view


/** \brief Destructor de la clase
  *
  * Destruye los objetos creados y libera la memoria
  */
Asiento1View::~Asiento1View() {
}// end intapunts3view

void Asiento1View::calculaypintatotales() {
    m_totaldebe->setText(totaldebe().toQString());
    m_totalhaber->setText(totalhaber().toQString());
    Fixed desc = totaldebe() - totalhaber();
    m_descuadre->setText(desc.toQString());
}// end calculaypintatotales


void Asiento1View::trataestadoAsiento1() {
    _depura("Asiento1View::trataestadoAsiento1",0);
    if (estadoAsiento1() == ASCerrado) {
        asientocerradop();
    } else {
        asientoabiertop();
    }// end if
    _depura("END Asiento1View::trataestadoAsiento1",0);
}// end calculaypintatotales



/** \brief Pone la pantalla en el modo de asiento abierto
  * Activa los botones de cierre y pone los elementos como estan configurados
  */
void Asiento1View::asientoabiertop() {
    _depura("Asiento1View::asientoabiertop",0);
    m_descuadre->setEnabled(TRUE);
    nuevoasiento->setEnabled(TRUE);
    botonabrirasiento->setEnabled(FALSE);
    botoncerrarasiento->setEnabled(TRUE);
    subform2->setPaletteBackgroundColor(confpr->valor(CONF_BG_APUNTESA));
    subform2->setPaletteForegroundColor(confpr->valor(CONF_FG_APUNTESA));
    subform2->setReadOnly(FALSE);
    botonborrarasiento->setEnabled(TRUE);
    botoniva->setEnabled(TRUE);
    botoninteligente->setEnabled(TRUE);
}// end asientoabiertop


/** \brief Pone la pantalla en el modo de asiento cerrado
  * Activa los botones de apertura y pone los elementos como estan configurados
  */
void Asiento1View::asientocerradop() {
    _depura("Asiento1View::asientocerradop",0);
    m_descuadre->setEnabled(TRUE);
    nuevoasiento->setEnabled(TRUE);
    botonabrirasiento->setEnabled(TRUE);
    botoncerrarasiento->setEnabled(FALSE);
    subform2->setPaletteBackgroundColor(confpr->valor(CONF_BG_APUNTES));
    subform2->setPaletteForegroundColor(confpr->valor(CONF_FG_APUNTES));
    subform2->setReadOnly(TRUE);
    botonborrarasiento->setEnabled(TRUE);
    botoniva->setEnabled(FALSE);
    botoninteligente->setEnabled(TRUE);
}// end asientocerradop




/**
 * Esta funcion se activa cuando se pulsa sobre el boton nuevo asiento del
 * formulario
 */
void Asiento1View::boton_nuevoasiento() {
    m_fecha->setText(QDate::currentDate().toString("dd/MM/yyyy"));
    iniciar_asiento_nuevo();
}// end boton_nuevoasiento

/**
 * Esta función se encarga de hacer las inicializaciones en un asiento nuevo
 */
void Asiento1View::iniciar_asiento_nuevo() {
    asientoview *nuevoasiento = new asientoview(companyact);
    nuevoasiento->inicializa(companyact);
    int idasiento = nuevoasiento->creaasiento( m_fecha->text(), m_fecha->text(),0,1);
    delete nuevoasiento;
    cargaasientos();
    muestraasiento(idasiento);
    boton_abrirasiento();
    subform2->iniciar_asiento_nuevo(m_fecha->text());
}// end iniciar_asiento_nuevo


/**************************************************************************************************************************
***************************************************************************************************************************
		IMPLEMENTACION DE LISTASIENTOS
***************************************************************************************************************************/

ListAsientos::ListAsientos(empresa *emp) {
    companyact = emp;
    cursorasientos = NULL;
    /// Creamos el objeto de filtrado de asientos para que el filtro funcione siempre bien desde esta ventana.
    filt = new filtrarasientosview(companyact,0,"");

}// end ListAsientos

ListAsientos::~ListAsientos() {
    delete filt;
    if (cursorasientos != NULL ) {
        delete cursorasientos;
    }// end if
}// end ~ListAsientos

/** \brief Prepara el cursor que sirve para recorrer los asientos uno a uno.
 * Carga el cursor que sirve para hacer todo *
 * el recorrido a través de los asientos.                 *
 * numasiento: 0 indica el primer asiento
 *            -1 indica el último asiento.
           otros indica el asiento o el inmediatamente más bajo
 
  Esta función no hace cambios en la presentación, solo realiza una
  carga del cursor que sirve para recorrer los asientos.
 */
void ListAsientos::cargaasientos() {
    _depura("ListAsientos::cargaasientos",0);
    QString cantapunt ="";
    QString saldototal = "";
    QString nombreasiento = "";
    QString query;
    QString cadwhere;
    QString textsaldototal = "";
    QString textcantapunt = "";
    QString textnombreasiento= "";
    QString textejercicio="";
    QString ejercicio = "";

    cantapunt = companyact->sanearCadena(filt->cantidadapunte->text());
    saldototal = companyact->sanearCadena(filt->saldoasiento->text());
    nombreasiento = companyact->sanearCadena(filt->nombreasiento->text());
    ejercicio = companyact->sanearCadena(filt->ejercicio());


    if (cursorasientos != NULL ) {
        delete cursorasientos;
    }// end if

    int pand=0; /// Indica si se tiene que agregar el AND o no en el select
    if (saldototal != "") {
        cadwhere = " WHERE ";
        textsaldototal = " idasiento IN (SELECT idasiento FROM (SELECT idasiento, sum(debe) AS total from apunte GROUP BY idasiento) AS foo WHERE foo.total="+saldototal+")";
        pand = 1;
    }// end if
    if (cantapunt != "" ) {
        cadwhere = " WHERE ";
        if (pand)
            textcantapunt = " AND ";
        textcantapunt += " idasiento IN (SELECT idasiento FROM apunte where debe="+cantapunt+" OR haber = "+cantapunt+")";
        pand = 1;
    }// end if
    if (nombreasiento != "") {
        cadwhere = " WHERE ";
        if (pand)
            textnombreasiento = " AND ";
        textnombreasiento += " idasiento in (SELECT idasiento FROM apunte WHERE conceptocontable LIKE '%"+nombreasiento+"%' )";
        pand = 1;
    }// end if


    /// Los ejercicios los pondremos como filtraje de la introducción de asientos
    if (ejercicio != "--") {
        if (pand)
            textejercicio = " AND EXTRACT(YEAR FROM fecha)='"+ ejercicio +"'";
        else
            textejercicio = " WHERE EXTRACT(YEAR FROM fecha)='"+ ejercicio +"'";
    }// end if

    /// Se ordenan los asientos por año y por numero de orden.
    query = "SELECT * FROM asiento "+cadwhere+textsaldototal+textcantapunt+textnombreasiento+textejercicio+" ORDER BY EXTRACT (YEAR FROM fecha), ordenasiento";
    //   }// end if

    cursorasientos = companyact->cargacursor(query);
    if (cursorasientos->eof()) {
        QMessageBox::warning(0, "No existe asiento", "No existe ningun asiento para mostrar.", "Cerrar",0,0,0);
        return;
    }// end if
    _depura("End cargaasientos\n",0);
}// end cargaasientos





/** \brief Slot que responde a la pulsación del botón de inicio
  *
  * Comprueba que existen registros en el cursor de asientos a mostrar y si existen se desplaza al primer registro y lo muestra.
  * En el caso de que no haya asiento a mostrar vacia la pantalla para que no salga basura.
  */
void ListAsientos::boton_inicio() {
    if (cursorasientos->numregistros() != 0) {
        cursorasientos->primerregistro();
        pintaasiento(cursorasientos->valor("idasiento"));
    }// end if
}


/** \brief Slot que responde a la pulsación del botón de fin
  *
  * Comprueba que existen registros en el cursor de asientos a mostrar y si existen se desplaza al ultimo registro y lo muestra.
  * En el caso de que no haya asiento a mostrar vacia la pantalla para que no salga basura.
  */
void ListAsientos::boton_fin() {
    if (cursorasientos->numregistros() != 0 ) {
        cursorasientos->ultimoregistro();
        pintaasiento(cursorasientos->valor("idasiento"));
    }// end if
}

/** \brief Slot que responde a la pulsación del botón de siguiente registro
  *
  * Comprueba que existen registros en el cursor de asientos a mostrar y si existen se desplaza al siguiente registro y lo muestra.
  * En el caso de que no haya asiento a mostrar vacia la pantalla para que no salga basura.
  */
void ListAsientos::boton_siguiente() {
    ///  Si no hay nada que mostrar vacia la pantalla para que no queden resto.
    if (cursorasientos->numregistros() == 0) {
        return;
    }// end if
    if (!cursorasientos->esultimoregistro()) {
        cursorasientos->siguienteregistro();
        pintaasiento(cursorasientos->valor("idasiento"));
    }// end if
}

/** \brief Slot que responde a la pulsación del botón de anterior registro
  *
  * Comprueba que existen registros en el cursor de asientos a mostrar y si existen se desplaza al registro anterior y lo muestra.
  * En el caso de que no haya asiento a mostrar vacia la pantalla para que no salga basura.
  */
void ListAsientos::boton_anterior() {
    ///  Si no hay nada que mostrar vacia la pantalla para que no queden resto.
    if (cursorasientos->numregistros() == 0) {
        return;
    }// end if
    if (!cursorasientos->esprimerregistro()) {
        cursorasientos->registroanterior();
        pintaasiento(cursorasientos->valor("idasiento"));
    }// end if
}


void ListAsientos::situarasiento(QString numasiento) {
	_depura("ListAsientos::situarasiento"+numasiento,0);
    cursorasientos->primerregistro();
    while (cursorasientos->valor("idasiento") != numasiento && !cursorasientos->esultimoregistro())
        cursorasientos->siguienteregistro();
	_depura("ListAsientos::situarasiento"+cursorasientos->valor("idasiento"),0);
}// end situarasiento

