//
//
// C++ Implementation: $MODULE$
//
// Description: 
//
//
// Author: Tomeu Borr? Riera <tborras@conetxia.com>, (C) 2003
//
// Copyright: See COPYING file that comes with this distribution
//
//
#include "configuracion.h"

#include <unistd.h>

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
   chdir (dir.c_str());
    
   // Primero leemos la configuracion local
   leeconfig (CONFGLOBAL);
   // Y  luego añadimos la configuracion local, asi los valores por defecto son los globales
   // Y los que estan en local sustituyen a los existentes.
   string dir1 = getenv("HOME");
   dir1 = dir1 + CONFLOCAL;
	leeconfig ((char *)dir1.c_str());

}// end configuracion

configuracion::~configuracion() {
}// end ~configuracion


void configuracion::leeconfig(char *fich) {
   ifstream filestr(fich);
  string a;
	while (filestr.good()) {
		filestr >> a;
		if (a== "CONF_BG_APUNTES") {           /* Aqui van las configuraciones de la introduccion de apuntes */
				filestr >> a;
				valores[CONF_BG_APUNTES] = a;
		} else if (a == "CONF_FG_APUNTES") {
			   filestr >> a;
				valores[CONF_FG_APUNTES] = a;
		} else if (a == "CONF_FG_APUNTESA") {
			   filestr >> a;
				valores[CONF_FG_APUNTESA] = a;
		} else if (a == "CONF_BG_APUNTESA") {
			   filestr >> a;
				valores[CONF_BG_APUNTESA] = a;
		} else if (a == "CONF_FONTSIZE_APUNTES") {
			   filestr >> a;
				valores[CONF_FONTSIZE_APUNTES] = a;
		} else if (a == "CONF_FONTFAMILY_APUNTES") {
			   filestr >> a;
				valores[CONF_FONTFAMILY_APUNTES] = a;
		} else if (a == "CONF_CONTRAPARTIDA_APUNTES") {
			   filestr >> a;
				valores[CONF_CONTRAPARTIDA_APUNTES] = a;
		} else if (a == "CONF_CANAL_APUNTES") {
			   filestr >> a;
				valores[CONF_CANAL_APUNTES] = a;
		} else if (a == "CONF_CCOSTE_APUNTES") {
			   filestr >> a;
				valores[CONF_CCOSTE_APUNTES] = a;
		} else if (a == "CONF_BACKGROUND") {
			   filestr >> a;
				valores[CONF_BACKGROUND] = a;
		} else if (a == "CONF_SPLASH") {
			   filestr >> a;
				valores[CONF_SPLASH] = a;
		} else if (a == "CONF_BG_DIARIO") {  /* Aqui van las configuraciones de diario */
			   filestr >> a;
				valores[CONF_BG_DIARIO] = a;
		} else if (a == "CONF_FG_DIARIO1") {
			   filestr >> a;
				valores[CONF_FG_DIARIO1] = a;
		} else if (a == "CONF_FG_DIARIO2") {
			   filestr >> a;
				valores[CONF_FG_DIARIO2] = a;
		} else if (a == "CONF_FONTSIZE_DIARIO") {
			   filestr >> a;
				valores[CONF_FONTSIZE_DIARIO] = a;
		} else if (a == "CONF_FONTFAMILY_DIARIO") {
			   filestr >> a;
				valores[CONF_FONTFAMILY_DIARIO] = a;
		} else if (a == "CONF_CONTRAPARTIDA_DIARIO") {
			   filestr >> a;
				valores[CONF_CONTRAPARTIDA_DIARIO] = a;
		} else if (a == "CONF_CANAL_DIARIO") {
			   filestr >> a;
				valores[CONF_CANAL_DIARIO] = a;
		} else if (a == "CONF_CCOSTE_DIARIO") {
			   filestr >> a;
				valores[CONF_CCOSTE_EXTRACTO] = a;
		} else if (a == "CONF_BG_EXTRACTO") {     /* Aqui van las configuraciones de extracto */
			   filestr >> a;
				valores[CONF_BG_EXTRACTO] = a;
		} else if (a == "CONF_CONTRAPARTIDA_EXTRACTO") {
			   filestr >> a;
				valores[CONF_CONTRAPARTIDA_EXTRACTO] = a;
		} else if (a == "CONF_CANAL_EXTRACTO") {
			   filestr >> a;
				valores[CONF_CANAL_EXTRACTO] = a;
		} else if (a == "CONF_CCOSTE_EXTRACTO") {
			   filestr >> a;
				valores[CONF_CCOSTE_EXTRACTO] = a;
		} else if (a == "CONF_BG_BALANCE") {   /*Aqui van las configuraciones de balance */
			   filestr >> a;
				valores[CONF_BG_BALANCE] = a;
		} else if (a == "CONF_BG_BALANCETREE") {
			   filestr >> a;
				valores[CONF_BG_BALANCETREE] = a;
		} else if (a == "CONF_METABASE") {    /* Aqui van las configuraciones de la base de datos */
			   filestr >> a;
				valores[CONF_METABASE] = a;
		} else if (a == "CONF_SERVIDOR") {    
			   filestr >> a;
				valores[CONF_SERVIDOR] = a;
		} else if (a == "CONF_PUERTO") {    
			   filestr >> a;
				valores[CONF_PUERTO] = a;
		} else if (a == "CONF_EDITOR") {    /* Aqui van las configuraciones de los programas auxiliares */
			   filestr >> a;
				valores[CONF_EDITOR] = a;
		} else if (a == "CONF_NAVEGADOR") { 
			   filestr >> a;
				valores[CONF_NAVEGADOR] = a;
		} else if (a == "CONF_EJECUTABLES") {
			   filestr >> a;
				valores[CONF_EJECUTABLES] = a;
		} else if (a == "CONF_PLANTILLA") {
			   filestr >> a;
				valores[CONF_PLANTILLA] = a;
		} else if (a== "CONF_TRADUCCION") {
				filestr >> a;
				valores[CONF_TRADUCCION] = a;
		} else if (a == "CONF_DIR_TRADUCCION") {
				filestr >> a;
				valores[CONF_DIR_TRADUCCION] = a;
		}// end if
		fprintf(stderr,"%s\n",a.c_str());
	}// end while  
   filestr.close();
valores[CONF_FONTFAMILY_BULMAGES]="Luxi Sans";
valores[CONF_FONTSIZE_BULMAGES]= "12";
}// end leeconfig


string configuracion::valor(int i) {
	return (valores[i]);
}// end valor


void configuracion::setValor(int i, string valor) {
  valores[i] = valor;
}


