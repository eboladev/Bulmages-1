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

#include "bulmafact.h"
#include "listventanas.h"
#include "funcaux.h"
#include "aboutview.h"

#include <QImage>
#include <QPixmap>
#include <Q3ToolBar>
#include <QToolButton>
#include <Q3PopupMenu>
#include <QMenuBar>
#include <Q3TextEdit>
#include <QFile>
#include <Q3FileDialog>
#include <QStatusBar>
#include <QMessageBox>
#include <QPrinter>
#include <QApplication>
#include <Q3Accel>
#include <QTextStream>
#include <QPainter>
#include <Q3PaintDeviceMetrics>
#include <Q3WhatsThis>
#include <QWorkspace>
#include <Q3VBox>
#include <Q3ListBox>
#include <QMainWindow>


bulmafact::bulmafact(QString bd) : bulmafactbase( 0, "bulmafact", Qt::WDestructiveClose ) {
	//  QPixmap fondo;

	m_company=new company();

	m_company->init(bd);
	view_back = new Q3VBox( this);
	view_back->setFrameStyle( QFrame::StyledPanel | QFrame::Sunken );
	pWorkspace = new QWorkspace( view_back, "WorkSpace" );

	pWorkspace->setScrollBarsEnabled( TRUE );
	setCentralWidget(view_back);
	// setCentralWidget(pWorkspace);
	pWorkspace->showMaximized();
	m_company->setWorkspace(pWorkspace);

	/// Aqui creamos la ventana dock para meter las distintas ventanas.
	// listventanas *list = new listventanas ("Ventanas",0, 0);
	m_list = new listventanas("Ventanas",this,Qt::Tool);
	// list->showMaximized();

	/// TODO, ESTA LINEA DEBE VOLVER A PONERSE CUANDO ESTEMOS CON QT4 Full Featured.
	// addDockWidget(Qt::LeftDockWidgetArea, list);
	addToolBar(m_list,Qt::DockLeft);
	// addDockWindow(m_list, Qt::DockLeft);

	m_company->setListVentanas(m_list);
	m_company->createMainWindows();
	showMaximized();
	statusBar()->message( tr("Ready"), 2000 );
	_depura("Fin de bulmafact constructor\n",0);

	// fondo.load(confpr->valor(CONF_BACKGROUND));
	// pWorkspace->setBackgroundPixmap (fondo);
}// end bulmafact


bulmafact::~bulmafact() {
	_depura("Destructor de BulmaFact",0);
	// En windows no termina bien la ejecucion del programa y por eso
	// agregamos esta salida rapida.
#ifdef WINDOWS
	exit(0);
#endif
	_depura("End Destructor de BulmaFact",0);
}


void bulmafact::listproviders() {
	m_company->listproviders();
}


void bulmafact::listClients() {
	m_company->listClients();
}


/**
* This action opens the edit/new client window
*/
void bulmafact::newClient() {
	m_company->newClient();
}


void bulmafact::listBudgets() {
	m_company->listBudgets();
}


void bulmafact::listClientDelivNotes() {
	m_company->listClientDelivNotes();
}


void bulmafact::s_trabajadores() {
	m_company->s_trabajadores();
}


void bulmafact::s_listPedidosCli() {
	m_company->s_listPedidosCli();
}// end s_listPedidosCli


void bulmafact::listarticles() {
	_depura("listarticlesaction activado",0);
	m_company->listarticles();
}// end listararticlesaction


void bulmafact::listdelivnotes() {
	_depura("listdelivnotes activado",0);
	m_company->lAlbaranesProveedor();
}// end listdelivnotesaction


void bulmafact::s_newProveedor() {
	_depura("provideraction activado",0);
	m_company->s_newProveedor();
}// end provideraction


void bulmafact::caja() {
	_depura("caja",0);
}// end caja


void bulmafact::clientes() {
	_depura("clientes",0);
}// end clientes


void bulmafact::emitirfactura() {
	_depura("emitir factura activado",0);
	QMessageBox::information( this, "BulmaFact",
                              "Pulse aceptar para emitir un monton de facturas");
}// end emitirfactura


void bulmafact::recibirfactura() {
	_depura("recibir factura activado",0);
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


void bulmafact::newClientDelivNote() {
	m_company->newClientDelivNote();
}// end provideraction


void bulmafact::s_FPago() {
	m_company->s_FPago();
}// end s_FPago


void bulmafact::s_Familias() {
	m_company->s_Familias();
}// end s_Familias


void bulmafact::s_seriesFactura() {
	m_company->s_seriesFactura();
}// end s_Familias


void bulmafact::s_ventanaCompleta() {
	if (isFullScreen()) {
		showNormal();
	} else {
		showFullScreen();
	}// end if
}// end s_ventanaCompleta


void bulmafact::s_About()  {
	aboutview about;
	about.exec();
}// end slotHelpAbout


void bulmafact::closeEvent( QCloseEvent *) {
	_depura("closeEvent",0);
	delete m_company;
#ifdef WINDOWS
	exit(0);
#endif
	delete m_list;
	delete pWorkspace;
	delete view_back;
}

