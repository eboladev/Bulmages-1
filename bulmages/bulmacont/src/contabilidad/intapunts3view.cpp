/***************************************************************************
                          intapunts3view.cpp  -  description
                             -------------------
    begin                : mar may 27 2003
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

#include "intapunts3view.h"
#include "asientoview.h"
#include "calendario.h"
#include "ivaview.h"
#include "aplinteligentesview.h"
#include "listcuentasview1.h"
#include "cuentaview.h"
#include "duplicarasientoview.h"
#include "selectccosteview.h"
#include "selectcanalview.h"

#include "busquedafecha.h"

#define COL_FECHA       0
#define COL_SUBCUENTA   1
#define COL_CONCEPTO    6
#define COL_DEBE        4
#define COL_HABER       5
#define COL_NOMCUENTA   2
#define COL_CONTRAPARTIDA 3
#define COL_IDBORRADOR    7
#define COL_TIPOIVA       8
#define COL_IVA           9
#define COL_CCOSTE       10
#define COL_CANAL        11
#define COL_IDCUENTA     12
#define COL_IDCONTRAPARTIDA 13
#define COL_IDCANAL         14
#define COL_IDCCOSTE        15
#define COL_ORDEN	    16


/// Usar esta macro es peligroso ya que el cursor puede estar vacio.
/// es preferible usar la función idAsiento() que hace la comprobación.
#define IDASIENTO cursorasientos->valor("idasiento").ascii()
#define QS_IDASIENTO cursorasientos->valor("idasiento")
#define ORDENASIENTO cursorasientos->valor("ordenasiento").ascii()
#define ROWACTUAL tapunts3->currentRow()

/// Define el número de filas que va a tener la tabla de apuntes.
#define TAPUNTS_NUM_ROWS 10000

/** \brief Devuelve en un QString el valor del asiento que se está visualizando
  *
  *  Si no hay ningún asiento viendose devuelve "-1"
  */
QString intapunts3view::idAsiento() {
    if (cursorasientos->eof())
        return "-1";
    return IDASIENTO;
}

/** \brief Constructor de la clase, inicializa los componentes
  * \param emp empresa que llama al objeto 
  * \param parent widget padre de éste
  * \param nam Nombre que recibe el widget o ventana
  */
intapunts3view::intapunts3view(empresa *emp,QWidget *parent, const char *name, int  ) : intapunts3dlg(parent,name) {
    fprintf(stderr,"Constructor de intapunts3view\n");
    companyact = emp;
    idasiento=-1;
    QFont tapunts_font(  tapunts3->font() );

    tapunts_font.setPointSize(atoi(confpr->valor(CONF_FONTSIZE_APUNTES).ascii()));
    tapunts_font.setFamily(confpr->valor(CONF_FONTFAMILY_APUNTES).ascii());
    tapunts3->setFont( tapunts_font );

    tapunts3->setNumRows( 0 );
    tapunts3->setNumCols( 0 );
    tapunts3->setRowMovingEnabled( TRUE );
    tapunts3->setColumnMovingEnabled( TRUE );
    tapunts3->setSorting( TRUE );
    tapunts3->setSelectionMode( Q3Table::SingleRow );

    tapunts3->setNumCols(17);
    tapunts3->horizontalHeader()->setLabel( COL_FECHA, tr( "FECHA" ) );
    tapunts3->horizontalHeader()->setLabel( COL_SUBCUENTA, tr( "SUBCUENTA" ) );
    tapunts3->horizontalHeader()->setLabel( COL_CONCEPTO, tr( "CONCEPTO" ) );
    tapunts3->horizontalHeader()->setLabel( COL_DEBE, tr( "DEBE" ) );
    tapunts3->horizontalHeader()->setLabel( COL_HABER, tr( "HABER" ) );
    tapunts3->horizontalHeader()->setLabel( COL_NOMCUENTA, tr( "NOMBRE CUENTA" ) );
    tapunts3->horizontalHeader()->setLabel( COL_CONTRAPARTIDA, tr( "CONTRAPARTIDA" ) );
    tapunts3->horizontalHeader()->setLabel( COL_TIPOIVA, tr( "TIPO IVA" ) );
    tapunts3->horizontalHeader()->setLabel( COL_IVA, tr( "FACTURA/FECHA" ) );
    tapunts3->horizontalHeader()->setLabel( COL_CCOSTE, tr( "CCOSTE") );
    tapunts3->horizontalHeader()->setLabel( COL_CANAL, tr( "CANAL") );
    tapunts3->horizontalHeader()->setLabel( COL_IDCCOSTE, tr( "IDCCOSTE") );
    tapunts3->horizontalHeader()->setLabel( COL_IDCANAL, tr( "IDCANAL") );
    tapunts3->horizontalHeader()->setLabel( COL_IDCUENTA, tr( "IDCUENTA") );
    int tamletra = confpr->valor(CONF_FONTSIZE_APUNTES).toInt();
    tapunts3->setColumnWidth(COL_FECHA,tamletra*8);
    tapunts3->setColumnWidth(COL_SUBCUENTA,tamletra*10);
    tapunts3->setColumnWidth(COL_CONCEPTO,tamletra*25);
    tapunts3->setColumnWidth(COL_DEBE,tamletra*8);
    tapunts3->setColumnWidth(COL_HABER,tamletra*8);
    tapunts3->setColumnWidth(COL_NOMCUENTA,tamletra*20);
    tapunts3->setColumnWidth(COL_CONTRAPARTIDA,tamletra*10);
    tapunts3->setColumnWidth(COL_TIPOIVA,tamletra*8);
    tapunts3->setColumnWidth(COL_IVA,tamletra*15);
    tapunts3->setColumnWidth(COL_CCOSTE, tamletra*8);
    tapunts3->setColumnWidth(COL_CANAL, tamletra*8);
    tapunts3->hideColumn(COL_IDBORRADOR);
    tapunts3->hideColumn(COL_TIPOIVA);
    tapunts3->hideColumn(COL_IDCUENTA);
    tapunts3->hideColumn(COL_IDCONTRAPARTIDA);
    tapunts3->hideColumn(COL_IDCANAL);
    tapunts3->hideColumn(COL_IDCCOSTE);
    tapunts3->hideColumn(COL_ORDEN);

    /// Dependiendo de la configuración mostramos o no mostramos la contrapartida, los canales y los centros de coste
    if (confpr->valor(CONF_CONTRAPARTIDA_APUNTES) != "YES")
        tapunts3->hideColumn(COL_CONTRAPARTIDA);
    if (confpr->valor(CONF_CANAL_APUNTES) != "YES")
        tapunts3->hideColumn(COL_CANAL);
    if (confpr->valor(CONF_CCOSTE_APUNTES) != "YES")
        tapunts3->hideColumn(COL_CCOSTE);

    /// Al construir la clase cursorasientos debe valer NULL, indicando que no tiene ningn valor válido.
    cursorasientos = NULL;

    /// Creamos el objeto de filtrado de asientos para que el filtro funcione siempre bien desde esta ventana.
    filt = new filtrarasientosview(companyact,0,"");

    /// Consideramos que no hay row actual
    rowactual = -1;

    /// Creamos el layout para Plugins y le damos los atributos publicos.
    layoutPlugins = new QHBoxLayout( 0, 0, 6, "LayoutPlugins");
    layoutPlugins->setMargin(0);
    layoutPlugins->setSpacing(0);

/// OJO CON ESTA LINEA QUE LOS PLUGINS NO FUNCIONAN SIN ELLA
//    layout6->addLayout( layoutPlugins);


    /// Calculamos el nmero de dígitos que gastan las cuentas
    numdigitos = companyact->numdigitosempresa();

    /// Indicamos que el asiento por defecto esta cerrado.
    abierto = 0;
    /// Vamos a cargar el cursor de los asientos
    cargarcursor();
    /// Nos desplazamos hasta el final
    boton_fin();

    fprintf(stderr,"FIN del Constructor de intapunts3view\n");
}// end intapunts3view


/** \brief Destructor de la clase
  *
  * Destruye los objetos creados y libera la memoria
  */
intapunts3view::~intapunts3view() {
    delete filt;
    if (cursorasientos != NULL ) {
        delete cursorasientos;
    }// end if
}// end intapunts3view




/** \brief Prepara el cursor que sirve para recorrer los asientos uno a uno.
 * Carga el cursor que sirve para hacer todo *
 * el recorrido a través de los asientos.                 *
 * numasiento: 0 indica el primer asiento
 *            -1 indica el último asiento.
           otros indica el asiento o el inmediatamente más bajo
 
  Esta función no hace cambios en la presentación, solo realiza una
  carga del cursor que sirve para recorrer los asientos.
 */
void intapunts3view::cargarcursor() {
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

    fprintf(stderr," Inicio de la carga del cursor\n");

    cantapunt = companyact->sanearCadena(filt->cantidadapunte->text());
    saldototal = companyact->sanearCadena(filt->saldoasiento->text());
    nombreasiento = companyact->sanearCadena(filt->nombreasiento->text());
    ejercicio = companyact->sanearCadena(filt->ejercicio());

    fprintf(stderr," Inicio de la perdición del cursorasientos\n");

    if (cursorasientos != NULL ) {
        delete cursorasientos;
    }// end if

    fprintf(stderr," Inicio de creación del query\n");

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

    fprintf(stderr," Carga del cursor\n");
    cursorasientos = companyact->cargacursor(query,"cursorasientos");
    if (cursorasientos->eof()) {
        QMessageBox::warning(0, tr("No existe asiento"), tr("No existe ningun asiento para mostrar."), tr("Cerrar"),0,0,0);
        return;
    }// end if
    fprintf(stderr,"Fin del cargar cursor\n");
}// end cargarcursor


/** \brief Slot que responde a la pulsación del botón de inicio
  *
  * Comprueba que existen registros en el cursor de asientos a mostrar y si existen se desplaza al primer registro y lo muestra.
  * En el caso de que no haya asiento a mostrar vacia la pantalla para que no salga basura.
  */
void intapunts3view::boton_inicio() {
    if (cursorasientos->numregistros() != 0) {
        cursorasientos->primerregistro();
        muestraasiento(atoi(cursorasientos->valor("idasiento").ascii()));
    } else {
        vaciarapuntes();
    }// end if
}


/** \brief Slot que responde a la pulsación del botón de fin
  *
  * Comprueba que existen registros en el cursor de asientos a mostrar y si existen se desplaza al ultimo registro y lo muestra.
  * En el caso de que no haya asiento a mostrar vacia la pantalla para que no salga basura.
  */
void intapunts3view::boton_fin() {
    if (cursorasientos->numregistros() != 0 ) {
        cursorasientos->ultimoregistro();
        muestraasiento(atoi(cursorasientos->valor("idasiento").ascii()));
    } else {
        vaciarapuntes();
    }// end if
}

