/***************************************************************************
 *   Copyright (C) 2008 by Adelaida Clavaguera Mora <laida@alaxarxa.net>   *
 *   Copyright (C) 2008 by Leopold Palomo Avellaneda <leo@alaxarxa.net>    *
 *   Copyright (C) 2006 by Fco. Javier M. C. >fcojavmc@todo-redes.com>     *
 *   Copyright (C) 2005 by Tomeu Borras Riera <tborras@conetxia.com>       *
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

#include <QtCore/QString>
#include "pluginbc_cuentasanuales2ods.h"

///
/**
**/

QString PluginBc_CuentasAnuales2ODS::cuentaAnualCAAPGC08()
{
    return QString::QString ( "\
#!/usr/bin/python\n\
# -*- coding: utf8 -*-\n\
\n\
import ooolib\n\
\n\
# Crea el documento\n\
\
\
\
\n\
doc = ooolib.Calc(\"ACTIVO\")\n\n\
doc.set_column_property(1, 'width', '0.5in')\n\
doc.set_column_property(2, 'width', '5in')\n\
\
doc.set_cell_property('bold', True)\n\
doc.set_cell_value(2,2, 'string', 'BALANCE ABREVIADO AL CIERRE DEL EJERCICIO')\n\
doc.set_cell_value(2,4, 'string', 'ACTIVO')\n\
doc.set_cell_value(3,4, 'string', 'NOTAS')\n\
doc.set_cell_value(4,4, 'string', 'Ejercicio N')\n\
doc.set_cell_value(5,4, 'string', 'Ejercicio N-1')\n\
doc.set_cell_property('bold', False)\n\
\
\
\
doc.set_cell_property('bold', True)\n\
doc.set_cell_value(2,6, 'string', 'A) ACTIVO NO CORRIENTE')\n\
doc.set_cell_value(4,6, 'formula', '=D8+D9+D10+D11+D12+D13')\n\
doc.set_cell_value(5,6, 'formula', '=E8+E9+E10+E11+E12+E13')\n\
doc.set_cell_property('bold', False)\n\
\
\
\
doc.set_cell_property('bold', True)\n\
doc.set_cell_value(2,8, 'string', 'I. Inmovilizado intangible.')\n\
doc.set_cell_value(4,8, 'float', '" + ( saldoCuenta ( 20 ) + saldoCuenta ( 280 ) + saldoCuenta ( 290 ) ).toQString ( '.' ) + "')\n\
doc.set_cell_value(5,8, 'float', '" + ( saldoCuentaAnt ( 20 ) + saldoCuentaAnt ( 280 ) + saldoCuentaAnt ( 290 ) ).toQString ( '.' ) + "')\n\
doc.set_cell_property('bold', False)\n\
\
doc.set_cell_property('bold', True)\n\
doc.set_cell_value(2,9, 'string', 'II. Inmovilizado material.')\n\
doc.set_cell_value(4,9, 'float', '" + ( saldoCuenta ( 21 ) + saldoCuenta ( 281 ) + saldoCuenta ( 291 ) + saldoCuenta ( 23 ) ).toQString ( '.' ) + "')\n\
doc.set_cell_value(5,9, 'float', '" + ( saldoCuentaAnt ( 21 ) + saldoCuentaAnt ( 281 ) + saldoCuentaAnt ( 291 ) + saldoCuentaAnt ( 23 ) ).toQString ( '.' ) + "')\n\
doc.set_cell_property('bold', False)\n\
\
doc.set_cell_property('bold', True)\n\
doc.set_cell_value(2,10, 'string', 'III. Inversiones inmobiliarias.')\n\
doc.set_cell_value(4,10, 'float', '" + ( saldoCuenta ( 22 ) + saldoCuenta ( 282 ) + saldoCuenta ( 292 ) ).toQString ( '.' ) + "')\n\
doc.set_cell_value(5,10, 'float', '" + ( saldoCuentaAnt ( 22 ) + saldoCuentaAnt ( 282 ) + saldoCuentaAnt ( 292 ) ).toQString ( '.' ) + "')\n\
doc.set_cell_property('bold', False)\n\
\
doc.set_cell_property('bold', True)\n\
doc.set_cell_value(2,11, 'string', 'IV. Inversiones en empresas del grupo y asociadas a largo plazo.')\n\
doc.set_cell_value(4,11, 'float', '" + ( saldoCuenta ( 2403 ) + saldoCuenta ( 2404 ) + saldoCuenta ( 2413 ) + saldoCuenta ( 2414 ) + saldoCuenta ( 2423 ) + saldoCuenta ( 2424 ) + saldoCuenta ( 2493 ) + saldoCuenta ( 2494 ) + saldoCuenta ( 293 ) + saldoCuenta ( 2943 ) + saldoCuenta ( 2944 ) + saldoCuenta ( 2953 ) + saldoCuenta ( 2954 ) ).toQString ( '.' ) + "')\n\
doc.set_cell_value(5,11, 'float', '" + ( saldoCuentaAnt ( 2403 ) + saldoCuentaAnt ( 2404 ) + saldoCuentaAnt ( 2413 ) + saldoCuentaAnt ( 2414 ) + saldoCuentaAnt ( 2423 ) + saldoCuentaAnt ( 2424 ) + saldoCuentaAnt ( 2493 ) + saldoCuentaAnt ( 2494 ) + saldoCuentaAnt ( 293 ) + saldoCuentaAnt ( 2943 ) + saldoCuentaAnt ( 2944 ) + saldoCuentaAnt ( 2953 ) + saldoCuentaAnt ( 2954 ) ).toQString ( '.' ) + "')\n\
doc.set_cell_property('bold', False)\n\
\
doc.set_cell_property('bold', True)\n\
doc.set_cell_value(2,12, 'string', 'V. Inversiones financieras a largo plazo.')\n\
doc.set_cell_value(4,12, 'float', '" + ( saldoCuenta ( 2405 ) + saldoCuenta ( 2415 ) + saldoCuenta ( 2425 ) + saldoCuenta ( 2495 ) + saldoCuenta ( 250 ) + saldoCuenta ( 251 ) + saldoCuenta ( 252 ) + saldoCuenta ( 253 ) + saldoCuenta ( 254 ) + saldoCuenta ( 255 ) + saldoCuenta ( 257 ) + saldoCuenta ( 258 ) + saldoCuenta ( 259 ) + saldoCuenta ( 26 ) + saldoCuenta ( 2945 ) + saldoCuenta ( 2955 ) + saldoCuenta ( 297 ) + saldoCuenta ( 298 ) ).toQString ( '.' ) + "')\n\
doc.set_cell_value(5,12, 'float', '" + ( saldoCuentaAnt ( 2405 ) + saldoCuentaAnt ( 2415 ) + saldoCuentaAnt ( 2425 ) + saldoCuentaAnt ( 2495 ) + saldoCuentaAnt ( 250 ) + saldoCuentaAnt ( 251 ) + saldoCuentaAnt ( 252 ) + saldoCuentaAnt ( 253 ) + saldoCuentaAnt ( 254 ) + saldoCuentaAnt ( 255 ) + saldoCuentaAnt ( 257 ) + saldoCuentaAnt ( 258 ) + saldoCuentaAnt ( 259 ) + saldoCuentaAnt ( 26 ) + saldoCuentaAnt ( 2945 ) + saldoCuentaAnt ( 2955 ) + saldoCuentaAnt ( 297 ) + saldoCuentaAnt ( 298 ) ).toQString ( '.' ) + "')\n\
doc.set_cell_property('bold', False)\n\
\
doc.set_cell_property('bold', True)\n\
doc.set_cell_value(2,13, 'string', 'VI. Activos por Impuesto diferido.')\n\
doc.set_cell_value(4,13, 'float', '" + ( saldoCuenta ( 474 ) ).toQString ( '.' ) + "')\n\
doc.set_cell_value(5,13, 'float', '" + ( saldoCuentaAnt ( 474 ) ).toQString ( '.' ) + "')\n\
doc.set_cell_property('bold', False)\n\
\
\
\
\
doc.set_cell_property('bold', True)\n\
doc.set_cell_value(2,15, 'string', 'B) ACTIVO CORRIENTE')\n\
doc.set_cell_value(4,15, 'formula', '=D17+D18+D22+D23+D24+D25+D26')\n\
doc.set_cell_value(5,15, 'formula', '=E17+E18+E22+E23+E24+E25+E26')\n\
doc.set_cell_property('bold', False)\n\
\
\
\
doc.set_cell_property('bold', True)\n\
doc.set_cell_value(2,17, 'string', 'I. Activos no corrientes mantenidos para la venta.')\n\
doc.set_cell_value(4,17, 'float', '" + ( saldoCuenta ( 580 ) + saldoCuenta ( 581 ) + saldoCuenta ( 582 ) + saldoCuenta ( 583 ) + saldoCuenta ( 584 ) + saldoCuenta ( 599 ) ).toQString ( '.' ) + "')\n\
doc.set_cell_value(5,17, 'float', '" + ( saldoCuentaAnt ( 580 ) + saldoCuentaAnt ( 581 ) + saldoCuentaAnt ( 582 ) + saldoCuentaAnt ( 583 ) + saldoCuentaAnt ( 584 ) + saldoCuentaAnt ( 599 ) ).toQString ( '.' ) + "')\n\
doc.set_cell_property('bold', False)\n\
\
doc.set_cell_property('bold', True)\n\
doc.set_cell_value(2,18, 'string', 'II. Existencias.')\n\
doc.set_cell_value(4,18, 'float', '" + ( saldoCuenta ( 30 ) + saldoCuenta ( 31 ) + saldoCuenta ( 32 ) + saldoCuenta ( 33 ) + saldoCuenta ( 34 ) + saldoCuenta ( 35 ) + saldoCuenta ( 36 ) + saldoCuenta ( 39 ) + saldoCuenta ( 407 ) ).toQString ( '.' ) + "')\n\
doc.set_cell_value(5,18, 'float', '" + ( saldoCuentaAnt ( 30 ) + saldoCuentaAnt ( 31 ) + saldoCuentaAnt ( 32 ) + saldoCuentaAnt ( 33 ) + saldoCuentaAnt ( 34 ) + saldoCuentaAnt ( 35 ) + saldoCuentaAnt ( 36 ) + saldoCuentaAnt ( 39 ) + saldoCuentaAnt ( 407 ) ).toQString ( '.' ) + "')\n\
doc.set_cell_property('bold', False)\n\
\
doc.set_cell_property('bold', True)\n\
doc.set_cell_value(2,19, 'string', 'III. Deudores comerciales y otras cuentas a cobrar.')\n\
doc.set_cell_value(4,19, 'formula', '=D20+D21+D22')\n\
doc.set_cell_value(5,19, 'formula', '=E20+E21+E22')\n\
doc.set_cell_property('bold', False)\n\
\
doc.set_cell_value(2,20, 'string', '1. Clientes por ventas y prestaciones de servicios.')\n\
doc.set_cell_value(4,20, 'float', '" + ( saldoCuenta ( 430 ) + saldoCuenta ( 431 ) + saldoCuenta ( 432 ) + saldoCuenta ( 433 ) + saldoCuenta ( 434 ) + saldoCuenta ( 435 ) + saldoCuenta ( 436 ) + saldoCuenta ( 437 ) + saldoCuenta ( 490 ) + saldoCuenta ( 493 ) ).toQString ( '.' ) + "')\n\
doc.set_cell_value(5,20, 'float', '" + ( saldoCuentaAnt ( 430 ) + saldoCuentaAnt ( 431 ) + saldoCuentaAnt ( 432 ) + saldoCuentaAnt ( 433 ) + saldoCuentaAnt ( 434 ) + saldoCuentaAnt ( 435 ) + saldoCuentaAnt ( 436 ) + saldoCuentaAnt ( 437 ) + saldoCuentaAnt ( 490 ) + saldoCuentaAnt ( 493 ) ).toQString ( '.' ) + "')\n\
\
doc.set_cell_value(2,21, 'string', '2. Accionistas (socios) por desembolsos exigidos.')\n\
doc.set_cell_value(4,21, 'float', '" + ( saldoCuenta ( 5580 ) ).toQString ( '.' ) + "')\n\
doc.set_cell_value(5,21, 'float', '" + ( saldoCuentaAnt ( 5580 ) ).toQString ( '.' ) + "')\n\
\
doc.set_cell_value(2,22, 'string', '3. Otros deudores.')\n\
doc.set_cell_value(4,22, 'float', '" + ( saldoCuenta ( 44 ) + saldoCuenta ( 460 ) + saldoCuenta ( 470 ) + saldoCuenta ( 471 ) + saldoCuenta ( 472 ) + saldoCuenta ( 5531 ) + saldoCuenta ( 5533 ) + saldoCuenta ( 544 ) ).toQString ( '.' ) + "')\n\
doc.set_cell_value(5,22, 'float', '" + ( saldoCuentaAnt ( 44 ) + saldoCuentaAnt ( 460 ) + saldoCuentaAnt ( 470 ) + saldoCuentaAnt ( 471 ) + saldoCuentaAnt ( 472 ) + saldoCuentaAnt ( 5531 ) + saldoCuentaAnt ( 5533 ) + saldoCuentaAnt ( 544 ) ).toQString ( '.' ) + "')\n\
\
doc.set_cell_property('bold', True)\n\
doc.set_cell_value(2,23, 'string', 'IV. Inversiones en empresas del grupo y asociadas a corto plazo.')\n\
doc.set_cell_value(4,23, 'float', '" + ( saldoCuenta ( 5303 ) + saldoCuenta ( 5304 ) + saldoCuenta ( 5313 ) + saldoCuenta ( 5314 ) + saldoCuenta ( 5323 ) + saldoCuenta ( 5324 ) + saldoCuenta ( 5333 ) + saldoCuenta ( 5334 ) + saldoCuenta ( 5343 ) + saldoCuenta ( 5344 ) + saldoCuenta ( 5353 ) + saldoCuenta ( 5354 ) + saldoCuenta ( 5393 ) + saldoCuenta ( 5394 ) + saldoCuenta ( 5523 ) + saldoCuenta ( 5524 ) + saldoCuenta ( 593 ) + saldoCuenta ( 5943 ) + saldoCuenta ( 5944 ) + saldoCuenta ( 5953 ) + saldoCuenta ( 5954 ) ).toQString ( '.' ) + "')\n\
doc.set_cell_value(5,23, 'float', '" + ( saldoCuentaAnt ( 5303 ) + saldoCuentaAnt ( 5304 ) + saldoCuentaAnt ( 5313 ) + saldoCuentaAnt ( 5314 ) + saldoCuentaAnt ( 5323 ) + saldoCuentaAnt ( 5324 ) + saldoCuentaAnt ( 5333 ) + saldoCuentaAnt ( 5334 ) + saldoCuentaAnt ( 5343 ) + saldoCuentaAnt ( 5344 ) + saldoCuentaAnt ( 5353 ) + saldoCuentaAnt ( 5354 ) + saldoCuentaAnt ( 5393 ) + saldoCuentaAnt ( 5394 ) + saldoCuentaAnt ( 5523 ) + saldoCuentaAnt ( 5524 ) + saldoCuentaAnt ( 593 ) + saldoCuentaAnt ( 5943 ) + saldoCuentaAnt ( 5944 ) + saldoCuentaAnt ( 5953 ) + saldoCuentaAnt ( 5954 ) ).toQString ( '.' ) + "')\n\
doc.set_cell_property('bold', False)\n\
\
doc.set_cell_property('bold', True)\n\
doc.set_cell_value(2,24, 'string', 'V. Inversiones financieras a corto plazo.')\n\
doc.set_cell_value(4,24, 'float', '" + ( saldoCuenta ( 5305 ) + saldoCuenta ( 5315 ) + saldoCuenta ( 5325 ) + saldoCuenta ( 5335 ) + saldoCuenta ( 5345 ) + saldoCuenta ( 5355 ) + saldoCuenta ( 5395 ) + saldoCuenta ( 540 ) + saldoCuenta ( 541 ) + saldoCuenta ( 542 ) + saldoCuenta ( 543 ) + saldoCuenta ( 545 ) + saldoCuenta ( 546 ) + saldoCuenta ( 547 ) + saldoCuenta ( 548 ) + saldoCuenta ( 549 ) + saldoCuenta ( 551 ) + saldoCuenta ( 5525 ) + saldoCuenta ( 5590 ) + saldoCuenta ( 5593 ) + saldoCuenta ( 565 ) + saldoCuenta ( 566 ) + saldoCuenta ( 5945 ) + saldoCuenta ( 5955 ) + saldoCuenta ( 597 ) + saldoCuenta ( 598 ) ).toQString ( '.' ) + "')\n\
doc.set_cell_value(5,24, 'float', '" + ( saldoCuentaAnt ( 5305 ) + saldoCuentaAnt ( 5315 ) + saldoCuentaAnt ( 5325 ) + saldoCuentaAnt ( 5335 ) + saldoCuentaAnt ( 5345 ) + saldoCuentaAnt ( 5355 ) + saldoCuentaAnt ( 5395 ) + saldoCuentaAnt ( 540 ) + saldoCuentaAnt ( 541 ) + saldoCuentaAnt ( 542 ) + saldoCuentaAnt ( 543 ) + saldoCuentaAnt ( 545 ) + saldoCuentaAnt ( 546 ) + saldoCuentaAnt ( 547 ) + saldoCuentaAnt ( 548 ) + saldoCuentaAnt ( 549 ) + saldoCuentaAnt ( 551 ) + saldoCuentaAnt ( 5525 ) + saldoCuentaAnt ( 5590 ) + saldoCuentaAnt ( 5593 ) + saldoCuentaAnt ( 565 ) + saldoCuentaAnt ( 566 ) + saldoCuentaAnt ( 5945 ) + saldoCuentaAnt ( 5955 ) + saldoCuentaAnt ( 597 ) + saldoCuentaAnt ( 598 ) ).toQString ( '.' ) + "')\n\
doc.set_cell_property('bold', False)\n\
\
doc.set_cell_property('bold', True)\n\
doc.set_cell_value(2,25, 'string', 'VI. Periodificaciones a corto plazo.')\n\
doc.set_cell_value(4,25, 'float', '" + ( saldoCuenta ( 480 ) + saldoCuenta ( 567 ) ).toQString ( '.' ) + "')\n\
doc.set_cell_value(5,25, 'float', '" + ( saldoCuentaAnt ( 480 ) + saldoCuentaAnt ( 567 ) ).toQString ( '.' ) + "')\n\
doc.set_cell_property('bold', False)\n\
\
doc.set_cell_property('bold', True)\n\
doc.set_cell_value(2,26, 'string', 'VII. Efectivo y otros activos liquidos equivalentes.')\n\
doc.set_cell_value(4,26, 'float', '" + ( saldoCuenta ( 57 ) ).toQString ( '.' ) + "')\n\
doc.set_cell_value(5,26, 'float', '" + ( saldoCuentaAnt ( 57 ) ).toQString ( '.' ) + "')\n\
doc.set_cell_property('bold', False)\n\
\
\
\
doc.set_cell_property('bold', True)\n\
doc.set_cell_value(2,28, 'string', 'TOTAL ACTIVO  (A + B)')\n\
doc.set_cell_value(4,28, 'formula', '=+D6+D15')\n\
doc.set_cell_value(5,28, 'formula', '=+E6+E15')\n\
doc.set_cell_property('bold', False)\n\
\
\
\
\
doc.new_sheet(\"PATRIMONIO NETO Y PASIVO\")\n\n\
doc.set_column_property(1, 'width', '0.5in')\n\
doc.set_column_property(2, 'width', '5in')\n\
\
doc.set_cell_property('bold', True)\n\
doc.set_cell_value(2,2, 'string', 'BALANCE ABREVIADO AL CIERRE DEL EJERCICIO')\n\
doc.set_cell_value(2,4, 'string', 'PATRIMONIO NETO Y PASIVO')\n\
doc.set_cell_value(3,4, 'string', 'NOTAS')\n\
doc.set_cell_value(4,4, 'string', 'Ejercicio N')\n\
doc.set_cell_value(5,4, 'string', 'Ejercicio N-1')\n\
doc.set_cell_property('bold', False)\n\
\
\
\
doc.set_cell_property('bold', True)\n\
doc.set_cell_value(2,6, 'string', 'A) PATRIMONIO NETO')\n\
doc.set_cell_value(4,6, 'formula', '=D8+D20+D21')\n\
doc.set_cell_value(5,6, 'formula', '=E8+E20+E21')\n\
doc.set_cell_property('bold', False)\n\
\
\
doc.set_cell_property('bold', True)\n\
doc.set_cell_value(2,8, 'string', 'A-1) Fondos propios.')\n\
doc.set_cell_value(4,8, 'formula', '=D9+D12+D13+D14+D15+D16+D17+D18+D19')\n\
doc.set_cell_value(5,8, 'formula', '=E9+E12+E13+E14+E15+E16+E17+E18+E19')\n\
doc.set_cell_property('bold', False)\n\
\
doc.set_cell_property('bold', True)\n\
doc.set_cell_value(2,9, 'string', 'I. Capital.')\n\
doc.set_cell_value(4,9, 'formula', '=D10+D11')\n\
doc.set_cell_value(5,9, 'formula', '=E10+E11')\n\
doc.set_cell_property('bold', False)\n\
\
doc.set_cell_value(2,10, 'string', '1. Capital escriturado.')\n\
doc.set_cell_value(4,10, 'float', '" + ( saldoCuenta ( 100 ) + saldoCuenta ( 101 ) + saldoCuenta ( 102 ) ).toQString ( '.' ) + "')\n\
doc.set_cell_value(5,10, 'float', '" + ( saldoCuentaAnt ( 100 ) + saldoCuentaAnt ( 101 ) + saldoCuentaAnt ( 102 ) ).toQString ( '.' ) + "')\n\
\
doc.set_cell_value(2,11, 'string', '2. (Capital no exigido).')\n\
doc.set_cell_value(4,11, 'float', '" + ( saldoCuenta ( 1030 ) + saldoCuenta ( 1040 ) ).toQString ( '.' ) + "')\n\
doc.set_cell_value(5,11, 'float', '" + ( saldoCuentaAnt ( 1030 ) + saldoCuentaAnt ( 1040 ) ).toQString ( '.' ) + "')\n\
\
doc.set_cell_property('bold', True)\n\
doc.set_cell_value(2,12, 'string', 'II. Prima de emision.')\n\
doc.set_cell_value(4,12, 'float', '" + ( saldoCuenta ( 110 ) ).toQString ( '.' ) + "')\n\
doc.set_cell_value(5,12, 'float', '" + ( saldoCuentaAnt ( 110 ) ).toQString ( '.' ) + "')\n\
doc.set_cell_property('bold', False)\n\
\
doc.set_cell_property('bold', True)\n\
doc.set_cell_value(2,13, 'string', 'III. Reservas.')\n\
doc.set_cell_value(4,13, 'float', '" + ( saldoCuenta ( 112 ) + saldoCuenta ( 113 ) + saldoCuenta ( 114 ) + saldoCuenta ( 115 ) + saldoCuenta ( 119 ) ).toQString ( '.' ) + "')\n\
doc.set_cell_value(5,13, 'float', '" + ( saldoCuentaAnt ( 112 ) + saldoCuentaAnt ( 113 ) + saldoCuentaAnt ( 114 ) + saldoCuentaAnt ( 115 ) + saldoCuentaAnt ( 119 ) ).toQString ( '.' ) + "')\n\
doc.set_cell_property('bold', False)\n\
\
doc.set_cell_property('bold', True)\n\
doc.set_cell_value(2,14, 'string', 'IV. (Acciones y participaciones en patrimonio propias).')\n\
doc.set_cell_value(4,14, 'float', '" + ( saldoCuenta ( 108 ) + saldoCuenta ( 109 ) ).toQString ( '.' ) + "')\n\
doc.set_cell_value(5,14, 'float', '" + ( saldoCuentaAnt ( 108 ) + saldoCuentaAnt ( 109 ) ).toQString ( '.' ) + "')\n\
doc.set_cell_property('bold', False)\n\
\
doc.set_cell_property('bold', True)\n\
doc.set_cell_value(2,15, 'string', 'V. Resultados de ejercicios anteriores.')\n\
doc.set_cell_value(4,15, 'float', '" + ( saldoCuenta ( 120 ) + saldoCuenta ( 121 ) ).toQString ( '.' ) + "')\n\
doc.set_cell_value(5,15, 'float', '" + ( saldoCuentaAnt ( 120 ) + saldoCuentaAnt ( 121 ) ).toQString ( '.' ) + "')\n\
doc.set_cell_property('bold', False)\n\
\
doc.set_cell_property('bold', True)\n\
doc.set_cell_value(2,16, 'string', 'VI. Otras aportaciones de socios.')\n\
doc.set_cell_value(4,16, 'float', '" + ( saldoCuenta ( 118 ) ).toQString ( '.' ) + "')\n\
doc.set_cell_value(5,16, 'float', '" + ( saldoCuentaAnt ( 118 ) ).toQString ( '.' ) + "')\n\
doc.set_cell_property('bold', False)\n\
\
doc.set_cell_property('bold', True)\n\
doc.set_cell_value(2,17, 'string', 'VII. Resultado del ejercicio.')\n\
doc.set_cell_value(4,17, 'float', '" + ( saldoCuenta ( 129 ) ).toQString ( '.' ) + "')\n\
doc.set_cell_value(5,17, 'float', '" + ( saldoCuentaAnt ( 129 ) ).toQString ( '.' ) + "')\n\
doc.set_cell_property('bold', False)\n\
\
doc.set_cell_property('bold', True)\n\
doc.set_cell_value(2,18, 'string', 'VIII. (Dividendo a cuenta).')\n\
doc.set_cell_value(4,18, 'float', '" + ( saldoCuenta ( 557 ) ).toQString ( '.' ) + "')\n\
doc.set_cell_value(5,18, 'float', '" + ( saldoCuentaAnt ( 557 ) ).toQString ( '.' ) + "')\n\
doc.set_cell_property('bold', False)\n\
\
doc.set_cell_property('bold', True)\n\
doc.set_cell_value(2,19, 'string', 'IX. Otros instrumentos de patrimonio neto.')\n\
doc.set_cell_value(4,19, 'float', '" + ( saldoCuenta ( 111 ) ).toQString ( '.' ) + "')\n\
doc.set_cell_value(5,19, 'float', '" + ( saldoCuentaAnt ( 111 ) ).toQString ( '.' ) + "')\n\
doc.set_cell_property('bold', False)\n\
\
doc.set_cell_property('bold', True)\n\
doc.set_cell_value(2,20, 'string', 'A-2) Ajustes por cambios de valor.')\n\
doc.set_cell_value(4,20, 'float', '" + ( saldoCuenta ( 133 ) + saldoCuenta ( 1340 ) + saldoCuenta ( 137 ) ).toQString ( '.' ) + "')\n\
doc.set_cell_value(5,20, 'float', '" + ( saldoCuentaAnt ( 133 ) + saldoCuentaAnt ( 1340 ) + saldoCuentaAnt ( 137 ) ).toQString ( '.' ) + "')\n\
doc.set_cell_property('bold', False)\n\
\
doc.set_cell_property('bold', True)\n\
doc.set_cell_value(2,21, 'string', 'A-3) Subvenciones, donaciones y legados recibidos.')\n\
doc.set_cell_value(4,21, 'float', '" + ( saldoCuenta ( 130 ) + saldoCuenta ( 131 ) + saldoCuenta ( 132 ) ).toQString ( '.' ) + "')\n\
doc.set_cell_value(5,21, 'float', '" + ( saldoCuentaAnt ( 130 ) + saldoCuentaAnt ( 131 ) + saldoCuentaAnt ( 132 ) ).toQString ( '.' ) + "')\n\
doc.set_cell_property('bold', False)\n\
\
doc.set_cell_property('bold', True)\n\
doc.set_cell_value(2,23, 'string', 'B) PASIVO NO CORRIENTE')\n\
doc.set_cell_value(4,23, 'formula', '=D25+D26+D30+D31+D32')\n\
doc.set_cell_value(5,23, 'formula', '=E25+E26+E30+E31+E32')\n\
doc.set_cell_property('bold', False)\n\
\
\
doc.set_cell_property('bold', True)\n\
doc.set_cell_value(2,25, 'string', 'I. Provisiones a largo plazo.')\n\
doc.set_cell_value(4,25, 'float', '" + ( saldoCuenta ( 14 ) ).toQString ( '.' ) + "')\n\
doc.set_cell_value(5,25, 'float', '" + ( saldoCuentaAnt ( 14 ) ).toQString ( '.' ) + "')\n\
doc.set_cell_property('bold', False)\n\
\
doc.set_cell_property('bold', True)\n\
doc.set_cell_value(2,26, 'string', 'II. Deudas a largo plazo.')\n\
doc.set_cell_value(4,26, 'formula', '=D27+D28+D29')\n\
doc.set_cell_value(5,26, 'formula', '=E27+E28+E29')\n\
doc.set_cell_property('bold', False)\n\
\
doc.set_cell_value(2,27, 'string', '1. Deudas con entidades de credito.')\n\
doc.set_cell_value(4,27, 'float', '" + ( saldoCuenta ( 1605 ) + saldoCuenta ( 170 ) ).toQString ( '.' ) + "')\n\
doc.set_cell_value(5,27, 'float', '" + ( saldoCuentaAnt ( 1605 ) + saldoCuentaAnt ( 170 ) ).toQString ( '.' ) + "')\n\
\
doc.set_cell_value(2,28, 'string', '2. Acreedores por arrendamiento financiero.')\n\
doc.set_cell_value(4,28, 'float', '" + ( saldoCuenta ( 1625 ) + saldoCuenta ( 174 ) ).toQString ( '.' ) + "')\n\
doc.set_cell_value(5,28, 'float', '" + ( saldoCuentaAnt ( 1625 ) + saldoCuentaAnt ( 174 ) ).toQString ( '.' ) + "')\n\
\
doc.set_cell_value(2,29, 'string', '3. Otras deudas a largo plazo.')\n\
doc.set_cell_value(4,29, 'float', '" + ( saldoCuenta ( 1615 ) + saldoCuenta ( 1635 ) + saldoCuenta ( 171 ) + saldoCuenta ( 172 ) + saldoCuenta ( 173 ) + saldoCuenta ( 175 ) + saldoCuenta ( 176 ) + saldoCuenta ( 177 ) + saldoCuenta ( 178 ) + saldoCuenta ( 179 ) + saldoCuenta ( 180 ) + saldoCuenta ( 185 ) + saldoCuenta ( 189 ) ).toQString ( '.' ) + "')\n\
doc.set_cell_value(5,29, 'float', '" + ( saldoCuentaAnt ( 1615 ) + saldoCuentaAnt ( 1635 ) + saldoCuentaAnt ( 171 ) + saldoCuentaAnt ( 172 ) + saldoCuentaAnt ( 173 ) + saldoCuentaAnt ( 175 ) + saldoCuentaAnt ( 176 ) + saldoCuentaAnt ( 177 ) + saldoCuentaAnt ( 178 ) + saldoCuentaAnt ( 179 ) + saldoCuentaAnt ( 180 ) + saldoCuentaAnt ( 185 ) + saldoCuentaAnt ( 189 ) ).toQString ( '.' ) + "')\n\
\
doc.set_cell_property('bold', True)\n\
doc.set_cell_value(2,30, 'string', 'III. Deudas con empresas del grupo y asociadas a largo plazo.')\n\
doc.set_cell_value(4,30, 'float', '" + ( saldoCuenta ( 1603 ) + saldoCuenta ( 1604 ) + saldoCuenta ( 1613 ) + saldoCuenta ( 1614 ) + saldoCuenta ( 1623 ) + saldoCuenta ( 1624 ) + saldoCuenta ( 1633 ) + saldoCuenta ( 1634 ) ).toQString ( '.' ) + "')\n\
doc.set_cell_value(5,30, 'float', '" + ( saldoCuentaAnt ( 1603 ) + saldoCuentaAnt ( 1604 ) + saldoCuentaAnt ( 1613 ) + saldoCuentaAnt ( 1614 ) + saldoCuentaAnt ( 1623 ) + saldoCuentaAnt ( 1624 ) + saldoCuentaAnt ( 1633 ) + saldoCuentaAnt ( 1634 ) ).toQString ( '.' ) + "')\n\
doc.set_cell_property('bold', False)\n\
\
doc.set_cell_property('bold', True)\n\
doc.set_cell_value(2,31, 'string', 'IV. Pasivos por impuesto diferido.')\n\
doc.set_cell_value(4,31, 'float', '" + ( saldoCuenta ( 479 ) ).toQString ( '.' ) + "')\n\
doc.set_cell_value(5,31, 'float', '" + ( saldoCuentaAnt ( 479 ) ).toQString ( '.' ) + "')\n\
doc.set_cell_property('bold', False)\n\
\
doc.set_cell_property('bold', True)\n\
doc.set_cell_value(2,32, 'string', 'V. Periodificaciones a largo plazo.')\n\
doc.set_cell_value(4,32, 'float', '" + ( saldoCuenta ( 181 ) ).toQString ( '.' ) + "')\n\
doc.set_cell_value(5,32, 'float', '" + ( saldoCuentaAnt ( 181 ) ).toQString ( '.' ) + "')\n\
doc.set_cell_property('bold', False)\n\
\
doc.set_cell_property('bold', True)\n\
doc.set_cell_value(2,34, 'string', 'C) PASIVO CORRIENTE')\n\
doc.set_cell_value(4,34, 'formula', '=D36+D37+D41+D42+D45+D46')\n\
doc.set_cell_value(5,34, 'formula', '=E36+E37+E41+E42+E45+E46')\n\
doc.set_cell_property('bold', False)\n\
\
doc.set_cell_property('bold', True)\n\
doc.set_cell_value(2,36, 'string', 'I. Pasivos vinculados con activos no corrientes mantenidos para la venta.')\n\
doc.set_cell_value(4,36, 'float', '" + ( saldoCuenta ( 585 ) + saldoCuenta ( 586 ) + saldoCuenta ( 587 ) + saldoCuenta ( 588 ) + saldoCuenta ( 589 ) ).toQString ( '.' ) + "')\n\
doc.set_cell_value(5,36, 'float', '" + ( saldoCuentaAnt ( 585 ) + saldoCuentaAnt ( 586 ) + saldoCuentaAnt ( 587 ) + saldoCuentaAnt ( 588 ) + saldoCuentaAnt ( 589 ) ).toQString ( '.' ) + "')\n\
doc.set_cell_property('bold', False)\n\
\
doc.set_cell_property('bold', True)\n\
doc.set_cell_value(2,37, 'string', 'II. Provisiones a corto plazo.')\n\
doc.set_cell_value(4,37, 'float', '" + ( saldoCuenta ( 499 ) + saldoCuenta ( 529 ) ).toQString ( '.' ) + "')\n\
doc.set_cell_value(5,37, 'float', '" + ( saldoCuentaAnt ( 499 ) + saldoCuentaAnt ( 529 ) ).toQString ( '.' ) + "')\n\
doc.set_cell_property('bold', False)\n\
\
doc.set_cell_property('bold', True)\n\
doc.set_cell_value(2,38, 'string', 'III. Deudas a corto plazo.')\n\
doc.set_cell_value(4,38, 'formula', '=D39+D40+D41')\n\
doc.set_cell_value(5,38, 'formula', '=E39+E40+E41')\n\
doc.set_cell_property('bold', False)\n\
\
doc.set_cell_value(2,39, 'string', '1. Deudas con entidades de credito.')\n\
doc.set_cell_value(4,39, 'float', '" + ( saldoCuenta ( 5105 ) + saldoCuenta ( 520 ) + saldoCuenta ( 527 ) ).toQString ( '.' ) + "')\n\
doc.set_cell_value(5,39, 'float', '" + ( saldoCuentaAnt ( 5105 ) + saldoCuentaAnt ( 520 ) + saldoCuentaAnt ( 527 ) ).toQString ( '.' ) + "')\n\
\
doc.set_cell_value(2,40, 'string', '2. Acreedores por arrendamiento financiero.')\n\
doc.set_cell_value(4,40, 'float', '" + ( saldoCuenta ( 5125 ) + saldoCuenta ( 524 ) ).toQString ( '.' ) + "')\n\
doc.set_cell_value(5,40, 'float', '" + ( saldoCuentaAnt ( 5125 ) + saldoCuentaAnt ( 524 ) ).toQString ( '.' ) + "')\n\
\
doc.set_cell_value(2,41, 'string', '3. Otras deudas a corto plazo.')\n\
doc.set_cell_value(4,41, 'float', '" + ( saldoCuenta ( 1034 ) + saldoCuenta ( 1044 ) + saldoCuenta ( 190 ) + saldoCuenta ( 192 ) + saldoCuenta ( 194 ) + saldoCuenta ( 500 ) + saldoCuenta ( 501 ) + saldoCuenta ( 505 ) + saldoCuenta ( 506 ) + saldoCuenta ( 509 ) + saldoCuenta ( 5115 ) + saldoCuenta ( 5135 ) + saldoCuenta ( 5145 ) + saldoCuenta ( 521 ) + saldoCuenta ( 522 ) + saldoCuenta ( 523 ) + saldoCuenta ( 525 ) + saldoCuenta ( 526 ) + saldoCuenta ( 528 ) + saldoCuenta ( 551 ) + saldoCuenta ( 5525 ) + saldoCuenta ( 5530 ) + saldoCuenta ( 5532 ) + saldoCuenta ( 555 ) + saldoCuenta ( 5565 ) + saldoCuenta ( 5566 ) + saldoCuenta ( 5595 ) + saldoCuenta ( 5598 ) + saldoCuenta ( 560 ) + saldoCuenta ( 561 ) + saldoCuenta ( 569 ) ).toQString ( '.' ) + "')\n\
doc.set_cell_value(5,41, 'float', '" + ( saldoCuentaAnt ( 1034 ) + saldoCuentaAnt ( 1044 ) + saldoCuentaAnt ( 190 ) + saldoCuentaAnt ( 192 ) + saldoCuentaAnt ( 194 ) + saldoCuentaAnt ( 500 ) + saldoCuentaAnt ( 501 ) + saldoCuentaAnt ( 505 ) + saldoCuentaAnt ( 506 ) + saldoCuentaAnt ( 509 ) + saldoCuentaAnt ( 5115 ) + saldoCuentaAnt ( 5135 ) + saldoCuentaAnt ( 5145 ) + saldoCuentaAnt ( 521 ) + saldoCuentaAnt ( 522 ) + saldoCuentaAnt ( 523 ) + saldoCuentaAnt ( 525 ) + saldoCuentaAnt ( 526 ) + saldoCuentaAnt ( 528 ) + saldoCuentaAnt ( 551 ) + saldoCuentaAnt ( 5525 ) + saldoCuentaAnt ( 5530 ) + saldoCuentaAnt ( 5532 ) + saldoCuentaAnt ( 555 ) + saldoCuentaAnt ( 5565 ) + saldoCuentaAnt ( 5566 ) + saldoCuentaAnt ( 5595 ) + saldoCuentaAnt ( 5598 ) + saldoCuentaAnt ( 560 ) + saldoCuentaAnt ( 561 ) + saldoCuentaAnt ( 569 ) ).toQString ( '.' ) + "')\n\
\
doc.set_cell_property('bold', True)\n\
doc.set_cell_value(2,42, 'string', 'IV. Deudas con empresas del grupo y asociadas a corto plazo.')\n\
doc.set_cell_value(4,42, 'float', '" + ( saldoCuenta ( 5103 ) + saldoCuenta ( 5104 ) + saldoCuenta ( 5113 ) + saldoCuenta ( 5114 ) + saldoCuenta ( 5123 ) + saldoCuenta ( 5124 ) + saldoCuenta ( 5133 ) + saldoCuenta ( 5134 ) + saldoCuenta ( 5143 ) + saldoCuenta ( 5144 ) + saldoCuenta ( 5523 ) + saldoCuenta ( 5524 ) + saldoCuenta ( 5563 ) + saldoCuenta ( 5564 ) ).toQString ( '.' ) + "')\n\
doc.set_cell_value(5,42, 'float', '" + ( saldoCuentaAnt ( 5103 ) + saldoCuentaAnt ( 5104 ) + saldoCuentaAnt ( 5113 ) + saldoCuentaAnt ( 5114 ) + saldoCuentaAnt ( 5123 ) + saldoCuentaAnt ( 5124 ) + saldoCuentaAnt ( 5133 ) + saldoCuentaAnt ( 5134 ) + saldoCuentaAnt ( 5143 ) + saldoCuentaAnt ( 5144 ) + saldoCuentaAnt ( 5523 ) + saldoCuentaAnt ( 5524 ) + saldoCuentaAnt ( 5563 ) + saldoCuentaAnt ( 5564 ) ).toQString ( '.' ) + "')\n\
doc.set_cell_property('bold', False)\n\
\
doc.set_cell_property('bold', True)\n\
doc.set_cell_value(2,43, 'string', 'V. Acreedores comerciales y otras cuentas a pagar.')\n\
doc.set_cell_value(4,43, 'formula', '=D44+D45')\n\
doc.set_cell_value(5,43, 'formula', '=E44+E45')\n\
doc.set_cell_property('bold', False)\n\
\
doc.set_cell_value(2,44, 'string', '1. Proveedores.')\n\
doc.set_cell_value(4,44, 'float', '" + ( saldoCuenta ( 400 ) + saldoCuenta ( 401 ) + saldoCuenta ( 403 ) + saldoCuenta ( 404 ) + saldoCuenta ( 405 ) + saldoCuenta ( 406 ) ).toQString ( '.' ) + "')\n\
doc.set_cell_value(5,44, 'float', '" + ( saldoCuentaAnt ( 400 ) + saldoCuentaAnt ( 401 ) + saldoCuentaAnt ( 403 ) + saldoCuentaAnt ( 404 ) + saldoCuentaAnt ( 405 ) + saldoCuentaAnt ( 406 ) ).toQString ( '.' ) + "')\n\
\
doc.set_cell_value(2,45, 'string', '2. Otros acreedores.')\n\
doc.set_cell_value(4,45, 'float', '" + ( saldoCuenta ( 41 ) + saldoCuenta ( 438 ) + saldoCuenta ( 465 ) + saldoCuenta ( 466 ) + saldoCuenta ( 475 ) + saldoCuenta ( 476 ) + saldoCuenta ( 477 ) ).toQString ( '.' ) + "')\n\
doc.set_cell_value(5,45, 'float', '" + ( saldoCuentaAnt ( 41 ) + saldoCuentaAnt ( 438 ) + saldoCuentaAnt ( 465 ) + saldoCuentaAnt ( 466 ) + saldoCuentaAnt ( 475 ) + saldoCuentaAnt ( 476 ) + saldoCuentaAnt ( 477 ) ).toQString ( '.' ) + "')\n\
\
doc.set_cell_property('bold', True)\n\
doc.set_cell_value(2,46, 'string', 'VI. Periodificaciones a corto plazo.')\n\
doc.set_cell_value(4,46, 'float', '" + ( saldoCuenta ( 485 ) + saldoCuenta ( 568 ) ).toQString ( '.' ) + "')\n\
doc.set_cell_value(5,46, 'float', '" + ( saldoCuentaAnt ( 485 ) + saldoCuentaAnt ( 568 ) ).toQString ( '.' ) + "')\n\
doc.set_cell_property('bold', False)\n\
\
\
doc.set_cell_property('bold', True)\n\
doc.set_cell_value(2,48, 'string', 'TOTAL PATRIMONIO NETO Y PASIVO (A + B + C)')\n\
doc.set_cell_value(4,48, 'formula', '=D6+D23+D34')\n\
doc.set_cell_value(5,48, 'formula', '=E6+E23+E34')\n\
doc.set_cell_property('bold', False)\n\
\
\
\
\
\
\
\
doc.new_sheet(\"PERDIDAS_Y_GANANCIAS\")\n\n\
doc.set_column_property(1, 'width', '0.5in')\n\
doc.set_column_property(2, 'width', '5in')\n\
\
doc.set_cell_property('bold', True)\n\
doc.set_cell_value(2,2, 'string', 'CUENTA DE PÉRDIDAS Y GANANCIAS ABREVIADA CORRESPONDIENTE AL EJERCICIO TERMINADO')\n\
doc.set_cell_value(2,4, 'string', 'PERDIDAS Y GANANCIAS')\n\
doc.set_cell_value(3,4, 'string', 'NOTAS')\n\
doc.set_cell_value(4,4, 'string', 'Ejercicio N')\n\
doc.set_cell_value(5,4, 'string', 'Ejercicio N-1')\n\
doc.set_cell_property('bold', False)\n\
\
\
\
doc.set_cell_property('bold', True)\n\
doc.set_cell_value(2,6, 'string', '1. Importe neto de la cifra de negocios.')\n\
doc.set_cell_value(4,6, 'float', '" + ( saldoCuenta ( 700 ) + saldoCuenta ( 701 ) + saldoCuenta ( 702 ) + saldoCuenta ( 703 ) + saldoCuenta ( 704 ) + saldoCuenta ( 705 ) + saldoCuenta ( 706 ) + saldoCuenta ( 708 ) + saldoCuenta ( 709 ) ).toQString ( '.' ) + "')\n\
doc.set_cell_value(5,6, 'float', '" + ( saldoCuentaAnt ( 700 ) + saldoCuentaAnt ( 701 ) + saldoCuentaAnt ( 702 ) + saldoCuentaAnt ( 703 ) + saldoCuentaAnt ( 704 ) + saldoCuentaAnt ( 705 ) + saldoCuentaAnt ( 706 ) + saldoCuentaAnt ( 708 ) + saldoCuentaAnt ( 709 ) ).toQString ( '.' ) + "')\n\
doc.set_cell_property('bold', False)\n\
\
doc.set_cell_property('bold', True)\n\
doc.set_cell_value(2,7, 'string', '2. Variacion de existencias de productos terminados y en curso de fabricacion.')\n\
doc.set_cell_value(4,7, 'float', '" + ( saldoCuenta ( 6930 ) + saldoCuenta ( 71 ) + saldoCuenta ( 7930 ) ).toQString ( '.' ) + "')\n\
doc.set_cell_value(5,7, 'float', '" + ( saldoCuentaAnt ( 6930 ) + saldoCuentaAnt ( 71 ) + saldoCuentaAnt ( 7930 ) ).toQString ( '.' ) + "')\n\
doc.set_cell_property('bold', False)\n\
\
doc.set_cell_property('bold', True)\n\
doc.set_cell_value(2,8, 'string', '3. Trabajos realizados por la empresa para su activo.')\n\
doc.set_cell_value(4,8, 'float', '" + ( saldoCuenta ( 73 ) ).toQString ( '.' ) + "')\n\
doc.set_cell_value(5,8, 'float', '" + ( saldoCuentaAnt ( 73 ) ).toQString ( '.' ) + "')\n\
doc.set_cell_property('bold', False)\n\
\
doc.set_cell_property('bold', True)\n\
doc.set_cell_value(2,9, 'string', '4. Aprovisionamientos.')\n\
doc.set_cell_value(4,9, 'float', '" + ( saldoCuenta ( 600 ) + saldoCuenta ( 601 ) + saldoCuenta ( 602 ) + saldoCuenta ( 606 ) + saldoCuenta ( 607 ) + saldoCuenta ( 608 ) + saldoCuenta ( 609 ) + saldoCuenta ( 61 ) + saldoCuenta ( 6931 ) + saldoCuenta ( 6932 ) + saldoCuenta ( 6933 ) + saldoCuenta ( 7931 ) + saldoCuenta ( 7932 ) + saldoCuenta ( 7933 ) ).toQString ( '.' ) + "')\n\
doc.set_cell_value(5,9, 'float', '" + ( saldoCuentaAnt ( 600 ) + saldoCuentaAnt ( 601 ) + saldoCuentaAnt ( 602 ) + saldoCuentaAnt ( 606 ) + saldoCuentaAnt ( 607 ) + saldoCuentaAnt ( 608 ) + saldoCuentaAnt ( 609 ) + saldoCuentaAnt ( 61 ) + saldoCuentaAnt ( 6931 ) + saldoCuentaAnt ( 6932 ) + saldoCuentaAnt ( 6933 ) + saldoCuentaAnt ( 7931 ) + saldoCuentaAnt ( 7932 ) + saldoCuentaAnt ( 7933 ) ).toQString ( '.' ) + "')\n\
doc.set_cell_property('bold', False)\n\
\
doc.set_cell_property('bold', True)\n\
doc.set_cell_value(2,10, 'string', '5. Otros ingresos de explotacion.')\n\
doc.set_cell_value(4,10, 'float', '" + ( saldoCuenta ( 740 ) + saldoCuenta ( 747 ) + saldoCuenta ( 75 ) ).toQString ( '.' ) + "')\n\
doc.set_cell_value(5,10, 'float', '" + ( saldoCuentaAnt ( 740 ) + saldoCuentaAnt ( 747 ) + saldoCuentaAnt ( 75 ) ).toQString ( '.' ) + "')\n\
doc.set_cell_property('bold', False)\n\
\
doc.set_cell_property('bold', True)\n\
doc.set_cell_value(2,11, 'string', '6. Gastos de personal.')\n\
doc.set_cell_value(4,11, 'float', '" + ( saldoCuenta ( 64 ) + saldoCuenta ( 7950 ) + saldoCuenta ( 7957 ) ).toQString ( '.' ) + "')\n\
doc.set_cell_value(5,11, 'float', '" + ( saldoCuentaAnt ( 64 ) + saldoCuentaAnt ( 7950 ) + saldoCuentaAnt ( 7957 ) ).toQString ( '.' ) + "')\n\
doc.set_cell_property('bold', False)\n\
\
doc.set_cell_property('bold', True)\n\
doc.set_cell_value(2,12, 'string', '7. Otros gastos de explotacion.')\n\
doc.set_cell_value(4,12, 'float', '" + ( saldoCuenta ( 62 ) + saldoCuenta ( 631 ) + saldoCuenta ( 634 ) + saldoCuenta ( 636 ) + saldoCuenta ( 639 ) + saldoCuenta ( 65 ) + saldoCuenta ( 694 ) + saldoCuenta ( 695 ) + saldoCuenta ( 794 ) + saldoCuenta ( 7954 ) ).toQString ( '.' ) + "')\n\
doc.set_cell_value(5,12, 'float', '" + ( saldoCuentaAnt ( 62 ) + saldoCuentaAnt ( 631 ) + saldoCuentaAnt ( 634 ) + saldoCuentaAnt ( 636 ) + saldoCuentaAnt ( 639 ) + saldoCuentaAnt ( 65 ) + saldoCuentaAnt ( 694 ) + saldoCuentaAnt ( 695 ) + saldoCuentaAnt ( 794 ) + saldoCuentaAnt ( 7954 ) ).toQString ( '.' ) + "')\n\
doc.set_cell_property('bold', False)\n\
\
doc.set_cell_property('bold', True)\n\
doc.set_cell_value(2,13, 'string', '8. Amortizacion del inmovilizado.')\n\
doc.set_cell_value(4,13, 'float', '" + ( saldoCuenta ( 68 ) ).toQString ( '.' ) + "')\n\
doc.set_cell_value(5,13, 'float', '" + ( saldoCuentaAnt ( 68 ) ).toQString ( '.' ) + "')\n\
doc.set_cell_property('bold', False)\n\
\
doc.set_cell_property('bold', True)\n\
doc.set_cell_value(2,14, 'string', '9. Imputacion de subvenciones de inmovilizado no financiero y otras.')\n\
doc.set_cell_value(4,14, 'float', '" + ( saldoCuenta ( 746 ) ).toQString ( '.' ) + "')\n\
doc.set_cell_value(5,14, 'float', '" + ( saldoCuentaAnt ( 746 ) ).toQString ( '.' ) + "')\n\
doc.set_cell_property('bold', False)\n\
\
doc.set_cell_property('bold', True)\n\
doc.set_cell_value(2,15, 'string', '10. Excesos de provisiones.')\n\
doc.set_cell_value(4,15, 'float', '" + ( saldoCuenta ( 7951 ) + saldoCuenta ( 7952 ) + saldoCuenta ( 7955 ) + saldoCuenta ( 7956 ) ).toQString ( '.' ) + "')\n\
doc.set_cell_value(5,15, 'float', '" + ( saldoCuentaAnt ( 7951 ) + saldoCuentaAnt ( 7952 ) + saldoCuentaAnt ( 7955 ) + saldoCuentaAnt ( 7956 ) ).toQString ( '.' ) + "')\n\
doc.set_cell_property('bold', False)\n\
\
doc.set_cell_property('bold', True)\n\
doc.set_cell_value(2,16, 'string', '11. Deterioro y resultado por enajenaciones del inmovilizado.')\n\
doc.set_cell_value(4,16, 'float', '" + ( saldoCuenta ( 670 ) + saldoCuenta ( 671 ) + saldoCuenta ( 672 ) + saldoCuenta ( 690 ) + saldoCuenta ( 691 ) + saldoCuenta ( 692 ) + saldoCuenta ( 770 ) + saldoCuenta ( 771 ) + saldoCuenta ( 772 ) + saldoCuenta ( 790 ) + saldoCuenta ( 791 ) + saldoCuenta ( 792 ) ).toQString ( '.' ) + "')\n\
doc.set_cell_value(5,16, 'float', '" + ( saldoCuentaAnt ( 670 ) + saldoCuentaAnt ( 671 ) + saldoCuentaAnt ( 672 ) + saldoCuentaAnt ( 690 ) + saldoCuentaAnt ( 691 ) + saldoCuentaAnt ( 692 ) + saldoCuentaAnt ( 770 ) + saldoCuentaAnt ( 771 ) + saldoCuentaAnt ( 772 ) + saldoCuentaAnt ( 790 ) + saldoCuentaAnt ( 791 ) + saldoCuentaAnt ( 792 ) ).toQString ( '.' ) + "')\n\
doc.set_cell_property('bold', False)\n\
\
\
doc.set_cell_property('bold', True)\n\
doc.set_cell_value(2,18, 'string', 'A) RESULTADO DE EXPLOTACIÓN')\n\
doc.set_cell_value(4,18, 'formula', '=D6+D7+D8+D9+D10+D11+D12+D13+D14+D15+D16')\n\
doc.set_cell_value(5,18, 'formula', '=E6+E7+E8+E9+E10+E11+E12+E13+E14+E15+E16')\n\
doc.set_cell_property('bold', False)\n\
\
doc.set_cell_property('bold', True)\n\
doc.set_cell_value(2,19, 'string', '(1+2+3+4+5+6+7+8+9+10+11)')\n\
\
\
doc.set_cell_property('bold', True)\n\
doc.set_cell_value(2,21, 'string', '12. Ingresos financieros.')\n\
doc.set_cell_value(4,21, 'float', '" + ( saldoCuenta ( 760 ) + saldoCuenta ( 761 ) + saldoCuenta ( 762 ) + saldoCuenta ( 767 ) + saldoCuenta ( 769 ) ).toQString ( '.' ) + "')\n\
doc.set_cell_value(5,21, 'float', '" + ( saldoCuentaAnt ( 760 ) + saldoCuentaAnt ( 761 ) + saldoCuentaAnt ( 762 ) + saldoCuentaAnt ( 767 ) + saldoCuentaAnt ( 769 ) ).toQString ( '.' ) + "')\n\
doc.set_cell_property('bold', False)\n\
\
doc.set_cell_property('bold', True)\n\
doc.set_cell_value(2,22, 'string', '13. Gastos financieros.')\n\
doc.set_cell_value(4,22, 'float', '" + ( saldoCuenta ( 660 ) + saldoCuenta ( 661 ) + saldoCuenta ( 662 ) + saldoCuenta ( 664 ) + saldoCuenta ( 665 ) + saldoCuenta ( 669 ) ).toQString ( '.' ) + "')\n\
doc.set_cell_value(5,22, 'float', '" + ( saldoCuentaAnt ( 660 ) + saldoCuentaAnt ( 661 ) + saldoCuentaAnt ( 662 ) + saldoCuentaAnt ( 664 ) + saldoCuentaAnt ( 665 ) + saldoCuentaAnt ( 669 ) ).toQString ( '.' ) + "')\n\
doc.set_cell_property('bold', False)\n\
\
doc.set_cell_property('bold', True)\n\
doc.set_cell_value(2,23, 'string', '14. Variacion de valor razonable en instrumentos financieros.')\n\
doc.set_cell_value(4,23, 'float', '" + ( saldoCuenta ( 663 ) + saldoCuenta ( 763 ) ).toQString ( '.' ) + "')\n\
doc.set_cell_value(5,23, 'float', '" + ( saldoCuentaAnt ( 663 ) + saldoCuentaAnt ( 763 ) ).toQString ( '.' ) + "')\n\
doc.set_cell_property('bold', False)\n\
\
doc.set_cell_property('bold', True)\n\
doc.set_cell_value(2,24, 'string', '15. Diferencias de cambio.')\n\
doc.set_cell_value(4,24, 'float', '" + ( saldoCuenta ( 668 ) + saldoCuenta ( 768 ) ).toQString ( '.' ) + "')\n\
doc.set_cell_value(5,24, 'float', '" + ( saldoCuentaAnt ( 668 ) + saldoCuentaAnt ( 768 ) ).toQString ( '.' ) + "')\n\
doc.set_cell_property('bold', False)\n\
\
doc.set_cell_property('bold', True)\n\
doc.set_cell_value(2,25, 'string', '16. Deterioro y resultado por enajenaciones de instrumentos financieros.')\n\
doc.set_cell_value(4,25, 'float', '" + ( saldoCuenta ( 666 ) + saldoCuenta ( 667 ) + saldoCuenta ( 673 ) + saldoCuenta ( 675 ) + saldoCuenta ( 696 ) + saldoCuenta ( 697 ) + saldoCuenta ( 698 ) + saldoCuenta ( 699 ) + saldoCuenta ( 766 ) + saldoCuenta ( 773 ) + saldoCuenta ( 775 ) + saldoCuenta ( 796 ) + saldoCuenta ( 797 ) + saldoCuenta ( 798 ) + saldoCuenta ( 799 ) ).toQString ( '.' ) + "')\n\
doc.set_cell_value(5,25, 'float', '" + ( saldoCuentaAnt ( 666 ) + saldoCuentaAnt ( 667 ) + saldoCuentaAnt ( 673 ) + saldoCuentaAnt ( 675 ) + saldoCuentaAnt ( 696 ) + saldoCuentaAnt ( 697 ) + saldoCuentaAnt ( 698 ) + saldoCuentaAnt ( 699 ) + saldoCuentaAnt ( 766 ) + saldoCuentaAnt ( 773 ) + saldoCuentaAnt ( 775 ) + saldoCuentaAnt ( 796 ) + saldoCuentaAnt ( 797 ) + saldoCuentaAnt ( 798 ) + saldoCuentaAnt ( 799 ) ).toQString ( '.' ) + "')\n\
doc.set_cell_property('bold', False)\n\
\
\
doc.set_cell_property('bold', True)\n\
doc.set_cell_value(2,27, 'string', 'B) RESULTADO FINANCIERO (12+13+14+15+16 )')\n\
doc.set_cell_value(4,27, 'formula', '=D21+D22+D23+D24+D25')\n\
doc.set_cell_value(5,27, 'formula', '=E21+E22+E23+E24+E25')\n\
doc.set_cell_property('bold', False)\n\
\
\
doc.set_cell_property('bold', True)\n\
doc.set_cell_value(2,29, 'string', 'C) RESULTADO ANTES DE IMPUESTOS (A+B)')\n\
doc.set_cell_value(4,29, 'formula', '=D18+D27')\n\
doc.set_cell_value(5,29, 'formula', '=E18+E27')\n\
doc.set_cell_property('bold', False)\n\
\
\
doc.set_cell_property('bold', True)\n\
doc.set_cell_value(2,31, 'string', '17. Impuestos sobre beneficios.')\n\
doc.set_cell_value(4,31, 'float', '" + ( saldoCuenta ( 6300 ) + saldoCuenta ( 6301 ) + saldoCuenta ( 633 ) + saldoCuenta ( 638 ) ).toQString ( '.' ) + "')\n\
doc.set_cell_value(5,31, 'float', '" + ( saldoCuentaAnt ( 6300 ) + saldoCuentaAnt ( 6301 ) + saldoCuentaAnt ( 633 ) + saldoCuentaAnt ( 638 ) ).toQString ( '.' ) + "')\n\
doc.set_cell_property('bold', False)\n\
\
\
doc.set_cell_property('bold', True)\n\
doc.set_cell_value(2,33, 'string', 'D) RESULTADO DEL EJERCICIO (C + 17)')\n\
doc.set_cell_value(4,33, 'formula', '=D29+D31')\n\
doc.set_cell_value(5,33, 'formula', '=E29+E31')\n\
doc.set_cell_property('bold', False)\n\
\
\
\
\
\
doc.new_sheet(\"INGRESOS Y GASTOS\")\n\n\
doc.set_column_property(1, 'width', '0.5in')\n\
doc.set_column_property(2, 'width', '5in')\n\
\
doc.set_cell_property('bold', True)\n\
doc.set_cell_value(2,2, 'string', 'ESTADO ABREVIADO DE INGRESOS Y GASTOS RECONOCIDOS CORRESPONDIENTE AL EJERCICIO TERMINADO')\n\
doc.set_cell_value(2,4, 'string', 'INGRESOS Y GASTOS')\n\
doc.set_cell_value(3,4, 'string', 'NOTAS')\n\
doc.set_cell_value(4,4, 'string', 'Ejercicio N')\n\
doc.set_cell_value(5,4, 'string', 'Ejercicio N-1')\n\
doc.set_cell_property('bold', False)\n\
\
\
\
doc.set_cell_property('bold', True)\n\
doc.set_cell_value(2,6, 'string', 'A) Resultado de la cuenta de perdidas y ganancias')\n\
doc.set_cell_value(4,6, 'formula', '=" + _ ( "PERDIDAS_Y_GANANCIAS" ) + ".D33')\n\
doc.set_cell_value(5,6, 'formula', '=PERDIDAS_Y_GANANCIAS.E33')\n\
doc.set_cell_property('bold', False)\n\
\
doc.set_cell_property('bold', True)\n\
doc.set_cell_value(2,7, 'string', 'Ingresos y gastos imputados directamente al patrimonio neto')\n\
doc.set_cell_property('bold', False)\n\
\
\
doc.set_cell_property('bold', True)\n\
doc.set_cell_value(2,9, 'string', '" + _ ( "I. Por valoracion de instrumentos financieros." ) + "')\n\
doc.set_cell_value(4,9, 'float', '" + ( saldoCuenta ( 800 ) + saldoCuenta ( 89 ) + saldoCuenta ( 900 ) + saldoCuenta ( 991 ) + saldoCuenta ( 992 ) ).toQString ( '.' ) + "')\n\
doc.set_cell_value(5,9, 'float', '" + ( saldoCuentaAnt ( 800 ) + saldoCuentaAnt ( 89 ) + saldoCuentaAnt ( 900 ) + saldoCuentaAnt ( 991 ) + saldoCuentaAnt ( 992 ) ).toQString ( '.' ) + "')\n\
doc.set_cell_property('bold', False)\n\
\
doc.set_cell_property('bold', True)\n\
doc.set_cell_value(2,10, 'string', 'II. Por coberturas de flujos de efectivo.')\n\
doc.set_cell_value(4,10, 'float', '" + ( saldoCuenta ( 810 ) + saldoCuenta ( 910 ) ).toQString ( '.' ) + "')\n\
doc.set_cell_value(5,10, 'float', '" + ( saldoCuentaAnt ( 810 ) + saldoCuentaAnt ( 910 ) ).toQString ( '.' ) + "')\n\
doc.set_cell_property('bold', False)\n\
\
doc.set_cell_property('bold', True)\n\
doc.set_cell_value(2,11, 'string', 'III. Subvenciones, donaciones y legados recibidos.')\n\
doc.set_cell_value(4,11, 'float', '" + ( saldoCuenta ( 94 ) ).toQString ( '.' ) + "')\n\
doc.set_cell_value(5,11, 'float', '" + ( saldoCuentaAnt ( 94 ) ).toQString ( '.' ) + "')\n\
doc.set_cell_property('bold', False)\n\
\
doc.set_cell_property('bold', True)\n\
doc.set_cell_value(2,12, 'string', 'IV. Por ganancias y perdidas actuariales y otros ajustes')\n\
doc.set_cell_value(4,12, 'float', '" + ( saldoCuenta ( 85 ) + saldoCuenta ( 95 ) ).toQString ( '.' ) + "')\n\
doc.set_cell_value(5,12, 'float', '" + ( saldoCuentaAnt ( 85 ) + saldoCuentaAnt ( 95 ) ).toQString ( '.' ) + "')\n\
doc.set_cell_property('bold', False)\n\
\
doc.set_cell_property('bold', True)\n\
doc.set_cell_value(2,13, 'string', 'V. Efecto impositivo.')\n\
doc.set_cell_value(4,13, 'float', '" + ( saldoCuenta ( 8300 ) + saldoCuenta ( 8301 ) + saldoCuenta ( 833 ) + saldoCuenta ( 834 ) + saldoCuenta ( 835 ) + saldoCuenta ( 838 ) ).toQString ( '.' ) + "')\n\
doc.set_cell_value(5,13, 'float', '" + ( saldoCuentaAnt ( 8300 ) + saldoCuentaAnt ( 8301 ) + saldoCuentaAnt ( 833 ) + saldoCuentaAnt ( 834 ) + saldoCuentaAnt ( 835 ) + saldoCuentaAnt ( 838 ) ).toQString ( '.' ) + "')\n\
doc.set_cell_property('bold', False)\n\
\
\
doc.set_cell_property('bold', True)\n\
doc.set_cell_value(2,15, 'string', 'B) Total Ingresos y gastos imputados directamente al patrimonio neto ')\n\
doc.set_cell_value(4,15, 'formula', '=D9+D10+D11+D12+D13')\n\
doc.set_cell_value(5,15, 'formula', '=E9+E10+E11+E12+E13')\n\
doc.set_cell_property('bold', False)\n\
\
doc.set_cell_property('bold', True)\n\
doc.set_cell_value(2,16, 'string', '(I+II+III+IV+V)')\n\
doc.set_cell_property('bold', False)\n\
\
doc.set_cell_property('bold', True)\n\
doc.set_cell_value(2,17, 'string', 'Transferencias a la cuenta de perdidas y ganancias')\n\
doc.set_cell_property('bold', False)\n\
\
\
doc.set_cell_property('bold', True)\n\
doc.set_cell_value(2,19, 'string', 'VI. Por valoracion de instrumentos financieros.')\n\
doc.set_cell_value(4,19, 'float', '" + ( saldoCuenta ( 802 ) + saldoCuenta ( 902 ) + saldoCuenta ( 993 ) + saldoCuenta ( 994 ) ).toQString ( '.' ) + "')\n\
doc.set_cell_value(5,19, 'float', '" + ( saldoCuentaAnt ( 802 ) + saldoCuentaAnt ( 902 ) + saldoCuentaAnt ( 993 ) + saldoCuentaAnt ( 994 ) ).toQString ( '.' ) + "')\n\
doc.set_cell_property('bold', False)\n\
\
doc.set_cell_property('bold', True)\n\
doc.set_cell_value(2,20, 'string', 'VII. Por coberturas de flujos de efectivo.')\n\
doc.set_cell_value(4,20, 'float', '" + ( saldoCuenta ( 812 ) + saldoCuenta ( 912 ) ).toQString ( '.' ) + "')\n\
doc.set_cell_value(5,20, 'float', '" + ( saldoCuentaAnt ( 812 ) + saldoCuentaAnt ( 912 ) ).toQString ( '.' ) + "')\n\
doc.set_cell_property('bold', False)\n\
\
doc.set_cell_property('bold', True)\n\
doc.set_cell_value(2,21, 'string', 'VIII. Subvenciones, donaciones y legados recibidos.')\n\
doc.set_cell_value(4,21, 'float', '" + ( saldoCuenta ( 84 ) ).toQString ( '.' ) + "')\n\
doc.set_cell_value(5,21, 'float', '" + ( saldoCuentaAnt ( 84 ) ).toQString ( '.' ) + "')\n\
doc.set_cell_property('bold', False)\n\
\
doc.set_cell_property('bold', True)\n\
doc.set_cell_value(2,22, 'string', 'IX. Efecto impositivo.')\n\
doc.set_cell_value(4,22, 'float', '" + ( saldoCuenta ( 8301 ) + saldoCuenta ( 836 ) + saldoCuenta ( 837 ) ).toQString ( '.' ) + "')\n\
doc.set_cell_value(5,22, 'float', '" + ( saldoCuentaAnt ( 8301 ) + saldoCuentaAnt ( 836 ) + saldoCuentaAnt ( 837 ) ).toQString ( '.' ) + "')\n\
doc.set_cell_property('bold', False)\n\
\
\
doc.set_cell_property('bold', True)\n\
doc.set_cell_value(2,24, 'string', 'C) Total transferencias a la cuenta de perdidas y ganancias (VI+VII+VIII+IX)')\n\
doc.set_cell_value(4,24, 'formula', '=D19+D20+D21+D22')\n\
doc.set_cell_value(5,24, 'formula', '=E19+E20+E21+E22')\n\
doc.set_cell_property('bold', False)\n\
\
\
\
doc.set_cell_property('bold', True)\n\
doc.set_cell_value(2,26, 'string', 'TOTAL DE INGRESOS Y GASTOS RECONOCIDOS (A+ B +C)')\n\
doc.set_cell_value(4,26, 'formula', '=D6+D15+D24')\n\
doc.set_cell_value(5,26, 'formula', '=E6+E15+E24')\n\
doc.set_cell_property('bold', False)\n\
\
\
\
\
\
\
# Guardamos el documento\n\
doc.save(\"canualesods.ods\")\n" );
                          }


