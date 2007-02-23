/***************************************************************************
 *   Copyright (C) 2005 by Ricardo Diaz                                    *
 *   richard@galdi.es                                                      *
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

#include "arbol.h"


Arbol::Arbol() {
    unsigned int i;
    /// Una contabilidad podra tener hasta 70 cuentas de nivel 2 como mucho, creo.
    raiz = new tipohoja*[70];
    /// Vamos a hacer un recorrido por el array para inicializar los punteros a NULO.
    for (i = 0; i < 70; i++) {
        raiz[i] = NULL;
    }
}


Arbol::~Arbol() {
    unsigned int i;
    /// Vamos a eliminar toda la reserva de memoria din&aacute;mica ocupada.
    for (i = 0; i < 70; i++) {
        raiz[i] = NULL;
    }
    delete raiz;
    delete hoja;
    delete rama;
}


void Arbol::nuevarama(cursor2 *ramas) {
    unsigned int i;
    /// Rellenamos los valores de inicializacion para una hoja.
    hoja = new tipohoja;
    hoja->idcuenta = atoi(ramas->valor("idcuenta").toAscii().constData());
    hoja->codigo = QString(ramas->valor("codigo"));
    hoja->descripcion = QString(ramas->valor("descripcion"));
    hoja->saldoant = hoja->debe = hoja->haber = hoja->saldo = hoja->debeej = hoja->haberej = hoja->saldoej = 0.00;
    hoja->numapuntes = 0;
    hoja->ramas = NULL;
    /// Buscamos en que rama hay que poner la hoja.
    for(i = 0; i < 70 && raiz[i]; i++) {}
    raiz[i] = hoja;
}


void Arbol::inicializa(cursor2 *ramas) {
    unsigned int i;
    QString padre;
    tiporama* guia = NULL;
    ramas->primerregistro();
    /// Hay que construir el arbol con todas sus hojitas (cuentas).
    for(i = 0; raiz[i] && i < 70; i++) {
        padre = raiz[i]->codigo;
        /// Creamos las primeras hojas en la ra&iacute;z y sintetizamos el resto
        /// recursivamente (si es que existen aun m&aacute;s hojas hijas).
        while (!ramas->eof() && ramas->valor("codigo").startsWith(padre)) {
            if (ramas->valor("nivel").toInt() > 2) {
                /// Creamos una hoja.
                hoja = new tipohoja;
                hoja->idcuenta = atoi(ramas->valor("idcuenta").toAscii().constData());
                hoja->codigo = QString(ramas->valor("codigo"));
                hoja->descripcion = QString(ramas->valor("descripcion"));
                hoja->saldoant = hoja->debe = hoja->haber = hoja->saldo = hoja->debeej = hoja->haberej = hoja->saldoej = 0.00;
                hoja->numapuntes = 0;
                hoja->ramas = NULL;
                rama = new tiporama;
                rama->hoja = hoja;
                rama->sgte = NULL;
                /// Creamos la uni&oacute;n con la ra&iacute;z.
                if (!raiz[i]->ramas) {
                    raiz[i]->ramas = guia = rama;
                } else { /// Avanzamos el puntero a la siguiente supuesta rama.
                    guia->sgte = rama;
                    guia = guia->sgte;
                } // end if
                /// Buscamos si adem&aacute;s de esta hoja hay m&aacute;s ramas que
                /// crezcan, en tal caso habr&aacute; que sintetizarlas.
                SintetizarRamas(&ramas, &(hoja->ramas));
            } else {
                ramas->siguienteregistro();
            } // end if
        } // end while
    } // end for
}


void Arbol::SintetizarRamas(cursor2 **cuentas, tiporama **ramas) {
    tiporama *guia, *rama;
    tipohoja *hoja;
    int nivel;
    cursor2* ptrcuentas = *cuentas;
    nivel = atoi(ptrcuentas->valor("nivel").toAscii().constData());
    ptrcuentas->siguienteregistro();
    guia = NULL;
    while (!ptrcuentas->eof() && (atoi(ptrcuentas->valor("nivel").toAscii().constData()) > nivel)) {
        /// Reservamos un huequecito de memoria para almacenar los datos de la rama.
        rama = new tiporama;
        if (!guia) {
            *ramas = guia = rama;
        } else {
            guia->sgte = rama;
            guia = guia->sgte;
        }
        hoja = new tipohoja; /// Idem para una hojita.
        hoja->idcuenta = atoi(ptrcuentas->valor("idcuenta").toAscii().constData());
        hoja->codigo = ptrcuentas->valor("codigo");
        hoja->descripcion = ptrcuentas->valor("descripcion");
        hoja->saldoant = hoja->debe = hoja->haber = hoja->saldo = hoja->debeej = hoja->haberej = hoja->saldoej = 0.00;
        hoja->numapuntes = 0;
        hoja->ramas = NULL;
        guia->hoja = hoja;
        guia->sgte = NULL;
        /// Pero a&uacute;n puede haber m&aacute;s ramas que crezcan, en tal caso habr&aacute;
        /// que sintetizarlas
        SintetizarRamas(&ptrcuentas, &(hoja->ramas));
    } // end while
}


void Arbol::actualizahojas(cursor2 *cuenta) {
    unsigned int i = 0;
    bool actualizado;
    tipohoja *hojaraiz;

    QString cuentapadre = cuenta->valor("codigo").left(2);
    do {
        hojaraiz = raiz[i];
        i++;
    } while (i < 70 && cuentapadre != hojaraiz->codigo);
    actualizado = false;
    if (hojaraiz->ramas) {
        ActualizarHoja(&(hojaraiz->ramas), cuenta, &actualizado);
        if (actualizado) {
            hojaraiz->saldoant += cuenta->valor("saldoant").toDouble();
            hojaraiz->debe += cuenta->valor("debe").toDouble();
            hojaraiz->haber += cuenta->valor("haber").toDouble();
            hojaraiz->saldo += cuenta->valor("saldo").toDouble();
            hojaraiz->debeej += cuenta->valor("debeej").toDouble();
            hojaraiz->haberej += cuenta->valor("haberej").toDouble();
            hojaraiz->saldoej += cuenta->valor("saldoej").toDouble();
            hojaraiz->numapuntes += cuenta->valor("numapuntes").toInt();
        } // end if
    } // end if
}


void Arbol::ActualizarHoja(tiporama** ramaraiz, cursor2* cuenta, bool* actualizado) {
    tiporama* rama = *ramaraiz;
    /// Buscamos por cada una de las ramas.
    while (rama && !(*actualizado)) {
        int idcuenta = atoi(cuenta->valor("idcuenta").toAscii().constData());
        if (rama->hoja->idcuenta == idcuenta) {
            /// Ponemos los valores obtenidos de la BD.
            rama->hoja->saldoant = cuenta->valor("saldoant").toDouble();
            rama->hoja->debe = cuenta->valor("debe").toDouble();
            rama->hoja->haber = cuenta->valor("haber").toDouble();
            rama->hoja->saldo = cuenta->valor("saldo").toDouble();
            rama->hoja->debeej = cuenta->valor("debeej").toDouble();
            rama->hoja->haberej = cuenta->valor("haberej").toDouble();
            rama->hoja->saldoej = cuenta->valor("saldoej").toDouble();
            rama->hoja->numapuntes = cuenta->valor("numapuntes").toInt();
            *actualizado = true;
        } else {
            if (rama->hoja->ramas) {
                ActualizarHoja(&(rama->hoja->ramas), cuenta, &(*actualizado));
                /// A la vuelta, actualizamos los valores si alguna hoja fue actualizada.
                if (*actualizado) {
                    rama->hoja->saldoant += cuenta->valor("saldoant").toDouble();
                    rama->hoja->debe += cuenta->valor("debe").toDouble();
                    rama->hoja->haber += cuenta->valor("haber").toDouble();
                    rama->hoja->saldo += cuenta->valor("saldo").toDouble();
                    rama->hoja->debeej += cuenta->valor("debeej").toDouble();
                    rama->hoja->haberej += cuenta->valor("haberej").toDouble();
                    rama->hoja->saldoej += cuenta->valor("saldoej").toDouble();
                    rama->hoja->numapuntes += cuenta->valor("numapuntes").toInt();
                } // end if
            }
        } // end if
        rama = rama->sgte;
    } // end while
}


void Arbol::inicia() {
    /// Siempre ser&aacute; la primera del arbol.
    hojaactiva = QString("00");
}


bool Arbol::deshoja(unsigned int nivel, bool superiores) {
    unsigned int i;
    bool deshojada = false;
    if (nivel > 2) {
        /// Primero averiguamos en qu&eacute; ra&iacute;z deshojamos la &uacute;ltima
        /// vez para continuar por ah&iacute;.
        QString raizcandidata = hojaactiva.left(2);
        if (hojaactiva == QString("00"))
            i = 0;
        else {
            for(i = 0; i < 70 && raiz[i]; i++) {
                if (raiz[i]->codigo == raizcandidata)
                    break;
            } // end for
        } // end if
        /// Miramos si hay hoja candidata que deshojar.
        while (!deshojada && raiz[i]) {
            /// Si hay que sacar los niveles superiores tenemos que deshojar tambi&eacute;n
            /// en el nivel 2.
            if (superiores && raiz[i]->codigo > hojaactiva && raiz[i]->numapuntes > 0) {
                hoja = raiz[i];
                /// S&oacute;lo deshojamos si ha habido movimientos en la cuenta durante
                /// el ejercicio.
                if (hoja->numapuntes > 0) {
                    hojaactiva = hoja->codigo;
                    deshojada = true;
                }
            } else {
                if (raiz[i]->ramas)
                    /// Buscamos por las hojas hijas.
                    Deshojar(raiz[i]->ramas, nivel, superiores, &deshojada);
            } // end if
            i++;
        } // end while
    } else {
        if (hojaactiva == QString("00")) {
            if (raiz[0]) {
                hojaactiva = raiz[0]->codigo;
                hoja = raiz[0];
                deshojada = true;
            }
        } else {
            for (i = 0; i < 70 && raiz[i]; i++) {
                if(raiz[i]->codigo > hojaactiva) {
                    hoja = raiz[i];
                    /// S&oacute;lo deshojamos si ha habido movimientos en la cuenta
                    /// durante el ejercicio.
                    if (hoja->numapuntes > 0) {
                        hojaactiva = hoja->codigo;
                        deshojada = true;
                        break;
                    }
                } // end if
            } // end for
        } // end if
    } // end if
    return deshojada;
}


void Arbol::Deshojar(tiporama *rama, unsigned int nivel, bool superiores, bool *deshojada) {
    unsigned int nivelhoja = rama->hoja->codigo.length();
    if (hojaactiva >= rama->hoja->codigo) {
        if (rama->hoja->ramas && nivelhoja < nivel)
            Deshojar(rama->hoja->ramas, nivel, superiores, &(*deshojada));
        if (rama->sgte && !(*deshojada))
            Deshojar(rama->sgte, nivel, superiores, &(*deshojada));
    } else {
        /// No deshojaremos por aqu&iacute;, busquemos por otra rama.
        if (rama->hoja->numapuntes == 0) {
            if (rama->sgte)
                Deshojar(rama->sgte, nivel, superiores, &(*deshojada));
        } else { /// Deshojamos
            if (superiores) {
                if (nivelhoja <= nivel) {
                    hoja = rama->hoja;
                    hojaactiva = hoja->codigo;
                    *deshojada = true;
                } else {
                    if (rama->sgte)
                        /// Puede ser que hojas del mismo nivel en el arbol no sean del
                        /// mismo nivel de cuentas.
                        /// Ej: 4750001, 4751 y 4752 podrian ser hojas hermanas porque no
                        /// exista un 4750
                        /// En este caso, recorreremos tambien sus hojas hermanas por si
                        /// acaso.
                        Deshojar(rama->sgte, nivel, superiores, &(*deshojada));
                }
            } else {
                if (nivelhoja == nivel) {
                    hoja = rama->hoja;
                    hojaactiva = hoja->codigo;
                    *deshojada = true;
                } else {
                    if (rama->hoja->ramas && nivelhoja < nivel)
                        Deshojar(rama->hoja->ramas, nivel, superiores, &(*deshojada));
                    if (rama->sgte && !(*deshojada))
                        Deshojar(rama->sgte, nivel, superiores, &(*deshojada));
                } // end if
            } // end if
        } // end if
    } // end if
}


QString Arbol::hojaactual(QString valor) {
    QString resultado;
    if (valor == "saldoant")
        resultado.setNum(hoja->saldoant,'f', 2);
    if (valor == "debe" )
        resultado.setNum(hoja->debe,'f', 2);
    if (valor == "haber" )
        resultado.setNum(hoja->haber,'f', 2);
    if (valor == "saldo" )
        resultado.setNum(hoja->saldo,'f', 2);
    if (valor == "debeej" )
        resultado.setNum(hoja->debeej,'f', 2);
    if (valor == "haberej" )
        resultado.setNum(hoja->haberej,'f', 2);
    if (valor == "saldoej" )
        resultado.setNum(hoja->saldoej,'f', 2);
    if (valor == "codigo" )
        resultado = hoja->codigo;
    if (valor == "descripcion" )
        resultado = hoja->descripcion;
    return resultado;
}

bool Arbol::irHoja(QString codigo, unsigned int nivel) {
    hojaactiva = QString("00");
    while(deshoja(nivel,true) && hojaactual("codigo") != codigo);
    if(hojaactual("codigo") == codigo)
	return true;
    else
	return false;
}
