#include <QWidget>
#include <QProcess>
#include <QDockWidget>
#include <QMainWindow>

#ifndef WIN32
#include <QX11EmbedContainer>
#endif

#include "abrevs.h"
#include "blfunctions.h"
#include "trabajadores.h"
#include "btcompany.h"
#include "tickets.h"
#include "blmainwindow.h"
#include "blplugins.h"


Abrevs::Abrevs ( BlMainCompany *emp, QWidget *parent ) : BlWidget ( emp, parent )
{
    setupUi ( this );

    /// Vamos a probar con un docwindow.
    m_doc2 = new QDockWidget ( "Teclado", g_main );
    m_doc2->setFeatures ( QDockWidget::AllDockWidgetFeatures );
    g_main->addDockWidget ( Qt::TopDockWidgetArea, m_doc2 );
    m_doc2->hide();
/// **** PRUEBAS DE EMBEBIDO

    m_proc = new QProcess();

    /*
        m_proc->start("xvkbd");
        if (!m_proc->waitForStarted())
            return;
        QString winId = "";
        while (winId == "") winId = windowID("xvkbd - Virtual Keyboard");
    */


    /*
            m_proc->start("klavier");
            if (!m_proc->waitForStarted())
                return;
            QString winId = "";
            while (winId == "") winId = windowID("klavier");
    */


    m_proc->start ( "matchbox-keyboard -xid es" );
    if ( !m_proc->waitForStarted() )
        return;
    m_proc->waitForReadyRead();
    QString winId = "";
    winId = m_proc->readAllStandardOutput();

#ifndef WIN32

    QX11EmbedContainer *container = new QX11EmbedContainer ( m_doc2 );
    container->embedClient ( winId.toInt() );
    m_doc2->setWidget ( container );

#endif

}


Abrevs::~Abrevs()
{
    m_proc->kill();
    delete m_proc;
    delete m_doc2;
}

void Abrevs::on_mui_teclado_clicked()
{

    if ( m_doc2->isVisible() ) {
        m_doc2->hide();
    } else {
        m_doc2->show();
// g_main->topDock()->setHeight(400);
    }// end if
}

void Abrevs::on_mui_usuario_clicked()
{

    /// Vamos a probar con un docwindow.
    /*
        QDockWidget *doc2 = new QDockWidget("Trabajadores", g_main);
        doc2->setFeatures(QDockWidget::AllDockWidgetFeatures);
        g_main->addDockWidget(Qt::TopDockWidgetArea, doc2);
        doc2->show();
    */
/// **** PRUEBAS DE EMBEBIDO

    Trabajadores * trab = new Trabajadores ( mainCompany(), 0 );

//    doc2->setWidget(trab);
    trab->exec();
//    trab->setWindowModality(Qt::WindowModal);
}

void Abrevs::on_mui_aparcar_clicked()
{
    BtCompany * emp = ( BtCompany * ) mainCompany();
    
    QString valorBtInput = emp->valorBtInput();
    
    // El nombre del ticket no puede estar vacio.
    if ( valorBtInput == "" ) {
	/// Establece en BtInput el nombre del ticket actual y lo utiliza al aparcarlo.
        emp->setValorBtInput(emp->ticketActual()->dbValue ( "nomticket" ));
    } // end if


    if ( emp->valorBtInput() == emp->ticketActual()->nomTicketDefecto()) {
        mensajeAviso ( _ ( "Asigne un nombre al ticket antes de aparcarlo." ) );
        /// Restaura el BtInput antes de regresar. Evita que aparezca, por error, en el 'display' el texto
        /// establecido en la condicion anterior que evita que el nombre del ticket este vacio.
        emp->setValorBtInput(valorBtInput);
        return;
    } // end if


    if ( emp->ticketActual() ->listaLineas() ->count() == 0 ) {
        mensajeAviso ( _ ( "El ticket esta vacio. No se puede aparcar." ) );
        return;
    } // end if


    /// Miramos que no haya ningun ticket abierto con el nombre usado
    BtTicket *ticket;
    for ( int i = 0; i < emp->listaTickets() ->size(); ++i ) {
        ticket = emp->listaTickets() ->at ( i );
        if ( emp->valorBtInput() == ticket->dbValue ( "nomticket" ) && ticket != emp->ticketActual()) {
            mensajeAviso ( _ ( "Ya existe un ticket aparcado con el mismo nombre." ) );
            return;
        }// end if
    }// end for


    QString nomticket = emp->ticketActual() -> dbValue("nomticket");

    /// El nombre del ticket a aparcar lo coge del BtInput.
    emp->ticketActual() ->setDbValue ( "nomticket", emp->valorBtInput() );

    /// Quitamos el bloqueo
    emp->ticketActual() ->setDbValue( "bloqueadoticket", "FALSE");
    
    /// Llamamos a plugins para poder hacer lo pertinente
    g_plugins->lanza("Abrevs_on_mui_aparcar_clicked", this);

    BtTicket *tick;
    if ( nomticket == emp->ticketActual()->nomTicketDefecto()) {
      tick = emp->newBtTicket();
      tick->setDbValue ( "idtrabajador", emp->ticketActual() ->dbValue ( "idtrabajador" ) );
      emp->listaTickets() ->append ( tick );
    } else {
      tick = emp->listaTickets() ->at ( emp->listaTickets()->size() -1 );
    } // end if

    /// Ponemos al trabajador creado el trabajador del ticket actual.
    emp->setTicketActual ( tick );
    /// Borra el valor del Input.
    emp->pulsaTecla ( Qt::Key_F4, "" );

    /// Llamamos a plugins para poder hacer lo pertinente
    g_plugins->lanza("Abrevs_on_mui_aparcar_clicked_Post", this);
    
    /// Ponemos el nuevo bloqueo
    tick->setDbValue("bloqueadoticket", "TRUE");
    
    tick->pintar();
}

void Abrevs::on_mui_recuperar_clicked()
{

    Tickets * trab = new Tickets ( mainCompany(), 0 );
    trab->exec();
    /// Llamamos a plugins para poder hacer lo pertinente
    g_plugins->lanza("Abrevs_on_mui_recuperar_clicked_Post", this);

}


void Abrevs::on_mui_cliente_clicked()
{
    BtCompany * emp = ( BtCompany * ) mainCompany();

    if ( emp->valorBtInput() == "" ) return;
    QString query = "SELECT * FROM cliente WHERE codcliente = '" + emp->valorBtInput() + "'";
    BlDbRecordSet *cur = emp->loadQuery ( query );
    if ( !cur->eof() ) {
        emp->ticketActual() ->setDbValue ( "idcliente", cur->valor ( "idcliente" ) );
    } else {
        emp->ticketActual() ->setDbValue ( "idcliente", g_confpr->valor ( CONF_IDCLIENTE_DEFECTO ) );
    } // end if
    delete cur;
    emp->ticketActual() ->pintar();
    emp->setValorBtInput ( "" );
    emp->pulsaTecla ( 0, "" );
}

