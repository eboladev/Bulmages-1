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

#include "cambiactadlg.h"
#include "empresa.h"
#include "postgresiface2.h"


/**@ author Tomeu Borras */
class cambiactaview : public cambiactadlg {
Q_OBJECT
public:
	empresa *empresaactual;
public:
	cambiactaview(empresa *, QWidget *, const char *, bool );
    ~cambiactaview();
public slots:
	virtual void boton_buscactaorigen();
	virtual void boton_buscactadestino();
	virtual void accept();
	virtual void return_codigoinicial();
	virtual void return_codigofinal();
	virtual void return_fechainicial();
	virtual void return_fechafinal();
	virtual void codigo_textChanged(const QString &);
	virtual void fecha_textChanged(const QString &);
	
};

#endif
