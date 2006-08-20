/***************************************************************************
 *   Copyright (C) 2003 by Tomeu Borras Riera                              *
 *   tborras@conetxia.com                                                  *
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

/** \file configuracion.cpp
  * Este fichero contiene la implementaci� de la clase \ref configuracion que se encarga
  * de recoger todos los parametros de configuraci� de la aplicaci� y los centraliza 
  * en un nico objeto que luego es instanciado como objeto global \ref confpr
  * De este modo cualquier objeto de la aplicaci� puede consultar (sin demasiados problemas cual es la configuraci� que le corresponde).
  */

#include "configuracion.h"
#include "funcaux.h"
#include "QTextStream"

#ifndef WIN32
#include <unistd.h>
#endif

#include <QDir>

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

using namespace std;

#include "abreempresaview.h"

#ifdef WIN32
#define CONFGLOBAL "C:\\bulmages.conf"
#else
#define CONFGLOBAL "/etc/bulmages.conf"
#endif

#define CONFLOCAL "bulmages.conf"


/// The global object confpr is the instance of configuracion that is available to
/// all BulmaGes aplications
/// El objeto global confpr es la instancia de la clase configuracion. Este objeto
/// puede ser accedido desde todas las clases de la aplicacion.
configuracion *confpr;


/// Constructor de la clase que hace directamente la lectura de los dos posibles
/// archivos que pueden tener informacion de configuracion de Bulmages:
/// '/etc/bulmages.conf' y
/// '~/bulmages.conf'.
configuracion::configuracion() {
    _depura("configuracion::configuracion", 1);

    QDir homedir;
    /// Cambiamos a ~/.bulmages como directorio de trabajo.
    QString dir = getenv("HOME");
    fprintf(stderr, "\n %s \n", dir.toAscii().data());
    dir = dir + "/.bulmages/";

#ifdef WIN32

    _depura("Es WINDOWS !!!", 1);
    dir = "C:\\.bulmages\\";
#endif

    _depura("Vamos a comprobar la existencia\n", 1);
    /// Comprobamos la existencia del directorio personalizado de bulmages.
    if (!homedir.exists(dir))
        homedir.mkdir(dir);
    /// Solo cambiamos de directorio si no es windows.
    chdir(dir.toAscii().data());
    /// Primero leemos la configuracion global.
    _depura("Vamos a llamar a leeconfig\n", 1);
    leeconfig (CONFGLOBAL);
    /// Y luego anyadimos la configuracion local, asi los valores por defecto son los globales.
    /// Y los que estan en local sustituyen a los existentes.

    /// Solo leemos la configuracion local si no estamos en Windows.
#ifndef WIN32

    QString dir1 = getenv("HOME");
    dir1 = dir1 + "/" + CONFLOCAL;
    leeconfig(dir1);
#endif

    setValor(CONF_DIR_USER, dir);
    setValor(CONF_PRIVILEGIOS_USUARIO, "1");
    setValor(CONF_ALERTAS_DB, "Yes");
    setValor(CONF_LOGIN_USER, "");
    setValor(CONF_PASSWORD_USER, "");
    _depura("END configuracion:configuracion\n", 1);
}


/// El destructor de la clase no hace nada porque no hay que liberar memoria.
configuracion::~configuracion() {}


