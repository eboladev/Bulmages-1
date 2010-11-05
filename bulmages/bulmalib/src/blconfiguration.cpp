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

/// Este fichero contiene la implementacion de la clase 'configuracion' que se encarga
/// de recoger todos los parametros de configuracion de la aplicacion y los centraliza
/// en un unico objeto que luego es instanciado como objeto global 'confpr'
/// De este modo cualquier objeto de la aplicacion puede consultar
/// (sin demasiados problemas cual es la configuracion que le corresponde).

#include "QTextStream"
#include <QDir>

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#ifndef Q_OS_WIN32
#include <unistd.h>
#endif

#include "blconfig.h"
#include "blconfiguration.h"
#include "blcompanydialog.h"


/// El objeto global confpr es la instancia de la clase configuracion. Este objeto
/// puede ser accedido desde todas las clases de la aplicacion.
BlConfiguration BL_EXPORT *g_confpr = NULL;



void initConfiguration ( QString config )
{
    g_confpr = new BlConfiguration ( config );

	/// Inicializa el sistema de traducciones 'gettext'.
	setlocale(LC_ALL, "");
	bindtextdomain ("bulmalib", g_confpr->valor(CONF_DIR_TRADUCCION).toAscii().constData());
	textdomain ("bulmalib");
}

/// Constructor de la clase que hace directamente la lectura de los posibles
/// archivos que pueden tener informacion de configuracion. Como parametro
/// reciben el nombre del programa y buscan en este orden su archivo de configuracion.
///
/// En Linux:
/// 1) /etc/bulmages/ -> Opciones genericas para todos los usuarios.
/// 2) /home/~/.bulmages/  -> Opciones especificas para cada usuario.
///
/// En Windows:
/// 1) %ProgramFiles%\bulmages\etc\ -> Opciones genericas para todos los usuarios.
/// 2) %USERPROFILE%\.bulmages\  -> Opciones especificas para cada usuario.
///
/// Las opciones se iran cargando desde las mas genericas a las mas especificas.
/// Estas ultimas prevaleceran a las genericas cuando existan.
///
/// Dentro de cada directorio hay las siguientes preferencias a la hora de aplicar
/// las configuraciones:
///
/// 1) bulmages.conf -> archivo para todos los programas del paquete Bulmages.
/// 2) bulmaxxx.conf -> archivo especifico a un solo programa del paquete.
///
/// NOTA: No se puede utilizar blDebug dentro de esta clase porque necesita
/// valores que no se disponen antes de leer el archivo de configuraci&oacute;n.


