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

#include <QString>
#include "pluginbc_cuentasanuales2ods.h"

///
/**
**/
QString PluginBc_CuentasAnuales2ODS::cuentaAnualAsociancionSinLucro ()
{
    return QString::QString ( "\
#!/usr/bin/python\n\
# -*- coding: utf8 -*-\n\
\n\
import ooolib\n\
\n\
# Crea el documento\n\
doc = ooolib.Calc(\"Activo\")\n\
\n\
\
doc.set_column_property(1, 'width', '0.5in')\n\
doc.set_column_property(2, 'width', '5in')\n\
doc.set_cell_property('bold', True)\n\
doc.set_cell_value(2, 2, 'string', 'BALANCE DE SITUACIÓN ABREVIADO')\n\
doc.set_cell_value(2, 3, 'string', '" + QDateTime::currentDateTime().toString() + "')\n\
doc.set_cell_value(2, 5, 'string', 'ACTIVO')\n\
doc.set_cell_value(3, 5, 'string', 'Ejercicio N')\n\
doc.set_cell_value(4, 5, 'string', 'Ejercicio N-1')\n\
doc.set_cell_property('bold', False)\n\
\
doc.set_cell_property('bold', True)\n\
doc.set_cell_value(2, 7, 'string', 'A) Fundadores/asociados por desembolsos no exigidos')\n\
doc.set_cell_value(3, 7, 'float', '" + ( saldoCuenta ( 196 ) + saldoCuenta ( 197 ) ).toQString ( '.' ) + "')\n\
doc.set_cell_value(4, 7, 'float', '" + ( saldoCuentaAnt ( 196 ) + saldoCuentaAnt ( 197 ) ).toQString ( '.' ) + "')\n\
doc.set_cell_property('bold', False)\n\
\
doc.set_cell_property('bold', True)\n\
doc.set_cell_value(2, 9, 'string', 'B) Inmovilizado')\n\
doc.set_cell_value(3, 9, 'formula', '=C10+C11+C12+C13+C14')\n\
doc.set_cell_value(4, 9, 'formula', '=D10+D11+D12+D13+D14')\n\
doc.set_cell_property('bold', False)\n\
\
doc.set_cell_value(2, 10, 'string', '   I.   Gastos de establecimiento')\n\
doc.set_cell_value(3, 10, 'float', '" + saldoCuenta ( 20 ).toQString ( '.' ) + "')\n\
doc.set_cell_value(4, 10, 'float', '" + saldoCuentaAnt ( 20 ).toQString ( '.' ) + "')\n\
\
\
doc.set_cell_value(2, 11, 'string', '   II.  Inmovilizaciones inmateriales')\n\
doc.set_cell_value(3, 11, 'float', '" + ( saldoCuenta ( 21 ) + ( saldoCuenta ( 281 ) ) + ( saldoCuenta ( 291 ) ) ).toQString ( '.' ) + "')\n\
doc.set_cell_value(4, 11, 'float', '" + ( saldoCuentaAnt ( 21 ) + ( saldoCuentaAnt ( 281 ) ) + ( saldoCuentaAnt ( 291 ) ) ).toQString ( '.' ) + "')\n\
\
doc.set_cell_value(2, 12, 'string', '   III. Bienes de Patrimonio Historico')\n\
doc.set_cell_value(3, 12, 'float', '" + ( saldoCuenta ( 230 ) + saldoCuenta ( 231 ) + saldoCuenta ( 232 ) + saldoCuenta ( 233 ) + saldoCuenta ( 234 ) + saldoCuenta ( 239 ) + ( saldoCuentaAnt ( 2921 ) ) ).toQString ( '.' ) + "')\n\
doc.set_cell_value(4, 12, 'float', '" + ( saldoCuentaAnt ( 230 ) + saldoCuentaAnt ( 231 ) + saldoCuentaAnt ( 232 ) + saldoCuentaAnt ( 233 ) + saldoCuentaAnt ( 234 ) + saldoCuentaAnt ( 239 ) + ( saldoCuentaAnt ( 2921 ) ) ).toQString ( '.' ) + "')\n\
\
doc.set_cell_value(2, 13, 'string', '   IV.  Otras inmovilizaciones materiales')\n\
doc.set_cell_value(3, 13, 'float', '" + ( saldoCuenta ( 22 ) + ( saldoCuenta ( 282 ) ) + ( saldoCuenta ( 2920 ) ) ).toQString ( '.' ) + "')\n\
doc.set_cell_value(4, 13, 'float', '" + ( saldoCuentaAnt ( 22 ) + ( saldoCuentaAnt ( 282 ) ) + ( saldoCuentaAnt ( 2920 ) ) ).toQString ( '.' ) + "')\n\
\
doc.set_cell_value(2, 14, 'string', '   V.   Inmovilizaciones financieras')\n\
doc.set_cell_value(3, 14, 'float', '" + ( saldoCuenta ( 240 ) + saldoCuenta ( 241 ) + saldoCuenta ( 242 ) + saldoCuenta ( 243 ) + saldoCuenta ( 244 ) + saldoCuenta ( 245 ) + saldoCuenta ( 246 )  + saldoCuenta ( 247 ) + saldoCuenta ( 250 ) + saldoCuenta ( 251 ) + saldoCuenta ( 252 ) + saldoCuenta ( 253 ) + saldoCuenta ( 254 ) + saldoCuenta ( 256 ) + saldoCuenta ( 257 ) + saldoCuenta ( 258 ) + saldoCuenta ( 26 ) + ( saldoCuenta ( 293 ) ) + ( saldoCuenta ( 294 ) ) + ( saldoCuenta ( 295 ) ) + ( saldoCuenta ( 296 ) ) + ( saldoCuenta ( 297 ) ) + ( saldoCuenta ( 298 ) ) ).toQString ( '.' ) + "')\n\
doc.set_cell_value(4, 14, 'float', '" + ( saldoCuentaAnt ( 240 ) + saldoCuentaAnt ( 241 ) + saldoCuentaAnt ( 242 ) + saldoCuentaAnt ( 243 ) + saldoCuentaAnt ( 244 ) + saldoCuentaAnt ( 245 ) + saldoCuentaAnt ( 246 )  + saldoCuentaAnt ( 247 ) + saldoCuentaAnt ( 250 ) + saldoCuentaAnt ( 251 ) + saldoCuentaAnt ( 252 ) + saldoCuentaAnt ( 253 ) + saldoCuentaAnt ( 254 ) + saldoCuentaAnt ( 256 ) + saldoCuentaAnt ( 257 ) + saldoCuentaAnt ( 258 ) + saldoCuentaAnt ( 26 ) + ( saldoCuentaAnt ( 293 ) ) + ( saldoCuentaAnt ( 294 ) ) + ( saldoCuentaAnt ( 295 ) ) + ( saldoCuentaAnt ( 296 ) ) + ( saldoCuentaAnt ( 297 ) ) + ( saldoCuentaAnt ( 298 ) ) ).toQString ( '.' ) + "')\n\
\
doc.set_cell_property('bold', True)\n\
doc.set_cell_value(2, 15, 'string', 'C) Gastos a distribuir en varios ejercicios')\n\
doc.set_cell_value(3, 15, 'float', '" + saldoCuenta ( 27 ).toQString ( '.' ) + "')\n\
doc.set_cell_value(4, 15, 'float', '" + saldoCuentaAnt ( 27 ).toQString ( '.' ) + "')\n\
doc.set_cell_property('bold', False)\n\
\
doc.set_cell_property('bold', True)\n\
doc.set_cell_value(2, 16, 'string', 'D) Activo circulante')\n\
doc.set_cell_value(3, 16, 'formula', '=C17+C18+C19+C20+C21+C22+C23')\n\
doc.set_cell_value(4, 16, 'formula', '=D17+D18+D19+D20+D21+D22+D23')\n\
doc.set_cell_property('bold', False)\n\
\
doc.set_cell_value(2, 17, 'string', '   I.   Fundadores/asociados por desembolsos exigidos')\n\
doc.set_cell_value(3, 17, 'float', '" + saldoCuenta ( 558 ).toQString ( '.' ) + "')\n\
doc.set_cell_value(4, 17, 'float', '" + saldoCuentaAnt ( 558 ).toQString ( '.' ) + "')\n\
\
doc.set_cell_value(2, 18, 'string', '   II.  Existencias')\n\
doc.set_cell_value(3, 18, 'float', '" + ( saldoCuenta ( 30 ) + saldoCuenta ( 31 ) + saldoCuenta ( 32 ) + saldoCuenta ( 33 ) + saldoCuenta ( 34 ) + saldoCuenta ( 35 ) + saldoCuenta ( 36 ) + ( saldoCuenta ( 39 ) ) + saldoCuenta ( 407 ) ).toQString ( '.' ) + "')\n\
doc.set_cell_value(4, 18, 'float', '"  + ( saldoCuentaAnt ( 30 ) + saldoCuentaAnt ( 31 ) + saldoCuentaAnt ( 32 ) + saldoCuentaAnt ( 33 ) + saldoCuentaAnt ( 34 ) + saldoCuentaAnt ( 35 ) + saldoCuentaAnt ( 36 ) + ( saldoCuentaAnt ( 39 ) ) + saldoCuentaAnt ( 407 ) ).toQString ( '.' ) + "')\n\
\
doc.set_cell_value(2, 19, 'string', '   III. Usuarios y otros deudores de la actividad propia')\n\
doc.set_cell_value(3, 19, 'float', '"  + ( saldoCuenta ( 446 ) + saldoCuenta ( 447 ) + ( saldoCuenta ( 495 ) ) ).toQString ( '.' ) + "')\n\
doc.set_cell_value(4, 19, 'float', '"  + ( saldoCuentaAnt ( 446 ) + saldoCuentaAnt ( 447 ) + ( saldoCuentaAnt ( 495 ) ) ).toQString ( '.' ) + "')\n\
\
doc.set_cell_value(2, 20, 'string', '   IV.  Otros deudores')\n\
doc.set_cell_value(3, 20, 'float', '" + ( saldoCuenta ( 430 ) + saldoCuenta ( 431 ) + saldoCuenta ( 432 ) + saldoCuenta ( 433 ) + saldoCuenta ( 435 ) + ( saldoCuenta ( 436 ) ) + saldoCuenta ( 440 )  + saldoCuenta ( 441 ) + saldoCuenta ( 445 ) + saldoCuenta ( 449 ) + saldoCuenta ( 460 ) + saldoCuenta ( 464 ) + saldoCuenta ( 470 ) + saldoCuenta ( 471 ) + saldoCuenta ( 472 ) + saldoCuenta ( 474 ) + ( saldoCuenta ( 490 ) ) + ( saldoCuenta ( 493 ) ) + ( saldoCuenta ( 494 ) ) + saldoCuenta ( 544 ) + cuentaPositiva ( saldoCuenta ( 551 ) ) + cuentaPositiva ( saldoCuenta ( 552 ) ) + cuentaPositiva ( saldoCuenta ( 553 ) ) ).toQString ( '.' ) + "')\n\
doc.set_cell_value(4, 20, 'float', '" + ( saldoCuentaAnt ( 430 ) + saldoCuentaAnt ( 431 ) + saldoCuentaAnt ( 432 ) + saldoCuentaAnt ( 433 ) + saldoCuentaAnt ( 435 ) + ( saldoCuentaAnt ( 436 ) ) + saldoCuentaAnt ( 440 )  + saldoCuentaAnt ( 441 ) + saldoCuentaAnt ( 445 ) + saldoCuentaAnt ( 449 ) + saldoCuentaAnt ( 460 ) + saldoCuentaAnt ( 464 ) + saldoCuentaAnt ( 470 ) + saldoCuentaAnt ( 471 ) + saldoCuentaAnt ( 472 ) + saldoCuentaAnt ( 474 ) + ( saldoCuentaAnt ( 490 ) ) + ( saldoCuentaAnt ( 493 ) ) + ( saldoCuentaAnt ( 494 ) ) + saldoCuentaAnt ( 544 ) + cuentaPositiva ( saldoCuentaAnt ( 551 ) ) + cuentaPositiva ( saldoCuentaAnt ( 552 ) ) + cuentaPositiva ( saldoCuentaAnt ( 553 ) ) ).toQString ( '.' ) + "')\n\
\
doc.set_cell_value(2, 21, 'string', '   IV.  Inversiones financieras temporales')\n\
doc.set_cell_value(3, 21, 'float', '" + ( saldoCuenta ( 53 ) + saldoCuenta ( 540 ) + saldoCuenta ( 541 ) + saldoCuenta ( 542 ) + saldoCuenta ( 543 ) + saldoCuenta ( 545 ) + saldoCuenta ( 546 )  + saldoCuenta ( 547 ) + saldoCuenta ( 548 ) + ( saldoCuenta ( 549 ) ) + saldoCuenta ( 565 ) + saldoCuenta ( 566 ) + ( saldoCuenta ( 59 ) ) ).toQString ( '.' ) + "')\n\
doc.set_cell_value(4, 21, 'float', '" + ( saldoCuentaAnt ( 53 ) + saldoCuentaAnt ( 540 ) + saldoCuentaAnt ( 541 ) + saldoCuentaAnt ( 542 ) + saldoCuentaAnt ( 543 ) + saldoCuentaAnt ( 545 ) + saldoCuentaAnt ( 546 )  + saldoCuentaAnt ( 547 ) + saldoCuentaAnt ( 548 ) + ( saldoCuentaAnt ( 549 ) ) + saldoCuentaAnt ( 565 ) + saldoCuentaAnt ( 566 ) + ( saldoCuentaAnt ( 59 ) ) ).toQString ( '.' ) + "')\n\
\
doc.set_cell_value(2, 22, 'string', '   VI.  Tesoreria')\n\
doc.set_cell_value(3, 22, 'float', '" + saldoCuenta ( 57 ).toQString ( '.' ) + "')\n\
doc.set_cell_value(4, 22, 'float', '" + saldoCuentaAnt ( 57 ).toQString ( '.' ) + "')\n\
\
doc.set_cell_value(2, 23, 'string', '   VII. Ajustes por periodificacion')\n\
doc.set_cell_value(3, 23, 'float', '" + ( saldoCuenta ( 480 ) + saldoCuenta ( 580 ) ).toQString ( '.' ) + "')\n\
doc.set_cell_value(4, 23, 'float', '" + ( saldoCuentaAnt ( 480 ) + saldoCuentaAnt ( 580 ) ).toQString ( '.' ) + "')\n\
\
doc.set_cell_property('bold', True)\n\
doc.set_cell_value(2, 25, 'string', 'TOTAL GENERAL (A+B+C+D)')\n\
doc.set_cell_value(3, 25, 'formula', '=C7+C9+C15+C16')\n\
doc.set_cell_value(4, 25, 'formula', '=D7+D9+D15+D16')\n\
doc.set_cell_property('bold', False)\n\
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
doc.set_cell_value(2, 2, 'string', 'BALANCE DE SITUACIÓN ABREVIADO')\n\
doc.set_cell_value(2, 5, 'string', 'PASIVO')\n\
doc.set_cell_value(3, 5, 'string', 'Ejercicio N')\n\
doc.set_cell_value(4, 5, 'string', 'Ejercicio N-1')\n\
doc.set_cell_property('bold', False)\n\
\
doc.set_cell_property('bold', True)\n\
doc.set_cell_value(2, 7, 'string', 'A) Fondos propios')\n\
doc.set_cell_value(3, 7, 'formula', '=C8+C9+C10+C11+C12')\n\
doc.set_cell_value(4, 7, 'formula', '=D8+D9+D10+D11+D12')\n\
doc.set_cell_property('bold', False)\n\
\
doc.set_cell_value(2, 8, 'string', '   I.   Dotacion fundacional/Fondo social')\n\
doc.set_cell_value(3, 8, 'float', '" + ( - saldoCuenta ( 10 ) ).toQString ( '.' ) + "')\n\
doc.set_cell_value(4, 8, 'float', '" + ( - saldoCuentaAnt ( 10 ) ).toQString ( '.' ) + "')\n\
\
doc.set_cell_value(2, 9, 'string', '   II.  Reserva de revalorizacion')\n\
doc.set_cell_value(3, 9, 'float', '" + ( - saldoCuenta ( 111 ) ).toQString ( '.' ) + "')\n\
doc.set_cell_value(4, 9, 'float', '" + ( - saldoCuentaAnt ( 111 ) ).toQString ( '.' ) + "')\n\
\
doc.set_cell_value(2, 10, 'string', '   III. Reservas')\n\
doc.set_cell_value(3, 10, 'float', '"  + ( - ( saldoCuenta ( 113 ) + saldoCuenta ( 116 ) + saldoCuenta ( 117 ) ) ).toQString ( '.' ) + "')\n\
doc.set_cell_value(4, 10, 'float', '" + ( - ( saldoCuentaAnt ( 113 ) + saldoCuentaAnt ( 116 ) + saldoCuentaAnt ( 117 ) ) ).toQString ( '.' ) + "')\n\
\
doc.set_cell_value(2, 11, 'string', '   IV.  Excedentes de ejercicions anteriores')\n\
doc.set_cell_value(3, 11, 'float', '" + ( - ( saldoCuenta ( 120 ) + ( saldoCuenta ( 121 ) ) ) ).toQString ( '.' ) + "')\n\
doc.set_cell_value(4, 11, 'float', '" + ( - ( saldoCuentaAnt ( 120 ) + ( saldoCuentaAnt ( 121 ) ) ) ).toQString ( '.' ) + "')\n\
\
doc.set_cell_value(2, 12, 'string', '   V.   Excedentes del ejercicio (positivo o negativo)')\n\
doc.set_cell_value(3, 12, 'float', '" + ( - saldoCuenta ( 129 ) ).toQString ( '.' ) + "')\n\
doc.set_cell_value(4, 12, 'float', '"  + ( - saldoCuentaAnt ( 129 ) ).toQString ( '.' ) + "')\n\
\
doc.set_cell_property('bold', True)\n\
doc.set_cell_value(2, 14, 'string', 'B) Ingresos a distribuir en varios ejercicios')\n\
doc.set_cell_value(3, 14, 'float', '" + ( - saldoCuenta ( 13 ) ).toQString ( '.' ) + "')\n\
doc.set_cell_value(4, 14, 'float', '" + ( - saldoCuentaAnt ( 13 ) ).toQString ( '.' ) + "')\n\
doc.set_cell_property('bold', False)\n\
\
doc.set_cell_property('bold', True)\n\
doc.set_cell_value(2, 16, 'string', 'C) Provisiones para riesgos y gastos')\n\
doc.set_cell_value(3, 16, 'float', '" + ( -  saldoCuenta ( 14 ) ).toQString ( '.' ) + "')\n\
doc.set_cell_value(4, 16, 'float', '" + ( - saldoCuentaAnt ( 14 ) ).toQString ( '.' ) + "')\n\
doc.set_cell_property('bold', False)\n\
\
doc.set_cell_property('bold', True)\n\
doc.set_cell_value(2, 17, 'string', 'D) Acreedores a largo plazo')\n\
doc.set_cell_value(3, 17, 'float', '"  + ( - ( saldoCuenta ( 15 ) + saldoCuenta ( 16 ) + saldoCuenta ( 17 ) + saldoCuenta ( 18 ) + saldoCuenta ( 248 ) + saldoCuenta ( 249 ) + saldoCuenta ( 259 ) ) ).toQString ( '.' ) + "')\n\
doc.set_cell_value(4, 17, 'float', '"  + ( - ( saldoCuentaAnt ( 15 ) + saldoCuentaAnt ( 16 ) + saldoCuentaAnt ( 17 ) + saldoCuentaAnt ( 18 ) + saldoCuentaAnt ( 248 ) + saldoCuentaAnt ( 249 ) + saldoCuentaAnt ( 259 ) ) ).toQString ( '.' ) + "')\n\
doc.set_cell_property('bold', False)\n\
\
doc.set_cell_property('bold', True)\n\
doc.set_cell_value(2, 19, 'string', 'E) Acreedores a corto plazo')\n\
doc.set_cell_value(3, 19, 'float', '" + ( - ( saldoCuenta ( 400 ) + saldoCuenta ( 401 ) + saldoCuenta ( 402 ) + saldoCuenta ( 403 ) + ( saldoCuenta ( 406 ) ) + saldoCuenta ( 41 ) + saldoCuenta ( 437 ) + saldoCuenta ( 465 ) + saldoCuenta ( 475 ) + saldoCuenta ( 476 ) + saldoCuenta ( 477 ) + saldoCuenta ( 479 ) + saldoCuenta ( 485 ) + saldoCuenta ( 499 ) + saldoCuenta ( 50 ) + saldoCuenta ( 51 ) + saldoCuenta ( 52 ) + cuentaNegativa ( saldoCuenta ( 551 ) ) + cuentaNegativa ( saldoCuenta ( 552 ) ) + cuentaNegativa ( saldoCuenta ( 553 ) ) + saldoCuenta ( 555 ) + saldoCuenta ( 556 ) + saldoCuenta ( 560 ) + saldoCuenta ( 561 ) + saldoCuenta ( 585 ) ) ).toQString ( '.' ) + "')\n\
doc.set_cell_value(4, 19, 'float', '" + ( - ( saldoCuentaAnt ( 400 ) + saldoCuentaAnt ( 401 ) + saldoCuentaAnt ( 402 ) + saldoCuentaAnt ( 403 ) + ( saldoCuentaAnt ( 406 ) ) + saldoCuentaAnt ( 41 ) + saldoCuentaAnt ( 437 ) + saldoCuentaAnt ( 465 ) + saldoCuentaAnt ( 475 ) + saldoCuentaAnt ( 476 ) + saldoCuentaAnt ( 477 ) + saldoCuentaAnt ( 479 ) + saldoCuentaAnt ( 485 ) + saldoCuentaAnt ( 499 ) + saldoCuentaAnt ( 50 ) + saldoCuentaAnt ( 51 ) + saldoCuentaAnt ( 52 ) + cuentaNegativa ( saldoCuentaAnt ( 551 ) ) + cuentaNegativa ( saldoCuentaAnt ( 552 ) ) + cuentaNegativa ( saldoCuentaAnt ( 553 ) ) + saldoCuentaAnt ( 555 ) + saldoCuentaAnt ( 556 ) + saldoCuentaAnt ( 560 ) + saldoCuentaAnt ( 561 ) + saldoCuentaAnt ( 585 ) ) ).toQString ( '.' ) + "')\n\
doc.set_cell_property('bold', False)\n\
\
doc.set_cell_property('bold', True)\n\
doc.set_cell_value(2, 21, 'string', 'TOTAL GENERAL (A+B+C+D+E)')\n\
doc.set_cell_value(3, 21, 'formula', '=C7+C14+C16+C17+C19')\n\
doc.set_cell_value(4, 21, 'formula', '=D7+D14+D16+D17+D19')\n\
doc.set_cell_property('bold', False)\n\
\
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
doc.set_cell_value(2, 2, 'string', 'CUENTA DE RESULTADOS ABREVIADA')\n\
doc.set_cell_value(2, 5, 'string', 'DEBE')\n\
doc.set_cell_property('bold', False)\n\
\
doc.set_cell_property('bold', True)\n\
doc.set_cell_value(2, 7, 'string', 'A) GASTOS')\n\
doc.set_cell_value(3, 7, 'formula', '=0')\n\
doc.set_cell_value(4, 7, 'formula', '=0')\n\
doc.set_cell_property('bold', False)\n\
\
doc.set_cell_value(2, 8, 'string', '       1. Ayudas monetarias y otros')\n\
doc.set_cell_value(3, 8, 'formula', '=C9+C10+C11')\n\
doc.set_cell_value(4, 8, 'formula', '=D9+D10+D11')\n\
\
doc.set_cell_value(2, 9, 'string', '            a) Ayudas monetarias')\n\
doc.set_cell_value(3, 9, 'float', '" + ( saldoCuenta ( 650 ) + saldoCuenta ( 651 ) + saldoCuenta ( 652 ) ).toQString ( '.' ) + "')\n\
doc.set_cell_value(4, 9, 'float', '" + ( saldoCuentaAnt ( 650 ) + saldoCuentaAnt ( 651 ) + saldoCuentaAnt ( 652 ) ).toQString ( '.' ) + "')\n\
\
doc.set_cell_value(2, 10, 'string', '            b) Gastos por colaboraciones y del organo de gobierno')\n\
doc.set_cell_value(3, 10, 'float', '" + ( saldoCuenta ( 653 ) + saldoCuenta ( 654 ) ).toQString ( '.' ) + "')\n\
doc.set_cell_value(4, 10, 'float', '" + ( saldoCuentaAnt ( 653 ) + saldoCuentaAnt ( 654 ) ).toQString ( '.' ) + "')\n\
\
doc.set_cell_value(2, 11, 'string', '            c) Reintegro de ayudas y asignaciones')\n\
doc.set_cell_value(3, 11, 'float', '" + ( saldoCuenta ( 728 ) ).toQString ( '.' ) + "')\n\
doc.set_cell_value(4, 11, 'float', '" + ( saldoCuentaAnt ( 728 ) ).toQString ( '.' ) + "')\n\
\
doc.set_cell_value(2, 12, 'string', '       2. Consumos de explotacion')\n\
doc.set_cell_value(3, 12, 'float', '" + ( saldoCuenta ( 60 ) +  saldoCuenta ( 61 ) +  saldoCuenta ( 71 ) ).toQString ( '.' ) + "')\n\
doc.set_cell_value(4, 12, 'float', '" + ( saldoCuentaAnt ( 60 ) +  saldoCuentaAnt ( 61 ) +  saldoCuentaAnt ( 71 ) ).toQString ( '.' ) + "')\n\
\
doc.set_cell_value(2, 13, 'string', '       3. Gastos de personal')\n\
doc.set_cell_value(3, 13, 'formula', '=SUM(C14:C15)')\n\
doc.set_cell_value(4, 13, 'formula', '=SUM(D14:D15)')\n\
\
doc.set_cell_value(2, 14, 'string', '            a) Sueldos, salarios y asimilados')\n\
doc.set_cell_value(3, 14, 'float', '" + ( saldoCuenta ( 640 ) + saldoCuenta ( 641 ) ).toQString ( '.' ) + "')\n\
doc.set_cell_value(4, 14, 'float', '" + ( saldoCuentaAnt ( 640 ) + saldoCuentaAnt ( 641 ) ).toQString ( '.' ) + "')\n\
doc.set_cell_value(2, 15, 'string', '            b) Cargas sociales')\n\
doc.set_cell_value(3, 15, 'float', '" + ( saldoCuenta ( 642 ) + saldoCuenta ( 643 ) + saldoCuenta ( 649 ) ).toQString ( '.' ) + "')\n\
doc.set_cell_value(4, 15, 'float', '" + ( saldoCuentaAnt ( 642 ) + saldoCuentaAnt ( 643 ) + saldoCuentaAnt ( 649 ) ).toQString ( '.' ) + "')\n\
\
doc.set_cell_value(2, 16, 'string', '       4. Dotaciones para amortizaciones de inmovilizado')\n\
doc.set_cell_value(3, 16, 'float', '" + saldoCuenta ( 68 ).toQString ( '.' ) + "')\n\
doc.set_cell_value(4, 16, 'float', '" + saldoCuentaAnt ( 68 ).toQString ( '.' ) + "')\n\
\
doc.set_cell_value(2, 17, 'string', '       5. Otros gastos')\n\
doc.set_cell_value(3, 17, 'float', '" + ( saldoCuenta ( 62 ) +  saldoCuenta ( 631 ) +  saldoCuenta ( 634 ) + ( saldoCuenta ( 636 ) ) + ( saldoCuenta ( 639 ) ) +  saldoCuenta ( 656 ) +  saldoCuenta ( 659 ) +  saldoCuenta ( 690 ) ).toQString ( '.' ) + "')\n\
doc.set_cell_value(4, 17, 'float', '" + ( saldoCuentaAnt ( 62 ) +  saldoCuentaAnt ( 631 ) +  saldoCuentaAnt ( 634 ) + ( saldoCuentaAnt ( 636 ) ) + ( saldoCuentaAnt ( 639 ) ) +  saldoCuentaAnt ( 656 ) +  saldoCuentaAnt ( 659 ) +  saldoCuentaAnt ( 690 ) ).toQString ( '.' ) + "')\n\
\
doc.set_cell_value(2, 18, 'string', '       6. Variaciones de las provisiones de la actividad')\n\
doc.set_cell_value(3, 18, 'float', '" + ( saldoCuenta ( 655 ) +  saldoCuenta ( 693 ) +  saldoCuenta ( 694 ) + saldoCuenta ( 695 ) + ( saldoCuenta ( 793 ) ) +  ( saldoCuenta ( 794 ) ) + ( saldoCuenta ( 795 ) ) ).toQString ( '.' ) + "')\n\
doc.set_cell_value(4, 18, 'float', '" + ( saldoCuentaAnt ( 655 ) +  saldoCuentaAnt ( 693 ) +  saldoCuentaAnt ( 694 ) + saldoCuentaAnt ( 695 ) + ( saldoCuentaAnt ( 793 ) ) +  ( saldoCuentaAnt ( 794 ) ) + ( saldoCuentaAnt ( 795 ) ) ).toQString ( '.' ) + "')\n\
\
doc.set_cell_value(2, 20, 'string', '   I.   RESULTADOS POSITIVOS DE LA EXPLOTACIÓN')\n\
doc.set_cell_value(2, 21, 'string', '        (B1+B2+B3-A1-A2-A3-A4-A5-A6)')\n\
doc.set_cell_value(3, 20, 'formula', '= if ((Haber.C8+Haber.C13+Haber.C14-C8-C12-C13-C16-C17-C18)>0;Haber.C8+Haber.C13+Haber.C14-C8-C12-C13-C16-C17-C18;T(0))')\n\
doc.set_cell_value(4, 20, 'formula', '= if((Haber.D8+Haber.D13+Haber.D14-D8-D12-D13-D16-D17-D18) >0;Haber.D8+Haber.D13+Haber.D14-D8-D12-D13-D16-D17-D18;T(0))')\n\
\
doc.set_cell_value(2, 23, 'string', '       7. Gastos financieros y gastos asimilados')\n\
doc.set_cell_value(3, 23, 'formula', '=SUM(C24:C27)')\n\
doc.set_cell_value(4, 23, 'formula', '=SUM(D24:D27)')\n\
\
doc.set_cell_value(2, 24, 'string', '            a) Por deudas con entidades del grupo')\n\
doc.set_cell_value(3, 24, 'float', '" + ( saldoCuenta ( 6610 ) +  saldoCuenta ( 6615 ) +  saldoCuenta ( 6620 ) + saldoCuenta ( 6630 ) + saldoCuenta ( 6640 ) +  saldoCuenta ( 6650 ) ).toQString ( '.' ) + "')\n\
doc.set_cell_value(4, 24, 'float', '" + ( saldoCuentaAnt ( 6610 ) +  saldoCuentaAnt ( 6615 ) +  saldoCuentaAnt ( 6620 ) + saldoCuentaAnt ( 6630 ) + saldoCuentaAnt ( 6640 ) +  saldoCuentaAnt ( 6650 ) ).toQString ( '.' ) + "')\n\
\
doc.set_cell_value(2, 25, 'string', '            d) Por deudas con entidades asociadas')\n\
doc.set_cell_value(3, 25, 'float', '" + ( saldoCuenta ( 6611 ) +  saldoCuenta ( 6616 ) +  saldoCuenta ( 6621 ) + saldoCuenta ( 6631 ) + saldoCuenta ( 6641 ) +  saldoCuenta ( 6651 ) ).toQString ( '.' ) + "')\n\
doc.set_cell_value(4, 25, 'float', '" + ( saldoCuentaAnt ( 6611 ) +  saldoCuentaAnt ( 6616 ) +  saldoCuentaAnt ( 6621 ) + saldoCuentaAnt ( 6631 ) + saldoCuentaAnt ( 6641 ) +  saldoCuentaAnt ( 6651 ) ).toQString ( '.' ) + "')\n\
\
doc.set_cell_value(2, 26, 'string', '            c) Por otras deudas')\n\
doc.set_cell_value(3, 26, 'float', '" + ( saldoCuenta ( 6613 ) +  saldoCuenta ( 6618 ) +  saldoCuenta ( 6622 ) + saldoCuenta ( 6623 )  +  saldoCuenta ( 6643 ) +  saldoCuenta ( 6653 ) + saldoCuenta ( 669 ) + saldoCuenta ( 6632 ) +  saldoCuenta ( 6633 ) ).toQString ( '.' ) + "')\n\
doc.set_cell_value(4, 26, 'float', '" + ( saldoCuentaAnt ( 6613 ) +  saldoCuentaAnt ( 6618 ) +  saldoCuentaAnt ( 6622 ) + saldoCuentaAnt ( 6623 )  +  saldoCuentaAnt ( 6643 ) +  saldoCuentaAnt ( 6653 ) + saldoCuentaAnt ( 669 ) + saldoCuentaAnt ( 6632 ) +  saldoCuentaAnt ( 6633 ) ).toQString ( '.' ) + "')\n\
\
doc.set_cell_value(2, 27, 'string', '            d) Perdidas de inversiones financieras')\n\
doc.set_cell_value(3, 27, 'float', '" + ( saldoCuenta ( 666 ) + saldoCuenta ( 667 ) ).toQString ( '.' ) + "')\n\
doc.set_cell_value(4, 27, 'float', '" + ( saldoCuentaAnt ( 666 ) + saldoCuentaAnt ( 667 ) ).toQString ( '.' ) + "')\n\
\
doc.set_cell_value(2, 28, 'string', '       8. Variacion de las provisiones de inversiones financieras')\n\
doc.set_cell_value(3, 28, 'float', '" + ( saldoCuenta ( 6963 ) + saldoCuenta ( 6965 ) + saldoCuenta ( 6966 ) + saldoCuenta ( 697 ) + saldoCuenta ( 698 ) + saldoCuenta ( 699 ) + ( saldoCuenta ( 7963 ) ) + ( saldoCuenta ( 7965 ) ) + ( saldoCuenta ( 7966 ) ) + ( saldoCuenta ( 797 ) ) + ( saldoCuenta ( 798 ) ) + ( saldoCuenta ( 799 ) ) ).toQString ( '.' ) + "')\n\
doc.set_cell_value(4, 28, 'float', '" + ( saldoCuentaAnt ( 6963 ) + saldoCuentaAnt ( 6965 ) + saldoCuentaAnt ( 6966 ) + saldoCuentaAnt ( 697 ) + saldoCuentaAnt ( 698 ) + saldoCuentaAnt ( 699 ) + ( saldoCuentaAnt ( 7963 ) ) + ( saldoCuentaAnt ( 7965 ) ) + ( saldoCuentaAnt ( 7966 ) ) + ( saldoCuentaAnt ( 797 ) ) + ( saldoCuentaAnt ( 798 ) ) + ( saldoCuentaAnt ( 799 ) ) ).toQString ( '.' ) + "')\n\
\
doc.set_cell_value(2, 29, 'string', '       9. Diferencias negativas de cambio')\n\
doc.set_cell_value(3, 29, 'float', '" + saldoCuenta ( 668 ).toQString ( '.' ) + "')\n\
doc.set_cell_value(4, 29, 'float', '" + saldoCuentaAnt ( 668 ).toQString ( '.' ) + "')\n\
\
doc.set_cell_value(2, 30, 'string', '   II.  RESULTADOS FINANCIEROS POSITIVOS')\n\
doc.set_cell_value(2, 31, 'string', '        (B4+B5-A7-A8-A9)')\n\
doc.set_cell_value(3, 30, 'formula', '=if((Haber.C19+Haber.C24-C23-C28-C29)>0;Haber.C19+Haber.C24-C23-C28-C29;T(0))')\n\
doc.set_cell_value(4, 30, 'formula', '=if((Haber.D19+Haber.D24-D23-D28-D29)>0;Haber.D19+Haber.D24-D23-D28-D29;T(0))')\n\
\
doc.set_cell_value(2, 33, 'string', '   III. RESULTADOS POSITIVOS DE LAS ACTIVIDADES ORDINARIAS')\n\
doc.set_cell_value(2, 34, 'string', '        (AI+AII-BI-BII)')\n\
doc.set_cell_value(3, 33, 'formula', '=if((C20+C30-Haber.C16-Haber.C26)>0;C20+C30-Haber.C16-Haber.C26;T(0))')\n\
doc.set_cell_value(4, 33, 'formula', '=if((D20+D30-Haber.D16-Haber.D26)>0;D20+D30-Haber.D16-Haber.D26;T(0))')\n\
\
doc.set_cell_value(2, 36, 'string', '      10. Variacion de las provisiones de inmobilizado inmaterial, material y cartera de control')\n\
doc.set_cell_value(3, 36, 'float', '" + ( saldoCuenta ( 691 ) + saldoCuenta ( 692 ) + saldoCuenta ( 6960 ) + saldoCuenta ( 6961 ) + ( saldoCuenta ( 791 ) ) + ( saldoCuenta ( 792 ) ) + ( saldoCuenta ( 7960 ) ) + ( saldoCuenta ( 7961 ) ) ).toQString ( '.' ) + "')\n\
doc.set_cell_value(4, 36, 'float', '" + ( saldoCuentaAnt ( 691 ) + saldoCuentaAnt ( 692 ) + saldoCuentaAnt ( 6960 ) + saldoCuentaAnt ( 6961 ) + ( saldoCuentaAnt ( 791 ) ) + ( saldoCuentaAnt ( 792 ) ) + ( saldoCuentaAnt ( 7960 ) ) + ( saldoCuentaAnt ( 7961 ) ) ).toQString ( '.' ) + "')\n\
\
doc.set_cell_value(2, 37, 'string', '      11. Perdidas procedentes del inmovilizado inmaterial, material y cartera de control')\n\
doc.set_cell_value(3, 37, 'float', '" + ( saldoCuenta ( 670 ) + saldoCuenta ( 671 ) + saldoCuenta ( 672 ) + saldoCuenta ( 673 ) ).toQString ( '.' ) + "')\n\
doc.set_cell_value(4, 37, 'float', '" + ( saldoCuentaAnt ( 670 ) + saldoCuentaAnt ( 671 ) + saldoCuentaAnt ( 672 ) + saldoCuentaAnt ( 673 ) ).toQString ( '.' ) + "')\n\
\
doc.set_cell_value(2, 38, 'string', '      12. Perdidas por operaciones con obligaciones propias')\n\
doc.set_cell_value(3, 38, 'float', '" + saldoCuenta ( 674 ).toQString ( '.' ) + "')\n\
doc.set_cell_value(4, 38, 'float', '" + saldoCuentaAnt ( 674 ).toQString ( '.' ) + "')\n\
\
doc.set_cell_value(2, 39, 'string', '      13. Gastos extraordinarios')\n\
doc.set_cell_value(3, 39, 'float', '" + saldoCuenta ( 678 ).toQString ( '.' ) + "')\n\
doc.set_cell_value(4, 39, 'float', '" + saldoCuentaAnt ( 678 ).toQString ( '.' ) + "')\n\
\
doc.set_cell_value(2, 40, 'string', '      14. Gastos y perdidas de otros ejercicios')\n\
doc.set_cell_value(3, 40, 'float', '" + saldoCuenta ( 679 ).toQString ( '.' ) + "')\n\
doc.set_cell_value(4, 40, 'float', '" + saldoCuentaAnt ( 679 ).toQString ( '.' ) + "')\n\
\
doc.set_cell_value(2, 42, 'string', '   IV.  RESULTADOS EXTRAORDINARIOS POSITIVOS')\n\
doc.set_cell_value(2, 43, 'string', '        (B6+B7+B8+B9+B10-A10-A11-A12-A13-A14)')\n\
doc.set_cell_value(3, 42, 'formula', '=if((Haber.C32+Haber.C33+Haber.C34+Haber.C35+Haber.C36-C37-C38-C39-C40)>0;Haber.C32+Haber.C33+Haber.C34+Haber.C35+Haber.C36-C37-C38-C39-C40;T(0))')\n\
doc.set_cell_value(4, 42, 'formula', '=if((Haber.D32+Haber.D33+Haber.D34+Haber.D35+Haber.D36-D37-D38-D39-D40)>0;Haber.D32+Haber.D33+Haber.D34+Haber.D35+Haber.D36-D37-D38-D39-D40;T(0))')\n\
\
doc.set_cell_value(2, 45, 'string', '   V.   RESULTADOS POSITIVOS ANTES DE IMPUESTOS')\n\
doc.set_cell_value(2, 46, 'string', '        (AIII+AIV-BIII-BIV)')\n\
doc.set_cell_value(3, 45, 'formula', '=if((C33+C42-Haber.C29-Haber.C38)>0;C33+C42-Haber.C29-Haber.C38;T(0))')\n\
doc.set_cell_value(4, 45, 'formula', '=if((D33+D42-Haber.D29-Haber.D38)>0;D33+D42-Haber.D29-Haber.D38;T(0))')\n\
\
doc.set_cell_value(2, 48, 'string', '      15. Impuesto sobre Sociedades')\n\
doc.set_cell_value(3, 48, 'float', '" + ( saldoCuenta ( 630 ) + saldoCuenta ( 633 ) + ( saldoCuenta ( 638 ) ) ).toQString ( '.' ) + "')\n\
doc.set_cell_value(4, 48, 'float', '" + ( saldoCuentaAnt ( 630 ) + saldoCuentaAnt ( 633 ) + ( saldoCuentaAnt ( 638 ) ) ).toQString ( '.' ) + "')\n\
\
doc.set_cell_value(2, 49, 'string', '      16. Otros impuestos')\n\
doc.set_cell_value(3, 49, 'formula', '=0')\n\
doc.set_cell_value(4, 49, 'formula', '=0')\n\
\
doc.set_cell_value(2, 51, 'string', '   VI.  EXCEDENTE POSITIVO DEL EJERCICIO (AHORRO)')\n\
doc.set_cell_value(2, 52, 'string', '        (AV-A15-A16)')\n\
doc.set_cell_value(3, 51, 'formula', '=if((C45-C48-C49)>0;C45-C48-C49;T(0))')\n\
doc.set_cell_value(4, 51, 'formula', '=if((D45-D48-D49)>0;D45-D48-D49;T(0))')\n\
\
\
\
\
\
doc.new_sheet(\"Haber\")\n\n\
doc.set_column_property(1, 'width', '0.5in')\n\
doc.set_column_property(2, 'width', '5in')\n\
doc.set_cell_property('bold', True)\n\
doc.set_cell_value(2, 2, 'string', 'CUENTA DE RESULTADOS ABREVIADA')\n\
doc.set_cell_value(2, 5, 'string', 'HABER')\n\
doc.set_cell_property('bold', False)\n\
\
doc.set_cell_property('bold', True)\n\
doc.set_cell_value(2, 7, 'string', 'B) INGRESOS')\n\
doc.set_cell_property('bold', False)\n\
\
doc.set_cell_value(2, 8, 'string', '       1. Ingresos de la entidad por la actividad propia')\n\
doc.set_cell_value(3, 8, 'formula', '=SUM(C9:C12)')\n\
doc.set_cell_value(4, 8, 'formula', '=SUM(D9:D12)')\n\
\
doc.set_cell_value(2, 9, 'string', '            a) Quotas de usuarios y afiliados')\n\
doc.set_cell_value(3, 9, 'float', '" + ( - ( saldoCuenta ( 720 ) + saldoCuenta ( 721 ) ) ).toQString ( '.' ) + "')\n\
doc.set_cell_value(4, 9, 'float', '" + ( - ( saldoCuentaAnt ( 720 ) + saldoCuentaAnt ( 721 ) ) ).toQString ( '.' ) + "')\n\
\
doc.set_cell_value(2, 10, 'string', '            b) Ingresos de pomociones, patrocinadores y colaboraciones')\n\
doc.set_cell_value(3, 10, 'float', '" + ( - ( saldoCuenta ( 722 ) + saldoCuenta ( 723 ) ) ).toQString ( '.' ) + "')\n\
doc.set_cell_value(4, 10, 'float', '" + ( - ( saldoCuentaAnt ( 722 ) + saldoCuentaAnt ( 723 ) ) ).toQString ( '.' ) + "')\n\
\
doc.set_cell_value(2, 11, 'string', '            c) Subvenciones, donaciones y legados imputados a resultados del ejercicio')\n\
doc.set_cell_value(3, 11, 'float', '" + ( - ( saldoCuenta ( 725 ) + saldoCuenta ( 726 ) ) ).toQString ( '.' ) + "')\n\
doc.set_cell_value(4, 11, 'float', '" + ( - ( saldoCuentaAnt ( 725 ) + saldoCuentaAnt ( 726 ) ) ).toQString ( '.' ) + "')\n\
\
doc.set_cell_value(2, 12, 'string', '            d) Reintegros de subvenciones donaciones y legados')\n\
doc.set_cell_value(3, 12, 'float', '" + ( - ( saldoCuenta ( 658 ) ) ).toQString ( '.' ) + "')\n\
doc.set_cell_value(4, 12, 'float', '" + ( - ( saldoCuentaAnt ( 658 ) ) ).toQString ( '.' ) + "')\n\
\
doc.set_cell_value(2, 13, 'string', '       2. Ventas y otros ingresos ordinarios de la actividad mercantil')\n\
doc.set_cell_value(3, 13, 'float', '" + ( - saldoCuenta ( 70 ) ).toQString ( '.' ) + "')\n\
doc.set_cell_value(4, 13, 'float', '" + ( - saldoCuentaAnt ( 70 ) ).toQString ( '.' ) + "')\n\
\
doc.set_cell_value(2, 14, 'string', '       3. Otros ingresos')\n\
doc.set_cell_value(3, 14, 'float', '" + ( - ( saldoCuenta ( 73 ) + saldoCuenta ( 74 ) + saldoCuenta ( 75 ) + saldoCuenta ( 790 ) ) ).toQString ( '.' ) + "')\n\
doc.set_cell_value(4, 14, 'float', '" + ( - ( saldoCuentaAnt ( 73 ) + saldoCuentaAnt ( 74 ) + saldoCuentaAnt ( 75 ) + saldoCuentaAnt ( 790 ) ) ).toQString ( '.' ) + "')\n\
\
doc.set_cell_value(2, 16, 'string', '   I.   RESULTADOS NEGATIVOS DE EXPLOTACIÓN')\n\
doc.set_cell_value(2, 17, 'string', '        (A1+A2+A3+A4+A5+A6-B1-B2-B3)')\n\
doc.set_cell_value(3, 16, 'formula', '=if((Debe.C8+Debe.C12+Debe.C13+Debe.C16+Debe.C17+Debe.C18-C8-C13-C14)>0;Debe.C8+Debe.C12+Debe.C13+Debe.C16+Debe.C17+Debe.C18-C8-C13-C14;T(0))')\n\
doc.set_cell_value(4, 16, 'formula', '=if((Debe.D8+Debe.D12+Debe.D13+Debe.D16+Debe.D17+Debe.D18-D8-D13-D14)>0;Debe.D8+Debe.D12+Debe.D13+Debe.D16+Debe.D17+Debe.D18-D8-D13-D14;T(0))')\n\
\
doc.set_cell_value(2, 19, 'string', '       4. Ingresos financieros')\n\
doc.set_cell_value(3, 19, 'formula', '=SUM(C20:C23)')\n\
doc.set_cell_value(4, 19, 'formula', '=SUM(D20:D23)')\n\
\
doc.set_cell_value(2, 20, 'string', '            a) En entidades del grupo')\n\
doc.set_cell_value(3, 20, 'float', '" + ( - ( saldoCuenta ( 7600 ) + saldoCuenta ( 7610 ) + saldoCuenta ( 7620 ) + saldoCuenta ( 7630 ) + saldoCuenta ( 7650 ) ) ).toQString ( '.' ) + "')\n\
doc.set_cell_value(4, 20, 'float', '" + ( - ( saldoCuentaAnt ( 7600 ) + saldoCuentaAnt ( 7610 ) + saldoCuentaAnt ( 7620 ) + saldoCuentaAnt ( 7630 ) + saldoCuentaAnt ( 7650 ) ) ).toQString ( '.' ) + "')\n\
\
doc.set_cell_value(2, 21, 'string', '            b) En entidades asociadas')\n\
doc.set_cell_value(3, 21, 'float', '" + ( - ( saldoCuenta ( 7601 ) + saldoCuenta ( 7611 ) + saldoCuenta ( 7621 ) + saldoCuenta ( 7631 ) + saldoCuenta ( 7651 ) ) ).toQString ( '.' ) + "')\n\
doc.set_cell_value(4, 21, 'float', '" + ( - ( saldoCuentaAnt ( 7601 ) + saldoCuentaAnt ( 7611 ) + saldoCuentaAnt ( 7621 ) + saldoCuentaAnt ( 7631 ) + saldoCuentaAnt ( 7651 ) ) ).toQString ( '.' ) + "')\n\
\
doc.set_cell_value(2, 22, 'string', '            c) Otros')\n\
doc.set_cell_value(3, 22, 'float', '" + ( - ( saldoCuenta ( 7603 ) + saldoCuenta ( 7613 ) + saldoCuenta ( 7623 ) + saldoCuenta ( 7633 ) + saldoCuenta ( 7653 ) + saldoCuenta ( 769 ) ) ).toQString ( '.' ) + "')\n\
doc.set_cell_value(4, 22, 'float', '" + ( - ( saldoCuentaAnt ( 7603 ) + saldoCuentaAnt ( 7613 ) + saldoCuentaAnt ( 7623 ) + saldoCuentaAnt ( 7633 ) + saldoCuentaAnt ( 7653 ) + saldoCuentaAnt ( 769 ) ) ).toQString ( '.' ) + "')\n\
\
doc.set_cell_value(2, 23, 'string', '            d) Beneficios en inversiones financieras')\n\
doc.set_cell_value(3, 23, 'float', '" + ( - saldoCuenta ( 766 ) ).toQString ( '.' ) + "')\n\
doc.set_cell_value(4, 23, 'float', '" + ( - saldoCuentaAnt ( 766 ) ).toQString ( '.' ) + "')\n\
\
doc.set_cell_value(2, 24, 'string', '       5. Diferencias positivas de cambio')\n\
doc.set_cell_value(3, 24, 'float', '" + ( - saldoCuenta ( 768 ) ).toQString ( '.' ) + "')\n\
doc.set_cell_value(4, 24, 'float', '" + ( - saldoCuentaAnt ( 768 ) ).toQString ( '.' ) + "')\n\
\
doc.set_cell_value(2, 26, 'string', '   II.  RESULTADOS FINANCIEROS NEGATIVOS')\n\
doc.set_cell_value(2, 27, 'string', '        (A7+A8+A9-B4-B5)')\n\
doc.set_cell_value(3, 26, 'formula', '=if((Debe.C23+Debe.C28+Debe.C29-C19-C24)>0;Debe.C23+Debe.C28+Debe.C29-C19-C24;T(0))')\n\
doc.set_cell_value(4, 26, 'formula', '=if((Debe.D23+Debe.D28+Debe.D29-D19-D24)>0;Debe.D23+Debe.D28+Debe.D29-D19-D24;T(0))')\n\
\
doc.set_cell_value(2, 29, 'string', '   III. RESULTADOS NEGATIVOS DE LAS ACTIVIDADES ORDINARIAS')\n\
doc.set_cell_value(2, 30, 'string', '        (BI+BII-AI-AII)')\n\
doc.set_cell_value(3, 29, 'formula', '=if((C16+C26-Debe.C20-Debe.C30)>0;C16+C26-Debe.C20-Debe.C30;T(0))')\n\
doc.set_cell_value(4, 29, 'formula', '=if((D16+D26-Debe.D20-Debe.D30)>0;D16+D26-Debe.D20-Debe.D30;T(0))')\n\
\
doc.set_cell_value(2, 32, 'string', '       6. Beneficios en enajenacion de inmovilizado inmaterial, material y cartera de control')\n\
doc.set_cell_value(3, 32, 'float', '" + ( - ( saldoCuenta ( 770 ) + saldoCuenta ( 771 ) + saldoCuenta ( 772 ) + saldoCuenta ( 773 ) ) ).toQString ( '.' ) + "')\n\
doc.set_cell_value(4, 32, 'float', '" + ( - ( saldoCuentaAnt ( 770 ) + saldoCuentaAnt ( 771 ) + saldoCuentaAnt ( 772 ) + saldoCuentaAnt ( 773 ) ) ).toQString ( '.' ) + "')\n\
\
doc.set_cell_value(2, 33, 'string', '       7 Beneficios por operaciones con obligaciones propias')\n\
doc.set_cell_value(3, 33, 'float', '" + ( - saldoCuenta ( 774 ) ).toQString ( '.' ) + "')\n\
doc.set_cell_value(4, 33, 'float', '" + ( - saldoCuentaAnt ( 774 ) ).toQString ( '.' ) + "')\n\
\
doc.set_cell_value(2, 34, 'string', '       8 Subvenciones, donaciones y legados de capital y otros afectos a la actividad mercantil traspasados  al resultado del ejercicio')\n\
doc.set_cell_value(3, 34, 'float', '" + ( - saldoCuenta ( 775 ) ).toQString ( '.' ) + "')\n\
doc.set_cell_value(4, 34, 'float', '" + ( - saldoCuentaAnt ( 775 ) ).toQString ( '.' ) + "')\n\
\
doc.set_cell_value(2, 35, 'string', '       9. Ingresos extraordinarios')\n\
doc.set_cell_value(3, 35, 'float', '" + ( - saldoCuenta ( 778 ) ).toQString ( '.' ) + "')\n\
doc.set_cell_value(4, 35, 'float', '" + ( - saldoCuentaAnt ( 778 ) ).toQString ( '.' ) + "')\n\
\
doc.set_cell_value(2, 36, 'string', '      10. Ingresos y beneficios de otros ejercicios')\n\
doc.set_cell_value(3, 36, 'float', '" + ( - saldoCuenta ( 779 ) ).toQString ( '.' ) + "')\n\
doc.set_cell_value(4, 36, 'float', '" + ( - saldoCuentaAnt ( 779 ) ).toQString ( '.' ) + "')\n\
\
doc.set_cell_value(2, 38, 'string', '   IV. RESULTADOS EXTRAORDINARIOS NEGATIVOS')\n\
doc.set_cell_value(2, 39, 'string', '       (A10+A11+A12+A13+A14-B6-B7-B8-B9-B10)')\n\
doc.set_cell_value(3, 38, 'formula', '=if((Debe.C36+Debe.C37+Debe.C38+Debe.C39+Debe.C40-C32-C33-C34-C35-C36)>0;Debe.C36+Debe.C37+Debe.C38+Debe.C39+Debe.C40-C32-C33-C34-C35-C36;T(0))')\n\
doc.set_cell_value(4, 38, 'formula', '=if((Debe.D36+Debe.D37+Debe.D38+Debe.D39+Debe.D40-D32-D33-D34-D35-D36)>0;Debe.D36+Debe.D37+Debe.D38+Debe.D39+Debe.D40-D32-D33-D34-D35-D36;T(0))')\n\
\
doc.set_cell_value(2, 41, 'string', '   V.  RESULTADOS NEGATIVOS ANTES DE IMPUESTOS')\n\
doc.set_cell_value(2, 42, 'string', '       (BIII+BIV-AIII-AIV)')\n\
doc.set_cell_value(3, 41, 'formula', '=if((C29+C38-Debe.C33-Debe.C42)>0;C29+C38-Debe.C33-Debe.C42;T(0))')\n\
doc.set_cell_value(4, 41, 'formula', '=if((D29+D38-Debe.D33-Debe.D42)>0;D29+D38-Debe.D33-Debe.D42;T(0))')\n\
\
doc.set_cell_value(2, 44, 'string', '   VI. EXCEDENTE NEGATIVO DEL EJERCICIO (DESAHORRO)')\n\
doc.set_cell_value(2, 45, 'string', '       (BV+A15+A16)')\n\
doc.set_cell_value(3, 44, 'formula', '=if((C41+Debe.C48+Debe.C49)>0;C41+Debe.C48+Debe.C49;T(0))')\n\
doc.set_cell_value(4, 44, 'formula', '=if((D41+Debe.D48+Debe.D49)>0;D41+Debe.D48+Debe.D49;T(0))')\n\
\
\
\
\
\
# Guardamos el documento\n\
doc.save(\"canualesods.ods\")\n" );
                          }


