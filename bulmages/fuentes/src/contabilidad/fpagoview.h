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
  * Fichero que contiene la declaración de la clase \ref fpagoview que trata la ventana de gestión de formas de pago
  * \author Tomeu Borrás Riera
  */
#ifndef FPAGOVIEW_H
#define FPAGOVIEW_H

#include <fpagodlg.h>
#include "postgresiface2.h"


class empresa;

/**
  * \brief la clase fpagoview se encarga de la gestión de formas de pago
  *
  * Las formas de pago son unas plantillas que se usan para determinar las formas en que se pagan  o se
  * cobran las facturas emitidas o recibidas
  */
class fpagoview : public fpagodlg  {
Q_OBJECT
private:
    empresa *empresaactual;
    postgresiface2 *conexionbase;
   cursor2 *m_curfpago;   
   bool m_modificado; ///Indica si se han modificado campos en la pantalla.
   int m_posactual; /// Indica la posición en el combobox que está seleccionada. Se usa para hacer saves en caso de modificaciones.
private:
    void pintar(QString idtipoiva="");
    void mostrarplantilla(int pos=0); 
public:
    fpagoview(empresa *, QWidget *parent = 0, const char *name = 0);
    ~fpagoview();
private slots:
    virtual void s_setModificado() {m_modificado=TRUE;};
    virtual void s_releaseModificado() {m_modificado=FALSE;}
    virtual void cambiacombo(int);
    virtual void s_saveFPago();
    virtual void s_newFPago();
    virtual void s_deleteFPago();
    virtual void close();    
};

#endif
