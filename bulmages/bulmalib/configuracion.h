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
#include <qtranslator.h>

#include "postgresiface2.h"

#include <fstream>
#include <iostream>
#include <iomanip>
#include <stdlib.h>
#include <qstring.h>


/// Indica el color de fondo de la ventana de apuntes cuando esta cerrada.
#define CONF_BG_APUNTES 1
/// Indica el color de la fuente de la ventana de apuntes cuando esta cerrada
#define CONF_FG_APUNTES 2
/// Indica el color de fondo de la ventana de apuntes cuando esta abierta
#define CONF_BG_APUNTESA 3
/// Indica el color de la fuente cuando los aputnes estan abiertos
#define CONF_FG_APUNTESA 4
/// indica el tamaño de la tipografia de la introducción de aputnes
#define CONF_FONTSIZE_APUNTES 5
/// Indica la fuente de la pantalla de introducción de apuntes
#define CONF_FONTFAMILY_APUNTES       6
///Indica el color de fondo del libro diario 
#define CONF_BG_DIARIO              106
/// Indica el color de la letra del diario en el pijama opcion 1
#define CONF_FG_DIARIO1             107
/// Indica el color de la letra del diario en el pijama opción 2
#define CONF_FG_DIARIO2             108
/// Indica el tipo de letra del libro diario
#define CONF_FONTFAMILY_DIARIO      109
/// Indica el tamaño de letra del libro diario
#define CONF_FONTSIZE_DIARIO        110
/// Indica si en el diario debe aparecer la contrapartida o no
#define CONF_CONTRAPARTIDA_DIARIO   111
/// Indica si en el diario debe aparecer el canal o no
#define CONF_CANAL_DIARIO           112
/// Indica si en el diario debe aparecer el centro de coste o no
#define CONF_CCOSTE_DIARIO          113

/// Indica cual es el color de fondo del extracto
#define CONF_BG_EXTRACTO            120
/// Indica cual es el color de fondo del balance
#define CONF_BG_BALANCE             121
/// Indica cual es el color de fondo del balance desplegable
#define CONF_BG_BALANCETREE         122
/// Indica si en el extracto de cuentas debe aparecer la contrapartida o no
#define CONF_CONTRAPARTIDA_EXTRACTO 123
/// Indica si en el extracto de cuentas debe aparecer el canal o no
#define CONF_CANAL_EXTRACTO         124
/// Indica si en el extracto de cuentas debe aparecer o no el centro de coste
#define CONF_CCOSTE_EXTRACTO        125

/// Indica si en la introducción de apuntes debe aparecer el campo de contrapartida.
#define CONF_CONTRAPARTIDA_APUNTES  130
/// Indica si en la introducción de apuntes debe aparecer la columna de canales
#define CONF_CANAL_APUNTES          131
/// Indica si en la introducción de apuntes deben aparecer los centros de coste
#define CONF_CCOSTE_APUNTES         132

/// Indica que base de datos es la META (OBSOLETO=
#define CONF_METABASE 180

/// Indica por defecto que servidor de bases de datos utilizar.
#define CONF_SERVIDOR 181
/// Indica por defecto que puerto tiene el servidor de bases de datos para conectar con el
#define CONF_PUERTO   182
/// Indica que navegador utilizar para mostrar los resultados de los listados (por defecto mozilla)
#define CONF_NAVEGADOR  197
/// Indica que editor usar para mostrar listados (por defecto kedit)
#define CONF_EDITOR     198
/// Indica que imagen utilizar como splash de BulmaCont
#define CONF_SPLASH     199
/// Indica que imagen utilizar como Background
#define CONF_BACKGROUND 200
/// Indica que imagen utilizar como splash de Bulmafact
#define CONF_SPLASH_BULMAFACT 196
/// # Esto indica donde se van a albergar los programas auxiliares a bulmages (cargaemp, creaemp, borraemp)
#define CONF_EJECUTABLES 300
/// # Esto indica que CSS usar con los listados
#define CONF_PLANTILLA   301

/// Esto indica que idioma utilizar (es ca en fr de o locales(coger el idioma de las locales))
#define CONF_TRADUCCION  305
/// Indica en que directorio se encuentran los archivos de traducción
#define CONF_DIR_TRADUCCION 306
/// Indica cual va a ser el archivo de log del programa
#define CONF_ARCHIVO_LOG 307

/// Indica donde se hayan ubicados los archivos de formato de REPORTS
#define CONF_DIR_REPORTS 308
/// Indica la ruta donde se encuentran los archivos de formato de KUGAR
#define CONF_DIR_KUGAR   309

/// Indica cual va a ser el tipo de letra por defecto.
#define CONF_FONTFAMILY_BULMAGES  310
/// Indica cual va a ser el tamaño de fuente por defecto
#define CONF_FONTSIZE_BULMAGES    311

/// Indica donde estan los datos del programa. Por defecto /usr/share/bulmages
#define CONF_PROGDATA            320

/// Determina el directorio donde se hayan ubicados los archivos de datos para OPENREPORTS
#define CONF_DIR_OPENREPORTS 330

/// Dictamina cuales son los privilegios del usuario.
#define CONF_PRIVILEGIOS_USUARIO    420

/// ALERTAS_DB Determina si los errores de la base de datos deben ser alertados via messagebox o no.
#define CONF_ALERTAS_DB             421

/// CONF_LOGIN_USER    Determina el login de usuario. Inicialmente vale  ""
#define CONF_LOGIN_USER             422

/// CONF_PASSWORD_USER     Determina el password del usuario. Inicialmente vale ""
#define CONF_PASSWORD_USER          423


/// CONF_NUM_AUT_*		Determina si se utilizarán numeradores automaticos para presupuestos, albaranes y facturas  Inicialmente vale ""
#define CONF_NUM_AUT_PRESUP 500
#define CONF_NUM_AUT_ALBARAN 501
#define CONF_NUM_AUT_FACTURA 502

/// CONF_MOSTRAR_ALMACEN		Determina si se visualizará el almacén en los listados y si aparecera inhibido en las entradas de datos  Inicialmente vale ""
#define CONF_MOSTRAR_ALMACEN	510




/** Se encarga de recoger todos los parametros de configuración de la aplicación y los centraliza 
  * en un único objeto que luego es instanciado como objeto global \ref confpr
  * De este modo cualquier objeto de la aplicación puede consultar (sin demasiados problemas cual es la configuración que le corresponde).
  * <b>NOTA: también ha adoptado la capacidad de ser un paso de parametros configurables de la aplicación. Esto se traduce en que algunos parametros se pueden modificar en tiempo de ejecución.</b>
  */
class configuracion {
    private:
    	/// La forma de almacenar los campos va a ser mediante un mapa de valores. De esta forma no tendremos memoria desaprovechada.
    	QMap <int, QString> m_valores;
    public:
        configuracion();
        ~configuracion();
        QString valor(int);
        void setValor(int, QString);
        void leeconfig(char *);
        void saveconfig();
        QString nombre(int);
};


// ATENCION.- Este es un objeto global, y va a existir durante toda la aplicacion asi que
// Se va a llamar muy a menudo a este objeto en los demás archivos.
/// Existe una instancia del objeto global confpr
extern configuracion *confpr;
/// Existe una instancia del objeto global theApp
extern QApplication *theApp;
/// Existe una instancia del objeto global traductor
extern QTranslator * traductor;

#endif
