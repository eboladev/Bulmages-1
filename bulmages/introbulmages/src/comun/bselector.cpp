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


#include "bselector.h"
//#include "logpass.h"

#ifndef WIN32
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#endif


BSelector::BSelector(QWidget * parent,const char * name) : UIselector(parent,name) {
//Al crear el selector, todos los modulos estan cerrados = NULL
	m_tipoempresa = "";
}


BSelector::~BSelector() {
}

//Boton Salir
void BSelector::salir_clicked() {
//ctllog->add(LOG_SEG | LOG_TRA, 1,"BslSld002","---Saliendo de la aplicación---" );
    close();
}

//Boton para abrir el dialogo de configuraciones personalizadas
void BSelector::configura_clicked() {
    BConfiguracion * VentanaConfiguracion = new BConfiguracion(this, 0,"Ventana Configuracion",0);
    VentanaConfiguracion->exec();
    delete VentanaConfiguracion;
}

//Boton para entrar en la ventana de Iglues
void BSelector::m_iglues_clicked() {
   system("mozilla http://www.iglues.org");
}

//Boton para entrar en el modulo de TPV
void BSelector::m_bulmatpv_clicked() {
   if (m_tipoempresa != "BulmaFact" ) {
      abreempresaview *empcont = new abreempresaview(0,"bulmatpv", "abreempresa", true);
      empcont->exec();
      m_empresabd = empcont->nomDB();
//      tipo = empcont->tipo;
   }// end while
   if (m_empresabd != "") {
      char cadena[300];
      sprintf(cadena,"bulmatpv bulmatpv %s %s %s", m_empresabd.ascii(), confpr->valor(CONF_LOGIN_USER).c_str(), confpr->valor(CONF_PASSWORD_USER).c_str());
      system (cadena);
   }// end if
}// end m_bulmatpv_clicked


//Boton cambio de Empresa y/o Usuario
void BSelector::seleccionaempresa_clicked() {
   abreempresaview *empcont = new abreempresaview(0, "","abreempresa", true);
   empcont->exec();
   m_empresabd = empcont->nomDB();
   // Cambiamos el nombre en la pantalla.
   nombreempresa->setText(empcont->nomEmpresa());
   m_tipoempresa = empcont->tipoEmpresa();
   delete empcont;
}// end seleccionaempresa_clicked


/// Boton para entrar en el modulo de CONTABILIDAD 
void BSelector::contabilidad_clicked() {
   if (m_tipoempresa != "BulmaCont" ) {
      abreempresaview *empcont = new abreempresaview(0,"BulmaCont", "abreempresa", true);
      empcont->exec();
      m_empresabd = empcont->nomDB();
   }// end if
   if (m_empresabd != "") {
      char cadena[300];
      sprintf(cadena,"bulmacont bulmacont %s %s %s", m_empresabd.ascii(), confpr->valor(CONF_LOGIN_USER).c_str(), confpr->valor(CONF_PASSWORD_USER).c_str());
      system (cadena);
   }// end if
}// end contabilidad_clicked


//Boton para entrar en el modulo de PRODUCCION
void BSelector::produccion_clicked() {
//Al crear un nuevo modulo, le paso como primer parametro un puntero al selector.
//De este modo puedo acceder facilmente al selector desde el modulo.
   system("mozilla http://localhost/galopin/");
}


//Boton para entrar en el modulo de STOCKS Y ALMACENES
void BSelector::almacen_clicked() {
   fprintf(stderr,"SE ha pulsado sobre la opción del BContawev");
   system("mozilla http://localhost/bcontaweb/");
//   system("netscape http://localhost/bcontaweb/");
}


//Boton para entrar en el modulo de Facturación
void BSelector::m_bulmafact_clicked() {
   if (m_tipoempresa != "BulmaFact" ) {
      abreempresaview *empcont = new abreempresaview(0,"BulmaFact", "abreempresa", true);
      empcont->exec();
      m_empresabd = empcont->nomDB();
   }// end while
   if (m_empresabd != "") {
      char cadena[300];
      sprintf(cadena,"bulmafact bulmafact %s %s %s", m_empresabd.ascii(), confpr->valor(CONF_LOGIN_USER).c_str(), confpr->valor(CONF_PASSWORD_USER).c_str());
      system (cadena);
   }// end if
}

