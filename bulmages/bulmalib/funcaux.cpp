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

#include "funcaux.h"
#include "configuracion.h"


/// Definimos aqui la variable global g_main para que sea accesible desde esta libreria.
QMainWindow *g_main = NULL;


/// Esta funcion permite editar un texto en un QTextEdit y devuelve el texto editado.
QString editaTexto(QString texto) {
    QTextEdit *ed = new QTextEdit(0);
    ed->setFixedSize(450, 250);
    ed->setPlainText(texto);
    g_main->setEnabled(FALSE);
    ed->show();
    while (!ed->isHidden()) {
        theApp->processEvents();
    } // end while
    g_main->setEnabled(TRUE);
    QString vuelta = ed->toPlainText();
    return vuelta;
}


/// Proteje cadenas de texto pasandoles una sustitucion de codigos especiales de XML.
QString XMLProtect(const QString& string) {
    QString s = string;
    s.replace("&", "&amp;");
    s.replace(">", "&gt;");
    s.replace("<", "&lt;");
    s.replace("\"", "&quot;");
    s.replace("\'", "&apos;");
    return s;
}


/// Extiende un string a un numero de cuenta sustituyendo los '.' por ceros.
/// cad = Cadena inicial.
/// num1 = Numero de digitos totales de la cuenta.
/// Devuelve un string con el codigo de cuenta extendido al numero de digitos indicado.
string extiendecodigo(string cad, unsigned int num1) {
    string cod = cad;
    unsigned int num = num1;
    if (cod.length() < num) {
        string str7(num - cod.length() + 1, '0');
        int pos = cod.find(".", 0);
        if (pos > 0) {
            cod.replace(pos, 1, str7);
        } // end if
    } // end if
    return (cod);
}


/// Extiende un string a un numero de cuenta sustituyendo los '.' por ceros.
/// cad = Cadena inicial
/// num1 = Numero de digitos totales de la cuenta.
/// Devuelve un QString con la cuenta extendida al nmero de digitos indicado.
QString extiendecodigo(QString cad, unsigned int num1) {
    QString cod = cad;
    int num = num1;
    if (cod.length() < num) {
        string str7(num - cod.length() + 1, '0');
        int pos = cod.indexOf(".", 0);
        if (pos > 0) {
            cod.replace(pos, 1, str7.c_str());
        } // end if
    } // end if
    return (cod);
}


/// Redondeo de numeros en punto flotante.
/// n = Numero a redondear
/// d = Numero de decimales
/// Devuelve numero redondeado
float fround(float n, unsigned d) {
    return floor(n * pow(10., d) + .5) / pow (10., d);
}


/// Esta funcion convierte un numero con decimales a un entero. Usando la regla
/// que si el el primer decimal es mayor o igual a 5 se devuelve el entero superior. AInteligentesView
/// valor = Numero a convertir.
int roundI(double valor) {
    int retorno;
    double mayor = floor(valor);
    if ((mayor - valor) >= 0.5)
        retorno = (int) mayor - 1;
    else
        retorno = (int) mayor;
    return retorno;
}


