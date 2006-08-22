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

#ifndef ASIENTO1VIEW_H
#define ASIENTO1VIEW_H

#include "ui_asientobase.h"

#include <QWidget>
#include <qstring.h>
#include <QHBoxLayout>

#include "qtable2.h"
#include "postgresiface2.h"
#include "configuracion.h"
#include "fixed.h"
#include "busquedafecha.h"
#include "asiento1.h"

class empresa;

/**
  *@author Tomeu Borrás Riera 
 * \class Asiento1View asiento1view.h
 * \brief Se encarga de controlar la ventana de introducción de apuntes.
 *
 * Esta es una de las clases mas complejas del programa ya que controla toda la acción y casi toda la interactuación del usuario con el programa. Pretende conseguir una interficie que resulte muy rígida y cómodo para el usuario que introduzca datos.
 Hereda intapunts3dlg  
*/
class ListAsientos {

private:
    empresa *m_companyact;
    /// Este es el cursor que se usará para recorrer la lista de asientos.
    cursor2 *cursorasientos;

public:

    ListAsientos(empresa *);
    virtual ~ListAsientos();
    void cargaasientos();
    void boton_inicio();
    void boton_fin();
    void boton_siguiente();
    void boton_anterior();

    void boton_filtrar() {
	_depura("Funcion no implementada", 2);
    };

    virtual void muestraasiento(QString ) {
        mensajeInfo( "Funcion no implementada.");
    };
    virtual void pintaasiento(QString ) {
        _depura("Funcion no implementada.", 2);
    };
    void situarasiento(QString);
    bool esprimerasiento() {
        return cursorasientos->esprimerregistro();
    };
    bool esultimoasiento() {
        return cursorasientos->esultimoregistro();
    };

    QString idasientoanterior() {
        if (!cursorasientos->esprimerregistro()) {
            cursorasientos->registroanterior();
            QString id = cursorasientos->valor("idasiento");
            cursorasientos->siguienteregistro();
            return id;
        } else {
            return "";
        }
    };

    QString idasientosiguiente() {
        if (!cursorasientos->esultimoregistro()) {
            cursorasientos->siguienteregistro();
            QString id = cursorasientos->valor("idasiento");
            cursorasientos->registroanterior();
            return id;
        } else {
            return "";
        }
    };

};


class Asiento1View : public QWidget, public Ui_AsientoBase,  public Asiento1, public ListAsientos {
    Q_OBJECT
private:

    /// ESte puntero del tipo \ref empresa contiene la referencia a la clase que ha inicializado este objeto.
    empresa *m_companyact;


    void pintafecha(QString val) {
        mui_fecha->setText(val);
    };

    void pintaordenasiento(QString val) {
        m_ordenasiento->setText(val);
    };

    void pintaclase(QString val) {
	mui_claseAsiento->setCurrentIndex(val.toInt());
    };

    void pintacomentariosasiento(QString text) {
	mui_comentariosAsiento->setText(text);
    };

    virtual void calculaypintatotales(QString);
    void prepguardar();

public:
    void pintaasiento(QString v) {
        _depura("Asiento1View::pintaasiento ", 0);
        cargar(v);
        _depura("END Asiento1View::pintaasiento ", 0);

    };

    void muestraasiento(QString v) {
        _depura("Asiento1View::muestraasiento ", 0);
        situarasiento(v);
        pintaasiento(v);
        _depura("END Asiento1View::muestraasiento ", 0);
    };

    void muestraasiento(int v) {
        muestraasiento(QString::number(v));
    };

    ///Para poder enganchar plugins a esta ventana se ha habilitado este layout.
    QHBoxLayout *layoutPlugins;
public:


    void setFecha(QString val) {
        mui_fecha->setText(val);
    };

    virtual void trataestadoAsiento1();
    void asientoabiertop();
    void asientocerradop();
    void iniciar_asiento_nuevo();

    Asiento1View(empresa *, QWidget *parent=0, const char *name=0, int flags=0);
    ~Asiento1View();
public slots:
    virtual void on_mui_abrirasiento_clicked() {
        abreAsiento1();
    };
    virtual void on_mui_cerrarasiento_clicked() {
	prepguardar();
        cierraAsiento1();
    };
    virtual void on_mui_guardarasiento_clicked() { prepguardar();guardar();};
    virtual void on_mui_nuevoasiento_clicked();
    virtual void on_mui_borrar_clicked();
    virtual void on_mui_duplicar_clicked();
    virtual void on_mui_inteligente_clicked();
    virtual void on_mui_fecha_returnPressed();


    /** \brief SLOT que responde a la pulsación del botón de iva.
      * Crea la clase \ref ivaview y la inicializa con el identificador de borrador para que se presente con los datos ya introducidos.
      * La clase ivaview hace una inserción o una modificación segun exista o no una entrada de iva para dicho borrador.
      */
    virtual void on_mui_iva_clicked() {
        mui_list->boton_iva();
    };

    virtual void s_lineaValueChanged() {
        calculaypintatotales(idasiento());
    };

    /** Al pulsar return sobre el número de asiento se procede como si fuese una
      * carga de dicho asiento.
      */
    void eturn_numasiento() {
        boton_cargarasiento();
    };
    virtual void boton_cargarasiento();
};

#endif
