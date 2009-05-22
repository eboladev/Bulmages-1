#include "cobrar.h"
#include <QWidget>

Cobrar::Cobrar ( BtCompany *emp, QWidget *parent ) : BlWidget ( emp, parent )
{
    setupUi ( this );
}


Cobrar::~Cobrar()
{}

void Cobrar::on_mui_cobrar_released()
{
    ( ( BtCompany * ) mainCompany() ) ->pulsaTecla ( Qt::Key_F1 );
}

void Cobrar::on_mui_abrircajon_released()
{
    ( ( BtCompany * ) mainCompany() ) ->pulsaTecla ( Qt::Key_F3 );
}
