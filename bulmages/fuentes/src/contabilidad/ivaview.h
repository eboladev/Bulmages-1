/***************************************************************************
                          ivaview.h  -  description
                             -------------------
    begin                : Tue Jan 28 2003
    copyright            : (C) 2003 by Josep Burcion and Tomeu Borrás Riera
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

#ifndef IVAVIEW_H
#define IVAVIEW_H

#include <qwidget.h>
#include <ivadlg.h>
#include <qlineedit.h>
#include <qcombobox.h>

#include "postgresiface2.h"

/** 
  * @author Tomeu Borrás Riera 
  */

class empresa;  
  
class ivaview : public ivadlg  {
  Q_OBJECT
private:
   cursor2 *m_cursorFPago;   // Cursor para las formas de pago.
   
public:
  empresa *empresaactual;
  postgresiface2 *conexionbase;
  int idborrador;
  int idregistroiva;
  int numdigitos;

public: 
  ivaview(empresa *, QWidget *parent=0, const char *name=0);
  ~ivaview();
  void inicializa1(int );
  void guardaprevpago(int );
  void guardaprevpago();
  void guardaiva(int );
  void guardaiva();
  void cambiadasubcuenta(int);
  void cargacobros();
private:
  void cargarComboFPago(QString);
  int buscaborradoriva(int);
  int buscaborradorcliente(int);
  int buscaborradorservicio(int);
  void cargaiva(QString);
  void cargaComboTipoIva();
  void calculaTotales();

public slots:
  virtual void accept();
  virtual void boton_borrar();
  virtual void boton_buscacuenta();
  virtual void cambiadacontrapartida();
  virtual void boton_generarPrevisiones();
  virtual void returnContrapartida();
//  virtual void currentIvaChanged(int, int);

private slots:
  virtual void tcambiaseleccion();
  virtual void cambiadogrid(int, int);
  virtual void pulsadomas(int, int, int);
  virtual void currentIvaChanged(int, int);
  
};

#endif
