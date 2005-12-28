//
// C++ Interface: busquedafamilia
//
// Description: 
//
//
// Author: Tomeu Borras <tborras@conetxia.com>, (C) 2005
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef BUSQUEDAFAMILIA_H
#define BUSQUEDAFAMILIA_H

#include <busquedafamiliabase.h>
#include <qlineedit.h>
#include <qlabel.h>

class company;
/**
@author Tomeu Borras
*/
class BusquedaFamilia : public BusquedaFamiliaBase {
Q_OBJECT
private:
company *companyact;
QString mdb_idfamilia;
QString mdb_nombrefamilia;
QString mdb_codigocompletofamilia;
public:
    BusquedaFamilia(QWidget *parent = 0, const char *name = 0);
    ~BusquedaFamilia();
    void setcompany(company *comp) {companyact = comp;};
    virtual QString codigocompletofamilia() {return m_codigocompletofamilia->text();};
    virtual QString idfamilia() {return mdb_idfamilia;};
    virtual QString nombrefamilia() {return nombrefamilia();};
    virtual void setidfamilia(QString val);
    virtual void setcodigocompletofamilia(QString val);
public slots:
	virtual void s_searchFamilia();
	virtual void s_codigofamiliatextChanged(const QString &);
signals:
	void valueChanged(QString );
};

#endif
