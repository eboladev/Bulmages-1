/***************************************************************************
 *   Copyright (C) 2003 by Josep Burcion                                   *
 *   josep@burcion.com                                                     *
 *   http://www.iglues.org Asociacion Iglues -- Contabilidad Linux         *
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

#ifndef MODGEN_H
#define MODGEN_H

#include <QtCore/QFile>
#include <QtCore/QThread>
#include <QtWidgets/QProgressDialog>
#include <QtCore/QEvent>
#include <QtCore/QTextStream>
#include <QtCore/QObject>

#include "blfunctions.h"


enum {
    sleep10 = 1001,
    sleep3 = 1002,
    acabado = 1003
};


/// Clase base para rellenar todos los formularios oficiales desde el pdf de www.aeat.es
class BC_EXPORT BcPsThread : public QThread
{
public:
    BcPsThread ( QString, QString, QProgressDialog * );
    void run();
    QString m_pdfname, m_tempname;
    QProgressDialog *m_progressdia;
};


class BC_EXPORT BcPsProgressDialog : public QProgressDialog
{
public:
    BcPsProgressDialog ( QString etiqueta, QString btcancelar, int minimo, int maximo, QWidget *widget, Qt::WindowFlags bandera );
    //void customEvent(QCustomEvent *);
};


/// Clase base para modelos de declaraci&oacute;n de la agencia tributaria en formato postscript.
/** Utiliza los formularios oficiales obtenidos de www.aeat.es convertidos de pdf a postscript.
    funciona insertando c&oacute;digo postscript para escribir los n&uacute;meros en las casillas.
    Usa la fuente Courier-Bold a 12 puntos. */
class BC_EXPORT BcPsGenerateModel : public QObject
{
public:
    BcPsGenerateModel();
    ~BcPsGenerateModel();
    void download_form ( QWidget *, QString, QString );
    BcPsThread *convierte_a_postscript;
    /// Genera 2 cadenas con parte entera y fraccionaria del float.
    void formatdigits ( QString *, QString *, float );
    /// Escribe el texto "a la izquierda de" (o sea, alineado a la derecha).
    void escrizq ( QString, int, int );
    /// Funcion sobrecargada, igual que antes pero primero convierte float en cadena.
    void escrizq ( float, int, int );
    /// Igual que escrizq pero alineado a la izquierda.
    void escrder ( QString, int, int );
    /// Sobrecargada igual que la anterior.
    void escrder ( float, int, int );
    ///escribe primera cadena a la izquierda y segunda a la derecha del punto dado.
    void escrizqder ( QString, QString, int, int );
    /// Igual, pero primero convierte float en 2 cadenas con formatdigits.
    void escrizqder ( float, int, int );
    /// Pone una cruz en la casilla dada por sus coordenadas.
    void marca_casilla ( QString, int, int );
    /// Inserta una marca de agua en el documento con la palabra BORRADOR bien grande, que
    /// sea vea!!
    void marcadeagua_borrador();
    QFile m_fichlec;
    QFile m_fich;
    QTextStream m_output;
    /// True si el documento a generar es un borrador (mucho m&aacute;s r&aacute;pido de
    /// generar, pero sin n&uacute;mero de serie v&aacute;lido)
    bool m_es_borrador;
};

#endif

