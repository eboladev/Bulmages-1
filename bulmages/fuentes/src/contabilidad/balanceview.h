/***************************************************************************
                          balanceview.h  -  description
                             -------------------
    begin                : sáb abr 26 2003
    copyright            : (C) 2003 by Tomeu Borrás Riera
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

#ifndef BALANCEVIEW_H
#define BALANCEVIEW_H

#include "balancedlg.h"
#include "intapunts3view.h"
#include "diarioview1.h"
#include "extractoview1.h"
#include "qtable1.h"
#include "qlayout.h"
#include "postgresiface2.h"
#include "configuracion.h"
#include "listcuentasview1.h"

class intapunts3view;
class diarioview1;
class extractoview1;
class empresa;

/**
 * \author Tomeu Borrás Riera
 * \class balanceview balanceview.h
 * \brief Balance de Sumas y Saldos plano
 * \todo Hay que eliminar el ccostes[200] y cambiarlo por un cursor
 * 
 * Clase que muestra un balance de sumas y saldos de la empresa abierta. 
 Esta clase  genera el balance de sumas y saldos y lo muestra por pantalla en una
 ventana diseñada específicamente para ello.
 */

class balanceview : public balancedlg  {
  Q_OBJECT
private:
/// Empresa con la que se ha inicializado la clase
  empresa *empresaactual;
/// Base de datos con la que trabaja la conexión
  postgresiface2* conexionbase;
/// Estas tres clases deben ser sustituidas por algo en la clase empresa
  intapunts3view *introapunts;
  diarioview1 *diario;
  extractoview1 *extracto;
/// Cursor para recorrer las cuentas
  cursor2 *cursorcta;
/// El número de dígitos que tienen las cuentas de la empresa
  int numdigitos;
/// Array que almacena los identificadores de los centros de coste
  int ccostes[200];
/// En esta tabla se sustituye el listado que tiene de QTDesigner por este de aqui
  QTable1 *listado;
public: 
  balanceview(empresa *, QWidget *parent=0, const char *name=0, int flags=0);
  void inicializa1(QString, QString, QString, QString, int);
  void inicializa2(intapunts3view *, diarioview1 *, extractoview1 *);
  ~balanceview();
  void cargacostes();
private:
  void boton_extracto1(int);
  void boton_asiento();
  void boton_diario1(int);
  void presentar();
public slots:
  virtual void accept();
  virtual void boton_imprimir();
private slots:
  virtual void return_fechafinal();
  virtual void return_fechainicial();
  virtual void return_codigoinicial();
  virtual void return_codigofinal();
  virtual void boton_buscacuentainicial();
  virtual void boton_buscacuentafinal();
  virtual void boton_fechainicial();
  virtual void boton_fechafinal();
  virtual void contextmenu(int , int , const QPoint &);
  virtual void nivelactivated(int);
  virtual void codigo_textChanged(const QString &);
  virtual void fecha_textChanged(const QString &);

};
#endif
