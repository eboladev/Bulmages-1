/***************************************************************************
                          ListRegistroIvaView.h  -  description
                             -------------------
    begin                : Thu Jan 30 2003
    copyright            : (C) 2003 by Tomeu Borras� Riera
    email                : tborras@conetxia.com
 ***************************************************************************/
/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/
/***************************************************************************
      LISTA DE BUGS CONOCIDOS
      
      - Las cuentas del estilo 47200477 o 47700472 deformaran los resultados
       ya que la busqueda esta hecha con un LIKE que no contempla estos casos.
 
      - Las fechas incorrectas producen mal funcionamiento pej: 31/06/2003 no
      dara ningun registro pq dia 31 no existe.
 ***************************************************************************/

#ifndef LISTREGISTROIVAVIEW_H
#define LISTREGISTROIVAVIEW_H

// #include <QTableWidgetItem>
#include <QWidget>
#include <qlineedit.h>
#include <q3table.h>
#include <q3popupmenu.h>
#include "ui_listregistroivabase.h"

#include "empresa.h"

/** *@author Tomeu Borras� Riera */


class ListRegistroIvaView : public QWidget, public Ui_ListRegistroIvaBase  {
    Q_OBJECT
private:
	/// Opciones del menu contextual
	QAction *m_verreg;
	QAction *m_verasiento;
public:
    empresa *m_companyact;
public:
    ListRegistroIvaView(empresa *, QString ejerActual="",QWidget *parent=0, const char *name=0);
    ~ListRegistroIvaView();
    void inicializa();
public slots:
    virtual void on_mui_tablasoportado_cellDoubleClicked(int, int);
    virtual void on_mui_tablarepercutido_cellDoubleClicked(int, int);
    virtual void on_mui_tablarepercutido_pintaMenu(QMenu *menu);
    virtual void on_mui_tablasoportado_pintaMenu(QMenu *menu);
    virtual void on_mui_tablasoportado_trataMenu(QAction *ac);
    virtual void on_mui_tablarepercutido_trataMenu(QAction *ac);
    virtual void on_mui_actualizar_clicked();
    virtual void boton_print();
    virtual void on_mui_list_editFinished(int row, int col) {};
};

#endif
