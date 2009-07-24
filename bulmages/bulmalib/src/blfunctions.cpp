/***************************************************************************
 *   Copyright (C) 2003 by Tomeu Borras Riera and Josep Burcion            *
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

/// Fichero de definicion de funciones auxiliares que no es preciso encapsular
/// En este fichero se definen todas aquellas funciones que por su brevedad o aislamiento son
/// utilizadas de forma regular en el programa. Implementadas en blfunctions.cpp
/// Dichas funciones normalmente son de uso general, por lo que es normal ver este
/// archivo incluido en la practica totalidad de los demas ficheros.
#include <QString>
#include <QTextEdit>
#include <QDir>
#include <QTextStream>
#include <QLocale>
#include <QProcess>
#include <QTime>

#include "blfunctions.h"
#include "blconfiguration.h"
#include "blmainwindow.h"
#include "local_BlI18n.h"

/// Esta funcion permite editar un texto en un QTextEdit y devuelve el texto editado.
QString editaTexto ( QString texto )
{
    QTextEdit * ed = new QTextEdit ( 0 );
    ed->setFixedSize ( 450, 250 );
    ed->setPlainText ( texto );
    g_main->setEnabled ( FALSE );
    ed->show();
    while ( !ed->isHidden() ) {
        g_theApp->processEvents();
    } // end while
    g_main->setEnabled ( TRUE );
    QString vuelta = ed->toPlainText();
    return vuelta;
}

/// Reformatea una cadena de caracteres para poder ser escrita desde un script.
/// La conversion es a strings de C
/// cambia los slash por slash + slash
/// cambia las comillas por slash + comillas
/// cambia los tabuladores por slah + t
QString parsearCode ( const QString &cad )
{
    QString result = cad;
    result.replace ( "\\", "\\\\" );
    result.replace ( "\"", "\\\"" );
    result.replace ( "\t", "\\t" );
    return result;
}

/// Escapa los minimos caracteres necesarios para
/// escribir a cadena a XML, sin usar CDATA, para ahorrar bytes y porque (me suena que) puede no
/// estar permitido en todos los lugares de un fichero xml segun esquemas o dtds especificos
QString xmlEscape ( const QString& param )
{
    QString text = param;
    text.replace ( "&", "&amp;" );
    text.replace ( '"', "&quot;" );
    text.replace ( "<", "&lt;" );
    text.replace ( ">", "&gt;" );
    text.replace ( "\n", "<br />" );

    return text;
}
/// Escapa los caracteres necesarios para que la cadena pueda ser embebida dentro de un
/// script en python
QString pythonEscape ( const QString& param )
{
    QString text = param;
    text.replace ( "'", "\\'" );
    text.replace ( "\n", "\\n" );
    return text;
}

QString genEscape ( const QString &param, int tipoEscape )
{
    QString param1;
    switch ( tipoEscape ) {
    case 1:
        param1 =  xmlEscape ( param );
        break;
    case 2:
        param1 = pythonEscape ( param );
        break;
    default:
        param1 = param;

    } // end switch
    return param1;
}

/// Proteje cadenas de texto pasandoles una sustitucion de codigos especiales de XML.
/// Nota: ahora ficha.cpp ya respeta el encoding del XML
/// original y pone referencias numericas de caracter solo a los caracteres
/// no soportados en el encoding. Ya no hace tanta falta
/// esta funcion, y creo que con escapeXml pasamos (para
/// listados rml al menos). Podemos tener el buffer en
/// memoria en unicode hasta escribirlo a fichero en ficha::generaRML
QString XMLProtect ( const QString &string )
{
    /// Recorre todo el QString y sustituye los caracteres NO ASCII y
    /// los caracteres que no van muy bien en un XML.

    QString cadena = string;
    QString cadenatmp = "";
    QChar *data = cadena.data();

    /// Cambia tambien otros caracteres no adecuados.
    /*
        cadenatmp.replace ( "&", "&#38;" );
        cadenatmp.replace ( ">", "&#62;" );
        cadenatmp.replace ( "<", "&#60;" );
        cadenatmp.replace ( "\"", "&#34;" );
        cadenatmp.replace ( "\'", "&#39;" );
    */
    int i;
    for ( i = 0; i < cadena.length(); i++ ) {
        if ( data->unicode() > 127 ) {
            cadenatmp = cadenatmp + QString ( "<![CDATA[&#" ) + QString::number ( data->unicode() ) + QString ( ";]]>" );
        } else if ( (data->unicode() == 10) | (data->unicode() == 13) ) {
            /// Cambiamos los Intros por el formato HTML.
            cadenatmp = cadenatmp + QString ( "\n" );

        } else if ( data->unicode() == 37 ) { // %
            cadenatmp = cadenatmp + QString ( "<![CDATA[&#37;]]>" );
        } else if ( data->unicode() == 38 ) { // &
            cadenatmp = cadenatmp + QString ( "<![CDATA[&#38;]]>" );
        } else if ( data->unicode() == 62 ) { // >
            cadenatmp = cadenatmp + QString ( "<![CDATA[&#62;]]>" );
        } else if ( data->unicode() == 60 ) { // <
            cadenatmp = cadenatmp + QString ( "<![CDATA[&#60;]]>" );
        } else if ( data->unicode() == 34 ) { // "
            cadenatmp = cadenatmp + QString ( "<![CDATA[&#34;]]>" );
        } else if ( data->unicode() == 39 ) { // '
            cadenatmp = cadenatmp + QString ( "<![CDATA[&#39;]]>" );

        } else {
            cadenatmp = cadenatmp + QString ( *data );
        } // end if
        ++data;
    } // end if
    return cadenatmp;
}


