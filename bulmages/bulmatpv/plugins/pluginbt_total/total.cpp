#include "total.h"
#include <QWidget>

Total::Total ( BtCompany *emp, QWidget *parent ) : BlWidget ( emp, parent )
{
    setupUi ( this );
}


Total::~Total()
{}

void Total::on_mui_cobrar_released()
{
    ( ( BtCompany * ) mainCompany() ) ->pulsaTecla ( Qt::Key_F1 );
}

void Total::on_mui_abrircajon_released()
{
    ( ( BtCompany * ) mainCompany() ) ->pulsaTecla ( Qt::Key_F3 );
}

void Total::on_mui_imprimir_released()
{
    ( ( BtCompany * ) mainCompany() ) ->pulsaTecla ( Qt::Key_F2 );
}
