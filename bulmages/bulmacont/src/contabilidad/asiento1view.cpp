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
#include "duplicarasientoview.h"
#include "aplinteligentesview.h"
#include "listlinasiento1view.h"
#include "empresa.h"



/*
/// Usar esta macro es peligroso ya que el cursor puede estar vacio.
/// es preferible usar la función idAsiento() que hace la comprobación.
#define IDASIENTO cursorasientos->valor("idasiento").ascii()
#define QS_IDASIENTO cursorasientos->valor("idasiento")
#define ORDENASIENTO cursorasientos->valor("ordenasiento").ascii()
#define ROWACTUAL tapunts3->currentRow()
*/

/** \brief Constructor de la clase, inicializa los componentes
  * \param emp empresa que llama al objeto 
  * \param parent widget padre de éste
  * \param nam Nombre que recibe el widget o ventana
  */
Asiento1View::Asiento1View(empresa *emp,QWidget *parent, const char *name, int  ) : QWidget (parent,name) ,  Asiento1(emp) , ListAsientos(emp) {
    setupUi(this);
    _depura("Constructor de Asiento1View\n",0);
    m_companyact = emp;

    mui_list->setcompany(m_companyact);
    setListLinAsiento1( mui_list );

    /// Hacemos la carga del listado de asientos.
    cargaasientos();
    /// Desplazamos hasta el último asiento.
    boton_fin();
    _depura("FIN del Constructor de Asiento1View\n",0);
}

/** \brief Destructor de la clase
  *
  * Destruye los objetos creados y libera la memoria
  */
Asiento1View::~Asiento1View() {}

void Asiento1View::calculaypintatotales(QString idasiento) {
    m_totaldebe->setText(totaldebe(idasiento).toQString());
    m_totalhaber->setText(totalhaber(idasiento).toQString());
    Fixed desc = totaldebe(idasiento) - totalhaber(idasiento);
    m_descuadre->setText(desc.toQString());
}

void Asiento1View::trataestadoAsiento1() {
    _depura("Asiento1View::trataestadoAsiento1",0);
    if (estadoAsiento1() == ASCerrado) {
        asientocerradop();
    } else {
        asientoabiertop();
    }// end if
    _depura("END Asiento1View::trataestadoAsiento1",0);
}

/** \brief Pone la pantalla en el modo de asiento abierto
  * Activa los botones de cierre y pone los elementos como estan configurados
  */
void Asiento1View::asientoabiertop() {
    _depura("Asiento1View::asientoabiertop",0);
    m_descuadre->setEnabled(TRUE);
    mui_abrirasiento->setEnabled(FALSE);
    mui_cerrarasiento->setEnabled(TRUE);
    botonborrarasiento->setEnabled(TRUE);
    botoniva->setEnabled(TRUE);
    botoninteligente->setEnabled(TRUE);
}

/** \brief Pone la pantalla en el modo de asiento cerrado
  * Activa los botones de apertura y pone los elementos como estan configurados
  */
void Asiento1View::asientocerradop() {
    _depura("Asiento1View::asientocerradop",0);
    mui_abrirasiento->setEnabled(TRUE);
    mui_cerrarasiento->setEnabled(FALSE);
    botonborrarasiento->setEnabled(TRUE);
    botoniva->setEnabled(FALSE);
    botoninteligente->setEnabled(TRUE);
}

/**
 * Esta funcion se activa cuando se pulsa sobre el boton nuevo asiento del
 * formulario
 */
void Asiento1View::boton_nuevoasiento() {
    m_fecha->setText(QDate::currentDate().toString("dd/MM/yyyy"));
    iniciar_asiento_nuevo();
}

/**
 * Esta función se encarga de hacer las inicializaciones en un asiento nuevo
 */
void Asiento1View::iniciar_asiento_nuevo() {
    int idasiento = m_companyact->nuevoasiento(m_fecha->text(), m_fecha->text(),0,1);
    if (idasiento <= 0) {
        _depura("No se pudo crear el asiento",2);
        return;
    }// end if
    cargaasientos();
    muestraasiento(idasiento);
    abreAsiento1();
    mui_list->setDBTableName(QString::number(idasiento));
}

