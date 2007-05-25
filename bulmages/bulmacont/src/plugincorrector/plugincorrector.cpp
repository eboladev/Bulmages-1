/***************************************************************************
 *   Copyright (C) 2005 by Tomeu Borras Riera                              *
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

#include <QAction>
#include <QMessageBox>
#include <QStringList>
#include <QWidget>
#include <QIcon>
#include <QApplication>
#include <QObject>

#include "plugincorrector.h"
#include "correctorwidget.h"
#include "empresa.h"


void entryPoint(Bulmacont *bcont) {
    _depura("Entrada del plugin Corrector", 10);
    empresa *emp = bcont->empresaactual();
    /// Vamos a probar con un docwindow.
    QDockWidget *doc1 = new QDockWidget("Corrector", bcont);
    doc1->setFeatures(QDockWidget::AllDockWidgetFeatures);

    doc1->setGeometry(100, 100, 100, 500);
    doc1->resize(330, 400);
    bcont->addDockWidget(Qt::RightDockWidgetArea, doc1);
    doc1->show();

    correctorwidget *corr = new correctorwidget(doc1);
    corr->setEmpresa(emp);
    corr->dock = doc1;

    doc1->setWidget(corr);

    /// A&ntilde;ade en el men&uacute; del programa la opci&oacuteMn para
    /// acceder al corrector.
    QAction *viewCorrector = new QAction("&Corrector", 0);
    viewCorrector->setCheckable(TRUE);
    viewCorrector->setChecked(TRUE);
    viewCorrector->setStatusTip("Muestra/oculta el corrector");
    viewCorrector->setWhatsThis("Corrector.\n\nMuestra/oculta el corrector");
    QObject::connect(viewCorrector, SIGNAL(toggled(bool)), corr, SLOT(cambia(bool)));
    bcont->mui_MenuVer()->addSeparator();
    bcont->mui_MenuVer()->addAction(viewCorrector);
    _depura("Iniciado correctamente el plugin Corrector", 10);
}

