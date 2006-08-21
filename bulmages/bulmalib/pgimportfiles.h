/***************************************************************************
 *   Copyright (C) 2003 by Santiago Capel                                  *
 *   Santiago Capel Torres (GestiONG)                                      *
 *   Tomeu Borras Riera                                                    *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 ***************************************************************************/

#ifndef __PGIMPORTFILES__
#define __PGIMPORTFILES__

#include <QFile>
#include <QString>
#include <QXmlDefaultHandler>
#include <Q3PtrList>

#include "postgresiface2.h"
#include "funcaux.h"

#define IMPORT_TODO 0xFFFFFFFF
/// Tipos para BulmaCont:
#define IMPORT_CUENTAS 1
#define IMPORT_TIPOSIVA 2
#define IMPORT_ASIENTOS 4
#define IMPORT_AINTELIGENTES 8
#define IMPORT_BALANCES 16
#define IMPORT_COBROS 32
#define IMPORT_FACTURAS 64

/// Tipos para BulmaFact:
#define IMPORT_FAMILIAS 1
#define IMPORT_ALBARANESCLIENTE 2
#define IMPORT_CLIENTES 128
#define IMPORT_PROVEEDORES 256
#define IMPORT_ARTICULOS 512
#define IMPORT_FACTURASCLIENTE 1024
#define IMPORT_ALMACENES 2048
#define IMPORT_TRABAJADORES 4096
#define IMPORT_FORMAS_PAGO 8192
#define IMPORT_PRESUPUESTOSCLIENTE 16384
#define IMPORT_PEDIDOSCLIENTE  4

/// Clase para importacion y exportacion a distintos formatos de archivo de datos.
class pgimportfiles {
private:
    /// Base de datos con la que trabaja la clase y de la que se hace importacion/exportacion.
    postgresiface2 *conexionbase;
    /// La clase puede hacer una simulacion o no dependiendo del valor de esta variable.
    bool m_modoTest;
    /// Las importaciones y exportaciones pueden ser entre dos fechas, m_fInicial indica la
    /// fecha inicial a partir de la que hacer la importacion.
    QString m_fInicial;
    /// Las importaciones y exportaciones pueden ser entre dos fechas, m_fFinal indica la
    /// fecha final a partir de la que hacer la importacion.
    QString m_fFinal;

public:
    /// Para que la clase pueda emitir el estado de completitud se inicializa con un puntero
    /// a funcion.
    virtual void alerta(int, int) {}
    ;
    /// Para que la clase pueda emitir mensajes de error o de alerta se inicializa con un
    /// puntero de funcion.
    virtual void mensajeria(QString) {
        _depura("Funcion no implementada en esta clase\n", 2);
    };
    void setFInicial(QString f) {
        m_fInicial = f;
    };
    void setFFinal(QString f) {
        m_fFinal = f;
    };
    void setModoTest() {
        m_modoTest = TRUE;
    };
    void setModoNormal() {
        m_modoTest = FALSE;
    };
    bool modoTest() {
        return (m_modoTest == TRUE);
    };
    pgimportfiles(postgresiface2 *);
    virtual ~pgimportfiles() {}
    ;
    int contaplus2Bulmages(QFile &, QFile &);
    int bulmages2Contaplus(QFile &, QFile &);
    /// Esta funcion pasa datos de una empresa al formato XML.
    int bulmages2XML(QFile &, unsigned long long int tipo = IMPORT_TODO);
    int bulmafact2XML(QFile &, unsigned long long int tipo = IMPORT_TODO);
    /// Esta funcion pasa datos de XML a bulmages.
    int XML2Bulmages(QFile &, unsigned long long int tip = IMPORT_TODO);
    int XML2BulmaFact(QFile &, unsigned long long int tip = IMPORT_TODO);
    QString searchParent(QString);
};


