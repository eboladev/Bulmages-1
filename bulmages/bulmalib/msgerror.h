//
// C++ Interface: msgerror
//
// Description: 
//
//
// Author: Tomeu Borras <tborras@conetxia.com>, (C) 2005
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef MSGERROR_H
#define MSGERROR_H

#include <ui_msgerrorbase.h>

#include <q3frame.h>
#include <qdialog.h>

/**
@author Tomeu Borras
*/
class msgError :  public QDialog, public Ui_msgErrorBase
{
Q_OBJECT
public:
    msgError(QWidget *parent = 0, const char *name = 0);
    msgError(QString, QString);
    virtual ~msgError(); 
    
    void hideFrame() {m_framedetalles->hide();};
    void showFrame() {m_framedetalles->show();};  
    
public slots:    
    virtual void s_mostrarDetalles() {
    	if (m_framedetalles->isVisible())
		hideFrame();
	else
		 showFrame();
	};         
};

#endif
