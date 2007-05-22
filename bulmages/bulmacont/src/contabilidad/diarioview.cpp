/***************************************************************************
 *   Copyright (C) 2003 by Tomeu Borras                                    *
 *   tborras@conetxia.com                                                  *
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

#include <QWidget>
#include <QLineEdit>
#include <QColor>
#include <QLayout>
#include <QCheckBox>
#include <QPixmap>
#include <QTextStream>
#include <QFileDialog>

#include "empresa.h"
#include "diarioview.h"
#include "funcaux.h"
#include "diarioprintview.h"
#include "diarioprint.h"
#include "configuracion.h"
#include "calendario.h"
#include "asiento1view.h"
#include "extractoview1.h"
#include "balanceview.h"
#include "selectccosteview.h"
#include "selectcanalview.h"
#include "busquedafecha.h"


DiarioView::DiarioView(empresa *emp, QWidget *parent, int)
        : Ficha(parent) {
    _depura("DiarioView::DiarioView", 0);
    setupUi(this);
    m_companyact = emp;
    mui_list->setEmpresaBase(emp);
    mui_contrapartida->setempresa(emp);
    /// Iniciamos los componentes de la fecha para que al principio aparezcan
    /// como el a&ntilde;o inicial.
    char cadena[10];
    sprintf(cadena, "%2.2d/%2.2d/%4.4d", 1, 1, QDate::currentDate().year());
    mui_fechainicial->setText(cadena);
    sprintf(cadena, "%2.2d/%2.2d/%4.4d", 31, 12, QDate::currentDate().year());
    mui_fechafinal->setText(cadena);
    m_companyact->meteWindow(windowTitle(), this);
    _depura("END DiarioView::DiarioView", 0);
}


DiarioView::~DiarioView() {
    _depura("DiarioView::~DiarioView", 0);
    m_companyact->sacaWindow(this);
    _depura("END DiarioView::~DiarioView", 0);
}


void DiarioView::on_mui_actualizar_clicked() {
    accept();
}


void DiarioView::on_mui_configurar_clicked() {
    mui_list->showConfig();
}


void DiarioView::inicializa1(QString finicial, QString ffinal, int) {
    _depura("DiarioView::inicializa1", 0);
    QString s1, s2, s3;
    QDate fecha1aux;
    int dia, mes, ano;

    s1 = finicial;
    s2 = s1.mid(0, 2);
    dia = atoi(s2.toAscii());
    s2 = s1.mid(3, 2);
    mes = atoi(s2.toAscii());
    s2 = s1.mid(6, 4);
    ano = atoi(s2.toAscii());
    fecha1aux.setYMD(ano, mes, dia);
    QString cadena2;
    cadena2.sprintf("%2.2d/%2.2d/%4.4d", fecha1aux.day(), fecha1aux.month(), fecha1aux.year());
    mui_fechainicial->setText(cadena2);

    s1 = ffinal;
    s2 = s1.mid(0,2);
    dia = atoi(s2.toAscii());
    s2 = s1.mid(3,2);
    mes = atoi(s2.toAscii());
    s2 = s1.mid(6,4);
    ano = atoi(s2.toAscii());
    fecha1aux.setYMD(ano, mes, dia);
    cadena2.sprintf("%2.2d/%2.2d/%4.4d", fecha1aux.day(), fecha1aux.month(), fecha1aux.year());
    mui_fechafinal->setText(cadena2);
    _depura("END DiarioView::inicializa1", 0);
}


/// SLOT que responde a la pulsaci&oacute;n del bot&oacute;n de imprimir.
/** Muestra el formulario de impresi&oacute;n de diario y lo ejecuta \ref DiarioPrintView */
void DiarioView::boton_imprimir() {
    _depura("DiarioView::boton_imprimir", 0);
    DiarioPrintView *print = new DiarioPrintView(m_companyact, 0);
    print->exec();
    _depura("END DiarioView::boton_imprimir", 0);
}


/// Se ha pulsado sobre el bot&oacute;n guardar del formulario.
void DiarioView::boton_guardar() {
    _depura("DiarioView::boton_guardar", 0);
    QString fn = QFileDialog::getSaveFileName(this,
                 tr("Guardar Libro Diario"),
                 confpr->valor(CONF_DIR_USER),
                 tr("Diarios (*.txt)"));

    if (!fn.isEmpty()) {
        /// Si se ha proporcionado un nombre de archivo v&aacute;lido
        /// invocamos la clase diarioprint y hacemos que guarde el archivo.
        diarioprint diariop(m_companyact);
        QString finicial = mui_fechainicial->text();
        QString ffinal = mui_fechafinal->text();
        diariop.inicializa(m_companyact);
        diariop.inicializa1((char *) finicial.toAscii().constData(), (char *)ffinal.toAscii().constData());
        diariop.inicializa2((char *) fn.toAscii().constData());
        diariop.accept();
    } // end if
    _depura("END DiarioView::boton_guardar", 0);
}