/** \brief Slot que responde a la pulsación del botón de siguiente registro
  *
  * Comprueba que existen registros en el cursor de asientos a mostrar y si existen se desplaza al siguiente registro y lo muestra.
  * En el caso de que no haya asiento a mostrar vacia la pantalla para que no salga basura.
  */
void intapunts3view::boton_siguiente() {
    ///  Si no hay nada que mostrar vacia la pantalla para que no queden resto.
    if (cursorasientos->numregistros() == 0) {
        vaciarapuntes();
        return;
    }// end if
    if (!cursorasientos->esultimoregistro()) {
        cursorasientos->siguienteregistro();
        muestraasiento(atoi(cursorasientos->valor("idasiento").ascii()));
    }// end if
}

/** \brief Slot que responde a la pulsación del botón de anterior registro
  *
  * Comprueba que existen registros en el cursor de asientos a mostrar y si existen se desplaza al registro anterior y lo muestra.
  * En el caso de que no haya asiento a mostrar vacia la pantalla para que no salga basura.
  */
void intapunts3view::boton_anterior() {
    ///  Si no hay nada que mostrar vacia la pantalla para que no queden resto.
    if (cursorasientos->numregistros() == 0) {
        vaciarapuntes();
        return;
    }// end if
    if (!cursorasientos->esprimerregistro()) {
        cursorasientos->registroanterior();
        muestraasiento(atoi(cursorasientos->valor("idasiento").ascii()));
    }// end if
}



/**
 * Esta funcion se llama desde remoto para obligar a la ventana
 * a que nos muestre un asiento determinado.
 
   Se desplaza a travé del cursor de asientos hasta encontrar el
   asiento que se desea. Si no se encuentra en el cursor se situa al final
   del cursor y muestra el asiento (siempre que exista).
 */
void intapunts3view::muestraasiento(int numasiento) {
    cursor2 *cursoraux, *cursoraux1;
    QString fechaasiento2;
    QString cadena;
    int num,num2;
    if (abierto) {
        int valor = QMessageBox::warning( 0, tr("Cerrar Asiento"), tr("El asiento actual esta abierto, debe cerrarlo antes de continuar, ¿Cerrar?."), tr("Cerrar"), tr("Dejar Abierto"),0,0,1);
        if (valor == 0) {
            boton_cerrarasiento();
            /// Si el asiento se mantiene abierto, entonces abortamos la función.
            if (abierto) {
                return;
            }// end if
        } else {
            guardaborrador(rowactual);
        }// end if
    }// end if


    cursorasientos->primerregistro();
    if (cursorasientos->eof()) {
        QMessageBox::warning(0,tr("No hay asiento"), tr("No hay asiento para mostrar"),"OK",0,0);
        return;
    }//
    while (idAsiento().toInt() != numasiento && !cursorasientos->esultimoregistro())
        cursorasientos->siguienteregistro();

    vaciarapuntes();
    companyact->begin();
    cursoraux= companyact->cargaasiento(numasiento);
    num = cursoraux->numregistros();
    if (num == 0) {
        // No existe el numero de asiento que se ha introducido, por tanto lo avisamos y salimos.
        int valor = QMessageBox::warning( 0, "No existe asiento", "No existe un asiento con el número que ha proporcionado, ¿desea crear uno?.", QMessageBox::Yes, QMessageBox::No);
        if (valor == QMessageBox::Yes) {
            asientoview *nuevoasiento = new asientoview(companyact);
            nuevoasiento->inicializa(companyact);
            int nasiento = nuevoasiento->creaasiento(
                               m_fecha->fecha(), m_fecha->fecha(),numasiento,1);
            delete nuevoasiento;
            cargarcursor();
            muestraasiento(nasiento);
            boton_abrirasiento();
            return;
        } else {
            cadena.sprintf("%s",ORDENASIENTO);
            idasiento1->setText(cadena);
            return;
        }// end if
    } else {
        fechaasiento2 = cursoraux->valor(2);
    }// end if
    delete cursoraux;
    cursoraux=companyact->cargaapuntes(numasiento);
    num2 = cursoraux->numregistros();
    cursoraux1=companyact->cargaborradores(numasiento);
    num = cursoraux1->numregistros();
    if (num2 != num) {
        // El asiento esta abierto y por tanto se muestra como abierto
        asientoabiertop();
    } else {
        asientocerradop();
    }// end if
    companyact->commit();
    delete cursoraux1;
    delete cursoraux;
    // Como ya hemos hecho los ajustes para que lo que se
    // muestre sea correcto, llamamos a la funcionde repintar.
    repinta(numasiento);
    rowactual  =-1;
}// end muestraasiento


/**
 * Esta funcion se llama desde remoto para obligar a la ventana
 * a que nos muestre un asiento determinado.
    
    Lo busca directamente en la base de datos, no el el cursor 
    'cursorasientos'. Útil para visualizar asientos de ejercicios
    anteriores.
 */
void intapunts3view::flashAsiento(int numasiento) {
    QString query;
    query.sprintf("SELECT * FROM asiento WHERE idasiento='%d'",numasiento);
    vaciarapuntes();
    companyact->begin();
    cursor2 * recordSet = companyact->cargacursor(query,"recordSet1");
    companyact->commit();
    if (!recordSet->eof())
        repinta(numasiento);
}


/**
 * Esta funcion se llama desde remoto para obligar a la ventana
 * a que nos muestre un asiento determinado.
 */
void intapunts3view::repinta(int numasiento) {
    cursor2 *cursorasiento, *cursoraux, *cursoriva;
    QString cadena, ordenAsiento, query, fechaasiento2;
    int num, i;

    companyact->begin();
    query.sprintf("SELECT * FROM asiento where idasiento=%d",numasiento);
    cursoraux = companyact->cargacursor(query,"uncursor");
    query.sprintf("SELECT borrador.debe, borrador.haber, borrador.conceptocontable, borrador.idborrador, borrador.idc_coste, borrador.idcanal,cuenta.codigo,cuenta.idcuenta, cuenta.descripcion AS desccuenta, borrador.fecha AS borrfecha , borrador.contrapartida AS contrapartida, borrador.orden AS orden FROM  borrador, cuenta WHERE borrador.idasiento=%d AND cuenta.idcuenta=borrador.idcuenta ORDER BY orden",numasiento);
    cursorasiento = companyact->cargacursor(query,"micursor");
    companyact->commit();
    num = cursoraux->numregistros();
    if (num ==0) {
        // No existe el numero de apunte que se ha introducido, por tanto lo avisamos y salimos.
        QMessageBox::warning( 0, "No existe asiento", "No existe un asiento con el número que ha proporcionado, ¿desea crear uno?.", QMessageBox::Yes, QMessageBox::No);
        return;
    } else {
        ordenAsiento = cursoraux->valor("ordenasiento");
        fechaasiento2 = cursoraux->valor("fecha");
    }// end if
    num=cursorasiento->numregistros();
    // Hacemos que no haya ningn apunte como preestablecido.
    tapunts3->setNumRows(0);
    tapunts3->setNumRows(TAPUNTS_NUM_ROWS);
    i=0;
    while (!cursorasiento->eof()) {
        cadena.sprintf("%10.10s",cursorasiento->valor("borrfecha").ascii());
        tapunts3->setText(i,COL_FECHA,cadena);
        tapunts3->setText(i,COL_NOMCUENTA,cursorasiento->valor("desccuenta"));
        tapunts3->setText(i,COL_IDCUENTA,cursorasiento->valor("idcuenta"));
        tapunts3->setText(i,COL_SUBCUENTA,cursorasiento->valor("codigo"));
        cadena.sprintf("%2.2f",atof(cursorasiento->valor("debe").ascii()));
        tapunts3->setText(i,COL_DEBE,cadena);
        cadena.sprintf("%2.2f",atof(cursorasiento->valor("haber").ascii()));
        tapunts3->setText(i,COL_HABER,cadena);
        tapunts3->setText(i,COL_CONCEPTO,cursorasiento->valor("conceptocontable"));
        tapunts3->setText(i,COL_IDBORRADOR,cursorasiento->valor("idborrador"));
        tapunts3->setText(i,COL_ORDEN,cursorasiento->valor("orden"));
        // Vamos a mirar si existe registro de iva para este apunte y lo anotamos.
        query.sprintf("SELECT * FROM registroiva WHERE  idborrador=%s",cursorasiento->valor("idborrador").ascii());
        companyact->begin();
        cursoriva = companyact->cargacursor(query,"cursoriva0");
        companyact->commit();
        if (!cursoriva->eof()) {
            tapunts3->setText(i,COL_IVA,"FRA. "+cursoriva->valor("factura")+" De: "+cursoriva->valor("ffactura"));
        }// end if
        delete cursoriva;

        // Carga del centro de coste.
        if (cursorasiento->valor("idc_coste") != "") {
            tapunts3->setText(i, COL_IDCCOSTE, cursorasiento->valor("idc_coste"));
            query.sprintf("SELECT nombre FROM c_coste WHERE idc_coste=%s",cursorasiento->valor("idc_coste").ascii());
            companyact->begin();
            cursoriva = companyact->cargacursor(query,"cursorccoste");
            companyact->commit();
            if (!cursoriva->eof()) {
                tapunts3->setText(i,COL_CCOSTE,cursoriva->valor("nombre"));
            }// end if
            delete cursoriva;
        }// end if

        // Carga del centro del canal
        if (cursorasiento->valor("idcanal") != "") {
            tapunts3->setText(i,COL_IDCANAL, cursorasiento->valor("idcanal"));
            query.sprintf("SELECT nombre FROM canal WHERE idcanal=%s",cursorasiento->valor("idcanal").ascii());
            companyact->begin();
            cursoriva = companyact->cargacursor(query,"cursorcanal");
            companyact->commit();
            if (!cursoriva->eof()) {
                tapunts3->setText(i,COL_CANAL,cursoriva->valor("nombre"));
            }// end if
            delete cursoriva;
        }// end if


        // Vamos a cargar la contrapartida, que aunque no aparece hay que ponerla para que se pueda usar.
        cadena=cursorasiento->valor("contrapartida");
        if (cadena.stripWhiteSpace() !="") {
            query.sprintf("SELECT * FROM cuenta WHERE idcuenta=%s",cadena.ascii());
            companyact->begin();
            cursor2 *cursorcontrapartida = companyact->cargacursor(query,"cursorcontrapartida");
            companyact->commit();
            if (!cursorcontrapartida->eof()) {
                fprintf(stderr,"%s\n", cursorcontrapartida->valor("codigo").ascii());
                tapunts3->setText(i,COL_CONTRAPARTIDA,cursorcontrapartida->valor("codigo"));
                tapunts3->setText(i,COL_IDCONTRAPARTIDA,cursorcontrapartida->valor("idcuenta"));
            }// end if
            delete cursorcontrapartida;
        }// end if
        // Pasamos al siguiente registro del asiento.
        cursorasiento->siguienteregistro();
        i++;
    }// end while

    idasiento1->setText(ordenAsiento);
    QDate * aux = new QDate();
    aux->setYMD(fechaasiento2.mid(6,4).toInt(),fechaasiento2.mid(3,2).toInt(),fechaasiento2.mid(0,2).toInt());

    m_fecha->setText(aux->toString("dd/MM/yyyy"));
    delete cursorasiento;
    delete cursoraux;
    calculadescuadre();
}// end repinta



