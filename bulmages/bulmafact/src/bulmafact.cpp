#include "bulmafact.h"

#include <qimage.h>
#include <qpixmap.h>
#include <qtoolbar.h>
#include <qtoolbutton.h>
#include <qpopupmenu.h>
#include <qmenubar.h>
#include <qtextedit.h>
#include <qfile.h>
#include <qfiledialog.h>
#include <qstatusbar.h>
#include <qmessagebox.h>
#include <qprinter.h>
#include <qapplication.h>
#include <qaccel.h>
#include <qtextstream.h>
#include <qpainter.h>
#include <qpaintdevicemetrics.h>
#include <qwhatsthis.h>

#include <qworkspace.h>
#include <qvbox.h>


#include <qlistbox.h>
#include <qdockwindow.h>

#include "listventanas.h"

bulmafact::bulmafact(QString bd) : bulmafactbase( 0, "bulmafact", WDestructiveClose ) {
  QPixmap fondo;
  m_company.init(bd);
  view_back = new QVBox( this);
  pWorkspace = new QWorkspace( view_back, "WorkSpace" );
  pWorkspace->setScrollBarsEnabled( TRUE );
  setCentralWidget(view_back);
  pWorkspace->showMaximized();
  
  m_company.setWorkspace(pWorkspace);  
  
  /// Aqui creamos la ventana dock para meter las distintas ventanas.  
  listventanas *list = new listventanas (this, "Ventanas");
  moveDockWindow(list, Left); 
  m_company.setListVentanas(list);
  m_company.createMainWindows();
  showMaximized();
  statusBar()->message( tr("Ready"), 2000 );
  fprintf(stderr,"Fin de bulmafact constructor\n");
}// end bulmafact


bulmafact::~bulmafact() {
}


void bulmafact::listproviders() {
   m_company.listproviders();
}


void bulmafact::listClients() {
   fprintf(stderr,"listclientsaction activado \n");
   m_company.listClients();
}

/**
* This action opens the edit/new client window
*/
void bulmafact::newClient() {
   m_company.newClient();
}


void bulmafact::listBudgets() {
   m_company.listBudgets();
}


void bulmafact::listClientDelivNotes() {
   m_company.listClientDelivNotes();
}

void bulmafact::s_trabajadores() {
   m_company.s_trabajadores();
}

void bulmafact::listarticles() {
   fprintf(stderr,"listarticlesaction activado \n");
   m_company.listarticles();
}// end listararticlesaction

void bulmafact::listorders() {
   fprintf(stderr,"listordersaction activado \n");
   m_company.listorders();
}// end listordersaction

void bulmafact::newOrder() {
   m_company.newOrder();
}// end newOrder action


void bulmafact::listdelivnotes() {
   fprintf(stderr,"listdelivnotes activado \n");
   m_company.lAlbaranesProveedor();
}// end listdelivnotesaction


void bulmafact::proveedores() {
   fprintf(stderr,"provideraction activado \n");
   m_company.listproviders();
}// end provideraction

void bulmafact::caja() {
   fprintf(stderr,"caja\n");
}// end caja


void bulmafact::clientes() {
   fprintf(stderr,"clientes\n");
}// end clientes


void bulmafact::emitirfactura() {
   fprintf(stderr,"emitir factura activado\n");
   QMessageBox::information( this, "BulmaFact",
    "Pulse aceptar para emitir un monton de facturas");
}// end emitirfactura

void bulmafact::recibirfactura() {
   fprintf(stderr,"recibir factura activado\n");
   QMessageBox::critical( this, "BulmaFact",
    "Pulse aceptar para recibir(destruir) un monton de facturas");
}// end recibirfactura

void bulmafact::about() {
    QMessageBox::about( this, tr("Qt Application Example"),
			tr("This example demonstrates simple use of "
			"QMainWindow,\nQMenuBar and QToolBar."));
}


void bulmafact::aboutQt() {
    QMessageBox::aboutQt( this, tr("Qt Application Example") );
}


void bulmafact::newBudget() {
   m_company.newBudget();
}// end provideraction


void bulmafact::newClientDelivNote() {
   m_company.newClientDelivNote();
}// end provideraction

void bulmafact::s_FPago() {
	m_company.s_FPago();
}// end s_FPago


void bulmafact::s_Familias() {
	m_company.s_Familias();
}// end s_Familias

void bulmafact::s_ventanaCompleta() {
	if (isFullScreen()) {
		showNormal();
	} else {
		showFullScreen();
	}// end if
}// end s_ventanaCompleta

