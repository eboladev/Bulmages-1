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
/**
@author Tomeu Borras
*/

#ifndef TIPOIVAVIEW_H
#define TIPOIVAVIEW_H

#include "postgresiface2.h"
#include <tipoivadlg.h>

class empresa;

/** Esta clase hace el mantenimiento de la tabla de Tipos de IVA
  * Su funcionamiento se basa un poco en el funcionamiento de la pantalla de canales.
  */
class tipoivaview : public tipoivadlg {
Q_OBJECT
private:
   empresa *empresaactual;
   postgresiface2 *conexionbase;
   QString m_idtipoiva;
   cursor2 *m_curtipoiva;
public:
    tipoivaview(empresa *, QWidget *parent = 0, const char *name = 0);
    ~tipoivaview();
private:
    void pintar();
    void mostrarplantilla();
private slots:
    virtual void cambiacombo(int);
};

#endif