/**
 * \brief Esta funcion se encarga de vaciar toda la tabla de apuntes
 * que aparece en el formulario.
 */
void intapunts3view::vaciarapuntes() {
    tapunts3->setNumRows(0);
}// end vaciarapuntes


/** \brief Pone la pantalla en el modo de asiento abierto
  * Activa los botones de cierre y pone los elementos como estan configurados
  */
void intapunts3view::asientoabiertop() {
    descuadre->setEnabled(TRUE);
    nuevoasiento->setEnabled(TRUE);
    botonabrirasiento->setEnabled(FALSE);
    botoncerrarasiento->setEnabled(TRUE);
    tapunts3->setPaletteBackgroundColor(confpr->valor(CONF_BG_APUNTESA).ascii());
    tapunts3->setPaletteForegroundColor(confpr->valor(CONF_FG_APUNTESA).ascii());
    tapunts3->setReadOnly(FALSE);
    botonborrarasiento->setEnabled(TRUE);
    botoniva->setEnabled(TRUE);
    botoninteligente->setEnabled(TRUE);
    abierto = 1;
}// end asientoabiertop


/** \brief Pone la pantalla en el modo de asiento cerrado
  * Activa los botones de apertura y pone los elementos como estan configurados
  */
void intapunts3view::asientocerradop() {
    descuadre->setEnabled(TRUE);
    nuevoasiento->setEnabled(TRUE);
    botonabrirasiento->setEnabled(TRUE);
    botoncerrarasiento->setEnabled(FALSE);
    abierto = 0;
    tapunts3->setPaletteBackgroundColor(confpr->valor(CONF_BG_APUNTES).ascii());
    tapunts3->setPaletteForegroundColor(confpr->valor(CONF_FG_APUNTES).ascii());
    tapunts3->setReadOnly(TRUE);
    botonborrarasiento->setEnabled(TRUE);
    botoniva->setEnabled(FALSE);
    botoninteligente->setEnabled(TRUE);
}// end asientocerradop


/**
 * Esta función se activa cuando se pulsa sobre el boton abrir asiento del
 * formulario
 */
void intapunts3view::boton_abrirasiento() {
    idasiento=idAsiento().toInt();
    if (idasiento == -1) {
        QMessageBox::warning(0, tr("No hay asiento"), tr("No hay ningún asiento"),"OK",0,0);
        return;
    }
    if ((companyact->abreasiento(idasiento))==42501) {
        QMessageBox::warning( 0, tr("PRIVILEGIOS"), tr("No tiene suficientes privilegios para realizar esta acción."), QMessageBox::Yes, 0);
        return;
    }
    asientoabiertop();
}// end boton_abrirasiento


/**
 * Esta función se activa cuando se pulsa sobre el boton cerrar asiento del
 * formulario
*/
void intapunts3view::boton_cerrarasiento() {
    int eleccion;
    guardaborrador(rowactual);
    calculadescuadre();
    if (( descuadre->text() != "0.00") && ( descuadre->text() != "-0.00")) {
        eleccion = QMessageBox::information( 0, "Asiento descuadrado, no se puede cerrar", "No se pueden cerrar asientos descuadrados.");
        return;
    }// end if

    /// Busca si es necesario presentar la ventana de registro de facturas.
    buscaFactura();

    /// Realizamos la operación en la base de datos.
    idasiento = idAsiento().toInt();
    if (idasiento==-1) {
        QMessageBox::warning(0,tr("No hay asiento para cerrar"),tr("No hay ningún asiento para cerrarlo"),"OK",0,0);
        return;
    }// end if
    companyact->begin();
    companyact->cierraasiento(idasiento);
    companyact->commit();
    asientocerradop();

    /// Como pueden haber cambiado cosas, hacemos un repintado para que se actualizen
    repinta(idasiento);
    m_fecha->selectAll();
    m_fecha->setFocus();


    idasiento=-1;
}// end boton_cerrarasiento


/**
  * Buscamos en el asiento si hay indicios de una factura y actuamos en consecuencia.
  */
void intapunts3view::buscaFactura() {
    QString cuentas="";
    QString query = "SELECT valor FROM configuracion WHERE nombre='RegistroEmitida' OR nombre='RegistroSoportada'";
    cursor2 *curvalor = companyact->cargacursor(query);
    while (!curvalor->eof()) {
        cuentas += curvalor->valor("valor")+"%|"; // Preparamos una expresión regular para usar en la consulta
        curvalor->siguienteregistro();
    }// end while
    delete curvalor;
    cuentas.truncate(cuentas.length()-1); // Le quitamos el último '|' que nos sobra

    /// Recorremos la tabla en busca de entradas de factura no introducidas y las preguntamos antes de cerrar nada.
    /// Esta versión se basa en la base de datos pq es mejor ya que así somos más eficaces.
    QString SQLQuery = "SELECT bcontrapartidaborr(idborrador) AS contra FROM borrador LEFT JOIN cuenta ON borrador.idcuenta=cuenta.idcuenta WHERE idasiento="+QString::number(idAsiento().toInt())+" AND codigo SIMILAR TO '"+companyact->sanearCadena(cuentas.ascii())+"' GROUP BY contra";

    cursor2 *cursborr= companyact->cargacursor(SQLQuery);
    while (!cursborr->eof()) {
        int idborrador = cursborr->valor("contra").toInt();
        ivaview *regivaview=new ivaview(companyact,0,"");
        regivaview->inicializa1(idborrador);
        regivaview->exec();
        delete regivaview;
        cursborr->siguienteregistro();
    }// end while
    delete cursborr;
}// end buscaFactura


/**
 * Esta función se activa cuando se pulsa sobre el boton nuevo asiento del
 * formulario
 */
void intapunts3view::boton_nuevoasiento() {
    m_fecha->setText(QDate::currentDate().toString("dd/MM/yyyy"));
    iniciar_asiento_nuevo();
}// end boton_nuevoasiento

/**
 * Esta función se encarga de hacer las inicializaciones en un asiento nuevo
 */
void intapunts3view::iniciar_asiento_nuevo() {
    asientoview *nuevoasiento = new asientoview(companyact);
    nuevoasiento->inicializa(companyact);
    idasiento = nuevoasiento->creaasiento( m_fecha->text(), m_fecha->text(),0,1);
    delete nuevoasiento;
    cargarcursor();
    muestraasiento(idasiento);
    boton_abrirasiento();

    tapunts3->setText(0,0,m_fecha->text());
    /// Comprobamos si existe un centro de coste por defecto y lo usamos
    selectccosteview *selccostes = companyact->getselccostes();
    QString ccoste = QString::number(selccostes->firstccoste());
    if ( ccoste != "0") {
        tapunts3->setText(0,COL_CCOSTE,selccostes->nomcoste());
        tapunts3->setText(0,COL_IDCCOSTE, ccoste);
    }// end if

    /// Comprobamos si existe un canal por defecto y lo usamos
    selectcanalview *selcanales = companyact->getselcanales();
    QString idcanal = QString::number(selcanales->firstcanal());
    if ( idcanal != "0") {
        tapunts3->setText(0,COL_CANAL,selcanales->nomcanal());
        tapunts3->setText(0,COL_IDCANAL, idcanal);
    }// end if
    rowactual=0;
    tapunts3->setCurrentCell(0,0);
    tapunts3->setFocus();
}// end iniciar_asiento_nuevo

/** \brief Muestra un menu contextual con las opciones solicitadas
  */