/// Clase para leer archivos de XML y hacer la importacion de datos.
class StructureParser : public QXmlDefaultHandler {
private:
    postgresiface2 *conexionbase;
    QString cadintermedia; /// Esta variable va almacenando los valores que van saliendo en la clase.
    /// Variables usadas para almacenar los datos de un asiento.
    QString idasiento;
    QString ordenasiento;
    QString fechaasiento;
    /// Variables usadas para almacenar los datos de un apunte.
    QString idapunte;
    QString idborrador;
    QString fechaapunte;
    QString codigocuentaapunte;
    QString debeapunte;
    QString haberapunte;
    QString conceptocontableapunte;
    int m_ordenapunte;
    /// Variables usadas para almacenar los datos de una cuenta.
    QString idcuenta;
    QString descripcioncuenta;
    QString codigocuenta;
    QString codigopadre;
    QString m_bloqueadaCuenta;
    QString m_nodebeCuenta;
    QString m_nohaberCuenta;
    QString m_tipoCuenta;
    /// Variables usadas para almacenar los datos del registro de IVA.
    QString m_idRegistroIva;
    QString m_rIvaContrapartida;
    QString m_rIvaBaseImp;
    QString m_rIvaIva;
    QString m_rIvaFFactura;
    QString m_rIvaFactura;
    QString m_rIvaCIF;
    QString m_rIvaIdFPago;
    QString m_rIvRecRegIva;
    /// Variables usadas para almacenar los datos de la tabla de IVA's.
    QString m_idTipoIva;
    QString m_baseIva;
    QString m_nombreTipoIva;
    /// El tagpadre indica en que posicion estamos. Si estamos en un asiento, un apunte, una cuenta, etc etc etc.
    QString tagpadre;

public:
    StructureParser(postgresiface2 *, unsigned int tip = IMPORT_TODO);
    ~StructureParser();
    bool startDocument();
    bool startElement(const QString&, const QString&, const QString&, const QXmlAttributes&);
    bool endElement(const QString&, const QString&, const QString&);
    bool characters(const QString&);

private:
    QString indent;
    unsigned int m_tipo;
};


/// Clase para leer archivos de XML y hacer la importacion de datos.
/// Usamos este tipo para almacenar todos los valores que va recogiendo la clase.
typedef QMap<QString, QString> tvalores;


class ImportBulmaFact : public QXmlDefaultHandler {
private:
    postgresiface2 *conexionbase;
    QString cadintermedia; /// Esta variable va almacenando los valores que van saliendo en la clase.
    /// Variables usadas para almacenar los datos de un asiento.
    tvalores valores;
    /// El tagpadre indica en que posicion estamos. Si estamos en un asiento, un apunte, una cuenta, etc etc etc.
    QString tagpadre;
    pgimportfiles *pgimport;
    /// Estas estructuras sirven para guardar datos intermedios.
    Q3PtrList<tvalores> listalpresupuesto;
    Q3PtrList<tvalores> listadpresupuesto;
    Q3PtrList<tvalores> listalfactura;
    Q3PtrList<tvalores> listadfactura;
    Q3PtrList<tvalores> listalalbaran;
    Q3PtrList<tvalores> listadalbaran;
    Q3PtrList<tvalores> listalpedidocliente;
    Q3PtrList<tvalores> listadpedidocliente;

public:
    ImportBulmaFact(pgimportfiles *, postgresiface2 *, unsigned long long int tip = IMPORT_TODO);
    ~ImportBulmaFact();
    bool startDocument();
    bool startElement(const QString&, const QString&, const QString&, const QXmlAttributes&);
    bool endElement(const QString&, const QString&, const QString&);
    bool characters(const QString&);

private:
    QString indent;
    unsigned int m_tipo;
    void printcontents();
    int trataCliente();
    int trataProveedor();
    int trataFormaPago();
    int trataAlmacen();
    int trataArticulo();
    int trataFactura();
    int trataLFactura();
    int trataDFactura();
    int trataPresupuesto();
    int trataLPresupuesto();
    int trataDPresupuesto();
    int trataFamilia();
    int trataAlbaran();
    int trataLAlbaran();
    int trataDAlbaran();
    int trataPedidoCliente();
    int trataLPedidoCliente();
    int trataDPedidoCliente();
};

#endif

