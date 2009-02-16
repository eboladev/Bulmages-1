#include <QWidget>
#include <QPushButton>

#include "tickets.h"
#include "blfunctions.h"
#include "empresatpv.h"
#include "ticket.h"


Tickets::Tickets ( BlMainCompany *emp, QWidget *parent ) : QDialog ( parent ), BlMainCompanyPointer ( emp )
{
    setupUi ( this );
    EmpresaTPV *emp1 = ( EmpresaTPV * ) emp;
    Ticket *ticket;
    for ( int i = 0; i < emp1->listaTickets() ->size(); ++i ) {
        ticket = emp1->listaTickets() ->at ( i );
        if ( ticket->DBvalue ( "idtrabajador" ) == emp1->ticketActual() ->DBvalue ( "idtrabajador" ) ) {
            QPushButton * toolbutton = new QPushButton ( mui_frame );
            toolbutton->setText ( ticket->DBvalue ( "nomticket" ) );

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
}


Tickets::~Tickets()
{}

void Tickets::ticketClicked()
{
    EmpresaTPV * emp1 = ( EmpresaTPV * ) empresaBase();
    QPushButton *toolbutton = ( QPushButton * ) sender();
    Ticket *ticket;
    for ( int i = 0; i < emp1->listaTickets() ->size(); ++i ) {
        ticket = emp1->listaTickets() ->at ( i );

        if ( toolbutton->text() == ticket->DBvalue ( "nomticket" )
                && ticket->DBvalue ( "idtrabajador" ) == emp1->ticketActual() ->DBvalue ( "idtrabajador" ) ) {
            ( ( EmpresaTPV * ) empresaBase() ) ->setTicketActual ( ticket );
            ticket->pintar();
        }// end if
    }// end for

    done ( 0 );
}
