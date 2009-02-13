#include <QWidget>
#include <QPushButton>
#include "trabajadores.h"
#include "funcaux.h"
#include "ticket.h"
#include "empresatpv.h"


Trabajadores::Trabajadores ( EmpresaBase *emp, QWidget *parent ) : QDialog ( parent ), PEmpresaBase ( emp )
{
    setupUi ( this );

    BlDbRecordSet *cur = empresaBase() ->cargacursor ( "SELECT * FROM trabajador" );
    while ( !cur->eof() ) {
        QPushButton * toolbutton = new QPushButton ( mui_frame );
        toolbutton->setText ( cur->valor ( "nomtrabajador" ) + " " + cur->valor ( "apellidostrabajador" ) );

        QVBoxLayout *m_hboxLayout1 = mui_frame->findChild<QVBoxLayout *> ( "hboxLayout1" );
        if ( !m_hboxLayout1 ) {
            m_hboxLayout1 = new QVBoxLayout ( mui_frame );
            m_hboxLayout1->setSpacing ( 5 );
            m_hboxLayout1->setMargin ( 5 );
            m_hboxLayout1->setObjectName ( QString::fromUtf8 ( "hboxLayout1" ) );
        } // end if
        m_hboxLayout1->addWidget ( toolbutton );
        connect ( toolbutton, SIGNAL ( pressed() ), this, SLOT ( trabajadorClicked() ) );
        cur->siguienteregistro();
    } // end while
    delete cur;
}


Trabajadores::~Trabajadores()
{}



void Trabajadores::trabajadorClicked()
{
    EmpresaTPV * emp1 = ( EmpresaTPV * ) empresaBase();
    Ticket *ticket = NULL;
    Ticket *ticketv = NULL;
    bool encontrado = FALSE;

    /// Buscamos cual ha sido el trabajador pulsado.
    BlDbRecordSet *cur = empresaBase() ->cargacursor ( "SELECT * FROM trabajador" );
    while ( !encontrado ) {
        if ( ( ( QPushButton * ) sender() ) ->text() == cur->valor ( "nomtrabajador" ) + " " + cur->valor ( "apellidostrabajador" ) ) {
            encontrado = TRUE;
        } else {
            cur->siguienteregistro();
        } // end if
    } // end while

    /// Buscamos el ticket vacio de este trabajador y lo pintamos
    for ( int i = 0; i < emp1->listaTickets() ->size(); ++i ) {
        ticket = emp1->listaTickets() ->at ( i );

        if ( "" == ticket->DBvalue ( "nomticket" ) && cur->valor ( "idtrabajador" ) == ticket->DBvalue ( "idtrabajador" ) ) {
            ( ( EmpresaTPV * ) empresaBase() ) ->setTicketActual ( ticket );
            ticket->pintar();
            ticketv = ticket;
        }// end if

    }// end for

    /// Si el trabajador no tiene ticket vacio lo creamos y le ponemos el idtrabajador.
    if ( !ticketv ) {
        Ticket * tick = emp1->newTicket();
        tick->setDBvalue ( "idtrabajador", cur->valor ( "idtrabajador" ) );
        emp1->setTicketActual ( tick );
        emp1->listaTickets() ->append ( tick );
        tick->pintar();
    }// end if
    delete cur;

    done ( 0 );
}