/// Puesto que la configuracion funciona sobre un array y sobre defines en dicho array
/// esta funcion dado un define devuelve el nombre utilizado.
/// Esta funcion es til para hacer la insercion inicial de elementos
/// Tambien es util para hacer nosotros el guardado de los parametros.
QString configuracion::nombre(int i) {
    if (i == CONF_BG_APUNTES)
        return "CONF_BG_APUNTES";
    if (i == CONF_FG_APUNTES)
        return "CONF_FG_APUNTES";
    if (i == CONF_BG_APUNTESA)
        return "CONF_BG_APUNTESA";
    if (i == CONF_FG_APUNTESA)
        return "CONF_FG_APUNTESA";
    if (i == CONF_FONTSIZE_APUNTES)
        return "CONF_FONTSIZE_APUNTES";
    if (i == CONF_FONTFAMILY_APUNTES)
        return "CONF_FONTFAMILY_APUNTES";
    if (i == CONF_BG_DIARIO)
        return "CONF_BG_DIARIO";
    if (i == CONF_FG_DIARIO1)
        return "CONF_FG_DIARIO1";
    if (i == CONF_FG_DIARIO2)
        return "CONF_FG_DIARIO2";
    if (i == CONF_FONTFAMILY_DIARIO)
        return "CONF_FONTFAMILY_DIARIO";
    if (i == CONF_FONTSIZE_DIARIO)
        return "CONF_FONTSIZE_DIARIO";
    if (i == CONF_CONTRAPARTIDA_DIARIO)
        return "CONF_CONTRAPARTIDA_DIARIO";
    if (i == CONF_CANAL_DIARIO)
        return "CONF_CANAL_DIARIO";
    if (i == CONF_CCOSTE_DIARIO)
        return "CONF_CCOSTE_DIARIO";
    if (i == CONF_BG_EXTRACTO)
        return "CONF_BG_EXTRACTO";
    if (i == CONF_BG_BALANCE)
        return "CONF_BG_BALANCE";
    if (i == CONF_BG_BALANCETREE)
        return "CONF_BG_BALANCETREE";
    if (i == CONF_CONTRAPARTIDA_EXTRACTO)
        return "CONF_CONTRAPARTIDA_EXTRACTO";
    if (i == CONF_CANAL_EXTRACTO)
        return "CONF_CANAL_EXTRACTO";
    if (i == CONF_CCOSTE_EXTRACTO)
        return "CONF_CCOSTE_EXTRACTO";
    if (i == CONF_CONTRAPARTIDA_APUNTES)
        return "CONF_CONTRAPARTIDA_APUNTES";
    if (i == CONF_CANAL_APUNTES)
        return "CONF_CANAL_APUNTES";
    if (i == CONF_CCOSTE_APUNTES)
        return "CONF_CCOSTE_APUNTES";
    if (i == CONF_METABASE)
        return "CONF_METABASE";
    if (i == CONF_SERVIDOR)
        return "CONF_SERVIDOR";
    if (i == CONF_PUERTO)
        return "CONF_PUERTO";
    if (i == CONF_FLIP)
        return "CONF_FLIP";
    if (i == CONF_PYTHON)
        return "CONF_PYTHON";
    if (i == CONF_DIR_TMP)
        return "CONF_DIR_TMP";
    if (i == CONF_PDF)
        return "CONF_PDF";
    if (i == CONF_NAVEGADOR)
        return "CONF_NAVEGADOR";
    if (i == CONF_EDITOR)
        return "CONF_EDITOR";
    if (i == CONF_SPLASH)
        return "CONF_SPLASH";
    if (i == CONF_SPLASH_BULMAFACT)
        return "CONF_SPLASH_BULMAFACT";
    if (i == CONF_BACKGROUND)
        return "CONF_BACKGROUND";
    if (i == CONF_EJECUTABLES)
        return "CONF_EJECUTABLES";
    if (i == CONF_PLANTILLA)
        return "CONF_PLANTILLA";
    if (i == CONF_TRADUCCION)
        return "CONF_TRADUCCION";
    if (i == CONF_DIR_TRADUCCION)
        return "CONF_DIR_TRADUCCION";
    if (i == CONF_ARCHIVO_LOG)
        return "CONF_ARCHIVO_LOG";
    if (i == CONF_DIR_REPORTS)
        return "CONF_DIR_REPORTS";
    if (i == CONF_DIR_KUGAR)
        return "CONF_DIR_KUGAR";
    if (i == CONF_DIR_OPENREPORTS)
        return "CONF_DIR_OPENREPORTS";
    if (i == CONF_PLUGINS_BULMACONT)
        return "CONF_PLUGINS_BULMACONT";
    if (i == CONF_PLUGINS_BULMAFACT)
        return "CONF_PLUGINS_BULMAFACT";
    if (i == CONF_PLUGINS_BULMAGES)
        return "CONF_PLUGINS_BULMAGES";
    if (i == CONF_DIR_IMG_ARTICLES)
        return "CONF_DIR_IMG_ARTICLES";
    if (i == CONF_DIR_IMG_PERSONAL)
        return "CONF_DIR_IMG_PERSONAL";
    if (i == CONF_FONTFAMILY_BULMAGES)
        return "CONF_FONTFAMILY_BULMAGES";
    if (i == CONF_FONTSIZE_BULMAGES)
        return "CONF_FONTSIZE_BULMAGES";
    if (i == CONF_PROGDATA)
        return "CONF_PROGDATA";
    if (i == CONF_NUM_AUT_PRESUP)
        return "CONF_NUM_AUT_PRESUP";
    if (i == CONF_NUM_AUT_FACTURA)
        return "CONF_NUM_AUT_FACTURA";
    if (i == CONF_NUM_AUT_ALBARAN)
        return "CONF_NUM_AUT_ALBARAN";
    if (i == CONF_MOSTRAR_ALMACEN)
        return "CONF_MOSTRAR_ALMACEN";
    if (i == CONF_BG_LISTPRESUPUESTOS)
        return "CONF_BG_LISTPRESUPUESTOS";
    if (i == CONF_BG_LINPRESUPUESTOS)
        return "CONF_BG_LINPRESUPUESTOS";
    if (i == CONF_BG_DESCPRESUPUESTOS)
        return "CONF_BG_DESCPRESUPUESTOS";
    if (i == CONF_BG_LISTPEDIDOSCLIENTE)
        return "CONF_BG_LISTPEDIDOSCLIENTE";
    if (i == CONF_BG_LINPEDIDOSCLIENTE)
        return "CONF_BG_LINPEDIDOSCLIENTE";
    if (i == CONF_BG_DESCPEDIDOSCLIENTE)
        return "CONF_BG_DESCPEDIDOSCLIENTE";
    if (i == CONF_BG_LISTALBARANESCLIENTE)
        return "CONF_BG_LISTALBARANESCLIENTE";
    if (i == CONF_BG_LINALBARANESCLIENTE)
        return "CONF_BG_LINALBARANESCLIENTE";
    if (i == CONF_BG_DESCALBARANESCLIENTE)
        return "CONF_BG_DESCALBARANESCLIENTE";
    if (i == CONF_BG_LISTFACTURASCLIENTE)
        return "CONF_BG_LISTFACTURASCLIENTE";
    if (i == CONF_BG_LINFACTURASCLIENTE)
        return "CONF_BG_LINFACTURASCLIENTE";
    if (i == CONF_BG_DESCFACTURASCLIENTE )
        return "CONF_BG_DESCFACTURASCLIENTE";
    if (i == CONF_BG_LISTARTICULOS )
        return "CONF_BG_LISTARTICULOS";

    return "";
}


