//
// C++ Interface: %{MODULE}
//
// Description: 
//
//
// Author: %{AUTHOR} <%{EMAIL}>, (C) %{YEAR}
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef FPAGOVIEW_H
#define FPAGOVIEW_H

#include <fpagodlg.h>
#include "postgresiface2.h"

/**
@author Tomeu Borras
*/
class empresa;


class fpagoview : public fpagodlg  {
Q_OBJECT
private:
    empresa *empresaactual;
    postgresiface2 *conexionbase;
   cursor2 *m_curfpago;   
private:
    void pintar(QString idtipoiva="");
    void mostrarplantilla(int pos=0); 
public:
    fpagoview(empresa *, QWidget *parent = 0, const char *name = 0);
    ~fpagoview();
private slots:
    virtual void cambiacombo(int);
    virtual void s_saveFPago();
    virtual void s_newFPago();
    virtual void s_deleteFPago();
};

#endif
