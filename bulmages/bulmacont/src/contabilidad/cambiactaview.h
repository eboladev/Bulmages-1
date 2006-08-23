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
#ifndef CAMBIACTAVIEW_H
#define CAMBIACTAVIEW_H

#include "ui_cambiactabase.h"
#include "postgresiface2.h"


/**@ author Tomeu Borras */

class empresa;

class cambiactaview : public QDialog, public Ui_CambiaCuentaBase {
Q_OBJECT
private:
	empresa *empresaactual;
public:
	cambiactaview(empresa *, QWidget *, const char *, bool );
    ~cambiactaview();
public slots:
	virtual void accept();
	
};

#endif
