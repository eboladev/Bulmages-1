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
/** \file configuracion.cpp
  * Este fichero contiene la implementación de la clase \ref configuracion que se encarga
  * de recoger todos los parametros de configuración de la aplicación y los centraliza 
  * en un único objeto que luego es instanciado como objeto global \ref confpr
  * De este modo cualquier objeto de la aplicación puede consultar (sin demasiados problemas cual es la configuración que le corresponde).
  */

#include "configuracion.h"

#ifndef WIN32
#include <unistd.h>
#endif

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
using namespace std;


#define CONFGLOBAL "/etc/bulmages.conf"
#define CONFLOCAL "bulmages.conf"

/// The global object confpr is the instance of configuracion that is available to all BulmaGés aplications
/// El objeto global confpr es la instancia de la clase configuracion. Este objeto puede ser accedido desde todas las clases de la aplicación.
configuracion *confpr;


/** Constructor de la clase que hace directamente la lectura de los dos posibles
  * archivos que pueden tener información de configuración de Bulmages
  * /etc/bulmages.conf y
  * ~/bulmages.conf
  */
configuracion::configuracion() {
   /// Creamos el directorio personalizado de bulmages.
   system ("mkdir ~/.bulmages");
   /// Cambiamos a ~/.bulmages como directorio de trabajo.
   QString dir = getenv("HOME");
   dir = dir + "/.bulmages";

#ifndef WIN32
    /// Solo cambiamos de directorio si no es windows
   chdir (dir.ascii());
#endif

   /// Primero leemos la configuracion global
   leeconfig (CONFGLOBAL);
   /// Y  luego añadimos la configuracion local, asi los valores por defecto son los globales
   /// Y los que estan en local sustituyen a los existentes.
   QString dir1 = getenv("HOME");
   dir1 = dir1 + "/"+ CONFLOCAL;
   leeconfig ((char *)dir1.ascii());
   setValor(CONF_DIR_USER,dir);
   setValor(CONF_PRIVILEGIOS_USUARIO, "1");
   setValor(CONF_ALERTAS_DB, "Yes");
   setValor(CONF_LOGIN_USER, "");
   setValor(CONF_PASSWORD_USER, "");
}// end configuracion

/** El destructor de la clase no hace nada porque no hay que liberar memoria
  */
configuracion::~configuracion() {
}// end ~configuracion


/** Puesto que la configuración funciona sobre un array y sobre defines en dicho array
  * esta función dado un define devuelve el nombre utilizado.
  * Esta función es útil para hacer la inserción inicial de elementos
  * También es útil para hacer nosotros el guardado de los parametros.
  */
QString configuracion::nombre(int i) {
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
if (i== CONF_DIR_KUGAR) return "CONF_DIR_KUGAR";
if (i== CONF_DIR_OPENREPORTS) return "CONF_DIR_OPENREPORTS";
if (i== CONF_PLUGINS_BULMACONT) return "CONF_PLUGINS_BULMACONT";

if (i== CONF_DIR_IMG_ARTICLES) return "CONF_DIR_IMG_ARTICLES";
if (i== CONF_DIR_IMG_PERSONAL) return "CONF_DIR_IMG_PERSONAL";


if (i== CONF_FONTFAMILY_BULMAGES) return "CONF_FONTFAMILY_BULMAGES";
if (i== CONF_FONTSIZE_BULMAGES) return "CONF_FONTSIZE_BULMAGES";

if (i== CONF_PROGDATA) return "CONF_PROGDATA";

if (i== CONF_NUM_AUT_PRESUP) return "CONF_NUM_AUT_PRESUP";
if (i== CONF_NUM_AUT_FACTURA) return "CONF_NUM_AUT_FACTURA";
if (i== CONF_NUM_AUT_ALBARAN) return "CONF_NUM_AUT_ALBARAN";

if (i== CONF_MOSTRAR_ALMACEN) return "CONF_MOSTRAR_ALMACEN";


if (i== CONF_BG_LISTPRESUPUESTOS) return "CONF_BG_LISTPRESUPUESTOS";
if (i== CONF_BG_LINPRESUPUESTOS) return "CONF_BG_LINPRESUPUESTOS";
if (i== CONF_BG_DESCPRESUPUESTOS) return "CONF_BG_DESCPRESUPUESTOS";

if (i== CONF_BG_LISTPEDIDOSCLIENTE) return "CONF_BG_LISTPEDIDOSCLIENTE";
if (i== CONF_BG_LINPEDIDOSCLIENTE) return "CONF_BG_LINPEDIDOSCLIENTE";
if (i== CONF_BG_DESCPEDIDOSCLIENTE) return "CONF_BG_DESCPEDIDOSCLIENTE";

if (i== CONF_BG_LISTALBARANESCLIENTE) return "CONF_BG_LISTALBARANESCLIENTE";
if (i== CONF_BG_LINALBARANESCLIENTE) return "CONF_BG_LINALBARANESCLIENTE";
if (i== CONF_BG_DESCALBARANESCLIENTE) return "CONF_BG_DESCALBARANESCLIENTE";

if (i== CONF_BG_LISTFACTURASCLIENTE) return "CONF_BG_LISTFACTURASCLIENTE";
if (i== CONF_BG_LINFACTURASCLIENTE) return "CONF_BG_LINFACTURASCLIENTE";
if (i== CONF_BG_DESCFACTURASCLIENTE ) return "CONF_BG_DESCFACTURASCLIENTE";

if (i== CONF_BG_LISTARTICULOS ) return "CONF_BG_LISTARTICULOS";

return "";
}// end nombre


/** This method writes the configuration of the system to the home bulmages.conf file
  * Este metodo escribe la configuración del sistema en el fichero bulmages.conf del home del usuario.
  */
void configuracion::saveconfig() {
   QString dir1 = getenv("HOME");
   dir1 = dir1 + "/"+CONFLOCAL;
   ofstream filestr((char *) dir1.ascii());
   for (int i =0; i<1000;i++) {
      if (nombre(i) != "") {
         filestr << nombre(i);
         filestr << "   ";
         filestr << valor(i);
         filestr << endl;
      }// end if
   }// end for
   filestr.close();
}// end saveconfig


/** This method reads the configuration params from a file
  * \param fich File that contains the configuration.
  * Lee la configuración del fichero de configuración pasado y rellena la estructura.
  */
void configuracion::leeconfig(char *fich) {
	ifstream filestr(fich);
	string a;
        while (filestr.good()) {
               filestr >> a;
               for (int i=0;i<1000;i++) {
                  if (a==nombre(i)) {
                     filestr >> a;
		     m_valores[i]=a;
		     break;
                  }// end if
               }// end for
        }// end while
   filestr.close();
}// end leeconfig


/** Devuelve el valor de un campo determinado
  * \param i Parametro del que se quiere el valor
  * \return El valor que tiene dicho parametro.
  */
QString configuracion::valor(int i) {
        return (m_valores[i]);
}// end valor


/** Establece el valor de un campo determinado con la tupla que se pasa como parametro
  * \param i El indice del parametro a cambiar
  * \param valor El valor que tomará dicho parámetro
  */
void configuracion::setValor(int i, QString valor) {
  m_valores[i] = valor;
}// end setValor


