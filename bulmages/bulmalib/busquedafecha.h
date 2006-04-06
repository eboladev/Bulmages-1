//
// C++ Interface: busquedafecha
//
// Description: 
//
//
// Author: Tomeu Borras <tborras@conetxia.com>, (C) 2005
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef BUSQUEDAFECHA_H
#define BUSQUEDAFECHA_H

#include "ui_busquedafechabase.h"
#include <qlineedit.h>
#include <qwidget.h>

/**
@author Tomeu Borras
*/
class BusquedaFecha :  public QWidget, public Ui_BusquedaFechaBase
{
Q_OBJECT
private:
public:
    BusquedaFecha(QWidget *parent = 0, const char *name = 0);
    ~BusquedaFecha();
        virtual void setfecha(QString val) {m_fecha->setText(val);};   
	virtual void setText(QString val) {m_fecha->setText(val);};
	virtual QString fecha() {return(m_fecha->text());};
	virtual QString text() {return m_fecha->text();};
public slots:
	virtual void s_searchFecha();
	virtual void s_fechatextChanged(const QString &);   
	virtual void s_fechalostFocus(); 
	virtual void s_returnPressed() {s_fechalostFocus();emit returnPressed();};

	virtual void selectAll() {m_fecha->selectAll();};
	virtual void setFocus() {m_fecha->setFocus();};
signals:
	void valueChanged(QString);
	void returnPressed();
};

#endif