void intapunts3view::contextmenu(int row, int col, const QPoint &poin) {
    Q3PopupMenu *popup;
    Q3PopupMenu *menucanal = new Q3PopupMenu( this );
    Q3PopupMenu *menucoste = new Q3PopupMenu( this );
    int opcion;
    cursor2 *cur;
    QString num_apuntes;

    if (abierto) {
        // Si el asiento esta abierto mostramos el popup para asientos abiertos
        QString query;
        popup = new Q3PopupMenu;
        popup->insertItem(tr(tr("Igual que la anterior (*)")),4);
        switch (col) {
        case COL_CANAL:
            query = "SELECT * FROM canal";
            companyact->begin();
            cur = companyact->cargacursor(query,"canales");
            menucanal->insertItem(tr("Ninguno"), 1000);
            companyact->commit();
            while (!cur->eof()) {
                menucanal->insertItem(cur->valor("nombre"),1000+atoi(cur->valor("idcanal").ascii()));
                cur->siguienteregistro();
            }// end while
            delete cur;
            popup->insertItem(tr("&Seleccionar Canal"),menucanal);
            break;
        case COL_CCOSTE:
            query = "SELECT * FROM c_coste";
            companyact->begin();
            cur  = companyact->cargacursor(query,"costes");
            menucoste->insertItem(tr("Ninguno"), 1000);
            companyact->commit();
            while (!cur->eof()) {
                menucoste->insertItem(cur->valor("nombre"), 1000+atoi(cur->valor("idc_coste").ascii()));
                cur->siguienteregistro();
            }// end while
            delete cur;
            popup->insertItem(tr("&Seleccionar Centro de Coste"),menucoste);
            break;
        case COL_SUBCUENTA:
        case COL_CONTRAPARTIDA:
        case COL_FECHA:
        case COL_NOMCUENTA:
            popup->insertItem(tr("Seleccionar Valor (+)"),5);
            break;
        case COL_DEBE:
        case COL_HABER:
            popup->insertItem(tr("Introducir Descuadre (+)"),5);
            break;
        }// end switch
        popup->insertSeparator();
        popup->insertItem(tr("Duplicar Apunte"),1);
        popup->insertItem(tr("Borrar Apunte"),6);
        popup->insertItem(tr("Vaciar Asiento"),7);
        popup->insertSeparator();
        popup->insertItem(tr("Subir (Ctrl Arriba)"),2);
        popup->insertItem(tr("Bajar (Ctrl Abajo)"),3);
        opcion = popup->exec(poin);
        delete popup;
        delete menucanal;
        delete menucoste;

        switch(opcion) {
        case 1:
            duplicarapunte();
            break;
        case 2:
            subirapunte(row);
            repinta(idAsiento().toInt());
            tapunts3->setCurrentCell(row-1,col);
            break;
        case 3:
            bajarapunte(row);
            repinta(idAsiento().toInt());
            tapunts3->setCurrentCell(row+1,col);
            break;
        case 4:
            duplicar(col);
            break;
        case 5:
            switch(col) {
            case COL_FECHA: {
                    int dia, mes, ano;
                    Q3PtrList<QDate> a;
                    QString cadena;
                    calendario *cal = new calendario(0,0);
                    cal->exec();
                    a = cal->dn->selectedDates();
                    dia = a.first()->day();
                    mes = a.first()->month();
                    ano = a.first()->year();
                    cadena.sprintf("%2.2d/%2.2d/%d",dia, mes, ano);
                    fprintf(stderr,"Se ha pulsado:%s\n", cadena.ascii());
                    tapunts3->setText(row, COL_FECHA, cadena);
                    delete cal;
                    break;
                }// end case
            case COL_NOMCUENTA:
            case COL_SUBCUENTA:
                // Hacemos aparecer la ventana de cuentas
                tapunts3->setText(row,COL_SUBCUENTA,"");
                cambiadasubcuenta(row);
                break;
            case COL_CONTRAPARTIDA:
                tapunts3->setText(row,COL_CONTRAPARTIDA,"");
                cambiadacontrapartida(row);
                break;
            case COL_DEBE:
                tapunts3->setText(row,COL_DEBE, descuadre->text());
                calculadescuadre();
                break;
            case COL_HABER:
                tapunts3->setText(row,COL_HABER, descuadre->text());
                calculadescuadre();
                break;
            }// end switch
            break;
        case 6:
            borraborrador(row);
            break;
        case 7:
            if(QMessageBox::question(this, tr("Cuidado!"), tr("Seguro que quiere borrar todos los apuntes del asiento?"),tr("&NO"), tr("&SI"), QString::null, 1, 0)) {
                for(; tapunts3->text(0,COL_IDBORRADOR) != ""; borraborrador(0))
                    ;
            }// end if
            break;
        default:
            switch(col) {
            case COL_CANAL:
                if (opcion == 1000) {
                    tapunts3->setText(row, COL_CANAL, "");
                    tapunts3->setText(row, COL_IDCANAL, "");
                }// end if
                if (opcion > 1000) {
                    QString query1;
                    opcion -= 1000;
                    query1.sprintf("SELECT * FROM canal WHERE idcanal=%d", opcion);
                    companyact->begin();
                    cur = companyact->cargacursor(query1.ascii(),"canales1");
                    companyact->commit();
                    if (!cur->eof()) {
                        tapunts3->setText(row,COL_CANAL, cur->valor("nombre"));
                        tapunts3->setText(row,COL_IDCANAL, cur->valor("idcanal"));
                    }// end if
                    delete cur;
                }// end if
                break;
            case COL_CCOSTE:
                if (opcion == 1000) {
                    tapunts3->setText(row, COL_CCOSTE, "");
                    tapunts3->setText(row, COL_IDCCOSTE, "");
                }// end if
                if (opcion > 1000) {
                    QString query1;
                    opcion -= 1000;
                    query1.sprintf("SELECT * FROM c_coste WHERE idc_coste=%d", opcion);
                    companyact->begin();
                    cur = companyact->cargacursor(query1.ascii(),"canales1");
                    companyact->commit();
                    if (!cur->eof()) {
                        tapunts3->setText(row,COL_CCOSTE, cur->valor("nombre"));
                        tapunts3->setText(row,COL_IDCCOSTE, cur->valor("idc_coste"));
                    }// end if
                    delete cur;
                }// end if
                break;
            }// end switch
            break;
        }// end switch
    } else {
        // Si el asiento esta cerrado el menú a mostrar es diferente
        popup = new Q3PopupMenu;
        popup->insertItem(tr("Ver Diario (Este día)"),101);
        popup->insertItem(tr("Ver Diario (Este mes)"),103);
        popup->insertItem(tr("Ver Diario (Este año)"),104);
        popup->insertSeparator();
        popup->insertItem(tr("Ver Extracto (Este día)"),111);
        popup->insertItem(tr("Ver Extracto (Este mes)"),113);
        popup->insertItem(tr("Ver Extracto (Este año)"),114);
        popup->insertSeparator();
        popup->insertItem(tr("Ver Balance (Este día)"),121);
        popup->insertItem(tr("Ver Balance (Este mes)"),123);
        popup->insertItem(tr("Ver Balance (Este año)"),124);
        popup->insertSeparator();
        if (col == COL_NOMCUENTA || col == COL_CONTRAPARTIDA || col == COL_SUBCUENTA) {
            popup->insertItem(tr("Editar Cuenta"),130);
            popup->insertItem(tr("Sustituir Cuenta"), 140);
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
            break;
        case 121:
            boton_balance1(0);
            break;
        case 123:
            boton_balance1(1);
            break;
        case 124:
            boton_balance1(2);
            break;
        case 140:
            /// Aun no esta implementada la sustitución de cuentas desde el menú contextual.
            fprintf(stderr,"Aun no esta implementada la sustitución de cuentas desde el menú contextual\n");
            break;
        case 130:
            // Se ha elegido la opción de editar cuenta.
            // Abrimos la ventana de edición de cuentas.
            QString idcuenta;
            if (col == COL_SUBCUENTA || col == COL_NOMCUENTA)
                idcuenta = tapunts3->text(row,COL_IDCUENTA);
            else
                idcuenta = tapunts3->text(row,COL_IDCONTRAPARTIDA);
            cuentaview *nuevae = new cuentaview(companyact,0,"",true);
            nuevae->cargacuenta(atoi(idcuenta.ascii()));
            nuevae->exec();
            delete nuevae;
            repinta(idAsiento().toInt());
            break;
        }// end switch
        delete popup;
    }// end if
}// end contextmenu


/** \brief SLOT que responde al cambio de casilla en la tabla.
  * Mira que columna es en la que se esta y actua en consecuencia.
  */
void intapunts3view::apuntecambiadogrid(int row, int col) {
    switch(col) {
    case COL_FECHA:
        tapunts3->setText(row,col,normalizafecha(tapunts3->text(row,col)).toString("dd/MM/yyyy"));
        break;
    case COL_SUBCUENTA:
        cambiadasubcuenta(row);
        break;
    case COL_CONTRAPARTIDA:
        cambiadacontrapartida(row);
        break;
    case COL_DEBE:
        cambiadodebe(row);
        break;
    case COL_HABER:
        cambiadohaber(row);
        break;
    case COL_CONCEPTO:
        //         tapunts3->setCurrentCell(row+1,COL_FECHA);
        break;
    }// end switch
}// end apuntecambiadogrid

/** Cuando se cambia de fila en la tabla del apunte se guarda el contenido
  * en la base de datos 
  * Esta función se encarga de determinar si ha habido un cambio de fila y
  * actua en consecuencia guardando y calculando el descuadre
  */
void intapunts3view::tcambiaseleccion() {
    QString codcuenta;
    if (rowactual != tapunts3->currentRow() && abierto) {
        guardaborrador(rowactual);
        calculadescuadre();
    }// end if
    rowactual = tapunts3->currentRow();
}// end tcambiaseleccion


/** \brief Se va a hacer que el contenido de la casilla actual sea igual que el de la anterior.
 
En la introducción de apuntes, al pulsar el * o con el correspondiente menu contextual se consigue el 
efecto del duplicado de contenido basado en la linea anterior.
\sa \ref pulsadomas \ref calculadescuadre
 */
void intapunts3view::duplicar(int col) {
    if (rowactual >0) {
        if (col == COL_SUBCUENTA || col == COL_NOMCUENTA) {
            tapunts3->setText(rowactual,COL_IDCUENTA, tapunts3->text(rowactual-1, COL_IDCUENTA));
            tapunts3->setText(rowactual, COL_NOMCUENTA, tapunts3->text(rowactual-1, COL_NOMCUENTA));
            tapunts3->setText(rowactual, COL_SUBCUENTA, tapunts3->text(rowactual-1, COL_SUBCUENTA));
        } else if (col == COL_CANAL) {
            tapunts3->setText(rowactual, COL_CANAL, tapunts3->text(rowactual-1, COL_CANAL));
            tapunts3->setText(rowactual, COL_IDCANAL, tapunts3->text(rowactual-1, COL_IDCANAL));
        } else if (col == COL_CCOSTE) {
            tapunts3->setText(rowactual, COL_CCOSTE, tapunts3->text(rowactual-1, COL_CCOSTE));
            tapunts3->setText(rowactual, COL_IDCCOSTE, tapunts3->text(rowactual-1, COL_IDCCOSTE));
        } else {
            tapunts3->setText(rowactual,col, tapunts3->text(rowactual-1,col));
        }// end if
    }// end if
}// end duplicar


/** \brief Calcula el desucadre del asiento que se esta viendo
  * Esta función se conecta a la base de datos y calcula los descuadres basandose en la tabla de borradores.
  */
void intapunts3view::calculadescuadre() {
    QString query = "SELECT sum(debe) as tdebe, sum(haber) AS thaber, sum(debe)-sum(haber) AS desc FROM borrador WHERE idasiento="+QS_IDASIENTO;
    companyact->begin();
    cursor2 *cur = companyact->cargacursor(query,"descuadres");
    companyact->commit();
    descuadre->setText(cur->valor("desc"));
    totalhaber1->setText(cur->valor("thaber"));
    totaldebe1->setText(cur->valor("tdebe"));
    delete cur;
}// end calculadescuadre


/** Esta funciono borra el borrador que se le pasa como parámetro
  * El parametro es el row de la tabla que se quiere borrar.
  */
void intapunts3view::borraborrador(int row) {
    // Para borrar un borrador primero tiene que haber un borrador seleccionado
    if (!tapunts3->text(row, COL_IDBORRADOR).isEmpty()) {
        QString idborrador = tapunts3->text(row, COL_IDBORRADOR);
        companyact->begin();
        QString query = "DELETE FROM registroiva WHERE idborrador="+idborrador;
        int error = companyact->ejecuta(query);
        if (error) {
            companyact->rollback();
            return;
        }// end if
        query = "DELETE FROM borrador WHERE idborrador="+idborrador;
        error = companyact->ejecuta(query);
        if (error) {
            companyact->rollback();
            return;
        }// end if
        companyact->commit();
        int rowaux = row;
        while (!tapunts3->text(rowaux,COL_IDBORRADOR).isNull()) {
            for (int i=0; i < tapunts3->numCols(); i++)
                tapunts3->setText(rowaux, i, tapunts3->text(rowaux+1, i));
            error = guardaborrador(rowaux);  // Guardamos el borrador para que la base de datos está en coherencia con esto.
            if (error) {
                return;
            }// end if
            rowaux++;
        }// end while
        calculadescuadre();
    }// end if
}// end borraborrador