/// Proteje cadenas de texto pasandoles una sustitucion de codigos especiales de XML.
QString XMLDesProtect ( const QString &string )
{
    /// Recorre todo el QString y sustituye los caracteres NO ASCII y
    /// los caracteres que no van muy bien en un XML.
    QString cadenatmp = string;

    /// Cambia tambien otros caracteres no adecuados.
    cadenatmp.replace ( "&#38;", "&" );
    cadenatmp.replace ( "&#62;", ">" );
    cadenatmp.replace ( "&#60;", "<" );
    cadenatmp.replace ( "&#34;", "\"" );
    cadenatmp.replace ( "&#39;", "\'" );
    cadenatmp.replace ( "<br />", "\n" );

    for ( int i = 126; i < 256; i++ ) {
        cadenatmp.replace ( "&#" + QString::number ( i ) + ";", QChar ( i ) );
    } // end for

    return cadenatmp;
}


/// Extiende un string a un numero de cuenta sustituyendo los '.' por ceros.
/// cad = Cadena inicial
/// num1 = Numero de digitos totales de la cuenta.
/// Devuelve un QString con la cuenta extendida al nmero de digitos indicado.
QString extiendecodigo ( QString cad, unsigned int num1 )
{
    _depura ( "BlFunctions::extiendecodigo", 0, cad + "--" + QString::number ( num1 ) );
    QString cod = cad;
    int num = num1;
    if ( cod.length() < num ) {
        QString str7 ( num - cod.length() + 1, '0' );
        int pos = cod.indexOf ( ".", 0 );
        if ( pos > 0 ) {
            cod.replace ( pos, 1, str7 );
        } // end if
    } // end if
    _depura ( "END BlFunctions::extiendecodigo", 0 );
    return ( cod );
}


/// Redondeo de numeros en punto flotante.
/// n = Numero a redondear
/// d = Numero de decimales
/// Devuelve numero redondeado
float fround ( float n, unsigned d )
{
    return floor ( n * pow ( 10., d ) + .5 ) / pow ( 10., d );
}


/// Esta funcion convierte un numero con decimales a un entero. Usando la regla
/// que si el el primer decimal es mayor o igual a 5 se devuelve el entero superior. AInteligentesView
/// valor = Numero a convertir.
int roundI ( double valor )
{
    int retorno;
    double mayor = floor ( valor );
    if ( ( mayor - valor ) >= 0.5 )
        retorno = ( int ) mayor - 1;
    else
        retorno = ( int ) mayor;
    return retorno;
}


/// Procesa el string pasado como parametro y devuelve una estructura del tipo QDate
/// Esta funcion extiende la fecha pasada como parametro
/// QString y devuelve la fecha en formato QDate.
/// fechaintro string con la fecha a ser normalizada.
QDate normalizafecha ( QString fechaintro )
{
    QDate fecharesult;
    int d, M, y;
    switch ( fechaintro.length() ) {
    case 4: /// fecha tipo ddMM
        d = fechaintro.mid ( 0, 2 ).toInt();
        M = fechaintro.mid ( 2, 2 ).toInt();
        y = QDate::currentDate().year();
        break;
    case 5:/// fecha tipo dd/MM
        d = fechaintro.mid ( 0, 2 ).toInt();
        M = fechaintro.mid ( 3, 2 ).toInt();
        y = QDate::currentDate().year();
        break;
    case 6: /// fecha tipo ddMMyy
        d = fechaintro.mid ( 0, 2 ).toInt();
        M = fechaintro.mid ( 2, 2 ).toInt();
        y = 2000 + fechaintro.mid ( 4, 2 ).toInt();
        break;
    case 8:
        if ( fechaintro.contains ( "/", Qt::CaseSensitive ) || fechaintro.contains ( "-", Qt::CaseSensitive ) ) {
            /// fecha tipo dd/MM/yy o dd-MM-yy
            d = fechaintro.mid ( 0, 2 ).toInt();
            M = fechaintro.mid ( 3, 2 ).toInt();
            y = 2000 + fechaintro.mid ( 6, 2 ).toInt();
        } else {
            /// o bien tipo ddMMyyyy
            d = fechaintro.mid ( 0, 2 ).toInt();
            M = fechaintro.mid ( 2, 2 ).toInt();
            y = fechaintro.mid ( 4, 4 ).toInt();
        } // end if
        break;
    case 10: /// fecha tipo dd/MM/yyyy
        d = fechaintro.mid ( 0, 2 ).toInt();
        M = fechaintro.mid ( 3, 2 ).toInt();
        y = fechaintro.mid ( 6, 4 ).toInt();
        break;
    default:
        d = QDate::currentDate().day();
        M = QDate::currentDate().month();
        y = QDate::currentDate().year();
    } // end switch
    if ( !fecharesult.setDate ( y, M, d ) )
        fecharesult = QDate::currentDate();
    return ( fecharesult );
}


