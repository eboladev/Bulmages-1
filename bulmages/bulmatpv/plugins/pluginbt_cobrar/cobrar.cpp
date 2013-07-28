#include "cobrar.h"
#include <QtWidgets/QWidget>

Cobrar::Cobrar ( BtCompany *emp, QWidget *parent ) : BlWidget ( emp, parent )
{
    setupUi ( this );
}


Cobrar::~Cobrar()
{}

void Cobrar::on_mui_cobrar_clicked()
{
    ( ( BtCompany * ) mainCompany() ) ->pulsaTecla ( Qt::Key_F1 );
}

void Cobrar::on_mui_abrircajon_clicked()
{
    ( ( BtCompany * ) mainCompany() ) ->pulsaTecla ( Qt::Key_F3 );
}