void Asiento1View::eturn_fechaasiento() {
    QString query;
    if (estadoAsiento1() != Asiento1::ASCerrado) { //cambiar la fecha del asiento
        setDBvalue("fecha",m_fecha->text());
        guardar();
    } else {
        iniciar_asiento_nuevo();
    }
}

/** \brief Se ha pulsado sobre el botón de duplicar asiento
  *
  * Inicializa el dialogo de duplicación de asientos y lo presenta.
  * Cuando se ha terminado carga el cursor de presentación y repinta el asiento
  * para que actualize los cambios
  */
void Asiento1View::boton_duplicarasiento() {
    duplicarasientoview *dupli= new duplicarasientoview(m_companyact,0,"",true);
    // Establecemos los parametros para el nuevo asiento a duplicar
    dupli->inicializa(idasiento(), idasiento());
    dupli->exec();
    cargaasientos();
    boton_fin();
    delete dupli;
}



/** Se ha pulsado sobre el botón de generar asientos inteligentes. Se inicializa la clase \ref aplinteligentesview y se muestra ese diálogo para que se opere con los asientos plantilla
*/
void Asiento1View::boton_inteligente() {
    int numasiento;
    if (estadoasiento() != Asiento1::ASCerrado) {
        // El asiento esta abierto y por tanto se muestra como abierto
        asientoabiertop();
        numasiento = idasiento().toInt();
    } else {
        numasiento = 0;
    }// end if
    aplinteligentesview *nueva=new aplinteligentesview(m_companyact, 0,"");
    nueva->inicializa(numasiento);
    nueva->exec();
    delete nueva;
}

/**
  * Se ha pulsado sobre el boton de cargar asiento con lo
  * que debemos comprobar que el numero introducido es correcto
  * y hacer las gestiones oportunas para mostrar el asiento en
  * pantalla o crearlo si hace falta.
  */
void Asiento1View::boton_cargarasiento() {
    _depura("Asiento1View::boton_cargarasiento",2);
    QString idas="";
    QString query = "SELECT idasiento FROM asiento WHERE ordenasiento = "+m_ordenasiento->text()+" ORDER BY ordenasiento DESC";
    cursor2 *curs = m_companyact->cargacursor(query);
    if (!curs->eof()) {
        idas = curs->valor("idasiento");
        cargar(idas);
    } else {
        _depura("Asiento inexistente",2);
        pintaAsiento1();
    }// end if
    delete curs;
    _depura("END Asiento1View::boton_cargarasiento",2);
}

/**
  * Prepara para guardar.
*/
void Asiento1View::prepguardar() {
	setDBvalue("fecha", m_fecha->text());
	setDBvalue("ordenasiento", m_ordenasiento->text());
	setDBvalue("comentariosasiento", mui_comentariosAsiento->text());
	setDBvalue("clase", QString::number(mui_claseAsiento->currentIndex()));
}



/**************************************************************************************************************************
***************************************************************************************************************************
		IMPLEMENTACION DE LISTASIENTOS
***************************************************************************************************************************/

ListAsientos::ListAsientos(empresa *emp) {
    m_companyact = emp;
    cursorasientos = NULL;
    /// Creamos el objeto de filtrado de asientos para que el filtro funcione siempre bien desde esta ventana.
    filt = new filtrarasientosview(m_companyact,0,"");

}

ListAsientos::~ListAsientos() {
    delete filt;
    if (cursorasientos != NULL ) {
        delete cursorasientos;
    }// end if
}

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

    cantapunt = m_companyact->sanearCadena(filt->cantidadapunte->text());
    saldototal = m_companyact->sanearCadena(filt->saldoasiento->text());
    nombreasiento = m_companyact->sanearCadena(filt->nombreasiento->text());
    ejercicio = m_companyact->sanearCadena(filt->ejercicio());


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

    cursorasientos = m_companyact->cargacursor(query);
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
    _depura("ListAsientos::situarasiento "+numasiento,0);
    cursorasientos->primerregistro();
    while (cursorasientos->valor("idasiento") != numasiento && !cursorasientos->esultimoregistro())
        cursorasientos->siguienteregistro();
    _depura("END ListAsientos::situarasiento",0);
}

