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


#include <qpopupmenu.h>
#include <qlineedit.h>
#include <qlistview.h>
#include <qtoolbutton.h>
#include <qpushbutton.h>
#include <stdlib.h>
#include <qinputdialog.h>
#include <qcolor.h>
#include <qfont.h>


#include "intapunts3view.h"
#include "asientoview.h"
#include "calendario.h"
#include "ivaview.h"
#include "aplinteligentesview.h"
#include "listcuentasview1.h"
#include "cuentaview.h"
#include "duplicarasientoview.h"


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

#define IDASIENTO cursorasientos->valor("idasiento").ascii()
#define ORDENASIENTO cursorasientos->valor("ordenasiento").ascii()


intapunts3view::intapunts3view(empresa *emp,QWidget *parent, const char *name ) : intapunts3dlg(parent,name) {
    empresaactual = emp;
    idasiento=-1;
    tapunts = new QTable1(this,"");
    tapunts->setGeometry( QRect( 10, 75, 693, 260 ) );
    tapunts->setSizePolicy( QSizePolicy( (QSizePolicy::SizeType)5, (QSizePolicy::SizeType)1, 0, 0, tapunts->sizePolicy().hasHeightForWidth() ) );
    QFont tapunts_font(  tapunts->font() );
    tapunts_font.setPointSize(atoi(confpr->valor(CONF_FONTSIZE_APUNTES).c_str()));
    tapunts_font.setFamily(confpr->valor(CONF_FONTFAMILY_APUNTES).c_str());
    tapunts->setFont( tapunts_font );

    tapunts->setNumRows( 0 );
    tapunts->setNumCols( 0 );
    tapunts->setRowMovingEnabled( TRUE );
    tapunts->setColumnMovingEnabled( TRUE );
    tapunts->setSorting( TRUE );
    tapunts->setSelectionMode( QTable::SingleRow );
    intapunts3dlgLayout->addWidget( tapunts, 1, 0 );
    //connect( tapunts, SIGNAL( clicked(int,int,int,const QPoint&) ), this, SLOT( tpulsado(int,int,int,const QPoint&) ) );
    connect( tapunts, SIGNAL( valueChanged(int,int) ), this, SLOT( apuntecambiadogrid(int,int) ) );
    connect( tapunts, SIGNAL( selectionChanged() ), this, SLOT( tcambiaseleccion() ) );
    //connect( tapunts, SIGNAL( currentChanged(int,int) ), this, SLOT( currentChanged(int,int) ) );
    connect( tapunts, SIGNAL( contextMenuRequested(int,int,const QPoint&) ), this, SLOT( contextmenu(int,int,const QPoint&) ) );
    connect( tapunts, SIGNAL( pulsadomas(int, int, int)), this, SLOT(pulsadomas(int, int, int)));

    tapunts->setNumCols(16);
    tapunts->horizontalHeader()->setLabel( COL_FECHA, tr( "FECHA" ) );
    tapunts->horizontalHeader()->setLabel( COL_SUBCUENTA, tr( "SUBCUENTA" ) );
    tapunts->horizontalHeader()->setLabel( COL_CONCEPTO, tr( "CONCEPTO" ) );
    tapunts->horizontalHeader()->setLabel( COL_DEBE, tr( "DEBE" ) );
    tapunts->horizontalHeader()->setLabel( COL_HABER, tr( "HABER" ) );
    tapunts->horizontalHeader()->setLabel( COL_NOMCUENTA, tr( "NOMBRE CUENTA" ) );
    tapunts->horizontalHeader()->setLabel( COL_CONTRAPARTIDA, tr( "CONTRAPARTIDA" ) );
    tapunts->horizontalHeader()->setLabel( COL_TIPOIVA, tr( "TIPO IVA" ) );
    tapunts->horizontalHeader()->setLabel( COL_IVA, tr( "IVA" ) );
    tapunts->horizontalHeader()->setLabel( COL_CCOSTE, tr( "CCOSTE") );
    tapunts->horizontalHeader()->setLabel( COL_CANAL, tr( "CANAL") );
    tapunts->horizontalHeader()->setLabel( COL_IDCCOSTE, tr( "IDCCOSTE") );
    tapunts->horizontalHeader()->setLabel( COL_IDCANAL, tr( "IDCANAL") );
    tapunts->horizontalHeader()->setLabel( COL_IDCUENTA, tr( "IDCUENTA") );
    tapunts->setColumnWidth(COL_FECHA,75);
    tapunts->setColumnWidth(COL_SUBCUENTA,100);
    tapunts->setColumnWidth(COL_CONCEPTO,250);
    tapunts->setColumnWidth(COL_DEBE,75);
    tapunts->setColumnWidth(COL_HABER,75);
    tapunts->setColumnWidth(COL_NOMCUENTA,150);
    tapunts->setColumnWidth(COL_CONTRAPARTIDA,100);
    tapunts->setColumnWidth(COL_TIPOIVA,75);
    tapunts->setColumnWidth(COL_IVA,75);
    tapunts->setColumnWidth(COL_CCOSTE, 75);
    tapunts->setColumnWidth(COL_CANAL, 75);
    tapunts->hideColumn(COL_IDBORRADOR);
    tapunts->hideColumn(COL_TIPOIVA);

    tapunts->hideColumn(COL_IDCUENTA);
    tapunts->hideColumn(COL_IDCONTRAPARTIDA);
    tapunts->hideColumn(COL_IDCANAL);
    tapunts->hideColumn(COL_IDCCOSTE);

    // Dependiendo de la configuración mostramos o no mostramos la contrapartida, los canales y los centros de coste
    if (confpr->valor(CONF_CONTRAPARTIDA_APUNTES) != "YES")
        tapunts->hideColumn(COL_CONTRAPARTIDA);
    if (confpr->valor(CONF_CANAL_APUNTES) != "YES")
        tapunts->hideColumn(COL_CANAL);
    if (confpr->valor(CONF_CCOSTE_APUNTES) != "YES")
        tapunts->hideColumn(COL_CCOSTE);


    cursorasientos = NULL;

    // Creamos el objeto de filtrado de asientos para que el filtro funcione siempre bien
    // desde esta ventana.
    filt = new filtrarasientosview(0,"");

    // Consideramos que no hay row actual
    rowactual = -1;

}

intapunts3view::~intapunts3view() {
    delete tapunts;
    delete filt;
}// end ~intapunts3view

/******************************************************************
 * Esta funcion se encarga de inicializar el formulario. Carga los apuntes
 * de la empresa, etc et etc
 ******************************************************************/
int intapunts3view::inicializa(postgresiface2 *conexion) {
    cursor2 *cursoraux1;
    conexionbase = conexion;
    // Al final la inicializacion con el nombre de la empresa desaparecera
    // Ya que la empresa esta abierta con la conexion.
    // Vamos a cargar el número de digitos de cuenta para poder hacer una introduccion de numeros de cuenta mas practica.
    conexionbase->begin();
    QString query = "SELECT * FROM configuracion WHERE nombre= 'CodCuenta'";
    cursoraux1 = conexionbase->cargacursor(query,"codcuenta");
    conexionbase->commit();
    numdigitos=cursoraux1->valor(2).length();
    delete cursoraux1;
    abierto = 0;
    // Vamos a cargar el cursor de los asientos
    cargarcursor(-1);
    boton_fin();
    return(0);
}// end inicializa


/**********************************************************
 * Esta funcion carga el cursor que sirve para hacer todo *
 * el recorrido a través de los asientos.                 *
 * numasiento: 0 indica el primer asiento
 *            -1 indica el ultimo asiento.
           otros indica el asiento o el inmediatamente más bajo
 
  Esta funcion no hace cambios en la presentación, solo realiza una
  carga del cursor que sirve para recorrer los asientos.
 **********************************************************/
void intapunts3view::cargarcursor(int numasiento) {
    QString cantapunt ="";
    QString saldototal = "";
    QString nombreasiento = "";
    QString query;
    QString cadwhere;
    QString textsaldototal = "";
    QString textcantapunt = "";
    QString textnombreasiento= "";
    QString textejercicio="";
    int ordenasiento=0;
    
    if (numasiento > 0) ordenasiento =atoi(ORDENASIENTO);
    fprintf(stderr,"Ejercicio: %s, IdAsiento: %d, OrdenAsiento: %d \n", EjercicioActual.ascii(), numasiento, ordenasiento);
    
    cantapunt = filt->cantidadapunte->text();
    saldototal = filt->saldoasiento->text();
    nombreasiento = filt->nombreasiento->text();
    int pand=0;
    if (cursorasientos != NULL )
        delete cursorasientos;

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
    if (pand) textejercicio = " AND EXTRACT(YEAR FROM fecha)='"+ EjercicioActual +"'";
    else textejercicio = " WHERE EXTRACT(YEAR FROM fecha)='"+ EjercicioActual +"'";
    if ((numasiento != 0) && (numasiento != -1)) {
        //query = "SELECT * FROM asiento ORDER BY ordenasiento";
        query = "SELECT * FROM asiento" + textejercicio + " ORDER BY ordenasiento";
    } else {
        //query = "SELECT * FROM asiento "+cadwhere+textsaldototal+textcantapunt+textnombreasiento+"  ORDER BY ordenasiento";
        query = "SELECT * FROM asiento "+cadwhere+textsaldototal+textcantapunt+textnombreasiento+textejercicio+" ORDER BY ordenasiento";
    }// end if
    conexionbase->begin();
    cursorasientos = conexionbase->cargacursor(query,"cursorasientos");
    conexionbase->commit();
    if (cursorasientos->numregistros() == 0) {
        QMessageBox::warning( 0, "No existe asiento", "No existe ningun asiento para mostrar.", QMessageBox::Yes, QMessageBox::No);
    }// end if
    if (numasiento > 0 && (cursorasientos->numregistros() != 0)) {
        cursorasientos->ultimoregistro();
        //while (!cursorasientos->bof() && atoi(cursorasientos->valor("idasiento").ascii())>numasiento) {
        while (!cursorasientos->bof() && atoi(cursorasientos->valor("ordenasiento").ascii())>ordenasiento) {
            cursorasientos->registroanterior();
        }// end while
        //Si el asiento no existe porque acabamos de borrarlo, entonces nos situamos sobre el primer registro
        if (cursorasientos->bof()) cursorasientos->primerregistro();
    }// end if
}// end cargarcursor

