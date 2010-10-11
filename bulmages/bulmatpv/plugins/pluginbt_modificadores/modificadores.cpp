#include <QWidget>
#include <QPushButton>

#include "modificadores.h"
#include "blfunctions.h"
#include "btticket.h"
#include "btcompany.h"


Modificadores::Modificadores ( BlMainCompany *emp, QWidget *parent ) : QDialog ( parent ), BlMainCompanyPointer ( emp )
{
    blDebug("Modificadores::Modificadores");
    setupUi ( this );
    bool hayModificadores = FALSE;
    
    if (! ((BtCompany *) emp)->ticketActual()->lineaActBtTicket() ) {
	blDebug("END Modificadores::Modificadores");
	return;
    } // end if
    QString idarticulo = ((BtCompany *) emp)->ticketActual()->lineaActBtTicket()->dbValue ( "idarticulo");
    
    BlDbRecordSet *cur = mainCompany() ->loadQuery ( "SELECT * FROM modificador WHERE idarticulo = " + idarticulo );
    while ( !cur->eof() ) {
	hayModificadores = TRUE;
        QPushButton * toolbutton = new QPushButton ( mui_frame );
        toolbutton->setText ( cur->valor ( "nombremodificador" ) );

        QVBoxLayout *m_hboxLayout1 = mui_frame->findChild<QVBoxLayout *> ( "hboxLayout1" );
        if ( !m_hboxLayout1 ) {
            m_hboxLayout1 = new QVBoxLayout ( mui_frame );
            m_hboxLayout1->setSpacing ( 5 );
            m_hboxLayout1->setMargin ( 5 );
            m_hboxLayout1->setObjectName ( QString::fromUtf8 ( "hboxLayout1" ) );
        } // end if
        m_hboxLayout1->addWidget ( toolbutton );
        connect ( toolbutton, SIGNAL ( pressed() ), this, SLOT ( trabajadorClicked() ) );
        cur->nextRecord();
    } // end while
    delete cur;
    
    if (!hayModificadores) done(0);
    blDebug("END Modificadores::Modificadores");

}


Modificadores::~Modificadores()
{}



void Modificadores::trabajadorClicked()
{
    blDebug("Modificadores::trabajadorClicked");

    BtCompany * emp1 = ( BtCompany * ) mainCompany();
    BtTicket *ticket = NULL;
    BtTicket *ticketv = NULL;
    bool encontrado = FALSE;

    QString idarticulo =  emp1->ticketActual()->lineaActBtTicket()->dbValue ( "idarticulo");
	
    /// Buscamos cual ha sido el modificador pulsado.
    BlDbRecordSet *cur = mainCompany() ->loadQuery ( "SELECT * FROM modificador WHERE idarticulo = " + idarticulo );
    while ( !encontrado ) {
        if ( ( ( QPushButton * ) sender() ) ->text() == cur->valor ( "nombremodificador" ) ) {
            encontrado = TRUE;
        } else {
            cur->nextRecord();
        } // end if
    } // end while

    /// Buscamos un espacio vacio en la tabla para poner el modificador.
    int i = 3;
    while ( (i <= 9) && (emp1->ticketActual()->lineaActBtTicket()->dbValue ( "idmodificador" + QString::number(i)) != "") ) {
      i++;
    } // end while
    if (i <= 9) 
      emp1->ticketActual()->lineaActBtTicket()->setDbValue ( "idmodificador" + QString::number(i), cur->valor("idmodificador"));



    delete cur;

    done ( 0 );

    blDebug("END Modificadores::trabajadorClicked");

}

