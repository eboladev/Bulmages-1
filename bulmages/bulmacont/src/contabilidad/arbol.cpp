//
// C++ Implementation: arbol
//
// Description: 
//
//
// Author: Ricardo Di­az <richard@galdi.es>, (C) 2005
//
// Copyright: See COPYING file that comes with this distribution
//
//
/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#include "arbol.h"

Arbol::Arbol()
{
    unsigned int i;
    
    raiz = new tipohoja*[70]; // una contabilidad podra tener hasta 70 cuentas de nivel 2 como mucho, creo
    for(i=0; i<70; i++){ // Vamos a hacer un recorrido por el array para inicializar los punteros a NULO
	raiz[i] = NULL;
    }
}

Arbol::~Arbol()
{
    unsigned int i;
    
    // Vamos a eliminar toda la reserva de memoria dinamica ocupada
    for(i=0; i<70; i++){
	raiz[i] = NULL;
    }
    delete raiz;
    delete hoja;
    delete rama;
}

void Arbol::nuevarama(cursor2* ramas)
{
    unsigned int i;
    
    // rellenamos los valores de inicializacion para una hoja
    hoja = new tipohoja;
    hoja->idcuenta = atoi(ramas->valor("idcuenta").ascii());
    hoja->codigo = QString(ramas->valor("codigo"));
    hoja->descripcion = QString(ramas->valor("descripcion"));
    hoja->saldoant= hoja->debe= hoja->haber= hoja->saldo= hoja->debeej= hoja->haberej= hoja->saldoej= 0.00;
    hoja->numapuntes = 0;
    hoja->ramas = NULL;
    // buscamos en que rama hay que poner la hoja
    for(i=0; i<70 && raiz[i]; i++){}
    raiz[i] = hoja;
}

void Arbol::inicializa(cursor2* ramas)
{
    unsigned int i;
    QString padre;
    tiporama* guia = NULL;
    
    ramas->primerregistro();
    // Hay que construir el arbol con todas sus hojitas (cuentas)
    for(i=0; raiz[i] && i<70; i++){
	padre = raiz[i]->codigo;
	// Creamos las primeras hojas en la raiz y sintetizamos el resto recursivamente (si es que existen aun mas hojas hijas)
	while(!ramas->eof() && ramas->valor("codigo").startsWith(padre)){ 
	    if(ramas->valor("nivel").toInt() > 2){
		// Creamos una hoja
		hoja = new tipohoja;
		hoja->idcuenta = atoi(ramas->valor("idcuenta").ascii());
		hoja->codigo = QString(ramas->valor("codigo"));
		hoja->descripcion = QString(ramas->valor("descripcion"));
		hoja->saldoant = hoja->debe = hoja->haber = hoja->saldo = hoja->debeej = hoja->haberej = hoja->saldoej = 0.00;
		hoja->numapuntes = 0;
		hoja->ramas = NULL;
		rama = new tiporama;
		rama->hoja = hoja;
		rama->sgte = NULL;
		if(!raiz[i]->ramas){ // Creamos la union con la raiz
		    raiz[i]->ramas = guia = rama;
		}
		else{ // Avanzamos el puntero a la sgte. supuesta rama
		    guia->sgte = rama;
		    guia = guia->sgte;
		}// end if
		
		// Buscamos si ademas de esta hoja hay mas ramas que crezcan, en tal caso habra que sintetizarlas
		SintetizarRamas(&ramas, &(hoja->ramas));
	    }
	    else{
		ramas->siguienteregistro();
	    }// end if
	}// end while
    }// end for
}

void Arbol::SintetizarRamas(cursor2** cuentas, tiporama** ramas)
{
    tiporama *guia, *rama;
    tipohoja *hoja;
    int nivel;
    cursor2* ptrcuentas = *cuentas;
    
    nivel = atoi(ptrcuentas->valor("nivel").ascii());
    ptrcuentas->siguienteregistro();
    guia = NULL;
    while( !ptrcuentas->eof() && (atoi(ptrcuentas->valor("nivel").ascii()) > nivel) ){
	rama = new tiporama; // Reservamos un huequecito de memoria para almacenar los datos de la rama
	if(!guia){
	    *ramas = guia = rama;
	}
	else{
    	    guia->sgte = rama;
	    guia = guia->sgte;
	}
	hoja = new tipohoja; // Idem para una hojita
	hoja->idcuenta = atoi(ptrcuentas->valor("idcuenta").ascii());
	hoja->codigo = ptrcuentas->valor("codigo");
	hoja->descripcion = ptrcuentas->valor("descripcion");
	hoja->saldoant = hoja->debe = hoja->haber = hoja->saldo = hoja->debeej = hoja->haberej = hoja->saldoej = 0.00;
	hoja->numapuntes = 0;
	hoja->ramas = NULL;
	guia->hoja = hoja;
	guia->sgte = NULL;
	// Pero aun puede haber mas ramas que crezcan, en tal caso habra que sintetizarlas
	SintetizarRamas(&ptrcuentas, &(hoja->ramas));
    } // end while
}