/// Esta funcion guarda en la base de datos el borrador de la columna que se ha pasado.
/// Si todo va bien devuelve 0
/// Si hay errores devuelve 1
int intapunts3view::guardaborrador(int row) {
    QString query;
    QString idborrador;
    QString debe;
    QString haber;
    QString idcuenta;
    QString fecha;
    QString concepto;
    QString contrapartida;
    QString idcanal;
    QString idc_coste;
    QString orden;
    int datos=0;
    // Si no hay asiento lo calculamos.
    idasiento = idAsiento().toInt();
    if (idasiento==-1) {
        QMessageBox::warning(0,tr("No hay asiento"), tr("No hay asiento para guardar"),"OK",0,0);
        return 1;
    }// end if

    // Hacemos la recoleccion de datos.
    if ( !tapunts3->text(row, COL_IDBORRADOR).isEmpty()) {
        idborrador = tapunts3->text(row,COL_IDBORRADOR);
        datos = 1;
    }// end if
    if (!tapunts3->text(row, COL_DEBE).isEmpty()) {
        debe = companyact->sanearCadena(tapunts3->text(row, COL_DEBE));
        datos = 1;
    } else {
        debe = "0";
    }// end if
    if (!tapunts3->text(row, COL_HABER).isEmpty()) {
        haber = companyact->sanearCadena(tapunts3->text(row, COL_HABER));
        datos = 1;
    } else {
        haber = "0";
    }// end if
    if (!tapunts3->text(row,COL_IDCUENTA).isEmpty()) {
        idcuenta = tapunts3->text(row, COL_IDCUENTA);
        datos = 1;
    } else {
        idcuenta = "NULL";
    }// end if
    if (!tapunts3->text(row,COL_IDCONTRAPARTIDA).isEmpty() && tapunts3->text(row,COL_IDCONTRAPARTIDA)!="") {
        contrapartida = tapunts3->text(row, COL_IDCONTRAPARTIDA);
        datos = 1;
    } else {
        contrapartida = "NULL";
    }// end if

    if (!tapunts3->text(row,COL_FECHA).isEmpty()) {
        fecha = "'"+ companyact->sanearCadena(tapunts3->text(row, COL_FECHA))+"'";
        datos = 1;
    } else {
        fecha = "NULL";
    }// end if

    if (!tapunts3->text(row,COL_IDCANAL).isEmpty()) {
        idcanal =  tapunts3->text(row, COL_IDCANAL);
        datos = 1;
    } else {
        idcanal = "NULL";
    }// end if

    if (!tapunts3->text(row,COL_IDCCOSTE).isEmpty()) {
        idc_coste =  tapunts3->text(row, COL_IDCCOSTE);
        datos = 1;
    } else {
        idc_coste = "NULL";
    }// end if

    if (!tapunts3->text(row,COL_CONCEPTO).isEmpty()) {
        concepto = "'"+ companyact->sanearCadena(tapunts3->text(row, COL_CONCEPTO))+"'";
        datos = 1;
    } else {
        concepto = "''";
    }// end if

    if(!tapunts3->text(row,COL_ORDEN).isEmpty()) {
        orden = tapunts3->text(row,COL_ORDEN);
    } else {
        orden = QString::number(row+1);
    }// end if


    if (datos) {
        if (idborrador != "") {
            /// El borrador existe, por lo que solo hay que hacer un update
            query = "UPDATE borrador SET orden="+orden+", conceptocontable="+concepto+", fecha="+fecha+", debe="+debe+",haber="+haber+", idcuenta="+idcuenta+", contrapartida="+contrapartida+", idcanal="+idcanal+", idc_coste="+idc_coste+" WHERE idborrador="+idborrador;
            companyact->begin();
            int error = companyact->ejecuta(query);
            if (error) {
                companyact->rollback();
                return 1;
            }// end if
            companyact->commit();
        } else if (idcuenta != "NULL") {
            /// El borrador no existe, por lo que hay que hacer un insert
            query.sprintf("INSERT INTO borrador (orden, conceptocontable, fecha, idcuenta, debe, haber, idasiento, contrapartida, idcanal, idc_coste) VALUES (%s,%s,%s,%s,%s,%s,'%d',%s,%s,%s)",orden.ascii(),concepto.ascii(),fecha.ascii(),idcuenta.ascii(),debe.ascii(),haber.ascii(),idasiento,contrapartida.ascii(),idcanal.ascii(),idc_coste.ascii());
            companyact->begin();
            int error = companyact->ejecuta(query);
            if (error) {
                companyact->rollback();
                return 1;
            }// end if
            query = "SELECT MAX (idborrador) AS id from borrador";
            cursor2 *cur= companyact->cargacursor(query,"cursorm");
            companyact->commit();
            tapunts3->setText(row, COL_IDBORRADOR,cur->valor("id"));
            delete cur;
        }// end if
    }// end if
    /// Ponemos el Saldo de Cuenta sin valor ya que este punto es un buen lugar para hacerlo.
    m_saldoCuenta->setText("");
    return 0;
}// end guardaborrador

/** Duplica un apunte entero, desplaza el contenido hacia abajo e inserta un
  * registro en la tabla de apuntes. Le asigna los valores pertienetes y calcula el descuadre y guarda el apunte
  */
void intapunts3view::duplicarapunte() {
    // Debemos desplazar todos los siguientes para abajo.
    int row = rowactual;
    while (!tapunts3->text(row,COL_IDBORRADOR).isNull())
        row++;
    while ( row > rowactual) {
        for (int i=0; i < tapunts3->numCols(); i++)
            tapunts3->setText(row, i, tapunts3->text(row-1, i));
        row --;
    }// end while
    tapunts3->setText(rowactual+1, COL_IDBORRADOR, "");
    calculadescuadre();
    guardaborrador(rowactual+1);
}// end duplicarapunte


/** \brief SLOT que responde a la pulsación del botón de iva.
  * Crea la clase \ref ivaview y la inicializa con el identificador de borrador para que se presente con los datos ya introducidos.
  * La clase ivaview hace una inserción o una modificación segun exista o no una entrada de iva para dicho borrador.
  */
void intapunts3view::boton_iva() {
    guardaborrador(ROWACTUAL);
    // Si ya hay una entrada de borrador, no vamos a preguntar nada.
    // Directamente vamos a editar dicho registro.
    int idborrador = tapunts3->text(ROWACTUAL,COL_IDBORRADOR).toInt();
    ivaview *nuevae=new ivaview(companyact, 0,"");
    nuevae->inicializa1(idborrador);
    nuevae->exec();
    delete nuevae;
    return;
}// end boton_iva


/** Esta función captura la pulsación de una tecla. No es muy acertado el nombre de pulsadomas
  * Ya que captura cualquier tecla.
  */