/// Esta funcion ajusta el codigo pasado al numero de digitos especificado.
/// Para ello bsca los ceros intermedios y los amplia hasta que el numero de caracteres sea el deseado.
/// Lo hace a partir del quinto digito por defecto. Aunque este parametro deberia ser configurable.
/// cod = string con el codigo actual.
/// num1 = numero de digitos que debe tener el codigo final.
/// Devuelve un QString con el codigo ajustado.

/// BUG: --- Esta funcion tiene un uso especifico de bulmacont y por eso no
/// deberia estar en bulmalib.
QString ajustacodigo ( QString cad, unsigned int num1 )
{
    QString cod = cad;
    unsigned int longcad = cad.length();
    if ( longcad > 4 ) {
        if ( longcad < num1 ) {
            QString str7 ( num1 - longcad, '0' );
            cod = cad.left ( 4 );
            cod += str7;
            cod += cad.right ( longcad - 4 );
        } // end if
        if ( longcad > num1 ) {
            cod = cad.left ( 4 );
            cod += cad.right ( num1 - 4 );
        } // end if
    } // end if
    return ( cod );
}


/// Sustituye cadenas en un archivo.
void reemplazaarchivo ( QString archivo, QString texto1, QString texto2, QString archivo2 )
{
    QString cadena = " sed -e \"s&" + texto1 + "&" + texto2 + "&g\"  " + archivo + " > " + archivo2 + "";
    int result = system ( cadena.toAscii().data() );
    if (result == -1) {
	mensajeError(_("Error al ejecutar el comando 'sed' [ blfunctions.cpp->reemplazaarchivo() ]."));
    } // end if
}


/// En la impresi&oacute;n de documentos con bgtrml2pdf esta funci&oacute;n hace casi todo
/// el trabajo de la invocaci&oacute;n de bgtrml2pdf para evitar trabajo duplicado.
void generaPDF ( const QString arch )
{
    _depura ( "generaPDF " + arch, 0 );

    QDir::setCurrent ( g_confpr->valor ( CONF_DIR_USER ) );
    QString cadsys;

#ifdef WINDOWS

    cadsys = g_confpr->valor ( CONF_PYTHON ) + " " + g_confpr->valor ( CONF_PROGDATA ) + "trml2pdf\\bgtrml2pdf " + arch + ".rml > " + g_confpr->valor ( CONF_DIR_USER ) + arch + ".pdf";
    int result1 = system ( cadsys.toAscii() );
    if (result1 == -1) {
	mensajeError(_("Error en PYTHON [ blfunctions->generaPDF() ]"));
    } // end if
    
    _depura ( cadsys, 0 );
    cadsys = g_confpr->valor ( CONF_FLIP ) + " -u " + g_confpr->valor ( CONF_DIR_USER ) + arch + ".pdf";
    int result2 = system ( cadsys.toAscii().data() );
    if (result2 == -1) {
	mensajeError(_("Error en FLIP [ blfunctions->generaPDF() ]"));
    } // end if

    _depura ( cadsys, 0 );

#else

    cadsys = "bgtrml2pdf " + arch + ".rml > " + arch + ".pdf";
    int result3 = system ( cadsys.toAscii().data() );
    if (result3 == -1) {
	mensajeError(_("Error en bgtrml2pdf [ blfunctions->generaPDF() ]"));
    } // end if

#endif

    _depura ( "END generaPDF " + arch, 0 );
}


/// Genera un ODS a partir de un pys sin abrirlo.
void generaPYS ( const QString arch )
{
    _depura ( "generaPYS " + arch, 0 );
    QDir::setCurrent ( g_confpr->valor ( CONF_DIR_USER ) );
    QString cadsys;

    QString cadena = "rm " + g_confpr->valor ( CONF_DIR_USER ) + arch + ".ods";
    int result1 = system ( cadena.toAscii() );
    if (result1 == -1) {
	mensajeError(_("Error al borrar archivo .ods [ blfunctions->generaPYS() ]"));
    } // end if

    cadena = " cd " + g_confpr->valor ( CONF_DIR_USER ) + "; python " + arch + ".pys";
    int result2 = system ( cadena.toAscii() );
    if (result2 == -1) {
	mensajeError(_("Error al ejecutar PYTHON [ blfunctions->generaPYS() ]"));
    } // end if

}


