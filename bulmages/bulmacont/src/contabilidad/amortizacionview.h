//
//
// C++ Interface: $MODULE$
//
// Description: 
//
//
// Author: Tomeu Borras <tborras@conetxia.com>, (C) 2003
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef AMORTIZACIONDLGAMORTIZACIONVIEW_H
#define AMORTIZACIONDLGAMORTIZACIONVIEW_H

#include "amortizaciondlg.h"
#include "postgresiface2.h"
#include "dialogchanges.h"
#include "ui_amortizacionbase.h"

class empresa;

/** 
  * @author Tomeu Borrás Riera 
  * \class amortizacionview amortizacionview.h
  * \brief Formulario de introducción y/o visión de amortizaciones.
  */
  
class amortizacionview : public QWidget, Ui_AmortizacionBase , dialogChanges {
  Q_OBJECT
    public:
  // Empresa con la que se trabaja
  empresa *m_companyact;
  // Esta variable indica cual es el indice de la cuenta de activo de la amortizacion que estamos viendo. Si no se está mostrando una cuenta este valor vale "".
  QString idctaactivo; 
  // Esta variable indica cual es el indice de la cuenta de amortizacion de la amortizacion que estamos viendo. Si no se está mostrando una cuenta este valor vale "".
  QString idctaamortizacion;
  // Esta variable indica cual es la amortizacion que se está enseñando. Si no tiene valor es que no se esta mostrando ninguna.
  QString idamortizacion;  
  // Esta variable indica cual es el asiento plantilla de amortización.
  QString idainteligente;   
  amortizacionview(empresa *, QWidget *, const char * );
  ~amortizacionview();
  void inicializa(QString);
  void trataModificado();
  public slots:
    virtual bool close(bool);
  virtual void accept();
  virtual void calculaamortizacion();
  virtual void contextMenuRequested(int, int, const QPoint &);
  virtual void s_saveAmortizacion();
  virtual void s_newAmortizacion();
  virtual void s_deleteAmortizacion();
};

#endif