///
/**
\param nombreprograma
**/
BlConfiguration::BlConfiguration ( QString nombreprograma )
{
//    blDebug("BlConfiguration::BlConfiguration", 0);

    /// Definimos los directorios donde buscar primero.
    m_dirGlobalConf = CONFIG_DIR_CONFIG;

    QString mensaje;
    QFile genericGlobalConfFile;
    QFile programGlobalConfFile;
    QFile genericLocalConfFile;
    QFile programLocalConfFile;
    QDir dirGlobalConf ( m_dirGlobalConf );

#ifndef Q_OS_WIN32
    QString dirusuario = getenv ( "HOME" );
#else
    QString dirusuario = getenv ( "UserProfile" );
    dirusuario.replace('\\', '/'); // Para no tener barras de los dos tipos mezclados en la misma ruta, usaremos las normales siempre
#endif

    m_dirLocalConf = dirusuario + "/.bulmages/";
    m_genericGlobalConfFile = "bulmages.conf";
    m_programGlobalConfFile = nombreprograma + ".conf";
    m_genericLocalConfFile = m_genericGlobalConfFile;
    m_programLocalConfFile = m_programGlobalConfFile;

    /// Comprobamos la existencia de los directorios y archivos de configuracion.
    /// Directorios y archivos obligatorios (sale si no existe):
    if ( !dirGlobalConf.exists() ) {
        mensaje = "--> ERROR: El directorio '" + m_dirGlobalConf + "' no existe. Debe crearlo. <--\n";
        fprintf ( stderr, "%s", mensaje.toAscii().constData() );
        exit ( -1 );
    } else {
        if ( !genericGlobalConfFile.exists ( m_dirGlobalConf + m_genericGlobalConfFile ) ) {
            mensaje = "--> ERROR: El archivo '" + m_dirGlobalConf + m_genericGlobalConfFile + "' no existe. Debe crearlo. <--\n";
            fprintf ( stderr, "%s", mensaje.toAscii().constData() );
            exit ( -1 );
        } else {
            /// 1) Leemos la configuracion del archivo generico global.
            mensaje = "--> Leyendo el archivo '" + m_dirGlobalConf + m_genericGlobalConfFile + "'<--\n";
            fprintf ( stderr, "%s", mensaje.toAscii().constData() );
            leeconfig ( m_dirGlobalConf + m_genericGlobalConfFile );
        }// end if
    } // end if

    /// Directorios y archivos opcionales:
    if ( !programGlobalConfFile.exists ( m_dirGlobalConf + m_programGlobalConfFile ) ) {
        mensaje = "--> El archivo '" + m_dirGlobalConf + m_programGlobalConfFile + "' no existe. <--\n";
        fprintf ( stderr, "%s", mensaje.toAscii().constData() );
    } else {
        /// 2) Leemos la configuracion del archivo especifico global.
        mensaje = "--> El archivo '" + m_dirGlobalConf + m_programGlobalConfFile + "' existe. Se va a leer.<--\n";
        fprintf ( stderr, "%s", mensaje.toAscii().constData() );
        leeconfig ( m_dirGlobalConf + m_programGlobalConfFile );
    }// end if

    /// Comprobamos si el usuario tiene creado su '/home/~/.bulmages/' directorio
    /// de configuracion.
    if ( !dirGlobalConf.exists ( m_dirLocalConf ) ) {
        if ( dirGlobalConf.mkdir ( m_dirLocalConf ) == TRUE ) {
            mensaje = "--> Se ha creado el directorio '" + m_dirLocalConf + "'. <--\n";
            fprintf ( stderr, "%s", mensaje.toAscii().constData() );
        } else {
            mensaje = "--> ERROR: No se ha podido crear el directorio '" + m_dirLocalConf + "'. <--\n";
            fprintf ( stderr, "%s", mensaje.toAscii().constData() );
            exit ( -1 );
        }// end if
    } // end if

    if ( !genericLocalConfFile.exists ( m_dirLocalConf + m_genericLocalConfFile ) ) {
        mensaje = "--> El archivo '" + m_dirLocalConf + m_genericLocalConfFile + "' no existe. <--\n";
        fprintf ( stderr, "%s", mensaje.toAscii().constData() );
    } else {
        /// 3) Leemos la configuracion del archivo generico local.
        mensaje = "--> El archivo '" + m_dirLocalConf + m_genericLocalConfFile + "' existe. Se va a leer.<--\n";
        fprintf ( stderr, "%s", mensaje.toAscii().constData() );
        leeconfig ( m_dirLocalConf + m_genericLocalConfFile );
    }// end if

    if ( !programLocalConfFile.exists ( m_dirLocalConf + m_programLocalConfFile ) ) {
        mensaje = "--> El archivo '" + m_dirLocalConf + m_programLocalConfFile + "' no existe. <--\n";
        fprintf ( stderr, "%s", mensaje.toAscii().constData() );
    } else {
        /// 4) Leemos la configuracion del archivo especifico local.
        mensaje = "--> El archivo '" + m_dirLocalConf + m_programLocalConfFile + "' existe. Se va a leer.<--\n";
        fprintf ( stderr, "%s", mensaje.toAscii().constData() );
        leeconfig ( m_dirLocalConf + m_programLocalConfFile );
    }// end if

    setValor ( CONF_DIR_USER, m_dirLocalConf );
    setValor ( CONF_PRIVILEGIOS_USUARIO, "1" );
    setValor ( CONF_LOGIN_USER, "" );
    setValor ( CONF_PASSWORD_USER, "" );

//    blDebug("END BlConfiguration::BlConfiguration", 0);
}


/// El destructor de la clase no hace nada porque no hay que liberar memoria.
/**
**/
BlConfiguration::~BlConfiguration()
{
//    blDebug("BlConfiguration::~BlConfiguration", 0);
//    blDebug("END BlConfiguration::~BlConfiguration", 0);
}


