/***************************************************************************
 *   Copyright (C) 2004 by Josep Burcion                                   *
 *   josep@burcion.com                                                     *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 ***************************************************************************/
#ifndef Bbuscador_H
#define Bbuscador_H

#include <qlineedit.h>
#include <qlabel.h>
#include <qcheckbox.h>

#include "uibuscador.h"
#include "bfempresa.h"

//#include "postgresiface2.h"


/**
@author Josep Burcion
*/
class MyQTable;

class Bbuscador : public UIbuscador {
    Q_OBJECT

public:
    //Bbuscador(QString* cadBusqueda, QWidget * parent = 0, const char * name = 0, WFlags f = WType_TopLevel);
    Bbuscador::Bbuscador(QWidget* parent=0, const char* name=0, WFlags f= WType_TopLevel,BfEmpresa* punteroEmpresaTrabajo=0, QString* cadenaRet=0, QString* tabla=0, QString* args=0, ...); 
    ~Bbuscador();

private:
    QString* cadRetorno;
    MyQTable* tablaResultados; //La clase MyQTable es herdera de QTable.
    BfEmpresa* EmpresaTrabajo;
    QString tablaBusqueda;

private slots:
    void llenarTabla(const QString &);
    void pulsadoEnter(int, int);


};

#endif

/***************************************************************************/
/* Reimplemantamos la clase QTable sobre MyQTable                          */
/***************************************************************************/
#include <qtable.h>

class MyQTable : public QTable{
  Q_OBJECT

public:
  MyQTable(QWidget * parent = 0, const char * name = 0 ):QTable(parent, name ) {
  }
  
  virtual bool eventFilter( QObject *obj, QEvent *event ) {
      if ( event->type() == QEvent::KeyPress ) {
          QKeyEvent *keyEvent = (QKeyEvent *) event;
          int key = keyEvent->key();
          if (key == Qt::Key_Enter || key == Qt::Key_Return) { // El enter
              emit keyEnterPressed(currentRow(), currentColumn());
              return TRUE;
          }
      }
      return QTable::eventFilter(obj, event);
  }
signals:
    void keyEnterPressed(int, int);
};