/// Se ha pulsado sobre el bot&oacute;n aceptar del formulario.
void DiarioView::accept() {
    _depura("DiarioView::accept", 0);
    presentar();
    _depura("END DiarioView::accept", 0);
}


/// Pinta el listado en el subformulario
void DiarioView::presentar() {
    _depura("DiarioView::presentar", 0);
    QString query = "SELECT *, cuenta.descripcion AS descripcioncuenta FROM borrador LEFT JOIN cuenta ON cuenta.idcuenta = borrador.idcuenta ";
    query += " LEFT JOIN (SELECT idc_coste, nombre AS nombrec_coste FROM c_coste) AS t1 ON t1.idc_coste = borrador.idc_coste ";
    query += " LEFT JOIN (SELECT (ordenasiento || ' - ' || fecha) AS ordenasiento, idasiento, fecha FROM asiento) AS t5 ON t5.idasiento = borrador.idasiento";
    query += " LEFT JOIN (SELECT idcanal, nombre as nombrecanal FROM canal) AS t2 ON t2.idcanal = borrador.idcanal";
    query += " LEFT JOIN (SELECT idregistroiva, factura, idborrador FROM registroiva) AS t3 ON t3.idborrador = borrador.idborrador ";
    QString cad = "";
    QString cadwhere=" WHERE ";
    QString cadand = "";
    QString totalcadena = "";

    if (mui_fechainicial->text() != "") {
        cad += cadwhere + cadand + "borrador.fecha >= '" + mui_fechainicial->text() + "'";
        cadwhere = "";
        cadand = " AND ";
    } // end if

    if (mui_fechafinal->text() != "") {
        cad += cadwhere + cadand + "borrador.fecha <= '" + mui_fechafinal->text() + "'";
        cadwhere = "";
        cadand = " AND ";
    } // end if

    totalcadena = query + cad + " ORDER BY t5.fecha, t5.ordenasiento ";

    cursor2 *cur = m_companyact->cargacursor(totalcadena);
    mui_list->cargar(cur);
    delete cur;

    cur = m_companyact->cargacursor("SELECT sum(debe) as totaldebe, sum(haber) as totalhaber from borrador " + cad);
    if (!cur->eof()) {
        totaldebe->setText(cur->valor("totaldebe"));
        totalhaber->setText(cur->valor("totalhaber"));
    } // end if
    delete cur;
    _depura("END DiarioView::presentar", 0);
}


/// Boton de impresion del diario.
void DiarioView::on_mui_imprimir_clicked() {
    _depura("DiarioView::on_mui_imprimir_clicked", 0);
    mui_list->imprimirPDF("diario");
    _depura("END DiarioView::on_mui_imprimir_clicked", 0);
}

/*
/// Boton de impresion del diario.
void DiarioView::on_mui_imprimir_clicked() {
    _depura("DiarioView::on_mui_imprimir_clicked", 0);
    QString archivo = confpr->valor(CONF_DIR_OPENREPORTS) + "diario.rml";
    QString archivod = confpr->valor(CONF_DIR_USER) + "diario.rml";
    QString archivologo = confpr->valor(CONF_DIR_OPENREPORTS) + "logo.jpg";
    /// Copiamos el archivo.
#ifdef WINDOWS

    archivo = "copy " + archivo + " " + archivod;
#else

    archivo = "cp " + archivo + " " + archivod;
#endif

    system(archivo.toAscii().constData());
    /// Copiamos el logo.
#ifdef WINDOWS

    archivologo = "copy " + archivologo + " " + confpr->valor(CONF_DIR_USER) + "logo.jpg";
#else

    archivologo = "cp " + archivologo + " " + confpr->valor(CONF_DIR_USER) + "logo.jpg";
#endif

    system(archivologo.toAscii().constData());
    QFile file;
    file.setFileName(archivod);
    file.open(QIODevice::ReadOnly);
    QTextStream stream(&file);
    QString buff = stream.readAll();
    file.close();
    QString fitxersortidatxt;
    /// L&iacute;nea de totales del presupuesto.
    fitxersortidatxt = "<blockTable style=\"tabla\" repeatRows=\"1\">";
    fitxersortidatxt += mui_list->imprimir();
    fitxersortidatxt += "</blockTable>";

    buff.replace("[story]", fitxersortidatxt);

    if (file.open(QIODevice::WriteOnly)) {
        QTextStream stream(&file);
        stream << buff;
        file.close();
    } // end if
    /// Crea el pdf y lo muestra.
    invocaPDF("diario");
    _depura("END DiarioView::on_mui_imprimir_clicked", 0);
}
*/