void intapunts3view::pulsadomas(int row, int col, int caracter) {
    QString query;
    Q3PopupMenu *menucanal = new Q3PopupMenu( this );
    Q3PopupMenu *menucoste = new Q3PopupMenu( this );
    cursor2 *cur;
    int opcion;
    Fixed tdebe("0.0");
    QString ttdebe;
    fprintf(stderr,"Se ha pulsado la tecla (%i) sobre tapunts y se ha disparado el evento %d, %d\n",caracter, row, col);
    if (abierto) {
        //Siempre teniendo en cuenta que el asiento este abierto.
        switch (caracter) {
        case 47:  // El signo de dividir /
            ttdebe = m_saldoCuenta->text();
            if (ttdebe.toFloat() < 0) {
                ttdebe = ttdebe.right(ttdebe.length()-1);
                tapunts3->setText(row,COL_DEBE, ttdebe);
                tapunts3->setText(row,COL_HABER, "0.00");
                calculadescuadre();
            } else {
                tapunts3->setText(row, COL_HABER, ttdebe);
                tapunts3->setText(row,COL_DEBE,"0.00");
                calculadescuadre();
            }// end if
            break;
        case '+':
            switch(col) {
            case COL_FECHA: {
                    int dia, mes, ano;
                    Q3PtrList<QDate> a;
                    QString cadena;
                    calendario *cal = new calendario(0,0);
                    cal->exec();
                    a = cal->dn->selectedDates();
                    dia = a.first()->day();
                    mes = a.first()->month();
                    ano = a.first()->year();
                    cadena.sprintf( "%2.2d/%2.2d/%d",dia, mes, ano);
                    fprintf(stderr,"Se ha pulsado:%s\n", cadena.ascii());
                    tapunts3->setText(row, COL_FECHA, cadena);
                    delete cal;
                    break;
                }// end case
            case COL_NOMCUENTA:
            case COL_SUBCUENTA:
                // Vaciamos el campo y simulamos un cambio
                tapunts3->setText(row,COL_SUBCUENTA,"");
                cambiadasubcuenta(row);
                break;
            case COL_CONTRAPARTIDA:
                // Vaciamos el campo y simulamos un cambio
                tapunts3->setText(row,COL_CONTRAPARTIDA,"");
                buscacontrapartida(row);
                break;
                break;
            case COL_DEBE:
            case COL_HABER:
                tdebe = Fixed(descuadre->text().ascii());
                if (tdebe < 0) {
                    tdebe = -tdebe;
                    QString cadena;
                    cadena.sprintf("%s",tdebe.toQString().ascii());
                    tapunts3->setText(row,COL_DEBE, cadena);
                    calculadescuadre();
                } else {
                    tapunts3->setText(row, COL_HABER, descuadre->text());
                    calculadescuadre();
                }// end if
                break;
            case COL_CONCEPTO:
                break;
            case COL_CCOSTE:
                // Si el asiento esta abierto mostramos el popup para asientos abiertos
                query = "SELECT * FROM c_coste";
                cur  = companyact->cargacursor(query,"costes");
                menucoste->insertItem(tr("Ninguno"), 1000);
                while (!cur->eof()) {
                    menucoste->insertItem(cur->valor("nombre"), 1000+atoi(cur->valor("idc_coste").ascii()));
                    cur->siguienteregistro();
                }// end while
                delete cur;
                opcion = menucoste->exec();
                if (opcion == 1000) {
                    tapunts3->setText(row, COL_CCOSTE, "");
                    tapunts3->setText(row, COL_IDCCOSTE, "");
                }// end if
                if (opcion > 1000) {
                    QString query1;
                    opcion -= 1000;
                    query1.sprintf("SELECT * FROM c_coste WHERE idc_coste=%d", opcion);
                    cur = companyact->cargacursor(query1.ascii(),"canales1");
                    if (!cur->eof()) {
                        tapunts3->setText(row,COL_CCOSTE, cur->valor("nombre"));
                        tapunts3->setText(row,COL_IDCCOSTE, cur->valor("idc_coste"));
                    }// end if
                    delete cur;
                }// end if
                //                delete menucoste;
                break;
            case COL_CANAL:
                // Si el asiento esta abierto mostramos el popup para asientos abiertos
                query = "SELECT * FROM canal";
                companyact->begin();
                cur = companyact->cargacursor(query,"canales");
                menucanal->insertItem(tr("Ninguno"), 1000);
                companyact->commit();
                while (!cur->eof()) {
                    menucanal->insertItem(cur->valor("nombre"),1000+atoi(cur->valor("idcanal").ascii()));
                    cur->siguienteregistro();
                }// end while
                delete cur;
                opcion = menucanal->exec();
                //                delete menucanal;
                if (opcion == 1000) {
                    tapunts3->setText(row, COL_CANAL, "");
                    tapunts3->setText(row, COL_IDCANAL, "");
                }// end if
                if (opcion > 1000) {
                    QString query1;
                    opcion -= 1000;
                    query1.sprintf("SELECT * FROM canal WHERE idcanal=%d", opcion);
                    companyact->begin();
                    cur = companyact->cargacursor(query1.ascii(),"canales1");
                    companyact->commit();
                    if (!cur->eof()) {
                        tapunts3->setText(row,COL_CANAL, cur->valor("nombre"));
                        tapunts3->setText(row,COL_IDCANAL, cur->valor("idcanal"));
                    }// end if
                    delete cur;
                }// end if
                break;
            }// end switch
            break;
        case '*':
            duplicar(col);
            break;
        case 4100:  // El ENTER
            switch (col) {
            case COL_FECHA:
                tapunts3->setText(row,col,normalizafecha(tapunts3->text(row,col)).toString("dd/MM/yyyy"));
                tapunts3->setCurrentCell(row, COL_SUBCUENTA);
                break;
            case COL_SUBCUENTA:
                tapunts3->setCurrentCell(row, COL_DEBE);
                /// Si la subcuenta tiene el DEBE bloqueado entonces pasamos el cursor al haber.
                query = "SELECT nodebe FROM cuenta WHERE codigo='"+companyact->sanearCadena(tapunts3->text(row,COL_SUBCUENTA))+"'";
                cur = companyact->cargacursor(query);
                if (!cur->eof()) {
                    if (cur->valor("nodebe") == "t")
                        tapunts3->setCurrentCell(row, COL_HABER);
                }// end if
                delete cur;
                break;
            case COL_CONTRAPARTIDA:
                tapunts3->setCurrentCell(row, COL_DEBE);
                break;
            case COL_IVA:
                //                Se ha verificado que es m� c�odo poder cambiar la fecha o dar al enter
                if (row >0) {
                    if (tapunts3->text(row-1,COL_CCOSTE) == "")
                        tapunts3->setText(row+1,COL_FECHA,normalizafecha(tapunts3->text(row,COL_FECHA)).toString("dd/MM/yyyy"));
                    tapunts3->setText(row+1,COL_CCOSTE, tapunts3->text(row,COL_CCOSTE));
                    tapunts3->setText(row+1,COL_CANAL, tapunts3->text(row,COL_CANAL));
                    tapunts3->setText(row+1,COL_IDCCOSTE, tapunts3->text(row,COL_IDCCOSTE));
                    tapunts3->setText(row+1,COL_IDCANAL, tapunts3->text(row,COL_IDCANAL));
                    tapunts3->setCurrentCell(row+1, COL_FECHA);
                } else {
                    tapunts3->setCurrentCell(row,COL_CCOSTE);
                }// end if
                break;
            case COL_CCOSTE:
                if (row > 0) {
                    tapunts3->setText(row+1,COL_FECHA,normalizafecha(tapunts3->text(row,COL_FECHA)).toString("dd/MM/yyyy"));
                    tapunts3->setText(row+1,COL_CCOSTE, tapunts3->text(row,COL_CCOSTE));
                    tapunts3->setText(row+1,COL_CANAL, tapunts3->text(row,COL_CANAL));
                    tapunts3->setText(row+1,COL_IDCCOSTE, tapunts3->text(row,COL_IDCCOSTE));
                    tapunts3->setText(row+1,COL_IDCANAL, tapunts3->text(row,COL_IDCANAL));
                    tapunts3->setCurrentCell(row+1, COL_FECHA);
                } else {
                    tapunts3->setCurrentCell(row,COL_CANAL);
                }// end if
                break;
            case COL_CANAL:
                tapunts3->setText(row+1,COL_FECHA,normalizafecha(tapunts3->text(row,COL_FECHA)).toString("dd/MM/yyyy"));
                tapunts3->setText(row+1,COL_CCOSTE, tapunts3->text(row,COL_CCOSTE));
                tapunts3->setText(row+1,COL_CANAL, tapunts3->text(row,COL_CANAL));
                tapunts3->setText(row+1,COL_IDCCOSTE, tapunts3->text(row,COL_IDCCOSTE));
                tapunts3->setText(row+1,COL_IDCANAL, tapunts3->text(row,COL_IDCANAL));
                tapunts3->setCurrentCell(row+1, COL_FECHA);
                break;
            case COL_CONCEPTO:
                tapunts3->setCurrentCell(row, COL_IVA);
                break;
            default:
                tapunts3->setCurrentCell(row, col+1);
                break;
            }// end switch
            break;
        case Qt::Key_Delete:  //Tecla Supr
            fprintf(stderr,"Se ha pulsado el Supr\n");
            borraborrador(row);
            break;
        case 4115: //Tecla Control + UP
            fprintf(stderr,"Se ha pulsado el ctrl + arriba\n");
            subirapunte(row);
            if (row > 0 )
                tapunts3->setCurrentCell(row, col);
            break;
        case 4117: //Tecla Control + DOWN
            bajarapunte(row);
            tapunts3->setCurrentCell(row, col);
            break;
        }// end switch
    }// end if
    delete menucoste;
    delete menucanal;
}// end pulsadomas


/** Detecta que ha escrito algun contenido en la columna de SUBCUENTA y determina si es una cuenta existente o no y si no lo es pregunta para crearla.
*/
void intapunts3view::cambiadasubcuenta(int row) {
    QString subcuenta = tapunts3->text(row,COL_SUBCUENTA);
    if (subcuenta == "") {
        // Hacemos aparecer la ventana de cuentas
        listcuentasview1 *listcuentas = new listcuentasview1(companyact);
        listcuentas->setModoLista();
        listcuentas->inicializa();
        listcuentas->exec();
        tapunts3->setText(row,COL_SUBCUENTA,listcuentas->codcuenta());
        tapunts3->setText(row,COL_IDCUENTA,listcuentas->idcuenta());
        delete listcuentas;
    }// end if
    // Hacemos la recodificacin
    QString cad = tapunts3->text(row,COL_SUBCUENTA);
    if (cad != "") {
        cad = extiendecodigo(cad,numdigitos);
        companyact->begin();
        QString  query= "SELECT codigo, descripcion, idcuenta, debe - haber AS saldo FROM cuenta WHERE codigo="+cad;
        cursor2 *cursorcta = companyact->cargacursor(query,"querycuenta");
        //cursor2 *cursorcta = companyact->cargacuenta(0, cad );
        companyact->commit();
        int num = cursorcta->numregistros();
        if (num >0) {
            tapunts3->setText(row,COL_SUBCUENTA,cursorcta->valor("codigo"));
            tapunts3->setText(row,COL_NOMCUENTA,cursorcta->valor("descripcion"));
            tapunts3->setText(row,COL_IDCUENTA,cursorcta->valor("idcuenta"));
            m_saldoCuenta->setText(cursorcta->valor("saldo"));
        } else {
            int valor;
            valor = QMessageBox::warning( 0, tr("No existe cuenta"), tr("No existe una cuenta con el codigo proporcionado, desea crear una?."), QMessageBox::Yes, QMessageBox::No);
            if (valor == QMessageBox::Yes) {
                cuentaview *cta = new cuentaview(companyact,0,0,true);
                /// Preparamos la clase para una inserci� de una nueva cuenta.
                cta->cuentanueva(cad);
                cta->exec();
                delete cta;
                /// PAra no hacer rollos rellamamos a la funci� ya que se supone que ya se ha hecho la inserci�.
                cambiadasubcuenta(row);
            }// end if
        }// end if
        delete cursorcta;
    }// end if
}// end cambiadasubcuenta


/** Si cambia el contenido de la fila de contrapartida se actua de manera similar a como si se hubiese cambiado la casilla de cuenta. Buscando que la cuenta exista y preguntando si insertarla o no
*/
void intapunts3view::buscacontrapartida(int row) {
    QString subcuenta = tapunts3->text(row,COL_CONTRAPARTIDA);
    // Hacemos aparecer la ventana de cuentas
    listcuentasview1 *listcuentas = new listcuentasview1(companyact);
    listcuentas->setModoLista();
    listcuentas->inicializa();
    listcuentas->exec();
    tapunts3->setText(row,COL_CONTRAPARTIDA,listcuentas->codcuenta());
    tapunts3->setText(row,COL_IDCONTRAPARTIDA,listcuentas->idcuenta());
    delete listcuentas;
}// end buscacontrapartida


void intapunts3view::cambiadacontrapartida(int row) {
    QString subcuenta = tapunts3->text(row,COL_CONTRAPARTIDA);
    // Hacemos la recodificacin
    QString cad = tapunts3->text(row,COL_CONTRAPARTIDA);
    if (cad != "") {
        cad = extiendecodigo(cad,numdigitos);
        companyact->begin();
        cursor2 *cursorcta = companyact->cargacuenta(0,cad );
        companyact->commit();
        if (!cursorcta->eof()) {
            tapunts3->setText(row,COL_CONTRAPARTIDA,cursorcta->valor(1));
            tapunts3->setText(row,COL_IDCONTRAPARTIDA,cursorcta->valor("idcuenta"));
        } else {
            QMessageBox::warning( 0, tr("No existe cuenta"), tr("No existe una cuenta con el código proporcionado, ¿desea crear una?."), QMessageBox::Yes, QMessageBox::No);
        }// end if
        delete cursorcta;
    } else {
        tapunts3->setText(row,COL_IDCONTRAPARTIDA,"");
        tapunts3->setText(row,COL_CONTRAPARTIDA,"");
    }// end if

}// end cambiadasubcuenta

void intapunts3view::cambiadodebe(int row) {
    QString debe;
    QString cad;
    Fixed ndebe;
    debe =  tapunts3->text(row,COL_DEBE);
    if (debe != "") {
        ndebe =Fixed(debe);
        if (ndebe != 0) {
            tapunts3->setText(row,COL_HABER,"0.00");
        }// end if
        //        cad.sprintf("%2.2f",ndebe);
        //        tapunts3->setText(row,COL_DEBE,cad);
    }// end if
    calculadescuadre();
}// end cambiadodebe


