//
//
// C++ Interface: $MODULE$
//
// Description: 
//
//
// Author: Tomeu Borras <tborras@conetxia.com>, (C) 2003
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef DUPLICARASIENTOVIEW_H
#define DUPLICARASIENTOVIEW_H

#include "duplicaasientodlg.h"
#include "empresa.h"

/**@author Tomeu Borras*/
class duplicarasientoview : public duplicaasientodlg
{
Q_OBJECT
public:
	empresa *empresaactual;
	int idasiento;
public:
	duplicarasientoview(empresa *, QWidget *, const char *, bool );
	~duplicarasientoview();
	void inicializa(QString, QString);
public slots:
	virtual void accept();
	virtual void lostFocus();

};

#endif
