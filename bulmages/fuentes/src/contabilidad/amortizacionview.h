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
QString idamortizacion;  // Esta variable indica cual es la amortizacion que se està enseñando. Si no tiene valor es que no se esta mostrando ninguna.
public:
    amortizacionview::amortizacionview(empresa *, QWidget *, const char *, bool );
    ~amortizacionview();
    void inicializa(QString);
public slots:
	virtual void accept();
};
#endif