/// Procesa el string pasado como parametro y devuelve una estructura del tipo QDate
/// Esta funcion extiende la fecha pasada como parametro
/// QString y devuelve la fecha en formato QDate.
/// fechaintro string con la fecha a ser normalizada.
QDate normalizafecha(QString fechaintro) {
    QDate fecharesult;
    int d, M, y;
    switch (fechaintro.length()) {
    case 4: /// fecha tipo ddMM
        d = fechaintro.mid(0, 2).toInt();
        M = fechaintro.mid(2, 2).toInt();
        y = QDate::currentDate().year();
        break;
    case 5:/// fecha tipo dd/MM
        d = fechaintro.mid(0, 2).toInt();
        M = fechaintro.mid(3, 2).toInt();
        y = QDate::currentDate().year();
        break;
    case 6: /// fecha tipo ddMMyy
        d = fechaintro.mid(0, 2).toInt();
        M = fechaintro.mid(2, 2).toInt();
        y = 2000 + fechaintro.mid(4, 2).toInt();
        break;
    case 8:
        if (fechaintro.contains("/", Qt::CaseSensitive) || fechaintro.contains("-", Qt::CaseSensitive)) {
            /// fecha tipo dd/MM/yy o dd-MM-yy
            d = fechaintro.mid(0, 2).toInt();
            M = fechaintro.mid(3, 2).toInt();
            y = 2000 + fechaintro.mid(6, 2).toInt();
        } else {
            /// o bien tipo ddMMyyyy
            d = fechaintro.mid(0,2).toInt();
            M = fechaintro.mid(2,2).toInt();
            y = fechaintro.mid(4,4).toInt();
        } // end if
        break;
    case 10: /// fecha tipo dd/MM/yyyy
        d = fechaintro.mid(0, 2).toInt();
        M = fechaintro.mid(3, 2).toInt();
        y = fechaintro.mid(6, 4).toInt();
        break;
    default:
        d = QDate::currentDate().day();
        M = QDate::currentDate().month();
        y = QDate::currentDate().year();
    } // end switch
    if (!fecharesult.setYMD(y, M, d))
        fecharesult = QDate::currentDate();
    return (fecharesult);
}


/// Esta funcion ajusta el codigo pasado al numero de digitos especificado.
/// Para ello bsca los ceros intermedios y los amplia hasta que el numero de caracteres sea el deseado.
/// Lo hace a partir del quinto digito por defecto. Aunque este parametro deberia ser configurable.
/// cod = string con el codigo actual.
/// num1 = numero de digitos que debe tener el codigo final.
/// Devuelve un QString con el codigo ajustado.

/// BUG: --- Esta funcion tiene un uso específico de bulmacont y por eso no
/// deberia estar en bulmalib.
QString ajustacodigo(QString cad, unsigned int num1) {
    QString cod = cad;
    unsigned int longcad = cad.length();
    if (longcad > 4) {
        if (longcad < num1) {
            string str7(num1 - longcad, '0');
            cod = cad.left(4);
            cod += QString(str7.c_str());
            cod += cad.right(longcad - 4);
        } // end if
        if (longcad > num1) {
            cod = cad.left(4);
            cod += cad.right(num1 - 4);
        } // end if
    } // end if
    return (cod);
}


/// Sustituye cadenas en un archivo.
void reemplazaarchivo(QString archivo, QString texto1, QString texto2, QString archivo2) {
    QString cadena = " sed -e \"s&" + texto1 + "&" + texto2 + "&g\"  " + archivo + " > " + archivo2 + "";
    system(cadena.toAscii().data());
}


/// En la impresi&oacute;n de documentos con trml2pdf esta funci&oacute;n hace casi todo
/// el trabajo de la invocaci&oacute;n de trml2pdf para evitar trabajo duplicado.
void generaPDF(const QString arch) {
    _depura("generaPDF " + arch, 0);
    QDir::setCurrent(confpr->valor(CONF_DIR_USER));
    QString cadsys;

#ifdef WINDOWS

    cadsys = confpr->valor(CONF_PYTHON) + " " + confpr->valor(CONF_PROGDATA) + "trml2pdf\\bgtrml2pdf " + arch + ".rml > " + confpr->valor(CONF_DIR_USER) + arch + ".pdf";
    system(cadsys.toAscii());
    _depura(cadsys, 0);
    cadsys = confpr->valor(CONF_FLIP) + " -u " + confpr->valor(CONF_DIR_USER) + arch + ".pdf";
    system(cadsys.toAscii().data());
    _depura(cadsys, 0);
#else

    cadsys = "bgtrml2pdf " + arch + ".rml > " + arch + ".pdf";
    system(cadsys.toAscii().data());
#endif
}


/// Genera un PDF a partir de un RML usando trml2pdf y adem&aacute;s lo muestra con el visor
/// de PDF pasado en la configuraci&oacute;n.
/// arch = Archivo RML.
void invocaPDF(const QString arch) {
    generaPDF(arch);
    QString cadsys = confpr->valor(CONF_PDF) + " " + confpr->valor(CONF_DIR_USER) + arch + ".pdf &";
    system(cadsys.toAscii().data());
}