void intapunts3view::cambiadohaber(int row) {
    QString haber;
    QString cad;
    Fixed nhaber;
    haber =  tapunts3->text(row,COL_HABER);
    if (haber != "") {
        nhaber = Fixed(haber);
        if (nhaber !=  0) {
            tapunts3->setText(row,COL_DEBE,"0.00");
        }// end if
    }// end if
    calculadescuadre();
}// end cambiadohaber


/** Se ha pulsado sobre el botón de generar asientos inteligentes. Se inicializa la clase \ref aplinteligentesview y se muestra ese diálogo para que se opere con los asientos plantilla
*/
void intapunts3view::boton_inteligente() {
    int numasiento;
    if (abierto==1) {
        // El asiento esta abierto y por tanto se muestra como abierto
        asientoabiertop();
        numasiento = idAsiento().toInt();
    } else {
        numasiento = 0;
    }// end if
    aplinteligentesview *nueva=new aplinteligentesview(companyact, 0,"");
    nueva->inicializa(numasiento, this);
    nueva->exec();
    delete nueva;
}// end boton_inteligente


/**
  * Esta función se activa cuando se pulsa sobre el botón nuevo asiento del
  * formulario
  */
void intapunts3view::boton_buscacuenta() {
    tapunts3->setText(rowactual, COL_SUBCUENTA,"");
    cambiadasubcuenta(rowactual);
}// end boton_buscacuenta


/**
  * Si el parametro pasado es un:
  * 0 -> del día actual
  * 1 -> del mes actual
  * 2 -> del año actual
  */
void intapunts3view::boton_extracto1(int tipo) {
    QDate fecha1, fecha2, fechaact;
    if(!tapunts3->text(tapunts3->currentRow(), COL_FECHA).isEmpty()) {
        fechaact = normalizafecha(tapunts3->text(tapunts3->currentRow(), COL_FECHA));
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
        companyact->extractoempresa()->inicializa1(tapunts3->text(tapunts3->currentRow(), COL_SUBCUENTA), tapunts3->text(tapunts3->currentRow(), COL_SUBCUENTA),fecha1.toString("dd/MM/yyyy"), fecha2.toString("dd/MM/yyyy"), 0);
    }// end if
    companyact->extractoempresa()->accept();
    companyact->libromayor();
}// end boton_extracto1


/**
  * Si el parametro pasado es un:
  * 0 -> del día actual
  * 1 -> del mes actual
  * 2 -> del año actual
  */
void intapunts3view::boton_diario1(int tipo) {
    QDate fecha1, fecha2, fechaact;
    if(!tapunts3->text(tapunts3->currentRow(), COL_FECHA).isEmpty()) {
        fechaact = normalizafecha(tapunts3->text(tapunts3->currentRow(), COL_FECHA) );
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
        companyact->diarioempresa()->inicializa1( fecha1.toString("dd/MM/yyyy"), fecha2.toString("dd/MM/yyyy"), 0);
    }// end if
    companyact->diarioempresa()->accept();
    companyact->librodiario();
}// end boton_diario1


/**
  * Si el parametro pasado es un:
  * 0 -> del día actual
  * 1 -> del mes actual
  * 2 -> del año actual
  */
void intapunts3view::boton_balance1(int tipo) {
    QDate fecha1, fecha2, fechaact;
    if(!tapunts3->text(tapunts3->currentRow(), COL_FECHA).isEmpty()) {
        fechaact = normalizafecha(tapunts3->text(tapunts3->currentRow(), COL_FECHA) );
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
        companyact->balanceempresa()->inicializa1(tapunts3->text(tapunts3->currentRow(), COL_SUBCUENTA), tapunts3->text(tapunts3->currentRow(), COL_SUBCUENTA), fecha1.toString("dd/MM/yyyy"), fecha2.toString("dd/MM/yyyy"), 0);
    }// end if
    companyact->balanceempresa()->accept();
    // La presentación que la haga la clase empresa. Que es quien se encarga de ello.
    companyact->librobalance();
}// end boton_balance1




/**
  * Esta función se invoca al pulsar sobre el botón de filtrado
  * Sirve para filtrar los asientos introducidos
  * El objeto filt se crea con el constructor de intapuntsview.
  */
void intapunts3view::boton_filtrar() {
    QString cadena;
    filt->exec();
    cadena =  filt->cantidadapunte->text();
    cargarcursor();
    boton_inicio();
}// end boton_filtrar

void intapunts3view::eturn_fechaasiento() {
    QString query;
    if (abierto) { //cambiar la fecha del asiento
        companyact->begin();
        query.sprintf("UPDATE asiento SET fecha='%s' WHERE idasiento='%s'",companyact->sanearCadena(m_fecha->text()).ascii(),idAsiento().ascii());
        fprintf(stderr,"%s\n",query.ascii());
        int error = companyact->ejecuta(query);
        if (error) {
            return;
        } else {
            companyact->commit();
        }// end if
    } else {
        iniciar_asiento_nuevo();
    }
}// end return_fechaasiento

/** Esta función genera el asiento de cierre de la empresa.
  * Debe haber un asiento abierto para que se realize la operación.
  */
void intapunts3view::asiento_cierre() {
    /// El asiento debe estar abierto para poder realizar el asiento de cierre.
    if (abierto) {
        int idcuenta;
        QString snuevodebe, snuevohaber;
        // Si no hay asiento lo calculamos.
        idasiento = idAsiento().toInt();
        if (idasiento==-1) {
            QMessageBox::warning(0,tr("No hay asiento"), tr("No hay asiento"),"OK",0,0);
            return;
        }// end if

        QString query ="SELECT idcuenta, sum(debe) AS sumdebe, sum(haber) AS sumhaber, sum(debe)-sum(haber) AS saldito FROM apunte WHERE idcuenta NOT IN (SELECT idcuenta FROM cuenta WHERE idgrupo=6 OR idgrupo=7)  AND fecha <= '"+m_fecha->text()+"' GROUP BY idcuenta ORDER BY saldito";
        companyact->begin();
        cursor2 *cursor=companyact->cargacursor(query, "cursor");
        companyact->commit();
        int orden=0;
        QString concepto="Asiento de Cierre";
        QString fecha = m_fecha->text();
        while (! cursor->eof()) {
            orden++;
            idcuenta = atoi(cursor->valor("idcuenta").ascii());
            if (cursor->valor("saldito").left(1) != "-") {
                snuevohaber = cursor->valor("saldito");
                snuevodebe = "0";
            } else {
                snuevodebe = "ABS("+cursor->valor("saldito")+")";
                snuevohaber = "0";
            }// end if
            if (cursor->valor("saldito") != "0.00") {
                // Inserci� de Borrador
                // El borrador no existe, por lo que hay que hacer un insert
                query.sprintf("INSERT INTO borrador (orden, conceptocontable, fecha, idcuenta, debe, haber, idasiento) VALUES (%d,'%s','%s',%d,%s,%s,%d)",
                              orden,
                              companyact->sanearCadena(concepto).ascii(),
                              companyact->sanearCadena(fecha).ascii(),
                              idcuenta,
                              snuevodebe.ascii(),
                              snuevohaber.ascii(),
                              idasiento);
                companyact->begin();
                int error = companyact->ejecuta(query);
                if (error) {
                    companyact->rollback();
                    delete cursor;
                    return;
                }// end if
                companyact->commit();
                // Fin de la inserci� de Borrador
            }// end if
            cursor->siguienteregistro();
        }// end while
        delete cursor;
        // muestraasiento(idasiento);
        repinta(idasiento);
    } else {
        QMessageBox::warning( 0, tr("Asiento Cerrado"), tr("Debe abrir un asiento primero.."), QMessageBox::Ok,0);
    }// end if
}// end asiento_cierre


/// La creación de un asiento de apertura debe basarse en un asiento de cierre.
void intapunts3view::asiento_apertura() {
    /// Preparamos los datos.
    QString concepto="Asiento de Apertura";
    QString fecha = m_fecha->text();
    idasiento= idAsiento().toInt();
    // Si no existe ningn asiento abortamos
    if (idasiento == -1) {
        QMessageBox::warning(0,tr("No hay asientos"), tr("No existe ningn asiento para realizar el asiento de apertura"), tr("Cerrar"),0,0,0);
        return;
    }// end if
    QString idasientocierre;

    if (abierto) {
        /// Buscamos el asiento anterior a este.
        QString SQLQuery = "SELECT * FROM asiento WHERE fecha < '"+fecha+"' AND idasiento < "+QString::number(idasiento)+" ORDER BY ordenasiento DESC";
        companyact->begin();
        cursor2 *cur=companyact->cargacursor(SQLQuery, "elquery");
        companyact->commit();
        if (!cur->eof()) {
            idasientocierre=cur->valor("idasiento");
        }// end if
        delete cur;
        /// Seleccionamos todos sus registros de borrador.
        QString SQLQuery1 = "SELECT * FROM borrador WHERE idasiento="+idasientocierre+" ORDER BY orden";
        companyact->begin();
        cur = companyact->cargacursor(SQLQuery1, "masquery");
        companyact->commit();
        while (!cur->eof()) {
            QString orden=cur->valor("orden");
            QString idcuenta = cur->valor("idcuenta");
            QString totaldebe = cur->valor("debe");
            QString totalhaber = cur->valor("haber");
            SQLQuery1.sprintf("INSERT INTO borrador (orden, conceptocontable, fecha, idcuenta, debe, haber, idasiento) VALUES (%d,'%s','%s',%d,%s,%s,%d)",
                              orden.toInt(),
                              companyact->sanearCadena(concepto).ascii(),
                              companyact->sanearCadena(fecha).ascii(),
                              idcuenta.toInt(),
                              totalhaber.ascii(),
                              totaldebe.ascii(),
                              idasiento);
            companyact->begin();
            int error = companyact->ejecuta(SQLQuery1);
            if(error) {
                companyact->rollback();
                delete cur;
                return;
            }// end if
            companyact->commit();
            cur->siguienteregistro();
        }// end while
        delete cur;
        // muestraasiento(idasiento);
        repinta(idasiento);
    } else {
        QMessageBox::warning( 0, tr("Asiento Cerrado"), tr("Debe abrir un asiento primero.."), QMessageBox::Ok,0);
    }// end if
}// end asiento_apertura


