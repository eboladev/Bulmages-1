/***************************************************************************
                          DiarioView.h  -  description
                             -------------------
    begin                : Thu Jan 9 2003
    copyright            : (C) 2003 by Tomeu Borrás Riera
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

#ifndef DIARIOVIEW12_H
#define DIARIOVIEW12_H

/**
 * \author Tomeu Borrás Riera
 * \class DiarioView DiarioView.h
 * \brief Libro diario
 * 
 * Clase que muestra el libro diario de la empresa entre dos fechas.
 */

#include "ui_diariobase.h"

class empresa;

class DiarioView : public QWidget, public Ui_DiarioBase  {
    Q_OBJECT
private:

    /// Empresa con la que esta trabajando esta clase.
    empresa *m_companyact;
public:
    DiarioView(empresa *, QWidget *parent=0, const char *name=0, int flags=0);
    ~DiarioView();
    void accept();
    /// Inicializa la clase con las fechas inicial y final. El ltimo parametro no se usa.
    void inicializa1(QString, QString, int);
private:
    /// Muestra en pantalla los resultados para los datos introducidos.
    void presentar();
public slots:
    /// Slot que responde a la pulsación del boton actualizar en el formulario.
    /// Supuestamente debe repintar el formulario con las nuevas opciones de filtrado que se hayan elegido.
    virtual void on_mui_actualizar_clicked() {
        accept();
    };

    /// Slot que responde a la pulsación del boton imprimir en el formulario.
    /// Realiza la impresión del listado que tenemos en pantalla.
    virtual void on_mui_imprimir_clicked();

    /// SLOT que responde a la pulsación del botón imprimir. Es invocado desde la clase \ref empresa
    virtual void boton_imprimir();
    /// SLOT que responde a la pulsación del botón guardar listado. Es invocado desde la clase \ref empresa
    virtual void boton_guardar();

    /// SLOT que responde a la activacion del boton de configurar listado. Muestra el configurador de subform3 \ref subform3
    virtual void on_mui_configurar_clicked() {
        mui_list->showConfig();
    };
};
#endif
