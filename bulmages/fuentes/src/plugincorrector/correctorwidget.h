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
#ifndef CORRECTORWIDGET_H
#define CORRECTORWIDGET_H

#include <qstring.h>
#include <qdockwindow.h>
#include "correctorwdt.h"
#include "postgresiface2.h"
#include "empresa.h"

/** @author Tomeu Borras */



class correctorwidget : public correctorwdt
{
Q_OBJECT
public:
	postgresiface2 *conexionbase;
	empresa *empresaactual;
	QString textBrowser;
	QDockWindow *dock;
public:
    correctorwidget( QWidget* parent = 0, const char* name = 0, WFlags fl = 0 );
    ~correctorwidget();
	void setEmpresa(empresa *empres) { empresaactual = empres; conexionbase = empres->bdempresa();}
	void agregarError(QString, QString, QString);
public slots:
	virtual void corregir();
	virtual void link(const QString &);
	virtual void alink(const QString &, const QString &);
	virtual void cambia(bool a) {
		fprintf(stderr,"CAmbia de corrector\n");
	       if (a) dock->show();
	       else dock->hide();};
};

#endif
