#include <QWidget>
#include <QPushButton>

#include "trabajadores.h"
#include "blfunctions.h"
#include "btticket.h"
#include "btcompany.h"


Trabajadores::Trabajadores ( BlMainCompany *emp, QWidget *parent ) : QDialog ( parent ), BlMainCompanyPointer ( emp )
{
    setupUi ( this );

    BlDbRecordSet *cur = mainCompany() ->loadQuery ( "SELECT * FROM trabajador" );
    while ( !cur->eof() ) {
        QPushButton * toolbutton = new QPushButton ( mui_frame );
        toolbutton->setText ( cur->valor ( "nomtrabajador" ) + " " + cur->valor ( "apellidostrabajador" ) );
        toolbutton->setMaximumHeight(200);

        QVBoxLayout *m_hboxLayout1 = mui_frame->findChild<QVBoxLayout *> ( "hboxLayout1" );
        if ( !m_hboxLayout1 ) {
            m_hboxLayout1 = new QVBoxLayout ( mui_frame );
            m_hboxLayout1->setSpacing ( 0 );
            m_hboxLayout1->setMargin ( 0 );
            m_hboxLayout1->setObjectName ( QString::fromUtf8 ( "hboxLayout1" ) );
        } // end if
        m_hboxLayout1->addWidget ( toolbutton );
        connect ( toolbutton, SIGNAL ( pressed() ), this, SLOT ( trabajadorClicked() ) );
        cur->nextRecord();
    } // end while
    delete cur;
}


Trabajadores::~Trabajadores()
{}



void Trabajadores::trabajadorClicked()
{
    BtCompany * emp1 = ( BtCompany * ) mainCompany();
    BtTicket *ticket = NULL;
    BtTicket *ticketv = NULL;
    bool encontrado = FALSE;

    /// Buscamos cual ha sido el trabajador pulsado.
    BlDbRecordSet *cur = mainCompany() ->loadQuery ( "SELECT * FROM trabajador" );
    while ( !encontrado && !cur->eof() ) {
        if ( ( ( QPushButton * ) sender() ) ->text() == cur->valor ( "nomtrabajador" ) + " " + cur->valor ( "apellidostrabajador" )  
	&& ((cur->valor("passwordtrabajador") == mui_password->text() || cur->valor("passwordtrabajador") == "")) ) {
            encontrado = TRUE;

        } else {
            cur->nextRecord();
        } // end if
    } // end while

    if (encontrado) {
      /// Buscamos el ticket vacio de este trabajador y lo pintamos
      for ( int i = 0; i < emp1->listaTickets() ->size(); ++i ) {
	  ticket = emp1->listaTickets() ->at ( i );

	  if ( ticket->nomTicketDefecto() == ticket->dbValue ( "nomticket" ) && cur->valor ( "idtrabajador" ) == ticket->dbValue ( "idtrabajador" ) ) {
	      ( ( BtCompany * ) mainCompany() ) ->setTicketActual ( ticket );
	      ticket->pintar();
	      ticketv = ticket;
	  }// end if

      }// end for

      /// Si el trabajador no tiene ticket vacio lo creamos y le ponemos el idtrabajador.
      if ( !ticketv ) {
	  BtTicket * tick = emp1->newBtTicket();
	  tick->setDbValue ( "idtrabajador", cur->valor ( "idtrabajador" ) );
	  emp1->setTicketActual ( tick );
	  emp1->listaTickets() ->append ( tick );
	  tick->pintar();
      }// end if
      delete cur;

      done ( 0 );
    }// end if
}

