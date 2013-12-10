/****************************************************************************
**
** Copyright (C) 2009 Fco. Javier M. C. <fcojavmc@todo-redes.com>
**
** Copyright (C) 2008 Nokia Corporation and/or its subsidiary(-ies).
** Contact: Qt Software Information (qt-info@nokia.com)
**
** This file is part of the example classes of the Qt Toolkit.
**
** No Commercial Usage
** This file contains pre-release code and may only be used for
** evaluation and testing purposes.  It may not be used for commercial
** development.  You may use this file in accordance with the terms and
** conditions contained in the either Technology Preview License
** Agreement or the Beta Release License Agreement.
**
** GNU General Public License Usage
** Alternatively, this file may be used under the terms of the GNU
** General Public License versions 2.0 or 3.0 as published by the Free
** Software Foundation and appearing in the file LICENSE.GPL included in
** the packaging of this file.  Please review the following information
** to ensure GNU General Public Licensing requirements will be met:
** http://www.fsf.org/licensing/licenses/info/GPLv2.html and
** http://www.gnu.org/copyleft/gpl.html.  In addition, as a special
** exception, Nokia gives you certain additional rights. These rights
** are described in the Nokia Qt GPL Exception version 1.3, included in
** the file GPL_EXCEPTION.txt in this package.
**
** Qt for Windows(R) Licensees
** As a special exception, Nokia, as the sole copyright holder for Qt
** Designer, grants users of the Qt/Eclipse Integration plug-in the
** right for the Qt/Eclipse Integration to link to functionality
** provided by Qt Designer and its related libraries.
**
** If you are unsure which license is appropriate for your use, please
** contact the sales department at qt-sales@nokia.com.
**
****************************************************************************/

#include <QtGui/QtGui>

#include "bgserver.h"
#include "bgbulmages.h"
#include "blfunctions.h"
#include "blconfiguration.h"
#include "mainwindow.h"


int main ( int argc, char **argv )
{

    Q_INIT_RESOURCE(bgbulmages);

    /// Leemos la configuracion que luego podremos usar siempre.
    initConfiguration ("bulmages");

    /// Inicializa el sistema de traducciones 'gettext'.
    setlocale(LC_ALL, "");
    blBindTextDomain ("bulmages", g_confpr->value(CONF_DIR_TRADUCCION).toLatin1().constData());
    blTextDomain ("bulmages");

    /// Creamos la aplicacion principal.
    QApplication app(argc, argv);

    /// Definimos la codificaci&oacute;n a Unicode.
    QTextCodec::setCodecForLocale ( QTextCodec::codecForName ( "UTF-8" ) );

   /// Leemos los argumentos pasados por linea de comandos.
   QString argument=""; 
   for(int i = 1; i < argc; i++) {
      argument += argv[i];
   } // end for
   
   /// Si el argumento -h se pasa mostramos la ayuda y salimos.
   if (argument.contains("-h")) {
      qDebug() << " Options -h show help" << endl << "-s only server. No ui interfac" << endl;
      exit(1);
   } // end if
   
   BgBulmaGes *bhges;
   
   qDebug() << argument << endl;

   /// Si no hay argumento -s (solo server) entonces creamos la interficie grafica. Y el Tray Icon
   if (!argument.contains("-s")) {
        qDebug() << "Tray Icon" << endl;
	if (!QSystemTrayIcon::isSystemTrayAvailable()) {
	    QMessageBox::critical(0, _("Bandeja"),
				    _("No se detecta ninguna bandeja en el sistema"));
	    return 1;
	} // end if
	QApplication::setQuitOnLastWindowClosed(false);

	bhges = new BgBulmaGes();
	bhges->hide();
	

  } // end if
      
    ///Creamos el servidor y lo pasamos.
    BgServer * server = new BgServer();
    qDebug() << "En espera " << endl;

    /// Damos el control de la aplicacion al QApplication
    return app.exec();
}