void Arbol::actualizahojas(cursor2* cuenta)
{
    unsigned int i=0;
    bool actualizado;
    tipohoja* hojaraiz;
    
    QString cuentapadre = cuenta->valor("codigo").left(2);
    do{
	hojaraiz = raiz[i];
	i++;
    }while(i<70 && cuentapadre != hojaraiz->codigo);
    actualizado = false;
    if(hojaraiz->ramas){
	ActualizarHoja(&(hojaraiz->ramas), cuenta, &actualizado);
	if(actualizado){
	    hojaraiz->saldoant += cuenta->valor("saldoant").toDouble();
	    hojaraiz->debe += cuenta->valor("debe").toDouble();
	    hojaraiz->haber += cuenta->valor("haber").toDouble();
	    hojaraiz->saldo += cuenta->valor("saldo").toDouble();
	    hojaraiz->debeej += cuenta->valor("debeej").toDouble();
	    hojaraiz->haberej += cuenta->valor("haberej").toDouble();
	    hojaraiz->saldoej += cuenta->valor("saldoej").toDouble();	
	    hojaraiz->numapuntes += cuenta->valor("numapuntes").toInt();	
	}// end if
    }// end if
}

void Arbol::ActualizarHoja(tiporama** ramaraiz, cursor2* cuenta, bool* actualizado)
{
    tiporama* rama = *ramaraiz;

    while(rama && !(*actualizado)){	// Buscamos por cada una de las ramas
	int idcuenta = atoi(cuenta->valor("idcuenta").ascii());
	if(rama->hoja->idcuenta == idcuenta){
	    // Ponemos los valores obtenidos de la BD
	    rama->hoja->saldoant = cuenta->valor("saldoant").toDouble();
	    rama->hoja->debe = cuenta->valor("debe").toDouble();
	    rama->hoja->haber = cuenta->valor("haber").toDouble();
	    rama->hoja->saldo = cuenta->valor("saldo").toDouble();
	    rama->hoja->debeej = cuenta->valor("debeej").toDouble();
	    rama->hoja->haberej = cuenta->valor("haberej").toDouble();
	    rama->hoja->saldoej = cuenta->valor("saldoej").toDouble();	
	    rama->hoja->numapuntes = cuenta->valor("numapuntes").toInt();
	    *actualizado = true;
	}
	else{
	    if(rama->hoja->ramas){
		ActualizarHoja(&(rama->hoja->ramas), cuenta, &(*actualizado));
		// A la vuelta, actualizamos los valores si alguna hoja fue actualizada
		if(*actualizado){
		    rama->hoja->saldoant += cuenta->valor("saldoant").toDouble();
		    rama->hoja->debe += cuenta->valor("debe").toDouble();
		    rama->hoja->haber += cuenta->valor("haber").toDouble();
		    rama->hoja->saldo += cuenta->valor("saldo").toDouble();
		    rama->hoja->debeej += cuenta->valor("debeej").toDouble();
		    rama->hoja->haberej += cuenta->valor("haberej").toDouble();
		    rama->hoja->saldoej += cuenta->valor("saldoej").toDouble();
		    rama->hoja->numapuntes += cuenta->valor("numapuntes").toInt();
		}// end if
	    }
	}// end if
	rama = rama->sgte;
    }// end while
}

void Arbol::inicia(){
    hojaactiva = QString("00");	// Siempre sera la primera del arbol
}

