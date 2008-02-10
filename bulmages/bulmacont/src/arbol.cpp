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


///
/**
**/
Arbol::Arbol()
{
    _depura ( "Arbol::Arbol", 0 );
    unsigned int i;
    /// Una contabilidad podra tener hasta 70 cuentas de nivel 2 como mucho, creo.
    raiz = new tipohoja * [70];
    /// Vamos a hacer un recorrido por el array para inicializar los punteros a NULO.
    for ( i = 0; i < 70; i++ ) {
        raiz[i] = NULL;
    } // end for
    _depura ( "END Arbol::Arbol", 0 );
}


///
/**
**/
Arbol::~Arbol()
{
    _depura ( "Arbol::~Arbol", 0 );
    unsigned int i;
    /// Vamos a eliminar toda la reserva de memoria din&aacute;mica ocupada.
    for ( i = 0; i < 70; i++ ) {
        raiz[i] = NULL;
    }
    delete raiz;
    delete hoja;
    delete rama;
    _depura ( "END Arbol::~Arbol", 0 );
}


///
/**
\param ramas
**/
void Arbol::nuevarama ( cursor2 *ramas )
{
    _depura ( "Arbol::nuevarama", 0 );
    unsigned int i;
    /// Rellenamos los valores de inicializacion para una hoja.
    hoja = new tipohoja;
    hoja->idcuenta = atoi ( ramas->valor ( "idcuenta" ).toAscii().constData() );
    hoja->codigo = QString ( ramas->valor ( "codigo" ) );
    hoja->descripcion = QString ( ramas->valor ( "descripcion" ) );
    hoja->saldoant = hoja->debe = hoja->haber = hoja->saldo = hoja->debeej = hoja->haberej = hoja->saldoej = Fixed ( "0.00" );
    hoja->numapuntes = 0;
    hoja->ramas = NULL;
    /// Buscamos en que rama hay que poner la hoja.
    for ( i = 0; i < 70 && raiz[i]; i++ ) {}
    raiz[i] = hoja;
    _depura ( "END Arbol::nuevarama", 0 );
}


///
/**
\param ramas
**/
void Arbol::inicializa ( cursor2 *ramas )
{
    _depura ( "Arbol::inicializa", 0 );
    unsigned int i;
    QString padre;
    tiporama* guia = NULL;
    ramas->primerregistro();
    /// Hay que construir el arbol con todas sus hojitas (cuentas).
    for ( i = 0; raiz[i] && i < 70; i++ ) {
        padre = raiz[i]->codigo;
        /// Creamos las primeras hojas en la ra&iacute;z y sintetizamos el resto
        /// recursivamente (si es que existen aun m&aacute;s hojas hijas).
        while ( !ramas->eof() && ramas->valor ( "codigo" ).startsWith ( padre ) ) {
            if ( ramas->valor ( "nivel" ).toInt() > 2 ) {
                /// Creamos una hoja.
                hoja = new tipohoja;
                hoja->idcuenta = atoi ( ramas->valor ( "idcuenta" ).toAscii().constData() );
                hoja->codigo = QString ( ramas->valor ( "codigo" ) );
                hoja->descripcion = QString ( ramas->valor ( "descripcion" ) );
                hoja->saldoant = hoja->debe = hoja->haber = hoja->saldo = hoja->debeej = hoja->haberej = hoja->saldoej = Fixed ( "0.00" );
                hoja->numapuntes = 0;
                hoja->ramas = NULL;
                rama = new tiporama;
                rama->hoja = hoja;
                rama->sgte = NULL;
                /// Creamos la uni&oacute;n con la ra&iacute;z.
                if ( !raiz[i]->ramas ) {
                    raiz[i]->ramas = guia = rama;
                } else { /// Avanzamos el puntero a la siguiente supuesta rama.
                    guia->sgte = rama;
                    guia = guia->sgte;
                } // end if
                /// Buscamos si adem&aacute;s de esta hoja hay m&aacute;s ramas que
                /// crezcan, en tal caso habr&aacute; que sintetizarlas.
                SintetizarRamas ( &ramas, & ( hoja->ramas ) );
            } else {
                ramas->siguienteregistro();
            } // end if
        } // end while
    } // end for
    _depura ( "END Arbol::inicializa", 0 );
}


