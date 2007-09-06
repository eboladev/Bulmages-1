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
#include <QTranslator>
#include <QTextCodec>
#include <QLocale>

#include <stdio.h>

 #include "plugincuadrante.h"
#include "company.h"
#include "funcaux.h"
#include "facturaview.h"
#include "presupuestoview.h"
#include "pedidoclienteview.h"
#include "albaranclienteview.h"
#include "pedidoproveedorview.h"
#include "cuadranteview.h"

myplugin4::myplugin4() {}


myplugin4::~myplugin4() {}


void myplugin4::elslot() {
   _depura("myplugin4::elslot", 0);
	CuadranteView *cuad = new CuadranteView((Company *)empresaBase(), 0);
	empresaBase()->pWorkspace()->addWindow(cuad);
        cuad->show();
   _depura("END myplugin4::elslot", 0);
}

void myplugin4::inicializa(Bulmafact *bges) {
    /// Creamos el men&uacute;.
    setEmpresaBase(bges->getcompany());
    m_bulmafact = bges;
    QMenu *pPluginMenu;
    /// Miramos si existe un menu Herramientas
    pPluginMenu = bges->menuBar()->findChild<QMenu *>("Herramientas");

    /// Creamos el men&uacute;.
    if (!pPluginMenu) {
        pPluginMenu = new QMenu("&Herramientas", bges->menuBar());
        pPluginMenu->setObjectName(QString::fromUtf8("Herramientas"));
    } // end if

    QAction *accion = new QAction("&Cuadrantes", 0);
    accion->setStatusTip("Permite realizar Plannings Laborales");
    accion->setWhatsThis("Podra destinar los trabajadores a distintos almacenes");
    connect(accion, SIGNAL(activated()), this, SLOT(elslot()));
    pPluginMenu->addAction(accion);
    /// A&ntilde;adimos la nueva opci&oacute;n al men&uacute; principal del programa.
    bges->menuBar()->insertMenu(bges->menuVentana->menuAction(), pPluginMenu);
}


void entryPoint(Bulmafact *bges) {
        /// Cargamos el sistema de traducciones una vez pasado por las configuraciones generales
        QTranslator *traductor = new QTranslator(0);
        if (confpr->valor(CONF_TRADUCCION) == "locales") {
            traductor->load(QString("plugincuadrante_") + QLocale::system().name(),
                            confpr->valor(CONF_DIR_TRADUCCION).toAscii().constData());
        } else {
            QString archivo = "plugincuadrante_" + confpr->valor(CONF_TRADUCCION);
           traductor->load(archivo, confpr->valor(CONF_DIR_TRADUCCION).toAscii().constData());
        } // end if
        theApp->installTranslator(traductor);

    myplugin4 *plug = new myplugin4();
    plug->inicializa(bges);
}


int AlmacenView_AlmacenView(AlmacenView *alm) {
   _depura("esxtoy en la clase almacen", 0);

        alm->addDBCampo("aperturaalmacen", DBCampo::DBvarchar, DBCampo::DBNothing,  "Apertura Mañana");
        alm->addDBCampo("cierrealmacen", DBCampo::DBvarchar, DBCampo::DBNothing, "Cierre Mañana");
        alm->addDBCampo("apertura1almacen", DBCampo::DBvarchar, DBCampo::DBNothing,  "Apertura Tarde");
        alm->addDBCampo("cierre1almacen", DBCampo::DBvarchar, DBCampo::DBNothing, "Cierre Tarde");


// ---------------
    /// Creamos el horario de mañana.
    QHBoxLayout *hboxLayout160 = new QHBoxLayout();
    hboxLayout160->setSpacing(2);
    hboxLayout160->setMargin(0);
    hboxLayout160->setObjectName(QString::fromUtf8("hboxLayout16"));

    /// Apertura
    QLabel *textLabel2_9_26 = new QLabel(alm->mui_frameplugin);
    textLabel2_9_26->setObjectName(QString::fromUtf8("textLabel2_9_2"));
    textLabel2_9_26->setText("Apertura Mañanas");
    hboxLayout160->addWidget(textLabel2_9_26);

    QLineEdit *horain = new QLineEdit(alm);
    horain->setObjectName("mui_aperturaalmacen");
    hboxLayout160->addWidget(horain);

    /// Cierre
    QLabel *textLabel2_9_27 = new QLabel(alm->mui_frameplugin);
    textLabel2_9_27->setText("Cierre Mañanas");
    hboxLayout160->addWidget(textLabel2_9_27);

    QLineEdit *horafin = new QLineEdit(alm);
    horafin->setObjectName("mui_cierrealmacen");

    hboxLayout160->addWidget(horafin);


// ---------------

// ---------------
    /// Creamos el horario de tardes.
    QHBoxLayout *hboxLayout170 = new QHBoxLayout();
    hboxLayout170->setSpacing(2);
    hboxLayout170->setMargin(0);
    hboxLayout170->setObjectName(QString::fromUtf8("hboxLayout17"));

    /// Apertura
    QLabel *textLabel2_9_28 = new QLabel(alm->mui_frameplugin);
    textLabel2_9_28->setText("Apertura Tardes");
    hboxLayout170->addWidget(textLabel2_9_28);

    QLineEdit *horaint = new QLineEdit(alm);
    horaint->setObjectName("mui_apertura1almacen");
    hboxLayout170->addWidget(horaint);

    /// Cierre
    QLabel *textLabel2_9_29 = new QLabel(alm->mui_frameplugin);
    textLabel2_9_29->setText("Cierre Tardes");
    hboxLayout170->addWidget(textLabel2_9_29);

    QLineEdit *horafint = new QLineEdit(alm);
    horafint->setObjectName("mui_cierre1almacen");

    hboxLayout170->addWidget(horafint);


// ---------------


    /// Comprobamos que exista el layout.
       QVBoxLayout *m_hboxLayout1 = alm->mui_frameplugin->findChild<QVBoxLayout *>("hboxLayout1");
       if (!m_hboxLayout1) {
                m_hboxLayout1 = new QVBoxLayout(alm->mui_frameplugin);
                m_hboxLayout1->setSpacing(0);
                m_hboxLayout1->setMargin(0);
                m_hboxLayout1->setObjectName(QString::fromUtf8("hboxLayout1"));
       } // end if
//       m_hboxLayout1->addWidget(horain);
       m_hboxLayout1->addLayout(hboxLayout160);
       m_hboxLayout1->addLayout(hboxLayout170);
    return 0;
}


