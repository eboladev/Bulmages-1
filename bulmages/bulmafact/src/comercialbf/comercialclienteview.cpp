

#include <QWidget>

#include "comercialclienteview.h"
#include "funcaux.h"



ComercialClienteView::ComercialClienteView(QWidget *parent, const char *nombre) : QWidget(parent) {	
	_depura("ComercialClienteView::ComercialClienteView",0);
	setupUi(this);
	_depura("ENd ComercialClienteView::ComercialClienteView",0);
}


ComercialClienteView::~ComercialClienteView() {
	_depura("ENd ~ComercialClienteView::ComercialClienteView",0);
}



