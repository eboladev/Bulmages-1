/***************************************************************************
                          filtrarextractosview.h  -  description
                             -------------------
    begin                : dom jul 27 2003
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

/*
#ifndef RESMENSUALVIEW_H
#define RESMENSUALVIEW_H

#include <qwidget.h>
#ifdef ESTADISTICAS
#include "resmensualdlg.h"
#endif

#ifdef GDCHART
#include "resmensualdlg1.h"
#endif

#include "postgresiface2.h"

*/

#ifndef FILTRAREXTRACTOSVIEW_H
#define FILTRAREXTRACTOSVIEW_H

#include <qwidget.h>
#include <qpopupmenu.h>
#include <qcombobox.h>

#include "filtrarextractosdlg.h"
#include "postgresiface2.h"
#include "selectccosteview.h"

class empresa;

/*
class resmensualview : public resmensualdlg  {
   Q_OBJECT
public: 
   postgresiface2 *conexionbase;
   string idmpatrimonial1, idmpatrimonial2, idmpatrimonial3;

   int inicializa(postgresiface2 *);
	resmensualview(QWidget *parent=0, const char *name=0);
	~resmensualview();
   void presentar();
   void presentarpie();
   void generargrafico(float *, char **);
public slots:
   virtual void accept();
   virtual void buscacodigocta();
   virtual void buscacodigocta1();
   virtual void buscacodigocta2();
   virtual void buscampatrimonial1();
   virtual void buscampatrimonial2();
   virtual void buscampatrimonial3();
};


*/


/***@ author Tomeu Borrás Riera  */

class filtrarextractosview : public filtrarextractosdlg  {

   Q_OBJECT
public:
  postgresiface2* conexionbase;
  int numdigitos;
  int ccostes[200];
  empresa *empresaactual;   
  selectccosteview *selccostes;
public:

  filtrarextractosview(empresa *, QWidget *parent=0, const char *name=0);
  ~filtrarextractosview();
  void cargacostes();
  void setccoste(int);
private:
   int m_colNomCoste;
   int m_colDescCoste;
   int m_colIdCoste;
   int m_colStatusCoste;  
public slots:
  virtual void codigo_textChanged(const QString &);
  virtual void boton_buscacontrapartida();
  virtual void return_codigo();
  virtual void boton_ccostes();
};

#endif

