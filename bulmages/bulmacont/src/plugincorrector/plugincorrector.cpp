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

#include <q3popupmenu.h>
#include <q3dockwindow.h>
#include <q3dockwindow.h>
#include <q3mainwindow.h>

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
    _depura("Estoy dentro del plugin\n", 0);
    empresa *emp = bcont->empresaactual();
    /// Vamos a probar con un docwindow.
    Q3DockWindow *doc1  = new Q3DockWindow(Q3DockWindow::OutsideDock, bcont, "Corrector");
    doc1->setGeometry(100, 100, 100, 500);
    doc1->setFixedExtentWidth(200);

    correctorwidget *corr = new correctorwidget(doc1, "correctorplugin");
    corr->setEmpresa(emp);
    corr->dock = doc1;

    doc1->setWidget(corr);
    doc1->setResizeEnabled(TRUE);
    doc1->setMovingEnabled(TRUE);
    //bcont->moveDockWindow(doc1,Qt::DockRight);
    doc1->dock();
    doc1->hide();
    doc1->showMaximized();

    /// El menu de empresa.
    QAction *viewCorrector = new QAction("&Corrector", 0);
    viewCorrector->setCheckable(TRUE);
    viewCorrector->setChecked(TRUE);
    viewCorrector->setStatusTip("Muestra/oculta el corrector");
    viewCorrector->setWhatsThis("Corrector.\n\nMuestra/oculta el corrector");
    QObject::connect(viewCorrector, SIGNAL(toggled(bool)), corr, SLOT(cambia(bool)));
    //viewCorrector->addTo(bcont->pVerMenu());
    _depura("END entryPoint\n", 0);
}

