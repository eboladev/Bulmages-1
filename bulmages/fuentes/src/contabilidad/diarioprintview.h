/***************************************************************************
                          diarioprintview.h  -  description
                             -------------------
    begin                : jue jun 26 2003
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

#ifndef DIARIOPRINTVIEW_H
#define DIARIOPRINTVIEW_H

#include <errno.h>

#include <qwidget.h>
#include "diarioprintdlg.h"
#include "postgresiface2.h"
#include <qlineedit.h>
#include <qdatetimeedit.h>
#include <qfiledialog.h>
#include <qwidget.h>
#include <qtable.h>
#include <qtoolbutton.h>
#include <qradiobutton.h>

#include "filtrardiarioview.h"

/** @author Tomeu Borrás Riera
  * @brief Ventana de impresión de diario
  * @class DiarioPrintView diarioprintview.h
  * Es la ventana de impresión de diarios.
 */
 
/// Declaramos la clase empresa como amiga para que la compilación no sea complicada.
class empresa;

class DiarioPrintView : public DiarioPrintDlg  {
   Q_OBJECT
public:
   /// La base de datos con la que trabaja esta clase
   postgresiface2 *conexionbase;  
   /// La empresa con la que trabaja esta clase.
   empresa *empresaactual;
   int numdigitos;
   /// Puntero al filtrado de datos
   filtrardiarioview *m_filt;
   
public:
   /// Establece cual es la ventana de filtro de diario. Es imprescindible inicializar el filtro antes de mostrar la ventana o el botón de filtro no funcionará.
   void setFiltro(filtrardiarioview *filtro) {m_filt = filtro;};
   void inicializa1(QString, QString);
   DiarioPrintView(empresa *emp, QWidget *parent, const char *name);
   ~DiarioPrintView();
   void presentar(char *tipus="html");
   // OJO con esta función que es una función para probar el Report Tool Kit
   void pruebasRTK();
   void pruebasRTKoo();
   void presentakugar();
private:
   QString montaQuery();
public slots:
   virtual void accept();
   virtual void boton_canales();
   virtual void boton_ccostes();
   virtual void s_botonFiltrar();
};

#endif
