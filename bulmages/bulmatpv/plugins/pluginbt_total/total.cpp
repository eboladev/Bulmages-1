#include "total.h"
#include <QWidget>

Total::Total ( BtCompany *emp, QWidget *parent ) : BlWidget ( emp, parent )
{
    setupUi ( this );
}


Total::~Total()
{}

void Total::on_mui_cobrar_clicked()
{
    ( ( BtCompany * ) mainCompany() ) ->pulsaTecla ( Qt::Key_F5 );
}

void Total::on_mui_abrircajon_clicked()
{
    ( ( BtCompany * ) mainCompany() ) ->pulsaTecla ( Qt::Key_F7 );
}

void Total::on_mui_imprimir_clicked()
{
    ( ( BtCompany * ) mainCompany() ) ->pulsaTecla ( Qt::Key_F6 );
}
