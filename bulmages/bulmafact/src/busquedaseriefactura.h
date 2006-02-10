#ifndef BUSQUEDASERIEFACTURA_H
#define BUSQUEDASERIEFACTURA_H

#include <Q3ComboBox>

#include "company.h"
#include "postgresiface2.h"

/**
@author Tomeu Borras
*/

class BusquedaSerieFactura : public Q3ComboBox {
Q_OBJECT
private:
company *companyact;
cursor2 *m_cursorcombo;

public:
    BusquedaSerieFactura(QWidget *parent = 0, const char *name = 0);
    ~BusquedaSerieFactura();
    void setcompany(company *comp) {companyact = comp;};
    virtual void setcodigoserie_factura(QString);
public slots:
    void m_activated(int index) {
	if (index >0)    
	    emit(valueChanged(m_cursorcombo->valor("codigoserie_factura",index-1)));
	else
	    emit(valueChanged(""));
    };
signals:
	void valueChanged(QString);
};

#endif


