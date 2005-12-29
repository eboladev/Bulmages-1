/***************************************************************************
                          asientosview.cpp  -  description
                          --------------------------------
    begin                : Sat Dec 21 2002
    copyright            : (C) 2002 by Tomeu Borr� Riera
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

#include "asientosview.h"
//#include <qsqldatabase.h>

#define COL_IDASIENTO 5
#define COL_FECHA 1
#define COL_DEBE 2
#define COL_HABER 3
#define COL_CERRADO 4
#define COL_ORDENASIENTO 0


/** El constructor de la clase inicializa algunas estructuras y configura la visi�
  * de la pantalla.
  */
asientosview::asientosview(empresa *emp,QWidget *parent, const char *name, bool modal) : asientosdlg(parent,name,modal) {
    empresaactual=emp;
    tablaasientos->setNumCols(6);
    tablaasientos->horizontalHeader()->setLabel( COL_IDASIENTO, tr( "Num." ) );
    tablaasientos->horizontalHeader()->setLabel( COL_FECHA, tr( "Fecha" ) );
    tablaasientos->horizontalHeader()->setLabel( COL_DEBE, tr( "Debe" ) );
    tablaasientos->horizontalHeader()->setLabel( COL_HABER, tr( "Haber" ) );
    tablaasientos->horizontalHeader()->setLabel( COL_CERRADO, tr( "Cerrado" ) );
    tablaasientos->horizontalHeader()->setLabel( COL_ORDENASIENTO, tr( "Orden" ) );
    tablaasientos->setColumnWidth(COL_IDASIENTO,75);
    tablaasientos->setColumnWidth(COL_FECHA,100);
    tablaasientos->setColumnWidth(COL_DEBE,75);
    tablaasientos->setColumnWidth(COL_HABER,75);
    tablaasientos->setColumnWidth(COL_CERRADO,75);
    tablaasientos->setColumnWidth(COL_ORDENASIENTO,75);
    tablaasientos->hideColumn(COL_IDASIENTO);
    tablaasientos->setRowMovingEnabled( TRUE );
    tablaasientos->setColumnMovingEnabled( TRUE );
    tablaasientos->setSorting( TRUE );
    tablaasientos->setSelectionMode( Q3Table::SingleRow );

}// end asientosview


asientosview::~asientosview() {}// end ~asientosview


/** Se ha pulsado sobre la tabla de asientos, con lo que se carga en introapunts
  * el asiento seleccionado y se cierra la ventana.
  */
void asientosview::pulsado(int a, int , int ,const QPoint &) {
    int idasiento;
    idasiento = atoi(tablaasientos->text(a,COL_IDASIENTO).ascii());
    introapunts->cargarcursor();
    introapunts->muestraasiento(idasiento);
    introapunts->show();
    introapunts->setFocus();
    done(1);
}// end pulsado


/** Inicializa la ventana, haciendo la consulta pertinente a la base de datos
  * y presentando los resultados en pantalla.
  */
void asientosview::inicializa(postgresiface2 *conn, intapunts3view *inta) {
    introapunts = inta;
    conexionbase = conn;
    /// Hacemos que tengan tanto introapuntes como asientosview el mismo filtro asi son compatibles uno con otro.
    filt = inta->filt;
    QString cantapunt = filt->cantidadapunte->text().ascii();
    QString saldototal = filt->saldoasiento->text().ascii();
    QString nombreasiento = filt->nombreasiento->text().ascii();
    QString ejercicio = filt->ejercicio();
    QString query;
    QString cadwhere;
    QString textsaldototal = "";
    QString textcantapunt = "";
    QString textnombreasiento= "";
    QString textejercicio = "";
    int pand=0;
    /// Componemos el query a partir de la parte de filtrado.
    if (saldototal != "") {
        cadwhere = " WHERE ";
        textsaldototal = " asiento.idasiento IN (SELECT idasiento FROM (SELECT idasiento, sum(debe) AS total from apunte GROUP BY idasiento) AS foo WHERE foo.total="+saldototal+")";
        pand = 1;
    }// end if
    if (cantapunt != "" ) {
        cadwhere = " WHERE ";
        if (pand)
            textcantapunt = " AND ";
        textcantapunt += " asiento.idasiento IN (SELECT idasiento FROM apunte where debe="+cantapunt+" OR haber = "+cantapunt+")";
        pand = 1;
    }// end if
    if (nombreasiento != "") {
        cadwhere = " WHERE ";
        if (pand)
            textnombreasiento = " AND ";
        textnombreasiento += " asiento.idasiento in (SELECT idasiento FROM apunte WHERE conceptocontable LIKE '%"+nombreasiento+"%' )";
        pand = 1;
    }// end if
    if (ejercicio != "--") {
        if (pand)
            textejercicio = " AND EXTRACT(YEAR FROM fecha)='"+ ejercicio +"'";
        else
            textejercicio = " WHERE EXTRACT(YEAR FROM fecha)='"+ ejercicio +"'";
    }// end if
    query = "SELECT asiento.ordenasiento, asiento.idasiento, asiento.fecha,  totaldebe, totalhaber, numap, numborr   from asiento  LEFT JOIN (SELECT count(idborrador) AS numborr, idasiento FROM borrador GROUP BY idasiento) as foo1 ON foo1.idasiento = asiento.idasiento LEFT JOIN (SELECT sum(debe) as totaldebe, sum(haber) as totalhaber, count(idapunte) as numap, idasiento from apunte group by idasiento) as fula ON asiento.idasiento = fula.idasiento   "+cadwhere+textsaldototal+textcantapunt+textnombreasiento+textejercicio+" ORDER BY EXTRACT (YEAR FROM asiento.fecha), asiento.ordenasiento";
    cursor2 *cursoraux= conexionbase->cargacursor(query);
    int numreg = cursoraux->numregistros();
    tablaasientos->setNumRows(numreg);
    int i =0;
    while (!cursoraux->eof()) {
        tablaasientos->setText(i,COL_ORDENASIENTO,cursoraux->valor("ordenasiento"));
        tablaasientos->setText(i,COL_IDASIENTO,cursoraux->valor("idasiento"));
        tablaasientos->setText(i,COL_FECHA,cursoraux->valor("fecha").mid(0,10));
        tablaasientos->setText(i,COL_DEBE,cursoraux->valor("totaldebe"));
        tablaasientos->setText(i,COL_HABER,cursoraux->valor("totalhaber"));

        /// Para determinar si un asiento est�abierto o no hay que saber que el nmero de apuntes de un asiento abierto es cero.
        if (cursoraux->valor("numap").toInt() > 0 ) {
            tablaasientos->setText(i,COL_CERRADO,tr("Cerrado"));
        } else {
            tablaasientos->setText(i,COL_CERRADO,tr("Abierto"));
        }// end if
        i++;
        cursoraux->siguienteregistro();
    }// end while
    delete cursoraux;
}// end inicializa



/****************************************************************
 * Esta funcion se invoca al pulsar sobre el boton de filtrado
 * Sirve para filtrar los asientos introducidos
 * El objeto filt se crea con el constructor de intapuntsview.
 ****************************************************************/
void asientosview::boton_filtrar() {
    //   char *cadena;
    filt->exec();
    //   cadena = (char *) filt->cantidadapunte->text().ascii();
    inicializa(conexionbase, introapunts);
}// end boton_filtrar
