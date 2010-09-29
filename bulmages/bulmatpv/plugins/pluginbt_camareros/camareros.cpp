#include <QWidget>
#include <QPushButton>

#include "camareros.h"
#include "blfunctions.h"
#include "btticket.h"
#include "btcompany.h"
#include "trabajadores.h"


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
  
    Trabajadores * trab = new Trabajadores ( mainCompany(), 0 );
    trab->exec();
  
}

//  ========================0

