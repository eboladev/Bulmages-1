//
// C++ Implementation: msgerror
//
// Description: 
//
//
// Author: Tomeu Borras <tborras@conetxia.com>, (C) 2005
//
// Copyright: See COPYING file that comes with this distribution
//
//
#include "msgerror.h"

#include <qlabel.h>
#include <q3textedit.h>

msgError::msgError(QWidget *parent, const char *name)
 : QDialog(parent, name){
  setupUi(this);

  QObject::connect(pushButton1,SIGNAL(clicked(bool)),this,SLOT(close()));
  QObject::connect(pushButton2,SIGNAL(clicked(bool)),this,SLOT(s_mostarDetalles()));
}

msgError::msgError(QString mensaje, QString descripcion) {
  setupUi(this);
	hideFrame();
	m_mensaje->setText(mensaje);
	m_detalles->setText(descripcion);
	exec();
}


msgError::~msgError()
{
}


