/***************************************************************************
 *   Copyright (C) 2003 by Santiago Capel                                  *
 *   Santiago Capel Torres (GestiONG)                                      *
 *   Tomeu Borrás Riera                                                    *
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


/** @autor Tomeu Borrás Riera
  * @class pgimportifles pgimportifles.h
  * @brief Clase para importación y exportación a distintos formatos
  */
class pgimportfiles {
private:
	/// Base de datos con la que trabaja la clase y de la que se hace importación / exportación
	postgresiface2 *conexionbase;
	/// Para que la clase pueda emitir el estado de completitud se inicializa con un puntero a función.
	void (*alerta)(int,int);
	/// Para que la clase pueda emitir mensajes de error o de alerta se inicializa con un puntero de función.
	void (*mensajeria) (QString);
	/// La clase puede hacer una simulación o no dependiendo del valor de esta variable
	bool m_modoTest;
	/// Las importaciones y exportaciones pueden ser entre dos fechas, m_fInicial indica la fecha inicial a partir de la que hacer la importación
	QString m_fInicial;
	/// Las importaciones y exportaciones pueden ser entre dos fechas, m_fFinal indica la fecha final a partir de la que hacer la importación
	QString m_fFinal;
public:
	void setFInicial(QString f) {m_fInicial = f;};
	void setFFinal(QString f) {m_fFinal = f;};
	void setModoTest() {m_modoTest=TRUE;};
	void setModoNormal() {m_modoTest=FALSE;};
	bool modoTest() {return m_modoTest == TRUE;};
	pgimportfiles(postgresiface2 *, void(*)(int,int), void(*) (QString));
	~pgimportfiles(){};
	int contaplus2Bulmages(QFile &, QFile &);
	int bulmages2Contaplus(QFile &, QFile &);
	/// Esta función pasa datos de una empresa al formato XML.
	int bulmages2XML(QFile &);
	/// Esta función pasa datos de XML a bulmagés.
	int XML2Bulmages(QFile &);
	QString searchParent(QString);
};




class StructureParser : public QXmlDefaultHandler {
private:
	void (*alerta)(int,int);
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
	
	/// El tagpadre indica en que posición estamos. Si estamos en un asiento, un apunte, una cuenta, etc etc etc
	QString tagpadre;
public:
    StructureParser(postgresiface2 *,void (*)(int,int));
    ~StructureParser();
    bool startDocument();
    bool startElement( const QString&, const QString&, const QString& ,
                       const QXmlAttributes& );
    bool endElement( const QString&, const QString&, const QString& );
    bool characters (const QString&);

private:
    QString indent;
};


#endif