/// Puesto que la configuracion funciona sobre un array y sobre defines en dicho array
/// esta funcion dado un define devuelve el nombre utilizado.
/// Esta funcion es util para hacer la insercion inicial de elementos
/// Tambien es util para hacer nosotros el guardado de los parametros.
/**
\param i
\return
**/
QString BlConfiguration::nombre ( int i )
{
    if ( i == CONF_BG_APUNTES )
        return "CONF_BG_APUNTES";
    if ( i == CONF_FG_APUNTES )
        return "CONF_FG_APUNTES";
    if ( i == CONF_BG_APUNTESA )
        return "CONF_BG_APUNTESA";
    if ( i == CONF_FG_APUNTESA )
        return "CONF_FG_APUNTESA";
    if ( i == CONF_FONTSIZE_APUNTES )
        return "CONF_FONTSIZE_APUNTES";
    if ( i == CONF_FONTFAMILY_APUNTES )
        return "CONF_FONTFAMILY_APUNTES";
    if ( i == CONF_BG_DIARIO )
        return "CONF_BG_DIARIO";
    if ( i == CONF_BG_ALT_DIARIO )
        return "CONF_BG_ALT_DIARIO";
    if ( i == CONF_FG_DIARIO1 )
        return "CONF_FG_DIARIO1";
    if ( i == CONF_FG_DIARIO2 )
        return "CONF_FG_DIARIO2";
    if ( i == CONF_FONTFAMILY_DIARIO )
        return "CONF_FONTFAMILY_DIARIO";
    if ( i == CONF_FONTSIZE_DIARIO )
        return "CONF_FONTSIZE_DIARIO";
    if ( i == CONF_CONTRAPARTIDA_DIARIO )
        return "CONF_CONTRAPARTIDA_DIARIO";
    if ( i == CONF_CANAL_DIARIO )
        return "CONF_CANAL_DIARIO";
    if ( i == CONF_CCOSTE_DIARIO )
        return "CONF_CCOSTE_DIARIO";
    if ( i == CONF_BG_EXTRACTO )
        return "CONF_BG_EXTRACTO";
    if ( i == CONF_BG_BALANCE )
        return "CONF_BG_BALANCE";
    if ( i == CONF_BG_BALANCETREE )
        return "CONF_BG_BALANCETREE";
    if ( i == CONF_CONTRAPARTIDA_EXTRACTO )
        return "CONF_CONTRAPARTIDA_EXTRACTO";
    if ( i == CONF_CANAL_EXTRACTO )
        return "CONF_CANAL_EXTRACTO";
    if ( i == CONF_CCOSTE_EXTRACTO )
        return "CONF_CCOSTE_EXTRACTO";
    if ( i == CONF_CONTRAPARTIDA_APUNTES )
        return "CONF_CONTRAPARTIDA_APUNTES";
    if ( i == CONF_CANAL_APUNTES )
        return "CONF_CANAL_APUNTES";
    if ( i == CONF_CCOSTE_APUNTES )
        return "CONF_CCOSTE_APUNTES";
    if ( i == CONF_METABASE )
        return "CONF_METABASE";
    if ( i == CONF_SERVIDOR )
        return "CONF_SERVIDOR";
    if ( i == CONF_PUERTO )
        return "CONF_PUERTO";
    if ( i == CONF_FLIP )
        return "CONF_FLIP";
    if ( i == CONF_PYTHON )
        return "CONF_PYTHON";
    if ( i == CONF_DIR_TMP )
        return "CONF_DIR_TMP";
    if ( i == CONF_PDF )
        return "CONF_PDF";
    if ( i == CONF_ODS )
        return "CONF_ODS";
    if ( i == CONF_SXC )
        return "CONF_SXC";
    if ( i == CONF_NAVEGADOR )
        return "CONF_NAVEGADOR";
    if ( i == CONF_EDITOR )
        return "CONF_EDITOR";
    if ( i == CONF_SPLASH )
        return "CONF_SPLASH";
    if ( i == CONF_SPLASH_BULMACONT )
        return "CONF_SPLASH_BULMACONT";
    if ( i == CONF_SPLASH_BULMAFACT )
        return "CONF_SPLASH_BULMAFACT";
    if ( i == CONF_SPLASH_BULMATPV )
        return "CONF_SPLASH_BULMATPV";
    if ( i == CONF_EJECUTABLES )
        return "CONF_EJECUTABLES";
    if ( i == CONF_PLANTILLA )
        return "CONF_PLANTILLA";
    if ( i == CONF_TRADUCCION )
        return "CONF_TRADUCCION";
    if ( i == CONF_DIR_TRADUCCION )
        return "CONF_DIR_TRADUCCION";
    if ( i == CONF_ARCHIVO_LOG )
        return "CONF_ARCHIVO_LOG";
    if ( i == CONF_DIR_REPORTS )
        return "CONF_DIR_REPORTS";
    if ( i == CONF_DIR_KUGAR )
        return "CONF_DIR_KUGAR";
    if ( i == CONF_DIR_OPENREPORTS )
        return "CONF_DIR_OPENREPORTS";
    if ( i == CONF_DIR_AINTELIGENTES )
        return "CONF_DIR_AINTELIGENTES";
    if ( i == CONF_PLUGINS_BULMACONT )
        return "CONF_PLUGINS_BULMACONT";
    if ( i == CONF_PLUGINS_BULMAFACT )
        return "CONF_PLUGINS_BULMAFACT";
    if ( i == CONF_PLUGINS_BULMAGES )
        return "CONF_PLUGINS_BULMAGES";
    if ( i == CONF_PLUGINS_BULMATPV )
        return "CONF_PLUGINS_BULMATPV";
    if ( i == CONF_DIR_IMG_ARTICLES )
        return "CONF_DIR_IMG_ARTICLES";
    if ( i == CONF_DIR_THUMB_ARTICLES )
        return "CONF_DIR_THUMB_ARTICLES";
    if ( i == CONF_DIR_IMG_PERSONAL )
        return "CONF_DIR_IMG_PERSONAL";
    if ( i == CONF_DIR_IMG_ALUMNOS )
        return "CONF_DIR_IMG_ALUMNOS";
    if ( i == CONF_FONTFAMILY_BULMAGES )
        return "CONF_FONTFAMILY_BULMAGES";
    if ( i == CONF_FONTSIZE_BULMAGES )
        return "CONF_FONTSIZE_BULMAGES";
    if ( i == CONF_PROGDATA )
        return "CONF_PROGDATA";
    if ( i == CONF_NUM_AUT_PRESUP )
        return "CONF_NUM_AUT_PRESUP";
    if ( i == CONF_NUM_AUT_FACTURA )
        return "CONF_NUM_AUT_FACTURA";
    if ( i == CONF_NUM_AUT_ALBARAN )
        return "CONF_NUM_AUT_ALBARAN";
    if ( i == CONF_MOSTRAR_ALMACEN )
        return "CONF_MOSTRAR_ALMACEN";
    if ( i == CONF_BG_LISTPRESUPUESTOS )
        return "CONF_BG_LISTPRESUPUESTOS";
    if ( i == CONF_BG_LINPRESUPUESTOS )
        return "CONF_BG_LINPRESUPUESTOS";
    if ( i == CONF_BG_DESCPRESUPUESTOS )
        return "CONF_BG_DESCPRESUPUESTOS";
    if ( i == CONF_BG_LISTPEDIDOSCLIENTE )
        return "CONF_BG_LISTPEDIDOSCLIENTE";
    if ( i == CONF_BG_LINPEDIDOSCLIENTE )
        return "CONF_BG_LINPEDIDOSCLIENTE";
    if ( i == CONF_BG_DESCPEDIDOSCLIENTE )
        return "CONF_BG_DESCPEDIDOSCLIENTE";
    if ( i == CONF_BG_LISTALBARANESCLIENTE )
        return "CONF_BG_LISTALBARANESCLIENTE";
    if ( i == CONF_BG_LINALBARANESCLIENTE )
        return "CONF_BG_LINALBARANESCLIENTE";
    if ( i == CONF_BG_DESCALBARANESCLIENTE )
        return "CONF_BG_DESCALBARANESCLIENTE";
    if ( i == CONF_BG_LISTFACTURASCLIENTE )
        return "CONF_BG_LISTFACTURASCLIENTE";
    if ( i == CONF_BG_LINFACTURASCLIENTE )
        return "CONF_BG_LINFACTURASCLIENTE";
    if ( i == CONF_BG_DESCFACTURASCLIENTE )
        return "CONF_BG_DESCFACTURASCLIENTE";
    if ( i == CONF_BG_LISTARTICULOS )
        return "CONF_BG_LISTARTICULOS";
    if ( i == CONF_DIR_CANUALES )
        return "CONF_DIR_CANUALES";
    if ( i == CONF_TICKET_PRINTER_FILE )
        return "CONF_TICKET_PRINTER_FILE";
    if ( i == CONF_CASHBOX_FILE )
        return "CONF_CASHBOX_FILE";
    if ( i == CONF_CASHBOX_OPEN_CODE )
        return "CONF_CASHBOX_OPEN_CODE";
    if ( i == CONF_DEBUG )
        return "CONF_DEBUG";
    if ( i == CONF_ALERTAS_DB )
        return "CONF_ALERTAS_DB";
    if ( i == CONF_IDCLIENTE_DEFECTO )
        return "CONF_IDCLIENTE_DEFECTO";
    if ( i == CONF_IDALMACEN_DEFECTO )
        return "CONF_IDALMACEN_DEFECTO";
    if ( i == CONF_IDARTICULO_DEFECTO )
        return "CONF_IDARTICULO_DEFECTO";
    if ( i == CONF_IDTRABAJADOR_DEFECTO )
        return "CONF_IDTRABAJADOR_DEFECTO";
    if ( i == CONF_TPV_VISOR_FILE )
        return "CONF_TPV_VISOR_FILE";
    if ( i == CONF_TPV_PRINTER_LOGO )
        return "CONF_TPV_PRINTER_LOGO";
    if ( i == CONF_TPV_FULLSCREEN )
        return "CONF_TPV_FULLSCREEN";
    if ( i == CONF_REFRESH_LIST )
        return "CONF_REFRESH_LIST";
    if ( i == CONF_SHOW_PROGRESS_BAR )
        return "CONF_SHOW_PROGRESS_BAR";
    if ( i == CONF_IDFORMA_PAGO_VISA )
        return "CONF_IDFORMA_PAGO_VISA";
    if ( i == CONF_IDFORMA_PAGO_CONTADO )
        return "CONF_IDFORMA_PAGO_CONTADO";
    if ( i == CONF_IDFORMA_PAGO_DEFECTO )
        return "CONF_IDFORMA_PAGO_DEFECTO";
    if ( i == CONF_SERIE_FACTURA_DEFECTO )
        return "CONF_SERIE_FACTURA_DEFECTO";
    if ( i == CONF_DIR_PLUGINS )
        return "CONF_DIR_PLUGINS";
    if ( i == CONF_ARTICULOS_VARIOS )
        return "CONF_ARTICULOS_VARIOS";
    if ( i == CONF_TPV_CELL_WIDTH )
        return "CONF_TPV_CELL_WIDTH";
    if ( i == CONF_TPV_CELLS_PER_ROW )
        return "CONF_TPV_CELLS_PER_ROW";
    if ( i == CONF_FACT_ALT )
        return "CONF_FACT_ALT";
    if ( i == CONF_CONT_ALT )
        return "CONF_CONT_ALT";
    if ( i == CONF_STYLESHEET )
        return "CONF_STYLESHEET";
    if ( i == CONF_BACKGROUND_COLOR )
        return "CONF_BACKGROUND_COLOR";
    if ( i == CONF_BACKGROUND_IMAGE )
        return "CONF_BACKGROUND_IMAGE";
    if ( i == CONF_CUPS_DEFAULT_PRINTER )
        return "CONF_CUPS_DEFAULT_PRINTER";
    if ( i == CONF_BLOCK_WINDOWS )
	return "CONF_BLOCK_WINDOWS";
    if ( i == CONF_DIR_CONFIG )
	return "CONF_DIR_CONFIG";
    if ( i == CONF_GLOBAL_CONFIG_USER )
	return "CONF_GLOBAL_CONFIG_USER";
    if ( i == CONF_GLOBAL_CONFIG_COMPANY )
	return "CONF_GLOBAL_CONFIG_COMPANY";
    if ( i == CONF_TPV_SERVER_SYNC_IP )
	return "CONF_TPV_SERVER_SYNC_IP";
    if ( i == CONF_CENTER_TEXT_FIELDS )
	return "CONF_CENTER_TEXT_FIELDS";
    if ( i == CONF_ASK_BEFORE_EXIT )
	return "CONF_ASK_BEFORE_EXIT";
    return "";
}


