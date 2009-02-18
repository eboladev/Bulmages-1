#include "cobrar.h"
#include <QWidget>

Cobrar::Cobrar ( EmpresaTPV *emp, QWidget *parent ) : BlWidget ( emp, parent )
{
    setupUi ( this );
}


Cobrar::~Cobrar()
{}

void Cobrar::on_mui_cobrar_clicked()
{
    ( ( EmpresaTPV * ) mainCompany() ) ->pulsaTecla ( Qt::Key_F1 );
}

void Cobrar::on_mui_abrircajon_clicked()
{
    ( ( EmpresaTPV * ) mainCompany() ) ->pulsaTecla ( Qt::Key_F3 );
}
