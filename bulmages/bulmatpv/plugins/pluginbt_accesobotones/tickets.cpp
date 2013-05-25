#include <QtWidgets/QWidget>
#include <QtWidgets/QPushButton>

#include "tickets.h"
#include "blfunctions.h"
#include "btcompany.h"
#include "btticket.h"


Tickets::Tickets ( BlMainCompany *emp, QWidget *parent ) : QDialog ( parent ), BlMainCompanyPointer ( emp )
{
    BL_FUNC_DEBUG
    setupUi ( this );
    BtCompany *emp1 = ( BtCompany * ) emp;
    BtTicket *ticket;
    for ( int i = 0; i < emp1->listaTickets() ->size(); ++i ) {
        ticket = emp1->listaTickets() ->at ( i );
        if ( ticket->dbValue ( "idtrabajador" ) == emp1->ticketActual() ->dbValue ( "idtrabajador" ) ) {
            QPushButton * toolbutton = new QPushButton ( mui_frame );
            toolbutton->setText ( ticket->dbValue ( "nomticket" ) );
	    
            QVBoxLayout *m_hboxLayout1 = mui_frame->findChild<QVBoxLayout *> ( "hboxLayout1" );
            if ( !m_hboxLayout1 ) {
                m_hboxLayout1 = new QVBoxLayout ( mui_frame );
                m_hboxLayout1->setSpacing ( 5 );
                m_hboxLayout1->setMargin ( 5 );
                m_hboxLayout1->setObjectName ( QString::fromUtf8 ( "hboxLayout1" ) );
            } // end if
            m_hboxLayout1->addWidget ( toolbutton );
            connect ( toolbutton, SIGNAL ( released() ), this, SLOT ( ticketClicked() ) );
        } // end if
    }// end for
}


Tickets::~Tickets()
{}

void Tickets::ticketClicked()
{
    BL_FUNC_DEBUG
    BtCompany * emp1 = ( BtCompany * ) mainCompany();
    QPushButton *toolbutton = ( QPushButton * ) sender();
    BtTicket *ticket;
    int i =   0;
    while ( i < emp1->listaTickets()->size() ) {
        ticket = emp1->listaTickets() ->at ( i++ );


	
        if ( toolbutton->text() == ticket->dbValue ( "nomticket" )
                && ticket->dbValue ( "idtrabajador" ) == emp1->ticketActual() ->dbValue ( "idtrabajador" ) ) {
	  
	  
	    /// Si el ticket seleccionado es el actual salimos sin hacer nada
	    if (ticket == emp1 ->ticketActual()) {
	      done ( 0 );
	      return;
	    } // end if	
	
	    /// Pintamos los tickets bloqueados como no seleccionables.
	    if (ticket->dbValue( "bloqueadoticket") == "true" && ticket->dbValue("nomticket") != "" ) {
		  if( QMessageBox::warning(this, _("Ticket bloqueado"),
                                _("Este ticket esta bloqueado por otro terminal.\nDesea abrirlo de todos modos?"),
                                 QMessageBox::Yes
                                | QMessageBox::No,
                                QMessageBox::No) == QMessageBox::No) {
		    return;
		  } // end if
		    
	    } // end if	  
	  
	  
	    emp1 ->ticketActual()->setDbValue("bloqueadoticket", "false");
            emp1 ->setTicketActual ( ticket );
	    ticket->setDbValue("bloqueadoticket", "true");
            ticket->pintar();
	    i = emp1->listaTickets()->size();
	} // end if
    }// end while


    done ( 0 );
}
