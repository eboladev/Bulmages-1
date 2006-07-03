/***************************************************************************
                          empresa.h  -  description
                             -------------------
    begin                : Wed Nov 27 2002
    copyright            : (C) 2002 by Tomeu Borr�
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

#ifndef EMPRESA_H
#define EMPRESA_H

#include <qmessagebox.h>
#include <q3filedialog.h>
#include <qstring.h>
#include <QWorkspace>
#include <qwidget.h>
#include <qsizepolicy.h>


#include "balanceview.h"
#include "diarioview.h"
#include "extractoview1.h"
#include "bmodelo347.h"
#include "postgresiface2.h"
#include "log.h"
#include "listventanas.h"


class selectccosteview;
class selectcanalview;
class balance1view;
class Asiento1View;
class DiarioView;
class balanceview;
class asientosview;

/**
 * @author Tomeu Borras Riera
 * \class empresa empresa.h
 * \brief Soporte principal para cada empresa contable
 *  
 * Clase que soporta todas las funciones de la empresa contable. Supuestamente el programa podra tener multiples empresas abiertas al mismo tiempo.
 * Esta clase es el corazon del programa donde acaban confluyendo todos los dem� objetos.
 * Se encarga de instanciar los objetos necesarios, inicializarlos e invocarlos cuando es preciso.
 * Tambien debe servir como punto de comunicacion entre los diferentes objetos de modo que estos solo tengan referencias a esta clase y haciendo el flujo del codigo mas facil de entender.
 * \todo proporcionar la funcionalidad de comunicacion entre las otras clases.
 */
class empresa  : public postgresiface2 {
private:
    listventanas *m_listventanas;

    QString nombre;
    int ano;
    QString contrasenya;
    QWorkspace *pWorkspace;

    balance1view *balance1;
    DiarioView *diario;
    extractoview1 *extracto;
    balanceview *balance;
    Asiento1View *introapunts2;
    asientosview *m_listasientos;
    unsigned int numdigitos;  // El numero de digitos que tienen las cuentas.

    // Hay unos selectores generales, que van a ser estos y que indican con que se trabaja, luego
    // Estos selectores generales se podran invocar desde distintas partes.
    selectccosteview *selccostes;
    selectcanalview *selcanales;
public:
    QString nombreDB;

public:
    empresa();
    virtual ~empresa();

    void init(QString);
    /// Returns a pointer to the database class
    postgresiface2 *bdempresa() {
        return (this);
    }

    balanceview *balanceempresa() {
        return balance;
    }
    extractoview1 *extractoempresa() {
        return extracto;
    }
    DiarioView *diarioempresa() {
        return diario;
    }
    balance1view *balance1empresa() {
        return balance1;
    }

    Asiento1View *intapuntsempresa() {
        return(introapunts2);
    }



    Asiento1View *intapuntsempresa2() {
        return(introapunts2);
    }
    int numdigitosempresa() {
        return(numdigitos);
    }
    QString nomuserempresa() {
        return(nombre);
    }
    QString nombreempresa() {
        return(nombre);
    }

    void setWorkspace(QWorkspace *qw) {
        pWorkspace=qw;
    };

    void setListVentanas(listventanas *doc) {
        m_listventanas = doc;
    };

    int meteWindow(QString nom, QObject *obj) {
        return m_listventanas->meteWindow(nom, obj);
    };

    void sacaWindow(QObject *nom) {
        m_listventanas->sacaWindow(nom);
    };

    int muestracuentas();
    int nuevacuenta();
    int muestraapuntes();
    int muestraapuntes1();
    int muestraasientos();
    int propiedadempresa();
    int nuevaempresa();
    int borrarempresa();
    /// This function search for a comapany
    QString searchCompany();
    int libromayor();
    int librodiario();
    int librobalance();
    int librobalancetree();
    int registroiva();
    int modelo347();
    int inicializa1();
    int boton_siguiente();
    int boton_anterior();
    int boton_primero();
    int boton_ultimo();
    int boton_guardar();
    int boton_imprimir();
    int boton_reload();
    int ainteligentes();
    int ccostes();
    int canales();
    int tiposIVA();
    int fPago();
    void cierraempresa();
    void abreempresa();
    void regularizaempresa();
    void maximiza();
    void Abrirasientos();
    void Ordenarasientos();
    void Filtro();
    void nuevoejercicio();
    int mpatrimoniales();
    int compbalance();
    int amortizaciones();
    void reemplazacuentaenasientos();
    int cambioejercicio();
    void centrocostedefecto();
    void canaldefecto();
    selectccosteview *getselccostes();
    selectcanalview *getselcanales();
    void recalculasaldos();
    int cobPag();  // Gestion de cobros y pagos
    virtual void s_asiento1();

};
#endif
