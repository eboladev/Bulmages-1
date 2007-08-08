/***************************************************************************
 *   Copyright (C) 2005 by Tomeu Borras Riera                              *
 *   tborras@conetxia.com                                                  *
 *   Copyright (C) 2006 by Fco. Javier M. C.                               *
 *   fcojavmc@todo-redes.com                                               *
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

#include <QMenu>
#include <QAction>
#include <QObject>
#include <QMessageBox>
#include <QInputDialog>
#include <QStringList>
#include <QClipboard>
#include <QTextStream>
#include <QTranslator>
#include <QTextCodec>
#include <QLocale>

#include <stdio.h>

#include "company.h"
#include "funcaux.h"
#include "pluginclipboardbf.h"





int entryPoint(QApplication *) {
    _depura("Punto de Entrada del plugin de Clipboard", 0);
        /// Cargamos el sistema de traducciones una vez pasado por las configuraciones generales
        QTranslator *traductor = new QTranslator(0);
        if (confpr->valor(CONF_TRADUCCION) == "locales") {
            traductor->load(QString("pluginclipboardbf_") + QLocale::system().name(),
                            confpr->valor(CONF_DIR_TRADUCCION).toAscii().constData());
        } else {
            QString archivo = "pluginclipboardbf_" + confpr->valor(CONF_TRADUCCION);
           traductor->load(archivo, confpr->valor(CONF_DIR_TRADUCCION).toAscii().constData());
        } // end if
        theApp->installTranslator(traductor);
    _depura("END Punto de Entrada del plugin de Clipboard\n", 0);
    return 0;
}



int SubForm3_SubForm3_Post(SubForm3 *sub) {
    _depura("SubForm3_SubForm3_Post", 0);
   myplugclipboard *subformclip = new myplugclipboard(sub);
   sub->connect(sub, SIGNAL(pintaMenu(QMenu *)), subformclip, SLOT(s_pintaMenu(QMenu *)));
   sub->connect(sub, SIGNAL(trataMenu(QAction *)), subformclip, SLOT(s_trataMenu(QAction *)));
   return 0;
}



myplugclipboard::myplugclipboard(SubForm3 *parent) : QObject(parent) {
}


myplugclipboard::~myplugclipboard(){
}

void myplugclipboard::s_pintaMenu(QMenu *menu) {
    menu->addSeparator();
    menu->addAction(tr("Pegar desde Hoja de Calculo"));
}

void myplugclipboard::s_trataMenu(QAction *action) {
    if (action->text() == tr("Pegar desde Hoja de Calculo")) {
	pegaSXC();
    } // end if
}

void myplugclipboard::pegaSXC() {
    _depura("myplugclipboard::pegaSXC", 0);
        SubForm3 *subform = (SubForm3 *) parent();
	QString clipboard = theApp->clipboard()->text();

	QStringList lineas = clipboard.split("\n");

	/// La primera linea tiene los nombres de las columnas.
	QStringList campos = lineas.at(0).simplified().split(" ");

	/// Calculamos el tamanyo de cada campo.
	int numcampos = campos.size();
	int numchars  = lineas.at(0).size() / numcampos;

	/// Iteramos para cada linea
        for (int i = 1; i < lineas.size() -1 ; ++i) {
		QString cadena_valores = lineas.at(i);

		/// Creamos un elemento en la factura
		SDBRecord  *linea1;
		linea1 = subform->lineaat(subform->rowCount() - 1);
		/// Haciendo el nuevo registro antes nos evitamos problemas de foco.
		subform->nuevoRegistro();

		/// Iteramos para cada columna.
		for (int j = 0; j < campos.size(); ++j) {
			/// Cogemos un valor.
			QString valorcampo = cadena_valores.left(numchars).simplified();
			cadena_valores = cadena_valores.right(cadena_valores.size()- numchars);
			linea1->setDBvalue(campos.at(j), valorcampo);
		} // end for
	} // end for
    _depura("END myplugclipboard::pegaSXC", 0);
}
