/***************************************************************************
 *   Copyright (C) 2004 by Tomeu Borras Riera                              *
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
#include <QObject>
#include <QMessageBox>
#include <QToolButton>
#include <QImage>
#include <QLabel>
#include <QPixmap>
//#include <QHBoxLayout>
//#include <QToolBar>
#include <QFrame>

#include "pluginadocumental.h"
#include "empresa.h"
#include "asiento1view.h"
#include "adocumental.h"

/// Inclusion de imagenes.
#include "napuntedoc.xpm"
#include "ndoc.xpm"


void entryPoint(Bulmacont *bcont) {
    _depura("Entrada del plugin ADocumental.", 10);

    QPixmap *img = new QPixmap(napuntedoc);
    QPixmap *img1 = new QPixmap(ndoc);

    empresa *emp = bcont->empresaactual();
    Asiento1View *intapunts = emp->intapuntsempresa();

    myplugin1 *pub = new myplugin1(emp);
    intapunts->hide();


    QToolButton *boton1 = new QToolButton();
    QAction *botonaction1 = new QAction(QIcon(*img), "" , 0);
    boton1->addAction(botonaction1);

    intapunts->hboxLayout1->addWidget(boton1);


    /*
        QHBoxLayout *layoutPlugins = intapunts->layoutPlugins;
        QToolButton *m_adocumental = new QToolButton(intapunts);
     
        m_adocumental->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
        m_adocumental->setIcon(QIcon(*img));
        layoutPlugins->addWidget(m_adocumental);
     
        QObject::connect(m_adocumental, SIGNAL(clicked()), pub, SLOT(boton_adjuntar()));
        QToolButton *m_adocumental1 = new QToolButton(intapunts);
     
        m_adocumental1->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
        m_adocumental1->setIcon(QIcon(*img1));
        layoutPlugins->addWidget(m_adocumental1);
     
        QObject::connect(m_adocumental1, SIGNAL(clicked()), pub, SLOT(boton_nuevoasientodocumental()));
        intapunts->showMaximized();
    */
    /// Hacemos la entrada de menu.
    QAction *ArchDoc = new QAction("&Archivo documental", 0);
    ArchDoc->setStatusTip("Archivo documental");
    ArchDoc->setWhatsThis("Archivo documental");
    QObject::connect(ArchDoc, SIGNAL(activated()), pub, SLOT(archDoc()));

    ///TODO: Hay que poner la opcion dentro del menu HERRAMIENTAS.
    //bcont->pHerramientas()->insertSeparator();
    //ArchDoc->addTo(bcont->pHerramientas());

    /// Creamos el men&uacute;.
    QMenu *pPluginMenu = new QMenu("&Plugin documental");

    pPluginMenu->addAction(ArchDoc);
    /// A&ntilde;adimos la nueva opci&oacute;n al men&uacute; principal del programa.
    bcont->menuBar()->addMenu(pPluginMenu);
}

