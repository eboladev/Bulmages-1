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
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/

#include "bmodelo347.h"

#include <errno.h>


BModelo347::BModelo347(postgresiface2 *DBconnect, QString ejerActual, QWidget *parent, Qt::WFlags f)
        : QDialog(parent, f) {
    _depura("BModelo347::BModelo347", 0);
    setupUi(this);
    importe->setText("3005.06");
    finicial->setText(normalizafecha("01/01/" + ejerActual).toString("dd/MM/yyyy"));
    ffinal->setText(normalizafecha("31/12/" + ejerActual).toString("dd/MM/yyyy"));
    DBConn = DBconnect;
    /// Carga las tablas en pantalla.
    click_recargar();
    _depura("END BModelo347::BModelo347", 0);
}


BModelo347::~BModelo347() {}


void BModelo347::click_recargar() {
    _depura("BModelo347::click_recargar", 0);
    int i = 0;
    QTableWidgetItem *item;
    QString query;
    /// A por la tabla de Ventas...pedazo de consulta SQL.
    query = QString("SELECT codigo, descripcion, cifent_cuenta as cif, cpent_cuenta as cp, importe FROM cuenta INNER JOIN (SELECT idcuenta, sum(apunte.debe) as importe FROM apunte WHERE idasiento IN (SELECT idasiento FROM (SELECT idcuenta FROM cuenta WHERE (codigo = '4770016' OR codigo = '4770007' OR codigo = '4770004')) AS iva INNER JOIN apunte USING (idcuenta) WHERE fecha <= '") + ffinal->text() + QString("' AND fecha >= '") + finicial->text() + QString("' GROUP BY idasiento) AND idcuenta IN (SELECT idcuenta FROM cuenta WHERE codigo LIKE '430%') GROUP BY idcuenta) AS facturado USING(idcuenta) WHERE importe > ") + importe->text() + QString(" ORDER BY descripcion");
    DBConn->begin();
    cursor2 *recordSet = DBConn->cargacursor(query, "recordSet");
    DBConn->commit();
    tablaventas->setColumnCount(5);
    tablaventas->setRowCount(recordSet->numregistros());
    i = 0;
    while (!recordSet->eof()) {
        item = new QTableWidgetItem(recordSet->valor("codigo"));
        tablaventas->setItem(i, 0, item);
        item = new QTableWidgetItem(recordSet->valor("descripcion"));
        tablaventas->setItem(i, 1, item);
        item = new QTableWidgetItem(recordSet->valor("cif"));
        tablaventas->setItem(i, 2, item);
        item = new QTableWidgetItem(recordSet->valor("cp"));
        tablaventas->setItem(i, 3, item);
        query.setNum(recordSet->valor("importe").toFloat(), 'f', 2); /// Round a 2 decimales.
        item = new QTableWidgetItem(query);
        tablaventas->setItem(i, 4, item);
        ++i;
        recordSet->siguienteregistro();
    } // end while
    /// A por la tabla de Compras...
    query = QString("SELECT codigo, descripcion, cifent_cuenta as cif, cpent_cuenta as cp, importe FROM cuenta INNER JOIN (SELECT idcuenta, sum(apunte.haber) as importe FROM apunte WHERE idasiento IN (SELECT idasiento FROM (SELECT idcuenta FROM cuenta WHERE (codigo = '4720016' OR codigo = '4720007' OR codigo = '4720004')) AS iva INNER JOIN apunte USING (idcuenta) WHERE fecha <= '") + ffinal->text() + QString("' AND fecha >= '") + finicial->text() + QString("' GROUP BY idasiento) AND idcuenta IN (SELECT idcuenta FROM cuenta WHERE codigo SIMILAR TO '4(0|1)0%') GROUP BY idcuenta) AS facturado USING(idcuenta) WHERE importe > ") + importe->text() + QString(" ORDER BY descripcion");
    DBConn->begin();
    recordSet = DBConn->cargacursor(query, "recordSet");
    DBConn->commit();
    tablacompras->setColumnCount(5);
    tablacompras->setRowCount(recordSet->numregistros());
    i = 0;
    while (!recordSet->eof()) {
        item = new QTableWidgetItem(recordSet->valor("codigo"));
        tablacompras->setItem(i, 0, item);
        item = new QTableWidgetItem(recordSet->valor("descripcion"));
        tablacompras->setItem(i, 1, item);
        item = new QTableWidgetItem(recordSet->valor("cif"));
        tablacompras->setItem(i, 2, item);
        item = new QTableWidgetItem(recordSet->valor("cp"));
        tablacompras->setItem(i, 3, item);
        query.setNum(recordSet->valor("importe").toFloat(), 'f', 2); /// Round a 2 decimales.
        item = new QTableWidgetItem(query);
        tablacompras->setItem(i, 4, item);
        ++i;
        recordSet->siguienteregistro();
    } // end while
    _depura("END BModelo347::click_recargar", 0);
}


