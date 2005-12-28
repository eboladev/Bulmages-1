/***************************************************************************
                          regivaprintview.h  -  description
                             -------------------
    begin                : dom jul 13 2003
    copyright            : (C) 2003 by Tomeu Borrás Riera
    email                : tborras@conetxia.com
    modificat per        : (C) 2003 Antoni Mirabete i Terés - amirabet@biada.org
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#ifndef REGIVAPRINTVIEW_H
#define REGIVAPRINTVIEW_H

#include <errno.h>
#include <qwidget.h>
#include <regivaprintdlg.h>
#include <qlineedit.h>
#include <qradiobutton.h>

#include "postgresiface2.h"

class empresa;

/**
  * Trata el formulario de impresión del registro de IVA
  * y genera los listados pertinentes
  */
class regivaprintview : public regivaprintdlg  {
    Q_OBJECT
public:
    char *fichero;
    postgresiface2 *conexionbase;
    empresa *empresaactual;
public:
    regivaprintview(empresa *, QWidget *parent=0, const char *name=0);
    ~regivaprintview();
    void inicializa1(QString, QString);
    void presentar(char *tipus);
public slots:
    virtual void accept();
};

#endif
