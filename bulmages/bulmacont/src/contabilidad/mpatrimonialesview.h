/***************************************************************************
                          mpatrimonialesview.h  -  description
                             -------------------
    begin                : mi�sep 10 2003
    copyright            : (C) 2003 by Tomeu Borr� Riera
    email                : tborras@conetxia.com
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#ifndef MPATRIMONIALESVIEW_H
#define MPATRIMONIALESVIEW_H

#include <QWidget>
#include "ui_mpatrimonialesbase.h"
#include "postgresiface2.h"

/***@author Tomeu Borras Riera */

class mpatrimonialesview : public QDialog, public Ui_MPatrimonialesBase  {
   Q_OBJECT
public:
   postgresiface2 *conexionbase; // Esta variable indica cual es la base de datos con la que se trabaja.
   int numdigitos;               // Esta variable indica el nmero de digitos que tienen las cuentas de nivel mas bajo.
   int modo;                     // Esta variable indica si se abre para busqueda o para edicion.
   QString nommasa;               // Esta variable indica el nombre de la masa, luego se usara
   QString idmasa;                // Esta variable indica el identificador de masa.

public: 
	mpatrimonialesview(QWidget *parent=0, const char *name=0, int fl=0);
	~mpatrimonialesview();
   int inicializa(postgresiface2 *);
   void inicializatabla();
   void setmodoselector() {modo = 1;}
   void setmodoeditor() {modo = 0;}
   QString getnommasa() {return nommasa;}
   QString getidmasa() {return idmasa;}
   
public slots:
   virtual void dbtabla(int, int, int, const QPoint &);
   virtual void nuevamasa();
   virtual void borrarmasa();
   virtual void editarmasa();
};

#endif
