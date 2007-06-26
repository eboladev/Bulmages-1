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


void pluginCAnualesODS::balanceSituacionODS() {
    /// Se genera el Balance de Situacion en formato SXW (Hoja de calculo).

    QString archivosalida = "\
#!/usr/bin/python\n\
\n\
import ooolib\n\
\n\
# Crea el documento\n\
doc = ooolib.Calc()\n\
\n\
\
doc.set_column_property(2, 'width', '5in')\n\
doc.set_cell_value(2, 2, 'string', 'BALANCE DE SITUACIÓN')\n\
doc.set_cell_value(2, 5, 'string', 'ACTIVO')\n\
\
doc.set_cell_value(2, 7, 'string', 'A) Fundadores/asociados por desembolsos no exigidos')\n\
doc.set_cell_value(3, 7, 'float', '" + (saldoCuenta(196) + saldoCuenta(197)).toQString() + "')\n\
\
doc.set_cell_value(2, 9, 'string', 'B) Inmovilizado')\n\
doc.set_cell_value(3, 9, 'formula', '=C10+C11')\n\
\
doc.set_cell_value(2, 10, 'string', '   I.  Gastos de investigación y desarrollo')\n\
doc.set_cell_value(3, 10, 'float', '" + saldoCuenta(20).toQString() + "')\n\
\
doc.set_cell_value(2, 11, 'string', '   II. Inmovilizaciones inmatariales')\n\
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
    connect(accion, SIGNAL(activated()), this, SLOT(balanceSituacionODS()));
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


