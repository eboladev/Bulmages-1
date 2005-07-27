/***************************************************************************
 *   Copyright (C) 2003 by Santiago Capel                                  *
 *   Santiago Capel Torres (GestiONG)                                      *
 *   Tomeu Borr� Riera                                                    *
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

#include <qfile.h>
#include <qstring.h>
#include <qxml.h>

#include "postgresiface2.h"

#define IMPORT_TODO 65535
// Tipos para BulmaCont
#define IMPORT_CUENTAS 1
#define IMPORT_TIPOSIVA 2
#define IMPORT_ASIENTOS 4
#define IMPORT_AINTELIGENTES 8
#define IMPORT_BALANCES 16
#define IMPORT_COBROS 32
#define IMPORT_FACTURAS 64
// Tipos para BulmaFact
#define IMPORT_CLIENTES 128
#define IMPORT_PROVEEDORES 256
#define IMPORT_ARTICULOS 512
#define IMPORT_FACTURASCLIENTE 1024
#define IMPORT_ALMACENES 2048
#define IMPORT_TRABAJADORES 4096
#define IMPORT_FORMAS_PAGO 8192
#define IMPORT_PRESUPUESTOSCLIENTE 16384
#define IMPORT_PEDIDOSCLIENTE  32768
#define IMPORT_ALBARANESCLIENTE 65536


/** @autor Tomeu Borr� Riera
  * @brief Clase para importaci� y exportaci� a distintos formatos de archivo de datos.
  */
class pgimportfiles {
private:
	/// Base de datos con la que trabaja la clase y de la que se hace importaci� / exportaci�
	postgresiface2 *conexionbase;
	/// Para que la clase pueda emitir el estado de completitud se inicializa con un puntero a funci�.
	virtual void alerta(int,int) {};
	/// Para que la clase pueda emitir mensajes de error o de alerta se inicializa con un puntero de funci�.
	virtual void mensajeria (QString) {};
	/// La clase puede hacer una simulaci� o no dependiendo del valor de esta variable
	bool m_modoTest;
	/// Las importaciones y exportaciones pueden ser entre dos fechas, m_fInicial indica la fecha inicial a partir de la que hacer la importaci�
	QString m_fInicial;
	/// Las importaciones y exportaciones pueden ser entre dos fechas, m_fFinal indica la fecha final a partir de la que hacer la importaci�
	QString m_fFinal;
public:
	void setFInicial(QString f) {m_fInicial = f;};
	void setFFinal(QString f) {m_fFinal = f;};
	void setModoTest() {m_modoTest=TRUE;};
	void setModoNormal() {m_modoTest=FALSE;};
	bool modoTest() {return m_modoTest == TRUE;};
	pgimportfiles(postgresiface2 *);
	virtual ~pgimportfiles(){};
	int contaplus2Bulmages(QFile &, QFile &);
	int bulmages2Contaplus(QFile &, QFile &);
	/// Esta funci� pasa datos de una empresa al formato XML.
	int bulmages2XML(QFile &, unsigned int tipo = IMPORT_TODO);
	int bulmafact2XML(QFile &, unsigned int tipo = IMPORT_TODO);
	/// Esta funci� pasa datos de XML a bulmag�.
	int XML2Bulmages(QFile &, unsigned int tip = IMPORT_TODO);
	QString searchParent(QString);
};



/** @autor Tomeu Borr� Riera
  * @class pgimportifles pgimportifles.h
  * @brief Clase para leer archivos de XML y hacer la importaci� de datos.
  */
class StructureParser : public QXmlDefaultHandler {
private:
	postgresiface2 *conexionbase;
	QString cadintermedia;		/// ESta variable va almacenando los valores que van saliendo en la clase.
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
	/// Variables usadas para almacenar los datos del registro de IVA
    	QString m_idRegistroIva;   
    	QString m_rIvaContrapartida;
    	QString m_rIvaBaseImp;	
    	QString m_rIvaIva;
    	QString m_rIvaFFactura;
    	QString m_rIvaFactura;
    	QString m_rIvaCIF;
    	QString m_rIvaIdFPago;
    	QString m_rIvRecRegIva;
	/// Variables usadas para almacenar los datos de la tabla de IVA's
	QString m_idTipoIva;
	QString m_baseIva;
	QString m_nombreTipoIva;
	
	/// El tagpadre indica en que posici� estamos. Si estamos en un asiento, un apunte, una cuenta, etc etc etc
	QString tagpadre;
public:
    StructureParser(postgresiface2 *, unsigned int tip=IMPORT_TODO);
    ~StructureParser();
    bool startDocument();
    bool startElement( const QString&, const QString&, const QString& ,
                       const QXmlAttributes& );
    bool endElement( const QString&, const QString&, const QString& );
    bool characters (const QString&);

private:
    QString indent;
unsigned int m_tipo;
};



/** @autor Tomeu Borr� Riera
  * @class pgimportifles pgimportifles.h
  * @brief Clase para leer archivos de XML y hacer la importaci� de datos.
  */
class ImportBulmaFact : public QXmlDefaultHandler {
private:
	postgresiface2 *conexionbase;
	QString cadintermedia;		/// ESta variable va almacenando los valores que van saliendo en la clase.
	/// Variables usadas para almacenar los datos de un asiento.
	QMap <QString, QString> valores;
	/// El tagpadre indica en que posici� estamos. Si estamos en un asiento, un apunte, una cuenta, etc etc etc
	QString tagpadre;
public:
    ImportBulmaFact(postgresiface2 *, unsigned int tip=IMPORT_TODO);
    ~ImportBulmaFact();
    bool startDocument();
    bool startElement( const QString&, const QString&, const QString& ,
                       const QXmlAttributes& );
    bool endElement( const QString&, const QString&, const QString& );
    bool characters (const QString&);

private:
    QString indent;
    unsigned int m_tipo;
};

#endif
