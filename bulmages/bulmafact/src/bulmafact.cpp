#include "bulmafact.h"

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
#include "listventanas.h"
#include "funcaux.h"
#include "aboutview.h"

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
    //  listventanas *list = new listventanas ("Ventanas",0, 0);
    m_list = new listventanas("Ventanas",this,Qt::Tool);
    //	list->showMaximized();

    /// TODO, ESTA LINEA DEBE VOLVER A PONERSE CUANDO ESTEMOS CON QT4 Full Featured.
    //  addDockWidget(Qt::LeftDockWidgetArea, list);
    addToolBar(m_list,Qt::DockLeft);
    //addDockWindow(m_list, Qt::DockLeft);

    m_company->setListVentanas(m_list);
    m_company->createMainWindows();
    showMaximized();
    statusBar()->message( tr("Ready"), 2000 );
    _depura("Fin de bulmafact constructor\n",0);

    //    fondo.load(confpr->valor(CONF_BACKGROUND));
    //    pWorkspace->setBackgroundPixmap (fondo);
}// end bulmafact


bulmafact::~bulmafact() {
    _depura("Destructor de BulmaFact",0);
    // En windows no termina bien la ejecucion del programa y por eso agregamos esta salida r�ida.
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
    fprintf(stderr,"listarticlesaction activado \n");
    m_company->listarticles();
}// end listararticlesaction


void bulmafact::listdelivnotes() {
    fprintf(stderr,"listdelivnotes activado \n");
    m_company->lAlbaranesProveedor();
}// end listdelivnotesaction


void bulmafact::s_newProveedor() {
    fprintf(stderr,"provideraction activado \n");
    m_company->s_newProveedor();
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
    aboutview *about=new aboutview();
    about->exec();
    delete about;
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

