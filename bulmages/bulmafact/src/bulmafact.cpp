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
/*
#include "filesave.xpm"
#include "fileopen.xpm"
#include "fileprint.xpm"
*/
bulmafact::bulmafact()
    : bulmafactbase( 0, "bulmafact", WDestructiveClose ) {

  QPixmap fondo;
  view_back = new QVBox( this);
//  view_back->setFrameStyle( QFrame::StyledPanel | QFrame::Sunken );
  pWorkspace = new QWorkspace( view_back, "WorkSpace" );
  pWorkspace->setScrollBarsEnabled( TRUE );
  setCentralWidget(view_back);
  
  pWorkspace->showMaximized();
 
   
    m_company.setWorkspace(pWorkspace);
//    m_company.setWorkspace(this);
    statusBar()->message( tr("Ready"), 2000 );
    resize( 450, 600 );
}


bulmafact::~bulmafact()
{
   
}




void bulmafact::listproviders()
{
   m_company.listproviders();
}


void bulmafact::listClients()
{
   fprintf(stderr,"listclientsaction activado \n");
   m_company.listClients();
}

/**
* This action opens the edit/new client window
*
*/
void bulmafact::newClient()
{
   m_company.newClient();
}


void bulmafact::listBudgets()
{
   m_company.listBudgets();
}


void bulmafact::listarticles()
{
   fprintf(stderr,"listarticlesaction activado \n");
   m_company.listarticles();
}// end listararticlesaction

void bulmafact::listorders()
{
   fprintf(stderr,"listordersaction activado \n");
   m_company.listorders();
}// end listordersaction

void bulmafact::listdelivnotes()
{
   fprintf(stderr,"listdelivnotes activado \n");
   m_company.listdelivnotes();
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

void bulmafact::about()
{
    QMessageBox::about( this, tr("Qt Application Example"),
			tr("This example demonstrates simple use of "
			"QMainWindow,\nQMenuBar and QToolBar."));
}


void bulmafact::aboutQt()
{
    QMessageBox::aboutQt( this, tr("Qt Application Example") );
}

void bulmafact::newBudget() {
   m_company.newBudget();
}// end provideraction


