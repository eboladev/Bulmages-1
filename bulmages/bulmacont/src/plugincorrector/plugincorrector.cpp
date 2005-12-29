#include <stdio.h>
#include "plugincorrector.h"
#include "correctorwidget.h"
#include "empresa.h"

#include <q3popupmenu.h>
#include <qaction.h>
#include <qobject.h>
#include <qmessagebox.h>
#include <q3dockwindow.h>
#include <q3mainwindow.h>


//#include <qwidgetplugin.h>
#include <qstringlist.h>
#include <qwidget.h>
#include <qicon.h>
#include <qapplication.h>
#include <qobject.h>
#include <q3dockwindow.h>

void entryPoint(Bulmages01 *bges) {
    fprintf(stderr,"Estoy dentro del plugin\n");
    empresa *emp = &(bges->empresaactual);
  // Vamos a probar con un docwindow.
  Q3DockWindow  *doc1  = new  Q3DockWindow(Q3DockWindow::OutsideDock, 0, "Corrector");
  doc1->setGeometry(100,100,100,500);
  doc1->setFixedExtentWidth(200);
  correctorwidget *corr = new correctorwidget(doc1,"correctorplugin");
  corr->setEmpresa(emp);
  corr->dock = doc1;
  doc1->setWidget(corr);
  doc1->setResizeEnabled(TRUE);
  doc1->setMovingEnabled(TRUE);
  bges->moveDockWindow(doc1,Qt::DockRight);
  doc1->dock();
  doc1->hide();

	
    //El menu de empresa
//  QAction  *viewCorrector = new QAction("Corrector", "&Corrector", 0, 0, 0, true);
  QAction  *viewCorrector = new QAction( "&Corrector", 0);

  viewCorrector->setStatusTip("Muestra/Oculta el corrector");
  viewCorrector->setWhatsThis("Corrector\n\nMuestra/oculta el corrector");
  QObject::connect(viewCorrector, SIGNAL(toggled(bool)), corr, SLOT(cambia(bool)));    
  viewCorrector->addTo(bges->pVerMenu());
   
}


