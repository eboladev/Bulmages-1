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

#include <QDateTime>
#include <QTextStream>

#include "gongimportfiles.h"

#define EURO                 166.386

#define LEN_CODIGO_CUENTA    12
#define LEN_TITULO           40
#define LEN_NIF              15
#define LEN_DOMICILIO        35
#define LEN_POBLACION        25
#define LEN_PROVINCIA        20
#define LEN_CODPOSTAL        5
#define LEN_DIVISA           1
#define LEN_CTA_CODDIVISA    5
#define LEN_CTA_DOCUMENTO    1
#define LEN_AJUSTAME         1
#define LEN_TIPOIVA          1


#define LEN_ASIEN            6
#define LEN_FECHA            8
#define LEN_SUBCTA           12
#define LEN_CONTRA           12
#define LEN_PTADEBE          16
#define LEN_CONCEPTO         25
#define LEN_PTAHABER         16
#define LEN_FACTURA          8
#define LEN_BASEIMPO         16
#define LEN_IVA              5
#define LEN_RECEQUIV         5
#define LEN_DOCUMENTO        10
#define LEN_DEPARTA          3
#define LEN_CLAVE            6
#define LEN_ESTADO           1
#define LEN_NCASADO          6
#define LEN_TCASADO          1
#define LEN_TRANS            6
#define LEN_CAMBIO           16
#define LEN_DEBEME           16
#define LEN_HABERME          16
#define LEN_AUXILIAR         1
#define LEN_SERIE            1
#define LEN_SUCURSAL         4
#define LEN_CODDIVISA        5
#define LEN_IMPAUXME         16
#define LEN_MONEDAUSO        1
#define LEN_EURODEBE         16
#define LEN_EUROHABER        16
#define LEN_BASEEURO         16
#define LEN_NOCONV           1
#define LEN_NUMEROINV        10


