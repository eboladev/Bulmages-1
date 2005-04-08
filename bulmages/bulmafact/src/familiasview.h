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

/**
@author Tomeu Borras
*/
class company;

class familiasview : public familiasdlg  {
Q_OBJECT
private:
   company *companyact;
   QString m_idfamilia;			// Indica cual es el registro que se está modificando. Sirve para saber los cambios que hay que guardar cuando se ha modificado.
   bool m_modificado;
public:
    familiasview(company *, QWidget *parent = 0, const char *name = 0);

    ~familiasview();
public slots:
	void s_setModificado() {m_modificado=TRUE;};
	void s_releaseModificado() {m_modificado=FALSE;};
	void pintar();
};

#endif