/// This method writes the configuration of the system to the home bulmages.conf file
/// Este metodo escribe la configuracion del sistema en el fichero bulmages.conf del 'home' del usuario.
void configuracion::saveconfig() {
    QString dir1 = getenv("HOME");
    dir1 = dir1 + "/" + CONFLOCAL;
    ofstream filestr((char *) dir1.data());
    for (int i = 0; i < 1000; i++) {
        if (nombre(i) != "") {
            filestr << nombre(i).toAscii().data();
            filestr << "   ";
            filestr << valor(i).toAscii().data();
            filestr << endl;
        } // end if
    } // end for
    filestr.close();
}


/// This method reads the configuration params from a file 'fich' that
/// contains the configuration.
/// Lee la configuracion del fichero de configuracion pasado y rellena la estructura.
bool configuracion::leeconfig(QString fich) {
    _depura("leeconfig(" + fich + ")\n", 0);
    QFile arch(fich);
    if (arch.open(QIODevice::ReadOnly)) {
        QTextStream in(&arch);
        while (!in.atEnd()) {
            QString cad = in.readLine();
            for (int i = 0; i < 1000; i++) {
                if (cad.startsWith(nombre(i)) && nombre(i) != "") {
                    _depura("[" + nombre(i) + "]" + "--->" + cad, 1);
                    cad = cad.right(cad.length() - nombre(i).length());
                    cad = cad.trimmed();
                    m_valores[i] = cad;
                } // end if
            } // end for
        } // end while
        arch.close();
        return TRUE;
    } // end if
    return FALSE;
}


/** Devuelve el valor de un campo determinado.
  * \param i Parametro del que se quiere el valor.
  * \return El valor que tiene dicho parametro.
  */
QString configuracion::valor(int i) {
    return (m_valores[i]);
}


/** Establece el valor de un campo determinado con la tupla que se pasa como parametro.
  * \param i El indice del parametro a cambiar.
  * \param valor El valor que tomaria dicho parametro.
  */
void configuracion::setValor(int i, QString valor) {
    m_valores[i] = valor;
}

