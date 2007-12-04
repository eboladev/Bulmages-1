#include "admin.h"
#include <QWidget>

Admin::Admin(EmpresaTPV *emp, QWidget *parent) : BLWidget(emp, parent){
	setupUi(this);
}


Admin::~Admin() {

}

void Admin::on_mui_z_clicked() {
	((EmpresaTPV *)empresaBase())->z();
}

void Admin::on_mui_x_clicked() {
	((EmpresaTPV *)empresaBase())->x();
}