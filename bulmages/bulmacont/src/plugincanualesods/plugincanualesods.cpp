/***************************************************************************
 *   Copyright (C) 2005 by Tomeu Borras Riera                              *
 *   tborras@conetxia.com                                                  *
 *   Copyright (C) 2006 by Fco. Javier M. C.                               *
 *   fcojavmc@todo-redes.com                                               *
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

#include <QMenu>
#include <QAction>
#include <QObject>
#include <QMessageBox>

#include <stdio.h>

#include "plugincanualesods.h"
#include "empresa.h"
#include "funcaux.h"

#include "datosview.h"


pluginCAnualesODS::pluginCAnualesODS() {}


pluginCAnualesODS::~pluginCAnualesODS() {}


Fixed pluginCAnualesODS::saldoCuenta(int cuenta) {
    QString query;
    cursor2 *cur;
    Fixed resultado;
    query.sprintf("select (debe - haber) as saldo from cuenta where codigo = %d", cuenta);
    conexionbase->begin();
    cur = conexionbase->cargacursor(query);
    resultado = cur->valor("saldo");
    conexionbase->commit();
    return resultado;
}


void pluginCAnualesODS::formDatosBalance() {
    int resultado;

    datosView *dv = new datosView(0);
    dv->exec();
    resultado = dv->result();

    /// Accepted = 1
    /// Rejected = 0

    if (resultado != 0) {
        /// Asigna valores
        ejercicioActual_fechaInicio = dv->mui_ejercicioActual_fechaInicio->text();
        ejercicioActual_fechaFin = dv->mui_ejercicioActual_fechaFin->text();
        ejercicioAnterior_fechaInicio = dv->mui_ejercicioAnterior_fechaInicio->text();
        ejercicioAnterior_fechaFin = dv->mui_ejercicioAnterior_fechaFin->text();
    } // end if
    delete dv;

    /// Se pulso el boton 'cancelar'
    if (resultado == 0) {
        return;
    } // end if

    /// Generamos el balance
    balanceSituacionODS();
}


void pluginCAnualesODS::balanceSituacionODS() {
    /// Se genera el Balance de Situacion en formato SXW (Hoja de calculo).

    QString archivosalida = "\
#!/usr/bin/python\n\
# -*- coding: utf8 -*-\n\
\n\
import ooolib\n\
\n\
# Crea el documento\n\
doc = ooolib.Calc(\"Activo\")\n\
\n\
doc.set_column_property(1, 'width', '0.5in')\n\
doc.set_column_property(2, 'width', '5in')\n\
doc.set_cell_property('bold', True)\n\
doc.set_cell_value(2, 2, 'string', 'BALANCE DE SITUACIÓN')\n\
doc.set_cell_value(2, 5, 'string', 'ACTIVO')\n\
doc.set_cell_property('bold', False)\n\
\
doc.set_cell_property('bold', True)\n\
doc.set_cell_value(2, 7, 'string', 'A) Accionistas (socios) por desembolsos no exigidos')\n\
doc.set_cell_value(3, 7, 'float', '" + (saldoCuenta(190) + saldoCuenta(191) + saldoCuenta(192) + saldoCuenta(193) + saldoCuenta(194) + saldoCuenta(195) + saldoCuenta(196)).toQString() + "')\n\
doc.set_cell_property('bold', False)\n\
\
doc.set_cell_property('bold', True)\n\
doc.set_cell_value(2, 9, 'string', 'B) Inmovilizado')\n\
doc.set_cell_value(3, 9, 'formula', '=C10+C11+C20+C28+C37')\n\
doc.set_cell_property('bold', False)\n\
\
doc.set_cell_value(2, 10, 'string', '   I.   Gastos de establecimiento')\n\
doc.set_cell_value(3, 10, 'float', '" + saldoCuenta(20).toQString() + "')\n\
\
doc.set_cell_value(2, 11, 'string', '   II.  Inmovilizaciones inmatariales')\n\
doc.set_cell_value(3, 11, 'formula', '=SUM(C12:C19)')\n\
\
doc.set_cell_value(2, 12, 'string', '       1. Gastos de investigación y desarrollo')\n\
doc.set_cell_value(3, 12, 'float', '" + saldoCuenta(210).toQString() + "')\n\
\
doc.set_cell_value(2, 13, 'string', '       2. Concesiones, patentes, licencias, marcas y similares')\n\
doc.set_cell_value(3, 13, 'float', '" + (saldoCuenta(211) + saldoCuenta(212)).toQString() + "')\n\
\
doc.set_cell_value(2, 14, 'string', '       3. Fondo de comercio')\n\
doc.set_cell_value(3, 14, 'float', '" + saldoCuenta(213).toQString() + "')\n\
\
doc.set_cell_value(2, 15, 'string', '       4. Derechos de traspaso')\n\
doc.set_cell_value(3, 15, 'float', '" + saldoCuenta(214).toQString() + "')\n\
\
doc.set_cell_value(2, 16, 'string', '       5. Aplicaciones informáticas')\n\
doc.set_cell_value(3, 16, 'float', '" + saldoCuenta(215).toQString() + "')\n\
\
doc.set_cell_value(2, 17, 'string', '       6. Anticipos')\n\
doc.set_cell_value(3, 17, 'float', '" + saldoCuenta(219).toQString() + "')\n\
\
doc.set_cell_value(2, 18, 'string', '       7. Provisiones')\n\
doc.set_cell_value(3, 18, 'float', '" + (-saldoCuenta(291)).toQString() + "')\n\
\
doc.set_cell_value(2, 19, 'string', '       8. Amortizaciones')\n\
doc.set_cell_value(3, 19, 'float', '" + (-saldoCuenta(281)).toQString() + "')\n\
\
doc.set_cell_value(2, 20, 'string', '   III. Inmovilizaciones materiales')\n\
doc.set_cell_value(3, 20, 'formula', '=SUM(C21:C27)')\n\
\
doc.set_cell_value(2, 21, 'string', '       1. Terrenos y construcciones')\n\
doc.set_cell_value(3, 21, 'float', '" + (saldoCuenta(220) + saldoCuenta(221)).toQString() + "')\n\
\
doc.set_cell_value(2, 22, 'string', '       2. Instalaciones técnicas y maquinaria')\n\
doc.set_cell_value(3, 22, 'float', '" + (saldoCuenta(222) + saldoCuenta(223)).toQString() + "')\n\
\
doc.set_cell_value(2, 23, 'string', '       3. Otras instalaciones, utillaje y mobiliario')\n\
doc.set_cell_value(3, 23, 'float', '" + (saldoCuenta(224) + saldoCuenta(225) + saldoCuenta(226)).toQString() + "')\n\
\
doc.set_cell_value(2, 24, 'string', '       4. Anticipos e inmovilizaciones materiales en curso')\n\
doc.set_cell_value(3, 24, 'float', '" + saldoCuenta(23).toQString() + "')\n\
\
doc.set_cell_value(2, 25, 'string', '       5. Otro inmobilizado')\n\
doc.set_cell_value(3, 25, 'float', '" + (saldoCuenta(227) + saldoCuenta(228) + saldoCuenta(229))  .toQString() + "')\n\
\
doc.set_cell_value(2, 26, 'string', '       6. Provisiones')\n\
doc.set_cell_value(3, 26, 'float', '" + (-saldoCuenta(292)).toQString() + "')\n\
\
doc.set_cell_value(2, 27, 'string', '       7. Amortizaciones')\n\
doc.set_cell_value(3, 27, 'float', '" + (-saldoCuenta(282)).toQString() + "')\n\
\
doc.set_cell_value(2, 28, 'string', '   IV.  Inmovilizaciones financieras')\n\
doc.set_cell_value(3, 28, 'formula', '=SUM(C29:C36)')\n\
\
doc.set_cell_value(2, 29, 'string', '       1. Participaciones en empresas del grupo')\n\
doc.set_cell_value(3, 29, 'float', '" + saldoCuenta(240).toQString() + "')\n\
\
doc.set_cell_value(2, 30, 'string', '       2. Créditos a empresas del grupo')\n\
doc.set_cell_value(3, 30, 'float', '" + (saldoCuenta(242) + saldoCuenta(244) + saldoCuenta(246)).toQString() + "')\n\
\
doc.set_cell_value(2, 31, 'string', '       3. Participaciones en empresas asociadas')\n\
doc.set_cell_value(3, 31, 'float', '" + saldoCuenta(241).toQString() + "')\n\
\
doc.set_cell_value(2, 32, 'string', '       4. Créditos a empresas asociadas')\n\
doc.set_cell_value(3, 32, 'float', '" + (saldoCuenta(243) + saldoCuenta(245) + saldoCuenta(247)).toQString() + "')\n\
\
doc.set_cell_value(2, 33, 'string', '       5. Cartera de valores a largo plazo')\n\
doc.set_cell_value(3, 33, 'float', '" + (saldoCuenta(250) + saldoCuenta(251) + saldoCuenta(256)).toQString() + "')\n\
\
doc.set_cell_value(2, 34, 'string', '       6. Otros créditos')\n\
doc.set_cell_value(3, 34, 'float', '" + (saldoCuenta(252) + saldoCuenta(253) + saldoCuenta(254) + saldoCuenta(257) + saldoCuenta(258)).toQString() + "')\n\
\
doc.set_cell_value(2, 35, 'string', '       7. Depósitos y fianzas constituidos a largo plazo')\n\
doc.set_cell_value(3, 35, 'float', '" + (saldoCuenta(260) + saldoCuenta(265)).toQString() + "')\n\
\
doc.set_cell_value(2, 36, 'string', '       8. Provisiones')\n\
doc.set_cell_value(3, 36, 'float', '" + ((-saldoCuenta(293)) + (-saldoCuenta(294)) + (-saldoCuenta(295)) + (-saldoCuenta(296)) + (-saldoCuenta(297)) + (-saldoCuenta(298))).toQString() + "')\n\
\
doc.set_cell_value(2, 37, 'string', '   V.   Acciones propias')\n\
doc.set_cell_value(3, 37, 'float', '" + saldoCuenta(198).toQString() + "')\n\
\
\
doc.set_cell_property('bold', True)\n\
doc.set_cell_value(2, 39, 'string', 'C) Gastos a distribuir en varios ejercicios')\n\
doc.set_cell_value(3, 39, 'float', '" + saldoCuenta(27).toQString() + "')\n\
doc.set_cell_property('bold', False)\n\
\
\
doc.set_cell_property('bold', True)\n\
doc.set_cell_value(2, 41, 'string', 'D) Activo circulante')\n\
doc.set_cell_value(3, 41, 'formula', '=C42+C43')\n\
doc.set_cell_property('bold', False)\n\
\
doc.set_cell_value(2, 42, 'string', '   I.   Accionistas por desembolsos exigidos')\n\
doc.set_cell_value(3, 42, 'float', '" + saldoCuenta(558).toQString() + "')\n\
\
doc.set_cell_value(2, 43, 'string', '   II.  Existencias')\n\
doc.set_cell_value(3, 43, 'formula', '=SUM(C44:C50)')\n\
\
doc.set_cell_value(2, 44, 'string', '       1. Comerciales')\n\
doc.set_cell_value(3, 44, 'float', '" + saldoCuenta(30).toQString() + "')\n\
\
doc.set_cell_value(2, 45, 'string', '       2. Materias primas y otros aprovisionamientos')\n\
doc.set_cell_value(3, 45, 'float', '" + (saldoCuenta(31) + saldoCuenta(32)).toQString() + "')\n\
\
doc.set_cell_value(2, 46, 'string', '       3. Productos en curso y semiterminados')\n\
doc.set_cell_value(3, 46, 'float', '" + (saldoCuenta(33) + saldoCuenta(34)).toQString() + "')\n\
\
doc.set_cell_value(2, 47, 'string', '       4. Productos terminados')\n\
doc.set_cell_value(3, 47, 'float', '" + saldoCuenta(35).toQString() + "')\n\
\
doc.set_cell_value(2, 48, 'string', '       5. Subproductos, residuos y materiales recuperados')\n\
doc.set_cell_value(3, 48, 'float', '" + saldoCuenta(36).toQString() + "')\n\
\
doc.set_cell_value(2, 49, 'string', '       6. Anticipos')\n\
doc.set_cell_value(3, 49, 'float', '" + saldoCuenta(407).toQString() + "')\n\
\
doc.set_cell_value(2, 50, 'string', '       7. Provisiones')\n\
doc.set_cell_value(3, 50, 'float', '" + (-saldoCuenta(39)).toQString() + "')\n\
\
\
\
\
doc.new_sheet(\"Pasivo\")\n\n\
doc.set_column_property(1, 'width', '0.5in')\n\
doc.set_column_property(2, 'width', '5in')\n\
doc.set_cell_property('bold', True)\n\
doc.set_cell_value(2, 2, 'string', 'BALANCE DE SITUACIÓN')\n\
doc.set_cell_value(2, 5, 'string', 'PASIVO')\n\
doc.set_cell_property('bold', False)\n\
\
doc.new_sheet(\"Debe\")\n\n\
doc.set_column_property(1, 'width', '0.5in')\n\
doc.set_column_property(2, 'width', '5in')\n\
doc.set_cell_property('bold', True)\n\
doc.set_cell_value(2, 2, 'string', 'CUENTA DE PÉRDIDAS Y GANANCIAS')\n\
doc.set_cell_value(2, 5, 'string', 'DEBE')\n\
doc.set_cell_property('bold', False)\n\
\
doc.new_sheet(\"Haber\")\n\n\
doc.set_column_property(1, 'width', '0.5in')\n\
doc.set_column_property(2, 'width', '5in')\n\
doc.set_cell_property('bold', True)\n\
doc.set_cell_value(2, 2, 'string', 'CUENTA DE PÉRDIDAS Y GANANCIAS')\n\
doc.set_cell_value(2, 5, 'string', 'HABER')\n\
doc.set_cell_property('bold', False)\n\
\
# Guardamo el documento\n\
doc.save(\"salida.ods\")\n";


    QString archivo = QString(getenv("HOME")) + "/.bulmages/salida-python-sxw.py";

    QFile file(archivo);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
        return;

    QTextStream out(&file);
    out << archivosalida.toAscii();

    mensajeInfo(archivo + "\nArchivo creado");

}


void pluginCAnualesODS::inicializa(Bulmacont *bcont) {
    empresaact = bcont->empresaactual();
    conexionbase = empresaact->bdempresa();
    /// Creamos el men&uacute;.
    QMenu *pPluginMenu = new QMenu("&Cuentas anuales");
    QAction *accion = new QAction("&Balance de situacion", 0);
    accion->setStatusTip("Balance de situacion");
    accion->setWhatsThis("Balance de situacion");
    connect(accion, SIGNAL(activated()), this, SLOT(formDatosBalance()));
    pPluginMenu->addAction(accion);
    /// A&ntilde;adimos la nueva opci&oacute;n al men&uacute; principal del programa.
    bcont->menuBar()->addMenu(pPluginMenu);
}


void entryPoint(Bulmacont *bcont) {
    _depura("Estoy dentro del plugin\n", 0);
    pluginCAnualesODS *plug = new pluginCAnualesODS();
    plug->inicializa(bcont);
    /// S&Oacute;LO A MODO DE EJEMPLO: se modifica el t&iacute;tulo de la ventana principal
    /// del programa para indicar que el plugin se ha cargado.
}

