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
/// utilizadas de forma regular en el programa. Implementadas en funcaux.cpp
/// Dichas funciones normalmente son de uso general, por lo que es normal ver este
/// archivo incluido en la practica totalidad de los demas ficheros.
#include <QString>
#include <QTextEdit>
#include <QDir>
#include <QTextStream>
#include <QLocale>
#include <QProcess>

#include "funcaux.h"
#include "configuracion.h"


/// Definimos aqui la variable global g_main para que sea accesible desde esta libreria.
QMainWindow *g_main = NULL;


/// Esta funcion permite editar un texto en un QTextEdit y devuelve el texto editado.
QString editaTexto ( QString texto )
{
    QTextEdit * ed = new QTextEdit ( 0 );
    ed->setFixedSize ( 450, 250 );
    ed->setPlainText ( texto );
    g_main->setEnabled ( FALSE );
    ed->show();
    while ( !ed->isHidden() ) {
        theApp->processEvents();
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

/// Proteje cadenas de texto pasandoles una sustitucion de codigos especiales de XML.
QString XMLProtect ( const QString &string )
{
    /*    QString s = string;
        s.replace("&", "&amp;");
        s.replace(">", "&gt;");
        s.replace("<", "&lt;");
        s.replace("\"", "&quot;");
        s.replace("\'", "&apos;");
        return s;
    */
    /// Recorre todo el QString y sustituye los caracteres NO ASCII y
    /// los caracteres que no van muy bien en un XML.

    QString cadena = string;
    QString cadenatmp = "";
    QChar *data = cadena.data();

    /// Cambia tambien otros caracteres no adecuados.
    cadenatmp.replace ( "&", "&#38;" );
    cadenatmp.replace ( ">", "&#62;" );
    cadenatmp.replace ( "<", "&#60;" );
    cadenatmp.replace ( "\"", "&#34;" );
    cadenatmp.replace ( "\'", "&#39;" );

    int i;
    for ( i = 0; i < cadena.length(); i++ ) {
        if ( data->unicode() > 127 ) {
            cadenatmp = cadenatmp + QString ( "&#" ) + QString::number ( data->unicode() ) + QString ( ";" );
        } else if ( data->unicode() == 10 | data->unicode() == 13 ) {
            /// Cambiamos los Intros por el formato HTML.
            cadenatmp = cadenatmp + QString ( "<br />" );
        } else {
            cadenatmp = cadenatmp + QString ( *data );
        } // end if
        ++data;
    } // end if

    return cadenatmp;
}


/// Extiende un string a un numero de cuenta sustituyendo los '.' por ceros.
/// cad = Cadena inicial
/// num1 = Numero de digitos totales de la cuenta.
/// Devuelve un QString con la cuenta extendida al nmero de digitos indicado.
QString extiendecodigo ( QString cad, unsigned int num1 )
{
    _depura ( "funcaux::extiendecodigo", 0, cad + "--" + QString::number ( num1 ) );
    QString cod = cad;
    int num = num1;
    if ( cod.length() < num ) {
        QString str7 ( num - cod.length() + 1, '0' );
        int pos = cod.indexOf ( ".", 0 );
        if ( pos > 0 ) {
            cod.replace ( pos, 1, str7 );
        } // end if
    } // end if
    _depura ( "END funcaux::extiendecodigo", 0 );
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

/// BUG: --- Esta funcion tiene un uso específico de bulmacont y por eso no
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
    system ( cadena.toAscii().data() );
}


/// En la impresi&oacute;n de documentos con bgtrml2pdf esta funci&oacute;n hace casi todo
/// el trabajo de la invocaci&oacute;n de bgtrml2pdf para evitar trabajo duplicado.
void generaPDF ( const QString arch )
{
    _depura ( "generaPDF " + arch, 0 );
    QDir::setCurrent ( confpr->valor ( CONF_DIR_USER ) );
    QString cadsys;

#ifdef WINDOWS

    cadsys = confpr->valor ( CONF_PYTHON ) + " " + confpr->valor ( CONF_PROGDATA ) + "trml2pdf\\bgtrml2pdf " + arch + ".rml > " + confpr->valor ( CONF_DIR_USER ) + arch + ".pdf";
    system ( cadsys.toAscii() );
    _depura ( cadsys, 0 );
    cadsys = confpr->valor ( CONF_FLIP ) + " -u " + confpr->valor ( CONF_DIR_USER ) + arch + ".pdf";
    system ( cadsys.toAscii().data() );
    _depura ( cadsys, 0 );
#else

    cadsys = "bgtrml2pdf " + arch + ".rml > " + arch + ".pdf";
    system ( cadsys.toAscii().data() );
#endif
}


/// Genera un PDF a partir de un RML usando trml2pdf y adem&aacute;s lo muestra con el visor
/// de PDF pasado en la configuraci&oacute;n.
/// arch = Archivo RML.
void invocaPDF ( const QString arch )
{
    generaPDF ( arch );
    QString cadsys = confpr->valor ( CONF_PDF ) + " " + confpr->valor ( CONF_DIR_USER ) + arch + ".pdf &";
    system ( cadsys.toAscii().data() );
}


/// De momento no se usa, pero sirve para enviar documentos por e-mail a un destinatario.
void mailsendPDF ( const QString arch, const QString to, const QString subject, const QString message )
{
    //FIXME: REVISAR PARAMETROS de mailsend o la posibilidad de anyadir otros programas
    //para enviar correo desde la ventana de configuracion del programa.
    QString cadsys = "mailsend -h " + arch + " -d " + to + " -f bulmages@iglues.org -t test@iglues.org -sub " + subject + " -m " + message;
    system ( cadsys.toAscii().data() );
}

QString windowID ( const QString &app )
{

    QString cad = "";

    if ( app != "" ) {
        cad = "xwininfo -int -name \"" + app + "\" | grep xwininfo | awk '{print $4}' > /tmp/xwinfo";
    } else {
        cad = "xwininfo -int | grep Window | awk '{print $4}' > /tmp/xwinfo";
    } // end if

    system ( cad.toAscii() );

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


/// cad = String a presentar como texto de depuracion o como mensaje de error.
/// nivel 0 = normal.
/// nivel 1 = Bajo.
/// nivel 2 = Alto (sale un popup).
/// nivel 4 = Comienza depuracion indiscriminada.
/// nivel 5 = Termina depuracion indiscriminada.
/// nivel 10 = Salida a terminal.
#ifdef DEPURA_DEBUG
void _depura ( const QString &cad, int nivel, const QString &param )
{
    /// Si el objeto confpr no esta creado puede dar segmentation fault.
    if ( confpr == NULL ) {
        return;
    } // end if

    static bool semaforo = 0;


    if ( confpr->valor ( CONF_DEBUG ) == "TRUE" ) {
        static QFile file ( confpr->valor ( CONF_DIR_USER ) + "bulmagesout.txt" );
        static QTextStream out ( &file );
        if ( !semaforo ) {
            if ( !file.open ( QIODevice::WriteOnly | QIODevice::Text ) )
                return;
            semaforo = 1;
        } // end if
        static int supnivel = 0;
        static int indice = 0;
        static QString mensajesanulados[7000];
        static QString clasesanuladas[7000];
        static int indiceclases = 0;

        if ( nivel == 5 ) {
            supnivel = 0;
            nivel = 2;
        } // end if
        if ( nivel == 4 ) {
            supnivel = 2;
            nivel = 2;
        } // end if
        if ( nivel == 0 ) {
            out << cad << " " << param << "\n" << flush;
        } else if ( nivel == 1 ) {
            out << cad << " " << param << "\n" << flush;
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
                                                 QApplication::translate ( "funcaux", "Informacion de depuracion" ),
                                                 cad + " " + param,
                                                 QApplication::translate ( "funcaux", "&Continuar" ),
                                                 QApplication::translate ( "funcaux", "&Omitir" ),
                                                 QApplication::translate ( "funcaux", "Omitir &clase" ),
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

#else

inline void _depura ( const QString &, int , const QString & )
{
    return;
}
#endif

void mensajeInfo ( QString cad )
{
    QMessageBox::information ( NULL,
                               QApplication::translate ( "funcaux", "Informacion del programa" ),
                               cad, QApplication::translate ( "funcaux", "&Continuar" ),
                               QString::null, 0 );
}


void mensajeAviso ( QString cad )
{
    QMessageBox::warning ( NULL,
                           QApplication::translate ( "funcaux", "Aviso del programa" ),
                           cad, QApplication::translate ( "funcaux", "&Continuar" ),
                           QString::null, 0 );
}


void mensajeError ( QString cad )
{
    QMessageBox::critical ( NULL,
                            QApplication::translate ( "funcaux", "Error del programa" ),
                            cad, QApplication::translate ( "funcaux", "&Continuar" ),
                            QString::null, 0 );
}


QString  num2texto ( QString numero, QString moneda, QString singular )
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

/// Carga las traducciones de un archivo concreto.
/// Distingue entre locales o un idioma configurado.
/// Hace la carga y la pone en funcionamiento.
void cargaTraducciones ( const QString &traduccion )
{
    /// Cargamos el sistema de traducciones una vez pasado por las configuraciones generales
    QTranslator * traductor = new QTranslator ( 0 );
    if ( confpr->valor ( CONF_TRADUCCION ) == "locales" ) {
        traductor->load ( traduccion + QString ( "_" ) + QLocale::system().name(),
                          confpr->valor ( CONF_DIR_TRADUCCION ).toAscii().constData() );
    } else {
        QString archivo = traduccion + "_" + confpr->valor ( CONF_TRADUCCION );
        traductor->load ( archivo, confpr->valor ( CONF_DIR_TRADUCCION ).toAscii().constData() );
    } // end if
    theApp->installTranslator ( traductor );
}


