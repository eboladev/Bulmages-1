#ifndef BUSQUEDAPROVINCIA_H
#define BUSQUEDAPROVINCIA_H

#include <QComboBox>

#include "company.h"
#include "postgresiface2.h"

/**
@author Tomeu Borras
*/

class BusquedaProvincia : public QComboBox {
Q_OBJECT
private:
company *companyact;
cursor2 *m_cursorcombo;

public:
    BusquedaProvincia(QWidget *parent = 0, const char *name = 0);
    ~BusquedaProvincia();
    void setcompany(company *comp) {companyact = comp;};
    virtual void setProvincia(QString provincia);
public slots:
    void m_activated(int index) {
	if (index >0)    
	    emit(valueChanged(m_cursorcombo->valor("provincia",index-1)));
	else
	    emit(valueChanged(""));
    };
signals:
	void valueChanged(QString);
};

#endif