///
/**
\param cuentas
\param ramas
**/
void Arbol::SintetizarRamas ( cursor2 **cuentas, tiporama **ramas )
{
    _depura ( "Arbol::SintetizarRamas", 0 );
    tiporama *guia, *rama;
    tipohoja *hoja;
    int nivel;
    cursor2* ptrcuentas = *cuentas;
    nivel = atoi ( ptrcuentas->valor ( "nivel" ).toAscii().constData() );
    ptrcuentas->siguienteregistro();
    guia = NULL;
    while ( !ptrcuentas->eof() && ( atoi ( ptrcuentas->valor ( "nivel" ).toAscii().constData() ) > nivel ) ) {
        /// Reservamos un huequecito de memoria para almacenar los datos de la rama.
        rama = new tiporama;
        if ( !guia ) {
            *ramas = guia = rama;
        } else {
            guia->sgte = rama;
            guia = guia->sgte;
        } // end if
        hoja = new tipohoja; /// Idem para una hojita.
        hoja->idcuenta = atoi ( ptrcuentas->valor ( "idcuenta" ).toAscii().constData() );
        hoja->codigo = ptrcuentas->valor ( "codigo" );
        hoja->descripcion = ptrcuentas->valor ( "descripcion" );
        hoja->saldoant = hoja->debe = hoja->haber = hoja->saldo = hoja->debeej = hoja->haberej = hoja->saldoej = Fixed ( "0.00" );
        hoja->numapuntes = 0;
        hoja->ramas = NULL;
        guia->hoja = hoja;
        guia->sgte = NULL;
        /// Pero a&uacute;n puede haber m&aacute;s ramas que crezcan, en tal caso habr&aacute;
        /// que sintetizarlas
        SintetizarRamas ( &ptrcuentas, & ( hoja->ramas ) );
    } // end while
    _depura ( "END Arbol::SintetizarRamas", 0 );
}


///
/**
\param cuenta
**/
void Arbol::actualizahojas ( cursor2 *cuenta )
{
    _depura ( "Arbol::actualizahojas", 0 );
    unsigned int i = 0;
    bool actualizado, condition = true;
    tipohoja *hojaraiz;

    QString cuentapadre = cuenta->valor ( "codigo" ).left ( 2 );
    /// Buscamos la rama del arbol que es la padre la cuenta pasada.
    do {
        hojaraiz = raiz[i];
        if(i <70)
          i++;
        else
          condition = false;
        if(condition)
          if(cuentapadre != hojaraiz->codigo)
            condition = false;
    } while ( condition );
    actualizado = false;

    /// Si la rama encontrada tiene hijos se actualiza
    if ( hojaraiz->ramas ) {
        ActualizarHoja ( & ( hojaraiz->ramas ), cuenta, &actualizado );
        if ( actualizado ) {
            hojaraiz->saldoant = hojaraiz->saldoant + Fixed ( cuenta->valor ( "saldoant" ) );
            hojaraiz->debe = hojaraiz->debe + Fixed ( cuenta->valor ( "debe" ) );
            hojaraiz->haber = hojaraiz->haber + Fixed ( cuenta->valor ( "haber" ) );
            hojaraiz->saldo = hojaraiz->saldo + Fixed ( cuenta->valor ( "saldo" ) );
            hojaraiz->debeej = hojaraiz->debeej + Fixed ( cuenta->valor ( "debeej" ) );
            hojaraiz->haberej = hojaraiz->haberej + Fixed ( cuenta->valor ( "haberej" ) );
            hojaraiz->saldoej = hojaraiz->saldoej + Fixed ( cuenta->valor ( "saldoej" ) );
            hojaraiz->numapuntes += cuenta->valor ( "numapuntes" ).toInt();
        } // end if
    } // end if
    _depura ( "END Arbol::actualizahojas", 0 );
}