/// Genera un ODS a partir de un pys usand python. y ademas lo muestra.
void invocaPYS ( const QString arch )
{
    generaPYS ( arch );
    QString cadena = g_confpr->valor ( CONF_ODS ) + " " + g_confpr->valor ( CONF_DIR_USER ) + arch + ".ods &";
    int result = system ( cadena.toAscii() );
    if (result == -1) {
	mensajeError(_("Error al ejecutar oocalc [ blfunctions->invocaPYS() ]"));
    } // end if

}

/// Genera un PDF a partir de un RML usando trml2pdf y adem&aacute;s lo muestra con el visor
/// de PDF pasado en la configuraci&oacute;n.
/// arch = Archivo RML.
void invocaPDF ( const QString arch )
{
    generaPDF ( arch );
    QString cadsys = g_confpr->valor ( CONF_PDF ) + " " + g_confpr->valor ( CONF_DIR_USER ) + arch + ".pdf &";
    int result = system ( cadsys.toAscii().data() );
    if (result == -1) {
	mensajeError(_("Error al ejecutar el visor de PDF [ blfunctions->invocaPDF() ]"));
    } // end if
    
}


/// De momento no se usa, pero sirve para enviar documentos por e-mail a un destinatario.
void mailsendPDF ( const QString arch, const QString to, const QString subject, const QString message )
{
    //FIXME: REVISAR PARAMETROS de mailsend o la posibilidad de anyadir otros programas
    //para enviar correo desde la ventana de configuracion del programa.
    QString cadsys = "mailsend -h " + arch + " -d " + to + " -f bulmages@iglues.org -t test@iglues.org -sub " + subject + " -m " + message;
    int result = system ( cadsys.toAscii().data() );
    if (result == -1) {
	mensajeError(_("Error al ejecutar mailsend [ blfunctions->mailsendPDF() ]"));
    } // end if
    
}


QString windowID ( const QString &app )
{

    QString cad = "";

    if ( app != "" ) {
        cad = "xwininfo -int -name \"" + app + "\" | grep xwininfo | awk '{print $4}' > /tmp/xwinfo";
    } else {
        cad = "xwininfo -int | grep Window | awk '{print $4}' > /tmp/xwinfo";
    } // end if

    int result = system ( cad.toAscii() );
    if (result == -1) {
	mensajeError(_("Error al ejecutar xwininfo [ blfunctions->windowID() ]"));
    } // end if
    

    QString winId = "";

    QFile file ( "/tmp/xwinfo" );
    if ( !file.open ( QIODevice::ReadOnly | QIODevice::Text ) )
        return "";

    QTextStream in ( &file );
    if ( !in.atEnd() ) {
        winId = in.readLine();
    } // end if


    file.close();

    return winId;
}


void _debugOn ()
{
    g_confpr->setValor ( CONF_DEBUG, "TRUE" );
}


void _debugOff ()
{
    g_confpr->setValor ( CONF_DEBUG, "FALSE" );
}


/// cad = String a presentar como texto de depuracion o como mensaje de error.
/// nivel 0 = normal.
/// nivel 1 = Bajo.
/// nivel 2 = Alto (sale un popup).
/// nivel 4 = Comienza depuracion indiscriminada.
/// nivel 5 = Termina depuracion indiscriminada.
/// nivel 10 = Salida a terminal.
#if CONFIG_DEBUG == TRUE
void _depura ( const QString &cad, int nivel, const QString &param )
{
    /// Si el objeto confpr no esta creado puede dar segmentation fault.
    if ( g_confpr == NULL ) {
        return;
    } // end if

    static bool semaforo = 0;


    if ( g_confpr->valor ( CONF_DEBUG ) == "TRUE" ) {
        static QFile file ( g_confpr->valor ( CONF_DIR_USER ) + "bulmagesout.txt" );
        static QTextStream out ( &file );

        static QFile filexml ( g_confpr->valor ( CONF_DIR_USER ) + "bulmagesout.xml" );
        static QTextStream outxml ( &filexml );

        static int auxxml = 0;
        static int supnivel = 0;
        static int indice = 0;
        static QString mensajesanulados[7000];
        static QString clasesanuladas[7000];
        static int indiceclases = 0;
        static QTime t;

        if ( !semaforo ) {
            t.start();
            if ( !file.open ( QIODevice::WriteOnly | QIODevice::Text ) )
                return;
            if ( !filexml.open ( QIODevice::WriteOnly | QIODevice::Text ) )
                return;
            semaforo = 1;
        } // end if


        if ( nivel == 5 ) {
            supnivel = 0;
            nivel = 2;
        } // end if
        if ( nivel == 4 ) {
            supnivel = 2;
            nivel = 2;
        } // end if
        if ( nivel == 0 || nivel == 1 ) {
            /// Si la cadena contiene END bajamos el nivel
            if ( !cad.contains ( " " ) ) {
                auxxml ++;
                if ( auxxml > 20 ) auxxml = 1;
            } // end if
            for ( int i = 0; i < auxxml; i++ ) {
                outxml << "    ";
                out << "    ";
            } // end for

            QString cad1 = cad;
            if ( cad.startsWith ( "END" ) ) {
                cad1 = "</" + cad1.remove ( 0, 4 ) + " time=\"" + QString::number ( t.elapsed() ) + "\" result=\"" + param + "\" >";
            } else if ( cad.contains ( " " ) ) {
                cad1 = "    <COMENT value=\"" + cad + " " + param + "\"></COMENT>";
            } else {
                cad1 = "<" + cad1 + " time=\"" + QString::number ( t.elapsed() ) + "\" param=\"" + param + "\" >";
            } // end if

            outxml << cad1  << "\n" << flush;
            out << cad << " " << param << "\n" << flush;
            if ( cad.startsWith ( "END" ) ) {
                auxxml --;
                if ( auxxml < 0 ) auxxml = 20;
            } // end if
        } // end if
        for ( int i = 0; i < indice; i++ ) {
            if ( cad == mensajesanulados[i] ) {
                return;
            } // end if
        } // end for
        for ( int i = 0; i < indiceclases; i++ ) {
            if ( cad.left ( cad.indexOf ( "::" ) ) == clasesanuladas[i] ) {
                return;
            } // end if
        } // end for

        if ( nivel == 2 || ( supnivel == 2 && nivel == 0 ) || nivel == 3 ) {
            out << cad << " " << param << "\n" << flush;
            int err = QMessageBox::information ( NULL,
                                                 _ ( "Informacion de depuracion" ),
                                                 cad + " " + param,
                                                 _ ( "&Continuar" ),
                                                 _ ( "&Omitir" ),
                                                 _ ( "Omitir &clase" ),
                                                 0, 1 );
            if ( err == 1 ) {
                mensajesanulados[indice++] = cad;
            } // end if
            if ( err == 2 ) {
                clasesanuladas[indiceclases++] = cad.left ( cad.indexOf ( "::" ) );
            } // end if
        } // end if

        file.flush();
    }
}