/// This method writes the configuration of the system to the home bulmages.conf file
/// Este metodo escribe la configuracion del sistema en el fichero bulmages.conf del
/// 'home' del usuario.
/**
**/
void BlConfiguration::saveconfig()
{
//    blDebug("BlConfiguration::saveconfig", 0);
    QString dir1 = getenv ( "HOME" );
    dir1 = dir1 + "/.bulmages/" + m_dirLocalConf;

    QFile file ( dir1 );
    if ( !file.open ( QIODevice::WriteOnly | QIODevice::Text ) )
        return;

    QTextStream filestr ( &file );
    for ( int i = 0; i < 1000; i++ ) {
        if ( nombre ( i ) != "" ) {
            filestr << nombre ( i ).toAscii().data();
            filestr << "   ";
            filestr << valor ( i ).toAscii().data();
            filestr << endl;
        } // end if
    } // end for
    file.close();
//    blDebug("END BlConfiguration::saveconfig", 0);
}


/// This method reads the configuration params from a file 'fich' that
/// contains the configuration.
/// Lee la configuracion del fichero de configuracion pasado y rellena la estructura.
/**
\param fich
\return
**/
bool BlConfiguration::leeconfig ( QString fich )
{
//    blDebug("BlConfiguration::leeconfig", 0);
    QFile arch ( fich );
    if ( arch.open ( QIODevice::ReadOnly ) ) {
        QString cadaux1 = "Leyendo configuracion" + fich + "\n";
        fprintf ( stderr, "%s", cadaux1.toAscii().constData() );
        fprintf ( stderr, "%s", "\n" );
        QTextStream in ( &arch );
        while ( !in.atEnd() ) {
            QString cad = in.readLine();
            /// Hacemos la lectura de lineas de configuracion multilinea.
            while ( cad.endsWith ( "\\" ) ) {
                cad = cad.left ( cad.length() - 2 ) + in.readLine().trimmed();
            } // end while

            QString simplificada = cad.simplified();
            QStringList list = simplificada.split ( QRegExp ( "\\s+" ) );

            for ( int i = 0; i < 1000; i++ ) {
                if (  list[0] == nombre ( i )  && nombre ( i ) != "" ) {
                    blDebug ( "[" + nombre ( i ) + "]" + "--->" + cad, 0 );
                    cad = cad.right ( cad.length() - nombre ( i ).length() );
                    cad = cad.trimmed();
                    m_valores[i] = cad;
                } // end if
            } // end for
        } // end while
        arch.close();
        fprintf ( stderr, "%s", "FIN Leyendo configuracion\n" );
        return TRUE;
    } // end if
//    blDebug("END BlConfiguration::leeconfig", 0);
    return FALSE;
}


/// Devuelve el valor de un campo determinado.
/**
\param i Par&aacute;metro del que se quiere el valor.
\return El valor que tiene dicho par&aacute;metro.
**/
QString BlConfiguration::valor ( int i )
{
//    blDebug("BlConfiguration::valor", 0);
    if ( m_valores.contains ( i ) ) {
        return ( m_valores[i] );
    } // end if
    return "";
//    blDebug("END BlConfiguration::valor", 0);
}


/// Establece el valor de un campo determinado con la tupla que se pasa como par&aacute;metro.
/**
\param i El &iacute;ndice del par&aacute;metro a cambiar.
\param valor El valor que tomar&iacute;a dicho par&aacute;metro.
**/
void BlConfiguration::setValor ( int i, QString valor )
{
//    blDebug("BlConfiguration::setValor", 0);
    m_valores[i] = valor;
//    blDebug("END BlConfiguration::setValor", 0);
}