int Contaplus2Fugit(QFile &fugitfile, QFile &subcuentas, QFile &asientos) {
    QTextStream fugit(&fugitfile);

    fugit << "<?xml version=\"1.0\" encoding = \"iso-8859-1\"?>\n"
    "<!DOCTYPE FUGIT>\n"
    "<FUGIT version='0.3.1' origen='contaplus'"
    " date='" << QDate().toString(Qt::ISODate) << "'>\n";
    QString lopd_str, cuenta_str;

    /// Subcuentas.
    QTextStream stream(&subcuentas);
    while (!subcuentas.atEnd()) {
        QString line = stream.readLine();
        if (line.length() < 2) {
            break;
        } // end if
        int pos = 0;
        QString cod = line.mid(pos, LEN_CODIGO_CUENTA).stripWhiteSpace();
        pos += LEN_CODIGO_CUENTA;
        QString titulo = line.mid(pos, LEN_TITULO).stripWhiteSpace();
        pos += LEN_TITULO;
        QString nif = line.mid(pos, LEN_NIF).stripWhiteSpace();
        pos += LEN_NIF;
        QString domicilio = line.mid(pos, LEN_DOMICILIO).stripWhiteSpace();
        pos += LEN_DOMICILIO;
        QString poblacion = line.mid(pos, LEN_POBLACION).stripWhiteSpace();
        pos += LEN_POBLACION;
        QString provincia = line.mid(pos, LEN_PROVINCIA).stripWhiteSpace();
        pos += LEN_PROVINCIA;
        QString codpostal = line.mid(pos, LEN_CODPOSTAL).stripWhiteSpace();
        pos += LEN_CODPOSTAL;
        QString divisa = line.mid(pos, LEN_DIVISA).stripWhiteSpace();
        pos += LEN_DIVISA;
        QString cta_coddivisa = line.mid(pos, LEN_CTA_CODDIVISA).stripWhiteSpace();
        pos += LEN_CTA_CODDIVISA;
        QString cta_documento = line.mid(pos, LEN_CTA_DOCUMENTO).stripWhiteSpace();
        pos += LEN_CTA_DOCUMENTO;
        QString ajustame = line.mid(pos, LEN_AJUSTAME).stripWhiteSpace();
        pos += LEN_AJUSTAME;
        QString tipoiva = line.mid(pos, LEN_TIPOIVA).stripWhiteSpace();
        pos += LEN_TIPOIVA;

        if (!nif.isEmpty() || !domicilio.isEmpty() || !poblacion.isEmpty()
                || !provincia.isEmpty() || !codpostal.isEmpty()) {
            lopd_str += "<LOPD>\n"
                        "\t<FIELD name='cif'>" + nif + "</FIELD>\n"
                        "\t<FIELD name='nombre'>" + titulo + "</FIELD>\n"
                        "\t<FIELD name='direccion'>" + domicilio + "</FIELD>\n"
                        "\t<FIELD name='localidad'>" + poblacion + "</FIELD>\n"
                        "\t<FIELD name='provincia'>" + provincia + "</FIELD>\n"
                        "\t<FIELD name='cp'>" + codpostal + "</FIELD>\n"
                        "</LOPD>\n";
        } // end if
        if (!cod.isEmpty()) {
            cuenta_str += "<CUENTA>\n"
                          "\t<FIELD name='cuenta'>" + cod + "</FIELD>\n"
                          "\t<FIELD name='descripcion'>" + titulo + "</FIELD>\n"
                          "\t<FIELD name='cif'>" + nif + "</FIELD>\n"
                          "</CUENTA>\n";
        } // end if
    } // end while
    fugit << "<FICHERO_LOPD>\n";
    fugit <<  lopd_str ;
    fugit << "</FICHERO_LOPD>\n";
    fugit << "<FICHERO_CUENTA>\n";
    fugit <<  cuenta_str ;
    fugit << "</FICHERO_CUENTA>\n\n";

    /// Asientos.
    fugit << "<FICHERO_ASIENTO>\n";
    QTextStream stream2(&asientos);
    QString lastasiento;
    int napunte = 0;
    while (!asientos.atEnd()) {
        QString line = stream2.readLine();
        if (line.length() < 2) {
            break;
        } // end if
        int pos = 0;
        QString asiento = line.mid(pos, LEN_ASIEN).stripWhiteSpace();
        pos += LEN_ASIEN;
        QString fecha = line.mid(pos, LEN_FECHA).stripWhiteSpace();
        fecha = fecha.mid(0, 4) + "-" + fecha.mid(4, 2) + "-" + fecha.mid(6, 2);
        pos += LEN_FECHA;
        QString subcta = line.mid(pos, LEN_SUBCTA).stripWhiteSpace();
        pos += LEN_SUBCTA;
        QString contra = line.mid(pos, LEN_CONTRA).stripWhiteSpace();
        pos += LEN_CONTRA;
        QString ptadebe = line.mid(pos, LEN_PTADEBE).stripWhiteSpace();
        pos += LEN_PTADEBE;
        QString concepto = line.mid(pos, LEN_CONCEPTO).stripWhiteSpace();
        pos += LEN_CONCEPTO;
        QString ptahaber = line.mid(pos, LEN_PTAHABER).stripWhiteSpace();
        pos += LEN_PTAHABER;
        QString factura = line.mid(pos, LEN_FACTURA).stripWhiteSpace();
        pos += LEN_FACTURA;
        QString baseimpo = line.mid(pos, LEN_BASEIMPO).stripWhiteSpace();
        pos += LEN_BASEIMPO;
        QString iva = line.mid(pos, LEN_IVA).stripWhiteSpace();
        pos += LEN_IVA;
        QString recequiv = line.mid(pos, LEN_RECEQUIV).stripWhiteSpace();
        pos += LEN_RECEQUIV;
        QString documento = line.mid(pos, LEN_DOCUMENTO).stripWhiteSpace();
        pos += LEN_DOCUMENTO;
        QString departa = line.mid(pos, LEN_DEPARTA).stripWhiteSpace();
        pos += LEN_DEPARTA;
        QString clave = line.mid(pos, LEN_CLAVE).stripWhiteSpace();
        pos += LEN_CLAVE;
        QString estado = line.mid(pos, LEN_ESTADO).stripWhiteSpace();
        pos += LEN_ESTADO;
        QString ncasado = line.mid(pos, LEN_NCASADO).stripWhiteSpace();
        pos += LEN_NCASADO;
        QString tcasado = line.mid(pos, LEN_TCASADO).stripWhiteSpace();
        pos += LEN_TCASADO;
        QString trans = line.mid(pos, LEN_TRANS).stripWhiteSpace();
        pos += LEN_TRANS;
        QString cambio = line.mid(pos, LEN_CAMBIO).stripWhiteSpace();
        pos += LEN_CAMBIO;
        QString debeme = line.mid(pos, LEN_DEBEME).stripWhiteSpace();
        pos += LEN_DEBEME;
        QString haberme = line.mid(pos, LEN_HABERME).stripWhiteSpace();
        pos += LEN_HABERME;
        QString auxiliar = line.mid(pos, LEN_AUXILIAR).stripWhiteSpace();
        pos += LEN_AUXILIAR;
        QString serie = line.mid(pos, LEN_SERIE).stripWhiteSpace();
        pos += LEN_SERIE;
        QString sucursal = line.mid(pos, LEN_SUCURSAL).stripWhiteSpace();
        pos += LEN_SUCURSAL;
        QString coddivisa = line.mid(pos, LEN_CODDIVISA).stripWhiteSpace();
        pos += LEN_CODDIVISA;
        QString impauxme = line.mid(pos, LEN_IMPAUXME).stripWhiteSpace();
        pos += LEN_IMPAUXME;
        QString monedauso = line.mid(pos, LEN_MONEDAUSO).stripWhiteSpace();
        pos += LEN_MONEDAUSO;
        QString eurodebe = line.mid(pos, LEN_EURODEBE).stripWhiteSpace();
        pos += LEN_EURODEBE;
        QString eurohaber = line.mid(pos, LEN_EUROHABER).stripWhiteSpace();
        pos += LEN_EUROHABER;
        QString baseeuro = line.mid(pos, LEN_BASEEURO).stripWhiteSpace();
        pos += LEN_BASEEURO;
        QString noconv = line.mid(pos, LEN_NOCONV).stripWhiteSpace();
        pos += LEN_NOCONV;
        QString numeroinv = line.mid(pos, LEN_NUMEROINV).stripWhiteSpace();
        pos += LEN_NUMEROINV;

        if (asiento != lastasiento) {
            if (!lastasiento.isEmpty())
                fugit <<  "</ASIENTO>\n";
            fugit << "<ASIENTO>\n"
            "\t<FIELD name='numasiento'>" + asiento + "</FIELD>\n"
            "\t<FIELD name='fecha'>" + fecha + "</FIELD>\n"
            "\t<FIELD name='numdocumento'>" + documento + "</FIELD>\n"
            "\t<FIELD name='numdiario'>" + departa + "</FIELD>\n"
            "\t<FIELD name='punteo'>" + estado + "</FIELD>\n";
            napunte = 0;
            lastasiento = asiento;
        } // end if
        napunte++;
        fugit << "\t<APUNTE>\n"
        "\t\t<FIELD name='numapunte'>" + QString::number(napunte) + "</FIELD>\n"
        "\t\t<FIELD name='cuenta'>" + subcta + "</FIELD>\n"
        "\t\t<FIELD name='contrapartida'>" + contra + "</FIELD>\n"
        "\t\t<FIELD name='concepto'>" + concepto + "</FIELD>\n";
        if (monedauso == "1") { /// Ptas.
            fugit <<
            "\t\t<FIELD name='debe'>" + QString::number((ptadebe.toDouble()) / EURO) + "</FIELD>\n"
            "\t\t<FIELD name='haber'>" + QString::number((ptahaber.toDouble()) / EURO) + "</FIELD>\n";
        } else {
            fugit <<
            "\t\t<FIELD name='debe'>" + QString::number((eurodebe.toDouble()) ) + "</FIELD>\n"
            "\t\t<FIELD name='haber'>" + QString::number((eurohaber.toDouble()) ) + "</FIELD>\n";
        } // end if
        fugit << "\t</APUNTE>\n";
    } // end while
    if (!lastasiento.isEmpty()) {
        fugit << "</ASIENTO>\n";
    } // end if
    fugit << "</FICHERO_ASIENTO>\n";
    fugit << "</FUGIT>";

    return 1;
}