bool Arbol::deshoja(unsigned int nivel, bool superiores){
    unsigned int i;
    bool deshojada = false;

    if(nivel > 2){
	// Primero, averiguamos en que raiz deshojamos la ultima vez para continuar por ahi
	QString raizcandidata = hojaactiva.left(2);
	if(hojaactiva == QString("00"))
	    i=0;
	else{
	    for(i=0; i<70 && raiz[i]; i++){
		if(raiz[i]->codigo == raizcandidata)
		    break;
	    }// end for
	}// end if
	
	// Miramos si hay hoja candidata que deshojar
	while(!deshojada && raiz[i]){
	    // Si hay que sacar los niveles superiores tenemos que deshojar tambien en el nivel 2
	    if(superiores && raiz[i]->codigo > hojaactiva && raiz[i]->numapuntes > 0){
		hoja = raiz[i];
		if(hoja->numapuntes > 0){ // Solo deshojamos si ha habido movimientos en la cuenta durante el ejercicio
		    hojaactiva = hoja->codigo;
		    deshojada = true;
		}
	    }
	    else{
		if(raiz[i]->ramas)
		    Deshojar(raiz[i]->ramas, nivel, superiores, &deshojada); // Buscamos por las hojas hijas
	    }// end if
	    i++;
	}// end while
    }
    else{
	if(hojaactiva == QString("00")){
	    if(raiz[0]){
		hojaactiva = raiz[0]->codigo;
		hoja = raiz[0];
		deshojada = true;
	    }
	}
	else{
	    for(i=0; i<70 && raiz[i]; i++){
		if(raiz[i]->codigo > hojaactiva){
		    hoja = raiz[i];
		    // Solo deshojamos si ha habido movimientos en la cuenta durante el ejercicio
		    if(hoja->numapuntes > 0){
			hojaactiva = hoja->codigo;			
			deshojada = true;
			break;
		    }
		}// end if
	    }// end for
	}// end if
    }// end if
    
    return deshojada;
}

void Arbol::Deshojar(tiporama* rama, unsigned int nivel, bool superiores, bool* deshojada){
    unsigned int nivelhoja = rama->hoja->codigo.length();
   
    if(hojaactiva >= rama->hoja->codigo){
	if(rama->hoja->ramas && nivelhoja < nivel)
	    Deshojar(rama->hoja->ramas, nivel, superiores, &(*deshojada));
	if(rama->sgte && !(*deshojada))
	    Deshojar(rama->sgte, nivel, superiores, &(*deshojada));
    }
    else{
	if(rama->hoja->numapuntes == 0){ // No deshojaremos por aqui, busquemos por otra rama
	    if(rama->sgte)
		Deshojar(rama->sgte, nivel, superiores, &(*deshojada));
	}
	else{ // Deshojamos
	    if(superiores){
		if(nivelhoja <= nivel){
		    hoja = rama->hoja;
		    hojaactiva = hoja->codigo;
		    *deshojada = true;
		}
		else{
		    if(rama->sgte)
			// Puede ser que hojas del mismo nivel en el arbol no sean del mismo nivel de cuentas.
			// Ej: 4750001, 4751 y 4752 podrian ser hojas hermanas porque no exista un 4750
			// En este caso, recorreremos tambien sus hojas hermanas por si acaso.
			Deshojar(rama->sgte, nivel, superiores, &(*deshojada));
		}
	    }
	    else{
		if(nivelhoja == nivel){
		    hoja = rama->hoja;
		    hojaactiva = hoja->codigo;
		    *deshojada = true;
		}
		else{
		    if(rama->hoja->ramas && nivelhoja < nivel)
			Deshojar(rama->hoja->ramas, nivel, superiores, &(*deshojada));
		    if(rama->sgte && !(*deshojada))
			Deshojar(rama->sgte, nivel, superiores, &(*deshojada));
		}// end if
	    }// end if
	}// end if
    }// end if
}

QString Arbol::hojaactual(QString valor){
    QString resultado;
    
    if( valor == "saldoant" )
	resultado.setNum(hoja->saldoant,'f',2);
    if( valor == "debe" )
	resultado.setNum(hoja->debe,'f',2);
    if( valor == "haber" )
	resultado.setNum(hoja->haber,'f',2);
    if( valor == "saldo" )
	resultado.setNum(hoja->saldo,'f',2);
    if( valor == "debeej" )
	resultado.setNum(hoja->debeej,'f',2);
    if( valor == "haberej" )
	resultado.setNum(hoja->haberej,'f',2);
    if( valor == "saldoej" )
	resultado.setNum(hoja->saldoej,'f',2);
    if( valor == "codigo" )
	resultado = hoja->codigo;
    if( valor == "descripcion" )
	resultado = hoja->descripcion;

    return resultado;
}

/*  
// 2 funciones utiles para depurar; activar y usarlas si se quiere ver el arbol por la salida estandar
  
void Arbol::imprimir()
{
    unsigned int i;
    
    for(i=0; raiz[i] && i<70; i++){
	fprintf(stderr, "\n%s:\n", raiz[i]->codigo.toAscii().data());
	if(raiz[i]->ramas)
	    ImprimirRama(raiz[i]->ramas);
    } // end for
}

void Arbol::ImprimirRama(tiporama* rama)
{
    fprintf(stderr, "%s.", rama->hoja->codigo.toAscii().data());
    if(rama->sgte){
	ImprimirRama(rama->sgte);
    }// end if
    if(rama->hoja->ramas){
	ImprimirRama(rama->hoja->ramas);
    }// end if
}
*/
