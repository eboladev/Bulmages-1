#include <QtWidgets/QWidget>
#include <QtWidgets/QPushButton>

#include "blmainwindow.h"
#include "trabajadores.h"
#include "blfunctions.h"
#include "blapplication.h"

Trabajadores::Trabajadores ( BlMainCompany *emp, QWidget *parent, bool deleteOnClose ) : QDialog ( parent ), BlMainCompanyPointer ( emp )
{
    setupUi ( this );

    connect (this, SIGNAL(rejected()), this, SLOT(closeDialog()) );
    
    mui_password->setFocus(Qt::OtherFocusReason);
    installEventFilter(this);
    
    m_validUser = false;
    m_deleteOnClose = deleteOnClose;
    
    BlDbRecordSet *cur = mainCompany() ->loadQuery ( "SELECT * FROM trabajador WHERE gerentetrabajador = true" );
    /// Si no hay gerentes entra cualquier ya que sino no se podria entrar nunca.
    if (cur->eof()) {
       m_validUser = true;
    } // end if
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
      delete cur;
      m_validUser = true;
      done ( 0 );
    } // end if
}

