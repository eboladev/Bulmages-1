/***************************************************************************
 *   Copyright (C) 2003 by Josep Burcion                                   *
 *   josep@burcion.com                                                     *
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
 ***************************************************************************/
#include "bbloqfecha.h"


myQListViewItem::myQListViewItem(myQListViewItem *parent, QString label1):Q3ListViewItem(parent, label1) {
    return;
};

myQListViewItem::myQListViewItem(Q3ListView *parent, QString label1):Q3ListViewItem(parent, label1) {
    return;
};


BbloqFecha::BbloqFecha(QString *NombreBaseDatos, QWidget * parent, const char * name, Qt::WFlags f) : UIbloqFecha(parent,name,f) {
    myQListViewItem *listMain, *listAux=0;
    QString query;
    listView1->setSorting(-1);

    postgresiface2 *DBconn = new postgresiface2();
    DBconn->inicializa(*NombreBaseDatos);

    conexionbase = DBconn;


    query.sprintf("SELECT * FROM ejercicios WHERE periodo=0 ORDER BY ejercicio DESC");
    cursor2 *curPeri,*curEjer = DBconn->cargacursor(query);
    while (!curEjer->eof()) {
        listMain = new myQListViewItem(listView1,curEjer->valor("ejercicio"));
	listMain->ej = curEjer->valor("ejercicio");
	listMain->per = "0";
        if (curEjer->valor("bloqueado")=="t" ) {
		 listMain->setText(1,"Bloqueado");
	} else {
		 listMain->setText(1,"Abierto");
	}// end if




        query.sprintf("SELECT * FROM ejercicios WHERE ejercicio='%s' ORDER BY periodo DESC",curEjer->valor("ejercicio").ascii());
        curPeri = DBconn->cargacursor(query);
        while (!curPeri->eof()) {
            switch (curPeri->valor("periodo").toInt()) {
            case 12:
                listAux = new myQListViewItem(listMain,tr("Diciembre "));
                break;
            case 11:
                listAux = new myQListViewItem(listMain,tr("Noviembre "));
                break;
            case 10:
                listAux = new myQListViewItem(listMain,tr("Octubre"   ));
                break;
            case  9:
                listAux = new myQListViewItem(listMain,tr("Septiembre"));
                break;
            case  8:
                listAux = new myQListViewItem(listMain,tr("Agosto    "));
                break;
            case  7:
                listAux = new myQListViewItem(listMain,tr("Julio     "));
                break;
            case  6:
                listAux = new myQListViewItem(listMain,tr("Junio     "));
                break;
            case  5:
                listAux = new myQListViewItem(listMain,tr("Mayo      "));
                break;
            case  4:
                listAux = new myQListViewItem(listMain,tr("Abril     "));
                break;
            case  3:
                listAux = new myQListViewItem(listMain,tr("Marzo     "));
                break;
            case  2:
                listAux = new myQListViewItem(listMain,tr("Febrero   "));
                break;
            case  1:
                listAux = new myQListViewItem(listMain,tr("Enero     "));
                break;
            }

            listAux->ej = curEjer->valor("ejercicio");
            listAux->per = curPeri->valor("periodo");
            curPeri->valor("bloqueado")=="t" ? listAux->setText(1,"Bloqueado") : listAux->setText(1,"Abierto");
            curPeri->siguienteregistro();
        }
        curEjer->siguienteregistro();
    }

}

BbloqFecha::~BbloqFecha() {}

void BbloqFecha::boto1_click() {
    delete this;
}

void BbloqFecha::dobleClick (Q3ListViewItem *item) {
    int error;
    myQListViewItem *it = (myQListViewItem *) item;
    if (item->text(1)=="Bloqueado") {
        item->setText(1,"Abierto");
        QString Query = "UPDATE ejercicios SET bloqueado = FALSE WHERE ejercicio = "+it->ej+" AND periodo = "+it->per;
        error = conexionbase->ejecuta(Query);
    } else {
        item->setText(1,"Bloqueado");
        QString Query = "UPDATE ejercicios SET bloqueado = TRUE WHERE ejercicio = "+it->ej+" AND periodo = "+it->per;
        error = conexionbase->ejecuta(Query);
    }// end if
}



