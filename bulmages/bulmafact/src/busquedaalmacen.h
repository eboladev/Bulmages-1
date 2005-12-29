#ifndef BUSQUEDAALMACEN_H
#define BUSQUEDAALMACEN_H

#include <qcombobox.h>

#include "company.h"
#include "postgresiface2.h"

/**
@author Tomeu Borras
*/

class BusquedaAlmacen : public QComboBox {
Q_OBJECT
private:
company *companyact;
cursor2 *m_cursorcombo;

public:
    BusquedaAlmacen(QWidget *parent = 0, const char *name = 0);
    ~BusquedaAlmacen();
    void setcompany(company *comp) {companyact = comp;};
    virtual void setidalmacen(QString idalmacen);
public slots:
    void m_activated(int index) {
	if (index >0)    
	    emit(valueChanged(m_cursorcombo->valor("idalmacen",index-1)));
	else
	    emit(valueChanged(""));
    };
signals:
	void valueChanged(QString);
};

#endif