void intapunts3view::boton_inicio() {
    if (cursorasientos->numregistros() != 0) {
        cursorasientos->primerregistro();
        muestraasiento(atoi(cursorasientos->valor("idasiento").ascii()));
    } else {
        vaciarapuntes();
    }// end if
}


void intapunts3view::boton_fin() {
    if (cursorasientos->numregistros() != 0 ) {
        cursorasientos->ultimoregistro();
        muestraasiento(atoi(cursorasientos->valor("idasiento").ascii()));
    } else {
        vaciarapuntes();
    }// end if
}


void intapunts3view::boton_siguiente() {
    //  muestraasiento(idasiento+1);
    if (!cursorasientos->esultimoregistro() && (cursorasientos->numregistros() != 0)) {
        cursorasientos->siguienteregistro();
        muestraasiento(atoi(cursorasientos->valor("idasiento").ascii()));
    }// end if
}


void intapunts3view::boton_anterior() {
    if (!cursorasientos->esprimerregistro() && (cursorasientos->numregistros() != 0)) {
        cursorasientos->registroanterior();
        muestraasiento(atoi(cursorasientos->valor("idasiento").ascii()));
    } else {
        //vaciarapuntes();
    }// end if
}



/******************************************************************
 * Esta funcion se llama desde remoto para obligar a la ventana
 * a que nos muestre un asiento determinado.
 
   Se desplaza a través del cursor de asientos hasta encontrar el
   asiento que se desea. Si no se encuentra en el cursor se situa al final
   del cursor y muestra el asiento (siempre que exista).
 ******************************************************************/
