/***************************************************************************
                          diarioprintview.h  -  description
                             -------------------
    begin                : jue jun 26 2003
    copyright            : (C) 2003 by Tomeu Borr� Riera
    email                : tborras@conetxia.com
    modificat per        : (C) 2003 Antoni Mirabete i Ter� - amirabet@biada.org
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
#include <q3datetimeedit.h>
#include <q3filedialog.h>
#include <qwidget.h>
#include <q3table.h>
#include <qtoolbutton.h>
#include <qradiobutton.h>

#include "filtrardiarioview.h"

/** @author Tomeu Borr� Riera
  * @brief Ventana de impresi� de diario
  * @class DiarioPrintView diarioprintview.h
  * Es la ventana de impresi� de diarios.
 */
 
/// Declaramos la clase empresa como amiga para que la compilaci� no sea complicada.
#include "empresa.h"

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
   /// Establece cual es la ventana de filtro de diario. Es imprescindible inicializar el filtro antes de mostrar la ventana o el bot� de filtro no funcionar�
   void setFiltro(filtrardiarioview *filtro) {m_filt = filtro;};
   void inicializa1(QString, QString);
   DiarioPrintView(empresa *emp, QWidget *parent, const char *name);
   ~DiarioPrintView();
   void presentar(char *tipus="html");
   // OJO con esta funci� que es una funci� para probar el Report Tool Kit
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
