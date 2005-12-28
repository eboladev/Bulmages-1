#ifndef BUSQUEDATRABAJADOR_H
#define BUSQUEDATRABAJADOR_H

#include <qcombobox.h>

#include "company.h"
#include "postgresiface2.h"

/**
@author Tomeu Borras
*/

class BusquedaTrabajador : public QComboBox {
Q_OBJECT
private:
company *companyact;
cursor2 *m_cursorcombo;

public:
    BusquedaTrabajador(QWidget *parent = 0, const char *name = 0);
    ~BusquedaTrabajador();
    void setcompany(company *comp) {companyact = comp;};
    virtual void setidtrabajador(QString idtrabajador);
public slots:
    void m_activated(int index) {
    	fprintf(stderr,"Activado el combo box\n");
	if (index >0)
	    emit(valueChanged(m_cursorcombo->valor("idtrabajador",index-1)));
	else 
	    emit(valueChanged(""));	
    };
signals:
	void valueChanged(QString);
};

#endif