/// De momento no se usa, pero sirve para enviar documentos por e-mail a un destinatario.
void mailsendPDF(const QString arch, const QString to, const QString subject, const QString message) {
    //FIXME: REVISAR PARAMETROS de mailsend o la posibilidad de anyadir otros programas
    //para enviar correo desde la ventana de configuracion del programa.
    QString cadsys = "mailsend -h " + arch + " -d " + to + " -f bulmages@iglues.org -t test@iglues.org -sub " + subject + " -m " + message;
    system(cadsys.toAscii().data());
}


/// cad = String a presentar como texto de depuracion o como mensaje de error.
/// nivel 0 = normal.
/// nivel 1 = Bajo.
/// nivel 2 = Alto (sale un popup).
/// nivel 4 = Comienza depuracion indiscriminada.
/// nivel 5 = Termina depuracion indiscriminada.
/// nivel 10 = Salida a terminal.
void _depura(QString cad, int nivel, QString param) {
    /// Si el objeto confpr no esta creado puede dar segmentation fault.
    if (confpr == NULL) {
        return;
    } // end if

    static bool semaforo = 0;
    static QFile file(confpr->valor(CONF_DIR_USER) + "bulmagesout.txt");
    static QTextStream out(&file);

    if (confpr->valor(CONF_DEBUG) == "TRUE") {
        if (!semaforo) {
            if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
                return;
            semaforo = 1;
        } // end if
        static int supnivel = 0;
        static int indice = 0;
        static QString mensajesanulados[7000];
        static QString clasesanuladas[7000];
        static int indiceclases = 0;

        if (nivel == 5) {
            supnivel = 0;
            nivel = 2;
        } // end if
        if (nivel == 4) {
            supnivel = 2;
            nivel = 2;
        } // end if
        if (nivel == 0) {
            out << cad << " " << param << "\n" << flush;
        } else if (nivel == 1) {
            out << cad << " " << param << "\n" << flush;
        } // end if
        for (int i = 0; i < indice; i++) {
            if (cad == mensajesanulados[i]) {
                return;
            } // end if
        } // end for
        for (int i = 0; i < indiceclases; i++) {
            if (cad.left(cad.indexOf("::")) == clasesanuladas[i]) {
                return;
            } // end if
        } // end for

        if (nivel == 2 || (supnivel == 2 && nivel == 0) || nivel == 3) {
            out << cad << " " << param << "\n" << flush;
            int err = QMessageBox::information(NULL,
                                               QApplication::translate("funcaux", "Informacion de depuracion"),
                                               cad + " " + param,
                                               QApplication::translate("funcaux", "&Continuar"),
                                               QApplication::translate("funcaux", "&Omitir"),
                                               QApplication::translate("funcaux", "Omitir &clase"),
                                               0, 1);
            if (err == 1) {
                mensajesanulados[indice++] = cad;
            } // end if
            if (err == 2) {
                clasesanuladas[indiceclases++] = cad.left(cad.indexOf("::"));
            } // end if
        } // end if

        file.flush();
    } else {
        if (nivel == 2) {
            out << cad << " " << param << "\n" << flush;
            QMessageBox::question(NULL,
                                  QApplication::translate("funcaux", "Informacion de depuracion"),
                                  cad, QApplication::translate("funcaux", "&Continuar"),
                                  QString::null, 0);
        } // end if
        if (nivel == 10) { /// Saca los mensajes por la consola de errores.
            QString cadenasalida;
            cadenasalida = "--> " + cad + " <--\n";
            fprintf(stderr, cadenasalida.toAscii().constData());
        } // end if
    } // end if
}


void mensajeInfo(QString cad) {
    QMessageBox::information(NULL,
                             QApplication::translate("funcaux", "Atencion"),
                             cad, QApplication::translate("funcaux", "&Continuar"),
                             QString::null, 0);
}

