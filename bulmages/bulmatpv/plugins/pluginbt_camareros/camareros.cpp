#include <QWidget>
#include <QPushButton>

#include "camareros.h"
#include "blfunctions.h"
#include "btticket.h"
#include "btcompany.h"



Camareros * g_camareros;
//Mesa  * g_camareroAct;
bool  g_bloqueo;


Camareros::Camareros ( BtCompany *emp, QWidget *parent ) : BlWidget ( emp, parent )
{
    setupUi ( this );

    /// Cambiamos el icono de las camareros.
//    mui_camareros->setIcon ( QIcon ( g_confpr->valor ( CONF_PROGDATA ) + "icons/table.svg" ) );

    m_centralWidget = NULL;
//    m_distro = NULL;
    g_camareros = this;
//    g_camareroAct = NULL;
}


Camareros::~Camareros()
{}

void Camareros::on_mui_camareros_clicked()
{
/*
    if (m_centralWidget == NULL) {
        /// Creamos y presentamos el plugin de camareros.
         if (m_distro == NULL)
            m_distro= new DistroCamareros((BtCompany *) mainCompany(), this);
        m_centralWidget = ((BtBulmaTPV *)g_main)->centralBtWidget();
        ((BtBulmaTPV *)g_main) -> setCentralBtWidget (m_distro);
    } else {
        ((BtBulmaTPV *)g_main) -> setCentralBtWidget (m_centralWidget);
        m_centralWidget = NULL;
        m_distro->exportXML();
    } // end if
*/
blMsgInfo("pulsado sobre el boton");
}

//  ========================0

