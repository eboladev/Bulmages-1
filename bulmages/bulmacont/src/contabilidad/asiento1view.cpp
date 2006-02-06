/***************************************************************************
                          intapunts3view.cpp  -  description
                             -------------------
    begin                : mar may 27 2003
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
#include <q3popupmenu.h>
#include <qlineedit.h>
#include <q3listview.h>
#include <qtoolbutton.h>
#include <qpushbutton.h>
//Added by qt3to4:
#include <QHBoxLayout>
#include <stdlib.h>
#include <qinputdialog.h>
#include <qcolor.h>
#include <qfont.h>
#include <qstringlist.h>
#include <qlayout.h>
#include <qlabel.h>

#include "asiento1view.h"

/*
#include "asientoview.h"
#include "calendario.h"
#include "ivaview.h"
#include "aplinteligentesview.h"
#include "listcuentasview1.h"
#include "cuentaview.h"
#include "duplicarasientoview.h"
#include "selectccosteview.h"
#include "selectcanalview.h"
*/

#include "busquedafecha.h"

#define COL_FECHA       0
#define COL_SUBCUENTA   1
#define COL_CONCEPTO    6
#define COL_DEBE        4
#define COL_HABER       5
#define COL_NOMCUENTA   2
#define COL_CONTRAPARTIDA 3
#define COL_IDBORRADOR    7
#define COL_TIPOIVA       8
#define COL_IVA           9
#define COL_CCOSTE       10
#define COL_CANAL        11
#define COL_IDCUENTA     12
#define COL_IDCONTRAPARTIDA 13
#define COL_IDCANAL         14
#define COL_IDCCOSTE        15
#define COL_ORDEN	    16


/// Usar esta macro es peligroso ya que el cursor puede estar vacio.
/// es preferible usar la función idAsiento() que hace la comprobación.
#define IDASIENTO cursorasientos->valor("idasiento").ascii()
#define QS_IDASIENTO cursorasientos->valor("idasiento")
#define ORDENASIENTO cursorasientos->valor("ordenasiento").ascii()
#define ROWACTUAL tapunts3->currentRow()

/// Define el número de filas que va a tener la tabla de apuntes.
#define TAPUNTS_NUM_ROWS 10000

/** \brief Devuelve en un QString el valor del asiento que se está visualizando
  *
  *  Si no hay ningún asiento viendose devuelve "-1"
  */
QString Asiento1View::idAsiento() {
    if (cursorasientos->eof())
        return "-1";
    return IDASIENTO;
}

/** \brief Constructor de la clase, inicializa los componentes
  * \param emp empresa que llama al objeto 
  * \param parent widget padre de éste
  * \param nam Nombre que recibe el widget o ventana
  */
Asiento1View::Asiento1View(empresa *emp,QWidget *parent, const char *name, int  ) : Asiento1Dlg (parent,name) ,  Asiento1(emp) {
    _depura("Constructor de Asiento1View\n",0);
    companyact = emp;
    setListLinAsiento1(subform2);

    _depura("FIN del Constructor de Asiento1View\n",0);
}// end intapunts3view


/** \brief Destructor de la clase
  *
  * Destruye los objetos creados y libera la memoria
  */
Asiento1View::~Asiento1View() {
    delete filt;
    if (cursorasientos != NULL ) {
        delete cursorasientos;
    }// end if
}// end intapunts3view



