/***************************************************************************
 *   Copyright (C) 2007 by Fco. Javier M. C.                               *
 *   fcojavmc@todo-redes.com                                               *
 *                                                                         *
 *   Copyright (C) 2007 by Tomeu Borras Riera                              *
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

#include <stdio.h>

#include <QFile>
#include <QTextStream>
#include <QTranslator>
#include <QTextCodec>
#include <QLocale>

#include "pluginsubformods.h"
#include "funcaux.h"


int entryPoint(QApplication *bges) {
    _depura("Punto de Entrada del plugin de Subformods\n", 0);
        /// Cargamos el sistema de traducciones una vez pasado por las configuraciones generales
        QTranslator *traductor = new QTranslator(0);
        if (confpr->valor(CONF_TRADUCCION) == "locales") {
            traductor->load(QString("pluginsubformods_") + QLocale::system().name(),
                            confpr->valor(CONF_DIR_TRADUCCION).toAscii().constData());
        } else {
            QString archivo = "pluginsubformods_" + confpr->valor(CONF_TRADUCCION);
           traductor->load(archivo, confpr->valor(CONF_DIR_TRADUCCION).toAscii().constData());
        } // end if
        theApp->installTranslator(traductor);
    _depura("END Punto de Entrada del plugin de Subformods\n", 0);
    return 0;
}


myplugsubformods::myplugsubformods(SubForm3 *parent) : QObject(parent) {
}


myplugsubformods::~myplugsubformods(){
}

void myplugsubformods::s_pintaMenu(QMenu *menu) {
    menu->addSeparator();
    QAction *ajustac = menu->addAction(tr("Exportar a hoja de calculo"));
}

void myplugsubformods::s_trataMenu(QAction *action) {
    _depura("myplugsubformods::s_trataMenu", 0);
    if (action->text() == tr("Exportar a hoja de calculo")) {
	sacaods();
    } // end if
}

void myplugsubformods::sacaods() {
    _depura("myplugsubformods::sacaods", 0);

    QString archivod = confpr->valor ( CONF_DIR_USER ) + "listadoods.py";
    SubForm3 * subf = (SubForm3 *) parent();


	QString fitxersortidatxt = "";

	fitxersortidatxt += "#!/usr/bin/python\n";
	fitxersortidatxt += "# -*- coding: utf8 -*-\n";
	fitxersortidatxt += "\n";
	fitxersortidatxt += "import ooolib\n";
	fitxersortidatxt += "\n";

	fitxersortidatxt += "# Crea el documento\n";
	fitxersortidatxt += "doc = ooolib.Calc(\"Listado\")\n";

	int y=1;
        int x = 1;

    /// Sacamos las cabeceras
    for (int h = 0; h < subf->mui_listcolumnas->rowCount(); ++h) {
        if (subf->mui_listcolumnas->item(h, 0)->checkState() == Qt::Checked) {

			fitxersortidatxt += "# Fila "+ QString::number ( y ) +"\n";

			QString textocabecera = (subf->mui_listcolumnas->item(h, 2)->text());
			textocabecera.replace(QString("\n"), QString("\\n\\\n"));

			fitxersortidatxt += "doc.set_cell_property('bold', True)\n";
    	    		fitxersortidatxt += "doc.set_cell_value(" + QString::number(x++) + "," + QString::number(y) + ", 'string', '" + textocabecera + "')\n";
			fitxersortidatxt += "doc.set_cell_property('bold', False)\n";
			fitxersortidatxt += "\n";

        } // end if
    } // end for


    y += 2;

    /// Sacamos el contenido
    for (int i = 0; i < subf->mui_list->rowCount(); ++i) {

        int x = 1;
        for (int j = 0; j < subf->mui_listcolumnas->rowCount(); ++j) {

            if (subf->mui_listcolumnas->item(j, 0)->checkState() == Qt::Checked) {

			fitxersortidatxt += "# Fila "+ QString::number ( y ) +"\n";

			QString textocontenido = (subf->mui_list->item(i, j)->text());
			textocontenido.replace(QString("\n"), QString("\\n\\\n"));

    	    		fitxersortidatxt += "doc.set_cell_value(" + QString::number(x++) + "," + QString::number(y) + ", 'string', '" + textocontenido + "')\n";
			fitxersortidatxt += "\n";

            } // end if
        } // end forXMLProtect(subf->mui_list->item(i, j)->text())
        y++;
    } // end for

	fitxersortidatxt += "doc.save(\"listadoods.ods\")\n";

	QString cadena ="rm "+confpr->valor ( CONF_DIR_USER ) + "listadoods.ods";
	system ( cadena.toAscii() );
	cadena ="rm "+ archivod;
	system ( cadena.toAscii() );

	QFile file ( archivod );
	if ( file.open ( QIODevice::WriteOnly ) )  {
		QTextStream stream ( &file );
		stream.setCodec("UTF-8");
		stream << fitxersortidatxt;
		file.close();
	} // end if

	cadena= " cd "+confpr->valor ( CONF_DIR_USER ) +"; python "+archivod;
	system ( cadena.toAscii() );
	cadena = "oocalc "+confpr->valor ( CONF_DIR_USER ) + "listadoods.ods &";
	system ( cadena.toAscii() );

    _depura("END myplugsubformods::sacaods", 0);
}


int SubForm3_SubForm3_Post(SubForm3 *sub) {
   myplugsubformods *subformods = new myplugsubformods(sub);
   sub->QObject::connect(sub, SIGNAL(pintaMenu(QMenu *)), subformods, SLOT(s_pintaMenu(QMenu *)));
   sub->QObject::connect(sub, SIGNAL(trataMenu(QAction *)), subformods, SLOT(s_trataMenu(QAction *)));
   return 0;
}

