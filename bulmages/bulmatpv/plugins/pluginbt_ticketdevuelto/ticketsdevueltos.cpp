#include "ticketsdevueltos.h"
#include "blfixed.h"
#include <QtWidgets/QWidget>


/// Una factura puede tener multiples bases imponibles. Por eso definimos el tipo base
/// como un QMap.
typedef QMap<QString, BlFixed> base;

TicketsDevueltos::TicketsDevueltos ( BtCompany *emp, QWidget *parent ) : BlWidget ( emp, parent )
{
    setupUi ( this );
}


TicketsDevueltos::~TicketsDevueltos()
{}


void TicketsDevueltos::on_mui_cancelar_clicked()
{
    ( ( QDialog * ) parent() )->accept();
}

void TicketsDevueltos::on_mui_ref_returnPressed()
{
    QString query1 = "UPDATE albaran SET olvidadoalbaran = true WHERE refalbaran = '" + mui_ref->text() + "'";
    mainCompany()->runQuery ( query1 );
    mui_ref->setText ( "" );
}