void intapunts3view::muestraasiento(int numasiento) {
    cursor2 *cursoraux, *cursoraux1;
    QString fechaasiento2;
    QString cadena;
    int num,num2;
    if (abierto) {
        int valor = QMessageBox::warning( 0, "Cerrar Asiento", "El asiento actual esta abierto, debe cerrarlo antes de continuar, ¿Cerrar?.", QMessageBox::Yes, QMessageBox::No);
        if (valor == QMessageBox::Yes) {
            boton_cerrarasiento();
            // Si el asiento se mantiene abierto, entonces abortamos la misión.
            if (abierto) {
                return;
            }// end if
        } else {
            guardaborrador(rowactual);
        }// end if
    }// end if


    cursorasientos->primerregistro();
    while (atoi(IDASIENTO) != numasiento && !cursorasientos->esultimoregistro())
        cursorasientos->siguienteregistro();

    vaciarapuntes();
    conexionbase->begin();
    cursoraux= conexionbase->cargaasiento(numasiento);
    num = cursoraux->numregistros();
    if (num == 0) {
        // No existe el numero de asiento que se ha introducido, por tanto lo avisamos y salimos.
        int valor = QMessageBox::warning( 0, "No existe asiento", "No existe un asiento con el numero que ha proporcionado, desea crear uno?.", QMessageBox::Yes, QMessageBox::No);
        if (valor == QMessageBox::Yes) {
            asientoview *nuevoasiento = new asientoview;
            nuevoasiento->inicializa(conexionbase);
            int nasiento = nuevoasiento->creaasiento(fechaasiento1->text(), fechaasiento1->text(),numasiento,1);
            delete nuevoasiento;
            cargarcursor(nasiento);
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
    cursoraux=conexionbase->cargaapuntes(numasiento);
    num2 = cursoraux->numregistros();
    cursoraux1=conexionbase->cargaborradores(numasiento);
    num = cursoraux1->numregistros();
    if (num2 != num) {
        // El asiento esta abierto y por tanto se muestra como abierto
        asientoabiertop();
    } else {
        asientocerradop();
    }// end if
    conexionbase->commit();
    delete cursoraux1;
    delete cursoraux;
    // Como ya hemos hecho los ajustes para que lo que se
    // muestre sea correcto, llamamos a la funcionde repintar.
    repinta(numasiento);
    rowactual  =-1;
}// end muestraasiento

/******************************************************************
 * Esta funcion se llama desde remoto para obligar a la ventana
 * a que nos muestre un asiento determinado.
    
    Lo busca directamente en la base de datos, no el el cursor 
    'cursorasientos'. Útil para visualizar asientos de ejercicios
    anteriores.
 ******************************************************************/
void intapunts3view::flashAsiento(int numasiento) {
    QString query;
    query.sprintf("SELECT * FROM asiento WHERE idasiento='%d'",numasiento);
    vaciarapuntes();
    conexionbase->begin();
    cursor2 * recordSet = conexionbase->cargacursor(query,"recordSet1");
    conexionbase->commit();
    if (!recordSet->eof()) repinta(numasiento);
}


/******************************************************************
 * Esta funcion se llama desde remoto para obligar a la ventana
 * a que nos muestre un asiento determinado.
 ******************************************************************/
void intapunts3view::repinta(int numasiento) {
    cursor2 *cursorasiento, *cursoraux, *cursoriva;
    QString cadena, ordenAsiento, query, fechaasiento2;
    int num, i;

    /*
    if (!cursorasientos->eof() && !cursorasientos->bof()) {
        idasiento = atoi(IDASIENTO);
    } else {
        return;
    }// end if
    */
    //   float debe,haber;
    //   float totaldebe=0, totalhaber=0;
    //vaciarapuntes();
    conexionbase->begin();
    query.sprintf("SELECT * FROM asiento where idasiento=%d",numasiento);
    cursoraux = conexionbase->cargacursor(query,"uncursor");
    query.sprintf("SELECT borrador.debe, borrador.haber, borrador.conceptocontable, borrador.idborrador, borrador.idc_coste, borrador.idcanal,cuenta.codigo,cuenta.idcuenta, cuenta.descripcion AS desccuenta, borrador.fecha AS borrfecha , borrador.contrapartida AS contrapartida FROM  borrador, cuenta WHERE borrador.idasiento=%d AND cuenta.idcuenta=borrador.idcuenta ORDER BY orden",numasiento);
    cursorasiento = conexionbase->cargacursor(query,"micursor");
    conexionbase->commit();
    num = cursoraux->numregistros();
    if (num ==0) {
        // No existe el numero de apunte que se ha introducido, por tanto lo avisamos y salimos.
        QMessageBox::warning( 0, "No existe asiento", "No existe un asiento con el numero que ha proporcionado, desea crear uno?.", QMessageBox::Yes, QMessageBox::No);
        return;
    } else {
        ordenAsiento = cursoraux->valor("ordenasiento");
        fechaasiento2 = cursoraux->valor("fecha");
    }// end if
    num=cursorasiento->numregistros();
    // Hacemos que no haya ningún apunte como preestablecido.
    // tapunts->setNumRows(100);
    tapunts->setNumRows(0);
    tapunts->setNumRows(100);
    i=0;
    while (!cursorasiento->eof()) {
        cadena.sprintf("%10.10s",cursorasiento->valor("borrfecha").ascii());
        // AQUI HAY UN BUEN EJEMPLO
        //         tapunts->setItem(i,COL_FECHA, new QTableItem1(tapunts, QTableItem::OnTyping,cadena,0));
        tapunts->setText(i,COL_FECHA,cadena);
        tapunts->setText(i,COL_NOMCUENTA,cursorasiento->valor("desccuenta"));
        tapunts->setText(i,COL_IDCUENTA,cursorasiento->valor("idcuenta"));
        tapunts->setText(i,COL_SUBCUENTA,cursorasiento->valor("codigo"));
        //         debe = atof(cursorasiento->valor("debe").c_str());
        //         totaldebe += debe;
        cadena.sprintf("%2.2f",atof(cursorasiento->valor("debe").ascii()));
        tapunts->setText(i,COL_DEBE,cadena);
        //         haber = atof(cursorasiento->valor("haber").c_str());
        //         totalhaber += haber;
        cadena.sprintf("%2.2f",atof(cursorasiento->valor("haber").ascii()));
        tapunts->setText(i,COL_HABER,cadena);
        tapunts->setText(i,COL_CONCEPTO,cursorasiento->valor("conceptocontable"));
        tapunts->setText(i,COL_IDBORRADOR,cursorasiento->valor("idborrador"));
        // Vamos a mirar si existe registro de iva para este apunte y lo anotamos.
        query.sprintf("SELECT * FROM registroiva WHERE idborrador=%s",cursorasiento->valor("idborrador").ascii());
        conexionbase->begin();
        cursoriva = conexionbase->cargacursor(query,"cursoriva0");
        conexionbase->commit();
        if (!cursoriva->eof()) {
            tapunts->setText(i,COL_IVA,cursoriva->valor("iva"));
        }// end if
        delete cursoriva;

        // Carga del centro de coste.
        if (cursorasiento->valor("idc_coste") != "") {
            tapunts->setText(i, COL_IDCCOSTE, cursorasiento->valor("idc_coste"));
            query.sprintf("SELECT nombre FROM c_coste WHERE idc_coste=%s",cursorasiento->valor("idc_coste").ascii());
            conexionbase->begin();
            cursoriva = conexionbase->cargacursor(query,"cursorccoste");
            conexionbase->commit();
            if (!cursoriva->eof()) {
                tapunts->setText(i,COL_CCOSTE,cursoriva->valor("nombre"));
            }// end if
            delete cursoriva;
        }// end if

        // Carga del centro del canal
        if (cursorasiento->valor("idcanal") != "") {
            tapunts->setText(i,COL_IDCANAL, cursorasiento->valor("idcanal"));
            query.sprintf("SELECT nombre FROM canal WHERE idcanal=%s",cursorasiento->valor("idcanal").ascii());
            conexionbase->begin();
            cursoriva = conexionbase->cargacursor(query,"cursorcanal");
            conexionbase->commit();
            if (!cursoriva->eof()) {
                tapunts->setText(i,COL_CANAL,cursoriva->valor("nombre"));
            }// end if
            delete cursoriva;
        }// end if


        // Vamos a cargar la contrapartida, que aunque no aparece hay que ponerla para que se pueda usar.
        cadena=cursorasiento->valor("contrapartida");
        if (cadena.stripWhiteSpace() !="") {
            query.sprintf("SELECT * FROM cuenta WHERE idcuenta=%s",cadena.ascii());
            fprintf(stderr,"%s\n",query.ascii());
            conexionbase->begin();
            cursor2 *cursorcontrapartida = conexionbase->cargacursor(query,"cursorcontrapartida");
            conexionbase->commit();
            if (!cursorcontrapartida->eof()) {
                fprintf(stderr,"%s\n", cursorcontrapartida->valor("codigo").ascii());
                tapunts->setText(i,COL_CONTRAPARTIDA,cursorcontrapartida->valor("codigo"));
                tapunts->setText(i,COL_IDCONTRAPARTIDA,cursorcontrapartida->valor("idcuenta"));
            }// end if
            delete cursorcontrapartida;
        }


        // Pasamos al siguiente registro del asiento.
        cursorasiento->siguienteregistro();
        i++;
    }// end while

    idasiento1->setText(ordenAsiento);

    QDate * aux = new QDate();
    //si  datestyle = 'sql, european'
    aux->setYMD(fechaasiento2.mid(6,4).toInt(),fechaasiento2.mid(3,2).toInt(),fechaasiento2.mid(0,2).toInt());

    fechaasiento1->setText(aux->toString("dd/MM/yyyy"));
    delete cursorasiento;
    delete cursoraux;
    calculadescuadre();
}// end repinta

/*******************************************************
 * Esta funcion se encarga de vaciar toda la tabla de apuntes
 * que aparece en el formulario.
 *******************************************************/
void intapunts3view::vaciarapuntes()  {
    tapunts->setNumRows(0);
}// end vaciarapuntes



void intapunts3view::asientoabiertop()  {
    descuadre->setEnabled(TRUE);
    botonborrarasiento->setEnabled(FALSE);
    nuevoasiento->setEnabled(FALSE);
    botonabrirasiento->setEnabled(FALSE);
    botoncerrarasiento->setEnabled(TRUE);
    tapunts->setPaletteBackgroundColor(confpr->valor(CONF_BG_APUNTESA).c_str());
    tapunts->setPaletteForegroundColor(confpr->valor(CONF_FG_APUNTESA).c_str());
    tapunts->setReadOnly(FALSE);
    botoniva->setEnabled(TRUE);
    botoninteligente->setEnabled(TRUE);
    abierto = 1;
}// end asientoabiertop


void intapunts3view::asientocerradop() {
    descuadre->setEnabled(TRUE);
    botonborrarasiento->setEnabled(TRUE);
    nuevoasiento->setEnabled(TRUE);
    botonabrirasiento->setEnabled(TRUE);
    botoncerrarasiento->setEnabled(FALSE);
    abierto = 0;
    tapunts->setPaletteBackgroundColor(confpr->valor(CONF_BG_APUNTES).c_str());
    tapunts->setPaletteForegroundColor(confpr->valor(CONF_FG_APUNTES).c_str());
    tapunts->setReadOnly(TRUE);
    botoniva->setEnabled(FALSE);
    botoninteligente->setEnabled(TRUE);
}// end asientocerradop



/*********************************************************************
 * Esta función se activa cuando se pulsa sobre el boton abrir asiento del
 * formulario
 *********************************************************************/
void intapunts3view::boton_abrirasiento() {
    conexionbase->begin();
    if (idasiento==-1) idasiento=atoi(IDASIENTO);
    if ((conexionbase->abreasiento(idasiento))==42501) QMessageBox::warning( 0, tr("PRIVILEGIOS"), tr("No tiene suficientes privilegios para realizar esta acción."), QMessageBox::Yes, 0);
    conexionbase->commit();
    asientoabiertop();
}// end boton_abrirasiento


/*********************************************************************
 * Esta función se activa cuando se pulsa sobre el boton cerrar asiento del
 * formulario
 *********************************************************************/
void intapunts3view::boton_cerrarasiento() {
    int eleccion;
    int i=0;
    //bool asientoVacio=true;

    guardaborrador(rowactual);
    if (( descuadre->text() != "0.00") && ( descuadre->text() != "-0.00")) {
        eleccion = QMessageBox::information( 0, "Asiento descuadrado, no se puede cerrar", "El asiento no puede guardarse, desea dejarlo abierto ?");
        return;
    }// end if

    // Recorremos la tabla en busca de entradas de iva no introducidas y las preguntamos antes de cerrar nada.
    while (!tapunts->text(i,COL_IDBORRADOR).isNull()) {
        if (tapunts->text(i,COL_IVA).isNull()) {
            QString codcuenta = tapunts->text(i,COL_SUBCUENTA);
            fprintf(stderr,"%s\n",codcuenta.ascii());
            codcuenta = codcuenta.mid(0,3);
            if (codcuenta == "477" || codcuenta == "472") {
                fprintf(stderr,"%s\n",codcuenta.ascii());
                int idborrador = atoi(tapunts->text(i,COL_IDBORRADOR).ascii());
                if (idborrador != 0) {
                    ivaview *nuevae=new ivaview(0,"");
                    if (codcuenta == "477") nuevae->inicializa(conexionbase,2); //IVA Repercutido (Ventas)
                    else nuevae->inicializa(conexionbase,1); //IVA Soportado (Compras)
                    nuevae->inicializa1(idborrador);
                    nuevae->exec();
                    delete nuevae;
                }// end if
            }// end if
        }// end if
        //asientoVacio = asientoVacio && (tapunts->text(i,COL_SUBCUENTA)=="");
        i++;
    }// end while

        // Realizamos la operación en la base de datos.
    if (idasiento==-1) idasiento=atoi(IDASIENTO);
    conexionbase->begin();
    //conexionbase->cierraasiento(atoi(IDASIENTO));
    conexionbase->cierraasiento(idasiento);
    conexionbase->commit();
    asientocerradop();

    // Como pueden haber cambiado cosas, hacemos un repintado para que se actualizen
    repinta(idasiento);
    fechaasiento1->selectAll();
    fechaasiento1->setFocus();
    idasiento=-1;

// Hay un problema con la comprobación
// Se borran asientos que no estan vacios
// Por ejemplo cuando se crea un asiento a partir de un asiento inteligente.
    
//    if (asientoVacio) {
//    if (i==1) {
//        QMessageBox::information( 0, tr("Asiento vacio"), tr("El asiento esta vacio, se procedera a borrarlo !"));
//        borrar_asiento(false); //el valor false indica que no nos muestre el dialogo de confirmación de borrado.
//    }// end if

}// end boton_cerrarasiento


/*********************************************************************
 * Esta función se activa cuando se pulsa sobre el boton nuevo asiento del
 * formulario
 *********************************************************************/
void intapunts3view::boton_nuevoasiento() {
    fechaasiento1->setText(QDate::currentDate().toString("dd/MM/yyyy") );
    iniciar_asiento_nuevo();
}// end boton_nuevoasiento

/*************************************************************************
 * Esta funcion se encarga de hacer las inicializaciones en un asiento nuevo
 ********************************************************************************/
void intapunts3view::iniciar_asiento_nuevo() {
    asientoview *nuevoasiento = new asientoview;
    nuevoasiento->inicializa(conexionbase);
    idasiento = nuevoasiento->creaasiento( fechaasiento1->text(), fechaasiento1->text(),0,1);
    delete nuevoasiento;
    if ( normalizafecha(fechaasiento1->text()).year() == EjercicioActual.toInt() ) {
        cargarcursor(idasiento);
        muestraasiento(idasiento);
    } else flashAsiento(idasiento);
    boton_abrirasiento();

    tapunts->setText(0,0,fechaasiento1->text());
    rowactual=0;
    tapunts->setCurrentCell(0,0);
    tapunts->setFocus();
}// end iniciar_asiento_nuevo


void intapunts3view::contextmenu(int row, int col, const QPoint &poin) {
    QPopupMenu *popup;
    QPopupMenu *menucanal = new QPopupMenu( this );
    QPopupMenu *menucoste = new QPopupMenu( this );
    int opcion;
    cursor2 *cur;
    QString query   ;

    if (abierto) {
        // Si el asiento esta abierto mostramos el popup para asientos abiertos
        popup = new QPopupMenu;
        popup->insertItem(tr("Igual que la anterior (*)"),4);
        switch (col) {
        case COL_CANAL:
            query = "SELECT * FROM canal";
            conexionbase->begin();
            cur = conexionbase->cargacursor(query,"canales");
            menucanal->insertItem(tr("Ninguno"), 1000);
            conexionbase->commit();
            while (!cur->eof()) {
                menucanal->insertItem(cur->valor("nombre"),1000+atoi(cur->valor("idcanal").ascii()));
                cur->siguienteregistro();
            }// end while
            delete cur;
            popup->insertItem(tr("&Seleccionar Canal"),menucanal);
            break;
        case COL_CCOSTE:
            query = "SELECT * FROM c_coste";
            conexionbase->begin();
            cur  = conexionbase->cargacursor(query,"costes");
            menucoste->insertItem(tr("Ninguno"), 1000);
            conexionbase->commit();
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
            repinta(atoi(IDASIENTO));
            tapunts->setCurrentCell(row-1,col);
            break;
        case 3:
            bajarapunte(row);
            repinta(atoi(IDASIENTO));
            tapunts->setCurrentCell(row+1,col);
            break;
        case 4:
            duplicar(col);
            break;
        case 6:
            borraborrador(row);
            break;
        case 5:
            switch(col) {
            case COL_FECHA: {
                    int dia, mes, ano;
                    QList<QDate> a;
                    QString cadena;
                    calendario *cal = new calendario(0,0);
                    cal->exec();
                    a = cal->dn->selectedDates();
                    dia = a.first()->day();
                    mes = a.first()->month();
                    ano = a.first()->year();
                    cadena.sprintf("%2.2d/%2.2d/%d",dia, mes, ano);
                    fprintf(stderr,"Se ha pulsado:%s\n", cadena.ascii());
                    tapunts->setText(row, COL_FECHA, cadena);
                    delete cal;
                    break;
                }// end case
            case COL_NOMCUENTA:
            case COL_SUBCUENTA:
                // Hacemos aparecer la ventana de cuentas
                tapunts->setText(row,COL_SUBCUENTA,"");
                cambiadasubcuenta(row);
                break;
            case COL_CONTRAPARTIDA:
                tapunts->setText(row,COL_CONTRAPARTIDA,"");
                cambiadacontrapartida(row);
                break;
            case COL_DEBE:
                tapunts->setText(row,COL_DEBE, descuadre->text());
                calculadescuadre();
                break;
            case COL_HABER:
                tapunts->setText(row,COL_HABER, descuadre->text());
                calculadescuadre();
                break;
            }// end switch
            break;
        default:
            switch(col) {
            case COL_CANAL:
                if (opcion == 1000) {
                    tapunts->setText(row, COL_CANAL, "");
                    tapunts->setText(row, COL_IDCANAL, "");
                }// end if
                if (opcion > 1000) {
                    QString query1;
                    opcion -= 1000;
                    query1.sprintf("SELECT * FROM canal WHERE idcanal=%d", opcion);
                    conexionbase->begin();
                    cur = conexionbase->cargacursor(query1.ascii(),"canales1");
                    conexionbase->commit();
                    if (!cur->eof()) {
                        tapunts->setText(row,COL_CANAL, cur->valor("nombre"));
                        tapunts->setText(row,COL_IDCANAL, cur->valor("idcanal"));
                    }// end if
                    delete cur;
                }// end if
                break;
            case COL_CCOSTE:
                if (opcion == 1000) {
                    tapunts->setText(row, COL_CCOSTE, "");
                    tapunts->setText(row, COL_IDCCOSTE, "");
                }// end if
                if (opcion > 1000) {
                    QString query1;
                    opcion -= 1000;
                    query1.sprintf("SELECT * FROM c_coste WHERE idc_coste=%d", opcion);
                    conexionbase->begin();
                    cur = conexionbase->cargacursor(query1.ascii(),"canales1");
                    conexionbase->commit();
                    if (!cur->eof()) {
                        tapunts->setText(row,COL_CCOSTE, cur->valor("nombre"));
                        tapunts->setText(row,COL_IDCCOSTE, cur->valor("idc_coste"));
                    }// end if
                    delete cur;
                }// end if
                break;
            }// end switch
            break;
        }// end switch
    } else {
        // Si el asiento esta cerrado el menu a mostrar es diferente
        popup = new QPopupMenu;
        popup->insertItem(tr("Ver Diario (Este dia)"),101);
        popup->insertItem(tr("Ver Diario (Este mes)"),103);
        popup->insertItem(tr("Ver Diario (Este año)"),104);
        popup->insertSeparator();
        popup->insertItem(tr("Ver Extracto (Este dia)"),111);
        popup->insertItem(tr("Ver Extracto (Este mes)"),113);
        popup->insertItem(tr("Ver Extracto (Este año)"),114);
        popup->insertSeparator();
        popup->insertItem(tr("Ver Balance (Este dia)"),121);
        popup->insertItem(tr("Ver Balance (Este mes)"),123);
        popup->insertItem(tr("Ver Balance (Este año)"),124);
        popup->insertSeparator();
        if (col == COL_NOMCUENTA || col == COL_CONTRAPARTIDA || col == COL_SUBCUENTA) {
            popup->insertItem(tr("Editar Cuenta"),130);
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
        case 130:
            // Se ha elegido la opción de editar cuenta.
            // Abrimos la ventana de edición de cuentas.
            QString idcuenta;
            if (col == COL_SUBCUENTA || col == COL_NOMCUENTA) 
                idcuenta = tapunts->text(row,COL_IDCUENTA);
            else
                 idcuenta = tapunts->text(row,COL_IDCONTRAPARTIDA);
            cuentaview *nuevae = new cuentaview(0,"",true);
            nuevae->inicializa(conexionbase);
            nuevae->cargacuenta(atoi(idcuenta.ascii()));
            nuevae->exec();
            delete nuevae;
            repinta(atoi(IDASIENTO));
        }// end switch
        delete popup;
    }// end if
}// end contextmenu


void intapunts3view::apuntecambiadogrid(int row, int col) {
    switch(col) {
    case COL_FECHA: {
            tapunts->setText(row,col,normalizafecha(tapunts->text(row,col)).toString("dd/MM/yyyy"));
            break;
        }
    case COL_SUBCUENTA:
        cambiadasubcuenta(row);
        break;
    case COL_CONTRAPARTIDA:
        cambiadacontrapartida(row);
        break;
    case COL_DEBE: {
            cambiadodebe(row);
            break;
        }// end case
    case COL_HABER: {
            cambiadohaber(row);
            break;
        }// end case
    case COL_CONCEPTO: {
            //         tapunts->setCurrentCell(row+1,COL_FECHA);
            break;
        }// end case
    }// end switch
}// end apuntecambiadogrid


void intapunts3view::tcambiaseleccion() {
    QString codcuenta;
    
    fprintf(stderr,"Canviat a la FILA: %d\n", tapunts->currentRow());
    if (rowactual != tapunts->currentRow() && abierto)
        guardaborrador(rowactual);
    rowactual = tapunts->currentRow();
}// end tcambiaseleccion


void intapunts3view::currentChanged() {
    fprintf(stderr,"currentChanged");
}// end currentChanged()


void intapunts3view::duplicar(int col) {
    if (rowactual >0) {
        if (col == COL_SUBCUENTA || col == COL_NOMCUENTA) {
            tapunts->setText(rowactual,COL_IDCUENTA, tapunts->text(rowactual-1, COL_IDCUENTA));
            tapunts->setText(rowactual, COL_NOMCUENTA, tapunts->text(rowactual-1, COL_NOMCUENTA));
            tapunts->setText(rowactual, COL_SUBCUENTA, tapunts->text(rowactual-1, COL_SUBCUENTA));
        } else if (col == COL_CANAL) {
            tapunts->setText(rowactual, COL_CANAL, tapunts->text(rowactual-1, COL_CANAL));
            tapunts->setText(rowactual, COL_IDCANAL, tapunts->text(rowactual-1, COL_IDCANAL));
        } else if (col == COL_CCOSTE) {
            tapunts->setText(rowactual, COL_CCOSTE, tapunts->text(rowactual-1, COL_CCOSTE));
            tapunts->setText(rowactual, COL_IDCCOSTE, tapunts->text(rowactual-1, COL_IDCCOSTE));
        } else {
            tapunts->setText(rowactual,col, tapunts->text(rowactual-1,col));
        }// end if
    }// end if
}// end duplicar


void intapunts3view::calculadescuadre() {
    double desc=0;
    double deb=0;
    double hab=0;
    QString cadena;
    for (int i=0;i<100;i++) {
        if (!tapunts->text(i,COL_DEBE).isNull()) {
            desc += atof(tapunts->text(i,COL_DEBE).ascii());
            deb  += atof(tapunts->text(i,COL_DEBE).ascii());
        }// end if
        if (!tapunts->text(i,COL_HABER).isNull()) {
            desc-= atof(tapunts->text(i,COL_HABER).ascii());
            hab += atof(tapunts->text(i,COL_HABER).ascii());
        }// end if
    }// end for
    cadena.sprintf("%2.2f",desc);
    descuadre->setText(cadena);
    cadena.sprintf("%2.2f",hab);
    totalhaber1->setText(cadena);
    cadena.sprintf("%2.2f",deb);
    totaldebe1->setText(cadena);
}// end calculadescuadre


// Esta funciono borra el borrador que se le pasa como parametro
// El parametro es el row de la tabla que se quiere borrar.
void intapunts3view::borraborrador(int row) {
    // Para borrar un borrador primero tiene que haber un borrador seleccionado
    if (!tapunts->text(row, COL_IDBORRADOR).isEmpty()) {
        QString idborrador = tapunts->text(row, COL_IDBORRADOR);
        QString query = "DELETE FROM borrador WHERE idborrador="+idborrador;
        conexionbase->begin();
        if (conexionbase->ejecuta(query)==42501) QMessageBox::warning( 0, tr("PRIVILEGIOS"), tr("No tiene suficientes privilegios para realizar esta acción."), QMessageBox::Yes, 0);
        query = "DELETE FROM registroiva WHERE idborrador="+idborrador;
        conexionbase->ejecuta(query);
        conexionbase->commit();
        int rowaux = row;
        while (!tapunts->text(rowaux,COL_IDBORRADOR).isNull()) {
            for (int i=0; i < tapunts->numCols(); i++)
                tapunts->setText(rowaux, i, tapunts->text(rowaux+1, i));
            rowaux++;
        }// end while
        calculadescuadre();
    }// end if
}// end borraborrador


// Esta funcion guarda en la base de datos el borrador de la columna que se ha pasado.
void intapunts3view::guardaborrador(int row) {
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
    int datos=0;
    fprintf(stderr,"guardaborrador: row:%d rowactual:%d\n",row, row);
    if (idasiento==-1) idasiento=atoi(IDASIENTO);
    // Hacemos la recoleccion de datos.
    if ( !tapunts->text(row, COL_IDBORRADOR).isEmpty()) {
        idborrador = tapunts->text(row,COL_IDBORRADOR);
        datos = 1;
    }// end if
    if (!tapunts->text(row, COL_DEBE).isEmpty()) {
        debe = tapunts->text(row, COL_DEBE);
        datos = 1;
    } else {
        debe = "0";
    }// end if
    if (!tapunts->text(row, COL_HABER).isEmpty()) {
        haber = tapunts->text(row, COL_HABER);
        datos = 1;
    } else {
        haber = "0";
    }// end if
    if (!tapunts->text(row,COL_IDCUENTA).isEmpty()) {
        idcuenta = tapunts->text(row, COL_IDCUENTA);
        datos = 1;
    } else {
        idcuenta = "NULL";
    }// end if
    if (!tapunts->text(row,COL_IDCONTRAPARTIDA).isEmpty() && tapunts->text(row,COL_IDCONTRAPARTIDA)!="") {
        contrapartida = tapunts->text(row, COL_IDCONTRAPARTIDA);
        datos = 1;
    } else {
        contrapartida = "NULL";
    }// end if

    if (!tapunts->text(row,COL_FECHA).isEmpty()) {
        fecha = "'"+ tapunts->text(row, COL_FECHA)+"'";
        datos = 1;
    } else {
        fecha = "NULL";
    }// end if

    if (!tapunts->text(row,COL_IDCANAL).isEmpty()) {
        idcanal =  tapunts->text(row, COL_IDCANAL);
        datos = 1;
    } else {
        idcanal = "NULL";
    }// end if

    if (!tapunts->text(row,COL_IDCCOSTE).isEmpty()) {
        idc_coste =  tapunts->text(row, COL_IDCCOSTE);
        datos = 1;
    } else {
        idc_coste = "NULL";
    }// end if

    if (!tapunts->text(row,COL_CONCEPTO).isEmpty()) {
        concepto = "'"+ tapunts->text(row, COL_CONCEPTO)+"'";
        datos = 1;
    } else {
        concepto = "''";
    }// end if
    QString rowtext;
    rowtext.sprintf("%d",row);

    if (datos) {
        if (idborrador != "") {
            // El borrador existe, por lo que solo hay que hacer un update
            query = "UPDATE borrador SET orden="+rowtext+", conceptocontable="+concepto+", fecha="+fecha+", debe="+debe+",haber="+haber+", idcuenta="+idcuenta+", contrapartida="+contrapartida+", idcanal="+idcanal+", idc_coste="+idc_coste+" WHERE idborrador="+idborrador;
            //fprintf(stderr,"%s\n",query.ascii());
            conexionbase->begin();
            if (conexionbase->ejecuta(query)==42501) QMessageBox::warning( 0, tr("PRIVILEGIOS"), tr("No tiene suficientes privilegios para realizar esta acción."), QMessageBox::Yes, 0);
            conexionbase->commit();
        } else {
            // El borrador no existe, por lo que hay que hacer un insert
            //query = "INSERT INTO borrador (orden, conceptocontable, fecha, idcuenta, debe, haber, idasiento, contrapartida, idcanal, idc_coste) VALUES ("+rowtext+","+concepto+", "+fecha+","+idcuenta+","+debe+","+haber+","+idasiento+","+contrapartida+","+idcanal+","+idc_coste+")";
            query.sprintf("INSERT INTO borrador (orden, conceptocontable, fecha, idcuenta, debe, haber, idasiento, contrapartida, idcanal, idc_coste) VALUES (%s,%s,%s,%s,%s,%s,'%d',%s,%s,%s)",rowtext.ascii(),concepto.ascii(),fecha.ascii(),idcuenta.ascii(),debe.ascii(),haber.ascii(),idasiento,contrapartida.ascii(),idcanal.ascii(),idc_coste.ascii());
            //fprintf(stderr,"%s\n",query.ascii());
            conexionbase->begin();
            if (conexionbase->ejecuta(query)==42501) QMessageBox::warning( 0, tr("PRIVILEGIOS"), tr("No tiene suficientes privilegios para realizar esta acción."), QMessageBox::Yes, 0);
            query = "SELECT MAX (idborrador) AS id from borrador";
            cursor2 *cur= conexionbase->cargacursor(query,"cursorm");
            conexionbase->commit();
            tapunts->setText(row, COL_IDBORRADOR,cur->valor("id"));
            delete cur;
        }// end if
    }// end if
}// end guardaborrador


void intapunts3view::duplicarapunte() {
    // Debemos desplazar todos los siguientes para abajo.
    int row = rowactual;
    while (!tapunts->text(row,COL_IDBORRADOR).isNull())
        row++;
    while ( row > rowactual) {
        for (int i=0; i < tapunts->numCols(); i++)
            tapunts->setText(row, i, tapunts->text(row-1, i));
        row --;
    }// end while
    tapunts->setText(rowactual+1, COL_IDBORRADOR, "");
    calculadescuadre();
    guardaborrador(rowactual+1);
}// end duplicarapunte


void intapunts3view::boton_iva() {
    QString codcuenta("xxx");
    int i;
    int sinIVA=0;
    guardaborrador(rowactual);
    if (tapunts->currentRow() > 0)
        if (tapunts->text(rowactual,COL_SUBCUENTA).left(3) == "")
            sinIVA = QMessageBox::information(this, tr("Registro de IVA"),
                         tr("Desea Gestionar una entrada EXENTA de IVA en el registro?"),
                         QMessageBox::Yes,QMessageBox::No);

    // Miramos que haya un row seleccionado.
    tapunts->currentRow() >=0 ? i=rowactual : i=0;

    while (codcuenta != "472" && codcuenta != "477" && codcuenta != "") 
        codcuenta = tapunts->text(i++,COL_SUBCUENTA).left(3);
    if (codcuenta == "" && rowactual > 0) {
        i=0;
        codcuenta="xxx";
        while (codcuenta != "472" && codcuenta != "477" && codcuenta != "")
            codcuenta = tapunts->text(i++,COL_SUBCUENTA).left(3);
    }
    if (codcuenta != "" && sinIVA!=QMessageBox::Yes) {
        tapunts->setCurrentCell(rowactual=i-1,COL_SUBCUENTA);
        int idborrador = tapunts->text(rowactual,COL_IDBORRADOR).toInt();
        codcuenta = tapunts->text(rowactual,COL_SUBCUENTA);
        codcuenta = codcuenta.left(3);
        int es_iva = (codcuenta == "472") + ((codcuenta == "477") * 2);
        if (idborrador != 0) {
             ivaview *nuevae=new ivaview(0,"");
             nuevae->inicializa(conexionbase,es_iva);
             nuevae->inicializa1(idborrador);
             nuevae->exec();
             delete nuevae;
        }// end if
        repinta(atoi(IDASIENTO));
    }
    else {
        if (sinIVA==0) sinIVA = QMessageBox::information(this, tr("Registro de IVA"),
                                          tr("Desea Gestionar una entrada EXENTA de IVA en el registro?"),
                                              QMessageBox::Yes,QMessageBox::No);
        if (sinIVA==QMessageBox::Yes) {
            //Creamos una entrada exenta de iva en el registro de IVA.
            i=0;
            codcuenta="xxx";
            while (codcuenta != "600" && codcuenta != "700" && codcuenta != "") 
                codcuenta = tapunts->text(i++,COL_SUBCUENTA).left(3);
            if (codcuenta != "") {
                --i;
                int idborrador = tapunts->text(i,COL_IDBORRADOR).toInt(); 
                codcuenta = tapunts->text(i,COL_SUBCUENTA);
                codcuenta = codcuenta.left(3);
                int es_iva = (codcuenta == "600") + ((codcuenta == "700") * 2);
                if (idborrador != 0) {
                     ivaview *nuevae=new ivaview(0,"");
                     nuevae->inicializa(conexionbase,es_iva);
                     nuevae->inicializa1(idborrador);
                     nuevae->exec();
                     delete nuevae;
                }// end if
                repinta(atoi(IDASIENTO));
            } else 
                QMessageBox::information(this, tr("Registro de IVA"),
                                          tr("Este Asiento no es de Compra ni Venta"),
                                              QMessageBox::Ok);
        }
    }// end if
}// end boton_iva



void intapunts3view::pulsadomas(int row, int col, int caracter) {
    float tdebe;
    fprintf(stderr,"Se ha pulsado la tecla (%i) sobre tapunts y se ha disparado el evento %d, %d\n",caracter, row, col);
    if (abierto) {
        //Siempre teniendo en cuenta que el asiento este abierto.
        switch (caracter) {
        case '+':
            switch(col) {
            case COL_FECHA: {
                    int dia, mes, ano;
                    QList<QDate> a;
                    QString cadena;
                    calendario *cal = new calendario(0,0);
                    cal->exec();
                    a = cal->dn->selectedDates();
                    dia = a.first()->day();
                    mes = a.first()->month();
                    ano = a.first()->year();
                    cadena.sprintf( "%2.2d/%2.2d/%d",dia, mes, ano);
                    fprintf(stderr,"Se ha pulsado:%s\n", cadena.ascii());
                    tapunts->setText(row, COL_FECHA, cadena);
                    delete cal;
                    break;
                }// end case
            case COL_NOMCUENTA:
            case COL_SUBCUENTA:
                // Vaciamos el campo y simulamos un cambio
                tapunts->setText(row,COL_SUBCUENTA,"");
                cambiadasubcuenta(row);
                break;
            case COL_CONTRAPARTIDA:
                // Vaciamos el campo y simulamos un cambio
                tapunts->setText(row,COL_CONTRAPARTIDA,"");
                buscacontrapartida(row);
                break;
                break;
            case COL_DEBE:
            case COL_HABER:
                tdebe = descuadre->text().toFloat();
                if (tdebe < 0) {
                    tdebe = -tdebe;
                    QString cadena;
                    cadena.sprintf("%2.2f",tdebe);
                    tapunts->setText(row,COL_DEBE, cadena);
                    calculadescuadre();
                } else {
                    tapunts->setText(row, COL_HABER, descuadre->text());
                    calculadescuadre();
                }// end if
                break;
            case COL_CONCEPTO:
                break;
            }// end switch
            break;
        case '*':
            fprintf(stderr,"Se ha pulsado el *\n");
            duplicar(col);
            break;
        case 4100:
            fprintf(stderr,"Se ha pulsado el enter\n");
            switch (col) {
            case COL_FECHA:
                        tapunts->setText(row,col,normalizafecha(tapunts->text(row,col)).toString("dd/MM/yyyy"));
                        tapunts->setCurrentCell(row, COL_SUBCUENTA);
                break;
            case COL_SUBCUENTA:
                tapunts->setCurrentCell(row, COL_DEBE);
                break;
            case COL_CONTRAPARTIDA:
                tapunts->setCurrentCell(row, COL_DEBE);
                break;
            case COL_IVA:
//                tapunts->setText(row+1,COL_FECHA,normalizafecha(fechaasiento1->text()).toString("dd/MM/yyyy"));
                tapunts->setText(row+1,COL_FECHA,normalizafecha(tapunts->text(row,COL_FECHA)).toString("dd/MM/yyyy"));
//                Se ha verificado que es más cómodo poder cambiar la fecha o dar al enter
                tapunts->setCurrentCell(row+1, COL_FECHA);

                break;
            case COL_CONCEPTO:
                tapunts->setCurrentCell(row, COL_IVA);
                break;
            default:
                tapunts->setCurrentCell(row, col+1);
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
                tapunts->setCurrentCell(row, col);
            break;
        case 4117: //Tecla Control + DOWN
            bajarapunte(row);
            tapunts->setCurrentCell(row, col);
            break;
        }// end switch
    }// end if
}// end pulsadomas

void intapunts3view::cambiadasubcuenta(int row) {
    QString subcuenta = tapunts->text(row,COL_SUBCUENTA);
    if (subcuenta == "") {
        // Hacemos aparecer la ventana de cuentas
        listcuentasview1 *listcuentas = new listcuentasview1();
        listcuentas->modo=1;
        listcuentas->inicializa(conexionbase);
        listcuentas->exec();
        tapunts->setText(row,COL_SUBCUENTA,listcuentas->codcuenta);
        tapunts->setText(row,COL_IDCUENTA,listcuentas->idcuenta);
        delete listcuentas;
    }// end if
    // Hacemos la recodificacin
    QString cad = tapunts->text(row,COL_SUBCUENTA);
    if (cad != "") {
        cad = extiendecodigo(cad,numdigitos);
        conexionbase->begin();
        cursor2 *cursorcta = conexionbase->cargacuenta(0, cad );
        conexionbase->commit();
        int num = cursorcta->numregistros();
        if (num >0) {
            tapunts->setText(row,COL_SUBCUENTA,cursorcta->valor(1));
            tapunts->setText(row,COL_NOMCUENTA,cursorcta->valor(2));
            tapunts->setText(row,COL_IDCUENTA,cursorcta->valor("idcuenta"));
        } else {
            QMessageBox::warning( 0, tr("No existe cuenta"), tr("No existe una cuenta con el codigo proporcionado, desea crear una?."), QMessageBox::Yes, QMessageBox::No);
        }// end if
        delete cursorcta;
    }// end if
}// end cambiadasubcuenta

void intapunts3view::buscacontrapartida(int row) {
    QString subcuenta = tapunts->text(row,COL_CONTRAPARTIDA);
    // Hacemos aparecer la ventana de cuentas
    listcuentasview1 *listcuentas = new listcuentasview1();
    listcuentas->modo=1;
    listcuentas->inicializa(conexionbase);
    listcuentas->exec();
    tapunts->setText(row,COL_CONTRAPARTIDA,listcuentas->codcuenta);
    tapunts->setText(row,COL_IDCONTRAPARTIDA,listcuentas->idcuenta);
    delete listcuentas;
}// end buscacontrapartida

void intapunts3view::cambiadacontrapartida(int row) {
    QString subcuenta = tapunts->text(row,COL_CONTRAPARTIDA);
    // Hacemos la recodificacin
    QString cad = tapunts->text(row,COL_CONTRAPARTIDA);
    if (cad != "") {
        cad = extiendecodigo(cad,numdigitos);
        conexionbase->begin();
        cursor2 *cursorcta = conexionbase->cargacuenta(0,cad );
        conexionbase->commit();
        if (!cursorcta->eof()) {
            tapunts->setText(row,COL_CONTRAPARTIDA,cursorcta->valor(1));
            tapunts->setText(row,COL_IDCONTRAPARTIDA,cursorcta->valor("idcuenta"));
        } else {
            QMessageBox::warning( 0, tr("No existe cuenta"), tr("No existe una cuenta con el codigo proporcionado, desea crear una?."), QMessageBox::Yes, QMessageBox::No);
        }// end if
        delete cursorcta;
    } else {
        tapunts->setText(row,COL_IDCONTRAPARTIDA,"");
        tapunts->setText(row,COL_CONTRAPARTIDA,"");
    }// end if

}// end cambiadasubcuenta

void intapunts3view::cambiadodebe(int row) {
    QString debe;
    QString cad;
    float ndebe;
    debe =  tapunts->text(row,COL_DEBE);
    if (debe != "") {
        ndebe =atof(debe.ascii());
        if (ndebe > 0.01) {
            tapunts->setText(row,COL_HABER,"0.00");
        }// end if
        cad.sprintf("%2.2f",ndebe);
        tapunts->setText(row,COL_DEBE,cad);
    }// end if
    calculadescuadre();
}// end cambiadodebe


void intapunts3view::cambiadohaber(int row) {
    QString haber;
    QString cad;
    float nhaber;
    haber =  tapunts->text(row,COL_HABER);
    if (haber != "") {
        nhaber =atof(haber.ascii());
        if (nhaber > 0.01) {
            tapunts->setText(row,COL_DEBE,"0.00");
        }// end if
        cad.sprintf("%2.2f",nhaber);
        tapunts->setText(row,COL_HABER,cad);
    }// end if
    calculadescuadre();
}// end cambiadohaber

void intapunts3view::boton_inteligente() {
    int numasiento;
    if (abierto==1) {
        // El asiento esta abierto y por tanto se muestra como abierto
        asientoabiertop();
        numasiento = atoi(IDASIENTO);
    } else {
        numasiento = 0;
    }// end if
    //  int idasiento = atoi(cursorasientos->valor("idasiento").c_str());
    aplinteligentesview *nueva=new aplinteligentesview(0,"");
    nueva->inicializa(conexionbase, numasiento, this);
    nueva->exec();
    //  numasiento = nueva->numasiento;
    //  if (numasiento != 0)
    //  muestraasiento(idasiento);
    // repinta();
    delete nueva;
}// end boton_inteligente


/*********************************************************************
 * Esta función se activa cuando se pulsa sobre el boton nuevo asiento del
 * formulario
 *********************************************************************/
void intapunts3view::boton_buscacuenta() {
    tapunts->setText(rowactual, COL_SUBCUENTA,"");
    cambiadasubcuenta(rowactual);
}// end boton_buscacuenta



// Si el parametro pasado es un:
// 0 -> del dia actual
// 1 -> del mes actual
// 2 -> del año actual
void intapunts3view::boton_extracto1(int tipo) {
    QDate fecha1, fecha2, fechaact;
    if(!tapunts->text(rowactual, COL_FECHA).isEmpty()) {
        fechaact = normalizafecha(tapunts->text(rowactual, COL_FECHA));
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
        extracto->inicializa1(tapunts->text(rowactual, COL_SUBCUENTA), tapunts->text(rowactual, COL_SUBCUENTA),fecha1.toString("dd/MM/yyyy"), fecha2.toString("dd/MM/yyyy"), 0);
    }// end if
    extracto->accept();
    extracto->show();
    extracto->setFocus();
}// end boton_extracto1



// Si el parametro pasado es un:
// 0 -> del dia actual
// 1 -> del mes actual
// 2 -> del año actual
void intapunts3view::boton_diario1(int tipo) {
    QDate fecha1, fecha2, fechaact;
    if(!tapunts->text(rowactual, COL_FECHA).isEmpty()) {
        fechaact = normalizafecha(tapunts->text(rowactual, COL_FECHA) );
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
        fprintf(stderr,"la fecha actual vale:%s\n", fechaact.toString("dd/MM/yyyy").ascii());
        fprintf(stderr,"el diario va a ir de %s a %s\n",fecha1.toString("dd/MM/yyyy").ascii(), fecha2.toString("dd/MM/yyyy").ascii());
        diario->inicializa1( fecha1.toString("dd/MM/yyyy"), fecha2.toString("dd/MM/yyyy"), 0);
    }// end if
    diario->accept();
    diario->show();
    diario->setFocus();
}// end boton_diario1


// Si el parametro pasado es un:
// 0 -> del dia actual
// 1 -> del mes actual
// 2 -> del año actual
void intapunts3view::boton_balance1(int tipo) {
    QDate fecha1, fecha2, fechaact;
    if(!tapunts->text(rowactual, COL_FECHA).isEmpty()) {
        fechaact = normalizafecha(tapunts->text(rowactual, COL_FECHA) );
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
        balance->inicializa1(tapunts->text(rowactual, COL_SUBCUENTA), tapunts->text(rowactual, COL_SUBCUENTA), fecha1.toString("dd/MM/yyyy"), fecha2.toString("dd/MM/yyyy"), 0);
    }// end if
    balance->accept();
    balance->show();
    balance->setFocus();
}// end boton_balance1


/****************************************************************
 * Esta funcion se invoca al pulsar sobre el boton de filtrado
 * Sirve para filtrar los asientos introducidos
 * El objeto filt se crea con el constructor de intapuntsview.
 ****************************************************************/
void intapunts3view::boton_filtrar() {
    QString cadena;
    filt->exec();
    cadena =  filt->cantidadapunte->text();
    cargarcursor(0);
    boton_inicio();
}// end boton_filtrar


void intapunts3view::return_fechaasiento() {
    QString query;
    int resultado;
    fechaasiento1->setText(normalizafecha(fechaasiento1->text()).toString("dd/MM/yyyy"));
    if (abierto) { //cambiar la fecha del asiento
        conexionbase->begin();
        query.sprintf("UPDATE asiento SET fecha='%s' WHERE idasiento='%s'",fechaasiento1->text().ascii(),IDASIENTO);
        fprintf(stderr,"%s\n",query.ascii());
        resultado = conexionbase->ejecuta(query);
        if (resultado != 0 and resultado != 42501) {
            conexionbase->rollback();
        } else {
            conexionbase->commit();
        }// end if
    } else { iniciar_asiento_nuevo(); }
}// end return_fechaasiento


int intapunts3view::inicializa1(extractoview1 *ext, diarioview1 *diar, balanceview *bal ) {
    extracto=ext;
    diario=diar;
    balance= bal;
    return(0);
}// end inicializa1



void intapunts3view::asiento_cierre() {
    int idcuenta;
    int numasiento;
    double diferencia;
    double nuevodebe, nuevohaber;
    QString nfecha = fechaasiento1->text();
    nfecha.sprintf("31/12/%s",EjercicioActual.ascii());
    asientoview *nuevoasiento = new asientoview;
    nuevoasiento->inicializa(conexionbase);
    numasiento = nuevoasiento->creaasiento( "Asiento de Cierre" ,nfecha ,0, 99); //99=>Cierre.
    QString query ="SELECT idcuenta, sum(debe) AS sumdebe, sum(haber) AS sumhaber, sum(debe)-sum(haber) AS saldito FROM apunte WHERE idcuenta NOT IN (SELECT idcuenta FROM cuenta WHERE idgrupo=6 OR idgrupo=7) GROUP BY idcuenta ORDER BY saldito";
    conexionbase->begin();
    cursor2 *cursor=conexionbase->cargacursor(query, "cursor");
    conexionbase->commit();
    while (! cursor->eof()) {
        idcuenta = atoi(cursor->valor("idcuenta").ascii());
        diferencia = atof(cursor->valor("sumdebe").ascii())-atof(cursor->valor("sumhaber").ascii());
        if (diferencia > 0) {
            nuevohaber = diferencia;
            nuevodebe= 0;
        } else {
            nuevodebe = -diferencia;
            nuevohaber=0;
        }// end if
        if (nuevodebe != nuevohaber) {
            conexionbase->nuevoborrador(idcuenta, numasiento,"Asiento de Cierre","", nuevodebe, nuevohaber,  nfecha, 0, 1, 0, 0);
        }// end if
        cursor->siguienteregistro();
    }// end while
    delete nuevoasiento;
    conexionbase->begin();
    conexionbase->cierraasiento(numasiento);
    conexionbase->commit();
    cargarcursor(numasiento);
    muestraasiento(numasiento);
}// end asiento_cierre


void intapunts3view::asiento_apertura() {
  cursor2 *cur=NULL;
  int idcuenta;
  int numasiento=0;
  double nuevodebe, nuevohaber;
  QString nfecha = normalizafecha("02/01/"+EjercicioActual).toString("dd/MM/yyyy");
  asientoview *nuevoasiento = new asientoview(this,"apertura");
  //Buscamos en la tabla ejercicios si existe un ejercicio anterior, si no existe usaremos la metaDB.
  QString query;
  postgresiface2 * DBconnEjActual = new postgresiface2();
  DBconnEjActual->inicializa(empresaactual->nombreDB);
  DBconnEjActual->begin();
  query.sprintf("SELECT * FROM ejercicios WHERE periodo='0' AND ejercicio='%d'",EjercicioActual.toInt()-1);
  cur = DBconnEjActual->cargacursor(query,"EjAnterior");
  DBconnEjActual->commit();
  if (!cur->eof()) { //Tenemos el Ejercicio anterior en la misma base de datos que el ejercicio actual
      DBconnEjActual->begin();
      query.sprintf("SELECT idasiento FROM asiento WHERE clase='99' AND EXTRACT(YEAR FROM fecha)='%d'",EjercicioActual.toInt()-1);
      cur = DBconnEjActual->cargacursor(query, "asiento99");
      query = "SELECT * FROM borrador WHERE idasiento =" + cur->valor("idasiento");
      cur = DBconnEjActual->cargacursor(query, "cierre");
      DBconnEjActual->commit();
      if (cur->eof()) cur=NULL;
      //El asiento de cierre del ejercicio anterior lo tenemos en "cur"
  }
  else { //No hemos encontrado el ejercicio anterior en la tabla ejercicios
         //Buscamos el nombre de la base de datos que contiene el ejercicio anterior en metaDB
      postgresiface2 *metabase = new postgresiface2();
      metabase->inicializa(confpr->valor(CONF_METABASE).c_str());
      metabase->begin();
      query.sprintf("SELECT * FROM empresa WHERE nombredb='%s'",empresaactual->nombreDB.ascii());
      cur = metabase->cargacursor(query,"empresa");
      metabase->commit();
      delete metabase;
      if (!cur->eof()) {
          if (cur->valor("ejant") != "") {
              //**************************************************************
              //Cargamos en un cursor el asiento de cierre del ejercicio anterior, que se supone es el ultimo asiento introducido.
              //***************************************************************
              postgresiface2 * DBconnEjAnt = new postgresiface2();
              DBconnEjAnt->inicializa(cur->valor("ejant"));
              DBconnEjAnt->begin();
              query="SELECT max(idasiento) as ultimo FROM asiento";
              cur = DBconnEjAnt->cargacursor(query, "max");
              query = "SELECT * FROM borrador WHERE idasiento ="+cur->valor("ultimo");
              cur = DBconnEjAnt->cargacursor(query, "cierre");
              DBconnEjAnt->commit();
              delete DBconnEjAnt;
              //**************************************************************
              //Fin de la connexión con el ejercicio anterior,
              //El asiento de cierre del ejercicio anterior lo tenemos en "cur"
              //***************************************************************
          }
          else cur=NULL; 
      }
  }
  if (cur != NULL) {  
  //Comprovamos si ya existe un asiento de apertura i la borramos
  DBconnEjActual->begin();
  query.sprintf("SELECT idasiento, ordenasiento FROM asiento WHERE EXTRACT(YEAR FROM fecha)='%s' AND clase='0'",EjercicioActual.ascii());
  cursor2 *aux = DBconnEjActual->cargacursor(query,"aux");
  if (!aux->eof()) {
      numasiento = aux->valor(0).toInt();
      query.sprintf("DELETE FROM apunte where idasiento=%d",numasiento);
      DBconnEjActual->ejecuta(query);
      query.sprintf("DELETE FROM borrador where idasiento=%d",numasiento);
      DBconnEjActual->ejecuta(query);
      DBconnEjActual->borrarasiento(numasiento);
  }
  delete aux;
  // Creamos un nuevo asiento, que sera el asiento de apertura.
     nuevoasiento->inicializa(DBconnEjActual);
     numasiento = nuevoasiento->creaasiento( "Asiento de Apertura", nfecha,0,0); //0=> Apertura
     while (!cur->eof()) {
         nuevodebe= cur->valor("haber").toDouble();
         nuevohaber = cur->valor("debe").toDouble();
         idcuenta = cur->valor("idcuenta").toInt();
         DBconnEjActual->nuevoborrador(idcuenta, numasiento,"Asiento de Apertura","", nuevodebe, nuevohaber, nfecha, 0, 1, 0, 0);
         cur->siguienteregistro();
     }// end while
     DBconnEjActual->begin();
     DBconnEjActual->cierraasiento(numasiento);
     DBconnEjActual->commit();
     cargarcursor(numasiento);
     muestraasiento(numasiento);
  }      
  else {
       QMessageBox::information( this, tr("Asiento de Apertura"), tr("No se ha podido encontrar el ejercicio anterior.\n\r El Asiento de Apertura tendrá que ser entrado manualmente."), QMessageBox::Ok);
  }
  
  delete cur;
  delete DBconnEjActual;
}// end asiento_apertura


void intapunts3view::asiento_regularizacion() {
    int idcuenta;
    int idcuenta1;
    int numasiento=0;
    double diferencia;
    double totaldebe, totalhaber;
    double totaldebe1 =0, totalhaber1=0;
    QString nfecha = fechaasiento1->text();
    nfecha.sprintf("31/12/%s",EjercicioActual.ascii());
    asientoview *nuevoasiento = new asientoview;
    nuevoasiento->inicializa(conexionbase);
    numasiento = nuevoasiento->creaasiento(tr("Asiento de Regularización") ,nfecha,0,98); //98=regularizacion
    QString query = "SELECT * FROM cuenta where codigo ='129'";
    conexionbase->begin();
    cursor2 *cur = conexionbase->cargacursor(query,"idcuenta");
    conexionbase->commit();
    idcuenta1 = atoi(cur->valor("idcuenta").ascii());
    delete cur;
    query = "SELECT idcuenta, sum(debe) AS sumdebe, sum(haber) AS sumhaber, sum(debe)-sum(haber) AS saldito from apunte WHERE idcuenta IN (SELECT idcuenta FROM cuenta where idgrupo=6 OR idgrupo=7) GROUP BY idcuenta ORDER BY saldito";
    fprintf(stderr,"%s\n",query.ascii());
    conexionbase->begin();
    cur = conexionbase->cargacursor(query,"cursor");
    conexionbase->commit();
    while (!cur->eof()) {
        idcuenta = atoi(cur->valor("idcuenta").ascii());
        //    totaldebe = atof(cur->valor("sumdebe").c_str());
        //    totalhaber = atof(cur->valor("sumhaber").c_str());
        diferencia = atof(cur->valor("sumdebe").ascii())-atof(cur->valor("sumhaber").ascii());
        if (diferencia > 0) {
            totalhaber = diferencia;
            totaldebe= 0;
        } else {
            totaldebe = -diferencia;
            totalhaber=0;
        }// end if
        totaldebe1 += totaldebe;
        totalhaber1 += totalhaber;

        conexionbase->nuevoborrador(idcuenta, numasiento,tr("Asiento de Regularizacion"),"", totaldebe, totalhaber,  nfecha, 0, 1, 0, 0);
        cur->siguienteregistro();
    }// end while
    delete cur;
    conexionbase->nuevoborrador(idcuenta1, numasiento,tr("Asiento de Regularizacion"),"", 0, totaldebe1,  nfecha, 0, 1, 0, 0);
    conexionbase->nuevoborrador(idcuenta1, numasiento,tr("Asiento de Regularizacion"),"", totalhaber1, 0, nfecha, 0, 1, 0, 0);
    conexionbase->begin();
    conexionbase->cierraasiento(numasiento);
    conexionbase->commit();
    cargarcursor(numasiento);
    muestraasiento(numasiento);
}// end asiento_regularizacion


void intapunts3view::return_numasiento() {
    boton_cargarasiento();
}// end return_cuenta

//**************************************************************
//* Se ha pulsado sobre el boton de cargar asiento con lo
//* que debemos comprobar que el numero introducido es correcto
//* y hacer las gestiones oportunas para mostrar el asiento en
//* pantalla o crearlo si hace falta.
//**************************************************************/
void intapunts3view::boton_cargarasiento() {
    QString query;
    int IdAsien=0;
    conexionbase->begin();
    query.sprintf("SELECT idasiento FROM asiento WHERE ordenasiento = %d AND EXTRACT(YEAR FROM fecha)=%s ",idasiento1->text().toInt(),EjercicioActual.ascii());
    cursor2 *curs = conexionbase->cargacursor(query, "micursor");
    conexionbase->commit();
    if (!curs->eof()) {
        IdAsien = atoi(curs->valor("idasiento").ascii());
        cargarcursor(IdAsien);
        muestraasiento(IdAsien);
    }// end if
    delete curs;
}// end boton_cargarasiento

//*********************************************************************
//* Esta función se activa cuando se pulsa sobre el boton borrar asiento
//* del formulario
//*********************************************************************/
void intapunts3view::boton_borrar_asiento() {
borrar_asiento(true);
}// end boto_borrar_asiento

void intapunts3view::borrar_asiento(bool confirmarBorrado) {
    QString query;
    int valor;
    int resultado;
    if (atoi(IDASIENTO) != 0) {
        if (confirmarBorrado) {
            valor = QMessageBox::warning( 0, "Borrar Asiento", "Se procedera a borrar el asiento.", QMessageBox::Yes, QMessageBox::No);
        } else { valor = QMessageBox::Yes ; }
        if (valor ==  QMessageBox::Yes) {
            conexionbase->begin();
            query.sprintf("DELETE FROM apunte where idasiento=%s",IDASIENTO);
            if ((resultado = conexionbase->ejecuta(query))==42501) QMessageBox::warning( 0, tr("PRIVILEGIOS"), tr("No tiene suficientes privilegios para realizar esta acción."), QMessageBox::Yes, 0);
            query.sprintf("DELETE FROM registroiva where idborrador IN (SELECT idborrador FROM borrador WHERE idasiento=%s)",IDASIENTO);
            resultado += conexionbase->ejecuta(query);
            query.sprintf("DELETE FROM borrador where idasiento=%s",IDASIENTO);
            resultado += conexionbase->ejecuta(query);
            resultado += conexionbase->borrarasiento(atoi(IDASIENTO));
            
            if (resultado != 0) {
                conexionbase->rollback();
            } else {
                conexionbase->commit();
            }// end if
            cargarcursor(atoi(IDASIENTO));
            repinta(atoi(IDASIENTO));
        }// end if
    }// end if
} //fin borrar_asiento

/******************************************************************
* Esta se encarga de la edicion de asientos.                      *
****************************************************************/
void intapunts3view::editarasiento() {
    guardaborrador(rowactual);
    asientoview *nuevoasiento= new asientoview(0,"",true);
    nuevoasiento->inicializa(conexionbase);
    nuevoasiento->cargaasiento(atoi(IDASIENTO));
    nuevoasiento->exec();
    cargarcursor(atoi(IDASIENTO));
    repinta(atoi(IDASIENTO));
    // muestraasiento(atoi(IDASIENTO));
}// end editarasiento



void intapunts3view::subirapunte(int row) {
    if (row > 0 ) {
        tapunts->swapRows(row, row-1);
        guardaborrador(row);
        guardaborrador(row-1);
    }// end if
}// end subirapunte


void intapunts3view::bajarapunte(int row) {
    if (!tapunts->text(row+1, COL_IDBORRADOR).isNull()) {
        tapunts->swapRows(row, row+1);
        guardaborrador(row);
        guardaborrador(row+1);
    }// end if
}// end subirapunte


void intapunts3view::fechaasiento1_textChanged( const QString & texto ) {
    if (texto=="+") {
        QList<QDate> a;
        fechaasiento1->setText("");
        calendario *cal = new calendario(0,0);
        cal->exec();
        a = cal->dn->selectedDates();
        fechaasiento1->setText(a.first()->toString("dd/MM/yyyy"));
        delete cal;
    }
    if (texto=="*")
        fechaasiento1->setText(QDate::currentDate().toString("dd/MM/yyyy") );
}//fin fechaasiento1_textChanged( const QString &texto )


void intapunts3view::boton_duplicarasiento() {
    duplicarasientoview *dupli= new duplicarasientoview(empresaactual,0,"",true);
    // Establecemos los parametros para el nuevo asiento a duplicar
    dupli->inicializa(idasiento1->text(), idasiento1->text());
    dupli->exec();
    cargarcursor(-1);
    boton_fin();
    repinta(atoi(IDASIENTO));
    delete dupli;
}// end boton_duplicarasiento

void intapunts3view::boton_fecha() {
    fechaasiento1->setText("+");
}// end boton_fecha

