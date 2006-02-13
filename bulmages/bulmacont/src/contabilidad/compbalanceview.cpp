/***************************************************************************
                          compbalanceview.cpp  -  description
                             -------------------
    begin                : s� sep 13 2003
    copyright            : (C) 2003 by Tomeu Borr� Riera
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

#include "compbalanceview.h"
#include <qradiobutton.h>
#include "mpatrimonialesview.h"
#include "mpatrimonialview.h"
#include "empresa.h"

#include "balancesprintview.h"

#include <qlineedit.h>
#include <q3table.h>
#include <qcombobox.h>

#define COL_IDCOMPBALANCE  0
#define COL_DESCRIPCION    2
#define COL_IDMPATRIMONIAL 1
#define COL_ORDEN          3
#define COL_TABULACION     4
#define COL_SALDO          5

compbalanceview::compbalanceview(empresa * emp,QWidget *parent, const char *name ) : compbalancedlg(parent,name) {
    empresaactual = emp;
    conexionbase =emp->bdempresa();
}


compbalanceview::~compbalanceview() {}


void compbalanceview::inicializa1(QString idbalancep) {
    idbalance = idbalancep;
    QString query = "SELECT * FROM balance WHERE idbalance="+idbalance;
    conexionbase->begin();
    cursor2 *mycursor = conexionbase->cargacursor(query,"balancequery");
    conexionbase->commit();
    if (!mycursor->eof()) {
        titbalance->setText(mycursor->valor("nombrebalance"));
    }// end if
    delete mycursor;

    // Hacemos la inicializacion del listado.
    listado->setNumCols(6);

    listado->horizontalHeader()->setLabel( COL_IDCOMPBALANCE, tr( "identificador" ) );
    listado->horizontalHeader()->setLabel( COL_DESCRIPCION, tr( "concepto" ) );
    listado->horizontalHeader()->setLabel( COL_IDMPATRIMONIAL, tr( "nmasa" ) );
    listado->horizontalHeader()->setLabel( COL_ORDEN, tr( "orden" ) );
    listado->horizontalHeader()->setLabel( COL_TABULACION, tr( "tabulacion" ) );
    listado->horizontalHeader()->setLabel( COL_SALDO, tr( "saldo") );

    listado->setColumnWidth(COL_IDCOMPBALANCE,25);
    listado->setColumnWidth(COL_DESCRIPCION,450);
    listado->setColumnWidth(COL_IDMPATRIMONIAL,35);
    listado->setColumnWidth(COL_ORDEN,25);
    listado->setColumnWidth(COL_TABULACION,25);
    listado->setColumnWidth(COL_SALDO,25);

    listado->hideColumn(COL_IDCOMPBALANCE);
    listado->hideColumn(COL_ORDEN);
    listado->hideColumn(COL_TABULACION);
    listado->hideColumn(COL_SALDO);

    //  listado->setColumnWidth(2,10);

    //   query = "SELECT *, saldompatrimonial (idmpatrimonial,'01/01/2003','31/12/2003') AS saldot FROM compbalance WHERE idbalance = "+idbalance+" ORDER BY orden";
    query = "SELECT *, compbalance.idbalance AS saldot FROM compbalance WHERE idbalance = "+idbalance+" ORDER BY orden";
    fprintf(stderr,"%s\n",query.ascii());
    conexionbase->begin();
    mycursor = conexionbase->cargacursor(query, "compbalancequery");
    conexionbase->commit();
    listado->setNumRows(mycursor->numregistros());
    int i=0;
    while (!mycursor->eof()) {
        int orden = atoi (mycursor->valor("tabulacion").ascii());
        QString texto = "";
        for (int j=0; j<orden; j++)
            texto += "   ";
        listado->setText(i,COL_IDCOMPBALANCE,mycursor->valor("idcompbalance"));
        listado->setText(i,COL_DESCRIPCION,(texto + mycursor->valor("concepto")));
        listado->setText(i,COL_IDMPATRIMONIAL,mycursor->valor("idmpatrimonial"));
        listado->setText(i,COL_ORDEN,mycursor->valor("orden"));
        listado->setText(i,COL_TABULACION,mycursor->valor("tabulacion"));
        listado->setText(i,COL_SALDO,mycursor->valor("saldot"));
        i++;
        mycursor->siguienteregistro();
    }// end while
    delete mycursor;

}// end inicializa1



void compbalanceview::borralinea() {
    int row = listado->currentRow();
    if (row > 0 && listado->numRows() > 0) {
        string idcompbalance = listado->text(row, COL_IDCOMPBALANCE).ascii();
        string idmasa = listado->text(row, COL_IDMPATRIMONIAL).ascii();
        QString query ;
        query.sprintf("DELETE FROM compmasap WHERE masaperteneciente = %s",idmasa.c_str());
        conexionbase->begin();
        conexionbase->ejecuta(query);
        query.sprintf("DELETE FROM mpatrimonial WHERE idmpatrimonial = %s",idmasa.c_str());
        conexionbase->ejecuta(query);
        query.sprintf("DELETE FROM compbalance WHERE idcompbalance = %s", idcompbalance.c_str());
        conexionbase->ejecuta(query);
        conexionbase->commit();
        inicializa1(idbalance);
        if (row > 1)
            listado->setCurrentCell(row-1,0);
    }// end if
}// end borralinea


void compbalanceview::agregalinea() {
    QString elorden;
    int row = listado->currentRow();
    if (row > 0 && listado->numRows() > 0) {
        elorden = listado->text(row,COL_ORDEN).ascii();
    } else {
        elorden = "0";
    }// end if
    if (mdb_idmpatrimonial == "") {
        mdb_idmpatrimonial = "NULL";
    }// end if
    int mdb_tipompatrimonial = m_tipompatrimonial->currentItem();
    conexionbase->begin();
    QString comentario = concepto->text();
    fprintf(stderr,"INSERCION POR FORMULA\n");
    // Lo primero es crear una masa patrimonial.
    QString query;
    query.sprintf("INSERT INTO mpatrimonial (idbalance, descmpatrimonial, tipompatrimonial) VALUES (%s,'%s', %d)",idbalance.ascii(),comentario.ascii(), mdb_tipompatrimonial);
    int error = conexionbase->ejecuta(query);
    if (error) {
        conexionbase->rollback();
        return;
    }// end if
    query.sprintf("SELECT max(idmpatrimonial) AS maxim FROM mpatrimonial");
    cursor2 * curs= conexionbase->cargacursor(query, "micursorcillo");
    QString idmpatrimonialnueva = curs->valor("maxim");
    delete curs;
    // Hay que interpretar la linea de la formula y insertar la masa patrimonial.
    // es una formula, hay que insertar una masa patrimonial
    //string query = "INSERT INTO mpatrinonial (idbalance, descmpatrimonial) VALUES("++","++")"
    QString formula1 = formula->text();
    formula1.replace(" ","");
    int iant=0;
    QString componente;
    for (int i=1; i <= formula1.length(); i++) {
        if(formula1[i] == '+' || formula1[i] == '-' || i == formula1.length()) {
            componente = formula1.mid(iant,i-iant);
            fprintf(stderr,"Hemos localizado el componente: %s\n", componente.ascii());
            // Comprobamos cual es el signo del componente
            QString signo;
            if (componente[0] == '-') {
                signo = "FALSE";
                componente = componente.mid(1,componente.length());
            } else if (componente[0] == '+') {
                signo = "TRUE";
                componente = componente.mid(1,componente.length());
            } else {
                signo = "TRUE";
            }// end if


            QString tipocompmasap;
            if (componente[0] == '>') {
                tipocompmasap = "1";
                componente = componente.mid(1,componente.length());
            } else if (componente[0] == '<') {
                tipocompmasap = "2";
                componente = componente.mid(1,componente.length());
            } else {
                tipocompmasap = "0";
            }// end if

            // Comprobamos si se trata de una masa patrimonial o de una cuenta.
            QString idmpatrimonial;
            QString codcuenta;
            if (componente[0] == 'M') {
                idmpatrimonial =  componente.mid(1,componente.length());
                query.sprintf("INSERT INTO compmasap(idmpatrimonial, masaperteneciente, signo, tipocompmasap) VALUES (%s,%s,%s,%s)",idmpatrimonial.ascii(), idmpatrimonialnueva.ascii(), signo.ascii(), tipocompmasap.ascii());
            } else {
                codcuenta = componente;
                query.sprintf("INSERT INTO compmasap(idcuenta, masaperteneciente, signo, tipocompmasap) VALUES (id_cuenta('%s'),%s,%s, %s)",codcuenta.ascii(), idmpatrimonialnueva.ascii(), signo.ascii(), tipocompmasap.ascii());
            }// end if
            conexionbase->ejecuta(query);
            iant = i;
        }// end if
    }// end for
    QString query1 = "UPDATE compbalance SET orden = orden +1 WHERE idbalance="+idbalance+" AND orden >= "+elorden+"+1";
    conexionbase->ejecuta(query1);
    query.sprintf("INSERT INTO compbalance (concepto, idbalance, idmpatrimonial, tabulacion, orden) VALUES ('%s',%s,%s, 0, %s+1)", comentario.ascii(), idbalance.ascii(), idmpatrimonialnueva.ascii(), elorden.ascii());
    conexionbase->ejecuta(query);
    conexionbase->commit();
    inicializa1(idbalance);
    listado->setCurrentCell(row+1,0);

}// end agregalinea


void compbalanceview::botonizq() {
    int row = listado->currentRow();
    string idcompbalance = listado->text(listado->currentRow(),0).ascii();
    string query = "UPDATE compbalance set tabulacion=tabulacion-1 WHERE idcompbalance="+idcompbalance;
    conexionbase->begin();
    conexionbase->ejecuta(query.c_str());
    conexionbase->commit();
    inicializa1(idbalance);
    listado->setCurrentCell(row,0);
}// end botonizq


void compbalanceview::botondrcha() {
    int row = listado->currentRow();
    string idcompbalance = listado->text(listado->currentRow(),COL_IDCOMPBALANCE).ascii();
    string query = "UPDATE compbalance set tabulacion=tabulacion+1 WHERE idcompbalance="+idcompbalance;
    conexionbase->begin();
    conexionbase->ejecuta(query.c_str());
    conexionbase->commit();
    inicializa1(idbalance);
    listado->setCurrentCell(row,0);
}// end botondrcha


void compbalanceview::botonarriba() {
    int row = listado->currentRow();
    if (row >0) {
        string idcompbalance = listado->text(row,COL_IDCOMPBALANCE).ascii();
        string idcompbalance1 = listado->text(row-1,COL_IDCOMPBALANCE).ascii();
        string query = "UPDATE compbalance set orden = "+(string)listado->text(row-1,3).ascii()+" WHERE idcompbalance = "+ idcompbalance;
        fprintf(stderr,"%s\n",query.c_str());
        conexionbase->begin();
        conexionbase->ejecuta(query.c_str());
        query = "UPDATE compbalance set orden = "+(string)listado->text(row,3).ascii()+" WHERE idcompbalance = "+idcompbalance1;
        fprintf(stderr,"%s\n",query.c_str());
        conexionbase->ejecuta(query.c_str());
        conexionbase->commit();
    }// end if
    inicializa1(idbalance);
    listado->setCurrentCell(row-1,0);

}// end botonarriba


void compbalanceview::botonabajo() {
    int row = listado->currentRow();
    if (row < listado->numRows()-1) {
        string idcompbalance = listado->text(row,COL_IDCOMPBALANCE).ascii();
        string idcompbalance1 = listado->text(row+1,COL_IDCOMPBALANCE).ascii();
        string query = "UPDATE compbalance set orden = "+(string)listado->text(row+1,3).ascii()+" WHERE idcompbalance = "+ idcompbalance;
        fprintf(stderr,"%s\n",query.c_str());
        conexionbase->begin();
        conexionbase->ejecuta(query.c_str());
        query = "UPDATE compbalance set orden = "+(string)listado->text(row,3).ascii()+" WHERE idcompbalance = "+idcompbalance1;
        fprintf(stderr,"%s\n",query.c_str());
        conexionbase->ejecuta(query.c_str());
        conexionbase->commit();
    }// end if
    inicializa1(idbalance);
    listado->setCurrentCell(row+1,0);
}// end botonarriba


void compbalanceview::listadopulsado(int row, int col, int a, const QPoint &mouse) {
    concepto->setText("");
    QString query = "SELECT * FROM compbalance WHERE idcompbalance = "+ listado->text(row,COL_IDCOMPBALANCE);
    cursor2 *curs = conexionbase->cargacursor(query);
    if (!curs->eof()) {
        concepto->setText(curs->valor("concepto"));
        mdb_idmpatrimonial = curs->valor("idmpatrimonial");


        /// Ponemos en el combo del tipo de masa el valor adecuado.
        query = "SELECT tipompatrimonial FROM mpatrimonial WHERE idmpatrimonial="+mdb_idmpatrimonial;
        cursor2 *mp = conexionbase->cargacursor(query);
        m_tipompatrimonial->setCurrentItem(mp->valor("tipompatrimonial").toInt());
        delete mp;


        query = "SELECT * FROM compmasap LEFT JOIN cuenta ON compmasap.idcuenta = cuenta.idcuenta LEFT JOIN mpatrimonial ON compmasap.idmpatrimonial = mpatrimonial.idmpatrimonial WHERE masaperteneciente = "+mdb_idmpatrimonial;
        QString formula1;
        cursor2 *curs1 = conexionbase->cargacursor(query);
        while (!curs1->eof()) {
            if (curs1->valor("signo") == "t") {
                formula1 += " +";
            } else {
                formula1 += " -";
            }// end if


            if (curs1->valor("tipocompmasap") == "1") {
                formula1 += ">";
            } else if (curs1->valor("tipocompmasap") == "2") {
                formula1 += "<";
            }// end if

            if (curs1->valor("codigo") == "") {
                formula1 += " M"+curs1->valor("idmpatrimonial");
            } else {
                formula1 += curs1->valor("codigo");
            }// end if
            curs1->siguienteregistro();
        }// end while
        delete curs1;
        formula->setText(formula1);
    }// end if
    delete curs;

    // Para quitar el warning
    col=a=0;
    mouse.isNull();
}// end listadopulsado



void compbalanceview::modificamasa() {
    // Creamos el objeto mpatrimonialview, y lo lanzamos.
    mpatrimonialview *masa=new mpatrimonialview(this,0);
    masa->inicializa(conexionbase);
    masa->inicializa1(mdb_idmpatrimonial);
    masa->exec();
    mdb_idmpatrimonial = masa->getidmasa();
    delete masa;
    // Como existe la posibilidad de que hayan cambiado las cosas forzamos un repintado
}// end modificamasa





void compbalanceview::listadodpulsado(int row, int col, int but, const QPoint &butt) {
    listadopulsado ( row, col, but, butt);
    if (mdb_idmpatrimonial != "") {
        modificamasa();
    } else {
        nuevamasa();
    }// end if
}// end listadopulsado


void compbalanceview::cambiado(int row, int col) {
    QPoint aux;
    listadopulsado(row, col,0, aux);
}// end cambiado


void compbalanceview::modificalinea() {
    int row = listado->currentRow();
    QString query;                            // Utilizado para hacer los querys
    QString idcompbalance = listado->text(row, COL_IDCOMPBALANCE);
    QString descripcion = concepto->text();
    if (mdb_idmpatrimonial == "") {
        mdb_idmpatrimonial="NULL";
        _depura ("no existe masa patrimonial asociada",2);
        return;
    }// end if
    conexionbase->begin();
    QString comentario = concepto->text();    // El concepto de la l�ea
    QString formula1 = formula->text();       // La f�mula de la linea de balance
    QString componente;                       // Esta variable es utilizada para desglosar la f�mula

    // es una formula, hay que insertar una masa patrimonial
    query.sprintf("UPDATE mpatrimonial SET idbalance=%s, tipompatrimonial=%d WHERE idmpatrimonial=%s", idbalance.ascii(),m_tipompatrimonial->currentItem(), mdb_idmpatrimonial.ascii());
    fprintf(stderr,"%s\n",query.ascii());
    conexionbase->ejecuta(query);

    query.sprintf("DELETE FROM compmasap WHERE masaperteneciente = %s", mdb_idmpatrimonial.ascii());
    fprintf(stderr,"%s\n",(char *)query.ascii());
    conexionbase->ejecuta(query);
    // Hay que interpretar la linea de la formula y insertar la masa patrimonial.
    // es una formula, hay que insertar una masa patrimonial
    //string query = "INSERT INTO mpatrinonial (idbalance, descmpatrimonial) VALUES("++","++")"
    formula1.replace(" ","");
    int iant=0;
    for (int i=1; i <= formula1.length(); i++) {
        if(formula1[i] == '+' || formula1[i] == '-' || i == formula1.length()) {
            componente = formula1.mid(iant,i-iant);
            fprintf(stderr,"Hemos localizado el componente: %s\n", componente.ascii());
            // Comprobamos cual es el signo del componente
            QString signo;
            if (componente[0] == '-') {
                signo = "FALSE";
                componente = componente.mid(1,componente.length());
            } else if (componente[0] == '+') {
                signo = "TRUE";
                componente = componente.mid(1,componente.length());
            } else {
                signo = "TRUE";
            }// end if


            QString tipocompmasap;
            if (componente[0] == '>') {
                tipocompmasap = "1";
                componente = componente.mid(1,componente.length());
            } else if (componente[0] == '<') {
                tipocompmasap = "2";
                componente = componente.mid(1,componente.length());
            } else {
                tipocompmasap = "0";
            }// end if

            // Comprobamos si se trata de una masa patrimonial o de una cuenta.
            QString idmpatrimonialcomp;
            QString codcuenta;

            if (componente[0] == 'M') {
                idmpatrimonialcomp =  componente.mid(1,componente.length());
                query.sprintf("INSERT INTO compmasap(idmpatrimonial, masaperteneciente, signo, tipocompmasap) VALUES (%s,%s,%s,%s)",idmpatrimonialcomp.ascii(), mdb_idmpatrimonial.ascii(), signo.ascii(), tipocompmasap.ascii());
            } else {
                codcuenta = componente;
                query.sprintf("INSERT INTO compmasap(idcuenta, masaperteneciente, signo, tipocompmasap) VALUES (id_cuenta('%s'),%s,%s, %s)",codcuenta.ascii(), mdb_idmpatrimonial.ascii(), signo.ascii(), tipocompmasap.ascii());
            }// end if

            conexionbase->ejecuta(query);
            iant = i;
        }// end if
    }// end for

    query.sprintf("UPDATE compbalance SET idmpatrimonial=%s, concepto='%s' WHERE idcompbalance=%s",mdb_idmpatrimonial.ascii(), descripcion.ascii(), idcompbalance.ascii());
    fprintf(stderr,"%s\n",query.ascii());
    conexionbase->ejecuta(query);
    conexionbase->commit();
    inicializa1(idbalance);
    //   listado->setCurrentCell(row,0);
}// end modificalinea


void compbalanceview::botonimprimir() {
    balancesprintview *b = new balancesprintview(empresaactual,this,0);
    b->setidbalance(idbalance);
    b->exec();
    delete b;
}// end botonimprimir

void compbalanceview::s_save() {
    QString query;
    query.sprintf("UPDATE balance SET nombrebalance='%s' WHERE idbalance=%s",titbalance->text().ascii(),idbalance.ascii());
    conexionbase->begin();
    conexionbase->ejecuta(query);
    conexionbase->commit();
}// end accept

