#ifndef BUSQUEDAFORMAPAGO_H
#define BUSQUEDAFORMAPAGO_H

#include <qcombobox.h>

#include "company.h"
#include "postgresiface2.h"

/**
@author Tomeu Borras
*/

class BusquedaFormaPago : public QComboBox {
Q_OBJECT
private:
company *companyact;
cursor2 *m_cursorcombo;

public:
    BusquedaFormaPago(QWidget *parent = 0, const char *name = 0);
    ~BusquedaFormaPago();
    void setcompany(company *comp) {companyact = comp;};
    virtual void setidforma_pago(QString idformapago);
public slots:
    void m_activated(int index) {
    	fprintf(stderr,"Activado el combo box\n");
	if (index >0)
	    emit(valueChanged(m_cursorcombo->valor("idforma_pago",index-1)));
	else 
	    emit(valueChanged(""));	
    };
signals:
	void valueChanged(QString);
};

#endif


