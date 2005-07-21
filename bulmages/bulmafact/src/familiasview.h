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
#ifndef FAMILIASVIEW_H
#define FAMILIASVIEW_H

#include <familiasdlg.h>
#include "dialogchanges.h"

/** @author Tomeu Borras */
class company;

class familiasview : public familiasdlg, dialogChanges  {
Q_OBJECT
private:
   company *companyact;
   QString m_idfamilia;			// Indica cual es el registro que se est�modificando. Sirve para saber los cambios que hay que guardar cuando se ha modificado.
   bool m_modoConsulta;   /// Indica si es modo consulta o modo edici�. (altera el comportamiento del doble click sobre la lista)
public:
    familiasview(company *, QWidget *parent = 0, const char *name = 0);

    ~familiasview();
    bool trataModificado();
    void mostrarplantilla();
    void setModoConsulta() {m_modoConsulta = TRUE;};
    void setModoEdicion() {m_modoConsulta = FALSE;};
    QString codigoCompletoFamilia();
    QString idFamilia();
    QString nombreFamilia();
public slots:
	virtual void pintar();
	virtual void s_doubleClicked(QListViewItem *);
	virtual void close();
	virtual void s_saveFamilia();
	virtual void s_newFamilia();
	virtual void s_deleteFamilia();
	virtual void s_changedFamilia();
	virtual void s_imprimir();
};

#endif
