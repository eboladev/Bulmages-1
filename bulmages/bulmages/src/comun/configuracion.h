//
//
// C++ Interface: $MODULE$
//
// Description: 
//
//
// Author: Tomeu Borr? Riera <tborras@conetxia.com>, (C) 2003
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef _CONFIGURACION1_H_
#define _CONFIGURACION1_H_


#include <qapplication.h>



// #include <qwidget.h>
#include <fstream>
#include <iostream>
#include <iomanip>
#include <stdlib.h>
#include <string>
using namespace std;
using std::string;
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#define CONF_BG_APUNTES 1
#define CONF_FG_APUNTES 2
#define CONF_BG_APUNTESA 3
#define CONF_FG_APUNTESA 4
#define CONF_FONTSIZE_APUNTES 5
#define CONF_FONTFAMILY_APUNTES       6

#define CONF_BG_DIARIO              106
#define CONF_FG_DIARIO1             107
#define CONF_FG_DIARIO2             108
#define CONF_FONTFAMILY_DIARIO      109
#define CONF_FONTSIZE_DIARIO        110
#define CONF_CONTRAPARTIDA_DIARIO   111
#define CONF_CANAL_DIARIO           112
#define CONF_CCOSTE_DIARIO          113

#define CONF_BG_EXTRACTO            120
#define CONF_BG_BALANCE             121
#define CONF_BG_BALANCETREE         122
#define CONF_CONTRAPARTIDA_EXTRACTO 123
#define CONF_CANAL_EXTRACTO         124
#define CONF_CCOSTE_EXTRACTO        125

#define CONF_CONTRAPARTIDA_APUNTES  130
#define CONF_CANAL_APUNTES          131
#define CONF_CCOSTE_APUNTES         132

#define CONF_METABASE 180
#define CONF_SERVIDOR 181
#define CONF_PUERTO   182

#define CONF_NAVEGADOR  197
#define CONF_EDITOR     198
#define CONF_SPLASH     199
#define CONF_BACKGROUND 200

#define CONF_EJECUTABLES 300
#define CONF_PLANTILLA   301

#define CONF_TRADUCCION  305
#define CONF_DIR_TRADUCCION 306


class configuracion {
	private:
	string valores[1000];
	int numeroprueba;
	
	public:
	configuracion();
	~configuracion();
	string valor(int);
   void leeconfig(char *);   
};


// ATENCION.- Este es un objeto global, y va a existir durante toda la aplicacion asi que
// Se va a llamar muy a menudo a este objeto en los demás archivos. 
extern configuracion *confpr;
extern QApplication *theApp;

#endif
