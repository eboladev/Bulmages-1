#include "total.h"
#include <QWidget>

Total::Total ( EmpresaTPV *emp, QWidget *parent ) : BlWidget ( emp, parent )
{
    setupUi ( this );
}


Total::~Total()
{}

void Total::on_mui_cobrar_clicked()
{
    ( ( EmpresaTPV * ) mainCompany() ) ->pulsaTecla ( Qt::Key_F1 );
}

void Total::on_mui_abrircajon_clicked()
{
    ( ( EmpresaTPV * ) mainCompany() ) ->pulsaTecla ( Qt::Key_F3 );
}

void Total::on_mui_imprimir_clicked()
{
    ( ( EmpresaTPV * ) mainCompany() ) ->pulsaTecla ( Qt::Key_F2 );
}
