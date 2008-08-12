#include "ticketsdevueltos.h"
#include "fixed.h"
#include <QWidget>


/// Una factura puede tener multiples bases imponibles. Por eso definimos el tipo base
/// como un QMap.
typedef QMap<QString, Fixed> base;

TicketsDevueltos::TicketsDevueltos ( EmpresaTPV *emp, QWidget *parent ) : BLWidget ( emp, parent )
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
    QString query1 = "UPDATE albaran SET olvidadoalbaran = TRUE WHERE refalbaran = '"+ mui_ref->text() + "'";
    empresaBase()->ejecuta ( query1 );
    mui_ref->setText("");
}


