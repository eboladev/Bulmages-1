/***************************************************************************
                          extractoprintview.h  -  description
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

#ifndef EXTRACTOPRINTVIEW_H
#define EXTRACTOPRINTVIEW_H

#include <errno.h>

#include <qwidget.h>
#include <extractoprintdlg.h>
#include <qlineedit.h>
#include <q3datetimeedit.h>
#include <q3filedialog.h>
#include <qwidget.h>
#include <q3table.h>
#include <qtoolbutton.h>
#include <qradiobutton.h>


#include "filtrarextractosview.h"
#include "postgresiface2.h"
/** @author Tomeu Borrás Riera
  * \class ExtractoPrintView extractoprintview.h
  * \brief Clase que corresponde con la ventana de impresión de extractos.
  * \todo Falta hacer que en los campos se haga el autocompletado.
  * \todo Falta incluir el filtrado en el extracto.
*/

class empresa;

class ExtractoPrintView : public ExtractoPrintDlg  {
   Q_OBJECT
public:
   char *fichero;
   postgresiface2 *conexionbase;
   empresa *empresaactual;
   filtrarextractosview *m_filt;
   
   void setFiltro(filtrarextractosview *filtro) {m_filt = filtro;};
   void inicializa1(QString , QString , QString , QString );
   ExtractoPrintView(empresa *, QWidget *, const char *);
   void presentar(char *tipus);
   ~ExtractoPrintView();
   void pruebasRTK();
   void presentakugar();
private:
   QString montaQuery();
public slots:
   virtual void accept();
   virtual void boton_ccostes();
   virtual void boton_canales();
   virtual void s_botonFiltrar();
};

#endif
