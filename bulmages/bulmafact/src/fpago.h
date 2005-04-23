//
// C++ Interface: %{MODULE}
//
// Description: 
//
//
// Author: Tomeu Borrás Riera, (C) 2005
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef FPAGO_H
#define FPAGO_H

#include <fpagobase.h>
#include "dialogchanges.h"


class company;

/**
Gestión de Formas de Pago

@author Tomeu Borras
*/
class fpago : public fpagobase, dialogChanges
{
Q_OBJECT
private:
	company *companyact;
	QString m_idforma_pago; // Indica cual es el objeto que se está mostrando.
	bool m_modoConsulta;   /// Indica si es modo consulta o modo edición. (altera el comportamiento del doble click sobre la lista)	
private:
	/// Se encarga de hacer la carga del query inicial y de mostrar la lista bien y presentar el elemento que se especifique.
	void pintar();
public:
    fpago(company * emp, QWidget *parent = 0, const char *name = 0);
    ~fpago();
    void setModoConsulta() {m_modoConsulta = TRUE;};
    void setModoEdicion() {m_modoConsulta = FALSE;};
    bool trataModificado(); 
private slots:
    virtual void s_lista(QListViewItem *);
    virtual void close();
    virtual void s_saveFPago();
    virtual void s_newFPago();
    virtual void s_deleteFPago();
};

#endif
