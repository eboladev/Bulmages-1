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
#include "dialogchanges.h"
#include <tipoivadlg.h>

class empresa;

/** Esta clase hace el mantenimiento de la tabla de Tipos de IVA
  * Su funcionamiento se basa un poco en el funcionamiento de la pantalla de canales.
  */
class tipoivaview : public tipoivadlg , dialogChanges {
Q_OBJECT
private:
   empresa *empresaactual;
   postgresiface2 *conexionbase;
   cursor2 *m_curtipoiva;
   int m_posactual; /// Indica la posición en el combobox que está seleccionada. Se usa para hacer saves en caso de modificaciones.
public:
    tipoivaview(empresa *, QWidget *parent = 0, const char *name = 0);
    ~tipoivaview();
private:
    void pintar(QString idtipoiva="");
    void mostrarplantilla(int pos=0);
private slots:
    virtual void cambiacombo(int);
    virtual void s_saveTipoIVA();
    virtual void s_newTipoIVA();
    virtual void s_deleteTipoIVA();
    virtual bool close(bool);
};

#endif
