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

class empresa;

/**@author Tomeu Borras*/
class amortizacionview : public amortizaciondlg
{
Q_OBJECT
public:
empresa *empresaactual;
postgresiface2 *conexionbase;
public:
    amortizacionview::amortizacionview(empresa *, QWidget *, const char *, bool );
    ~amortizacionview();
};

#endif
