/***************************************************************************
                          ivaview.h  -  description
                             -------------------
    begin                : Tue Jan 28 2003
    copyright            : (C) 2003 by Josep Burcion and Tomeu Borrás Riera
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

#ifndef IVAVIEW_H
#define IVAVIEW_H

#include <qwidget.h>
#include <ivadlg.h>
#include <qlineedit.h>
#include <qcombobox.h>

#include "postgresiface2.h"



class empresa;

/** @author Tomeu Borrás Riera
  * \brief Formulario de introducción y/o visión de una factura emitida o recibida.
  *
  * Clase que sirve para introducir toda la información relacionada con facturas emitidas o soportadas.
  Con la finalidad de almacenar la información necesaria para cumplimentar determinados documentos fiscales y
  también para almacenar toda la gestión de cobros y pagos.
  */
class ivaview : public ivadlg  {
    Q_OBJECT
private:
    /// Cursor para las formas de pago.
    cursor2 *m_cursorFPago;
public:
    /// Empresa con la que se trabaja
    empresa *empresaactual;
    /// Base de datos con la que se trabaja
    postgresiface2 *conexionbase;
    /// Borrador bajo el que se hace el registro de la factura
    int idborrador;
    /// Identificador de la tabla de facturas
    int idregistroiva;
    /// Numero de dígitos de la contabilidad.
    int numdigitos;

public:
    ivaview(empresa *, QWidget *parent=0, const char *name=0);
    ~ivaview();
    void inicializa1(int );
    void guardaprevpago(int );
    void guardaprevpago();
    void guardaiva(int );
    void guardaiva();
    void cambiadasubcuenta(int);
    void cargacobros();
private:
    void cargarComboFPago(QString);
    int buscaborradoriva(int);
    int buscaborradorcliente(int);
    int buscaborradorservicio(int);
    void cargaiva(QString);
    void cargaComboTipoIva();
    void calculaTotales();
    void buscafecha(int);

public slots:
    virtual void accept();
    virtual void boton_borrar();
    virtual void boton_buscacuenta();
    virtual void cambiadacontrapartida();
    virtual void boton_generarPrevisiones();
    virtual void boton_fecha();
    virtual void chContrapartida();

private slots:
    virtual void tcambiaseleccion();
    virtual void cambiadogrid(int, int);
    virtual void pulsadomas(int, int, int);
    virtual void currentIvaChanged(int, int);

};

#endif
