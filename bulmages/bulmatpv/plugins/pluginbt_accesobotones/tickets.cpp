#include <QWidget>
#include <QPushButton>

#include "tickets.h"
#include "blfunctions.h"
#include "btcompany.h"
#include "btticket.h"


Tickets::Tickets ( BlMainCompany *emp, QWidget *parent ) : QDialog ( parent ), BlMainCompanyPointer ( emp )
{
    _depura("Tickets::Tickets");
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
            connect ( toolbutton, SIGNAL ( pressed() ), this, SLOT ( ticketClicked() ) );
        } // end if
    }// end for
    _depura("END Tickets::Tickets");
}


Tickets::~Tickets()
{}

void Tickets::ticketClicked()
{
    _depura("Tickets::ticketClicked");
    BtCompany * emp1 = ( BtCompany * ) mainCompany();
    QPushButton *toolbutton = ( QPushButton * ) sender();
    BtTicket *ticket;
    for ( int i = 0; i < emp1->listaTickets() ->size(); ++i ) {
        ticket = emp1->listaTickets() ->at ( i );

        if ( toolbutton->text() == ticket->dbValue ( "nomticket" )
                && ticket->dbValue ( "idtrabajador" ) == emp1->ticketActual() ->dbValue ( "idtrabajador" ) ) {
            ( ( BtCompany * ) mainCompany() ) ->setTicketActual ( ticket );
            ticket->pintar();
        }// end if
    }// end for

    done ( 0 );
    _depura("END Tickets::ticketClicked");
}
