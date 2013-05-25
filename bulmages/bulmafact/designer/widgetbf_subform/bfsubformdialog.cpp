#include <QtGui/QtGui>
#include <QtDesigner>

#include "bfsubform.h"
#include "bfsubformdialog.h"


BfSubFormDialog::BfSubFormDialog(BfSubForm *sf, QWidget *parent)
    : QDialog(parent)
{
    setupUi(this);
}


void BfSubFormDialog::on_mui_nuevocampo_clicked() {

    mui_campolist->setRowCount(mui_campolist->rowCount() + 1);

}




void BfSubFormDialog::on_mui_borrarcampo_clicked() {
}


void BfSubFormDialog::on_mui_aceptar_clicked() {
}


void BfSubFormDialog::on_mui_cerrar_clicked() {
    close();
}