//#else
//
//inline void _depura ( const QString &, int , const QString & )
//{
//    return;
//}
#endif

void mensajeInfo ( QString cad, QWidget *parent )
{
    QMessageBox msgBox;
    msgBox.information ( parent,
                         _ ( "Informacion del programa" ),
                         cad, _ ( "&Continuar" ),
                         QString::null, 0 );
}


void mensajeAviso ( QString cad, QWidget *parent )
{
    QMessageBox msgBox;
    msgBox.warning ( parent,
                     _ ( "Aviso del programa" ),
                     cad, _ ( "&Continuar" ),
                     QString::null, 0 );
}


void mensajeError ( QString cad, QWidget *parent )
{
    QMessageBox msgBox;
    msgBox.critical ( parent,
                      _ ( "Error del programa" ),
                      cad, _ ( "&Continuar" ),
                      QString::null, 0 );
}


QString num2texto ( QString numero, QString moneda, QString singular )
{
    /// Si es 0 el n&uacute;mero, no tiene caso procesar toda la informaci&oacute;n.
    if ( numero == "0" || numero == "00" ) {
        return "cero " + moneda;
    } // end if

    /// En caso que sea un peso, pues igual que el 0 aparte que no muestre el plural "pesos".
    if ( numero == "1" ) {
        return "un " + singular;
    } // end if

    QMap<QString , QString > numeros;
    //numeros["unidad"][0][0] = "cero";
    numeros["unidad10"] = "un";
    numeros["unidad20"] = "dos";
    numeros["unidad30"] = "tres";
    numeros["unidad40"] = "cuatro";
    numeros["unidad50"] = "cinco";
    numeros["unidad60"] = "seis";
    numeros["unidad70"] = "siete";
    numeros["unidad80"] = "ocho";
    numeros["unidad90"] = "nueve";

    numeros["decenas10"] = "diez";
    numeros["decenas20"] = "veinte";
    numeros["decenas30"] = "treinta";
    numeros["decenas40"] = "cuarenta";
    numeros["decenas50"] = "cincuenta";
    numeros["decenas60"] = "sesenta";
    numeros["decenas70"] = "setenta";
    numeros["decenas80"] = "ochenta";
    numeros["decenas90"] = "noventa";
    numeros["decenas110"] = "dieci";
    numeros["decenas111"] = "once";
    numeros["decenas112"] = "doce";
    numeros["decenas113"] = "trece";
    numeros["decenas114"] = "catorce";
    numeros["decenas115"] = "quince";
    numeros["decenas21"] = "veinti";
    numeros["decenas31"] = "treinta y ";
    numeros["decenas41"] = "cuarenta y ";
    numeros["decenas51"] = "cincuenta y ";
    numeros["decenas61"] = "sesenta y ";
    numeros["decenas71"] = "setenta y ";
    numeros["decenas81"] = "ochenta y ";
    numeros["decenas91"] = "noventa y ";

    numeros["centenas10"] = "cien";
    numeros["centenas20"] = "doscientos ";
    numeros["centenas30"] = "trecientos ";
    numeros["centenas40"] = "cuatrocientos ";
    numeros["centenas50"] = "quinientos ";
    numeros["centenas60"] = "seiscientos ";
    numeros["centenas70"] = "setecientos ";
    numeros["centenas80"] = "ochocientos ";
    numeros["centenas90"] = "novecientos ";
    numeros["centenas11"] = "ciento ";

    QMap<QString, QString> postfijos;
    postfijos["1-0"] = "";
    postfijos["10-0"] = "";
    postfijos["100-0"] = "";
    postfijos["1000-0"] = " mil ";
    postfijos["10000-0"] = " mil ";
    postfijos["100000-0"] = " mil ";
    postfijos["1000000-0"] = " millon ";
    postfijos["10000000-0"] = " millon ";
    postfijos["100000000-0"] = " millon ";
    postfijos["1000000-1"] = " millones ";
    postfijos["10000000-1"] = " millones ";
    postfijos["100000000-1"] = " millones ";

    numero = numero.replace ( ",", "." );
    QString decimal_break = ".";
    //echo "test run on ".$numero."<br>";
    QString entero = numero.split ( decimal_break ).at ( 0 );
    QString decimal = numero.split ( decimal_break ).at ( 1 );

//    $entero=strtok($numero,$decimal_break);
//    $decimal=strtok($decimal_break);
    if ( decimal == "" ) {
        decimal = "00";
    } // end if
    if ( decimal.size() < 2 ) {
        decimal = decimal + "0";
    } // end if
    if ( decimal.size() > 2 ) {
        decimal = decimal.right ( 2 );
    } // end if
    //echo "entero ".$entero."<br> decimal ".$decimal."<br>";

    QString entero_breakdown = entero;

    QString breakdown_key = "1000000000000";
    QString num_string = "";
    QMap<QString, QString> breakdown;
    QString chundreds = "";
    QString tens = "";
    QString ctens = "";
    QString cctens = "";
    QString ones = "";
    QString cpostfijos = "";
    while ( breakdown_key.toDouble() > 0.5 ) {
//  _depura(num_string, 2);
        breakdown["entero" + breakdown_key + "number"] = /*floor(*/ QString::number ( entero_breakdown.toLongLong() / breakdown_key.toLongLong() );

//  _depura(breakdown["entero"+breakdown_key+"number"], 2);

        //echo " ".$breakdown["entero"][$breakdown_key]["number"]."<br>";
        if ( breakdown["entero" + breakdown_key+"number"].toLongLong() > 0 ) {
            //echo " further process <br>";
            breakdown["entero" + breakdown_key+"100"] = /*floor(*/ QString::number ( breakdown["entero" + breakdown_key + "number"].toLongLong() / 100 );
            breakdown["entero" + breakdown_key+"10"] = /*floor( */ QString::number ( ( breakdown["entero" + breakdown_key + "number"].toLongLong() % 100 ) / 10 );
            breakdown["entero" + breakdown_key+"1"] = /*floor(*/   QString::number ( breakdown["entero" + breakdown_key + "number"].toLongLong() % 10 );
            //echo " 100 ->".$breakdown["entero"][$breakdown_key][100]."<br>";
            //echo " 10   ->".$breakdown["entero"][$breakdown_key][10]."<br>";
            //echo " 1     ->".$breakdown["entero"][$breakdown_key][1]."<br>";

            QString hundreds = breakdown["entero" + breakdown_key + "100"];
            // if not a closed value at hundredths
            if ( ( breakdown["entero" + breakdown_key + "10"].toLongLong() + breakdown["entero"+breakdown_key+"1"].toLongLong() ) > 0 ) {
                chundreds = "1";
            } else {
                chundreds = "0";
            } // end if

            if ( numeros.contains ( "centenas" + hundreds + chundreds ) ) {
                //echo " centenas ".numeros["centenas"][$hundreds][$chundreds]."<br>";
                num_string += numeros["centenas" + hundreds + chundreds];
            } else {
                //echo " centenas ".numeros["centenas"][$hundreds][0]."<br>";
                if ( numeros.contains ( "centenas" + hundreds + "0" ) ) {
                    num_string += numeros["centenas" + hundreds + "0"];
                }
            } // end if

            if ( ( breakdown["entero" + breakdown_key + "1"].toLongLong() ) > 0 ) {
                ctens = "1";
                tens = breakdown["entero" + breakdown_key + "10"];
                //echo "NOT CLOSE TENTHS<br>";
                if ( breakdown["entero" + breakdown_key + "10"].toLongLong() == 1 ) {
                    if ( breakdown["entero" + breakdown_key + "1"].toLongLong() < 6 ) {
                        cctens = breakdown["entero" + breakdown_key + "1"];
                        //echo " decenas ".numeros["decenas"][$tens][$ctens][$cctens]."<br>";
                        num_string += numeros["decenas" + tens + ctens + cctens];
                    } else {
                        //echo " decenas ".numeros["decenas"][$tens][$ctens][0]."<br>";
                        num_string += numeros["decenas" + tens + ctens + "0"];
                    } // end if
                } else {
                    //echo " decenas ".numeros["decenas"][$tens][$ctens]."<br>";
                    if ( numeros.contains ( "decenas" + tens + ctens ) ) {
                        num_string += numeros["decenas" + tens + ctens];
                    }
                } // end if
            } else {
                //echo "CLOSED TENTHS<br>";
                ctens = "0";
                tens = breakdown["entero" + breakdown_key + "10"];
                //echo " decenas ".numeros["decenas"][$tens][$ctens]."<br>";
                if ( numeros.contains ( "decenas" + tens + ctens ) ) {
                    num_string += numeros["decenas" + tens + ctens];
                } // end if
            } // end if

            if ( cctens == "" ) {
                ones = breakdown["entero" + breakdown_key + "1"];
                if ( numeros.contains ( "unidad" + ones + "0" ) ) {
                    //echo " tens ".numeros["unidad"][$ones][0]."<br>";
                    num_string += numeros["unidad" + ones + "0"];
                } // end if
            } // end if

            cpostfijos = "-1";
            if ( breakdown["entero" + breakdown_key + "number"].toLongLong() > 1 ) {
                cpostfijos = "1";
            } // end if

            if ( postfijos.contains ( breakdown_key + "-" + cpostfijos ) ) {
                num_string += postfijos[breakdown_key + "-" + cpostfijos];
            } else {
                num_string += postfijos[breakdown_key + "-0"];
            } // end if
        } // end if
        cctens = "";
        entero_breakdown = QString::number ( entero_breakdown.toInt() % breakdown_key.toLongLong() );
        breakdown_key = QString::number ( breakdown_key.toLongLong() / 1000 );

        //echo "CADENA ".$num_string."<br>";
    } // end while
//    return  num_string+" "+moneda+" "+decimal+"/100 M.N.";
    if ( decimal != "0" && decimal != "00" ) {
        return num_string + " " + moneda + " con " + num2texto ( decimal + ".00", " centimos", " centimo" );
    } else {
        return num_string + " " + moneda;
    } // end if
}


