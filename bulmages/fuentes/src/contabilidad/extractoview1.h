/***************************************************************************
                          extractoview1.h  -  description
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
/** \file extractoview1.h
  * Este fichero contiene la definición de la clase \ref extractoview1
  * \author Tomeu Borrás Riera
  */
#ifndef EXTRACTOVIEW1_H
#define EXTRACTOVIEW1_H

#include "filtrarextractosview.h"
#include "extractodlg1.h"
#include "qtable1.h"
#include "postgresiface2.h"



class intapunts3view;
class diarioview1;
class balanceview;
class empresa;


/** @brief Esta clase Mantiene la generación de extractos contables
  * 
  * A partir de esta panalla se pueden llevar a cabo todos los temas de punteo y casación.
  * Y acceso a filtrado. También desde esta pantalla se puede pedir más información de una cuenta
  * determinada.
  */
class extractoview1 : public extractodlg1  {
    Q_OBJECT
public:
    /// Indica el número de registros que estamos viendo.
    int numregistro;
    /// Indica el número de registros que tiene el cursor2 \ref cursorcta
    int numregistros;
    /// Indica el número de dígitos que tienen las cuentas. (TAL VEZ DEBERIA ESTAR EN CONFPR)
    int numdigitos;
    /// El extracto se realiza sobre un cursor que siempre está en memoria.
    cursor2 *cursorcta;
    /// Siempre se propaga la empresa a estos objetos.
    empresa *empresaactual;
    /// Este objeto debería pasar por la empresa para no tener problemas (Que es quien debería hacer el paso de mensajes).
    diarioview1 *diario;
    /// Este puntero debería estar ubicado en la clase empresa (Que es quien debería hacer el paso de mensajes).
    intapunts3view *introapunts;
    /// Este puntero debería estar ubicado en la clase empresa (Que es quien debería hacer el paso de mensajes).
    balanceview *balance;
    /// Base de datos con la que se inicializa la clase para hacer todas sus operaciones
    postgresiface2* conexionbase;
    /// Tabla auxiliar que almacena los centros de coste (DEBERIA CAMBIARSE POR UN QMap)
    int ccostes[200];
    /// Esta clase es el filtro que se puede aplicar sobre los extractos. Es un dialog Qt separado
    filtrarextractosview *filt;
public:
    extractoview1(empresa *,QWidget *parent=0, const char *name=0, int flags=0);
    ~extractoview1();
    void inicializa1(QString, QString, QString, QString, int);
    void inicializa2(intapunts3view *, diarioview1 *, balanceview *);
    void vaciar();
    void presentar();
    void ajustes();
    void boton_filtrar();

public slots:
    virtual void accept();
    virtual void boton_siguiente();
    virtual void boton_anterior();
    virtual void boton_fin();
    virtual void boton_inicio();
    virtual void boton_buscacuentainicial();
    virtual void boton_buscacuentafinal();
    virtual void boton_borrapunteo();
    virtual void boton_imprimir();
    virtual void boton_guardar();
    virtual void boton_asiento();
    virtual void boton_diario1(int);
    virtual void boton_fechainicial();
    virtual void boton_fechafinal();
    virtual void boton_balance1(int);
    virtual void return_codigoinicial();
    virtual void return_codigofinal();
    virtual void return_fechainicial();
    virtual void return_fechafinal();
    virtual void contextmenu(int , int , const QPoint &);
    virtual void apuntecambiadogrid (int, int);
    virtual void boton_casacion();
    virtual void boton_guardarpunteo();
    virtual void boton_cargarpunteos();
    virtual void codigo_textChanged(const QString &);
    virtual void fecha_textChanged(const QString &);
};

#endif
