#ifndef COMERCIALCLIENTEVIEW_H
#define COMERCIALCLIENTEVIEW_H


/**
@author Tomeu Borras
*/




#include "company.h"
#include "ui_comercialclientebase.h"

class ComercialClienteView : public QWidget, public Ui_ComercialClienteBase {
Q_OBJECT
public:
void setcompany(company *comp) {
	_depura("ComercialClienteView::setcompany",0);
	mui_zonacomercial->setcompany(comp);
	mui_zonacomercial->setidzonacomercial("0");
};
ComercialClienteView(QWidget *parent=0, const char *nombre="");
~ComercialClienteView();
};



#endif

