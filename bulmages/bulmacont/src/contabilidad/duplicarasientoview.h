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

#include "ui_duplicarasientobase.h"

class empresa;

/**@author Tomeu Borras*/
class duplicarasientoview : public QDialog, public Ui_DuplicarAsientoBase
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