void centrarEnPantalla ( QWidget *ventana )
{
    QRect rect;
    QDesktopWidget *escritorio = new QDesktopWidget();
    rect = escritorio->availableGeometry();
    ventana->move ( rect.center() - ventana->rect().center() );
}


QString ascii127 ( const QString &orig )
{
    /// Recorre todo el QString y sustituye los caracteres NO ASCII y
    /// los caracteres que no van muy bien en un XML.

    QString cadena = orig;
    QString cadenatmp = "";


    /// Cambia tambien otros caracteres no adecuados.
    cadena = cadena.replace ( "a", "a" );
    cadena = cadena.replace ( "e", "e" );
    cadena = cadena.replace ( "i", "i" );
    cadena = cadena.replace ( "o", "o" );
    cadena = cadena.replace ( "i", "u" );

    cadena = cadena.replace ( "à", "a" );
    cadena = cadena.replace ( "è", "e" );
    cadena = cadena.replace ( "ì", "i" );
    cadena = cadena.replace ( "ò", "o" );
    cadena = cadena.replace ( "ù", "u" );

    cadena = cadena.replace ( "ä", "a" );
    cadena = cadena.replace ( "ë", "e" );
    cadena = cadena.replace ( "ï", "i" );
    cadena = cadena.replace ( "ö", "o" );
    cadena = cadena.replace ( "ü", "u" );

    cadena = cadena.replace ( "â", "a" );
    cadena = cadena.replace ( "ê", "e" );
    cadena = cadena.replace ( "î", "i" );
    cadena = cadena.replace ( "ô", "o" );
    cadena = cadena.replace ( "û", "u" );

    cadena = cadena.replace ( "ny", "n" );
    cadena = cadena.replace ( "ç", "c" );

    cadena = cadena.replace ( "&", "." );

    QChar *data = cadena.data();
    int i;
    for ( i = 0; i < cadena.length(); i++ ) {
        if ( data->unicode() <= 127 ) {
            cadenatmp = cadenatmp + QString ( *data );
        } // end if
        ++data;
    } // end if
    return cadenatmp;
}

