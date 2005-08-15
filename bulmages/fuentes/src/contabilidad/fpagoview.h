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
/** \file fpagoview.h
  * Fichero que contiene la declaraci� de la clase \ref fpagoview que trata la ventana de gesti� de formas de pago
  * \author Tomeu Borr� Riera
  */
#ifndef FPAGOVIEW_H
#define FPAGOVIEW_H

#include <fpagodlg.h>
#include "postgresiface2.h"
#include "dialogchanges.h"

class empresa;

/**
  * \brief la clase fpagoview se encarga de la gesti� de formas de pago
  *
  * Las formas de pago son unas plantillas que se usan para determinar las formas en que se pagan  o se
  * cobran las facturas emitidas o recibidas
  */
class fpagoview : public fpagodlg , dialogChanges {
Q_OBJECT
private:
    empresa *empresaactual;
    postgresiface2 *conexionbase;
   cursor2 *m_curfpago;   
   int m_posactual; /// Indica la posici� en el combobox que est�seleccionada. Se usa para hacer saves en caso de modificaciones.
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
    virtual bool close(bool);    
};

#endif