void BModelo347::click_imprimir() {
    _depura("BModelo347::click_imprimir", 0);
    int i, numventas, numcompras, error, pid;
    char *args[] = {"listado347.txt", "listado347.txt", NULL};
    QString codigo, descripcion, cif, importe, cp;
    if (fichero != NULL) {
        args[0] = fichero;
        args[1] = fichero;
    } // end if
    FILE *mifile;
    mifile = fopen(args[0], "wt");
    if (mifile != NULL) {
        fprintf(mifile, "LISTADO 347\n");
        fprintf(mifile, "\nCuenta	 Deudor 				 	    CIF/NIF     CP	Importe\n");
        fprintf(mifile, "________________________________________________________________________________________\n");
        numventas = tablaventas->rowCount();
        for (i = 0; i < numventas; i++) {
            codigo = tablaventas->item(i, 0)->text();
            descripcion = tablaventas->item(i, 1)->text();
            cif = tablaventas->item(i, 2)->text();
            cp = tablaventas->item(i, 3)->text();
            importe = tablaventas->item(i, 4)->text();
            fprintf(mifile, "%s %-50s %9s %6s %12.2f\n", codigo.toAscii().constData(), descripcion.toAscii().constData(), cif.toAscii().constData(), cp.toAscii().constData(), importe.toFloat());
        } // end for
        numcompras = tablacompras->rowCount();
        fprintf(mifile, "\nCuenta	 Acreedor				 	    CIF/NIF     CP	Importe\n");
        fprintf(mifile, "________________________________________________________________________________________\n");
        numcompras = tablacompras->rowCount();
        for (i = 0; i < numcompras; i++) {
            codigo = tablacompras->item(i, 0)->text();
            descripcion = tablacompras->item(i, 1)->text();
            cif = tablacompras->item(i, 2)->text();
            cp = tablacompras->item(i, 3)->text();
            importe = tablacompras->item(i, 4)->text();
            fprintf(mifile, "%s %-50s %9s %6s %12.2f\n", codigo.toAscii().constData(), descripcion.toAscii().constData(), cif.toAscii().constData(), cp.toAscii().constData(), importe.toFloat());
        } // end for
        fclose(mifile);
    } // end if
#ifndef WIN32
    if ((pid = fork()) < 0)
    {
        perror("Fork failed");
        exit(errno);
    } // end if
    if (!pid) {
        error = execvp(confpr->valor(CONF_EDITOR).toAscii(), args);
    } // end if
#endif
    _depura("END BModelo347::click_imprimir", 0);
}


void BModelo347::formatea_fecha() {
    _depura("BModelo347::formatea_fecha", 0);
    QLineEdit *fecha = (QLineEdit *) sender();
    fecha->setText(normalizafecha(fecha->text()).toString("dd/MM/yyyy"));
    _depura("END BModelo347::formatea_fecha", 0);
}