/// Trata una cadena para ser escrita dentro de un fichero en Python sin que esta
/// rompa la sintaxis relacionada con el indentado del lenguaje
QString data2python ( QString string )
{

    string = string.replace ( "\n", " " );
    string = string.replace ( "\t", " " );
    string = string.replace ( "\'", " " );
    string = string.replace ( "\"", " " );
    return string;
}


/// La tabla de conversion de codigos para el CIF
QChar codigosCIF[] = {QChar ( 'A' ), QChar ( 'B' ), QChar ( 'C' ), QChar ( 'D' ), QChar ( 'E' ), QChar ( 'F' ), QChar ( 'G' ), QChar ( 'H' ), QChar ( 'J' ), QChar ( 'P' ), QChar ( 'Q' ), QChar ( 'R' ), QChar ( 'S' ), QChar ( 'U' ), QChar ( 'V' ), QChar ( 'N' ), QChar ( 'W' ) };

char validacionesCIF[] = {'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J'};

/// La tabla de conversion de codigos para el NIF
char codigoNIF[] = {'T', 'R', 'W', 'A', 'G', 'M', 'Y', 'F', 'P', 'D', 'X', 'B', 'N', 'J', 'Z', 'S', 'Q', 'V', 'H', 'L', 'C', 'K', 'E', 'T'};


