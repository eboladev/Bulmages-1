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
#include "bccompany.h"
#include "bcform.h"


/// Clase que hace de intermediaria entre la tabla de presupuesto de la base de datos y
/// el programa.
class RegistroIva : public BcForm
{
protected:
    BcSubForm *m_lineas;

public:
    void setLineas ( BcSubForm *l ) {
        m_lineas = l;
    };
    RegistroIva ( BcCompany *, QWidget * );
    QString idregistroiva() {
        return dbValue ( "idregistroiva" );
    };
    QString contrapartida() {
        return dbValue ( "contrapartida" );
    };
    QString baseimp() {
        return dbValue ( "baseimp" );
    };
    QString iva() {
        return dbValue ( "iva" );
    };
    QString ffactura() {
        return dbValue ( "ffactura" );
    };
    QString femisionregistroiva() {
        return dbValue ( "femisionregistroiva" );
    };
    QString factura() {
        return dbValue ( "factura" );
    };
    QString idborrador() {
        return dbValue ( "idborrador" );
    };
    QString incregistro() {
        return dbValue ( "incregistro" );
    };
    QString regularizacion() {
        return dbValue ( "regularizacion" );
    };
    QString plan349() {
        return dbValue ( "plan349" );
    };
    QString numorden() {
        return dbValue ( "numorden" );
    };
    QString cif() {
        return dbValue ( "cif" );
    };
    QString idfpago() {
        return dbValue ( "idfpago" );
    };
    QString factemitida() {
        return dbValue ( "factemitida" );
    };
    QString rectificaaregistroiva() {
        return dbValue ( "rectificaaregistroiva" );
    };
    QString serieregistroiva() {
        return dbValue ( "serieregistroiva" );
    };
    void setidregistroiva ( const QString &val ) {
        setDbValue ( "idregistroiva", val );
    };
    void setcontrapartida ( const QString &val ) {
        setDbValue ( "contrapartida", val );
    };
    void setbaseimp ( const QString &val ) {
        setDbValue ( "baseimp", val );
    };
    void setiva ( const QString &val ) {
        setDbValue ( "iva", val );
    };
    void setffactura ( const QString &val ) {
        setDbValue ( "ffactura", val );
    };
    void setfactura ( const QString &val ) {
        setDbValue ( "factura", val );
    };
    void setidborrador ( const QString &val ) {
        setDbValue ( "idborrador", val );
    };
    void setincregistro ( const QString &val ) {
        setDbValue ( "incregistro", val );
    };
    void setregularizacion ( const QString &val ) {
        setDbValue ( "regularizacion", val );
    };
    void setplan349 ( const QString &val ) {
        setDbValue ( "plan349", val );
    };
    void setnumorden ( const QString &val ) {
        setDbValue ( "numorden", val );
    };
    void setcif ( const QString &val ) {
        setDbValue ( "cif", val );
    };
    void setidfpago ( const QString &val ) {
        setDbValue ( "idfpago", val );
    };
    void setfactemitida ( const QString &val ) {
        setDbValue ( "factemitida", val );
    };
    void setrectificaaregistroiva ( const QString &val ) {
        setDbValue ( "rectificaaregistroiva", val );
    };
    void setfemisionregistroiva ( const QString &val ) {
        setDbValue ( "femisionregistroiva", val );
    };
    void setserieregistroiva ( const QString &val ) {
        setDbValue ( "serieregistroiva", val );
    };
    virtual void pintaidregistroiva ( const QString & ) {
        
	BlDebug::blDebug ( Q_FUNC_INFO, 0, _("Funcion no implentada.") );
    };
    virtual void pintacontrapartida ( const QString & ) {
	BlDebug::blDebug ( Q_FUNC_INFO, 0, _("Funcion no implentada.") );
    };
    virtual void pintabaseimp ( const QString & ) {
	BlDebug::blDebug ( Q_FUNC_INFO, 0, _("Funcion no implentada.") );
    };
    virtual void pintaiva ( const QString & ) {
	BlDebug::blDebug ( Q_FUNC_INFO, 0, _("Funcion no implentada.") );
    };
    virtual void pintaffactura ( const QString & ) {
	BlDebug::blDebug ( Q_FUNC_INFO, 0, _("Funcion no implentada.") );
    };
    virtual void pintafactura ( const QString & ) {
	BlDebug::blDebug ( Q_FUNC_INFO, 0, _("Funcion no implentada.") );
    };
    virtual void pintaidborrador ( const QString & ) {
	BlDebug::blDebug ( Q_FUNC_INFO, 0, _("Funcion no implentada.") );
    };
    virtual void pintaincregistro ( const QString & ) {
	BlDebug::blDebug ( Q_FUNC_INFO, 0, _("Funcion no implentada.") );
    };
    virtual void pintaregularizacion ( const QString & ) {
	BlDebug::blDebug ( Q_FUNC_INFO, 0, _("Funcion no implentada.") );
    };
    virtual void pintaplan349 ( const QString & ) {
	BlDebug::blDebug ( Q_FUNC_INFO, 0, _("Funcion no implentada.") );
    };
    virtual void pintanumorden ( const QString & ) {
	BlDebug::blDebug ( Q_FUNC_INFO, 0, _("Funcion no implentada.") );
    };
    virtual void pintacif ( const QString & ) {
	BlDebug::blDebug ( Q_FUNC_INFO, 0, _("Funcion no implentada.") );
    };
    virtual void pintaidfpago ( const QString & ) {
	BlDebug::blDebug ( Q_FUNC_INFO, 0, _("Funcion no implentada.") );
    };
    virtual void pintafactemitida ( const QString & ) {
	BlDebug::blDebug ( Q_FUNC_INFO, 0, _("Funcion no implentada.") );
    };
    virtual void pintarectificaaregistroiva ( const QString & ) {
	BlDebug::blDebug ( Q_FUNC_INFO, 0, _("Funcion no implentada.") );
    };
    virtual void pintafemisionregistroiva ( const QString & ) {
	BlDebug::blDebug ( Q_FUNC_INFO, 0, _("Funcion no implentada.") );
    };
    virtual void pintaserieregistroiva ( const QString & ) {
	BlDebug::blDebug ( Q_FUNC_INFO, 0, _("Funcion no implentada.") );
    };
    virtual ~RegistroIva();
    virtual int load ( QString );
    virtual void pintaRegistroIva();
    virtual int save();
    virtual int remove();
    virtual void calculaPintaTotales() {}
    ;
    virtual void recalculaIva() {
	BlDebug::blDebug ( Q_FUNC_INFO, 0, _("Funcion no implentada.") );
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
