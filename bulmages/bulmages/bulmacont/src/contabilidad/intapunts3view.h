/***************************************************************************
                          intapunts3view.h  -  description
                             -------------------
    begin                : mar may 27 2003
    copyright            : (C) 2003 by Tomeu Borrás Riera and Josep Burcion
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

#ifndef INTAPUNTS3VIEW_H
#define INTAPUNTS3VIEW_H

#include "int_apunts3.h"

#include <qwidget.h>
#include <q3table.h>
#include <qstring.h>
//Added by qt3to4:
#include <QHBoxLayout>


#include "filtrarasientosview.h"
#include "extractoview1.h"
#include "balanceview.h"
#include "diarioview1.h"
#include "qtable1.h"
#include "postgresiface2.h"
#include "configuracion.h"

#include "fixed.h"
#include "busquedafecha.h"

class extractoview1;
class diarioview1;
class balanceview;
class empresa;

/**
  *@author Tomeu Borrás Riera 
 * \class intapunts3view intapunts3view.h
 * \brief Se encarga de controlar la ventana de introducción de apuntes.
 *
 * Esta es una de las clases mas complejas del programa ya que controla toda la acción y casi toda la interactuación del usuario con el programa. Pretende conseguir una interficie que resulte muy rígida y cómodo para el usuario que introduzca datos.
 Hereda intapunts3dlg  
*/

class intapunts3view : public intapunts3dlg  {
    Q_OBJECT
private:
    /// El identificador del asiento que se está visualizando en cada momento. Si no existe asiento actual vale -1
    int idasiento;
    /// El identificador de la fila sobre la que está el cursor. Si no hay row seleccionado valdrá-1
    int rowactual;
    /// Indica que el asiento esta abierto.
    int abierto;
    /// Puntero a la clase amiga \ref extractoview1 \todo el paso de mensajes debería hacerse a traves de la clase empresa.
    extractoview1 *extracto;
    /// Puntero a la clase amiga \ref diarioview1 \todo el paso de mensajes debería hacerse a través de la clase empresa y este puntero debe desaparacer.
    diarioview1 *diario;
    /// Puntero a la clase amiga \ref balanceview \todo el paso de mensajes debería hacerse a través de la clase empresa.
    balanceview *balance;
    /// Puntero a la conexión de la base de datos abierta actualmente.
    postgresiface2 *conexionbase;
    /// Indica el nmero de dígitos que usan por defecto las cuentas. Es un parametro sacado de la configuración de la empresa.
    unsigned int numdigitos;
public:

    /// Este es el cursor que se usará para recorrer la lista de asientos.
    cursor2 *cursorasientos;
    /// Este objeto contiene todas las opciones de filtraje necesarias para funcionar. es un objeto del tipo \ref filtrarasientosview
    filtrarasientosview *filt;
    /// ESte puntero del tipo \ref empresa contiene la referencia a la clase que ha inicializado este objeto.
    empresa *empresaactual;
    ///Para poder enganchar plugins a esta ventana se ha habilitado este layout.
    QHBoxLayout *layoutPlugins;
public:
    void setFecha(QString val) {m_fecha->setText(val);};

    QString idAsiento();
    intapunts3view(empresa *, QWidget *parent=0, const char *name=0, int flags=0);
    ~intapunts3view();
    int inicializa1(extractoview1 *, diarioview1 *, balanceview *);
    void cargarcursor();
    void repinta(int);
    void muestraasiento(int);
    void flashAsiento(int);
    void vaciarapuntes();
    void asientoabiertop();
    void asientocerradop();
    void tcambiaseleccion();
    int guardaborrador(int);
    void calculadescuadre();
    void duplicar(int);
    void duplicarapunte();
    void borraborrador(int);
    void cambiadasubcuenta(int);
    void cambiadacontrapartida(int);
    void buscacontrapartida(int);
    void cambiadodebe(int);
    void cambiadohaber(int);
    void asiento_cierre();
    void asiento_apertura();
    void asiento_regularizacion();
    void subirapunte(int);
    void bajarapunte(int);
    void iniciar_asiento_nuevo();
    void borrar_asiento(bool);
    void buscaFactura();
public slots:
    virtual void contextmenu(int, int,const QPoint &);
    virtual void apuntecambiadogrid(int, int);
    virtual void boton_iva();
    virtual void pulsadomas(int, int, int);
    virtual void boton_inteligente();
    virtual void boton_borrar_asiento();
    virtual void boton_buscacuenta();
    virtual void boton_extracto1(int);
    virtual void boton_diario1(int);
    virtual void boton_balance1(int);
    virtual void boton_filtrar();
    virtual void boton_abrirasiento();
    virtual void boton_cerrarasiento();
    virtual void boton_nuevoasiento();
    virtual void boton_inicio();
    virtual void boton_fin();
    virtual void boton_siguiente();
    virtual void boton_anterior();
    virtual void eturn_fechaasiento();
    virtual void boton_cargarasiento();
    virtual void eturn_numasiento();
    virtual void editarasiento();
    virtual void boton_duplicarasiento();
};

#endif
