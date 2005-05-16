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
#ifndef TIPOSARTICULOVIEW_H
#define TIPOSARTICULOVIEW_H

#include <tiposarticulodlg.h>
#include "dialogchanges.h"

/** @author Tomeu Borras */
class company;

class tiposarticuloview : public tiposarticulodlg, dialogChanges  {
Q_OBJECT
private:
   company *companyact;
   QString m_idtipo;                 // Indica cual es el registro que se está modificando. Sirve para saber los cambios que hay que guardar cuando se ha modificado.
   bool m_modoConsulta;   /// Indica si es modo consulta o modo edición. (altera el comportamiento del doble click sobre la lista)
public:
    tiposarticuloview(company *, QWidget *parent = 0, const char *name = 0);

    ~tiposarticuloview();
    bool trataModificado();
    void mostrarplantilla();
    void setModoConsulta() {m_modoConsulta = TRUE;};
    void setModoEdicion() {m_modoConsulta = FALSE;};
    QString codTipo();
public slots:
        virtual void pintar();
        virtual void s_doubleClicked(QListViewItem *);
        virtual void close();
        virtual void s_saveTipo();
        virtual void s_newTipo();
        virtual void s_deleteTipo();
        virtual void s_changedTipo();
};

#endif
