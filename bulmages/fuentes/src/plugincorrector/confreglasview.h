//
// C++ Interface: confreglasview
//
// Description: 
//
//
// Author: Tomeu Borras <tborras@conetxia.com>, (C) 2005
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef CONFREGLASVIEW_H
#define CONFREGLASVIEW_H

#include <confreglas.h>

/**
  * @author Tomeu Borras
  * \brief Permite configurar reglas que el corrector monitoree.
  *
  * \ Una funcionalidad extra para el corrector es permitir al usuario configurar determinadas reglas
  * para que haga un monitoreo de las mismas.
  * De esta forma el usuairo puede controlar determinados detalles de la contabilidad que de otra forma
  * le sería imposible controlar.
*/

class confreglasview : public confreglas {
Q_OBJECT
public:
    confreglasview(QWidget *parent = 0, const char *name = 0);

    ~confreglasview();

};

#endif