///
/**
\param ramaraiz
\param cuenta
\param actualizado
**/
void Arbol::ActualizarHoja ( tiporama** ramaraiz, cursor2* cuenta, bool* actualizado )
{
    _depura ( "Arbol::ActualizarHoja", 0 );


    tiporama* rama = *ramaraiz;

    int idcuenta = cuenta->valor ( "idcuenta" ).toInt();

    /// Buscamos por cada una de las ramas.
    while ( rama && ! ( *actualizado ) ) {

        if ( rama->hoja->idcuenta == idcuenta ) {
            /// Ponemos los valores obtenidos de la BD.
            rama->hoja->saldoant = Fixed ( cuenta->valor ( "saldoant" ) );
            rama->hoja->debe = Fixed ( cuenta->valor ( "debe" ) );
            rama->hoja->haber = Fixed ( cuenta->valor ( "haber" ) );
            rama->hoja->saldo = Fixed ( cuenta->valor ( "saldo" ) );
            rama->hoja->debeej = Fixed ( cuenta->valor ( "debeej" ) );
            rama->hoja->haberej = Fixed ( cuenta->valor ( "haberej" ) );
            rama->hoja->saldoej = Fixed ( cuenta->valor ( "saldoej" ) );
            rama->hoja->numapuntes = cuenta->valor ( "numapuntes" ).toInt();
            _depura ( "Arbol::ActualizarHoja", 2, cuenta->valor("codigo") + " " + QString::number(rama->hoja->numapuntes) );

            *actualizado = true;
        } else {
            if ( rama->hoja->ramas ) {
                ActualizarHoja ( & ( rama->hoja->ramas ), cuenta, & ( *actualizado ) );
                /// A la vuelta, actualizamos los valores si alguna hoja fue actualizada.
                if ( *actualizado ) {
                    rama->hoja->saldoant = rama->hoja->saldoant + Fixed ( cuenta->valor ( "saldoant" ) );
                    rama->hoja->debe = rama->hoja->debe + Fixed ( cuenta->valor ( "debe" ) );
                    rama->hoja->haber = rama->hoja->haber + Fixed ( cuenta->valor ( "haber" ) );
                    rama->hoja->saldo = rama->hoja->saldo + Fixed ( cuenta->valor ( "saldo" ) );
                    rama->hoja->debeej = rama->hoja->debeej + Fixed ( cuenta->valor ( "debeej" ) );
                    rama->hoja->haberej = rama->hoja->haberej + Fixed ( cuenta->valor ( "haberej" ) );
                    rama->hoja->saldoej = rama->hoja->saldoej + Fixed ( cuenta->valor ( "saldoej" ) );
                    rama->hoja->numapuntes += cuenta->valor ( "numapuntes" ).toInt();
                } // end if
            } // end if
        } // end if
        rama = rama->sgte;
    } // end while
    _depura ( "END Arbol::ActualizarHoja", 0 );
}


///
/**
**/
void Arbol::inicia()
{
    _depura ( "Arbol::inicia", 0 );
    /// Siempre ser&aacute; la primera del arbol.
    hojaactiva = QString ( "00" );
    _depura ( "END Arbol::inicia", 0 );
}


/// Busca el Siguiente elemento que no sea descartable.
/**
\param nivel
\param superiores
\return
**/
bool Arbol::deshoja ( unsigned int nivel, bool superiores )
{
    _depura ( "Arbol::deshoja", 0 );
    unsigned int i;
    bool deshojada = false;
    if ( nivel > 2 ) {
        /// Primero averiguamos en qu&eacute; ra&iacute;z deshojamos la &uacute;ltima
        /// vez para continuar por ah&iacute;.
        QString raizcandidata = hojaactiva.left ( 2 );
        if ( hojaactiva == QString ( "00" ) )
            i = 0;
        else {
            for ( i = 0; i < 70 && raiz[i]; i++ ) {
                if ( raiz[i]->codigo == raizcandidata )
                    break;
            } // end for
        } // end if
        /// Miramos si hay hoja candidata que deshojar.
        while ( !deshojada && raiz[i] ) {
            /// Si hay que sacar los niveles superiores tenemos que deshojar tambi&eacute;n
            /// en el nivel 2.
            if ( superiores && raiz[i]->codigo > hojaactiva && raiz[i]->numapuntes > 0 ) {
                hoja = raiz[i];
                /// S&oacute;lo deshojamos si ha habido movimientos en la cuenta durante
                /// el ejercicio.
                if ( hoja->numapuntes > 0 ) {
                    hojaactiva = hoja->codigo;
                    deshojada = true;
                } // end if
            } else {
                if ( raiz[i]->ramas )
                    /// Buscamos por las hojas hijas.
                    Deshojar ( raiz[i]->ramas, nivel, superiores, &deshojada );
            } // end if
            i++;
        } // end while
    } else {
        if ( hojaactiva == QString ( "00" ) ) {
            if ( raiz[0] ) {
                hojaactiva = raiz[0]->codigo;
                hoja = raiz[0];
                deshojada = true;
            } // end if
        } else {
            for ( i = 0; i < 70 && raiz[i]; i++ ) {
                if ( raiz[i]->codigo > hojaactiva ) {
                    hoja = raiz[i];
                    /// S&oacute;lo deshojamos si ha habido movimientos en la cuenta
                    /// durante el ejercicio.
                    if ( hoja->numapuntes > 0 ) {
                        hojaactiva = hoja->codigo;
                        deshojada = true;
                        break;
                    }
                } // end if
            } // end for
        } // end if
    } // end if
    return deshojada;
    _depura ( "END Arbol::deshoja", 0 );
}


