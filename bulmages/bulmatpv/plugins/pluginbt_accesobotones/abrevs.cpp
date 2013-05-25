#include <QtWidgets/QWidget>
#include <QtWidgets/QMainWindow>

#include "abrevs.h"
#include "blfunctions.h"
#include "btcompany.h"
#include "tickets.h"
#include "blmainwindow.h"
#include "blplugins.h"


Abrevs::Abrevs ( BlMainCompany *emp, QWidget *parent ) : BlWidget ( emp, parent )
{
    setupUi ( this );

}


Abrevs::~Abrevs()
{

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
        blMsgWarning ( _ ( "Asigne un nombre al ticket antes de aparcarlo." ) );
        /// Restaura el BtInput antes de regresar. Evita que aparezca, por error, en el 'display' el texto
        /// establecido en la condicion anterior que evita que el nombre del ticket este vacio.
        emp->setValorBtInput(valorBtInput);
        return;
    } // end if


    if ( emp->ticketActual() ->listaLineas() ->count() == 0 ) {
        blMsgWarning ( _ ( "El ticket esta vacio. No se puede aparcar." ) );
        return;
    } // end if


    /// Miramos que no haya ningun ticket abierto con el nombre usado
    BtTicket *ticket;
    for ( int i = 0; i < emp->listaTickets() ->size(); ++i ) {
        ticket = emp->listaTickets() ->at ( i );
        if ( emp->valorBtInput() == ticket->dbValue ( "nomticket" ) && ticket != emp->ticketActual()) {
            blMsgWarning ( _ ( "Ya existe un ticket aparcado con el mismo nombre." ) );
            return;
        }// end if
    }// end for


    QString nomticket = emp->ticketActual() -> dbValue("nomticket");

    /// El nombre del ticket a aparcar lo coge del BtInput.
    emp->ticketActual() ->setDbValue ( "nomticket", emp->valorBtInput() );

    /// Quitamos el bloqueo
    emp->ticketActual() ->setDbValue( "bloqueadoticket", "false");
    
    /// Llamamos a plugins para poder hacer lo pertinente
    g_plugins->run("Abrevs_on_mui_aparcar_clicked", this);

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
    g_plugins->run("Abrevs_on_mui_aparcar_clicked_Post", this);
    
    /// Ponemos el nuevo bloqueo
    tick->setDbValue("bloqueadoticket", "true");
    
    tick->pintar();
}

void Abrevs::on_mui_recuperar_clicked()
{
  
  
    BtCompany * emp = ( BtCompany * ) mainCompany();
    
    /// Si hay algun input primero miramos si este se corresponde con una mesa y lo mostramos directamente
    QString nommesa = emp->valorBtInput();
    if (nommesa != "") {
      
	for ( int i = 0; i < emp->listaTickets() ->size(); ++i ) {
	    BtTicket *ticket = emp->listaTickets() ->at ( i );
	    if (ticket->dbValue ( "nomticket" ) == nommesa &&  ticket->dbValue ( "idtrabajador" ) == emp->ticketActual() ->dbValue ( "idtrabajador" ) ) {
	      

	      /// Si el ticket seleccionado es el actual salimos sin hacer nada
	      if (ticket == emp ->ticketActual()) {
		/// Borra el valor del Input.
		emp->pulsaTecla ( Qt::Key_F4, "" );
		return;
	      } // end if	
	  
	      /// Pintamos los tickets bloqueados como no seleccionables.
	      if (ticket->dbValue( "bloqueadoticket") == "true" ) {
		    if( QMessageBox::warning(this, _("Ticket bloqueado"),
				  _("Este ticket esta bloqueado por otro terminal.\nDesea abrirlo de todos modos?"),
				  QMessageBox::Yes
				  | QMessageBox::No,
				  QMessageBox::No) == QMessageBox::No) {
		      /// Borra el valor del Input.
		      emp->pulsaTecla ( Qt::Key_F4, "" );
		      return;
		    } // end if
		      
	      } // end if	  
	    
	    
	      emp ->ticketActual()->setDbValue("bloqueadoticket", "false");
	      emp ->setTicketActual ( ticket );
	      ticket->setDbValue("bloqueadoticket", "true");
	      ticket->pintar();
	      
	      /// Borra el valor del Input.
	      emp->pulsaTecla ( Qt::Key_F4, "" );	      
	      
	      return;
	    } // end if
	}// end for
      
    } // end if
  
    Tickets * trab = new Tickets ( mainCompany(), 0 );
    trab->exec();
    /// Llamamos a plugins para poder hacer lo pertinente
    g_plugins->run("Abrevs_on_mui_recuperar_clicked_Post", this);
}

