/***************************************************************************
                          asientosview.h  -  description
                             -------------------
    begin                : Sat Dec 21 2002
    copyright            : (C) 2002 by Tomeu Borr� Riera
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

#ifndef ASIENTOSVIEW_H
#define ASIENTOSVIEW_H

#include <QWidget>

#include "postgresiface2.h"
#include "ui_asientolistbase.h"

/** @author Tomeu Borr� Riera
  * \brief Muestra un listado de todos los asientos existentes e informa de si estan abiertos o cerrados.
*/

class empresa;

class asientosview : public QWidget, public Ui_AsientoListBase  {
    Q_OBJECT
public:
    empresa *m_companyact;
public:
    asientosview(empresa *emp, QWidget *parent=0);
    ~asientosview();
    void inicializa();
public slots:
    virtual void on_mui_list_cellDoubleClicked(int, int);
    virtual void on_mui_actualizar_clicked() {
        inicializa();
    };
    virtual void on_mui_configurar_clicked() {
        mui_list->showConfig();
    };
    virtual void on_mui_imprimir_clicked();
};
#endif
