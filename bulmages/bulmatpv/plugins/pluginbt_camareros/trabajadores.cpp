#include <QtWidgets/QWidget>
#include <QtWidgets/QPushButton>

#include "blmainwindow.h"
#include "trabajadores.h"
#include "blfunctions.h"
#include "btticket.h"
#include "btcompany.h"
#include "blapplication.h"

Trabajadores::Trabajadores ( BlMainCompany *emp, QWidget *parent, bool deleteOnClose ) : QDialog ( parent ), BlMainCompanyPointer ( emp )
{
    setupUi ( this );

    connect (this, SIGNAL(rejected()), this, SLOT(closeDialog()) );
    
    mui_password->setFocus(Qt::OtherFocusReason);
    installEventFilter(this);
    
    m_validUser = false;
    m_deleteOnClose = deleteOnClose;
    
    BlDbRecordSet *cur = mainCompany() ->loadQuery ( "SELECT * FROM trabajador" );
    while ( !cur->eof() ) {
        QPushButton * toolbutton = new QPushButton ( mui_frame );
        toolbutton->setText ( cur->value( "nomtrabajador" ) + " " + cur->value( "apellidostrabajador" ) );
        toolbutton->setMaximumHeight(200);

        QVBoxLayout *m_hboxLayout1 = mui_frame->findChild<QVBoxLayout *> ( "hboxLayout1" );
        if ( !m_hboxLayout1 ) {
            m_hboxLayout1 = new QVBoxLayout ( mui_frame );
            m_hboxLayout1->setSpacing ( 0 );
            m_hboxLayout1->setMargin ( 0 );
            m_hboxLayout1->setObjectName ( QString::fromUtf8 ( "hboxLayout1" ) );
        } // end if
        m_hboxLayout1->addWidget ( toolbutton );
        connect ( toolbutton, SIGNAL ( clicked() ), this, SLOT ( trabajadorClicked() ) );
        cur->nextRecord();
    } // end while
    
    delete cur;
}


Trabajadores::~Trabajadores()
{
}

void Trabajadores::on_mui_cerrar_clicked()
{
    closeDialog();
}


void Trabajadores::closeDialog()
{
    if (!m_validUser && m_deleteOnClose) {
	exit(0);
    } else {
	close();
    } // end if
}


void Trabajadores::closeEvent(QCloseEvent *event)
{
    closeDialog();
}


void Trabajadores::trabajadorClicked()
{
    BtCompany *emp1 = ( BtCompany * ) mainCompany();
    BtTicket *ticket = NULL;
    BtTicket *ticketv = NULL;
    bool encontrado = false;

    /// Buscamos cual ha sido el trabajador pulsado.
    BlDbRecordSet *cur = mainCompany() ->loadQuery ( "SELECT * FROM trabajador" );
    while ( !encontrado && !cur->eof() ) {
        if ( ( ( QPushButton * ) sender() ) ->text() == cur->value( "nomtrabajador" ) + " " + cur->value( "apellidostrabajador" )  
	&& ((cur->value("passwordtrabajador") == mui_password->text() || cur->value("passwordtrabajador") == "")) ) {
            encontrado = true;

        } else {
            cur->nextRecord();
        } // end if
    } // end while

    if (encontrado) {
      /// Buscamos el ticket vacio de este trabajador y lo pintamos
      for ( int i = 0; i < emp1->listaTickets() ->size(); ++i ) {
	  ticket = emp1->listaTickets() ->at ( i );

	  if ( ticket->nomTicketDefecto() == ticket->dbValue ( "nomticket" ) && cur->value( "idtrabajador" ) == ticket->dbValue ( "idtrabajador" ) ) {
	      ( ( BtCompany * ) mainCompany() ) ->setTicketActual ( ticket );
	      ticket->pintar();
	      ticketv = ticket;
	  }// end if
      }// end for

      /// Si el trabajador no tiene ticket vacio lo creamos y le ponemos el idtrabajador.
      if ( !ticketv ) {
	  BtTicket * tick = emp1->newBtTicket();
	  tick->setDbValue ( "idtrabajador", cur->value( "idtrabajador" ) );
	  emp1->setTicketActual ( tick );
	  emp1->listaTickets() ->append ( tick );
	  tick->pintar();
      }// end if
      
      /// Segun los privilegios que tenga el camarero le desabilitamos determinados objetos.
      if(cur->value("admintrabajador") == "t") {
	QToolButton *toolz = g_main->findChild<QToolButton *> ( "mui_z" );
	if (toolz) {
	    toolz->setEnabled(true);
	} // end if	

	QToolButton *toolx = g_main->findChild<QToolButton *> ( "mui_x" );
	if (toolx) {
	    toolx->setEnabled(true);
	} // end if	

	QToolButton *toolborraticket = g_main->findChild<QToolButton *> ( "mui_borrarticket" );
	if (toolborraticket) {
	    toolborraticket->setEnabled(true);
	} // end if

      } else {

	QToolButton *toolz = g_main->findChild<QToolButton *> ( "mui_z" );
	if (toolz) {
	    toolz->setEnabled(false);
	} // end if	
	
	QToolButton *toolx = g_main->findChild<QToolButton *> ( "mui_x" );
	if (toolx) {
	    toolx->setEnabled(false);
	} // end if	


	QToolButton *toolborraticket = g_main->findChild<QToolButton *> ( "mui_borrarticket" );
	if (toolborraticket) {
	    toolborraticket->setEnabled(false);
	} // end if
	
      } // end if
      
      delete cur;

      m_validUser = true;
      done ( 0 );
    } // end if
}