/// Suma los digitos de un numero  y devuelve el resultado.
/// por ejemplo sumaDigitos(56) devolveria 5 + 6 = 11
int sumaDigitos ( int val )
{
    QString cad = QString::number ( val );
    int res = 0;
    for ( int i = 0; i < cad.size(); i++ ) {
        res += cad[i].digitValue();
    } // end for
    return res;
}


/// Dependiendo de lo que se haya introducido distingue si es un CIF o un NIF y valida
bool validarCIFNIF ( QString nifcif, QChar &digit )
{
    if ( nifcif[0].isDigit() )
        return validarNIF ( nifcif, digit );
    return validarCIF ( nifcif, digit );
}


/// Valida un nif de 9 digitos. Si el nif pasado es menor de 5 digitos
bool validarNIF ( QString nif1, QChar &digit )
{

    /// Quitamos los caracteres raros.
    QString nif = nif1.replace ( "-", "" );
    nif = nif.replace ( ".", "" );
    nif = nif.replace ( "_", "" );
    nif = nif1.left ( 8 );

    /// Si el CIF tiene menos de 4 caracteres validamos. Ya que igual queremos permitir CIF's Inventados.
    if ( nif1.size() < 5 ) {
        return TRUE;
    } // end if

    int modulo = nif.toInt() % 23;
    digit = QChar ( codigoNIF[modulo] );
    if ( nif1[8] == QChar ( codigoNIF[modulo] ) )
        return TRUE;
    return FALSE;

}

//
bool validarCIF ( QString cif1, QChar &digit )
{

    // ESPANYA EFECTIVIDAD A PARTIR DEL 1 JULIO 2008:
    // Orden EHA/451/2008 de 20 de febrero.
    // BOE Numero: 49 de martes, 26 febrero de 2008. Pagina: 11374.
    // ------------------------------------------------------------
    // A. Sociedades anonimas.
    // B. Sociedades de responsabilidad limitada.
    // C. Sociedades colectivas.
    // D. Sociedades comanditarias.
    // E. Comunidades de bienes y herencias yacentes.
    // F. Sociedades cooperativas.
    // G. Asociaciones.
    // H. Comunidades de propietarios en regimen de propiedad horizontal.
    // J. Sociedades civiles, con o sin personalidad juridica.
    // P. Corporaciones Locales.
    // Q. Organismos publicos.
    // R. Congregaciones e instituciones religiosas.
    // S. Organos de la Administracion del Estado y de las Comunidades Autonomas.
    // U. Uniones Temporales de Empresas.
    // V. Otros tipos no definidos en el resto de claves.
    //
    // N. Entidad extranjera. Con o sin personalidad juridica.
    // W. Establecimiento permanente de entidad no residente en territorio espanyol.

    QString cif = cif1.replace ( "-", "" );
    cif = cif.replace ( ".", "" );
    cif = cif.replace ( "_", "" );
    /// Si el CIF tiene menos de 4 caracteres validamos. Ya que igual queremos permitir CIF's Inventados.
    if ( cif.size() < 5 ) {
        return TRUE;
    } // end if
    int valpar = cif[2].digitValue() + cif[4].digitValue() + cif[6].digitValue();
    int valimpar = sumaDigitos ( cif[1].digitValue() * 2 ) + sumaDigitos ( cif[3].digitValue() * 2 ) + sumaDigitos ( cif[5].digitValue() * 2 ) + sumaDigitos ( cif[7].digitValue() * 2 );
    int sumapar = valpar + valimpar;
    QString textsumapar = QString::number ( sumapar );
    int c = textsumapar.right ( 1 ).toInt();
    int d = 10 - c;
    if ( cif[0] == 'N' || cif[0] == 'R' || cif[0] == 'K' || cif[0] == 'P' || cif[0] == 'Q' || cif[0] == 'S' || cif[0] == 'W' ) {
        digit = codigosCIF[d-1];
        if ( cif[8] == codigosCIF[d-1] ) {
            return TRUE;
        } else {
            return FALSE;
        } // end if
    } // end if
    if ( cif[0] == 'A' || cif[0] == 'B' || cif[0] == 'C' || cif[0] == 'D' || cif[0] == 'E' || cif[0] == 'F' || cif[0] == 'G' || cif[0] == 'H' || cif[0] == 'I' || cif[0] == 'J' || cif[0] == 'U' || cif[0] == 'V' ) {
        digit = QString::number ( d % 10 ) [0];
        if ( cif[8].digitValue() == d % 10 ) {
            return TRUE;
        } else {
            return FALSE;
        } // end if
    } //end if
    if ( cif[8] == QChar ( validacionesCIF[d-1] ) || cif[8].digitValue() == d % d ) {
        return TRUE;
    } //end if
    return FALSE;
}



