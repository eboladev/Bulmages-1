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
#include "logpass.h"

#ifndef WIN32
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#endif


BSelector::BSelector(QWidget * parent,const char * name) : UIselector(parent,name) {
//Al crear el selector, todos los modulos estan cerrados = NULL
    logpass *loggin1 = new logpass(0,"");
    loggin1->exec();
    fprintf(stderr,"Hemos aceptado y salido\n");
    loggin= loggin1->login;
    password= loggin1->password;
    delete loggin1;
}


BSelector::~BSelector()
{
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

//Boton para entrar en el modulo de VENTAS
void BSelector::ventas_clicked() {
}

//Boton para entrar en el modulo de COMPRAS
void BSelector::compras_clicked() {
//Al crear un nuevo modulo, le paso como primer parametro un puntero al selector.
//De este modo puedo acceder facilmente al selector desde el modulo.
}


//Boton cambio de Empresa y/o Usuario
void BSelector::seleccionaempresa_clicked() {
   abreempresaview *empcont = new abreempresaview(0, "hola", true);
   empcont->exec();
   fprintf(stderr,"Hemos cambiado la empresa\n");
   empresabd = empcont->empresabd;
   // Cambiamos el nombre en la pantalla.
   nombreempresa->setText(empcont->nombreempresa);
   delete empcont;
}// end seleccionaempresa_clicked


// Boton para entrar en el modulo de CONTABILIDAD 
void BSelector::contabilidad_clicked() {
/*   
   int pid;
   int error;
   //Abro Bulmages (contabilidad) con el nombre de la base de datos, Usuario y password
   //que han sido introducidos al principio al cargar el selector de módulos.
   // Cargamos la empresa guardada
   char *args[10];
   args[0]= (char *) "bulmages";
   args[1]=(char *) "bulmages";
   args[2]= (char *) empresabd.ascii();
   args[3]=(char *) loggin.ascii();
   args[4]=(char *) password.ascii();
   args[5]=NULL;
   if ((pid=fork()) < 0) {
      perror ("Fork failed");
      exit(errno);
   }// end if
   if (!pid) {
      for (int i=0;i<5;i++) fprintf(stderr,"%s\n", args[i]);
      string argumentos = "/home/tborras/bulmages/fuentes/bin/bulmages";
      error = execvp(argumentos.c_str(),args);
   }// end if
   if (pid) {
      waitpid (pid, NULL, 0);
   }// end if  
  */ 
   char cadena[300];
   sprintf(cadena,"bulmages bulmages %s %s %s", empresabd.ascii(), loggin.ascii(), password.ascii());
   system (cadena);
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


//Boton para entrar en el modulo de NOMINAS
void BSelector::nominas_clicked() {
}

