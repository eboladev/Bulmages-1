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
#include <qtable.h>
#include <qstring.h>


#include "filtrarasientosview.h"
#include "extractoview1.h"
#include "balanceview.h"
#include "diarioview1.h"
#include "qtable1.h"
#include "postgresiface2.h"
#include "configuracion.h"

class extractoview1;
class diarioview1;
class balanceview;
class empresa;

/**
  *@author Tomeu Borrás Riera 
 * \class intapunts3view intapunts3view.h
 * \brief Se encarga de controlar la ventana de introducción de apuntes.
 * \todo Eliminar los arrays de canales y centros de coste y sustituirlos por listas o por conjuntos.
 * Esta es una de las clases más complejas del programa ya que controla toda la acción y casi toda la interactuación del usuario con el programa. El intento es conseguir una interficia que resulte muy rápida y cómodo para el usuario que introduzca datos.
 Hereda intapunts3dlg  
*/

class intapunts3view : public intapunts3dlg  {
    Q_OBJECT
private:
    int cidapunte;
    int cdescapunte;
    int cfechaapunte;
    int idasiento;            // El identificador del asiento actual.
    int rowactual;            // El identificador del row actual. Si no hay row seleccionado valdrá -1
    float descuadre1;
    int abierto;              // Indica que el asiento esta abierto.
    extractoview1 *extracto;
    diarioview1 *diario;
    balanceview *balance;
    QTable *tapunts;
    postgresiface2 *conexionbase;
    unsigned int numdigitos;
    int ccostes[200];
    int ccanales[200];
public:
    QString idAsiento();
    cursor2 *cursorasientos;   // Este es el cursor que se usará para recorrer la lista de asientos.
    filtrarasientosview *filt; // Este objeto contiene todas las opciones de filtraje necesarias para funcionar.
    empresa *empresaactual;
    QHBoxLayout *layoutPlugins;
public:
    intapunts3view(empresa *, QWidget *parent=0, const char *name=0, int flags=0);
    ~intapunts3view();
    int inicializa(postgresiface2 *);
    int inicializa1(extractoview1 *, diarioview1 *, balanceview *);
    void cargarcursor();
    void repinta(int);
    void muestraasiento(int);
    void flashAsiento(int);
    void vaciarapuntes();
    void asientoabiertop();
    void asientocerradop();
    void tcambiaseleccion();
    void guardaborrador(int);
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
    virtual void fechaasiento1_textChanged( const QString & );
    virtual void boton_cargarasiento();
    virtual void eturn_numasiento();
    virtual void editarasiento();
    virtual void boton_duplicarasiento();
    virtual void boton_fecha();
};

#endif
