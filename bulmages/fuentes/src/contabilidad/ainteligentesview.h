/***************************************************************************
                          ainteligentesview.h  -  description
                             -------------------
    begin                : Thu Feb 6 2003
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

#ifndef AINTELIGENTESVIEW_H
#define AINTELIGENTESVIEW_H

#include <qwidget.h>
#include "ainteligentesdlg.h"
#include "postgresiface2.h"


/**
 * \author Tomeu Borrás Riera
 * \class ainteligentesview ainteligentesview.h
 * \brief Edición de Asientos Inteligentes
 * \todo Cambiar el array de centros de coste por un cursor
 * \todo Cambiar el array de canales por un cursor
 * 
 * Clase que sirve para editar y crear asientos plantilla (inteligentes)
 * Los asientos intelignetes son un mecanismo mediante el cual podemos hacer la introducción de asientos de una forma mucho más rápida que la convencional.
 */
class empresa;


class ainteligentesview : public ainteligentesdlg  {
   Q_OBJECT
private:
/// La clase carga un cursor con todos los asientos inteligentes disponibles para hacer una navegación sobre éstos.
   cursor2 *m_cAInteligentes;
/// La empresa con la que esta trabajando la clase.   
   empresa *empresaactual;
/// La base de datos con la que esta trabajando la clase.   
   postgresiface2 *conexionbase;
/// El identificador del asiento inteligente con el que estamos trabajando.   
   int m_idAsientoInteligente;
/// Listado de los centros de coste disponibles    
   int m_cCostes[100];
/// Listado de los canales disponibles   
   int m_cCanales[100];
/// Para la generación de los centros de coste y canales en el listado hacen falta estos dos campos   
   QComboBox *combocoste;
   QComboBox *combocanal;
/// Indica cual es la columna anterior con la que se ha tratado   
   int m_oldRow;
  
public: 
  ainteligentesview(empresa *, QWidget *parent=0, const char *name=0, bool modal=true);
  ~ainteligentesview();
private:
/// Función que inicializa todos los controles del formulario a los valores adecuados
  void repinta();
  void return_asiento();
/// Función que se encarga de rellenar los canales disponibles y el array \ref m_cCanales  
  void cargacanales();
/// Se encarga de rellenar los centros de coste y el array \ref m_cCostes
  void cargacostes();
/// Rellena el combobox m_ainteligente
  void cargacombo();
  
private slots:
  virtual void boton_nuevo();
  virtual void boton_save();
  virtual void boton_borrar();
  virtual void boton_inicio();
  virtual void boton_fin();
  virtual void boton_siguiente();
  virtual void boton_anterior();
  virtual void boton_exportar();
  virtual void boton_importar();
  virtual void currentChanged(int, int);
  virtual void selectionChanged();
  virtual void boton_igualant();
  virtual void boton_cuentas();
  virtual void accept();
  virtual void comboActivated (const QString &);
};

#endif
