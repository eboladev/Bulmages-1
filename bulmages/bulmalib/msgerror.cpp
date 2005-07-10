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

#include <qdialog.h>
#include <qlabel.h>
#include <qtextedit.h>

msgError::msgError(QWidget *parent, const char *name)
 : msgErrorBase(parent, name){
}

msgError::msgError(QString mensaje, QString descripcion) {
	hideFrame();
	m_mensaje->setText(mensaje);
	m_detalles->setText(descripcion);
	exec();
}


msgError::~msgError()
{
}


