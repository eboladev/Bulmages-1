/***************************************************************************
 *   Copyright (C) 2005 by Tomeu Borras Riera                              *
 *   tborras@conetxia.com                                                  *
 *   Copyright (C) 2005 by Alvaro de Miguel                                *
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
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/

#ifndef REGISTROIVA_H
#define REGISTROIVA_H

#include <QString>

#include "blfixed.h"
#include "listiva.h"
#include "listlinprevcobro.h"
#include "empresa.h"
#include "fichabc.h"


/// Clase que hace de intermediaria entre la tabla de presupuesto de la base de datos y
/// el programa.
class RegistroIva : public FichaBc
{
protected:
    SubForm2Bc *m_lineas;

public:
    void setLineas ( SubForm2Bc *l ) {
        m_lineas = l;
    };
    RegistroIva ( Empresa *, QWidget * );
    QString idregistroiva() {
        return DBvalue ( "idregistroiva" );
    };
    QString contrapartida() {
        return DBvalue ( "contrapartida" );
    };
    QString baseimp() {
        return DBvalue ( "baseimp" );
    };
    QString iva() {
        return DBvalue ( "iva" );
    };
    QString ffactura() {
        return DBvalue ( "ffactura" );
    };
    QString femisionregistroiva() {
        return DBvalue ( "femisionregistroiva" );
    };
    QString factura() {
        return DBvalue ( "factura" );
    };
    QString idborrador() {
        return DBvalue ( "idborrador" );
    };
    QString incregistro() {
        return DBvalue ( "incregistro" );
    };
    QString regularizacion() {
        return DBvalue ( "regularizacion" );
    };
    QString plan349() {
        return DBvalue ( "plan349" );
    };
    QString numorden() {
        return DBvalue ( "numorden" );
    };
    QString cif() {
        return DBvalue ( "cif" );
    };
    QString idfpago() {
        return DBvalue ( "idfpago" );
    };
    QString factemitida() {
        return DBvalue ( "factemitida" );
    };
    QString rectificaaregistroiva() {
        return DBvalue ( "rectificaaregistroiva" );
    };
    QString serieregistroiva() {
        return DBvalue ( "serieregistroiva" );
    };
    void setidregistroiva ( const QString &val ) {
        setDBvalue ( "idregistroiva", val );
    };
    void setcontrapartida ( const QString &val ) {
        setDBvalue ( "contrapartida", val );
    };
    void setbaseimp ( const QString &val ) {
        setDBvalue ( "baseimp", val );
    };
    void setiva ( const QString &val ) {
        setDBvalue ( "iva", val );
    };
    void setffactura ( const QString &val ) {
        setDBvalue ( "ffactura", val );
    };
    void setfactura ( const QString &val ) {
        setDBvalue ( "factura", val );
    };
    void setidborrador ( const QString &val ) {
        setDBvalue ( "idborrador", val );
    };
    void setincregistro ( const QString &val ) {
        setDBvalue ( "incregistro", val );
    };
    void setregularizacion ( const QString &val ) {
        setDBvalue ( "regularizacion", val );
    };
    void setplan349 ( const QString &val ) {
        setDBvalue ( "plan349", val );
    };
    void setnumorden ( const QString &val ) {
        setDBvalue ( "numorden", val );
    };
    void setcif ( const QString &val ) {
        setDBvalue ( "cif", val );
    };
    void setidfpago ( const QString &val ) {
        setDBvalue ( "idfpago", val );
    };
    void setfactemitida ( const QString &val ) {
        setDBvalue ( "factemitida", val );
    };
    void setrectificaaregistroiva ( const QString &val ) {
        setDBvalue ( "rectificaaregistroiva", val );
    };
    void setfemisionregistroiva ( const QString &val ) {
        setDBvalue ( "femisionregistroiva", val );
    };
    void setserieregistroiva ( const QString &val ) {
        setDBvalue ( "serieregistroiva", val );
    };
    virtual void pintaidregistroiva ( const QString & ) {
        _depura ( "funcion no implementada", 2 );
    };
    virtual void pintacontrapartida ( const QString & ) {
        _depura ( "funcion no implementada", 2 );
    };
    virtual void pintabaseimp ( const QString & ) {
        _depura ( "funcion no implementada", 2 );
    };
    virtual void pintaiva ( const QString & ) {
        _depura ( "funcion no implementada", 2 );
    };
    virtual void pintaffactura ( const QString & ) {
        _depura ( "funcion no implementada", 2 );
    };
    virtual void pintafactura ( const QString & ) {
        _depura ( "funcion no implementada", 2 );
    };
    virtual void pintaidborrador ( const QString & ) {
        _depura ( "funcion no implementada", 2 );
    };
    virtual void pintaincregistro ( const QString & ) {
        _depura ( "funcion no implementada", 2 );
    };
    virtual void pintaregularizacion ( const QString & ) {
        _depura ( "funcion no implementada", 2 );
    };
    virtual void pintaplan349 ( const QString & ) {
        _depura ( "funcion no implementada", 2 );
    };
    virtual void pintanumorden ( const QString & ) {
        _depura ( "funcion no implementada", 2 );
    };
    virtual void pintacif ( const QString & ) {
        _depura ( "funcion no implementada", 2 );
    };
    virtual void pintaidfpago ( const QString & ) {
        _depura ( "funcion no implementada", 2 );
    };
    virtual void pintafactemitida ( const QString & ) {
        _depura ( "funcion no implementada", 2 );
    };
    virtual void pintarectificaaregistroiva ( const QString & ) {
        _depura ( "funcion no implementada", 2 );
    };
    virtual void pintafemisionregistroiva ( const QString & ) {
        _depura ( "funcion no implementada", 2 );
    };
    virtual void pintaserieregistroiva ( const QString & ) {
        _depura ( "funcion no implementada", 2 );
    };
    virtual ~RegistroIva();
    virtual int cargar ( QString );
    virtual void pintaRegistroIva();
    virtual int guardar();
    virtual int borrar();
    virtual void calculaypintatotales() {}
    ;
    virtual void recalculaIva() {
        _depura ( "no implementada", 2 );
    };
    void vaciaRegistroIva();
    int buscaborradorservicio ( int );
    int buscaborradorcliente ( int );
    int buscaborradoriva ( int );
    void inicializa1 ( int );
    void buscafecha ( int );
    void buscaNumFactura ( int );
};

#endif

