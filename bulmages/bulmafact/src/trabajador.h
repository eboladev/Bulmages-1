//
// C++ Interface: %{MODULE}
//
// Description: 
//
//
// Author: Tomeu Borrass Riera, (C) 2005
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef TRABAJADOR_H
#define TRABAJADOR_H

#include <trabajadorbase.h>
#include "dialogchanges.h"


class company;

/**
Gestión de Formas de Pago

@author Tomeu Borras
*/
class Trabajador : public Trabajadorbase, dialogChanges
{
Q_OBJECT
private:
	company *companyact;
	QString m_idtrabajador; // Indica cual es el objeto que se está mostrando.
	bool m_modoConsulta;   /// Indica si es modo consulta o modo edición. (altera el comportamiento del doble click sobre la lista)	
	QString m_archivoimagen;
private:
	/// Se encarga de hacer la carga del query inicial y de mostrar la lista bien y presentar el elemento que se especifique.
	void pintar();
public:
    Trabajador(company * emp, QWidget *parent = 0, const char *name = 0);
    ~Trabajador();
    void setModoConsulta() {m_modoConsulta = TRUE;};
    void setModoEdicion() {m_modoConsulta = FALSE;};
    bool trataModificado(); 
private slots:
    virtual void s_lista(Q3ListViewItem *);
    virtual bool close(bool);
    virtual void s_saveTrabajador();
    virtual void s_newTrabajador();
    virtual void s_deleteTrabajador();
    virtual void s_cambiarImagen();
};

#endif