void intapunts3view::asiento_regularizacion() {
    /// Para poder generar un asiento de regularización debemos tener un asiento abierto.
    /// Sino, no merece la pena hacerlo.
    if (!abierto) {
        QMessageBox::warning( 0, tr("Asiento Cerrado"), tr("Debe abrir un asiento primero.."), QMessageBox::Ok,0);
	return;
    }// end if

        int idcuenta;
        int idcuenta1;
        Fixed diferencia("0");
        Fixed totaldebe("0"), totalhaber("0");
        Fixed totaldebe1("0"), totalhaber1("0");
        QString concepto="Asiento de Regularizacion";
        QString fecha = m_fecha->text();

        // Si no hay asiento lo calculamos.
        idasiento=idAsiento().toInt();
        if (idasiento==-1) {
            QMessageBox::warning(0,tr("No existe asiento"),tr("No existe asiento"),"OK",0,0);
            return;
        }// end if
        /// El parametro est�en la configuraci� de empresa.
	/// Buscamos la cuenta de regularizacion. Normalmente es la 129
        QString query = "SELECT * FROM cuenta WHERE codigo in (SELECT valor FROM configuracion WHERE nombre='CuentaRegularizacion')";
        cursor2 *cur = companyact->cargacursor(query,"idcuenta");
        idcuenta1 = cur->valor("idcuenta").toInt();
        delete cur;

	/// Hacemos el calculo de saldos hasta la fecha.
        query = "SELECT idcuenta, sum(debe) AS sumdebe, sum(haber) AS sumhaber, sum(debe)-sum(haber) AS saldito from apunte WHERE idcuenta IN (SELECT idcuenta FROM cuenta where idgrupo=6 OR idgrupo=7) AND fecha <= '"+m_fecha->text()+"' GROUP BY idcuenta ORDER BY saldito";
        cur = companyact->cargacursor(query);
        int orden=0;
        while (!cur->eof()) {
            orden++;
            idcuenta = atoi(cur->valor("idcuenta").ascii());
            diferencia = Fixed(cur->valor("sumdebe").ascii())-Fixed(cur->valor("sumhaber").ascii());
            if (diferencia > 0) {
                totalhaber = diferencia;
                totaldebe= 0;
            } else {
                totaldebe = -diferencia;
                totalhaber=0;
            }// end if
            totaldebe1 = totaldebe1 + totaldebe;
            totalhaber1 = totalhaber1 + totalhaber;
            // Inserci� de Borrador
            // El borrador no existe, por lo que hay que hacer un insert
            query.sprintf("INSERT INTO borrador (orden, conceptocontable, fecha, idcuenta, debe, haber, idasiento) VALUES (%d,'%s','%s',%d,%s,%s,%d)",
                          orden,
                          companyact->sanearCadena(concepto).ascii(),
                          companyact->sanearCadena(fecha).ascii(),
                          idcuenta,
                          totaldebe.toQString().ascii(),
                          totalhaber.toQString().ascii(),
                          idasiento);
            companyact->begin();
            int error = companyact->ejecuta(query);
            if (error) {
                companyact->rollback();
                delete cur;
                return;
            }// end if
            companyact->commit();
            // Fin de la inserci� de Borrador
            cur->siguienteregistro();
        }// end while
        delete cur;
        if (totaldebe1 > 0) {
            orden++;
            query.sprintf("INSERT INTO borrador (orden, conceptocontable, fecha, idcuenta, debe, haber, idasiento) VALUES (%d,'%s','%s',%d,0,%s,%d)",
                          orden,
                          companyact->sanearCadena(concepto).ascii(),
                          companyact->sanearCadena(fecha).ascii(),
                          idcuenta1,
                          totaldebe1.toQString().ascii(),
                          idasiento);
            companyact->begin();
            int error = companyact->ejecuta(query);
            if (error) {
                companyact->rollback();
                return;
            }// end if
            companyact->commit();
        } // end if
        if (totalhaber1 > 0) {
            orden++;
            query.sprintf("INSERT INTO borrador (orden, conceptocontable, fecha, idcuenta, debe, haber, idasiento) VALUES (%d,'%s','%s',%d,%s,0,%d)",
                          orden,
                          companyact->sanearCadena(concepto).ascii(),
                          companyact->sanearCadena(fecha).ascii(),
                          idcuenta1,
                          totalhaber1.toQString().ascii()
                          ,idasiento);

            companyact->begin();
            int error = companyact->ejecuta(query);
            if (error) {
                companyact->rollback();
                return;
            }// end if
            companyact->commit();
        }// end if
      //  muestraasiento(idasiento);
	repinta(idasiento);
}// end asiento_regularizacion

/** Al pulsar return sobre el número de asiento se procede como si fuese una
  * carga de dicho asiento.
  */
void intapunts3view::eturn_numasiento() {
    boton_cargarasiento();
}// end return_cuenta


/**
  * Se ha pulsado sobre el boton de cargar asiento con lo
  * que debemos comprobar que el numero introducido es correcto
  * y hacer las gestiones oportunas para mostrar el asiento en
  * pantalla o crearlo si hace falta.
  */
void intapunts3view::boton_cargarasiento() {
    QString query;
    int IdAsien=0;
    companyact->begin();
    query.sprintf("SELECT idasiento FROM asiento WHERE ordenasiento<=%d ORDER BY ordenasiento DESC",idasiento1->text().toInt());
    cursor2 *curs = companyact->cargacursor(query, "micursor");
    companyact->commit();
    if (!curs->eof()) {
        IdAsien = atoi(curs->valor("idasiento").ascii());
        cargarcursor();
        muestraasiento(IdAsien);
    }// end if
    delete curs;
}// end boton_cargarasiento

/**
  * Esta función se activa cuando se pulsa sobre el boton borrar asiento
  * del formulario
*/
void intapunts3view::boton_borrar_asiento() {
    borrar_asiento(true);
}// end boto_borrar_asiento

void intapunts3view::borrar_asiento(bool confirmarBorrado) {
    QString query;
    int valor;

    /// Vamos a necesitar el asiento anterior (Si es que existe.).
    int asientoant=-1;
    if (!cursorasientos->esprimerregistro() && (cursorasientos->numregistros() != 0)) {
        cursorasientos->registroanterior();
        asientoant= cursorasientos->valor("idasiento").toInt();
        cursorasientos->siguienteregistro();
    }// end if


    if (idAsiento().toInt() > 0) {
        if (confirmarBorrado) {
            valor = QMessageBox::warning( 0, "Borrar Asiento", "Se procedera a borrar el asiento.", QMessageBox::Yes, QMessageBox::No);
        } else {
            valor = QMessageBox::Yes ;
        }
        if (valor ==  QMessageBox::Yes) {
            companyact->begin();
            query.sprintf("DELETE FROM apunte where idasiento=%s",IDASIENTO);
            int error = companyact->ejecuta(query);
            if (error) {
                companyact->rollback();
                return;
            }// end if
            query.sprintf("DELETE FROM iva WHERE idregistroiva IN (SELECT idregistroiva FROM registroiva WHERE idborrador IN (SELECT idborrador FROM borrador WHERE idasiento=%s))",IDASIENTO);
            error = companyact->ejecuta(query);
            if (error) {
                companyact->rollback();
                return;
            }// end if
            query.sprintf("DELETE FROM registroiva where idborrador IN (SELECT idborrador FROM borrador WHERE idasiento=%s)",IDASIENTO);
            error = companyact->ejecuta(query);
            if (error) {
                companyact->rollback();
                return;
            }// end if
            query.sprintf("DELETE FROM borrador where idasiento=%s",IDASIENTO);
            error = companyact->ejecuta(query);
            if (error) {
                companyact->rollback();
                return;
            }// end if
            error = companyact->borrarasiento(atoi(IDASIENTO));
            if (error) {
                companyact->rollback();
                return;
            }// end if
            cargarcursor();
            repinta(asientoant);
        }// end if
    }// end if
} //fin borrar_asiento


/**
  * Esta se encarga de la edicion de asientos.
  */
void intapunts3view::editarasiento() {
    guardaborrador(rowactual);
    asientoview *nuevoasiento= new asientoview(companyact,0,"",true);
    nuevoasiento->inicializa(companyact);
    nuevoasiento->cargaasiento(idAsiento().toInt());
    nuevoasiento->exec();
    cargarcursor();
    repinta(idAsiento().toInt());
}// end editarasiento


/** Altera el orden de los apuntes subiendo el apunte sobre el que nos encontramos para ello lo que hace esta función es intercambiarlo con el apunte que está encima
  */
void intapunts3view::subirapunte(int row) {
    /// Como existen resticciones en la base de datos sobre el campo orden
    /// Debemos usar un campo intermedio.
    QString valorintermedio;
    if (row > 0 ) {
        //        tapunts3->swapRows(row, TAPUNTS_NUM_ROWS-1);
        //        guardaborrador(TAPUNTS_NUM_ROWS-1);
        tapunts3->swapRows(row, row-1);
        valorintermedio = tapunts3->text(row,COL_ORDEN);
        tapunts3->setText(row, COL_ORDEN, tapunts3->text(row-1, COL_ORDEN));
        tapunts3->setText(row-1, COL_ORDEN, "100000");
        guardaborrador(row-1);
        guardaborrador(row);
        tapunts3->setText(row-1, COL_ORDEN, valorintermedio);
        guardaborrador(row-1);
    }// end if
}// end subirapunte

/** Altera el orden de los apuntes subiendo el apunte sobre el que nos encontramos para ello lo que hace esta función es intercambiarlo con el apunte que está debajo
  */
void intapunts3view::bajarapunte(int row) {
    /// Como existen resticciones en la base de datos sobre el campo orden
    /// Debemos usar un campo intermedio.
    QString valorintermedio;
    if (!tapunts3->text(row+1, COL_IDBORRADOR).isNull()) {
        tapunts3->swapRows(row, row+1);
        valorintermedio = tapunts3->text(row,COL_ORDEN);
        tapunts3->setText(row, COL_ORDEN, tapunts3->text(row+1, COL_ORDEN));
        tapunts3->setText(row+1, COL_ORDEN, "100000");
        guardaborrador(row+1);
        guardaborrador(row);
        tapunts3->setText(row+1, COL_ORDEN, valorintermedio);
        guardaborrador(row+1);
    }// end if
}// end subirapunte


/** \brief Se ha pulsado sobre el botón de duplicar asiento
  *
  * Inicializa el dialogo de duplicación de asientos y lo presenta.
  * Cuando se ha terminado carga el cursor de presentación y repinta el asiento
  * para que actualize los cambios
  */
void intapunts3view::boton_duplicarasiento() {
    duplicarasientoview *dupli= new duplicarasientoview(companyact,0,"",true);
    // Establecemos los parametros para el nuevo asiento a duplicar
    dupli->inicializa(idasiento1->text(), idasiento1->text());
    dupli->exec();
    cargarcursor();
    boton_fin();
    repinta(idAsiento().toInt());
    delete dupli;
}// end boton_duplicarasiento



