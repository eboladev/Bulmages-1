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

/**@author Tomeu Borras*/
class amortizacionview : public amortizaciondlg
{
Q_OBJECT
public:
    amortizacionview::amortizacionview(QWidget *, const char *, bool );
    ~amortizacionview();
};

#endif