///
/**
\param rama
\param nivel
\param superiores
\param deshojada
**/
void Arbol::Deshojar ( tiporama *rama, unsigned int nivel, bool superiores, bool *deshojada )
{
    _depura ( "Arbol::Deshojar", 0 );
    unsigned int nivelhoja = rama->hoja->codigo.length();
    if ( hojaactiva >= rama->hoja->codigo ) {
        if ( rama->hoja->ramas && nivelhoja < nivel )
            Deshojar ( rama->hoja->ramas, nivel, superiores, & ( *deshojada ) );
        if ( rama->sgte && ! ( *deshojada ) )
            Deshojar ( rama->sgte, nivel, superiores, & ( *deshojada ) );
    } else {
        /// No deshojaremos por aqu&iacute;, busquemos por otra rama.
        if ( rama->hoja->numapuntes == 0 ) {
            if ( rama->sgte )
                Deshojar ( rama->sgte, nivel, superiores, & ( *deshojada ) );
        } else { /// Deshojamos
            if ( superiores ) {
                if ( nivelhoja <= nivel ) {
                    hoja = rama->hoja;
                    hojaactiva = hoja->codigo;
                    *deshojada = true;
                } else {
                    if ( rama->sgte )
                        /// Puede ser que hojas del mismo nivel en el arbol no sean del
                        /// mismo nivel de cuentas.
                        /// Ej: 4750001, 4751 y 4752 podrian ser hojas hermanas porque no
                        /// exista un 4750
                        /// En este caso, recorreremos tambien sus hojas hermanas por si
                        /// acaso.
                        Deshojar ( rama->sgte, nivel, superiores, & ( *deshojada ) );
                } // end if
            } else {
                if ( nivelhoja == nivel ) {
                    hoja = rama->hoja;
                    hojaactiva = hoja->codigo;
                    *deshojada = true;
                } else {
                    if ( rama->hoja->ramas && nivelhoja < nivel )
                        Deshojar ( rama->hoja->ramas, nivel, superiores, & ( *deshojada ) );
                    if ( rama->sgte && ! ( *deshojada ) )
                        Deshojar ( rama->sgte, nivel, superiores, & ( *deshojada ) );
                } // end if
            } // end if
        } // end if
    } // end if
    _depura ( "END Arbol::Deshojar", 0 );
}


///
/**
\param valor
\return
**/
QString Arbol::hojaactual ( QString valor )
{
    _depura ( "Arbol::hojaactual", 0 );
    QString resultado;
    if ( valor == "saldoant" )
        resultado = hoja->saldoant.toQString();
    else if ( valor == "debe" )
        resultado = hoja->debe.toQString();
    else if ( valor == "haber" )
        resultado = hoja->haber.toQString();
    else if ( valor == "saldo" )
        resultado = hoja->saldo.toQString();
    else if ( valor == "debeej" )
        resultado = hoja->debeej.toQString();
    else if ( valor == "haberej" )
        resultado = hoja->haberej.toQString();
    else if ( valor == "saldoej" )
        resultado = hoja->saldoej.toQString();
    else if ( valor == "codigo" )
        resultado = hoja->codigo;
    else if ( valor == "descripcion" )
        resultado = hoja->descripcion;
    else if ( valor == "idcuenta" )
        resultado.setNum ( hoja->idcuenta );
    else
        _depura ( "Campo " + valor + " no encontrado en la hoja del arbol", 2 );
    return resultado;
    _depura ( "END Arbol::hojaactual", 0 );
}


///
/**
\param codigo
\param nivel
\return
**/
bool Arbol::irHoja ( QString codigo, unsigned int nivel )
{
    _depura ( "Arbol::irHoja", 0 );
    hojaactiva = QString ( "00" );
    while ( deshoja ( nivel, true ) && hojaactual ( "codigo" ) != codigo );
    if ( hojaactual ( "codigo" ) == codigo ) {
        return true;
    } else {
        return false;
    } // end if
    _depura ( "END Arbol::irHoja", 0 );
}

