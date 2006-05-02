//
// C++ Interface: TarifaListSubform
//
// Description:
//
//
// Author: Tomeu Borras <tborras@conetxia.com>, (C) 2005
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef TARIFALISTSUBFORM_H
#define TARIFALISTSUBFORM_H

/**
@author Tomeu Borras
*/


#include <QEvent>
#include <QTableWidget>

#include "qtable2.h"
#include "subform2bf.h"




class TarifaListSubform : public SubForm2Bf {
Q_OBJECT
public:
	TarifaListSubform(QWidget *parent = 0, const char *name = 0);
	~TarifaListSubform() {};

public slots:
virtual void cargar() {
    _depura("TarifaListSubform::cargar\n",0);
    QString SQLQuery = "SELECT * FROM tarifa";
    cursor2 * cur= companyact()->cargacursor(SQLQuery);
	SubForm3::cargar(cur);
    delete cur;
};


};

#endif
