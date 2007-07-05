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
    /// Se genera el Balance de Situacion en formato ODS (Hoja de calculo OpenOffice.org).

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
doc.set_cell_value(2, 11, 'string', '   II.  Inmovilizaciones inmateriales')\n\
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
doc.set_cell_value(3, 41, 'formula', '=C42+C43+C51+C59+C68+C69+C70')\n\
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
doc.set_cell_value(2, 51, 'string', '   III. Deudores')\n\
doc.set_cell_value(3, 51, 'formula', '=SUM(C52:C58)')\n\
\
doc.set_cell_value(2, 52, 'string', '       1. Clientes por ventas y prestaciones de servicios')\n\
doc.set_cell_value(3, 52, 'float', '" + (saldoCuenta(430) + saldoCuenta(431) + saldoCuenta(435) - saldoCuenta(436)).toQString() + "')\n\
\
doc.set_cell_value(2, 53, 'string', '       2. Empresas del grupo, deudores')\n\
doc.set_cell_value(3, 53, 'float', '" + (saldoCuenta(432) + saldoCuenta(551)).toQString() + "')\n\
\
doc.set_cell_value(2, 54, 'string', '       3. Empresas asociadas, deudores')\n\
doc.set_cell_value(3, 54, 'float', '" + (saldoCuenta(433) + saldoCuenta(552)).toQString() + "')\n\
\
doc.set_cell_value(2, 55, 'string', '       4. Deudores varios')\n\
doc.set_cell_value(3, 55, 'float', '" + (saldoCuenta(44) + saldoCuenta(553)).toQString() + "')\n\
\
doc.set_cell_value(2, 56, 'string', '       5. Personal')\n\
doc.set_cell_value(3, 56, 'float', '" + (saldoCuenta(460) + saldoCuenta(544)).toQString() + "')\n\
\
doc.set_cell_value(2, 57, 'string', '       6. Administraciones Públicas')\n\
doc.set_cell_value(3, 57, 'float', '" + (saldoCuenta(470) + saldoCuenta(471) + saldoCuenta(472) + saldoCuenta(474)).toQString() + "')\n\
\
doc.set_cell_value(2, 58, 'string', '       7. Provisiones')\n\
doc.set_cell_value(3, 58, 'float', '" + ((-saldoCuenta(490)) + (-saldoCuenta(493)) + (-saldoCuenta(494))).toQString() + "')\n\
\
doc.set_cell_value(2, 59, 'string', '   IV.  Inversiones financieras temporales')\n\
doc.set_cell_value(3, 59, 'formula', '=SUM(C60:C67)')\n\
\
doc.set_cell_value(2, 60, 'string', '       1. Participaciones en empresas del grupo')\n\
doc.set_cell_value(3, 60, 'float', '" + (saldoCuenta(530) + (-saldoCuenta(538))).toQString() + "')\n\
\
doc.set_cell_value(2, 61, 'string', '       2. Créditos a empresas del grupo')\n\
doc.set_cell_value(3, 61, 'float', '" + (saldoCuenta(532) + saldoCuenta(534) + saldoCuenta(536)).toQString() + "')\n\
\
doc.set_cell_value(2, 62, 'string', '       3. Participaciones en empresas asociadas')\n\
doc.set_cell_value(3, 62, 'float', '" + (saldoCuenta(531) + (-saldoCuenta(539))).toQString() + "')\n\
\
doc.set_cell_value(2, 63, 'string', '       4. Creéditos a empresas asociadas')\n\
doc.set_cell_value(3, 63, 'float', '" + (saldoCuenta(533) + saldoCuenta(535) + saldoCuenta(537)).toQString() + "')\n\
\
doc.set_cell_value(2, 64, 'string', '       5. Cartera de valores a corto plazo')\n\
doc.set_cell_value(3, 64, 'float', '" + (saldoCuenta(540) + saldoCuenta(541) + saldoCuenta(546) + (-saldoCuenta(549))).toQString() + "')\n\
\
doc.set_cell_value(2, 65, 'string', '       6. Otros créditos')\n\
doc.set_cell_value(3, 65, 'float', '" + (saldoCuenta(542) + saldoCuenta(543) + saldoCuenta(545) + saldoCuenta(547) + saldoCuenta(548)).toQString() + "')\n\
\
doc.set_cell_value(2, 66, 'string', '       7. Depósitos y fianzas constituidos a corto plazo')\n\
doc.set_cell_value(3, 66, 'float', '" + (saldoCuenta(565) + saldoCuenta(566)).toQString() + "')\n\
\
doc.set_cell_value(2, 67, 'string', '       8. Provisiones')\n\
doc.set_cell_value(3, 67, 'float', '" + ((-saldoCuenta(593)) + (-saldoCuenta(594)) + (-saldoCuenta(595)) + (-saldoCuenta(596)) + (-saldoCuenta(597)) + (-saldoCuenta(598))).toQString() + "')\n\
\
doc.set_cell_value(2, 68, 'string', '   V.   Acciones propias a corto plazo')\n\
doc.set_cell_value(3, 68, 'formula', '=0')\n\
\
doc.set_cell_value(2, 69, 'string', '   VI.  Tesorería')\n\
doc.set_cell_value(3, 69, 'float', '" + saldoCuenta(57).toQString() + "')\n\
\
doc.set_cell_value(2, 70, 'string', '   VII. Ajustes por periodificación')\n\
doc.set_cell_value(3, 70, 'float', '" + (saldoCuenta(480) + saldoCuenta(580)).toQString() + "')\n\
\
doc.set_cell_property('bold', True)\n\
doc.set_cell_value(2, 72, 'string', 'TOTAL GENERAL (A+B+C+D)')\n\
doc.set_cell_value(3, 72, 'formula', '=C7+C9+C39+C41')\n\
doc.set_cell_property('bold', False)\n\
\
\
\
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
doc.set_cell_property('bold', True)\n\
doc.set_cell_value(2, 7, 'string', 'A) Fondos propios')\n\
doc.set_cell_value(3, 7, 'formula', '=C8+C9+C10+C11+C17+C21+C22')\n\
doc.set_cell_property('bold', False)\n\
\
doc.set_cell_value(2, 8, 'string', '   I.   Capital suscrito')\n\
doc.set_cell_value(3, 8, 'float', '" + saldoCuenta(10).toQString() + "')\n\
\
doc.set_cell_value(2, 9, 'string', '   II.  Prima de emisión')\n\
doc.set_cell_value(3, 9, 'float', '" + saldoCuenta(110).toQString() + "')\n\
\
doc.set_cell_value(2, 10, 'string', '   III. Reserva de revalorización')\n\
doc.set_cell_value(3, 10, 'float', '" + saldoCuenta(111).toQString() + "')\n\
\
doc.set_cell_value(2, 11, 'string', '   IV.  Reservas')\n\
doc.set_cell_value(3, 11, 'formula', '=SUM(C12:C16)')\n\
\
doc.set_cell_value(2, 12, 'string', '       1. Reserva legal')\n\
doc.set_cell_value(3, 12, 'float', '" + saldoCuenta(112).toQString() + "')\n\
\
doc.set_cell_value(2, 13, 'string', '       2. Reserva para acciones propias')\n\
doc.set_cell_value(3, 13, 'float', '" + saldoCuenta(115).toQString() + "')\n\
\
doc.set_cell_value(2, 14, 'string', '       3. Reserva para acciones de la sociedad dominante')\n\
doc.set_cell_value(3, 14, 'float', '" + saldoCuenta(114).toQString() + "')\n\
\
doc.set_cell_value(2, 15, 'string', '       4. Reservas estatutarias')\n\
doc.set_cell_value(3, 15, 'float', '" + saldoCuenta(116).toQString() + "')\n\
\
doc.set_cell_value(2, 16, 'string', '       5. Otras reservas')\n\
doc.set_cell_value(3, 16, 'float', '" + (saldoCuenta(113) + saldoCuenta(117) + saldoCuenta(118)).toQString() + "')\n\
\
doc.set_cell_value(2, 17, 'string', '   V.   Resultados de ejercicios anteriores')\n\
doc.set_cell_value(3, 17, 'formula', '=SUM(C18:C20)')\n\
\
doc.set_cell_value(2, 18, 'string', '       1. Remanente')\n\
doc.set_cell_value(3, 18, 'float', '" + saldoCuenta(120).toQString() + "')\n\
\
doc.set_cell_value(2, 19, 'string', '       2. Resultados negativos de ejercicios anteriores')\n\
doc.set_cell_value(3, 19, 'float', '" + (-saldoCuenta(121)).toQString() + "')\n\
\
doc.set_cell_value(2, 20, 'string', '       3. Aportaciones de socios para compensación de pérdidas')\n\
doc.set_cell_value(3, 20, 'float', '" + saldoCuenta(122).toQString() + "')\n\
\
doc.set_cell_value(2, 21, 'string', '   VI.  Pérdidas y ganancias (beneficio o pérdida)')\n\
doc.set_cell_value(3, 21, 'float', '" + saldoCuenta(129).toQString() + "')\n\
\
doc.set_cell_value(2, 22, 'string', '   VII. Dividendo a cuenta entregado en el ejercicio')\n\
doc.set_cell_value(3, 22, 'float', '" + (-saldoCuenta(557)).toQString() + "')\n\
\
doc.set_cell_property('bold', True)\n\
doc.set_cell_value(2, 24, 'string', 'B) Ingresos a distribuir en varios ejercicios')\n\
doc.set_cell_value(3, 24, 'formula', '=SUM(C25:C27)')\n\
doc.set_cell_property('bold', False)\n\
\
doc.set_cell_value(2, 25, 'string', '       1. Subvenciones de capital')\n\
doc.set_cell_value(3, 25, 'float', '" + (saldoCuenta(130) + saldoCuenta(131)).toQString() + "')\n\
\
doc.set_cell_value(2, 26, 'string', '       2. Diferencias positivas de cambio')\n\
doc.set_cell_value(3, 26, 'float', '" + saldoCuenta(136).toQString() + "')\n\
\
doc.set_cell_value(2, 27, 'string', '       3. Otros ingresos a distribuir en varios ejercicios')\n\
doc.set_cell_value(3, 27, 'float', '" + saldoCuenta(135).toQString() + "')\n\
\
doc.set_cell_property('bold', True)\n\
doc.set_cell_value(2, 29, 'string', 'C) Provisiones para riesgos y gastos')\n\
doc.set_cell_value(3, 29, 'formula', '=SUM(C30:C33)')\n\
doc.set_cell_property('bold', False)\n\
\
doc.set_cell_value(2, 30, 'string', '       1. Provisiones para pensiones y obligaciones similares')\n\
doc.set_cell_value(3, 30, 'float', '" + saldoCuenta(140).toQString() + "')\n\
\
doc.set_cell_value(2, 31, 'string', '       2. Provisiones para impuestos')\n\
doc.set_cell_value(3, 31, 'float', '" + saldoCuenta(141).toQString() + "')\n\
\
doc.set_cell_value(2, 32, 'string', '       3. Otras provisiones')\n\
doc.set_cell_value(3, 32, 'float', '" + (saldoCuenta(142) + saldoCuenta(143)).toQString() + "')\n\
\
doc.set_cell_value(2, 33, 'string', '       4. Fondo de reversión')\n\
doc.set_cell_value(3, 33, 'float', '" + saldoCuenta(144).toQString() + "')\n\
\
doc.set_cell_property('bold', True)\n\
doc.set_cell_value(2, 35, 'string', 'D) Acreedores a largo plazo')\n\
doc.set_cell_value(3, 35, 'formula', '=C36+C40+C41+C44+C48')\n\
doc.set_cell_property('bold', False)\n\
\
doc.set_cell_value(2, 36, 'string', '   I.   Emisiones de obligaciones y otros valores negociables')\n\
doc.set_cell_value(3, 11, 'formula', '=SUM(C37:C39)')\n\
\
doc.set_cell_value(2, 37, 'string', '       1. Obligaciones no convertibles')\n\
doc.set_cell_value(3, 37, 'float', '" + saldoCuenta(150).toQString() + "')\n\
\
doc.set_cell_value(2, 38, 'string', '       2. Obligaciones convertibles')\n\
doc.set_cell_value(3, 38, 'float', '" + saldoCuenta(151).toQString() + "')\n\
\
doc.set_cell_value(2, 39, 'string', '       3. Otras deudas representadas en valores negociables')\n\
doc.set_cell_value(3, 39, 'float', '" + saldoCuenta(155).toQString() + "')\n\
\
doc.set_cell_value(2, 40, 'string', '   II.  Deudas con entidades de crédito')\n\
doc.set_cell_value(3, 40, 'float', '" + saldoCuenta(170).toQString() + "')\n\
\
doc.set_cell_value(2, 41, 'string', '   III. Deudas con empresas del grupo y asociadas')\n\
doc.set_cell_value(3, 41, 'formula', '=SUM(C42:C43)')\n\
\
doc.set_cell_value(2, 42, 'string', '       1. Deudas con empresas del grupo')\n\
doc.set_cell_value(3, 42, 'float', '" + (saldoCuenta(160) + saldoCuenta(162) + saldoCuenta(164)).toQString() + "')\n\
\
doc.set_cell_value(2, 43, 'string', '       2. Deudas con empresas asociadas')\n\
doc.set_cell_value(3, 43, 'float', '" + (saldoCuenta(161) + saldoCuenta(163) + saldoCuenta(165)).toQString() + "')\n\
\
doc.set_cell_value(2, 44, 'string', '   IV.  Otros acreedores')\n\
doc.set_cell_value(2, 45, 'string', '       1. Deudas representadas por efectos a pagar')\n\
doc.set_cell_value(3, 45, 'float', '" + saldoCuenta(174).toQString() + "')\n\
\
doc.set_cell_value(2, 46, 'string', '       2. Otras deudas')\n\
doc.set_cell_value(3, 46, 'float', '" + (saldoCuenta(171) + saldoCuenta(172) + saldoCuenta(173)).toQString() + "')\n\
\
doc.set_cell_value(2, 47, 'string', '       3. Fianzas y depósitos recibidos a largo plazo')\n\
doc.set_cell_value(3, 47, 'float', '" + (saldoCuenta(180) + saldoCuenta(185)).toQString() + "')\n\
\
doc.set_cell_value(2, 48, 'string', '   V.   Desembolsos pendientes sobre acciones, no exigidos')\n\
doc.set_cell_value(2, 49, 'string', '       1. De empresas del grupo')\n\
doc.set_cell_value(3, 49, 'float', '" + saldoCuenta(248).toQString() + "')\n\
\
doc.set_cell_value(2, 50, 'string', '       2. De empresas asociadas')\n\
doc.set_cell_value(3, 50, 'float', '" + saldoCuenta(249).toQString() + "')\n\
\
doc.set_cell_value(2, 51, 'string', '       3. De otras empresas')\n\
doc.set_cell_value(3, 51, 'float', '" + saldoCuenta(259).toQString() + "')\n\
\
doc.set_cell_property('bold', True)\n\
doc.set_cell_value(2, 53, 'string', 'E) Acreedores a corto plazo')\n\
doc.set_cell_value(3, 53, 'formula', '=0')\n\
doc.set_cell_property('bold', False)\n\
\
doc.set_cell_value(2, 54, 'string', '   I.   Emisiones de obligaciones y otros valores negociables')\n\
doc.set_cell_value(2, 55, 'string', '       1. Obligaciones no convertibles')\n\
doc.set_cell_value(3, 55, 'float', '" + saldoCuenta(500).toQString() + "')\n\
\
doc.set_cell_value(2, 56, 'string', '       2. Obligaciones convertibles')\n\
doc.set_cell_value(3, 56, 'float', '" + saldoCuenta(501).toQString() + "')\n\
\
doc.set_cell_value(2, 57, 'string', '       3. Otras deudas representadas en valores negociables')\n\
doc.set_cell_value(3, 57, 'float', '" + saldoCuenta(505).toQString() + "')\n\
\
doc.set_cell_value(2, 58, 'string', '       4. Intereses de obligaciones y otros valores')\n\
doc.set_cell_value(3, 58, 'float', '" + saldoCuenta(506).toQString() + "')\n\
\
doc.set_cell_value(2, 59, 'string', '   II.  Deudas con entidades de crédito')\n\
doc.set_cell_value(2, 60, 'string', '       1. Préstamos y otras deudas')\n\
doc.set_cell_value(3, 60, 'float', '" + saldoCuenta(520).toQString() + "')\n\
\
doc.set_cell_value(2, 61, 'string', '       2. Deudas por intereses')\n\
doc.set_cell_value(3, 61, 'float', '" + saldoCuenta(526).toQString() + "')\n\
\
doc.set_cell_value(2, 62, 'string', '   III. Deudas con empresas del grupo y asociadas a corto plazo')\n\
doc.set_cell_value(2, 63, 'string', '       1. Deudas con empresas del grupo')\n\
doc.set_cell_value(3, 63, 'float', '" + (saldoCuenta(402) + saldoCuenta(510) + saldoCuenta(512) + saldoCuenta(514) + saldoCuenta(516) + saldoCuenta(551)).toQString() + "')\n\
\
doc.set_cell_value(2, 64, 'string', '       2. Deudas con empresas asociadas')\n\
doc.set_cell_value(3, 64, 'float', '" + (saldoCuenta(403) + saldoCuenta(511) + saldoCuenta(513) + saldoCuenta(515) + saldoCuenta(517) + saldoCuenta(552)).toQString() + "')\n\
\
doc.set_cell_value(2, 65, 'string', '   IV.  Acreedores comerciales')\n\
doc.set_cell_value(2, 66, 'string', '       1. Anticipos recibidos por pedidos')\n\
doc.set_cell_value(3, 66, 'float', '" + saldoCuenta(437).toQString() + "')\n\
\
doc.set_cell_value(2, 67, 'string', '       2. Deudas por compras o prestaciones de servicios')\n\
doc.set_cell_value(3, 67, 'float', '" + (saldoCuenta(400) + (-saldoCuenta(406)) + saldoCuenta(410) + saldoCuenta(419)).toQString() + "')\n\
\
doc.set_cell_value(2, 68, 'string', '       3. Deudas representadas por efectos a pagar')\n\
doc.set_cell_value(3, 68, 'float', '" + (saldoCuenta(401) + saldoCuenta(411)).toQString() + "')\n\
\
doc.set_cell_value(2, 69, 'string', '   V.   Otras deudas no comerciales')\n\
doc.set_cell_value(2, 70, 'string', '       1. Administraciones Públicas')\n\
doc.set_cell_value(3, 70, 'float', '" + (saldoCuenta(475) + saldoCuenta(476) + saldoCuenta(477) + saldoCuenta(479)).toQString() + "')\n\
\
doc.set_cell_value(2, 71, 'string', '       2. Deudas representadas por efectos a pagar')\n\
doc.set_cell_value(3, 71, 'float', '" + saldoCuenta(524).toQString() + "')\n\
\
doc.set_cell_value(2, 72, 'string', '       3. Otras deudas')\n\
doc.set_cell_value(3, 72, 'float', '" + (saldoCuenta(509) + saldoCuenta(521) + saldoCuenta(523) + saldoCuenta(525) + saldoCuenta(527) + saldoCuenta(553) + saldoCuenta(555) + saldoCuenta(556)).toQString() + "')\n\
\
doc.set_cell_value(2, 73, 'string', '       4. Remuneraciones pendientes de pago')\n\
doc.set_cell_value(3, 73, 'float', '" + saldoCuenta(465).toQString() + "')\n\
\
doc.set_cell_value(2, 74, 'string', '       5. Fianzas y depósitos recibidos a corto plazo')\n\
doc.set_cell_value(3, 74, 'float', '" + (saldoCuenta(560) + saldoCuenta(561)).toQString() + "')\n\
\
doc.set_cell_value(2, 75, 'string', '   VI.  Provisiones para operaciones de tráfico')\n\
doc.set_cell_value(3, 75, 'float', '" + saldoCuenta(499).toQString() + "')\n\
\
doc.set_cell_value(2, 76, 'string', '   VII. Ajustes por periodificación')\n\
doc.set_cell_value(3, 76, 'float', '" + (saldoCuenta(485) + saldoCuenta(585)).toQString() + "')\n\
\
doc.set_cell_property('bold', True)\n\
doc.set_cell_value(2, 78, 'string', 'TOTAL GENERAL (A+B+C+D+E)')\n\
doc.set_cell_value(3, 78, 'formula', '=C7+C24+29+35+53')\n\
doc.set_cell_property('bold', False)\n\
\
\
\
\
\
\
doc.new_sheet(\"Debe\")\n\n\
doc.set_column_property(1, 'width', '0.5in')\n\
doc.set_column_property(2, 'width', '5in')\n\
doc.set_cell_property('bold', True)\n\
doc.set_cell_value(2, 2, 'string', 'CUENTA DE PÉRDIDAS Y GANANCIAS')\n\
doc.set_cell_value(2, 5, 'string', 'DEBE')\n\
doc.set_cell_property('bold', False)\n\
\
doc.set_cell_property('bold', True)\n\
doc.set_cell_value(2, 7, 'string', 'A) GASTOS')\n\
doc.set_cell_value(3, 7, 'formula', '=0')\n\
doc.set_cell_property('bold', False)\n\
\
doc.set_cell_value(2, 8, 'string', '       1. Reducción de existencias de productos terminados y en curso de fabricación')\n\
doc.set_cell_value(3, 8, 'float', '" + saldoCuenta(71).toQString() + "')\n\
\
doc.set_cell_value(2, 9, 'string', '       2. Aprovisionamientos')\n\
doc.set_cell_value(3, 9, 'float', '" + (saldoCuenta(600) + (-saldoCuenta(6080))).toQString() + "')\n\
\
doc.set_cell_value(2, 10, 'string', '            a) Consumo de mercaderías')\n\
doc.set_cell_value(3, 10, 'float', '" + ((-saldoCuenta(6090)) + saldoCuenta(610)).toQString() + "')\n\
\
doc.set_cell_value(2, 11, 'string', '            b) Consumo de materias primas y otras materias consumibles')\n\
doc.set_cell_value(3, 11, 'float', '" + (saldoCuenta(601) + saldoCuenta(602) + (-saldoCuenta(6081)) + (-saldoCuenta(6082)) + (-saldoCuenta(6091)) + (-saldoCuenta(6092)) +saldoCuenta(611) + saldoCuenta(612)).toQString() + "')\n\
\
doc.set_cell_value(2, 12, 'string', '            c) Otros gastos externos')\n\
doc.set_cell_value(3, 12, 'float', '" + saldoCuenta(607).toQString() + "')\n\
\
doc.set_cell_value(2, 13, 'string', '       3. Gastos de personal')\n\
doc.set_cell_value(2, 14, 'string', '            a) Sueldos, salarios y asimilados')\n\
doc.set_cell_value(3, 14, 'float', '" + (saldoCuenta(640) + saldoCuenta(641)).toQString() + "')\n\
\
doc.set_cell_value(2, 15, 'string', '            b) Cargas sociales')\n\
doc.set_cell_value(3, 15, 'float', '" + (saldoCuenta(642) + saldoCuenta(643) + saldoCuenta(649)).toQString() + "')\n\
\
doc.set_cell_value(2, 16, 'string', '       4. Dotaciones para amortizaciones de inmovilizado')\n\
doc.set_cell_value(3, 16, 'float', '" + saldoCuenta(68).toQString() + "')\n\
\
doc.set_cell_value(2, 17, 'string', '       5. Variación de las provisiones de tráfico')\n\
doc.set_cell_value(2, 18, 'string', '            a) Variaciones de las provisiones de existencias')\n\
doc.set_cell_value(3, 18, 'float', '" + (saldoCuenta(693) + (-saldoCuenta(793))).toQString() + "')\n\
\
doc.set_cell_value(2, 19, 'string', '            b) Variaciones de provisiones y pérdidas de créditos incobrables')\n\
doc.set_cell_value(3, 19, 'float', '" + (saldoCuenta(650) + saldoCuenta(694) + (-saldoCuenta(794))).toQString() + "')\n\
\
doc.set_cell_value(2, 20, 'string', '            c) Variaciones de otras provisiones de tráfico')\n\
doc.set_cell_value(3, 20, 'float', '" + (saldoCuenta(695) + (-saldoCuenta(795))).toQString() + "')\n\
\
doc.set_cell_value(2, 21, 'string', '       6. Otros gastos de explotación')\n\
doc.set_cell_value(2, 22, 'string', '            a) Servicios exteriores')\n\
doc.set_cell_value(2, 23, 'string', '            b) Tributos')\n\
doc.set_cell_value(2, 24, 'string', '            c) Otros gastos de gestión corriente')\n\
doc.set_cell_value(2, 25, 'string', '            d) Dotación al fondo de reversión')\n\
\
doc.set_cell_value(2, 27, 'string', '   I.   BENEFICIOS DE EXLOTACIÓN')\n\
doc.set_cell_value(2, 28, 'string', '        (B1+B2+B3+B4-A1-A2-A3-A4-A5-A6)')\n\
\
doc.set_cell_value(2, 30, 'string', '       7. Gastos financieros y gastos asimilados')\n\
doc.set_cell_value(2, 31, 'string', '            a) Por deudas con empresas del grupo')\n\
doc.set_cell_value(2, 32, 'string', '            b) Por deudas con empresas asociadas')\n\
doc.set_cell_value(2, 33, 'string', '            c) Por deudas con terceros y gastos asimilados')\n\
doc.set_cell_value(2, 34, 'string', '            d) Pérdidas de inversiones financieras')\n\
doc.set_cell_value(2, 35, 'string', '       8. Variación de las provisiones de inversiones financieras')\n\
doc.set_cell_value(2, 36, 'string', '       9. Diferencias negativas de cambio')\n\
\
doc.set_cell_value(2, 38, 'string', '   II.  RESULTADOS FINANCIEROS POSITIVOS')\n\
doc.set_cell_value(2, 39, 'string', '        (B5+B6+B7+B8-A7-A8-A9)')\n\
\
doc.set_cell_value(2, 41, 'string', '      10. Variación de las provisiones de inmobilizado inmaterial, material y cartera de control')\n\
doc.set_cell_value(2, 42, 'string', '      11. Pérdidas procedentes del inmovilizado')\n\
doc.set_cell_value(2, 43, 'string', '      12. Pérdidas por operaciones con acciones y obligaciones propias')\n\
doc.set_cell_value(2, 44, 'string', '      13. Gastos extraordinarios')\n\
doc.set_cell_value(2, 45, 'string', '      14. Gastos y pérdidas de otros ejercicios')\n\
\
doc.set_cell_value(2, 47, 'string', '   IV.  RESULTADOS EXTRAORDINARIOS POSITIVOS')\n\
doc.set_cell_value(2, 48, 'string', '        (B9+B10+B11+B12+B13-A10-A11-A12-A13-A14)')\n\
\
doc.set_cell_value(2, 50, 'string', '   V.   BENEFICIOS ANTES DE IMPUESTOS')\n\
doc.set_cell_value(2, 51, 'string', '        (AIII+AIV-BIII-BIV)')\n\
\
doc.set_cell_value(2, 53, 'string', '      15. Impuesto sobre Sociedades')\n\
doc.set_cell_value(2, 54, 'string', '      16. Otros impuestos')\n\
\
doc.set_cell_value(2, 56, 'string', '   VI.  RESULTADO DEL EJERCICIO (BENEFICIOS)')\n\
doc.set_cell_value(2, 57, 'string', '        (AV-A15-A16)')\n\
\
\
\
\
\
\
\
\
\
\
\
doc.new_sheet(\"Haber\")\n\n\
doc.set_column_property(1, 'width', '0.5in')\n\
doc.set_column_property(2, 'width', '5in')\n\
doc.set_cell_property('bold', True)\n\
doc.set_cell_value(2, 2, 'string', 'CUENTA DE PÉRDIDAS Y GANANCIAS')\n\
doc.set_cell_value(2, 5, 'string', 'HABER')\n\
doc.set_cell_property('bold', False)\n\
\
doc.set_cell_value(2, 7, 'string', 'B) INGRESOS')\n\
doc.set_cell_value(2, 8, 'string', '       1. Importe neto de la cifra de negocio')\n\
doc.set_cell_value(2, 9, 'string', '            a) Ventas')\n\
doc.set_cell_value(2, 10, 'string', '            b) Prestaciones de servicios')\n\
doc.set_cell_value(2, 11, 'string', '            c) Devoluciones y \"rappels\" sobre ventas')\n\
doc.set_cell_value(2, 12, 'string', '       2. Aumento de las existencias de productos terminados y en curso de fabricación')\n\
doc.set_cell_value(2, 13, 'string', '       3. Trabajos efectuados por la empresa para el inmobilizado')\n\
doc.set_cell_value(2, 14, 'string', '       4. Otros ingresos de explotación')\n\
doc.set_cell_value(2, 15, 'string', '            a) Ingresos accesorios y otros de gestión corriente')\n\
doc.set_cell_value(2, 16, 'string', '            b) Subvenciones')\n\
doc.set_cell_value(2, 17, 'string', '            c) Exceso de provisiones de riesgos y gastos')\n\
\
doc.set_cell_value(2, 19, 'string', '   I.   PÉRDIDAS DE EXPLOTACIÓN')\n\
doc.set_cell_value(2, 20, 'string', '        (A1+A2+A3+A4+A5+A6-B1-B2-B3-B4)')\n\
\
doc.set_cell_value(2, 22, 'string', '       5. Ingresos de participaciones en capital')\n\
doc.set_cell_value(2, 23, 'string', '            a) En empresas del grupo')\n\
doc.set_cell_value(2, 24, 'string', '            b) En empresas asociadas')\n\
doc.set_cell_value(2, 25, 'string', '            c) En empresas fuera del grupo')\n\
doc.set_cell_value(2, 26, 'string', '       6. Ingresos de otros valores mobiliarios y de créditos del activo inmovilizado')\n\
doc.set_cell_value(2, 27, 'string', '            a) De empresas del grupo')\n\
doc.set_cell_value(2, 28, 'string', '            b) De empresas asociadas')\n\
doc.set_cell_value(2, 28, 'string', '            c) De empresas fuera del grupo')\n\
doc.set_cell_value(2, 30, 'string', '       7. Otros intereses e ingresos asimilados')\n\
doc.set_cell_value(2, 31, 'string', '            a) De empresas del grupo')\n\
doc.set_cell_value(2, 32, 'string', '            b) De empresas asociadas')\n\
doc.set_cell_value(2, 33, 'string', '            c) Otros intereses')\n\
doc.set_cell_value(2, 34, 'string', '            d) Beneficios en inversiones financieras temporales')\n\
doc.set_cell_value(2, 35, 'string', '       8. Diferencias positivas de cambio')\n\
\
doc.set_cell_value(2, 37, 'string', '   II.  RESULTADOS FINANCIEROS NEGATIVOS')\n\
doc.set_cell_value(2, 38, 'string', '        (A7+A8+A9+A10-B5-B6-B7-B8)')\n\
\
doc.set_cell_value(2, 40, 'string', '   III. PÉRDIDAS DE LAS ACTIVIDADES ORDINARIAS')\n\
doc.set_cell_value(2, 41, 'string', '        (BI+BII-A1-AII)')\n\
\
doc.set_cell_value(2, 43, 'string', '       9. Beneficios en enajenación de inmovilizado inmaterial, material y cartera de control')\n\
doc.set_cell_value(2, 44, 'string', '      10. Beneficios por operaciones con acciones y obligaciones propias')\n\
doc.set_cell_value(2, 45, 'string', '      11. Subvenciones de capital transferidas al resultado del ejercicio')\n\
doc.set_cell_value(2, 46, 'string', '      12. Ingresos extraordinarios')\n\
doc.set_cell_value(2, 47, 'string', '      13. Ingresos y beneficios de otros ejercicios')\n\
\
doc.set_cell_value(2, 49, 'string', '   IV. RESULTADOS EXTRAORDINARIOS NEGATIVOS')\n\
doc.set_cell_value(2, 50, 'string', '       (A11+A12+A13+A14+A15-B9-B10-B11-B12-B13)')\n\
\
doc.set_cell_value(2, 52, 'string', '   V.  PÉRDIDAS ANTES DE IMPUESTOS')\n\
doc.set_cell_value(2, 53, 'string', '       (BIII+BIV-AIII-AIV)')\n\
\
doc.set_cell_value(2, 55, 'string', '   VI. RESULTADO DEL EJERCICIO (PÉRDIDAS)')\n\
doc.set_cell_value(2, 56, 'string', '       (BV+A16+A17)')\n\
\
\
\
\
\
\
# Guardamos el documento\n\
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

