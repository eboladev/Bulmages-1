//
//
// C++ Implementation: $MODULE$
//
// Description: 
//
//
// Author: Tomeu Borrás Riera <tborras@conetxia.com>, (C) 2003
//
// Copyright: See COPYING file that comes with this distribution
//
//
#include "configuracion.h"

#ifndef WIN32
#include <unistd.h>
#endif

#define CONFGLOBAL "/etc/bulmages.conf"
#define CONFLOCAL "bulmages.conf"

configuracion *confpr;

configuracion::configuracion() {
   // Creamos el directorio personalizado de bulmages.
   system ("mkdir ~/.bulmages");
   // Cambiamos a ~/.bulmages como directorio de trabajo.
   string dir = getenv("HOME");
   dir = dir + "/.bulmages";
   fprintf(stderr,"Se ha establecido el directorio de trabajo: %s\n",dir.c_str());

#ifndef WIN32
    // Solo cambiamos de directorio si no es windows
   chdir (dir.c_str());
#endif    
   
   // Primero leemos la configuracion global
   leeconfig (CONFGLOBAL);
   // Y  luego añadimos la configuracion local, asi los valores por defecto son los globales
   // Y los que estan en local sustituyen a los existentes.
   string dir1 = getenv("HOME");
   dir1 = dir1 + "/"+ CONFLOCAL;
   leeconfig ((char *)dir1.c_str());
}// end configuracion

configuracion::~configuracion() {
}// end ~configuracion


string configuracion::nombre(int i) {
if (i== CONF_BG_APUNTES) return "CONF_BG_APUNTES";
if (i== CONF_FG_APUNTES) return "CONF_FG_APUNTES";
if (i== CONF_BG_APUNTESA) return "CONF_BG_APUNTESA";
if (i== CONF_FG_APUNTESA) return "CONF_FG_APUNTESA";
if (i== CONF_FONTSIZE_APUNTES) return "CONF_FONTSIZE_APUNTES";
if (i== CONF_FONTFAMILY_APUNTES) return "CONF_FONTFAMILY_APUNTES";

if (i== CONF_BG_DIARIO) return "CONF_BG_DIARIO";
if (i== CONF_FG_DIARIO1) return "CONF_FG_DIARIO1";
if (i== CONF_FG_DIARIO2) return "CONF_FG_DIARIO2";
if (i== CONF_FONTFAMILY_DIARIO) return "CONF_FONTFAMILY_DIARIO";
if (i== CONF_FONTSIZE_DIARIO) return "CONF_FONTSIZE_DIARIO";
if (i== CONF_CONTRAPARTIDA_DIARIO) return "CONF_CONTRAPARTIDA_DIARIO";
if (i== CONF_CANAL_DIARIO) return "CONF_CANAL_DIARIO";
if (i== CONF_CCOSTE_DIARIO) return "CONF_CCOSTE_DIARIO";

if (i== CONF_BG_EXTRACTO) return "CONF_BG_EXTRACTO";
if (i== CONF_BG_BALANCE) return "CONF_BG_BALANCE";
if (i== CONF_BG_BALANCETREE) return "CONF_BG_BALANCETREE";
if (i== CONF_CONTRAPARTIDA_EXTRACTO) return "CONF_CONTRAPARTIDA_EXTRACTO";
if (i== CONF_CANAL_EXTRACTO) return "CONF_CANAL_EXTRACTO";
if (i== CONF_CCOSTE_EXTRACTO) return "CONF_CCOSTE_EXTRACTO";

if (i== CONF_CONTRAPARTIDA_APUNTES) return "CONF_CONTRAPARTIDA_APUNTES";
if (i== CONF_CANAL_APUNTES) return "CONF_CANAL_APUNTES";
if (i== CONF_CCOSTE_APUNTES) return "CONF_CCOSTE_APUNTES";

if (i== CONF_METABASE) return "CONF_METABASE";
if (i== CONF_SERVIDOR) return "CONF_SERVIDOR";
if (i== CONF_PUERTO) return "CONF_PUERTO";

if (i== CONF_NAVEGADOR) return "CONF_NAVEGADOR";
if (i== CONF_EDITOR) return "CONF_EDITOR";
if (i== CONF_SPLASH) return "CONF_SPLASH";
if (i== CONF_SPLASH_BULMAFACT) return "CONF_SPLASH_BULMAFACT";
if (i== CONF_BACKGROUND) return "CONF_BACKGROUND";

if (i== CONF_EJECUTABLES) return "CONF_EJECUTABLES";
if (i== CONF_PLANTILLA) return "CONF_PLANTILLA";

if (i== CONF_TRADUCCION) return "CONF_TRADUCCION";
if (i== CONF_DIR_TRADUCCION) return "CONF_DIR_TRADUCCION";
if (i== CONF_ARCHIVO_LOG) return "CONF_ARCHIVO_LOG";
if (i== CONF_DIR_REPORTS) return "CONF_DIR_REPORTS";

if (i== CONF_FONTFAMILY_BULMAGES) return "CONF_FONTFAMILY_BULMAGES";
if (i== CONF_FONTSIZE_BULMAGES) return "CONF_FONTSIZE_BULMAGES";

if (i== CONF_PROGDATA) return "CONF_PROGDATA";

return "";
}// end nombre


void configuracion::saveconfig() {
   string dir1 = getenv("HOME");
   dir1 = dir1 + "/"+CONFLOCAL;
   fprintf(stderr,"CONFIGURACION LOCAL %s\n", dir1.c_str());
   ofstream filestr((char *) dir1.c_str());
   for (int i =0; i<500;i++) {
      if (nombre(i) != "") {
         filestr << nombre(i);
         filestr << "   ";
         filestr << valor(i);
         filestr << endl;
      }// end if
   }// end for
   filestr.close();
}// end saveconfig


void configuracion::leeconfig(char *fich) {
   ifstream filestr(fich);
  string a;
	while (filestr.good()) {
               filestr >> a;
               for (int i=0;i<500;i++) {
                  if (a==nombre(i)) {
                     filestr >> a;
                     valores[i] = a;
                  }// end if
               }// end for              
		fprintf(stderr,"%s\n",a.c_str());
	}// end while  
   filestr.close();
valores[PRIVILEGIOS_USUARIO]= "1";
}// end leeconfig



void configuracion::cargarEntorno(QString baseDatos) {
/*
    postgresiface2 DBConn;
    QString query;
    cursor2 *recordSet;
    DBConn.inicializa(baseDatos);
    DBConn.begin();
*/
}


string configuracion::valor(int i) {
	return (valores[i]);
}// end valor


void configuracion::setValor(int i, string valor) {
  valores[i] = valor;
}


